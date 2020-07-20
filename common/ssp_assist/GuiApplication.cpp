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
#include "GuiApplication.h"
CGuiApplication* g_pGuiApplication=NULL;

CGuiApplication::CGuiApplication(void)
{
	m_pMainWnd = NULL;
	m_pQApp = NULL;
	g_pGuiApplication = this;
	m_pDatabase = new CSsp_Database();			//���ݿ������ļ�
	m_pDatawindow = new CSsp_DatawindowMgr();	//���ݴ��������ļ�
	m_pFunpoint = new CSsp_FunPointConfig();	//���ܵ������ļ�
	m_pSvgLibCfg = new CSp_SvgLibConfig();
}


CGuiApplication::~CGuiApplication(void)
{
	if(m_pMainWnd != NULL)
	{
		delete m_pMainWnd;
		m_pMainWnd = NULL;
	}
	m_pDatawindow->Clear();
	Stop();	
	if(m_pDatawindow != NULL)
		delete m_pDatawindow;
	if(m_pDatabase != NULL)
		delete m_pDatabase;
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
// 	if(!SApplication::Start())
// 		return false;
	//TODO:�����������

	if(!m_pDatabase->Load(GetConfPath()+"sys_database.xml"))
	{
		LOGFAULT("���ݿ������ļ���ʧ��!file:%s", m_pDatabase->GetPathFile().data());
		SQt::ShowErrorBox(QObject::tr("����"),QObject::tr("���ݿ������ļ���ʧ��!���޷�ʹ�ò������߹���!"));
		//return false;
	}
	m_pDatawindow->SetDbOper(DB);
	m_pDatawindow->SetMdbOper(MDB);
	//if(!m_pDatawindow->Load(GetConfPath()+"sys_datawindow.xml"))
	if(!m_pDatawindow->LoadByDb())
	{
		LOGFAULT("���ݴ��������ļ���ʧ��!file:%s", m_pDatawindow->GetPathFile().data());
		//SQt::ShowErrorBox(QObject::tr("����"),QObject::tr("���ݴ��������ļ���ʧ��!"));
		//return false;
	}
#if 0
	if(!m_pFunpoint->LoadByDb(DB,""))
	{
		LOGFAULT("�����ݿ���ع��ܵ�����ʱʧ��!");
		SQt::ShowErrorBox(QObject::tr("����"),QObject::tr("�����ݿ���ع��ܵ�����ʱʧ��!"));
		return false;
	}
#else
	SString sText = "<?xml version='1.0' encoding='GBK'?>\r\n"
					"<fun_point key='' name='SSPƽ̨��������' desc='�����ܽڵ㣬��ʾϵͳ����'>\r\n"
					"	<fun_point key='code' name='��������' desc=''>\r\n"
					"		<fun_point key='code.db' name='���ݿ�����' desc='' >\r\n"
					"			<fun_point key='code.mysql.helper' name='MySQL���ݿ�����' desc='' />\r\n"
					"			<fun_point key='code.oracle.helper' name='Oracle���ݿ�����' desc='' />\r\n"
					"			<fun_point key='code.db.mdbsql' name='�ڴ��SQL�������' desc='' />\r\n"
					"		</fun_point>\r\n"
					"		<fun_point key='code.common' name='���ù���' desc='' >\r\n"
					"			<fun_point key='code.xml2saxex' name='XMLתSaxEx����' desc='' />\r\n"
					"			<fun_point key='code.file2cpp' name='�ļ�תC++���鶨��' desc='' />\r\n"
					"			<fun_point key='code.cpp2file' name='C++���鶨��ת�ļ�' desc='' />\r\n"
					"		</fun_point>\r\n"
					"	</fun_point>\r\n"
					"	<fun_point key='prj' name='�����Ŀ' desc=''>\r\n"
					"		<fun_point key='prj.helper' name='��Ŀ����' desc='' >\r\n"
					"			<fun_point key='prj.new.plugin' name='HMI�����Ŀ����' desc='' />\r\n"
					"			<fun_point key='prj.new.cmd' name='����̨��Ŀ����' desc='' />\r\n"
					"			<fun_point key='prj.new.gui' name='�˻�������Ŀ����' desc='' />\r\n"
					"		</fun_point>\r\n"
					"	</fun_point>\r\n"
					"\r\n"
					"\r\n"
					"\r\n"
					"	<fun_point key='hlp' name='����' desc=''>\r\n"
					"		<fun_point key='hlp.hlp' name='����' desc='' >\r\n"
					"			<fun_point key='sys.about' name='����' desc='' />\r\n"
					"		</fun_point>\r\n"
					"	</fun_point>\r\n"
					"</fun_point>";
	m_pFunpoint->SetFunPointXmlText(sText);
// 	if(!m_pFunpoint->Load(GetConfPath()+"sys_funpoint.xml"))
// 	{
// 		LOGFAULT("���ܵ������ļ���ʧ��!file:%s", m_pFunpoint->GetPathFile().data());
// 		SQt::ShowErrorBox(QObject::tr("����"),QObject::tr("���ܵ������ļ���ʧ��!"));
// 		return false;
// 	}
#endif
// 	if(!m_pSvgLibCfg->Load(GetConfPath()+"sys_svg_library.xml"))
// 	{
// 		LOGFAULT("SVGͼԪ�������ļ���ʧ��!file:%s", m_pSvgLibCfg->GetPathFile().data());
// 		SQt::ShowErrorBox(QObject::tr("����"),QObject::tr("SVGͼԪ�������ļ���ʧ��!"));
// 		//return false;
// 	}

	StartAgent(false);//�Է�������ʽ���д���ͻ���
	if(m_pMainWnd == NULL)
	{
		m_pMainWnd = new CGuiFrame(NULL);
		m_pMainWnd->SetStartSysName("SSP01ƽ̨��������V1.0.0");
		m_pMainWnd->Start();
	}

	m_pMainWnd->InitModule(m_pFunpoint->GetFunPointXmlText());
	m_pMainWnd->showMaximized();
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
