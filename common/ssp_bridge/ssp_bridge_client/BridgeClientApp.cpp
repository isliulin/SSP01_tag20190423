/**
 *
 * �� �� �� : BridgeClientApp.cpp
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

#include "BridgeClientApp.h"

CBridgeClientApp::CBridgeClientApp()
{

}

CBridgeClientApp::~CBridgeClientApp()
{

}

void usage()
{
	printf("\n");
	printf("Usage: ssp_bridge_client -conf=XML�����ļ���(ʹ�����������Ž�����)\n");
	printf("                         -bridge_ip=<�Žӷ�������IP>(ʹ��ָ������,��ͬ,������confͬʱʹ��)\n");
	printf("                         -bridge_port=<�Žӷ������Ķ˿�>\n");
	printf("                         -outside_port=<����������ŽӶ˿�>\n");
	printf("                         -inside_ip=<�ͻ��˲���Ž�IP>\n");
	printf("                         -inside_port=<�ͻ��˲���ŽӶ˿�>\n");
	printf("\n");
	SApi::UsSleep(1000000);
}
bool CBridgeClientApp::Start()
{
	SString sConf = GetParam("conf");
	if(sConf.length() > 0)
	{
		if(!SFile::exists(sConf))
		{
			LOGERROR("conf��Ӧ�����ļ�[%s]������!",sConf.data());
			usage();
			return false;
		}
		SXmlConfig xml;
		if(!xml.ReadConfig(sConf))
		{
			LOGERROR("�����ļ�[%s]��ʽ��Ч!",sConf.data());
			usage();
			return false;
		}
		SBaseConfig *tcp_bridges = xml.SearchChild("tcp_bridges");
		int i,cnt = tcp_bridges->GetChildCount("bridge");
		if(tcp_bridges == NULL || cnt <= 0)
		{
			LOGERROR("�����ļ�[%s]ȱ����Ч��������:tcp_bridges->bridge!",sConf.data());
			usage();
			return false;
		}
		m_Clients.setAutoDelete(true);
		for(i=0;i<cnt;i++)
		{
			SBaseConfig *bridge = tcp_bridges->GetChildNode(i,"bridge");
			if(bridge == NULL)
				continue;
			SString sBridgeIp = bridge->GetAttribute("bridge_ip");
			int iBridgePort = bridge->GetAttributeI("bridge_port");
			int outside_port = bridge->GetAttributeI("outside_port");
			SString inside_ip = bridge->GetAttribute("inside_ip");
			int inside_port = bridge->GetAttributeI("inside_port");
			SString desc = bridge->GetAttribute("desc");
			CBridgeClient *pClient = new CBridgeClient();
			m_Clients.append(pClient);
			pClient->SetConnectMode(true);
			pClient->m_sName = desc;
			if(!pClient->Start(sBridgeIp,iBridgePort,outside_port,inside_ip,inside_port))
			{
				LOGERROR("�޷����ӷ�����%s:%d!",sBridgeIp.data(),iBridgePort);				
			}
		}
		ProcCmd();
	}
	SString sBridgeIp = this->GetParam("bridge_ip");
	int iBridgePort = this->GetParamI("bridge_port");

	int outside_port = this->GetParamI("outside_port");
	SString inside_ip = this->GetParam("inside_ip");
	int inside_port = this->GetParamI("inside_port");

	if(sBridgeIp.length() == 0)
	{
		LOGERROR("ȱ����Ч�Ĳ���:bridge_ip!");
		usage();
		return false;
	}
	if(iBridgePort == 0)
	{
		LOGERROR("ȱ����Ч�Ĳ���:bridge_port!");
		usage();
		return false;
	}

	if(outside_port == 0)
	{
		LOGERROR("ȱ����Ч�Ĳ���:outside_port!");
		usage();
		return false;
	}
	if(inside_ip.length() == 0)
	{
		LOGERROR("ȱ����Ч�Ĳ���:inside_ip!");
		usage();
		return false;
	}
	if(inside_port == 0)
	{
		LOGERROR("ȱ����Ч�Ĳ���:inside_port!");
		usage();
		return false;
	}
	CBridgeClient *pClient = new CBridgeClient();
	m_Clients.append(pClient);
	pClient->SetConnectMode(true);
	pClient->m_sName = "";
	if(!pClient->Start(sBridgeIp,iBridgePort,outside_port,inside_ip,inside_port))
	{
		LOGERROR("�޷����ӷ�����%s:%d!",sBridgeIp.data(),iBridgePort);				
	}

// 	m_Client.SetConnectMode(true);
// 	if(!m_Client.Start(sBridgeIp,iBridgePort,outside_port,inside_ip,inside_port))
// 	{
// 		LOGERROR("�޷����ӷ�����%s:%d!",sBridgeIp.data(),iBridgePort);
// 		//return false;
// 	}
	ProcCmd();
	return true;
}

void CBridgeClientApp::ProcCmd()
{
	SString cmd;
	while(1)
	{
		cmd = CBridgeClient::GetInput();
		if(cmd == "")
		{
			printf("list   -�鿴��ǰ�Ž�ͨ��\n");
		}
		else if(cmd == "list")
		{
			int i,cnt = m_Clients.count();
			for(i=0;i<cnt;i++)
			{
				CBridgeClient *pClient = m_Clients[i];
				if(pClient == NULL)
					continue;
				printf("�Ž�ͨ��( %s:%d -> %s:%d  %s) ��Ự��:%d \n",
					pClient->m_sBrgIp.data(),pClient->m_iBrgPort,
					pClient->m_sInnerIp.data(),pClient->m_iInnerPort,
					pClient->m_sName.data(),pClient->m_TcpClientParams.count());

				int j,cnt2 = pClient->m_TcpClientParams.count();
				for(j=0;j<cnt2;j++)
				{
					CBridgeClient::stuTcpClientParam *p = pClient->m_TcpClientParams[j];
					if(p == NULL)
						continue;
					printf("\tidx:%02d session_id:%s in_bytes:%u out_bytes:%u \n",
						j+1,p->session_id.data(),p->in_bytes,p->out_bytes);
				}
			}
		}
	}
}

bool CBridgeClientApp::Stop()
{
	//m_Client.Stop();
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
bool CBridgeClientApp::ProcessTxtCmd(SString &sCmd,SString &sResult)
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
bool CBridgeClientApp::ProcessAgentMsg(WORD wMsgType,stuSpUnitAgentMsgHead *pMsgHead,SString &sHeadStr,BYTE* pBuffer,int iLength)
{
	return false;
}

