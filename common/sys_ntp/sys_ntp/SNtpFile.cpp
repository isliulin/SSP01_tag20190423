#include "SNtpFile.h"


SNtpFile::SNtpFile()
{
	m_SntpClient = new SNtpClient();
	m_SntpServer = new SNtpServer();
}

SNtpFile::~SNtpFile()
{
	delete m_SntpClient;
	delete m_SntpServer;
}

bool SNtpFile::Load(SString sPathFile)
{
	//bool a=CConfigBase::Load(sPathFile);
	SXmlConfig xml;
	if(!xml.ReadConfig(sPathFile))
	{
		LOGFAULT("�������ļ�[%s]ʱʧ��!�����ļ��Ƿ�����Ҹ�ʽ�Ƿ���ȷ!\n",sPathFile.data());
		exit(0);
		return false;
	}
	LOGDEBUG("��ʼ��ȡ����!\n");

	m_Lock.lock();
	SBaseConfig *pHd = xml.SearchChild("server");
	m_SntpServer->SetServer(pHd->GetAttribute("ip"), pHd->GetAttributeI("port"));
	if(pHd != NULL && pHd->GetAttribute("open").toLower() == "true")
		m_bOpenClient = true;
	else
		m_bOpenClient = false;
	pHd = xml.SearchChild("client");
	m_SntpClient->SetServer(pHd->GetAttribute("ip"), pHd->GetAttributeI("port"));
	if(pHd != NULL && pHd->GetAttribute("open").toLower() == "true")
		m_bOpenServer = true;
	else
		m_bOpenServer = false;
	m_iTimeSecs = pHd->GetAttributeI("m_iTimeSecs");
	
	LOGDEBUG("��ȡ�����ļ��ɹ�\n");

	m_Lock.unlock();
	return true;
}

bool SNtpFile::Run(int argc, char* argv[])
{
	if(this->m_bOpenClient)
	{
		if(this->m_SntpServer->StartServer(false))
		{
			LOGDEBUG("����ntp����˳ɹ���\n");
		}
		else
		{
			LOGERROR("����ntp�����ʧ�ܣ���رճ�������������\n");
			return false;
		}
	}
	S_CREATE_THREAD(ThreadStart,this);
	return true;
}



void* SNtpFile::ThreadStart(void *lp)
{
	SNtpFile *pThis = (SNtpFile*)lp;
	while(pThis->m_bOpenServer)
	{
		if(!pThis->m_SntpClient->GetTime(pThis->m_Now))
			LOGERROR("��ȡ��ǰ��������ʱ��ʧ�ܣ�\n");
		for(int i=0;i<1000000;++i)
			SApi::UsSleep(pThis->m_iTimeSecs);//windowsƽ̨���ܾ�ȷ��1000΢�룬����Ϊ1΢��ʱ������1000΢��
	}
	return NULL;
}