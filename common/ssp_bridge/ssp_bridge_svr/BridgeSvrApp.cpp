/**
 *
 * �� �� �� : BridgeSvrApp.cpp
 * �������� : 2018-10-12 21:44
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2018-10-12	�ۿ�������ļ�
 *
 **/

#include "BridgeSvrApp.h"

CBridgeSvrApp::CBridgeSvrApp()
{

}

CBridgeSvrApp::~CBridgeSvrApp()
{

}

bool CBridgeSvrApp::Start()
{
	int tcp_port = 0;
	if(this->IsParam("port"))
		tcp_port = this->GetParamI("port");
	if(tcp_port == 0)
		tcp_port = 6001;
	LOGDEBUG("�ŽӶ˿�Ϊ:%d, �����Ҫ���Ķ˿ڣ�����ʱ�Ӳ���: -port=�˿ں�",tcp_port);
	m_Server.Start(tcp_port);
	return true;
}

bool CBridgeSvrApp::Stop()
{
	m_Server.Stop();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ı�����
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-13 15:12
// ����˵��:  @sCmdΪ�������ݣ����ַ�����ʾȡ�������б�
//         :  @sResultΪ���ؽ�������ı�
// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool CBridgeSvrApp::ProcessTxtCmd(SString &sCmd,SString &sResult)
{
	SString sText;
//	int i;
	if(sCmd == "" || sCmd == " ")
	{
		sResult = "cmd                - help\r\n"
				  "cmd info           - client information\r\n"
				  "cmd units          - unit information\r\n"
				  "cmd procs          - process infomation\r\n";
	}
	if(sCmd == "info")
	{
// 		sResult += SString::toFormat("��%d�����ݿ�ͻ���������!\r\n", m_MdbService.GetSessionCount());
// 		for (i = 0; i < m_MdbService.GetSessionCount(); i++)
// 		{
// 			STcpServerSession *p = m_MdbService.GetSession(i);
// 			if (p == NULL)
// 				continue;
// 			sResult += SString::toFormat("    ��%d������:%s:%d -> MDB\r\n", i + 1, p->GetSessionSock()->GetPeerIp().data(), p->GetSessionSock()->GetPeerPort());
// 		}
// 		sResult += m_MdbService.GetMdbMgrPtr()->GetMdbMgrInfo();
	}

	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ������Ϣ����ӿڣ���������ʵ�֣����������뾡����С�����ٷ���
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-3 15:20
// ����˵��:  @wMsgType��ʾ��Ϣ����
//         :  @pMsgHeadΪ��Ϣͷ
//         :  @sHeadStr��Ϣͷ�ַ���
//         :  @pBufferΪ��ϢЯ���Ķ������������ݣ�NULL��ʾ�޶���������
//         :  @iLengthΪ���������ݳ���
// �� �� ֵ:  true��ʾ����ɹ���false��ʾ����ʧ�ܻ�δ����
//////////////////////////////////////////////////////////////////////////
bool CBridgeSvrApp::ProcessAgentMsg(WORD wMsgType,stuSpUnitAgentMsgHead *pMsgHead,SString &sHeadStr,BYTE* pBuffer,int iLength)
{
	return false;
}