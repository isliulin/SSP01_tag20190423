#include "SNtpApplication.h"

SNtpApplication::SNtpApplication()
{

}
SNtpApplication::~SNtpApplication()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������񣬿�������Ӧ�õķ��񣬸ú���������������ʵ�֣�������������ɺ���뷵��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-1 13:34
// ����˵��:  void
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool SNtpApplication::Start()
{
	m_SntpFile.Load(GetConfPath()+"sys_ntp.xml");
	if(!m_SntpFile.Run(m_iArgc,m_ppArgv))
		Quit();//�˳�����
	return true;
}

bool SNtpApplication::Stop()
{
	return true;
}

bool SNtpApplication::ProcessAgentMsg(WORD wMsgType,stuSpUnitAgentMsgHead *pMsgHead,SString &sHeadStr,BYTE* pBuffer/*=NULL*/,int iLength/*=0*/)
{
	return false;
}