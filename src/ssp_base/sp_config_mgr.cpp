/**
 *
 * �� �� �� : ConfigMgr.cpp
 * �������� : 2015-7-29 13:53
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : �����ļ����������������������ļ���֧�������ļ��Ķ�̬���أ��Զ����������ļ��仯��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-29	�ۿ�������ļ�
 *
 **/

#include "sp_config_mgr.h"

static CConfigMgr* g_pConfigMgr = NULL;


//////////////////////////////////////////////////////////////////////////
// ��    ��:  CConfigBase
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-29 13:56
// ��    ��:  �����ļ����࣬���������ļ����ء�����ȹ����ӿڣ������ܿ������ļ�Ӧ�Դ˽ӿ�
//////////////////////////////////////////////////////////////////////////

CConfigBase::CConfigBase()
{
	m_ConfigType = SPCFG_UNKNOWN;
	m_bAutoRelaod = false;
	if(g_pConfigMgr == NULL)
		g_pConfigMgr = new CConfigMgr();
	g_pConfigMgr->AddConfig(this);
}

CConfigBase::~CConfigBase()
{
	if(g_pConfigMgr != NULL)
		g_pConfigMgr->RemoveConfig(this);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CConfigMgr
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-29 14:16
// ��    ��:  �����ļ���������
//////////////////////////////////////////////////////////////////////////
CConfigMgr::CConfigMgr()
{
	m_bQuit = true;
	m_iReloadSecs = 5;
	m_Configs.setAutoDelete(false);
	m_RemovedConfigs.setAutoDelete(true);
	m_ppConfig = new CConfigBase*[SPCFG_MAX_SIZE];//���֧��100������
	memset(m_ppConfig,0,sizeof(CConfigBase*)*SPCFG_MAX_SIZE);
}

CConfigMgr::~CConfigMgr()
{
	delete[] m_ppConfig;
	m_Configs.clear();
	m_RemovedConfigs.clear();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡΨһȫ��ָ��
// ��    ��:  �ۿ���
// ����ʱ��:  2017-8-4 17:06
// ����˵��:  void
// �� �� ֵ:  CConfigMgr*
//////////////////////////////////////////////////////////////////////////
CConfigMgr* CConfigMgr::GetPtr()
{
	return g_pConfigMgr;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �豸Ψһָ�루��̬����ʹ�ã�
// ��    ��:  �ۿ���
// ����ʱ��:  2017-8-4 17:05
// ����˵��:  @ptr��ʾ��Ψһָ��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CConfigMgr::SetPtr(CConfigMgr *ptr)
{
	g_pConfigMgr = ptr;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������¼��ؼ������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-29 14:27
// ����˵��:  @iSecΪ����
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CConfigMgr::SetReloadSeconds(int iSec)
{
	g_pConfigMgr->m_iReloadSecs = iSec;
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������¼��ط���,�����������ļ�ɨ���̣߳���ʱɨ���ļ��޸�ʱ�䣬
//			  ��ʱ���Ĭ��5�룬��ͨ��SetReloadSeconds�޸�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-29 15:17
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CConfigMgr::StartReload()
{
	if(!g_pConfigMgr->m_bQuit)
		return;
	g_pConfigMgr->m_bQuit = false;
	S_CREATE_THREAD(ThreadReload,g_pConfigMgr);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �˳����¼��ط����̣߳�ϵͳ�˳�ʱ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-29 15:19
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CConfigMgr::Quit()
{
	if(g_pConfigMgr != NULL)
	{
		if(g_pConfigMgr->m_bQuit == false)
		{
			g_pConfigMgr->m_bQuit = true;
			while(g_pConfigMgr->m_Configs.count() > 0)
				SApi::UsSleep(1000);
		}
		else
		{
			SApi::UsSleep(100000);
			g_pConfigMgr->m_Configs.clear();
		}
		delete g_pConfigMgr;
		g_pConfigMgr = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������ļ��ൽ������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-29 15:24
// ����˵��:  @pCfgΪ�����ļ�ʵ��ָ��
// �� �� ֵ:  true��ʾ��ӳɹ���false��ʾʧ�ܣ�ָ�����������ļ��Ѵ���)
//////////////////////////////////////////////////////////////////////////
bool CConfigMgr::AddConfig(CConfigBase *pCfg)
{
	g_pConfigMgr->m_Configs.append(pCfg);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ��ָ���������ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-12 10:03
// ����˵��:  @pCfg
//         :  @bAddtoRemovedList�Ƿ�ɾ���������ļ���ӵ��ͷŶ��У��˳�Ӧ��ʱ�ͷ�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CConfigMgr::RemoveConfig(CConfigBase *pCfg,bool bAddtoRemovedList/*=false*/)
{
	g_pConfigMgr->m_Configs.remove(pCfg);
	if(bAddtoRemovedList)
		g_pConfigMgr->m_RemovedConfigs.append(pCfg);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ�����͵������ļ�ָ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-29 15:32
// ����˵��:  @cfgType��ʾ�����ļ�����
// �� �� ֵ:  CConfigBase*��NULL��ʾ������
//////////////////////////////////////////////////////////////////////////
CConfigBase* CConfigMgr::GetConfig(eSpConfigType cfgType)
{
	unsigned long pos=0;
	CConfigBase *p ;
	if(cfgType <= 0 || cfgType >= SPCFG_MAX_SIZE || g_pConfigMgr == NULL)
		return NULL;
	p = g_pConfigMgr->m_ppConfig[(int)cfgType];
	if(p != NULL)
		return p;
	p = g_pConfigMgr->m_Configs.FetchFirst(pos);
	while(p)
	{
		if(p->GetConfigType() == cfgType)
		{
			g_pConfigMgr->m_ppConfig[(int)cfgType] = p;
			return p;
		}
		p = g_pConfigMgr->m_Configs.FetchNext(pos);
	}
	LOGWARN("ָ�������ļ�����[%d]������!",cfgType);
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ܿ������ļ��Զ����¼��ص�ɨ���߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-29 15:26
// ����˵��:  @lpΪthisָ��
// �� �� ֵ:  NULL
//////////////////////////////////////////////////////////////////////////
void* CConfigMgr::ThreadReload(void* lp)
{
	CConfigMgr *pThis = (CConfigMgr*)lp;
	unsigned long pos;
	CConfigBase *p;
	bool ret;
	S_INTO_THREAD;
	while(!pThis->m_bQuit)
	{
		p = pThis->m_Configs.FetchFirst(pos);
		while(p)
		{
			if(p->GetAutoReload())
			{
				if(SFile::filetime(p->GetPathFile()) != p->m_LastModifyTime)
				{
					ret = p->ReLoad();
					if(ret == false)
					{
						LOGERROR("���¼��������ļ�[%s]ʱʧ��!",p->GetPathFile().data());
						p->m_LastModifyTime = SFile::filetime(p->GetPathFile());
					}
					else
					{
						LOGDEBUG("�����ļ�[%s]�仯,�����¼���!",p->GetPathFile().data());
					}
				}
			}
			p = pThis->m_Configs.FetchNext(pos);
		}
		SApi::UsSleep(pThis->m_iReloadSecs*1000000);
	}
	pThis->m_Configs.clear();
	return NULL;
}
