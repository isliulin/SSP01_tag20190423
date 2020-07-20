/**
 *
 * �� �� �� : ssp_plugin_mgr.cpp
 * �������� : 2017-8-4 9:59
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SSP_GUI�����������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2017-8-4	�ۿ�������ļ�
 *
 **/
#include "ssp_plugin_mgr.h"
#include "SApplication.h"
#include "SFile.h"
#include "sp_config_mgr.h"
#include "ssp_base.h"

CSspPlugin::CSspPlugin()
{

}
CSspPlugin::~CSspPlugin()
{

}
//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ز��
// ��    ��:  �ۿ���
// ����ʱ��:  2017-8-4 10:52
// ����˵��:  @sPluginFileΪ�����̬���ļ�ȫ·���ļ���
// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool CSspPlugin::Load(SString sPluginFile)
{
	m_sPluginFile = sPluginFile;
	bool err = false;
#ifdef WIN32
	m_hInstance = LoadLibrary(sPluginFile.data());
	if(m_hInstance == NULL)
	{
		LOGWARN("���ز��%sʧ��!",sPluginFile.data());
		return false;
	}

	m_pSSP_Init = (pFun_SSP_Init)GetProcAddress(m_hInstance, "SSP_Init");
	m_pSSP_Exit = (pFun_SSP_Exit)GetProcAddress(m_hInstance, "SSP_Exit");
	m_pSSP_GetPluginName = (pFun_SSP_GetPluginName)GetProcAddress(m_hInstance, "SSP_GetPluginName");
	m_pSSP_GetPluginVer = (pFun_SSP_GetPluginVer)GetProcAddress(m_hInstance, "SSP_GetPluginVer");
	m_pSSP_FunPointSupported = (pFun_SSP_FunPointSupported)GetProcAddress(m_hInstance, "SSP_FunPointSupported");
	//m_pSSP_SetGlobalPtr = (pFun_SSP_SetGlobalPtr)GetProcAddress(m_hInstance, "SSP_SetGlobalPtr");
	m_pSSP_NewView = (pFun_SSP_NewView)GetProcAddress(m_hInstance, "SSP_NewView");
	m_pSSP_GActSupported = (pFun_SSP_GActSupported)GetProcAddress(m_hInstance, "SSP_GActSupported");

#else
	m_hInstance = dlopen(sPluginFile.data(),RTLD_LAZY);
	char *pErr;

	if ((pErr = dlerror()) != NULL)
	{
		err = true;
		LOGWARN("dlopen error:%s\n",pErr);
	}
	if(m_hInstance == NULL)
	{
		LOGWARN("���ز��%sʧ��!",sPluginFile.data());
		return false;
	}
	m_pSSP_Init = (pFun_SSP_Init)dlsym(m_hInstance, "SSP_Init");
	if ((pErr = dlerror()) != NULL)
	{
		err = true;
		LOGWARN("dlsym error:%s\n",pErr);
	}
	m_pSSP_Exit = (pFun_SSP_Exit)dlsym(m_hInstance, "SSP_Exit");
	m_pSSP_GetPluginName = (pFun_SSP_GetPluginName)dlsym(m_hInstance, "SSP_GetPluginName");
	if ((pErr = dlerror()) != NULL)
	{
		err = true;
		LOGWARN("dlsym error:%s\n",pErr);
	}
	m_pSSP_GetPluginVer = (pFun_SSP_GetPluginVer)dlsym(m_hInstance, "SSP_GetPluginVer");
	if ((pErr = dlerror()) != NULL)
	{
		err = true;
		LOGWARN("dlsym error:%s\n",pErr);
	}
	m_pSSP_FunPointSupported = (pFun_SSP_FunPointSupported)dlsym(m_hInstance, "SSP_FunPointSupported");
	if ((pErr = dlerror()) != NULL)
	{
		err = true;
		LOGWARN("dlsym error:%s\n",pErr);
	}
	m_pSSP_NewView = (pFun_SSP_NewView)dlsym(m_hInstance, "SSP_NewView");
	if ((pErr = dlerror()) != NULL)
	{
		err = true;
		LOGWARN("dlsym error:%s\n",pErr);
	}
	m_pSSP_GActSupported = (pFun_SSP_GActSupported)dlsym(m_hInstance, "SSP_GActSupported");
// 	if ((pErr = dlerror()) != NULL)
// 	{
// 		err = true;
// 		LOGWARN("dlsym error:%s\n",pErr);
// 	}

#endif
	if(err || m_pSSP_Init == NULL || m_pSSP_GetPluginName == NULL || m_pSSP_GetPluginVer == NULL || m_pSSP_FunPointSupported == NULL || m_pSSP_NewView == NULL)
	{
		if(m_pSSP_Init == NULL && m_pSSP_GetPluginName == NULL && m_pSSP_GetPluginVer == NULL && m_pSSP_FunPointSupported == NULL && m_pSSP_NewView == NULL)
		{
			LOGDEBUG("���ز��%sʧ��!����������ھ����߱���DLL����ȫ����HMI���!",sPluginFile.data());
			return false;//����������ھ����߱���DLL����ȫ���ǲ��
		}
		LOGWARN("���ز��%sʧ��!",sPluginFile.data());
		return false;
	}
#ifdef _DEBUG
	int ret = m_pSSP_Init("debug=true;");
#else
	int ret = m_pSSP_Init("");
#endif
	if(ret != 0)
	{
		if(ret == 1 || ret == 2)
		{
			SString text;
#ifdef _DEBUG
			text.sprintf("���ܼ��ط�DEBUG�汾���[%s]!",sPluginFile.data());
#else
			text.sprintf("���ܼ���DEBUG�汾���[%s]!",sPluginFile.data());
#endif
			//SQt::ShowErrorBox("����",text);
			LOGDEBUG("���%s��ʼ��ʧ��!ret=%d, DEBUG�汾��һ�£�����",sPluginFile.data(),ret);
		}
		else
		{
			LOGWARN("���%s��ʼ��ʧ��!ret=%d",sPluginFile.data(),ret);
		}
		return false;
	}
	//m_pSSP_SetGlobalPtr(1,CConfigMgr::GetPtr());
	//m_pSSP_SetGlobalPtr(2,ssp_base::GetPtr());
	LOGDEBUG("���%s���سɹ�!",sPluginFile.data());
	return true;
}


CSspPluginMgr::CSspPluginMgr()
{
	m_Plugins.setAutoDelete(true);
}

CSspPluginMgr::~CSspPluginMgr()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʼ��������������������в��
// ��    ��:  �ۿ���
// ����ʱ��:  2017-8-4 11:16
// ����˵��:  void
// �� �� ֵ:  true��ʾ��ʼ���ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool CSspPluginMgr::Init()
{
	SString sPath = SBASE_SAPP->GetBinPath();
	SStringList pathList;
	pathList.append(sPath);					// ���ݵ�ǰĿ¼

#ifndef WIN32
	pathList.append(sPath + "../lib/");		// �³����Ϊ��libĿ¼��(LINUX)
#endif

	LOGDEBUG("׼������Ŀ¼[%s]HMI���",sPath.data());

	for (int dirIdx = 0; dirIdx < pathList.count(); ++dirIdx)
	{
		sPath = *(pathList.at(dirIdx));
#ifdef WIN32
		SDir dir(sPath,"*.dll");
#else
		SDir dir(sPath,"*.so");
#endif
		int i,cnt = dir.count();
		for(i=0;i<cnt;i++)
		{
			SString sFile = dir[i];
			printf("dir %d = %s  ",i,sFile.data());
#ifdef WIN32
			if(sFile == "." || sFile == ".." || sFile.left(7) != "plugin_" || sFile.right(4) != ".dll")
#else
			if(sFile == "." || sFile == ".." || sFile.left(10) != "libplugin_" || sFile.right(3) != ".so")
#endif
			{
				printf("����! \n");
				continue;
			}
			printf("����...\n");
			SString attr = dir.attribute(i);
			if(SString::GetAttributeValueI(attr,"isdir") != 1)
			{
				CSspPlugin *pPlugin = new CSspPlugin();
				if(!pPlugin->Load(sPath + sFile))
				{
					delete pPlugin;
				}
				else
					m_Plugins.append(pPlugin);
			}
		}
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �˳����в��
// ��    ��:  �ۿ���
// ����ʱ��:  2017-9-14 13:46
// ����˵��:  void
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CSspPluginMgr::Exit()
{
	unsigned long pos;
	CSspPlugin *p = m_Plugins.FetchFirst(pos);
	while(p)
	{
		if(p->m_pSSP_Exit)
			p->m_pSSP_Exit();
		p = m_Plugins.FetchNext(pos);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����µĴ���
// ��    ��:  �ۿ���
// ����ʱ��:  2017-8-4 13:05
// ����˵��:  @sFunNameΪ���ܵ�����
//         :  @parentΪ����������
// �� �� ֵ:  CBaseView*��ʾ�´򿪵Ĵ���ָ��, NULL��ʾ�����û�ж�Ӧ�Ĺ��ܵ㴰��
//////////////////////////////////////////////////////////////////////////
CBaseView* CSspPluginMgr::NewView(SString sFunName,QWidget* parent)
{
	unsigned long pos;
	CBaseView *pView;
	CSspPlugin *p = m_Plugins.FetchFirst(pos);
	while(p)
	{
		pView = (CBaseView*)p->m_pSSP_NewView(sFunName.data(),parent);
		if(pView != NULL)
			return pView;
		p = m_Plugins.FetchNext(pos);
	}
	return NULL;
}
