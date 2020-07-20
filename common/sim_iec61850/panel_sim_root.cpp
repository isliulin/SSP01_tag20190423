#include "panel_sim_root.h"
#include "GuiApplication.h"
#include <math.h>
panel_sim_root::panel_sim_root(CSimManager *pSim,QWidget *parent)
	: QWidget(parent)
{
	m_pSimManager = pSim;
	ui.setupUi(this);
	ui.pushButton_Start->setEnabled(true);
	ui.pushButton_Stop->setEnabled(false);
	connect(ui.pushButton_Start,SIGNAL(clicked()),this,SLOT(Start()));
	connect(ui.pushButton_Stop,SIGNAL(clicked()),this,SLOT(Stop()));

	m_iconRoot = QIcon(":/sim_iec61850/Resources/FONT02.ICO");
	m_iconRun = QIcon(":/sim_iec61850/Resources/20070402165933835.gif");
	m_iconStop = QIcon(":/sim_iec61850/Resources/20070402165933865.gif");

	connect(ui.twScripts, SIGNAL(itemPressed(QTreeWidgetItem*, int)), this, SLOT(treeItemPressed(QTreeWidgetItem*, int)));
	connect(ui.twScripts,SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),this,SLOT(OnTreeCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
	connect(ui.btnCheckScript,SIGNAL(clicked()),this,SLOT(OnScriptCheck()));
	connect(ui.btnStartScript,SIGNAL(clicked()),this,SLOT(OnScriptStart()));
	connect(ui.btnStopScript,SIGNAL(clicked()),this,SLOT(OnScriptEnd()));
	connect(ui.btnSearch,SIGNAL(clicked()),this,SLOT(OnFcdaSearch()));
	connect(ui.btnSave,SIGNAL(clicked()),this,SLOT(OnSaveScript()));
	connect(this,SIGNAL(sigScriptStatus(stuSimRunScript*)),this,SLOT(OnScriptStatus(stuSimRunScript*)));

	ui.edtScript->setEnabled(false);
	ui.edtScriptTimes->setEnabled(false);
	ui.edtScriptSepMs->setEnabled(false);
	ui.edtScriptTimes->setEnabled(false);
	ui.btnCheckScript->setEnabled(false);
	ui.btnStartScript->setEnabled(false);
	ui.btnStopScript->setEnabled(false);

	LoadScriptTree();
	m_iThreads = 0;
	m_bStop = true;
	m_bStarted = false;
}

panel_sim_root::~panel_sim_root()
{
	Stop();
}

void panel_sim_root::closeEvent(QCloseEvent * event)
{
	
}

void panel_sim_root::Start()
{
	m_bStarted = true;
	ui.pushButton_Start->setEnabled(false);
	m_pSimManager->m_MmsServer.OnProgressText("��������SV��GOOSEģ����...");
	SQt::ProcessUiEvents();
	if(!m_pSimManager->Start())
	{
		SQt::ShowErrorBox("����","����SV��GOOSEģ��������ʧ��!");
		m_pSimManager->m_MmsServer.OnProgressText("����SV��GOOSEģ��������ʧ��!");
		ui.pushButton_Start->setEnabled(true);
		return;
	}
	m_pSimManager->m_MmsServer.m_pSimConfig = m_pSimManager->m_pSimConfig;
	if(m_pSimManager->m_pSimConfig->m_bOpenMmsServer && SFile::exists(SBASE_SAPP->GetConfPath()+"1.scd"))
	{
#ifndef WIN32
		if(m_pSimManager->m_pSimConfig->m_bMmsSvrSimIpAddr)
			m_pSimManager->m_MmsServer.AddApIpAddr();
#endif

		SString scd=SBASE_SAPP->GetConfPath()+"1.scd";
		int id=0;
#if 1
		unsigned long pos;
		stuSclVtIed *pIed = m_pSimManager->m_pSimConfig->m_MmsServerIed.FetchFirst(pos);
		while(pIed)
		{
			m_pSimManager->m_MmsServer.AddIed(stuMmsServerIed::New(scd,++id,pIed->name,"S1"));
			pIed = m_pSimManager->m_pSimConfig->m_MmsServerIed.FetchNext(pos);
		}
#else
 		int ret = m_pSimManager->m_MmsServer.AddIedByScd(scd);
 		if(ret <0)
 			CGuiApplication::GetApp()->NewEvent("ϵͳ",0,0,"","��ʾ","����SCD�ļ�ʱʧ��!");
#endif
		CGuiApplication::GetApp()->NewEvent("ϵͳ",0,0,"","��ʾ",SString::toFormat("�ɹ�����%d��MMS���ʵ����!",id));

		m_pSimManager->m_MmsServer.Start();		
		CGuiApplication::GetApp()->NewEvent("ϵͳ",0,0,"","��ʾ",SString::toFormat("MMS�����������!"));
	}
	ui.pushButton_Stop->setEnabled(true);
	m_iThreads = 0;
	m_bStop = false;
	SKT_CREATE_THREAD(ThreadRunBcastScript,this);
	CGuiApplication::GetApp()->NewEvent("ϵͳ",0,0,"","��ʾ","ģ������������!");
	m_pSimManager->m_MmsServer.OnProgressText("ģ������������!");
}

void panel_sim_root::Stop()
{
	if(m_bStarted == false)
		return;
	m_bStarted = false;
	m_bStop = true;
	if(!m_pSimManager->Stop())
	{
		SQt::ShowErrorBox("����","ֹͣģ��������ʧ��!");
		return;
	}
#ifndef WIN32
	m_pSimManager->m_MmsServer.RemoveApIpAddr();
#endif
	if(m_pSimManager->m_pSimConfig->m_bOpenMmsServer)
		m_pSimManager->m_MmsServer.StopAndWait();
	while(m_iThreads > 0)
		SApi::UsSleep(100000);
	ui.pushButton_Start->setEnabled(true);
	ui.pushButton_Stop->setEnabled(false);
	CGuiApplication::GetApp()->NewEvent("ϵͳ",0,0,"","��ʾ","ģ��������ֹͣ!");
}

void panel_sim_root::LoadScriptTree()
{
	QFont font;
	font.setFamily("Courier");
	font.setFixedPitch(true);
	font.setPointSize(10);

	ui.edtScript->setFont(font);

	m_highlighter = new Highlighter(ui.edtScript->document());

	ui.twScripts->clear();
	m_pRoot = new QTreeWidgetItem();
	m_pRoot->setData(0,Qt::UserRole,QVariant(0));
	m_pRoot->setText(0,"CS���нű�");
	m_pRoot->setIcon(0,m_iconRoot);
	ui.twScripts->addTopLevelItem(m_pRoot);
	ui.twScripts->setCurrentItem(m_pRoot);

	unsigned long pos;
	stuSimRunScript *pScript = m_pSimManager->m_pSimConfig->m_Scripts.FetchFirst(pos);
	while(pScript)
	{
		//���
		QTreeWidgetItem *pItem = new QTreeWidgetItem();
		pItem->setData(0,Qt::UserRole,QVariant(1));
		pItem->setData(0,Qt::UserRole+1,QVariant(pScript->name.data()));
		pItem->setText(0,pScript->name.data());
		pItem->setToolTip(0,pScript->name.data());
		pItem->setIcon(0,m_iconStop);
		m_pRoot->addChild(pItem);
		pItem->setText(1,"δ����");

		pScript = m_pSimManager->m_pSimConfig->m_Scripts.FetchNext(pos);
	}
	m_pRoot->setExpanded(true);
}


void panel_sim_root::OnTreeCurrentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	if(previous != NULL)
	{
		if(previous->data(0,Qt::UserRole).toInt() == 1)
		{
			//����ǰһ��
			SString name = previous->data(0,Qt::UserRole+1).toString().toStdString().data();
			stuSimRunScript *pScript = m_pSimManager->m_pSimConfig->SearchRunScriptByName(name);
			pScript->script = ui.edtScript->toPlainText().toStdString().data();
			pScript->run_times = atoi(ui.edtScriptTimes->text().toStdString().data());
			pScript->run_sepms = atoi(ui.edtScriptSepMs->text().toStdString().data());
		}
	}
	if(current != NULL)
	{
		if(current->data(0,Qt::UserRole).toInt() == 1)
		{
			//���ص�ǰ�Ľű�
			ui.edtScript->setEnabled(true);
			ui.edtScriptTimes->setEnabled(true);
			ui.edtScriptSepMs->setEnabled(true);
			ui.edtScriptTimes->setEnabled(true);
			ui.btnCheckScript->setEnabled(true);
			ui.btnStartScript->setEnabled(true);
			ui.btnStopScript->setEnabled(true);
			SString name = current->data(0,Qt::UserRole+1).toString().toStdString().data();
			stuSimRunScript *pScript = m_pSimManager->m_pSimConfig->SearchRunScriptByName(name);
			ui.edtScript->setPlainText(pScript->script.data());
			ui.edtScriptTimes->setText(SString::toString(pScript->run_times).data());
			ui.edtScriptSepMs->setText(SString::toString(pScript->run_sepms).data());
// 			if(pScript->rt_running)
// 				ui.labScriptStatus->setText(SString::toFormat("��%d������...",pScript->rt_times).data());
// 			else
// 				ui.labScriptStatus->setText("δ����");
		}
		else
		{
			ui.edtScript->setEnabled(false);
			ui.edtScriptTimes->setEnabled(false);
			ui.edtScriptSepMs->setEnabled(false);
			ui.edtScriptTimes->setEnabled(false);
			ui.btnCheckScript->setEnabled(false);
			ui.btnStartScript->setEnabled(false);
			ui.btnStopScript->setEnabled(false);
			ui.edtScript->setPlainText("");
			ui.edtScriptTimes->setText("");
			ui.edtScriptSepMs->setText("");
		}
	}
}


void panel_sim_root::treeItemPressed( QTreeWidgetItem *item, int column )
{
	if (qApp->mouseButtons() == Qt::RightButton && item != NULL) 
	{
		QMenu menu(this);
		if(item->data(0,Qt::UserRole).toInt() == 0)
		{
			menu.addAction(tr("�������нű�"),this,SLOT(OnSaveScript()));
			menu.addAction(tr("������нű�"),this,SLOT(OnNewScript()));
		}
		else
		{			
			//menu.addAction(tr("����"),this,SLOT(OnRenameScript()));
			//menu.addSeparator();
			menu.addAction(tr("������"),this,SLOT(OnRenameScript()));
			menu.addAction(tr("ɾ��"),this,SLOT(OnRemoveScript()));
			menu.addSeparator();
			menu.addAction(tr("�㲥�˽ű�"),this,SLOT(OnBcastScript()));
		}
		menu.exec(QCursor::pos());
	}
}


void panel_sim_root::OnSaveScript()
{
	m_pSimManager->m_pSimConfig->Save();
}

struct stuThreadRunScriptParam
{
	panel_sim_root *pThis;
	stuSimRunScript *pScript;
};

#include "dlg_fcda_filter.h"

void panel_sim_root::OnFcdaSearch()
{
	static dlg_fcda_filter *dlg = NULL;

	if(dlg == NULL)
	{
		dlg = new dlg_fcda_filter();
		dlg->m_pSimConfig = m_pSimManager->m_pSimConfig;
		dlg->m_pSimMmsServer = &m_pSimManager->m_MmsServer;
		dlg->m_pTextEdit = ui.edtScript;
	}
	dlg->show();
}

void panel_sim_root::OnScriptStart()
{
	QTreeWidgetItem *pItem = ui.twScripts->currentItem();
	if(pItem == NULL || pItem->data(0,Qt::UserRole).toInt() == 0)
		return;
	SString name = pItem->data(0,Qt::UserRole+1).toString().toStdString().data();
	stuSimRunScript *pScript = m_pSimManager->m_pSimConfig->SearchRunScriptByName(name);
	if(pScript == NULL)
		return;

	//����ǰһ��
	pScript->script = ui.edtScript->toPlainText().toStdString().data();
	pScript->run_times = atoi(ui.edtScriptTimes->text().toStdString().data());
	pScript->run_sepms = atoi(ui.edtScriptSepMs->text().toStdString().data());


	if(pScript->rt_running)
	{
		SQt::ShowInfoBox("��ʾ","��ǰ�ű�����������...");
		return;
	}

	CSimScriptParser sim;
	if(!sim.LoadSyntaxText(pScript->script.data()))
	{
		QTextBlock block = ui.edtScript->document()->findBlockByNumber(sim.GetErrorTextRow());  
		ui.edtScript->setTextCursor(QTextCursor(block));  
		SQt::ShowErrorBox("����",SString::toFormat("�޷����нű�!\r\n��%d�нű��﷨����!",sim.GetErrorTextRow()).data());
		return;
	}
	stuThreadRunScriptParam *pParam = new stuThreadRunScriptParam();
	pParam->pScript = pScript;
	pParam->pThis = this;
	pScript->rt_stop = false;
	pScript->rt_times = 0;
	pScript->rt_running = true;
	emit sigScriptStatus(pScript);
	SKT_CREATE_THREAD(ThreadRunScript,pParam);
}

void panel_sim_root::OnScriptEnd()
{
	QTreeWidgetItem *pItem = ui.twScripts->currentItem();
	if(pItem == NULL || pItem->data(0,Qt::UserRole).toInt() == 0)
		return;
	SString name = pItem->data(0,Qt::UserRole+1).toString().toStdString().data();
	stuSimRunScript *pScript = m_pSimManager->m_pSimConfig->SearchRunScriptByName(name);
	if(pScript == NULL)
		return;
	if(!pScript->rt_running)
	{
		SQt::ShowInfoBox("��ʾ","��ǰ�ű���δ����!");
		return;
	}
	pScript->rt_stop = true;
}

void panel_sim_root::OnScriptCheck()
{
	QTreeWidgetItem *pItem = ui.twScripts->currentItem();
	if(pItem == NULL || pItem->data(0,Qt::UserRole).toInt() == 0)
		return;
	SString name = pItem->data(0,Qt::UserRole+1).toString().toStdString().data();
	stuSimRunScript *pScript = m_pSimManager->m_pSimConfig->SearchRunScriptByName(name);
	if(pScript == NULL)
		return;
	CSimScriptParser sim;
	if(!sim.LoadSyntaxText(pScript->script.data()))
	{
		QTextBlock block = ui.edtScript->document()->findBlockByNumber(sim.GetErrorTextRow());  
		ui.edtScript->setTextCursor(QTextCursor(block));  
		SQt::ShowErrorBox("����",SString::toFormat("��%d�нű��﷨����!",sim.GetErrorTextRow()).data());
	}
	else
	{
		SQt::ShowInfoBox("��ʾ","�ű�У��ͨ��!");
	}
}


void panel_sim_root::OnScriptStatus(stuSimRunScript* pScript)
{
	int i,cnt = m_pRoot->childCount();
	for(i=0;i<cnt;i++)
	{
		QTreeWidgetItem *pItem = m_pRoot->child(i);
		SString name = pItem->data(0,Qt::UserRole+1).toString().toStdString().data();
		if(name == pScript->name)
		{
			if(pScript->rt_running)
			{
				if(pScript->rt_times == 0)
					pItem->setText(1,"׼������...");
				else
					pItem->setText(1,SString::toFormat("��%d������...",pScript->rt_times).data());
				pItem->setIcon(0,m_iconRun);
			}
			else
			{
				pItem->setText(1,"δ����");
				pItem->setIcon(0,m_iconStop);
			}
			break;
		}
	}
}


void* panel_sim_root::ThreadRunScript(void *lp)
{
	stuThreadRunScriptParam *pParam = (stuThreadRunScriptParam*)lp;
	panel_sim_root *pThis = (panel_sim_root*)pParam->pThis;
	stuSimRunScript *pScript = pParam->pScript;
	pScript->rt_running = true;
	pScript->rt_times = 0;
	CSimScriptParser sim;
	sim.SetSimManagerPtr(pThis->m_pSimManager);
	if(!sim.LoadSyntaxText(pScript->script.data()))
	{
		pScript->rt_running = false;
		SSP_GUI->SetStatusTip(SString::toFormat("�ű�'%s'�﷨�����޷����м���!",pScript->name.data()));
		delete pParam;
		return NULL;
	}
	int i,cnt = pScript->run_times;
	SScriptParser::stuValue RetValue;
	SString sMain = "main";
	pThis->m_iThreads ++;
	for(i=0;pThis->m_bStop == false && pScript->rt_stop==false && i<cnt;i++)
	{
		pScript->rt_times++;
		emit pThis->sigScriptStatus(pScript);
		sim.RunFunction(RetValue,sMain);
		if(i!=cnt-1)
			SApi::UsSleep(pScript->run_sepms*1000);
	}
	pScript->rt_running = false;
	pScript->rt_stop = true;
	emit pThis->sigScriptStatus(pScript);
	delete pParam;
	pThis->m_iThreads --;
	return NULL;
}

void panel_sim_root::OnNewScript()
{
	SString name;
	while(1)
	{
		name = SQt::GetInput("����","������ű�����","�½ű�");
		if(name.length() == 0)
			return;
		if(m_pSimManager->m_pSimConfig->SearchRunScriptByName(name) != NULL)
		{
			SQt::ShowErrorBox("����",SString::toFormat("'%s'�Ѵ��ڣ�����������!",name.data()).data());
			continue;
		}
		break;
	}
	stuSimRunScript *pScript = new stuSimRunScript();
	pScript->name = name;
	pScript->run_times = 1;
	pScript->run_sepms = 1000;
	pScript->rt_running = false;
	pScript->rt_times = 0;
	pScript->script =	
		"void main()\r\n"
		"{\r\n"
		"  //bool GooseChg(int appid,string vals);\r\n"
		"  //     ����һ��GOOSE��λ��valsΪch_name1=val1;ch_name2=val2;\r\n"
		"  //     val����Ϊtrue/false��Ҳ����Ϊnow(��ʾ��ǰʱ��) \r\n"
		"  //bool SvChg(int appid,int ch_idx,float rms,int q);\r\n"
		"  //     �޸�SVֵ��ch_idΪͨ����(��1��ʼ),rmsΪ��Чֵ,qΪƷ�� \r\n"
		"  //bool SetLeaf(string ied_ld,string leaf_path,int vt,string val);\r\n"
		"  //     �޸�LNֵ��ied_ldIED��LD���ƣ�leaf_pathΪҶ�ӽڵ�·��,\r\n"
		"  //     vtΪֵ����(1-bool,2-int,3-float,4-utctime)\r\n"
		"  //e.g. :\r\n"
		"  //      GooseChg(0x1001,\"�غ�բ=true;��A��=false;\");\r\n"
		"  //      SvChg(0x4001,2,1000,0);\r\n"
		"  //      $SetLeaf(\"CL2223CTRL\",\"GGIO26$ST$Alm1$stVal\",1,\"1\");\r\n"
		"  //      $SetLeaf(\"CL2223CTRL\",\"GGIO26$ST$Alm1$t\",4,\"now\");\r\n"
		"  //TODO:�����￪ʼ��д�ű�...\r\n"
		"}\r\n";
	m_pSimManager->m_pSimConfig->m_Scripts.append(pScript);

	//���
	QTreeWidgetItem *pItem = new QTreeWidgetItem();
	pItem->setData(0,Qt::UserRole,QVariant(1));
	pItem->setData(0,Qt::UserRole+1,QVariant(pScript->name.data()));
	pItem->setText(0,pScript->name.data());
	pItem->setToolTip(0,pScript->name.data());
	pItem->setIcon(0,m_iconStop);
	m_pRoot->addChild(pItem);
	ui.twScripts->setCurrentItem(pItem);
}

struct stuBcastScriptInfo
{
	int sn;//����������
	int idx;//��ǰ֡�ţ���0��ʼ
	int frames;//��֡��
	int len;//��Ч����
	int times;//���д���
	int sep_ms;//���м��
	char title[32];
	char text[1028];
};

SString g_BcastScriptIpAddr="255.255.255.255";
void panel_sim_root::OnBcastScript()
{
	QTreeWidgetItem *pItem = ui.twScripts->currentItem();
	if(pItem == NULL || pItem->data(0,Qt::UserRole).toInt() == 0)
		return;
	SString old_name = pItem->data(0,Qt::UserRole+1).toString().toStdString().data();
	stuSimRunScript *pScript = m_pSimManager->m_pSimConfig->SearchRunScriptByName(old_name);
	if(pScript == NULL)
		return;
	pScript->script = ui.edtScript->toPlainText().toStdString().data();
	pScript->run_times = atoi(ui.edtScriptTimes->text().toStdString().data());
	pScript->run_sepms = atoi(ui.edtScriptSepMs->text().toStdString().data());

	SString sBip = SQt::GetInput("�㲥�˽ű�","������㲥��ַ",g_BcastScriptIpAddr.data());
	if(sBip.length() == 0)
		return;

	stuBcastScriptInfo frame;
	int cnt = pScript->script.length()/1024;
	if(pScript->script.length()%1024 != 0)
		cnt ++;
	SSocket udp;
	if(!udp.CreateBroadCast())
	{
		SQt::ShowErrorBox("����","�����㲥UDPʧ��!");
		return ;
	}
	memset(&frame.title,0,sizeof(frame.title));
	strcpy(frame.title,pScript->name.left(30).data());
	frame.sn = rand();
	frame.frames = cnt;
	frame.times = pScript->run_times;
	frame.sep_ms = pScript->run_sepms;
	int pos = 0;
	int len = pScript->script.length();
	for(int i=0;i<cnt;i++)
	{
		frame.idx = i;
		frame.len = len;
		if(frame.len > 1024)
			frame.len = 1024;
		memset(frame.text,0,sizeof(frame.text));
		strcpy(frame.text,pScript->script.mid(pos,frame.len));
		pos += frame.len;
		len -= frame.len;
		int ret = udp.SendTo(sBip.data(),1421,&frame,sizeof(frame));
		if(ret != sizeof(frame))
		{
			SQt::ShowErrorBox("����","���͹㲥UDP����ʱʧ��!");
			break;
		}
		udp.SendTo(sBip.data(),1421,&frame,sizeof(frame));
		udp.SendTo(sBip.data(),1421,&frame,sizeof(frame));
	}
}


void* panel_sim_root::ThreadRunBcastScript(void *lp)
{
	panel_sim_root *pThis = (panel_sim_root*)lp;
	int sn=-1,idx,frames;
	SString sTitle,sScriptText;
	int iTimes;
	int iSepMs;
	stuBcastScriptInfo frame;
	SSocket udp;
	if(!udp.CreateUdp() || !udp.Bind(1421))
	{
		SSP_GUI->SetStatusTip("��UDP�˿�1421ʱʧ��!");
		return NULL;
	}
	udp.SetTimeout(2000,2000);
	pThis->m_iThreads ++;
	int i,ret;
	sockaddr_in sa;
	while(pThis->m_bStop == false)
	{
		ret = udp.RecvFrom(&frame,sizeof(frame),sa);
		if(ret != sizeof(frame))
		{
			SApi::UsSleep(10000);
			continue;
		}
		if(frame.sn == sn)
			continue;
		sn = frame.sn;
		idx = frame.idx;
		frames = frame.frames;
		sTitle = frame.title;
		iTimes = frame.times;
		iSepMs = frame.sep_ms;
		sScriptText = frame.text;
		if(idx != 0)
		{
			continue;
		}
		bool berr=false;
		for(i=idx+1;i<frames;i++)
		{
			//�������µ�֡
			ret = udp.RecvFrom(&frame,sizeof(frame),sa);
			if(ret != sizeof(frame))
			{
				berr = true;
				break;
			}
			if(frame.sn != sn || frame.idx != idx+1)
				continue;
			idx = frame.idx;
			sScriptText += frame.text;
		}
		if(berr)
		{
			continue;
		}
		SSP_GUI->SetStatusTip(SString::toFormat("�յ��㲥�ű�[%s]��%d֡!��ʼ����...",sTitle.data(),frames));
		CSimScriptParser sim;
		sim.SetSimManagerPtr(pThis->m_pSimManager);
		if(!sim.LoadSyntaxText(sScriptText.data()))
		{
			SSP_GUI->SetStatusTip(SString::toFormat("�㲥�ű�'%s'�﷨�����޷����м���!",sTitle.data()));
			continue;
		}
		SScriptParser::stuValue RetValue;
		SString sMain = "main";
		for(i=0;pThis->m_bStop==false&&i<iTimes;i++)
		{
			SSP_GUI->SetStatusTip(SString::toFormat("�㲥�ű�[%s]��%d������...",sTitle.data(),i+1));
			sim.RunFunction(RetValue,sMain);
			if(i!=iTimes-1)
				SApi::UsSleep(iSepMs*1000);
		}
		SSP_GUI->SetStatusTip(SString::toFormat("�㲥�ű�[%s]���н���!",sTitle.data()));
	}
	pThis->m_iThreads --;
	return NULL;
}

void panel_sim_root::OnRenameScript()
{
	QTreeWidgetItem *pItem = ui.twScripts->currentItem();
	if(pItem == NULL || pItem->data(0,Qt::UserRole).toInt() == 0)
		return;
	SString old_name = pItem->data(0,Qt::UserRole+1).toString().toStdString().data();
	SString name;
	while(1)
	{
		name = SQt::GetInput("����","������ű�����",old_name.data());
		if(name.length() == 0 || name == old_name)
			return;
		if(m_pSimManager->m_pSimConfig->SearchRunScriptByName(name) != NULL)
		{
			SQt::ShowErrorBox("����",SString::toFormat("'%s'�Ѵ��ڣ�����������!",name.data()).data());
			continue;
		}
		break;
	}
	stuSimRunScript *pScript = m_pSimManager->m_pSimConfig->SearchRunScriptByName(old_name);
	if(pScript == NULL)
		return;
	pScript->name = name;
	pItem->setData(0,Qt::UserRole+1,QVariant(pScript->name.data()));
	pItem->setText(0,pScript->name.data());
	pItem->setToolTip(0,pScript->name.data());
}
void panel_sim_root::OnRemoveScript()
{
	QTreeWidgetItem *pItem = ui.twScripts->currentItem();
	if(pItem == NULL || pItem->data(0,Qt::UserRole).toInt() == 0)
		return;
	SString name = pItem->data(0,Qt::UserRole+1).toString().toStdString().data();
	if(!SQt::ShowQuestionBox("��ʾ",SString::toFormat("��ȷ��Ҫɾ���ű�'%s'��?",name.data()).data()))
		return;
	stuSimRunScript *pScript = m_pSimManager->m_pSimConfig->SearchRunScriptByName(name);
	if(pScript == NULL)
		return;
	m_pRoot->removeChild(pItem);
	m_pSimManager->m_pSimConfig->m_Scripts.remove(pScript);
}

bool panel_sim_root::CommandToStart()
{
	ui.pushButton_Start->setEnabled(false);
	m_pSimManager->m_MmsServer.OnProgressText("��������SV��GOOSEģ����...");
	SQt::ProcessUiEvents();
	if(!m_pSimManager->Start())
	{
		//SQt::ShowErrorBox("����","����SV��GOOSEģ��������ʧ��!");
		m_pSimManager->m_MmsServer.OnProgressText("����SV��GOOSEģ��������ʧ��!");
		ui.pushButton_Start->setEnabled(true);
		return false;
	}
	m_pSimManager->m_MmsServer.m_pSimConfig = m_pSimManager->m_pSimConfig;
	if(m_pSimManager->m_pSimConfig->m_bOpenMmsServer && SFile::exists(SBASE_SAPP->GetConfPath()+"1.scd"))
	{
#ifndef WIN32
		if(m_pSimManager->m_pSimConfig->m_bMmsSvrSimIpAddr)
			m_pSimManager->m_MmsServer.AddApIpAddr();
#endif

		SString scd=SBASE_SAPP->GetConfPath()+"1.scd";
		int id=0;
#if 1
		unsigned long pos;
		stuSclVtIed *pIed = m_pSimManager->m_pSimConfig->m_MmsServerIed.FetchFirst(pos);
		while(pIed)
		{
			m_pSimManager->m_MmsServer.AddIed(stuMmsServerIed::New(scd,++id,pIed->name,"S1"));
			pIed = m_pSimManager->m_pSimConfig->m_MmsServerIed.FetchNext(pos);
		}
#else
		int ret = m_pSimManager->m_MmsServer.AddIedByScd(scd);
		if(ret <0)
			CGuiApplication::GetApp()->NewEvent("ϵͳ",0,0,"","��ʾ","����SCD�ļ�ʱʧ��!");
#endif
		CGuiApplication::GetApp()->NewEvent("ϵͳ",0,0,"","��ʾ",SString::toFormat("�ɹ�����%d��MMS���ʵ����!",id));

		m_pSimManager->m_MmsServer.Start();		
		CGuiApplication::GetApp()->NewEvent("ϵͳ",0,0,"","��ʾ",SString::toFormat("MMS�����������!"));
	}
	ui.pushButton_Stop->setEnabled(true);
	m_iThreads = 0;
	m_bStop = false;
	SKT_CREATE_THREAD(ThreadRunBcastScript,this);
	CGuiApplication::GetApp()->NewEvent("ϵͳ",0,0,"","��ʾ","ģ������������!");
	m_pSimManager->m_MmsServer.OnProgressText("ģ������������!");
	return false;
}

bool panel_sim_root::CommandToStop()
{
	m_bStop = true;
	if(!m_pSimManager->Stop())
	{
		//SQt::ShowErrorBox("����","ֹͣģ��������ʧ��!");
		return false;
	}
#ifndef WIN32
	m_pSimManager->m_MmsServer.RemoveApIpAddr();
#endif
	if(m_pSimManager->m_pSimConfig->m_bOpenMmsServer)
		m_pSimManager->m_MmsServer.StopAndWait();
	while(m_iThreads > 0)
		SApi::UsSleep(100000);
	ui.pushButton_Start->setEnabled(true);
	ui.pushButton_Stop->setEnabled(false);
	CGuiApplication::GetApp()->NewEvent("ϵͳ",0,0,"","��ʾ","ģ��������ֹͣ!");
	return true;
}