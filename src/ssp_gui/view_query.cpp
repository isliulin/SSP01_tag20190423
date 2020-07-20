#include "view_query.h"
#include "ssp_gui.h"

view_query::view_query(QWidget *parent,SString sFunName,bool isMDB)
	: CBaseView(parent)
{
	m_tableRow = 0;
	ui.setupUi(this);
	m_bCondConfirm = false;
	m_refName = /*"ds_" + */sFunName;
	m_fieldlist = new SPtrList<CSsp_DwColumn>;
	dlg=new view_dw_condlg();
	m_font_underline.setUnderline(true);
	QString err="";
	this->isMDB=isMDB;
	if(!dlg->refreshTableByName(m_refName,err)){}
		//QMessageBox::information(this,tr("����"),tr("��ʼ����̬��ѯ��������ʧ��,ԭ��Ϊ")+err,QMessageBox::Ok);
	InitDataWindow(m_refName);
	if(dlg->isConditionEmpty())
		InitWidget(m_refName);
	else
	//if((!dlg->isConditionEmpty())&&dlg->getisQueryNecessary())
	{
		dlg->setModal(true);
		dlg->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::Tool| Qt::Dialog);
		dlg->exec();
		if(dlg->isOk())
		{
			CSsp_Dataset* dbSet=NULL;
			m_bCondConfirm = true;
			dbSet =  m_dataWindow->SearchDataset(m_refName);
			dbSet->SetCondition(*(dlg->getListDwCon()));
			SetCommonToWidget(dbSet);
		}
	}
// 	else
// 	{
// 		CSsp_Dataset* dbSet=NULL;
// 		dbSet =  m_dataWindow->SearchDataset(m_refName);
// 		SetCommonToWidget(dbSet);
// 	}

	timer = new QTimer(this);
	if(isMDB)
	{
		m_dataWindow->SearchDataset(m_refName)->RegisterTriggerCallback(OnMdbDatasetTrgCallback,this);	
		connect(timer,SIGNAL(timeout()),this,SLOT(timerOut()));
		timer->start(200);//�������ƻص���ˢ��Ƶ��
	}
	m_bNotPreShow = true;
	SQt::SetTableWidgetStyle(ui.tableWidget);
	//ui.tableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget->horizontalHeader()->setToolTip(tr("�����ͷ������Ӧ���"));
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(onPushButtonPressed()));
	connect(ui.pushButton_2,SIGNAL(clicked()),this,SLOT(onPushButton2Pressed()));
	connect(ui.pushButton_3,SIGNAL(clicked()),this,SLOT(onPushButton3Pressed()));
}

view_query::~view_query()
{
	delete m_fieldlist;
	delete dlg;
	if(isMDB)
	{
		m_dataWindow->SearchDataset(m_refName)->RemoveTriggerCallback(OnMdbDatasetTrgCallback,this);
	}
}
void view_query::OnPreShow()
{
	if(m_tableRow > 0 && !dlg->isConditionEmpty())
		return;//��ˢ�³���¼�����ж�̬����ʱ������ˢ��
	if(m_bNotPreShow)
	{
		m_bNotPreShow = false;
		return;
	}
	if(!m_bCondConfirm && (!dlg->isConditionEmpty())&&dlg->getisQueryNecessary())
	{
		dlg->setModal(true);
		dlg->exec();
		if(dlg->isOk())
		{
			m_bCondConfirm = true;
			CSsp_Dataset* dbSet=NULL;
			dbSet =  m_dataWindow->SearchDataset(m_refName);
			dbSet->SetCondition(*(dlg->getListDwCon()));
			SetCommonToWidget(dbSet);
		}
	}
	else
	{
		InitDataWindow(m_refName);
		InitWidget(m_refName);
	}
// 	InitDataWindow(m_refName);
// 	InitWidget(m_refName);

}

void view_query::timerOut()
{
	refreshReady=true;
}
void view_query::InitDataWindow(SString sWname)
{
	S_UNUSED(sWname);
	m_dataWindow = GET_DW_CFG;
}

bool view_query::InitWidget(SString sWname)
{
	S_UNUSED(sWname);
	m_tableRow =0;
	m_fieldNamelist.clear();
	m_fieldlist->clear();
	QPalette pal;
	pal.setColor(QPalette::Base, QColor(255, 255, 255));
	pal.setColor(QPalette::AlternateBase, QColor(10, 10, 10,40));
	ui.tableWidget->setPalette(pal);
	ui.tableWidget->setAlternatingRowColors(true);
 	CSsp_Dataset* dbSet=NULL;
 	dbSet = m_dataWindow->SearchDataset(m_refName);
 	SetCommonToWidget(dbSet);
	return true;
}
SPtrList<CSsp_DwColumn> *view_query::getFieldInfo(SString sWname,SPtrList<CSsp_DwColumn> *cFinfo)
{
	CSsp_Dataset* dbSet=NULL;
	dbSet =  m_dataWindow->SearchDataset(sWname);
	if (dbSet==NULL)
	{
		return m_fieldlist;
	}
	int iCount = dbSet->GetColumnCount();
	for(int i = 0;i<iCount;i++)
	{
		m_fieldlist->append(dbSet->GetColumn(i));
		if (cFinfo!=NULL)
		{
			cFinfo->append(dbSet->GetColumn(i));
		}
	}
	return m_fieldlist;
}
void view_query::SetCommonToWidget(CSsp_Dataset* dbSet)
{
	ui.tableWidget->clear();
	ui.tableWidget->setRowCount(0);

	m_bInsert = true;
	if (dbSet==NULL)
	{
		return;
	}
	SSP_GUI->ShowWaiting();
	getFieldInfo(m_refName);
	QStringList lHeader;
	for (int i = 0;i<m_fieldlist->count();i++)
	{
		lHeader<<QString::fromLocal8Bit((*m_fieldlist)[i]->m_sName.data());
		if ((*m_fieldlist)[i]->m_bPKey)
		{
			m_nPkey = i;
		}
	}
	SString sSql = dbSet->GetSql();
	m_tableName = GetTableFromSql(sSql);

	bool bAutoTrans = dbSet->GetAutoColumnTransform();
	SQt::ProcessUiEvents();
	m_Record =dbSet->GetRecordset();
	SQt::ProcessUiEvents();
	int iRow = m_Record->GetRows();
	m_tableRow = iRow;
	int iColumn = m_Record->GetColumns();
	for (int i = 0;i<iColumn;i++)
	{
		m_fieldNamelist.push_back(m_Record->GetColumnName(i));
	}
	ui.tableWidget->setColumnCount(iColumn);
	ui.tableWidget->setRowCount(iRow);
	ui.tableWidget->setHorizontalHeaderLabels(lHeader);
	SRecordset* pRef=NULL;
	CSsp_DwColumn *pDwCol;
	SString act;
	QTableWidgetItem * item =NULL;
	for (int i=0;i<iRow;i++)
	{
		SQt::ProcessUiEvents();
		for (int j = 0;j<iColumn;j++)
		{	
			if(!(j>=m_fieldlist->count()))
			{
				pDwCol = m_fieldlist->at(j);
				if (pDwCol->m_pRef==NULL)
				{
					if(!bAutoTrans && pDwCol->m_sConv!="")
						item = new QTableWidgetItem(pDwCol->GetConvDb2Sys(m_Record->GetValue(i,j)).data());
					else
						item = new QTableWidgetItem(m_Record->GetValue(i,j).data());
					item->setFlags(item->flags()&~Qt::ItemIsEditable);//���ɱ༭
					ui.tableWidget->setItem(i,j,item);
				}
				else
				{
					item = new QTableWidgetItem();
					item->setFlags(item->flags()&~Qt::ItemIsEditable);
					ui.tableWidget->setItem(i,j,item);
					pRef  = pDwCol->m_pRef->GetRecordset();
					//int iIndex = 0;
					for (int k = 0;k<pRef->GetRows();k++)
					{
						if (m_Record->GetValue(i,j)==pRef->GetValue(k,0))
						{
							item->setText(tr(pRef->GetValue(k,1).data()));
							break;
						}
						else if(k==pRef->GetRows()-1)//û�ҵ���Ӧ��ȡĬ��ֵ
						{
							//���û���ҵ�������Ϊ��, 20181122�޸�
							item = new QTableWidgetItem(QString(""));
							//item = new QTableWidgetItem(tr(pRef->GetValue(0,1).data()));
							item->setFlags(item->flags()&~Qt::ItemIsEditable);
							ui.tableWidget->setItem(i,j,item);
						}
					}
				}
				act = SString::GetAttributeValue(pDwCol->m_sExtAttr,"act");
				if(act == "gen_act" || act == "float_wnd" || act == "dialog_wnd")
				{
					QTableWidgetItem *pItem = ui.tableWidget->item(i,j);
					pItem->setFont(m_font_underline);
					pItem->setTextColor(Qt::blue);
					pItem->setTextAlignment(Qt::AlignCenter);
					act = SString::GetAttributeValue(pDwCol->m_sExtAttr,"tip");
					if(act.length() > 0)
						pItem->setToolTip(act.data());
				}
			}
		}
	}
	ui.tableWidget->resizeColumnsToContents();

	m_bInsert = false;
	SSP_GUI->HideWaiting();
}
bool view_query::SetTableValue(int row,int col,QString value)
{
	if(ui.tableWidget->item(row,col)==NULL)
		return false;
	ui.tableWidget->item(row,col)->setText(value);
	return true;
}
int view_query::getTableRowByFcol(QString colValue)
{

	for(int i=0;i<ui.tableWidget->rowCount();i++)
	{
		if(ui.tableWidget->item(i,0)->text()==colValue)
			return i;
	}
	return -1;
}
BYTE* view_query::OnMdbDatasetTrgCallback(void* cbParam, CMdbClient *pMdbClient,CSsp_Dataset *pDataset, eMdbTriggerType eType,int iTrgRows,int iRowSize,BYTE *pTrgData)
{
	S_UNUSED(pMdbClient);
	view_query *pThis=(view_query *)cbParam;
	if(eType == MDB_TRG_UPDATE&&pThis->refreshReady==true)
	{
		CSsp_Dataset* dbSet=NULL;
		dbSet =  pThis->m_dataWindow->SearchDataset(pThis->m_refName);
		if (dbSet==NULL)
		{
			return NULL;
		}
		bool bAutoTrans = dbSet->GetAutoColumnTransform();
		int i/*,j*/;
		//unsigned long pos;
		//CSsp_DwColumn *pCol;
		BYTE *pRow = pTrgData;
		unsigned int iPrimKey;
		int rowidx;
		SRecord *pR;
		SRecordset *pRs = pDataset->GetRecordset();

		
		if(pRs == NULL)
			return NULL;
		int oidbytes = pRs->GetOidBytes();
		if(oidbytes > (int)sizeof(iPrimKey))
		{
			LOGWARN("OID����[%d]����ȷ!�����нض�!",oidbytes);
			oidbytes = sizeof(iPrimKey);
		}
		pDataset->LockInnerRs();
		for(i=0;i<iTrgRows;i++)
		{
			memcpy(&iPrimKey,pRow,oidbytes);
			pR = pDataset->SearchRecordByOid(NULL,iPrimKey,rowidx);
			if(pR == NULL)
			{
				pRow += iRowSize;
				continue;
			}
			int tableRow=pThis->getTableRowByFcol(pR->GetValue(0).data());
			if(tableRow==-1)
			{
				pDataset->UnlockInnerRs();
				return NULL;
			}
// 			j = 0;
// 			for(int k=0;k<pR->GetCount();k++)
// 			{
// 				if(pR->GetValue(k)==SString::toFormat((const char*)pRow))
// 					pThis->SetTableValue(tableRow,k,pR->GetValue(k).data());
// 			}
			for(int k=0;k<pR->GetCount();k++)
			{
				if(!bAutoTrans && pDataset->GetColumn(k)->m_sConv!="")
					pThis->SetTableValue(tableRow,k,pDataset->GetColumn(k)->GetConvDb2Sys(pR->GetValue(k)).data());//��������
				else
					pThis->SetTableValue(tableRow,k,pR->GetValue(k).data());//��������
			}
			pRow += iRowSize;
		}
		pDataset->UnlockInnerRs();
		pThis->refreshReady=false;
	}
 	return NULL;
}

SString view_query::GetTableFromSql(SString sSql)
{
	SString sRight =sSql.right(sSql.length()- sSql.find(" from ",0,false)-6).stripWhiteSpace();
	if (sRight.find(' ')>0)
	{
		sRight = sRight.left(sRight.find(' '));
	}
	return sRight;
}

void view_query::onPushButtonPressed()
{
	//��ѯ
	if(dlg!=NULL&&(!dlg->isConditionEmpty()))
	{
		dlg->exec();
		if(!dlg->isOk())
			return;
		m_bCondConfirm = true;
		CSsp_Dataset* dbSet=NULL;
		dbSet =  m_dataWindow->SearchDataset(m_refName);
		dbSet->SetCondition(*(dlg->getListDwCon()));
		SetCommonToWidget(dbSet);
	}
	else
	{
		CSsp_Dataset* dbSet=NULL;
		dbSet =  m_dataWindow->SearchDataset(m_refName);
		SetCommonToWidget(dbSet);
	}
}
void view_query::on_tableWidget_itemClicked(QTableWidgetItem * item)
{
	if(item->row() < 0 || item->column() < 0)
		return;
	CSsp_Dataset* dbSet=NULL;
	dbSet =  m_dataWindow->SearchDataset(m_refName);
	if (dbSet==NULL)
	{
		SQt::ShowErrorBox(QObject::tr("��ʾ"),QObject::tr("��ȡ���ݼ�ʧ��"));
		return;
	}

	CSsp_DwColumn *pDwCol = m_fieldlist->at(item->column());
	if(pDwCol == NULL)
		return;
	SString act = SString::GetAttributeValue(pDwCol->m_sExtAttr,"act");
	if(act.length() == 0)
		return;
	if(act != "gen_act" && act != "float_wnd" && act != "dialog_wnd")
		return;
	SString sExtAttr = pDwCol->m_sExtAttr;
	int p1,p2;
	int col;
	SString colvalue;
	while((p1=sExtAttr.find("{@COL:")) >= 0)
	{
		p1+=6;
		p2 = sExtAttr.find("@}",p1);
		if(p2 < p1)
			break;
		col = sExtAttr.mid(p1,p2-p1).toInt();

		if(col > 0 && col <= m_fieldlist->count())
		{
			colvalue = dbSet->GetInnerRecordsetDirect()->GetValue(item->row(),col-1);
			sExtAttr = sExtAttr.left(p1-6)+colvalue+sExtAttr.mid(p2+2);
		}
		else
			break;
	}
	SSP_GUI->SetSessionAttributeValues(sExtAttr);

	if(act == "gen_act")//ͨ�ö���
	{
		int sn = SString::GetAttributeValueI(pDwCol->m_sExtAttr,"sn");
		SSP_GUI->RunGeneralAction(sn);
		OnPreShow();
	}
	else if(act == "float_wnd")//������̬����
	{
		int sn = SString::GetAttributeValueI(pDwCol->m_sExtAttr,"sn");
		SSP_GUI->OpenFloatWnd(sn,SString::GetAttributeValueI(pDwCol->m_sExtAttr,"w"),SString::GetAttributeValueI(pDwCol->m_sExtAttr,"h"),
			SString::GetAttributeValueI(pDwCol->m_sExtAttr,"alpha"),SString::GetAttributeValue(pDwCol->m_sExtAttr,"title"));
		//OnPreShow();
	}
	else if(act == "dialog_wnd")//������̬����
	{
		int sn = SString::GetAttributeValueI(pDwCol->m_sExtAttr,"sn");
		SSP_GUI->OpenDialogWnd(sn,SString::GetAttributeValueI(pDwCol->m_sExtAttr,"w"),SString::GetAttributeValueI(pDwCol->m_sExtAttr,"h"),
			SString::GetAttributeValueI(pDwCol->m_sExtAttr,"alpha"),SString::GetAttributeValue(pDwCol->m_sExtAttr,"title"));
		//OnPreShow();
	}
}


void view_query::queryTable()
{
// 	map<QString,QString> querymap=queryDlg->getMap();
// 	SetCommonToWidget(querymap);
}
void view_query::onPushButton2Pressed()
{
	//��ӡ
	SQt::TablePrint(m_dataWindow->SearchDataset(m_refName)->GetDesc(),ui.tableWidget);
}

void view_query::onPushButton3Pressed()
{
	//����
	SQt::TableExort(m_dataWindow->SearchDataset(m_refName)->GetDesc(),ui.tableWidget);
}