#include "view_mysql_mdbcfg.h"
#include "ssp_gui.h"

view_mysql_mdbcfg::view_mysql_mdbcfg(QWidget *parent)
	: CBaseView(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton_ConnDB,SIGNAL(clicked()),this,SLOT(OnConnDB()));
	connect(ui.pushButton_NewCfg,SIGNAL(clicked()),this,SLOT(OnNewCfg()));
	connect(ui.pushButton_LoadCfg,SIGNAL(clicked()),this,SLOT(OnLoadCfg()));
	connect(ui.pushButton_SaveCfg,SIGNAL(clicked()),this,SLOT(OnSaveCfg()));
	connect(ui.pushButton_ExportStruct,SIGNAL(clicked()),this,SLOT(OnExportStruct()));
	connect(ui.pushButton_ExportSQL,SIGNAL(clicked()),this,SLOT(OnExportSQL()));

	connect(ui.pushButton_ImportData_SDB  ,SIGNAL(clicked()),this,SLOT(OnImportData_SDB()));
	connect(ui.pushButton_ExportData_SDB  ,SIGNAL(clicked()),this,SLOT(OnExportData_SDB()));
	connect(ui.pushButton_ExportSQL_MYSQL ,SIGNAL(clicked()),this,SLOT(OnExportSQL_MYSQL()));
	connect(ui.pushButton_ExportSQL_ORACLE,SIGNAL(clicked()),this,SLOT(OnExportSQL_ORACLE()));
	connect(ui.treeWidget,SIGNAL(itemChanged (QTreeWidgetItem*,int)),this,SLOT(slotClickTreeChkbox(QTreeWidgetItem*,int)));

	QSettings settings("SKT", "SSP_ASSIST");
	ui.lineEdit_IP->setText(settings.value("mysql_host").toString());
	ui.lineEdit_Port->setText(settings.value("mysql_port").toString());
	ui.lineEdit_DbName->setText(settings.value("mysql_dbname").toString());
	ui.lineEdit_User->setText(settings.value("mysql_user").toString());
	ui.lineEdit_Password->setText(settings.value("mysql_password").toString());
	m_DbRestore.m_pDb = &m_mysql;

}

view_mysql_mdbcfg::~view_mysql_mdbcfg()
{

}

void view_mysql_mdbcfg::OnConnDB()
{
	SString sParams;
	sParams.sprintf("hostaddr=%s;port=%s;dbname=%s;user=%s;password=%s;",
		ui.lineEdit_IP->text().toStdString().data(),
		ui.lineEdit_Port->text().toStdString().data(),
		ui.lineEdit_DbName->text().toStdString().data(),
		ui.lineEdit_User->text().toStdString().data(),
		ui.lineEdit_Password->text().toStdString().data());
	m_mysql.SetParams(sParams);
	QSettings settings("SKT", "SSP_ASSIST");
	settings.setValue("mysql_host", ui.lineEdit_IP->text());
	settings.setValue("mysql_port", ui.lineEdit_Port->text());
	settings.setValue("mysql_dbname", ui.lineEdit_DbName->text());
	settings.setValue("mysql_user", ui.lineEdit_User->text());
	settings.setValue("mysql_password", ui.lineEdit_Password->text());
	if(m_mysql.GetStatus() == SDatabase::UNCONNECT || m_mysql.GetStatus() == SDatabase::DBERROR)
	{
		//��δ����
		if(!m_mysql.Connect())
		{
			SQt::ShowErrorBox("����","����MySQL���ݿ�ʱʧ��!");
		}
		else
		{
			ui.pushButton_ConnDB->setText("�Ͽ�(&D)");
			SString sFile = ui.lineEdit_CfgFile->text().toStdString().data();
			if(sFile.length() != 0)
			{
				m_XmlMdbCfg.clear();
				m_XmlMdbCfg.ReadConfig(sFile);
			}
			OnLoadTables();
		}
	}
	else
	{
		//�Ͽ�����
		m_mysql.DisConnect();
		ui.pushButton_ConnDB->setText("����(&C)");
	}
}

void view_mysql_mdbcfg::OnNewCfg()
{
	QString fileName = QFileDialog::getSaveFileName(this,tr("MDB�ڴ�������ļ�"), "" , tr("XML�ļ� (*.xml)"));
	if(fileName.length() == 0)
		return;
	SString sFile = fileName.toStdString().data();
	if(sFile.right(4).toLower() != ".xml")
	{
		sFile += ".xml";
	}
	ui.lineEdit_CfgFile->setText(sFile.data());
	m_XmlMdbCfg.clear();
}

void view_mysql_mdbcfg::OnLoadCfg()
{
	QString fileName = QFileDialog::getOpenFileName(this,tr("MDB�ڴ�������ļ�"), "", tr("XML�ļ� (*.xml)"));
	if(fileName.length() == 0)
		return;
	SString sFile = fileName.toStdString().data();
	m_XmlMdbCfg.clear();
	if(!m_XmlMdbCfg.ReadConfig(sFile))
	{
		SQt::ShowInfoBox(tr("��ʾ"),tr("�����ļ���ʧ��!"));
		return;
	}
	ui.lineEdit_CfgFile->setText(sFile.data());
	OnLoadTables();
}

SString view_mysql_mdbcfg::MySqlType2Mdb(SString sType)
{
	//decimal(12,0)
	//numeric
	sType = sType.toLower();
	if(sType.left(8) == "decimal(" || sType.left(8) == "numeric(")
	{
		SString str = sType.mid(8);
		if(str.length() > 0)
			str = str.left(str.length()-1);
		int pn = SString::GetIdAttributeI(1,str,",");
		int pm = SString::GetIdAttributeI(2,str,",");
		if(pm > 0)
		{
			//��С����ͳһӳ��Ϊ������
			return "float";
		}
		else if(pn <= 3)
			return "byte";
		else if(pn <=	5)
			return "short";
		else if(pn <= 12)
			return "int";
		else
			return "int64";
	}
	else if(sType.left(4) == "int(")
	{
// 		int pn = sType.mid(4).toInt();
// 		if(pn <= 3)
// 			return "byte";
// 		else if(pn <=	5)
// 			return "short";
// 		else if(pn <= 11)
			return "int";
// 		else
// 			return "int64";
	}
	else if(sType.left(7) == "bigint(")
	{
		return "int64";
	}
	else if(sType.left(8) == "tinyint(")
	{
		return "byte";
	}
	else if(sType == "float")
	{
		return "float";
	}
	else if(sType.left(4) == "blob" || sType.left(8) == "longblob" || sType.left(9) == "mediumblob" || sType.left(9) == "shortblob" || sType == "mediumtext" || sType == "text" || sType == "longtext")
		return "blob";
	else if(sType.left(5) == "char(")
		return sType;
	else if(sType.left(8) == "varchar(")
		return sType.mid(3);
	else if(sType == "datetime" || sType == "timestamp")
	{
		return "int";
	}
	else
		return "";
}

SString view_mysql_mdbcfg::MySqlType2Oracle(SString sType)
{
	//decimal(12,0)
	//numeric
	sType = sType.toLower();
	if(sType.left(8) == "decimal(" || sType.left(8) == "numeric(")
	{
		return sType;
	}
	else if(sType.left(4) == "int(")
	{
// 		int pn = sType.mid(4).toInt();
// 		if(pn <= 3)
// 			return "number(3,0)";
// 		else if(pn <=	5)
// 			return "number(5,0)";
// 		else if(pn <= 11)
			return "number(11,0)";
// 		else
// 			return "integer";
	}
	else if(sType.left(7) == "bigint(")
	{
		return "integer";
	}
	else if(sType.left(8) == "tinyint(")
	{
		return "number(3,0)";
	}
	else if(sType == "float")
	{
		return "float";
	}
	else if(sType.left(4) == "blob" || sType.left(8) == "longblob" || sType.left(9) == "mediumblob" || sType.left(9) == "shortblob" || sType == "mediumtext" || sType == "text" || sType == "longtext")
		return "blob";
	else if(sType.left(5) == "char(")
		return "varchar2"+sType.mid(4);
	else if(sType.left(8) == "varchar(")
		return "varchar2"+sType.mid(7);
	else if(sType == "datetime" || sType == "timestamp")
	{
		return sType;
	}
	else
		return "";
}

SString view_mysql_mdbcfg::MysqlKey2Mdb(SString sKey)
{
	if(sKey == "PRI")
		return "pk";
	else if(sKey == "MUL")
		return "idx";
	else
		return "";
}

void view_mysql_mdbcfg::OnLoadTables()
{
	if(m_mysql.GetConnectHandle() == NULL)
	{
		OnConnDB();
	}
	if(m_mysql.GetConnectHandle() == NULL)
	{
		return;
	}
	SString sql;
	SRecordset rsTables;
	ui.treeWidget->clear();
	
	QStringList head;
	head<<tr("��/�ֶ�")<<tr("����")<<tr("�����")<<tr("��")<<tr("ȱʡֵ")<<tr("��չ����");
	ui.treeWidget->clear();
	ui.treeWidget->setColumnCount(head.size());
	ui.treeWidget->setHeaderLabels(head);
	ui.treeWidget->setColumnWidth(0,300);
	ui.treeWidget->setColumnWidth(1,120);
	ui.treeWidget->setColumnWidth(2,60);
	ui.treeWidget->setColumnWidth(3,70);
	ui.treeWidget->setColumnWidth(4,60);
	ui.treeWidget->setColumnWidth(5,80);

	m_mysql.Retrieve("show tables",rsTables);
	int i,cnt = rsTables.GetRows();
	if(cnt == 0)
		return;
	SRecordset rsViews;
	m_mysql.Retrieve(SString::toFormat("SELECT table_name FROM information_schema.views where table_schema='%s'",
		ui.lineEdit_DbName->text().toStdString().data()),rsViews);

	SSP_GUI->SetStatusTip(SString::toFormat("��%d�����ݱ�",cnt));
	ui.pushButton_ConnDB->setEnabled(false);
	ui.pushButton_LoadCfg->setEnabled(false);
	ui.pushButton_SaveCfg->setEnabled(false);
	ui.pushButton_NewCfg->setEnabled(false);
	ui.pushButton_ExportStruct->setEnabled(false);
	ui.pushButton_ExportSQL->setEnabled(false);
	ui.pushButton_ImportData_SDB->setEnabled(false);
	ui.pushButton_ExportData_SDB->setEnabled(false);
	ui.pushButton_ExportSQL_MYSQL->setEnabled(false);
	ui.pushButton_ExportSQL_ORACLE->setEnabled(false);
	SSP_GUI->ShowWaiting();
	QStringList sl_name;
	sl_name<<tr("���б�");
	QTreeWidgetItem* all_table = new QTreeWidgetItem(ui.treeWidget,sl_name);
	all_table->setCheckState(0,Qt::Unchecked);
	all_table->setData(0,Qt::UserRole,0);
	ui.treeWidget->addTopLevelItem(all_table);
	ui.treeWidget->expandAll();
	SBaseConfig *pTabNode,*pFldNode;
	SString tab_name;
	int j,cnt2;
	int iExpectViews = 0;
	for(i=0;i<cnt;i++)
	{
		tab_name = rsTables.GetValue(i,0).data();
		//�������ͼ�����
		bool bView = false;
		for(j=0;j<rsViews.GetRows();j++)
		{
			if(tab_name == rsViews.GetValue(j,0))
			{
				bView = true;
				iExpectViews++;
				break;
			}
		}
		if(bView)
			continue;
		sl_name.clear();
		sl_name<<tab_name.data();
		QTreeWidgetItem* table = new QTreeWidgetItem(all_table,sl_name);
		pTabNode = m_XmlMdbCfg.SearchChild(SString::toFormat("table<name='%s'>",rsTables.GetValue(i,0).data()));
		table->setCheckState(0,pTabNode==NULL?(Qt::Unchecked):(Qt::Checked));
		/*
		field,type,null,key,default,extra
		ied_no	decimal(12,0)	NO	PRI	0	
		cpu_no	decimal(3,0)	NO	PRI	0	
		name	varchar(16)	NO			
		version	varchar(32)	YES			
		crc	varchar(16)	YES			
		version_time	varchar(16)	YES			
		mms_path	varchar(32)	YES			

		*/
		sql.sprintf("SHOW columns from %s",rsTables.GetValue(i,0).data());
		SRecordset rsFields;
		m_mysql.Retrieve(sql,rsFields);
		cnt2 = rsFields.GetRows();
		for(j=0;j<cnt2;j++)
		{
			SString sFieldName	= rsFields.GetValue(j,0);
			SString sFieldType	= rsFields.GetValue(j,1);
			SString sIsNull		= rsFields.GetValue(j,2);
			SString sKey		= rsFields.GetValue(j,3);
			SString sDefault	= rsFields.GetValue(j,4);
			SString sExtra		= rsFields.GetValue(j,5);
			sl_name.clear();
			sl_name<<sFieldName.data()<<sFieldType.data()<<sIsNull.data()<<sKey.data()<<sDefault.data()<<sExtra.data();
			QTreeWidgetItem* field = new QTreeWidgetItem(table,sl_name);
			if(MySqlType2Mdb(sFieldType).length() == 0)
			{
				table->setBackground(1,QBrush(QColor(255,0,0)));
				field->setForeground(1,QBrush(QColor(255,0,0)));
			}
			if(pTabNode == NULL)
				pFldNode = NULL;
			else
				pFldNode = pTabNode->SearchChild(SString::toFormat("field<name='%s'>",sFieldName.data()));
			field->setCheckState(0,pFldNode==NULL?(Qt::Unchecked):(Qt::Checked));
		}
		ui.treeWidget->collapseItem(table);
		SSP_GUI->SetStatusTip(SString::toFormat("�Ѽ���[%d/%d]�����ݱ�...",i+1,cnt));
		SQt::ProcessUiEvents();
	}
	SSP_GUI->HideWaiting();
	SSP_GUI->SetStatusTip(SString::toFormat("��%d�����ݱ���Ϣ�������%s!",cnt-iExpectViews,iExpectViews==0?"": (SString::toFormat("(%d����ͼ���ų�)",iExpectViews).data()) ));
	ui.pushButton_ConnDB->setEnabled(true);
	ui.pushButton_LoadCfg->setEnabled(true);
	ui.pushButton_SaveCfg->setEnabled(true);
	ui.pushButton_NewCfg->setEnabled(true);
	ui.pushButton_ExportStruct->setEnabled(true);
	ui.pushButton_ExportSQL->setEnabled(true);
	ui.pushButton_ImportData_SDB->setEnabled(true);
	ui.pushButton_ExportData_SDB->setEnabled(true);
	ui.pushButton_ExportSQL_MYSQL->setEnabled(true);
	ui.pushButton_ExportSQL_ORACLE->setEnabled(true);
}


void view_mysql_mdbcfg::slotClickTreeChkbox(QTreeWidgetItem* item ,int col)
{
	Qt::CheckState flag = item->checkState(col);//Qt::Unchecked;
	if( item->childCount()  == 0)
	{
		return;
	}
	for( int i=0;i<item->childCount();i++)
	{
		QTreeWidgetItem* ptmp = item->child(i);
		ptmp->setCheckState(0,flag);
	}
}

void view_mysql_mdbcfg::OnSaveCfg()
{
	QTreeWidgetItem *pRoot = ui.treeWidget->topLevelItem(0);
	if(pRoot == NULL)
		return;
	SString sFile = ui.lineEdit_CfgFile->text().toStdString().data();
	if(sFile.length() == 0)
	{
		SQt::ShowErrorBox(tr("����"),tr("��δ�½������MDB�ڴ�������ļ�!�޷����б���!"));
		return;
	}
	m_XmlMdbCfg.clear();
	m_XmlMdbCfg.SetNodeName("mdb-define");
	int iTable,iTables = pRoot->childCount();
	for(iTable=0;iTable<iTables;iTable++)
	{
		QTreeWidgetItem *pTable = pRoot->child(iTable);
		if(pTable == NULL || pTable->checkState(0) == Qt::Unchecked)
			continue;
		SBaseConfig *pTabNode = m_XmlMdbCfg.AddChildNode("table");
		pTabNode->SetAttribute("name",pTable->text(0).toStdString().data());
		pTabNode->SetAttribute("sync_record","true");
		int iField,iFields = pTable->childCount();
		for(iField=0;iField<iFields;iField++)
		{
			QTreeWidgetItem *pField = pTable->child(iField);
			if(pField == NULL || pField->checkState(0) == Qt::Unchecked)
				continue;
			SBaseConfig *pFldNode = pTabNode->AddChildNode("field");
			pFldNode->SetAttribute("name",pField->text(0).toStdString().data());
			pFldNode->SetAttribute("type",MySqlType2Mdb(pField->text(1).toStdString().data()));
			pFldNode->SetAttribute("isnull",pField->text(2).toStdString().data());
			pFldNode->SetAttribute("key",MysqlKey2Mdb(pField->text(3).toStdString().data()));
			pFldNode->SetAttribute("default",pField->text(4).toStdString().data());
		}
	}
	if(!m_XmlMdbCfg.SaveConfig(sFile))
	{
		SQt::ShowErrorBox(tr("����"),tr("�����ļ�ʱʧ��!"));
		return;
	}
	else
	{
		SQt::ShowInfoBox(tr("��ʾ"),tr("�ļ�����ɹ�!"));
	}
}

void view_mysql_mdbcfg::OnExportStruct()
{
	QTreeWidgetItem *pRoot = ui.treeWidget->topLevelItem(0);
	if(pRoot == NULL)
		return;
	QString fileName = QFileDialog::getSaveFileName(this,tr("MDB�ڴ��ṹ�����ļ�"), "" , tr("C Header files (*.h)"));
	if(fileName.length() == 0)
		return;
	SString sFile = fileName.toStdString().data();
	if(sFile.right(2).toLower() != ".h")
		sFile += ".h";
	SFile file(sFile);
	if(!file.open(IO_Truncate))
	{
		SQt::ShowErrorBox(tr("����"),tr("�����ļ�ʱʧ��!"));
		return;
	}
	SString sShortName = sFile;
	if(sShortName.findRev("\\") > 0)
		sShortName = sShortName.mid(sShortName.findRev("\\")+1);
	if(sShortName.findRev("/") > 0)
		sShortName = sShortName.mid(sShortName.findRev("/")+1);
	
	SDateTime now = SDateTime::currentDateTime();
	file.fprintf("/**\r\n"
		" *\r\n"
		" * �� �� �� : %s\r\n"
		" * �������� : %s\r\n"
		" * ��    �� : SSP_ASSIST(skt001@163.com)\r\n"
		" * �޸����� : $Date: $\r\n"
		" * ��ǰ�汾 : $Revision: $\r\n"
		" * �������� : \r\n"
		" * �޸ļ�¼ : \r\n"
		" *            $Log: $\r\n"
		" *\r\n"
		" * Ver  Date        Author       Comments\r\n"
		" * ---  ----------  -----------  -------------------------------------------\r\n"
		" * 001	%s  SSP_ASSIST  ����SSP_ASSIST�Զ������ļ�\r\n"
		" *\r\n"
		" **/\r\n\r\n",
		sShortName.data(),
		now.toString("yyyy-MM-dd hh:mm").data(),
		now.toString("yyyy-MM-dd").data()
		);
	SString sMacro = sShortName;
	sMacro.replace(".","_");
	sMacro = sMacro.toUpper();
	file.fprintf("#ifndef __MDB_STRUCT_DEF_%s__\r\n"
		"#define __MDB_STRUCT_DEF_%s__\r\n\r\n"
		"#pragma pack(push, Old, 1)\r\n"
		"\r\n",
		sMacro.data(),sMacro.data());
	int iTable,iTables = pRoot->childCount();
	for(iTable=0;iTable<iTables;iTable++)
	{
		QTreeWidgetItem *pTable = pRoot->child(iTable);
		if(pTable == NULL || pTable->checkState(0) == Qt::Unchecked)
			continue;
		file.fprintf("struct %s\r\n"
			"{\r\n",
			pTable->text(0).toStdString().data());
		int iField,iFields = pTable->childCount();
		for(iField=0;iField<iFields;iField++)
		{
			QTreeWidgetItem *pField = pTable->child(iField);
			if(pField == NULL || pField->checkState(0) == Qt::Unchecked)
				continue;
			SString name = pField->text(0).toStdString().data();
			SString sType = MySqlType2Mdb(pField->text(1).toStdString().data());

			SString sLine;
			if(sType.left(5) == "char(")
			{
				sLine.sprintf("char          %s[%d]",name.data(),sType.mid(5).toInt());
			}
			else
			{
				if(sType == "byte")
					sLine = "unsigned char ";
				else if(sType == "short")
					sLine = "short         ";
				else if(sType == "int")
					sLine = "int           ";
				else if(sType == "int64")
					sLine = "INT64         ";
				else if(sType == "float")
					sLine = "float         ";
				else if(sType == "blob")
					sLine = "unsigned int  ";
				else 
					sLine = "//unknown     ";
				sLine += name;
			}
			file.fprintf("\t%s;\r\n",sLine.data());
		}
		file.fprintf("};\r\n\r\n");
	}
	file.fprintf("#pragma pack(pop, Old)\r\n\r\n"
				"#endif//__MDB_STRUCT_DEF_%s__\r\n",
		sMacro.data(),sMacro.data());
	file.close();
	SQt::ShowInfoBox(tr("��ʾ"),tr("�ļ�����ɹ�!"));
}


void view_mysql_mdbcfg::OnExportSQL()
{
	QTreeWidgetItem *pRoot = ui.treeWidget->topLevelItem(0);
	if(pRoot == NULL)
		return;
	QString fileName = QFileDialog::getSaveFileName(this,tr("MDB�ڴ�⽨��SQL�ļ�"), "" , tr("SQL files (*.sql)"));
	if(fileName.length() == 0)
		return;
	SString sFile = fileName.toStdString().data();
	if(sFile.right(4).toLower() != ".sql")
		sFile += ".sql";
	SFile file(sFile);
	if(!file.open(IO_Truncate))
	{
		SQt::ShowErrorBox(tr("����"),tr("�����ļ�ʱʧ��!"));
		return;
	}
	SString sShortName = sFile;
	if(sShortName.findRev("\\") > 0)
		sShortName = sShortName.mid(sShortName.findRev("\\")+1);
	if(sShortName.findRev("/") > 0)
		sShortName = sShortName.mid(sShortName.findRev("/")+1);
	
	SDateTime now = SDateTime::currentDateTime();
	file.fprintf("/*########################################################################\r\n"
				"#*\r\n"
				"#* �� �� �� : %s\r\n"
				"#* �������� : %s\r\n"
				"#* ��    �� : SSP_ASSIST(skt001@163.com)\r\n"
				"#* �޸����� : $Date:$\r\n"
				"#* ��ǰ�汾 : $Revision: 001$\r\n"
				"#* �������� : MDB�﷨�����ݿ�ṹSQL�ű�\r\n"
				"#* �޸ļ�¼ : \r\n"
				"#*            $Log: $\r\n"
				"#*\r\n"
				"#* Ver  Date        Author      Comments\r\n"
				"#* ---  ----------  ----------  -------------------------------------------\r\n"
				"#* 001	%s  SSP_ASSIST  ��SSP01ƽ̨��SSP_ASSIS�Զ�����\r\n"
				"#*\r\n"
				"########################################################################*/\r\n"
				"\r\n\r\n",
		sShortName.data(),
		now.toString("yyyy-MM-dd hh:mm").data(),
		now.toString("yyyy-MM-dd").data()
		);
	
	int iTable,iTables = pRoot->childCount();
	SString sLine;
	for(iTable=0;iTable<iTables;iTable++)
	{
		QTreeWidgetItem *pTable = pRoot->child(iTable);
		if(pTable == NULL || pTable->checkState(0) == Qt::Unchecked)
			continue;
		SString tablename = pTable->text(0).toStdString().data();
		sLine.sprintf("drop table %s;\r\n"
			"create table %s\r\n"
			"(\r\n",
			tablename.data(),tablename.data());
		int iField,iFields = pTable->childCount();
		SString sKeyName;
		for(iField=0;iField<iFields;iField++)
		{
			QTreeWidgetItem *pField = pTable->child(iField);
			if(pField == NULL || pField->checkState(0) == Qt::Unchecked)
				continue;
			SString name = pField->text(0).toStdString().data();
			SString sType = MySqlType2Mdb(pField->text(1).toStdString().data());
			SString sIsNull = pField->text(2).toStdString().data();
			SString sKey = pField->text(3).toStdString().data();
			sLine += SString::toFormat("  %s %s%s,\r\n",name.data(),sType.data(),sIsNull=="NO"?" not null":"");
			if(sKey == "PRI")
			{
				if(sKeyName.length() > 0)
					sKeyName += ",";
				sKeyName += name;
			}
		}

		//���û�����������ˣ����Զ�������������ֹ���ɵ�����������Ҫ��
		if(sKeyName.length() == 0)
		{
			//�����һ���ֶεĶ��Ÿ�Ϊ�ո�
			(sLine.data())[sLine.length()-3] = ' ';
		}
		else
		{
	// 		//��������,���û����ȷָ���������������ֶ�һ����Ϊ����
	// 		if(sKeyName.length() == 0)
	// 		{
	// 			for(iField=0;iField<iFields;iField++)
	// 			{
	// 				QTreeWidgetItem *pField = pTable->child(iField);
	// 				if(pField == NULL || pField->checkState(0) == Qt::Unchecked)
	// 					continue;
	// 				SString name = pField->text(0).toStdString().data();
	// 				if(sKeyName.length() > 0)
	// 					sKeyName += ",";
	// 				sKeyName += name;
	// 			}
	// 		}
			sLine += SString::toFormat("  constraint pk_%s primary key(%s)\r\n",tablename.data(),sKeyName.data());
		}
		sLine += SString::toFormat(");\r\n\r\n");
		file.writeBlock((BYTE*)sLine.data(),sLine.length());
		sLine = "";
	}
	file.close();
	SQt::ShowInfoBox(tr("��ʾ"),tr("�ļ�����ɹ�!"));
}

void view_mysql_mdbcfg::OnExportSQL_MYSQL()
{
	QTreeWidgetItem *pRoot = ui.treeWidget->topLevelItem(0);
	if(pRoot == NULL)
		return;
	QString fileName = QFileDialog::getSaveFileName(this,tr("MySQL���ݿ⽨��SQL�ļ�"), "" , tr("SQL files (*.sql)"));
	if(fileName.length() == 0)
		return;
	SString sFile = fileName.toStdString().data();
	if(sFile.right(4).toLower() != ".sql")
		sFile += ".sql";
	SFile file(sFile);
	if(!file.open(IO_Truncate))
	{
		SQt::ShowErrorBox(tr("����"),tr("�����ļ�ʱʧ��!"));
		return;
	}
	SString sShortName = sFile;
	if(sShortName.findRev("\\") > 0)
		sShortName = sShortName.mid(sShortName.findRev("\\")+1);
	if(sShortName.findRev("/") > 0)
		sShortName = sShortName.mid(sShortName.findRev("/")+1);
	
	SDateTime now = SDateTime::currentDateTime();
	file.fprintf("-- ########################################################################\r\n"
				"-- #*\r\n"
				"-- #* �� �� �� : %s\r\n"
				"-- #* �������� : %s\r\n"
				"-- #* ��    �� : SSP_ASSIST(skt001@163.com)\r\n"
				"-- #* �޸����� : $Date:$\r\n"
				"-- #* ��ǰ�汾 : $Revision: 001$\r\n"
				"-- #* �������� : MySQL�﷨�����ݿ�ṹSQL�ű�\r\n"
				"-- #* �޸ļ�¼ : \r\n"
				"-- #*            $Log: $\r\n"
				"-- #*\r\n"
				"-- #* Ver  Date        Author      Comments\r\n"
				"-- #* ---  ----------  ----------  -------------------------------------------\r\n"
				"-- #* 001  %s  SSP_ASSIST  ��SSP01ƽ̨��SSP_ASSIS�Զ�����\r\n"
				"-- #*\r\n"
				"-- ########################################################################\r\n"
				"\r\n\r\n"
				"/*!40101 SET NAMES gb2312 */;\r\n\r\n",
		sShortName.data(),
		now.toString("yyyy-MM-dd hh:mm").data(),
		now.toString("yyyy-MM-dd").data()
		);
	
	int iTable,iTables = pRoot->childCount();
	SString sLine;
	SString sql;
	SRecordset rs;
	for(iTable=0;iTable<iTables;iTable++)
	{
		QTreeWidgetItem *pTable = pRoot->child(iTable);
		if(pTable == NULL || pTable->checkState(0) == Qt::Unchecked)
			continue;
		sLine = "";
		SString tablename = pTable->text(0).toStdString().data();
		sLine += SString::toFormat(
			"-- ########################################################################\r\n"
			"-- Table structure for %s\r\n"
			"-- ########################################################################\r\n"
			"drop table if exists `%s`;\r\n"
			"create table `%s`\r\n"
			"(\r\n",
			tablename.data(),tablename.data(),tablename.data());
		int iField,iFields = pTable->childCount();
		SString sKeyName;
		for(iField=0;iField<iFields;iField++)
		{
			QTreeWidgetItem *pField = pTable->child(iField);
// 			if(pField == NULL || pField->checkState(0) == Qt::Unchecked)
// 				continue;
			SString name = pField->text(0).toStdString().data();
			SString sType = pField->text(1).toStdString().data();
			SString sIsNull = pField->text(2).toStdString().data();
			SString sKey = pField->text(3).toStdString().data();
			SString sDefault = pField->text(4).toStdString().data();
			SString sExtra = pField->text(5).toStdString().data();
			if(sDefault.length() > 0)
				sDefault = SString::toFormat(" default '"+sDefault+"'");
			else if(sIsNull!="NO")
				sDefault =  " default null";
			if(sExtra.length() > 0)
				sExtra = " "+sExtra;
			sLine += SString::toFormat(
				"  `%s` %s%s%s%s,\r\n",
				name.data(),sType.data(),sIsNull=="NO"?" not null":"",
				sDefault.data(),sExtra.data());
			if(sKey == "PRI")
			{
				if(sKeyName.length() > 0)
					sKeyName += ",";
				sKeyName += "`";
				sKeyName += name;
				sKeyName += "`";
			}
		}
		//���û�����������ˣ����Զ�������������ֹ���ɵ�����������Ҫ��
		bool bHavePk = true;
		if(sKeyName.length() == 0)
		{
			bHavePk = false;
			//�����һ���ֶεĶ��Ÿ�Ϊ�ո�
			(sLine.data())[sLine.length()-3] = ' ';
		}
		else
		{
			//��������,���û����ȷָ���������������ֶ�һ����Ϊ����
// 			if(sKeyName.length() == 0)
// 			{
// 				bHavePk = false;
// 				for(iField=0;iField<iFields;iField++)
// 				{
// 					QTreeWidgetItem *pField = pTable->child(iField);
// 					if(pField == NULL || pField->checkState(0) == Qt::Unchecked)
// 						continue;
// 					SString name = pField->text(0).toStdString().data();
// 					if(sKeyName.length() > 0)
// 						sKeyName += ",";
// 					sKeyName += name;
// 				}
// 			}
			sLine += SString::toFormat("  primary key(%s)",sKeyName.data());
		}



		//��������
		sql.sprintf("show index from `%s` where key_name<>'PRIMARY'",tablename.data());
		m_mysql.Retrieve(sql,rs);
		SString sLastKeyName="";
		int icols = 0;
		int fromidx = bHavePk?1:0;
		for(int i=fromidx;i<rs.GetRows();i++)
		{
			int non_unique = rs.GetValue(i,1).toInt();//0:unique 1:normal
			SString key_name = rs.GetValue(i,2);
			SString col_name = rs.GetValue(i,4);
			if(key_name != sLastKeyName)
			{
				sLastKeyName = key_name;
				if(i > fromidx)
					sLine += ")";
				icols = 0;
				sLine += SString::toFormat(",\r\n  %skey %s (",(non_unique==0?"unique ":""),key_name.data());
			}
			if(icols > 0)
				sLine += ",";
			icols++;
			sKeyName += "`";
			sLine += col_name;
			sKeyName += "`";
		}
		if(rs.GetRows() > fromidx)
			sLine += ")\r\n";
		else
			sLine += "\r\n";
		sLine += SString::toFormat(") engine=innodb default charset=utf8;\r\n\r\n");
		file.writeBlock((BYTE*)sLine.data(),sLine.length());
	}
	file.close();
	SQt::ShowInfoBox(tr("��ʾ"),tr("�ļ�����ɹ�!"));
}

void view_mysql_mdbcfg::OnExportSQL_ORACLE()
{
	QTreeWidgetItem *pRoot = ui.treeWidget->topLevelItem(0);
	if(pRoot == NULL)
		return;
	QString fileName = QFileDialog::getSaveFileName(this,tr("Oracle���ݿ⽨��SQL�ļ�"), "" , tr("SQL files (*.sql)"));
	if(fileName.length() == 0)
		return;
	SString sFile = fileName.toStdString().data();
	if(sFile.right(4).toLower() != ".sql")
		sFile += ".sql";
	SFile file(sFile);
	if(!file.open(IO_Truncate))
	{
		SQt::ShowErrorBox(tr("����"),tr("�����ļ�ʱʧ��!"));
		return;
	}
	SString sShortName = sFile;
	if(sShortName.findRev("\\") > 0)
		sShortName = sShortName.mid(sShortName.findRev("\\")+1);
	if(sShortName.findRev("/") > 0)
		sShortName = sShortName.mid(sShortName.findRev("/")+1);

	SDateTime now = SDateTime::currentDateTime();
	file.fprintf("-- ########################################################################\r\n"
		"-- #*\r\n"
		"-- #* �� �� �� : %s\r\n"
		"-- #* �������� : %s\r\n"
		"-- #* ��    �� : SSP_ASSIST(skt001@163.com)\r\n"
		"-- #* �޸����� : $Date:$\r\n"
		"-- #* ��ǰ�汾 : $Revision: 001$\r\n"
		"-- #* �������� : Oracle�﷨�����ݿ�ṹSQL�ű�\r\n"
		"-- #* �޸ļ�¼ : \r\n"
		"-- #*            $Log: $\r\n"
		"-- #*\r\n"
		"-- #* Ver  Date        Author      Comments\r\n"
		"-- #* ---  ----------  ----------  -------------------------------------------\r\n"
		"-- #* 001  %s  SSP_ASSIST  ��SSP01ƽ̨��SSP_ASSIS�Զ�����\r\n"
		"-- #*\r\n"
		"-- ########################################################################\r\n"
		"\r\n\r\n",
		sShortName.data(),
		now.toString("yyyy-MM-dd hh:mm").data(),
		now.toString("yyyy-MM-dd").data()
		);

	int iTable,iTables = pRoot->childCount();
	SString sLine;
	SString sql;
	SRecordset rs;
	for(iTable=0;iTable<iTables;iTable++)
	{
		QTreeWidgetItem *pTable = pRoot->child(iTable);
		if(pTable == NULL || pTable->checkState(0) == Qt::Unchecked)
			continue;
		sLine = "";
		SString tablename = pTable->text(0).toStdString().data();
		sLine += SString::toFormat(
			"-- ########################################################################\r\n"
			"-- Table structure for %s\r\n"
			"-- ########################################################################\r\n"
			"prompt Creating table %s ...\r\n"
			"drop table %s;\r\n"
			"create table %s\r\n"
			"(\r\n",
			tablename.data(),tablename.data(),tablename.data(),tablename.data());
		int iField,iFields = pTable->childCount();
		SString sKeyName;
		for(iField=0;iField<iFields;iField++)
		{
			QTreeWidgetItem *pField = pTable->child(iField);
			// 			if(pField == NULL || pField->checkState(0) == Qt::Unchecked)
			// 				continue;
			SString name = pField->text(0).toStdString().data();
			SString sType = MySqlType2Oracle(pField->text(1).toStdString().data());
			SString sIsNull = pField->text(2).toStdString().data();
			SString sKey = pField->text(3).toStdString().data();
			SString sDefault = pField->text(4).toStdString().data();
			SString sExtra = pField->text(5).toStdString().data();
			if(sDefault.length() > 0)
				sDefault = SString::toFormat(" default '"+sDefault+"'");
			//else if(sIsNull!="NO")
			//	sDefault =  " default null";
			//if(sExtra.length() > 0)
			//	sExtra = " "+sExtra;
			sExtra = "";
			if(iField > 0)
				sLine += ",\r\n";
			sLine += SString::toFormat(
				"  %s %s%s%s%s",
				name.data(),sType.data(),
				sDefault.data(),
				sIsNull=="NO"?" not null":"",
				sExtra.data());
			if(sKey == "PRI")
			{
				if(sKeyName.length() > 0)
					sKeyName += ",";
				sKeyName += name;
			}
		}

		//��������,���û����ȷָ���������������ֶ�һ����Ϊ����
		bool bHavePk = true;
		if(sKeyName.length() == 0)
		{
			bHavePk = false;
// 			for(iField=0;iField<iFields;iField++)
// 			{
// 				QTreeWidgetItem *pField = pTable->child(iField);
// 				if(pField == NULL || pField->checkState(0) == Qt::Unchecked)
// 					continue;
// 				SString name = pField->text(0).toStdString().data();
// 				if(sKeyName.length() > 0)
// 					sKeyName += ",";
// 				sKeyName += name;
// 			}
		}
		sLine += "\r\n);\r\n";
		if(bHavePk)
		{
			sLine += SString::toFormat("alter table %s add constraint pk_%s primary key (%s);\r\n",
				tablename.data(),tablename.data(),sKeyName.data());
		}
		
		//��������
		sql.sprintf("show index from %s where key_name<>'PRIMARY'",tablename.data());
		m_mysql.Retrieve(sql,rs);
		SString sLastKeyName="";
		int icols = 0;
		int fromidx = bHavePk?1:0;
		for(int i=fromidx;i<rs.GetRows();i++)
		{
			int non_unique = rs.GetValue(i,1).toInt();//0:unique 1:normal
			SString key_name = rs.GetValue(i,2);
			SString col_name = rs.GetValue(i,4);
			if(key_name != sLastKeyName)
			{
				sLastKeyName = key_name;
				if(i > fromidx)
					sLine += ");\r\n";
				icols = 0;
				sLine += SString::toFormat("create %sindex %s on %s (",(non_unique==0?"unique ":""),key_name.data(),tablename.data());
			}
			if(icols > 0)
				sLine += ",";
			icols++;
			sLine += col_name;
		}
		if(rs.GetRows() > fromidx)
			sLine += ");\r\n";
		sLine += "\r\n";
		file.writeBlock((BYTE*)sLine.data(),sLine.length());
	}
	file.close();
	SQt::ShowInfoBox(tr("��ʾ"),tr("�ļ�����ɹ�!"));
}

void view_mysql_mdbcfg::OnExportData_SDB()
{
	//�������ݵ�SDB�ļ�
	QTreeWidgetItem *pRoot = ui.treeWidget->topLevelItem(0);
	if(pRoot == NULL)
		return;
	QString fileName = QFileDialog::getSaveFileName(this,tr("��������SSPƽ̨SDB���ݿⱸ���ļ�"), "" , tr("SDB files (*.sdb)"));
	if(fileName.length() == 0)
		return;
	SString sFile = fileName.toStdString().data();
	if(sFile.right(4).toLower() != ".sdb")
		sFile += ".sdb";
	SFile file(sFile);
	if(!file.open(IO_Truncate))
	{
		SQt::ShowErrorBox(tr("����"),tr("�����ļ�ʱʧ��!"));
		return;
	}
	SString sShortName = sFile;
	if(sShortName.findRev("\\") > 0)
		sShortName = sShortName.mid(sShortName.findRev("\\")+1);
	if(sShortName.findRev("/") > 0)
		sShortName = sShortName.mid(sShortName.findRev("/")+1);
	SSP_GUI->ShowWaiting();
	SDateTime now = SDateTime::currentDateTime();
	file.fprintf("<?xml version=\"1.0\" encoding=\"GBK\"?>\r\n"
		"<!--\r\n"
		"########################################################################\r\n"
		"#*\r\n"
		"#* �� �� �� : %s\r\n"
		"#* �������� : %s\r\n"
		"#* ��    �� : SSP_ASSIST(skt001@163.com)\r\n"
		"#* �޸����� : $Date:$\r\n"
		"#* ��ǰ�汾 : $Revision: 001$\r\n"
		"#* �������� : SSPƽ̨���ݿⱸ���ļ�(SDB)\r\n"
		"#* �޸ļ�¼ : \r\n"
		"#*            $Log: $\r\n"
		"#*\r\n"
		"#* Ver  Date        Author      Comments\r\n"
		"#* ---  ----------  ----------  -------------------------------------------\r\n"
		"#* 001  %s  SSP_ASSIST  ��SSP01ƽ̨��SSP_ASSIS�Զ�����\r\n"
		"#*\r\n"
		"########################################################################\r\n"
		"-->\r\n\r\n"
		"<ssp_db>\r\n",
		sShortName.data(),
		now.toString("yyyy-MM-dd hh:mm").data(),
		now.toString("yyyy-MM-dd").data()
		);

	int iTable,iTables = pRoot->childCount();
	SString sLine;
	SString sql;
	SRecordset rs;
	int iSaveTabs=0;
	int iSaveRows=0;
	for(iTable=0;iTable<iTables;iTable++)
	{
		QTreeWidgetItem *pTable = pRoot->child(iTable);
		if(pTable == NULL || pTable->checkState(0) == Qt::Unchecked)
			continue;
		sLine = "";
		SString tablename = pTable->text(0).toStdString().data();

		sLine += SString::toFormat("  <table name=\"%s\">\r\n",tablename.data());
		SString select_field;
		SStringList blob_fields;
		SStringList pk_fields;
		SStringList type_fields;
		SValueList<int> pk_field_idx;
		SValueList<bool> isnumber_field;
		int iField,iFields = pTable->childCount();
		for(iField=0;iField<iFields;iField++)
		{
			QTreeWidgetItem *pField = pTable->child(iField);
			SString name = pField->text(0).toStdString().data();
			SString sType = pField->text(1).toStdString().data();
			SString sIsNull = pField->text(2).toStdString().data();
			SString sKey = pField->text(3).toStdString().data();
			SString sDefault = pField->text(4).toStdString().data();
			SString sExtra = pField->text(5).toStdString().data();
			type_fields.append(sType);
			bool isnumber = (sType.find("decimal")>=0 || sType.find("number")>=0 || sType.find("int")>=0 || sType.find("float")>=0);
			isnumber_field.append(isnumber);
			sLine += SString::toFormat("    <column name=\"%s\" type=\"%s\" isnull=\"%s\" key=\"%s\" default=\"%s\" extattr=\"%s\" />\r\n",
				name.data(),sType.data(),sIsNull.data(),sKey.data(),sDefault.data(),sExtra.data());
			if(select_field.length() > 0)
				select_field += ",";
			if(MySqlType2Oracle(sType).toLower().find("blob") < 0)
				select_field += name;
			else
			{
				select_field += "null";
				blob_fields.append(name);
			}
			if(sKey == "PRI")
			{
				pk_fields.append(name);
				pk_field_idx.append(iField);
			}
		}

		//���û�ж����������������ֶ���Ϊ����ʹ��,BLOB�ֶγ���
		if(pk_fields.count() == 0)
		{
			for(iField=0;iField<iFields;iField++)
			{
				QTreeWidgetItem *pField = pTable->child(iField);
				SString name = pField->text(0).toStdString().data();
				SString sType = pField->text(1).toStdString().data();
				if(sType.toLower().find("blob") < 0)
				{
					pk_fields.append(name);
					pk_field_idx.append(iField);
				}
			}
		}

		//write rows
		{
			bool bSelectError = false;
			MYSQL *pMySQL = m_mysql.GetConnectHandle();
			SRecordset rs;
			int offset=0;
			int len = 10000;
			int iRowSize = 0;
			unsigned long pos;
			while(1)
			{
				sql.sprintf("select %s from %s limit %d,%d",select_field.data(),tablename.data(),offset,len);
				int ret = mysql_query(pMySQL,sql.data());
				if(ret != 0)
				{
					LOGERROR("Retrieve mysql_query error:%s",mysql_error(pMySQL));
					bSelectError = true;
					break;
				}

				MYSQL_RES *pRes;
				if (!(pRes=mysql_store_result(pMySQL)))
				{
					LOGERROR("Couldn't get result from %s",mysql_error(pMySQL));
					bSelectError = true;
					break;
				}

				int cols = (int)mysql_num_fields(pRes);
				int rows = (int)mysql_num_rows(pRes);
				int i,j;
				MYSQL_ROW row;
				stuTableField *pFld;
				row= mysql_fetch_row(pRes);
				for(i=0;row != NULL && i<rows;i++)
				{
					sLine += "\t<r><![CDATA[";
					for(j=0;j<cols;j++)
					{
						if(j > 0)
							sLine += ",";
						if(row[j] == NULL)
						{
							if(type_fields[j]->find("char")>=0)
								sLine += "''";
							else
								sLine += "null";
						}
						else
						{
							if(*isnumber_field[j])
							{
								sLine += row[j];
							}
							else
							{
// 								if(type_fields[j]->find("date")=0)
// 								{
// 									sLine += "to_date(";
// 									sLine = row[j];
// 									sLine += ",'yyyy-mm-dd hh24:mi:ss')";
// 								}
// 								else if(type_fields[j]->find("timestamp")>=0)
// 								{
// 									sLine += "to_timestamp(";
// 									sLine = row[j];
// 									sLine += ",'yyyy-mm-dd hh24:mi:ss')";
// 								}
// 								else
								{
									sLine += "'";
									if(strstr(row[j],"'") != NULL)
									{
										sql = row[j];
										sql.replace("'","''");
										sLine += sql;
									}
									else
										sLine += row[j];
									sLine += "'";
								}
							}
						}					
					}
					sLine += "]]>";
					if(blob_fields.count() == 0)
						sLine += "</r>\r\n";
					else
					{
						//��blob�ֶ�
						sLine += "\r\n";
						for(j=0;j<blob_fields.count();j++)
						{
							BYTE *pBlobBuffer = NULL;
							int iBlobBufferSize = 0;
							//�������ֶβ���
							sql = "";
							for(int k=0;k<pk_fields.count();k++)
							{
								if(sql.length() > 0)
									sql += " and ";
								sql += *pk_fields.at(k);
								sql += "='";
								sql += row[*pk_field_idx.at(k)];
								sql += "'";
							}
							if(m_mysql.ReadLobToMem(tablename,*blob_fields.at(j),sql,pBlobBuffer,iBlobBufferSize) && iBlobBufferSize > 0)
							{
								sLine += SString::toFormat("      <blob name=\"%s\" size=\"%d\" where=\"%s\"><![CDATA[",
									blob_fields.at(j)->data(),iBlobBufferSize,sql.data());
								if(pBlobBuffer != NULL && iBlobBufferSize > 0)
									sLine += SString::HexToStr(pBlobBuffer,iBlobBufferSize,false);
								sLine += "]]></blob>\r\n";
							}

						}
						sLine += "    </r>\r\n";
					}
					iSaveRows ++;
					if(sLine.length() > 65536)
					{
						file.writeBlock((BYTE*)sLine.data(),sLine.length());
						sLine = "";
						//SSP_GUI->SetStatusTip(SString::toFormat("�ѵ���%d�ű�,��%d����¼...",iSaveTabs,iSaveRows));
						SSP_GUI->SetStatusTip(SString::toFormat("����%d�ű�,��%d����¼...  ��ǰ��%s,����%d/%d����¼...",iSaveTabs,iSaveRows,tablename.data(),i,rows));
						SQt::ProcessUiEvents();
					}
					row= mysql_fetch_row(pRes);
				}
				
				mysql_free_result(pRes);
				
				if(rows < len)
					break;
				offset += len;
			}
		}
		sLine += "  </table>\r\n";
		file.writeBlock((BYTE*)sLine.data(),sLine.length());
		sLine = "";
		iSaveTabs++;
		SSP_GUI->SetStatusTip(SString::toFormat("�ѵ���%d�ű�,��%d����¼...",iSaveTabs,iSaveRows));
		SQt::ProcessUiEvents();
	}
	file.fprintf("</ssp_db>\r\n");
	SSP_GUI->HideWaiting();
	file.close();
	SSP_GUI->SetStatusTip(SString::toFormat("�ۼƵ���%d�ű�,��%d����¼!",iSaveTabs,iSaveRows));
	SQt::ShowInfoBox(tr("��ʾ"),SString::toFormat("��¼�����ɹ�!�ۼƵ���%d�ű�,��%d����¼!",iSaveTabs,iSaveRows).data());
}

void view_mysql_mdbcfg::OnImportData_SDB()
{
	QString fileName = QFileDialog::getOpenFileName(this,tr("�������SSPƽ̨SDB���ݿⱸ���ļ�"), "", tr("SDB�ļ� (*.sdb)"));
	if(fileName.length() == 0)
		return;
	m_DbRestore.m_bTrunkTab = (ui.checkBox_TrunkTabAtRest->checkState() == Qt::Checked);
	SString sFile = fileName.toStdString().data();
	if(!m_DbRestore.ParseXmlFile(sFile.data()))
	{
		SQt::ShowErrorBox(tr("����"),tr("�������ݿⱸ��ʱʧ��!"));
		return;
	}
	SSP_GUI->ShowWaiting();
	if(m_DbRestore.m_sql.length() > 0)
		m_DbRestore.ProcessEndSql();
	SSP_GUI->HideWaiting();
	SSP_GUI->SetStatusTip(SString::toFormat("�ۼƵ���%d�ű�,��%d����¼!%s",
		m_DbRestore.m_iSaveTabs,m_DbRestore.m_iSaveRows,m_DbRestore.m_iDbErrors==0?"":(SString::toFormat("����%d��SQLָ��ʧ��!",m_DbRestore.m_iDbErrors).data())));
	SQt::ShowInfoBox(tr("��ʾ"),SString::toFormat("�ۼƵ���%d�ű�,��%d����¼!%s",
		m_DbRestore.m_iSaveTabs,m_DbRestore.m_iSaveRows,m_DbRestore.m_iDbErrors==0?"":(SString::toFormat("����%d��SQLָ��ʧ��!",m_DbRestore.m_iDbErrors).data())).data());
}

