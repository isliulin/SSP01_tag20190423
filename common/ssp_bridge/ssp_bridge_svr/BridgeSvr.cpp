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

#include "BridgeSvr.h"

CSession::CSession(STcpServerSession *pServerSession)
{
	m_pSession = pServerSession;
	m_ChannelType = CHN_NONE;
	m_TcpSvrParams.setAutoDelete(true);
	m_TcpSvrParams.setShared(true);
}

CSession::~CSession()
{

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������,�麯�������������б����ȵ��ô˺���
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 18:38
// ����˵��:  void
// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
//////////////////////////////////////////////////////////////////////////
bool CSession::Start()
{
	if(!SService::Start())
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ֹͣ�������������б����ȵ��ô˺���
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 18:38
// ����˵��:  void
// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
//////////////////////////////////////////////////////////////////////////
bool CSession::Stop()
{
	if(!SService::Stop())
		return false;
	m_ListenSock.Close();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����TCP����ͨ��
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 18:38
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
bool CSession::CreateTcpSvr(SString &sExtAttr)
{
	int port = SString::GetAttributeValueI(sExtAttr,"port");
	if(!m_ListenSock.CreateTcp() || !m_ListenSock.Bind(port) || !m_ListenSock.Listen())
	{
		LOGERROR("��Ч����TCP�˿�:%d",port);
		return false;
	}
	LOGDEBUG("�ڶ˿�%d�����Ž�ͨ���ɹ�!",port);
	m_ChannelType = CHN_TCP_SERVER;
	SKT_CREATE_THREAD(ThreadTcpSvrListen,this);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ӿͻ��˽��յ������ݰ�
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 20:51
// ����˵��:  @pPaakage
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSession::ProcessClientPkg(stuSTcpPackage *pPackage)
{
	if(pPackage->m_wFrameType == PKG_CONTROL)
	{
		//��������
		LOGDEBUG("�յ���������:%s",pPackage->m_sHead.data());
		SString act = SString::GetAttributeValue(pPackage->m_sHead,"act");
		if(act == "create_tcpsvr")
		{
			bool ret = CreateTcpSvr(pPackage->m_sHead);
			SString sHead;
			sHead.sprintf("act=create_tcpsvr;result=%d;",ret?1:0);
			m_pSession->AddToSend(m_pSession->NewPackage(sHead,PKG_CONTROL));
			LOGWARN("���Ϳ�������:%s",sHead.data());
		}
		else if(act == "pipbroken")
		{
			SString session_id = SString::GetAttributeValue(pPackage->m_sHead,"session_id");
			unsigned long pos;
			m_TcpSvrParams.lock();
			stuTcpSvrParam *p = m_TcpSvrParams.FetchFirst(pos);
			while(p)
			{
				if(p->session_id == session_id)
					break;
				p = m_TcpSvrParams.FetchNext(pos);
			}
			m_TcpSvrParams.unlock();
			if(p == NULL)
				return;
			p->pSessionSock->Close();
		}
	}
	else if(pPackage->m_wFrameType == PKG_DATA && pPackage->m_pAsduBuffer != NULL)
	{
		//LOGDEBUG("�յ����ݰ�(len=%d):%s",pPackage->m_iAsduLen,SString::HexToStr(pPackage->m_pAsduBuffer,pPackage->m_iAsduLen>500?500:pPackage->m_iAsduLen).data());
		SString session_id = SString::GetAttributeValue(pPackage->m_sHead,"session_id");
		if(m_ChannelType == CHN_TCP_SERVER)
		{
			unsigned long pos;
			m_TcpSvrParams.lock();
			stuTcpSvrParam *p = m_TcpSvrParams.FetchFirst(pos);
			while(p)
			{
				if(p->session_id == session_id)
					break;
				p = m_TcpSvrParams.FetchNext(pos);
			}
			m_TcpSvrParams.unlock();
			if(p == NULL)
				return;
			int ret = p->pSessionSock->Send(pPackage->m_pAsduBuffer,pPackage->m_iAsduLen);
			if(ret != pPackage->m_iAsduLen)
			{
				SString sHead;
				sHead.sprintf("act=pipbroken;session_id=%ld;",(long)p->pSessionSock);
				m_pSession->AddToSend(m_pSession->NewPackage(sHead,PKG_CONTROL));
				LOGWARN("���Ϳ�������:%s",sHead.data());
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  TCPת������ͨ�������߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 19:05
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
void* CSession::ThreadTcpSvrListen(void *lp)
{
	CSession *pThis = (CSession*)lp;
	SSocket *pSock = NULL;
	SString sHead;
	pThis->BeginThread();
	while(pThis->IsQuit() == false)
	{
		if(pSock == NULL)
			pSock = new SSocket();
		if(!pThis->m_ListenSock.Accept(*pSock))
			break;
		stuTcpSvrParam *p = new stuTcpSvrParam();
		pThis->m_TcpSvrParams.append(p);
		p->pThis = pThis;
		p->pSessionSock = pSock;
		p->session_id.sprintf("%ld",(long)p->pSessionSock);
		pSock = NULL;
		sHead.sprintf("act=connect;session_id=%ld;",(long)p->pSessionSock);
		pThis->m_pSession->AddToSend(pThis->m_pSession->NewPackage(sHead,PKG_CONTROL));
		LOGWARN("���Ϳ�������:%s",sHead.data());
		stuSTcpPackage *pRecvPackage = NULL;
		for(int i=0;i<500;i++)
		{
			SApi::UsSleep(10000);
			pRecvPackage = pThis->m_pSession->SearchRecvPackage("connect");
			if(pRecvPackage != NULL)
				break;
		}
		if(pRecvPackage == NULL || SString::GetAttributeValueI(pRecvPackage->m_sHead,"result") != 1)
		{
			LOGWARN("�޷����ӵ�ָ�����Ž�Ŀ��!ͨ������ʧ��!");
			continue;
		}
		
		SKT_CREATE_THREAD(ThreadTcpSvrSession,p);
		SApi::UsSleep(100000);
	}
	pThis->EndThread();
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  TCPת������ͨ���շ��߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 19:05
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
void* CSession::ThreadTcpSvrSession(void *lp)
{
	stuTcpSvrParam *p = (stuTcpSvrParam*)lp;
	CSession *pThis = p->pThis;
	SSocket *pSock = p->pSessionSock;
	pSock->SetTimeout(10,5000);
	int ret;
	BYTE buf[1600];
	SString sHead;
	pThis->BeginThread();
	while(pThis->IsQuit() == false)
	{
		//ת������
		ret = pSock->CheckForRecv();
		if(ret == 0)
			continue;
		if(ret < 0)
		{
			//������ͨ���жϣ���Ҫת��ͨ���жϸ�������
			sHead.sprintf("act=pipbroken;session_id=%ld;",(long)pSock);
			pThis->m_pSession->AddToSend(pThis->m_pSession->NewPackage(sHead,PKG_CONTROL));
			LOGWARN("���Ϳ�������:%s",sHead.data());
		}
		ret = pSock->ReceiveOnce(buf,1600);
		if(ret <= 0)
		{
			//������ͨ���жϣ���Ҫת��ͨ���жϸ�������
			sHead = "act=pipbroken;";
			sHead.sprintf("act=pipbroken;session_id=%ld;",(long)pSock);
			pThis->m_pSession->AddToSend(pThis->m_pSession->NewPackage(sHead,PKG_CONTROL));
			LOGWARN("���Ϳ�������:%s",sHead.data());
			break;
		}
		//���յ������ݷ���ȥ
		sHead.sprintf("session_id=%s;",p->session_id.data());
		pThis->m_pSession->AddToSend(pThis->m_pSession->NewPackage(sHead,PKG_DATA,buf,ret));
	}
	pThis->EndThread();
	pThis->m_TcpSvrParams.remove(p);
	return NULL;
}

CBridgeSvr::CBridgeSvr()
{
	SetStartWord(0xE35B);
}

CBridgeSvr::~CBridgeSvr()
{

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��¼�ص��麯��
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 16:37
// ����˵��:  @ip��¼�ͻ��˵�IP
//            @port��¼�ͻ��˵Ķ˿�
//            @sLoginHead��¼�ַ���
// �� �� ֵ:  true��ʾ�����¼,false��ʾ�ܾ���¼
//////////////////////////////////////////////////////////////////////////
bool CBridgeSvr::OnLogin(SString ip,int port,SString &sLoginHead)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �»Ự�ص��麯��
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 16:37
// ����˵��:  @pSessionΪ�´����ĻỰʵ��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CBridgeSvr::OnNewSession(STcpServerSession *pSession)
{
	CSession *p = new CSession(pSession);
	p->Start();
	pSession->SetUserPtr(p);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ���Ự�Ļص��麯��
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 16:37
// ����˵��:  @pSessionΪ������ɾ���ĻỰʵ��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CBridgeSvr::OnDeleteSession(STcpServerSession *pSession)
{
	CSession *p = (CSession*)pSession->GetUserPtr();
	if(p != NULL)
	{
		p->StopAndWait(10);
		pSession->SetUserPtr(NULL);
		delete p;
	}
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �Ự�߳̽��յ����ĺ�Ļص��麯����������ͨ���˺�������ʱ��Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2018-10-12 16:37
// ����˵��:  @pSessionΪ�Ựʵ��ָ�룬����ͨ��ָ����Զ˷�������
//            @pPackageΪ�ս��յ������ݰ�
// �� �� ֵ:  true��ʾ�Ѿ�������ϣ��˱��Ŀ����ͷ��ˣ�false��ʾδ�����˱���Ӧ������ն���
//////////////////////////////////////////////////////////////////////////
bool CBridgeSvr::OnRecvFrame(STcpServerSession *pSession,stuSTcpPackage *pPackage)
{
	((CSession*)pSession->GetUserPtr())->ProcessClientPkg(pPackage);

	if(pPackage->m_wFrameType == PKG_CONTROL && SString::GetAttributeValue(pPackage->m_sHead,"act") == "connect")
		return false;
	return true;
}

