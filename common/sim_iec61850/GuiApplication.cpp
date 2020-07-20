/**
 *
 * �� �� �� : GuiApplication.cpp
 * �������� : 2015-7-30 15:55
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ͼ�ν���Ӧ�ó�����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30	�ۿ�������ļ�
 *
 **/
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include "GuiApplication.h"

CGuiApplication* g_pGuiApplication=NULL;

CGuiApplication::CGuiApplication(void)
{
	m_pMainWnd = NULL;
	m_pQApp = NULL;
	g_pGuiApplication = this;
	m_SimManager.m_pSimConfig = &m_SimConfig;
	SetApplicationId(101);
	m_pWndRealEvent = NULL;
	m_pDatabase = NULL;
#if 0 
	m_pDatabase = new CSsp_Database();			//���ݿ������ļ�
	m_pDatabase->SetPoolSize(2);
	//LoadLibrary("plugin_scl_importd.dll");
//�ֵ��� 20181112 ����й�SCD���빦��
	int db_pools = GetParamI("db_pools");
	if(db_pools == 0)
		db_pools = 10;
	if(db_pools >= 3 && db_pools < 100)
		m_pDatabase->SetPoolSize(db_pools);

	//�������ݿ�����
	if(!m_pDatabase->Load(GetConfPath()+"sys_database.xml"))
	{
		LOGFAULT("���ݿ������ļ���ʧ��!file:%s", m_pDatabase->GetPathFile().data());
		return;
	}
	//closeScdToMDB();
#endif
}


CGuiApplication::~CGuiApplication(void)
{
	if(m_pMainWnd != NULL)
	{
		delete m_pMainWnd;
		m_pMainWnd = NULL;
	}
	if(m_pWndRealEvent != NULL)
		delete m_pWndRealEvent;
	Stop();
	if(m_pFunpoint != NULL)
		delete m_pFunpoint;
	CConfigMgr::Quit();

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡAPPӦ��ʵ��ָ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 16:05
// ����˵��:  void
// �� �� ֵ:  CGuiApplication*
//////////////////////////////////////////////////////////////////////////
CGuiApplication* CGuiApplication::GetApp()
{
	return g_pGuiApplication;
}

QApplication* CGuiApplication::GetQApp()
{
	return g_pGuiApplication->m_pQApp;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �豸QTӦ����ָ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 16:05
// ����˵��:  @pApp
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CGuiApplication::SetQApplication(QApplication *pApp)
{
	m_pQApp = pApp;

	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������񣬿�������Ӧ�õķ��񣬸ú���������������ʵ�֣�������������ɺ���뷵��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 16:05
// ����˵��:  void
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CGuiApplication::Start()
{
	//TODO:�����������
	m_pFunpoint = new CSsp_FunPointConfig();	//���ܵ������ļ�

	SString sText = "<?xml version='1.0' encoding='GBK'?>\r\n"
					"<fun_point key='' name='UK-9000���վ���ܸ澯�������ܲ���ƽ̨' desc='�����ܽڵ㣬��ʾϵͳ����'>\r\n"
					"	<fun_point key='home' name='��ҳ' desc=''>\r\n"
					"		<fun_point key='home' name='��ҳ' desc='' >\r\n"
					"			<fun_point key='home' name='��ҳ' desc='' />\r\n"
					"		</fun_point>\r\n"
					"	</fun_point>\r\n"
					"	<fun_point key='mod' name='ģ�͹���' desc=''>\r\n"
					"		<fun_point key='mod.scd' name='SCLģ��' desc='' >\r\n"
					"			<fun_point key='mod.scd.edit' name='SCLģ�ͱ༭' desc='' />\r\n"
					"			<fun_point key='mod.scd.import' name='SCLģ�͵���' desc='' />\r\n"
					//"			<fun_point key='plugin_scl_import' name='SCDģ�͵���' desc='' />\r\n"
					"		</fun_point>\r\n"
					"		<fun_point key='mod.scd' name='ģ������' desc='' >\r\n"
					"			<fun_point key='pcap.replay' name='PCAP�ļ��ط�' desc='' />\r\n"
					"		</fun_point>\r\n"
					"	</fun_point>\r\n"
// 					"	<fun_point key='prj' name='ģ�͹���' desc=''>\r\n"
// 					"		<fun_point key='prj.helper' name='��Ŀ����' desc='' >\r\n"
// 					"			<fun_point key='prj.new.cmd' name='����̨��Ŀ����' desc='' />\r\n"
// 					"			<fun_point key='prj.new.gui' name='�˻�������Ŀ����' desc='' />\r\n"
// 					"		</fun_point>\r\n"
// 					"		<fun_point key='code.db' name='���ݿ��������' desc='' >\r\n"
// 					"			<fun_point key='code.db.mdbstu' name='�ڴ��C�ṹ����' desc='' />\r\n"
// 					"			<fun_point key='code.db.mdbsql' name='�ڴ��SQL�������' desc='' />\r\n"
// 					"		</fun_point>\r\n"
// 					"	</fun_point>\r\n"
					"\r\n"
					"\r\n"
					"\r\n"
					"	<fun_point key='vis' name='���ӻ�' desc=''>\r\n"
					"		<fun_point key='vis.vis' name='���ӻ�' desc='' >\r\n"
					"			<fun_point key='vis.ap' name='���ʵ�' desc='' />\r\n"
					"		</fun_point>\r\n"
					"	</fun_point>\r\n"
					"	<fun_point key='hlp' name='����' desc=''>\r\n"
					"		<fun_point key='hlp.hlp' name='����' desc='' >\r\n"
					"			<fun_point key='sys.about' name='����' desc='' />\r\n"
					"			<fun_point key='sys.help' name='����' desc='' />\r\n"
					"		</fun_point>\r\n"
					"	</fun_point>\r\n"
					"</fun_point>";
	m_pFunpoint->SetFunPointXmlText(sText);
	if(!m_SimConfig.Load(GetConfPath()+"sim_iec61850.xml"))
	{
		LOGERROR("���������ļ�ʱʧ��!");
		SQt::ShowErrorBox("��ʾ",SString::toFormat("���������ļ�(%ssim_iec61850.xml)ʱʧ��",GetConfPath().data()).data());
//		return false;
	}

	StartAgent(false);//�Է�������ʽ���д���ͻ���
	if(m_pMainWnd == NULL)
		m_pMainWnd = new CGuiFrame(NULL);
	m_pMainWnd->Start();
	m_pMainWnd->InitModule(m_pFunpoint->GetFunPointXmlText());
	m_pMainWnd->showMaximized();
	m_pWndRealEvent = new CFloatWnd_RealEvent();
#ifdef WIN32
	m_pWndRealEvent->setWindowFlags( Qt::Dialog | Qt::Tool | Qt::WindowStaysOnTopHint);
#else
	m_pWndRealEvent->setWindowFlags( Qt::Dialog );
#endif
	//m_pWndRealEvent->hide();
	m_pWndRealEvent->resize(1000,300);

	m_pMainWnd->SetStartTipText("�������!");
	m_pMainWnd->HideStartWnd();
	m_pQApp->exec();
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ֹͣ���񣬹ر�����Ӧ�õķ���
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 16:05
// ����˵��:  void
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CGuiApplication::Stop()
{
	SLog::quitLog();
	m_pQApp->quit();
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰģ�������״̬��Ϣ����Ӧ����ģ��Ӧ�г����н����豸��״̬��ϸ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 16:05
// ����˵��:  void
// �� �� ֵ:  SString
//////////////////////////////////////////////////////////////////////////
SString CGuiApplication::GetNowModInfo()
{
	return "";
}

void CGuiApplication::NewEvent(SString sEventType,int soc,int usec,SString sIedDesc,SString sEventLevel,SString sEventInfo)
{
	if(m_pWndRealEvent == NULL)
		return;
	stuRealEvent *pEvent = new stuRealEvent();
	pEvent->sEventType = sEventType;
	pEvent->soc = soc;
	pEvent->usec = usec;
	if(pEvent->soc == 0)
		SDateTime::getSystemTime(pEvent->soc,pEvent->usec);
	pEvent->sIedDesc = sIedDesc;
	pEvent->sEventLevel = sEventLevel;
	pEvent->sEventInfo = sEventInfo;
	
	m_pWndRealEvent->m_RealEvents.append(pEvent);
}

void InitConsoleWindow()
{
	int nCrt = 0;
	FILE* fp;
	AllocConsole();
	nCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	fp = _fdopen(nCrt, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
}

#include <QDebug>
void CGuiApplication::closeScdToMDB()
{
	SString sql="select count(*) as num from t_ssp_setup where set_key='scl.mdb'";
	int iNum = DB->SelectIntoI(sql);
	bool bRst = false;
	if(iNum == 0)
	{
		sql = "insert into t_ssp_setup(set_key,type_id,set_desc,set_default,set_value,set_dime,set_class,val_type,val_fmt) "
			"value('scl.mdb',1,'SCL�ļ���ͬ�������ڴ��(0��1��)','0','1','',0,3,'')";
		bRst = DB->ExecuteSQL(sql);
	} else
	{
		sql = "update t_ssp_setup set set_value='1' where set_key='scl.mdb'";//ǿ�ƽ��ڴ������ر�
		bRst = DB->ExecuteSQL(sql);
	}

	if (!bRst)
	{
		LOGERROR("���ݿ����ʧ�ܣ�%s!",sql.data());
	}
}