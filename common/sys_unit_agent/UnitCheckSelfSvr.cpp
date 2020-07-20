/**
 *
 * �� �� �� : UnitCheckSelfSvr.cpp
 * �������� : 2015-7-21 13:25
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ��Ԫ�Լ����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-21	�ۿ�������ļ�
 *
 **/

#include "UnitCheckSelfSvr.h"

CUnitCheckSelfSvr::CUnitCheckSelfSvr()
{
	m_pUnitConfig = NULL;
	m_Analogs.setAutoDelete(true);
	m_Analogs.setShared(true);
	m_States.setAutoDelete(true);
	m_States.setShared(true);
	m_HashAnalog.SetHashSize(512);
	m_HashState.SetHashSize(512);
	m_pFunAnalogChg = NULL;
	m_pFunStateChg = NULL;
}

CUnitCheckSelfSvr::~CUnitCheckSelfSvr()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ������Ԫ�Լ����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-21 13:33
// ����˵��:  void
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CUnitCheckSelfSvr::Start()
{
	S_CREATE_THREAD(ThreadCheckSelf,this);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �Լ����߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-21 13:51
// ����˵��:  @lpΪthis
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void* CUnitCheckSelfSvr::ThreadCheckSelf(void *lp)
{
	CUnitCheckSelfSvr *pThis = (CUnitCheckSelfSvr*)lp;
	int i;
	pThis->BeginThread();

	//�����Լ�
	pThis->StartupCheckSelf();

	while(!pThis->IsQuit())
	{
		//ʵʱ�Լ�
		pThis->RealtimeCheckSelf();

		//5��ִ��һ�λ����Լ죬������Ŀ�������״̬����ʵʱ�Լ캯�������п���ִ��Ƶ��
		for(i=0;i<50 && (!pThis->IsQuit());i++)
			SApi::UsSleep(100000);
	}

	pThis->EndThread();
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ģ����ֵ�ı��ͻ������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-21 13:40
// ����˵��:  @pAnalog
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CUnitCheckSelfSvr::AnalogChg(stuUnitCheckSelfAnalog *pAnalog)
{
	if(m_pFunAnalogChg != NULL)
		m_pFunAnalogChg(m_pFunCbParam,pAnalog);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ״̬��ֵͻ���ı�Ĵ���
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-21 13:40
// ����˵��:  @pState
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CUnitCheckSelfSvr::StateChk(stuUnitCheckSelfState *pState)
{
	if(m_pFunStateChg != NULL)
		m_pFunStateChg(m_pFunCbParam,pState);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ģ����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-21 13:40
// ����˵��:  @listΪ���÷��ص��б�
//         :  @iFromIdΪ��ʼID��-1��ʾ��������ʼID
//         :  @iToIdΪ����ID��-1��ʾ�����ƽ���ID
// �� �� ֵ:  ��������ģ��������,<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CUnitCheckSelfSvr::SearchAnalog(SPtrList<stuUnitCheckSelfAnalog> &list,int iFromId/*=-1*/,int iToId/*=-1*/)
{
	if(iFromId == -1 && iToId == -1)
	{
		m_Analogs.copyto(list);
		return list.count();
	}
	unsigned long pos;
	stuUnitCheckSelfAnalog *p = m_Analogs.FetchFirst(pos);
	while(p)
	{
		if((iFromId == -1||p->m_iInfoId >= iFromId) && (iToId == -1 || p->m_iInfoId <= iToId))
			list.append(p);
		p = m_Analogs.FetchNext(pos);
	}
	return list.count();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����״̬��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-21 13:40
// ����˵��:  @listΪ���÷��ص��б�
//         :  @iFromIdΪ��ʼID��-1��ʾ��������ʼID
//         :  @iToIdΪ����ID��-1��ʾ�����ƽ���ID
// �� �� ֵ:  ��������״̬������,<0��ʾʧ��
//////////////////////////////////////////////////////////////////////////
int CUnitCheckSelfSvr::SearchState(SPtrList<stuUnitCheckSelfState> &list,int iFromId/*=-1*/,int iToId/*=-1*/)
{
	if(iFromId == -1 && iToId == -1)
	{
		m_States.copyto(list);
		return list.count();
	}
	unsigned long pos;
	stuUnitCheckSelfState *p = m_States.FetchFirst(pos);
	while(p)
	{
		if((iFromId == -1||p->m_iInfoId >= iFromId) && (iToId == -1 || p->m_iInfoId <= iToId))
			list.append(p);
		p = m_States.FetchNext(pos);
	}
	return list.count();
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����Լ죬��Ԫ�����������ʱһ���Լ����Ŀ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-21 13:53
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CUnitCheckSelfSvr::StartupCheckSelf()
{
	//�ȸ��ݵ�ǰ��Ԫ��Ӳ�����ͣ�����֧�ֵ��Լ���Ϣ��
	NewAnalog(new stuUnitCheckSelfAnalog(CS_ANALOG_CPU_USED,"CPU������",0,"%",&m_pUnitConfig->m_ChkSelfCpu));
	if(m_pUnitConfig->m_ChkSelfAllDisk.m_bOpen)
	{
		NewAnalog(new stuUnitCheckSelfAnalog(CS_ANALOG_ALLDISK,"�洢����������",0,"GB"));
		NewAnalog(new stuUnitCheckSelfAnalog(CS_ANALOG_ALLDISK_USED,"�洢������������",0,"%",&m_pUnitConfig->m_ChkSelfAllDisk));
	}
	//TODO:���̷����Լ���Ϣ��

	if(m_pUnitConfig->m_ChkSelfTemp.m_bOpen)
		NewAnalog(new stuUnitCheckSelfAnalog(CS_ANALOG_TEMP,"װ���¶�",0,"��",&m_pUnitConfig->m_ChkSelfTemp));
	if(m_pUnitConfig->m_ChkSelfHumi.m_bOpen)
		NewAnalog(new stuUnitCheckSelfAnalog(CS_ANALOG_HUMI,"װ��ʪ��",0,"%",&m_pUnitConfig->m_ChkSelfHumi));
	if(m_pUnitConfig->m_ChkSelfPwr[0].m_bOpen)
		NewAnalog(new stuUnitCheckSelfAnalog(CS_ANALOG_PWR1,"װ�õ�Դ��ƽ1",0,"V",&m_pUnitConfig->m_ChkSelfPwr[0]));
	if(m_pUnitConfig->m_ChkSelfPwr[1].m_bOpen)
		NewAnalog(new stuUnitCheckSelfAnalog(CS_ANALOG_PWR2,"װ�õ�Դ��ƽ2",0,"V",&m_pUnitConfig->m_ChkSelfPwr[1]));
	if(m_pUnitConfig->m_ChkSelfPwr[2].m_bOpen)
		NewAnalog(new stuUnitCheckSelfAnalog(CS_ANALOG_PWR3,"װ�õ�Դ��ƽ3",0,"V",&m_pUnitConfig->m_ChkSelfPwr[2]));
	if(m_pUnitConfig->m_ChkSelfPwr[3].m_bOpen)
		NewAnalog(new stuUnitCheckSelfAnalog(CS_ANALOG_PWR4,"װ�õ�Դ��ƽ4",0,"V",&m_pUnitConfig->m_ChkSelfPwr[3]));
	//TODO:�⹦���Լ���Ϣ��
	//TODO:Ӳ���¶��Լ���Ϣ��

	//TODO:ִ�п����Լ�
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ʵʱ�Լ죬����Ԫ����������к�ʱִ�е��Լ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-21 13:54
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CUnitCheckSelfSvr::RealtimeCheckSelf()
{

	//TODO:
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��Ϣ��ɢ��ֵ���㺯��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-21 14:44
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
int CUnitCheckSelfSvr::HashKey_Analog(void *cbParam,void *pItem)
{
	return ((stuUnitCheckSelfAnalog*)pItem)->m_iInfoId;
}

int CUnitCheckSelfSvr::HashKey_State(void *cbParam,void *pItem)
{
	return ((stuUnitCheckSelfState*)pItem)->m_iInfoId;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����µ���Ϣ�㵽�б��ɢ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-21 14:44
// ����˵��:  @p
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CUnitCheckSelfSvr::NewAnalog(stuUnitCheckSelfAnalog *p)
{
	m_Analogs.append(p);
	m_HashAnalog.append(p);
}

void CUnitCheckSelfSvr::NewState(stuUnitCheckSelfState *p)
{
	m_States.append(p);
	m_HashState.append(p);
}

