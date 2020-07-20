/**
 *
 * �� �� �� : WatchDogPPC.cpp
 * �������� : 2011-4-13 9:26
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: 2011/08/31 08:43:31 $
 * ��ǰ�汾 : $Revision: 1.1 $
 * �������� : PowerPC8315ƽ̨�µ�Ӳ�����Ź�������
 * �޸ļ�¼ : 
 *            $Log: WatchDogPPC.cpp,v $
 *            Revision 1.1  2011/08/31 08:43:31  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.2  2011/04/21 02:14:16  shaokt
 *            *** empty log message ***
 *
 *            Revision 1.1  2011/04/20 05:44:39  shaokt
 *            *** empty log message ***
 *
 *
 **/

#include "WatchDog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWatchDogPPC::CWatchDogPPC()
{
	m_hHandle = 0;
}

CWatchDogPPC::~CWatchDogPPC()
{

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �򿪿��Ź��豸
// ��    ��:  �ۿ���
// ����ʱ��:  2011-4-13 9:26
// ����˵��:  void
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CWatchDogPPC::Open()
{
#ifdef WIN32
	return false;
#else
	if((m_hHandle = open(PPC_WATCHDOG_DEVICE, O_WRONLY)) <= 0) 
	{
		LOGERROR("�򿪿��Ź��豸(%s)ʧ��",PPC_WATCHDOG_DEVICE);
		m_hHandle = 0;
		return false;
	}
	return true;
#endif
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ι������δ���豸ʱ�Զ���
// ��    ��:  �ۿ���
// ����ʱ��:  2011-4-13 9:27
// ����˵��:  void
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CWatchDogPPC::FeedDog()
{
#ifdef WIN32
	return false;
#else
	if(m_hHandle <= 0 && Open() == false)
		return false;
	write(m_hHandle,"a",1);
#endif
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �رտ��Ź���ֹͣ��Ӳ���ػ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2011-4-13 9:27
// ����˵��:  void
// �� �� ֵ:  true
//////////////////////////////////////////////////////////////////////////
bool CWatchDogPPC::Close()
{
#ifdef WIN32
	return false;
#else
	if(m_hHandle <= 0)
		return false;
	write(m_hHandle,"V",1);
	close(m_hHandle);
	m_hHandle = 0;
	return true;
#endif
}
