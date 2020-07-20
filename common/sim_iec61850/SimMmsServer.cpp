/**
 *
 * �� �� �� : SimMmsServer.h
 * �������� : 2016-9-2 14:27
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : MMS�����ʵ���࣬ʹ��SMmsServer��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2016-9-2	�ۿ�������ļ�
 *
 **/
#include "SimMmsServer.h"
#include <math.h>

CSimMmsServer::CSimMmsServer()
{

}

CSimMmsServer::~CSimMmsServer()
{

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������,�麯�������������б����ȵ��ô˺���
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-2 14:27
// ����˵��:  void
// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
//////////////////////////////////////////////////////////////////////////
bool CSimMmsServer::Start()
{
	if(!SMmsServer::Start())
		return false;
	//SKT_CREATE_THREAD(ThreadSim,this);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ӳ��Ҷ�ӽڵ��û�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-2 14:27
// ����˵��:  @sIedLdName��ʾIED+LD����
//         :  @sLeafPath��ʾҶ�ӽڵ�·��
//         :  @ppLeafMap��ʾҶ�ӽڵ���û�ӳ��ָ��ĵ�ַ��ӳ��ָ��Ϊ�գ��û�������Ƿ���Ҫӳ�䣬��Ҫʱ����stuLeafMap������дָ�룬MMS�����ڲ��������ͷ�
// �� �� ֵ:  true��ʾ�ɹ�ӳ�䣬false��ʾ�޷�ӳ��
//////////////////////////////////////////////////////////////////////////
bool CSimMmsServer::OnMapLeafToUser(char* sIedLdName, char* sLeafPath, stuLeafMap **ppLeafMap)
{
	if(strstr(sLeafPath,"$stVal") != NULL || strstr(sLeafPath,"$t") != NULL)
	{
		*ppLeafMap = new stuLeafMap();
		m_StateLeafMap.append(*ppLeafMap);
		return true;
	}
	else if(strstr(sLeafPath,"$f") != NULL)
	{
		*ppLeafMap = new stuLeafMap();
		m_AnalogLeafMap.append(*ppLeafMap);
		return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ӷ��ʵ�IP��ַ
// ��    ��:  �ۿ���
// ����ʱ��:  2016-11-12 14:42
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSimMmsServer::AddApIpAddr()
{
	unsigned long pos,pos2,pos3;
	int idx = 10;
	SString local_netcard;
	stuSclVtSubNetwork *p = m_pSimConfig->m_VirtualTerminal.m_SubNetwork.FetchFirst(pos);
	while(p)
	{
		local_netcard = SString::GetAttributeValue(m_pSimConfig->m_sMmsExtAttribute,p->name);
		if(strstr(p->type,"MMS") != NULL && local_netcard.length() > 0)
		{			
			stuSclVtSubNetworkConnectAP *pAP = p->m_ConnectAP.FetchFirst(pos2);
			while(pAP)
			{
				stuSclVtSubNetworkConnectAPAddress *pAddr = pAP->m_Address.FetchFirst(pos3);
				while(pAddr)
				{
					if(strlen(pAddr->ip) > 0 && strlen(pAddr->ip_subnet) > 0)
					{
						::system(SString::toFormat("ifconfig %s:%d %s netmask %s",local_netcard.data(),idx++,pAddr->ip,pAddr->ip_subnet).data());
					}
					pAddr = pAP->m_Address.FetchNext(pos3);
				}
				pAP = p->m_ConnectAP.FetchNext(pos2);
			}
		}
		p = m_pSimConfig->m_VirtualTerminal.m_SubNetwork.FetchNext(pos);
	}
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ�����ʵ�IP
// ��    ��:  �ۿ���
// ����ʱ��:  2016-11-12 14:42
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSimMmsServer::RemoveApIpAddr()
{
	unsigned long pos,pos2,pos3;
	int idx = 10;
	SString local_netcard;
	stuSclVtSubNetwork *p = m_pSimConfig->m_VirtualTerminal.m_SubNetwork.FetchFirst(pos);
	while(p)
	{
		local_netcard = SString::GetAttributeValue(m_pSimConfig->m_sMmsExtAttribute,p->name);
		if(strstr(p->type,"MMS") != NULL && local_netcard.length() > 0)
		{			
			stuSclVtSubNetworkConnectAP *pAP = p->m_ConnectAP.FetchFirst(pos2);
			while(pAP)
			{
				stuSclVtSubNetworkConnectAPAddress *pAddr = pAP->m_Address.FetchFirst(pos3);
				while(pAddr)
				{
					if(strlen(pAddr->ip) > 0 && strlen(pAddr->ip_subnet) > 0)
					{
						::system(SString::toFormat("ifconfig %s:%d down",local_netcard.data(),idx++).data());
					}
					pAddr = pAP->m_Address.FetchNext(pos3);
				}
				pAP = p->m_ConnectAP.FetchNext(pos2);
			}
		}
		p = m_pSimConfig->m_VirtualTerminal.m_SubNetwork.FetchNext(pos);
	}
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ģ���̣߳���ʱ�����źŻ����ֵ�ı仯
// ��    ��:  �ۿ���
// ����ʱ��:  2016-9-2 14:39
// ����˵��:  @lp
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void* CSimMmsServer::ThreadSim(void *lp)
{
	CSimMmsServer* pThis = (CSimMmsServer*)lp;
	unsigned long pos;
	int st=0;
	float f;
	char *ld_name;
	int soc,usec;
	pThis->BeginThread();
	SApi::UsSleep(60000000);
	while(!pThis->IsQuit())
	{
		st = st==0?1:0;
		int cnt = 0;
		if(pThis->m_pSimConfig->m_iMmsStTimes > 0)
		{
			stuLeafMap *pLeaf = pThis->m_StateLeafMap.FetchFirst(pos);
			while(!pThis->IsQuit() && pLeaf && cnt < pThis->m_pSimConfig->m_iMmsStTimes)
			{
				//if(rand()%10 == 0 && cnt < pThis->m_pSimConfig->m_iMmsStTimes /*&& strstr(pThis->GetLeafMapIedLdName(pLeaf),"PT1001A") != NULL*/)
				{
					SDateTime::getSystemTime(soc,usec);
					if(strstr(pThis->GetLeafMapLeaf(pLeaf),"$t")!= NULL)
					{
						pThis->NewReportPush(pLeaf,soc,usec);
					}
					else
					{
						pThis->NewReportPush(pLeaf,st);
					}
					cnt++;
				}
				pLeaf = pThis->m_StateLeafMap.FetchNext(pos);
			}
			pThis->NewReportProc();

		}
		if(pThis->m_pSimConfig->m_iMmsMxTimes > 0)
		{
			cnt = 0;
			stuLeafMap *pLeaf = pThis->m_AnalogLeafMap.FetchFirst(pos);
			while(!pThis->IsQuit() && pLeaf && cnt < pThis->m_pSimConfig->m_iMmsMxTimes)
			{
				//if(rand()%10 == 0 && cnt < pThis->m_pSimConfig->m_iMmsMxTimes /* && strstr(pThis->GetLeafMapIedLdName(pLeaf),"PT1001A") != NULL*/)
				{
					int st = rand()%1000;
					if(st == 0)
						st = 10;
					f = (rand()%10000)/st;
					pThis->NewReportPush(pLeaf,f);
					cnt ++;
				}
				pLeaf = pThis->m_AnalogLeafMap.FetchNext(pos);
			}
			pThis->NewReportProc();

		}
		SApi::UsSleep(1000000);
	}
	pThis->EndThread();
	return NULL;
}


