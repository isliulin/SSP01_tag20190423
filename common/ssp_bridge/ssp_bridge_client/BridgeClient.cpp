/**
 *
 * �� �� �� : BridgeSvr.cpp
 * �������� : 2018-10-12 16:38
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

#include "BridgeClient.h"

CBridgeClient::CBridgeClient()
{
	SetStartWord(0xE35B);
	m_TcpClientParams.setAutoDelete(true);
	m_TcpClientParams.setShared(true);
}

CBridgeClient::~CBridgeClient()
{

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �Է�����ʽ�����ͻ��ˣ�������Connect����ͬʱʹ��
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 22:05
// ����˵��:  @ip��ʾ�����IP��ַ��@port��ʾ�˿�
// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
//////////////////////////////////////////////////////////////////////////
bool CBridgeClient::Start(SString ip,int port,int outside_port,SString inside_ip,int inside_port)
{
// 	int brg_port = GetInput("����������Ҫ�Žӵ�TCP�˿ں�","2000").toInt();
// 	SString inner_ip = GetInput("����������Ҫ�Žӵ�������IP","127.0.0.1");
// 	int inner_port = GetInput("����������Ҫ�Žӵ�����TCP�˿ں�",SString::toString(brg_port)).toInt();
	m_sBrgIp = ip;
	m_iBrgPort = outside_port;
	m_sInnerIp = inside_ip;
	m_iInnerPort = inside_port;

	if(!STcpClientBase::Start(ip,port))
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ֹͣ����
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 22:05
// ����˵��:  void
// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
//////////////////////////////////////////////////////////////////////////

bool CBridgeClient::Stop()
{
	if(!STcpClientBase::Stop())
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��¼�Ϸ���˺�Ļص�
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 22:05
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CBridgeClient::OnLogin()
{
	stuSTcpPackage *pRecvPackage = NULL;
	SString sHead;
	sHead.sprintf("act=create_tcpsvr;port=%d;",m_iBrgPort);
	if(!SendAndRecv(pRecvPackage,sHead,PKG_CONTROL,NULL,0,3,PKG_CONTROL) || pRecvPackage == NULL || pRecvPackage->m_wFrameType != PKG_CONTROL)
	{
		LOGERROR("����TCP�Ž�ͨ��ʧ��!");
		return ;
	}
	sHead = pRecvPackage->m_sHead;
	if(SString::GetAttributeValue(sHead,"act") != "create_tcpsvr" || SString::GetAttributeValue(sHead,"result") != "1")
	{
		LOGERROR("�Žӷ��񴴽�ʧ��!�Ž�ͨ��Ϊ: %s:%d --> %s:%d",m_sBrgIp.data(),m_iBrgPort,m_sInnerIp.data(),m_iInnerPort);
		return ;
	}
	LOGDEBUG("�Žӷ��񴴽��ɹ�!�Ž�ͨ��Ϊ: %s:%d --> %s:%d",m_sBrgIp.data(),m_iBrgPort,m_sInnerIp.data(),m_iInnerPort);

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �Ự�߳̽��յ����ĺ�Ļص��麯����������ͨ���˺�������ʱ��Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 22:05
// ����˵��:  @pSessionΪ�Ựʵ��ָ�룬����ͨ��ָ����Զ˷�������
//            @pPackageΪ�ս��յ������ݰ�
// �� �� ֵ:  true��ʾ�Ѿ�������ϣ��˱��Ŀ����ͷ��ˣ�false��ʾδ�����˱���Ӧ������ն���
//////////////////////////////////////////////////////////////////////////
bool CBridgeClient::OnRecvFrame(stuSTcpPackage *pPackage)
{
	if(pPackage->m_wFrameType == PKG_CONTROL)
	{
		LOGDEBUG("�յ���������:%s",pPackage->m_sHead.data());
		SString act = SString::GetAttributeValue(pPackage->m_sHead,"act");
		if(act == "connect")
		{
			SString sHead;
			SSocket *pSock = new SSocket();
			pSock->CreateTcp();
			bool bConn = pSock->Connect(m_sInnerIp,m_iInnerPort);
			SString session_id = SString::GetAttributeValue(pPackage->m_sHead,"session_id");
			sHead.sprintf("act=connect;session_id=%s;result=%d;",session_id.data(),bConn?1:0);
			AddToSend(NewPackage(sHead,PKG_CONTROL));
			LOGWARN("�����Ž�Ŀ��(%s:%d)%s!���Ϳ�������:%s",m_sInnerIp.data(),m_iInnerPort,bConn?"�ɹ�":"ʧ��",sHead.data());
			if(!bConn)
			{
				delete pSock;
				return true;
			}
			stuTcpClientParam *p = new stuTcpClientParam();
			p->pSock = pSock;
			p->pThis = this;;
			p->session_id = session_id;
			SKT_CREATE_THREAD(ThreadTcpSvrInnerClient,p);
			m_TcpClientParams.append(p);
		}
		else if(act == "pipbroken")
		{
			SString session_id = SString::GetAttributeValue(pPackage->m_sHead,"session_id");
			m_TcpClientParams.lock();
			unsigned long pos;
			stuTcpClientParam *p = m_TcpClientParams.FetchFirst(pos);
			while(p)
			{
				if(p->session_id == session_id)
					break;
				p = m_TcpClientParams.FetchNext(pos);
			}
			m_TcpClientParams.unlock();
			if(p == NULL)
				return true;
			p->pSock->Close();
		}
	}
	else if(pPackage->m_wFrameType == PKG_DATA && pPackage->m_iAsduLen > 0)
	{
		//LOGDEBUG("�յ����ݰ�(len=%d):%s",pPackage->m_iAsduLen,SString::HexToStr(pPackage->m_pAsduBuffer,pPackage->m_iAsduLen>500?500:pPackage->m_iAsduLen).data());
		unsigned long pos;
		SString sHead;
		SString session_id = SString::GetAttributeValue(pPackage->m_sHead,"session_id");
		m_TcpClientParams.lock();
		stuTcpClientParam *p = m_TcpClientParams.FetchFirst(pos);
		while(p)
		{
			if(p->session_id == session_id)
				break;
			p = m_TcpClientParams.FetchNext(pos);
		}
		m_TcpClientParams.unlock();
		if(p == NULL)
			return true;
		int ret = p->pSock->Send(pPackage->m_pAsduBuffer,pPackage->m_iAsduLen);
		if(ret != pPackage->m_iAsduLen)
		{
			sHead.sprintf("act=pipbroken;session_id=%s;",p->session_id.data());
			LOGWARN("���Ϳ�������:%s",sHead.data());
			AddToSend(NewPackage(sHead,PKG_CONTROL));
		}
		p->in_bytes += pPackage->m_iAsduLen;
	}
	return true;
}

SString CBridgeClient::GetInput(SString tip,SString def)
{
	char buf[1025];
	if(tip.length() > 0)
	{
		printf(tip.data());
		if(def.length() > 0)
			printf("(%s)",def.data());
		printf(":");
	}
	memset(buf,0,sizeof(buf));
	gets(buf);
	SString str = buf;
	if(str.length() == 0)
		return def;
	return str;
}

void* CBridgeClient::ThreadTcpSvrInnerClient(void *lp)
{
	stuTcpClientParam *p = (stuTcpClientParam*)lp;
	CBridgeClient *pThis = p->pThis;
	SSocket *pSock = p->pSock;
	int ret;
	SString sHead;
	BYTE buf[1600];
	pSock->SetTimeout(10,5000);
	pThis->BeginThread();
	while(pThis->IsQuit() == false)
	{
		ret = pSock->CheckForRecv();
		if(ret == 0)
			continue;
		if(ret < 0)
		{
			sHead.sprintf("act=pipbroken;session_id=%s;",p->session_id.data());
			pThis->AddToSend(pThis->NewPackage(sHead,PKG_CONTROL));
			LOGWARN("���Ϳ�������:%s",sHead.data());
			break;
		}
		ret = pSock->ReceiveOnce(buf,1600);
		if(ret <= 0)
		{
			sHead.sprintf("act=pipbroken;session_id=%s;",p->session_id.data());
			pThis->AddToSend(pThis->NewPackage(sHead,PKG_CONTROL));
			LOGWARN("���Ϳ�������:%s",sHead.data());
			break;
		}
		sHead.sprintf("session_id=%s;",p->session_id.data());
		p->out_bytes += ret;
		pThis->AddToSend(pThis->NewPackage(sHead,PKG_DATA,buf,ret));
	}
	pThis->EndThread();
	pThis->m_TcpClientParams.remove(p);
	return NULL;
}