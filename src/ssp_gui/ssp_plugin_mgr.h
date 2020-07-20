/**
 *
 * �� �� �� : ssp_plugin_mgr.h
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
#ifndef __SSP_PLUGIN_MGR_H__
#define __SSP_PLUGIN_MGR_H__

#include "ssp_plugin.h"
#include "SString.h"
#include "SList.h"
#include "ssp_baseview.h"

class CSspPlugin
{
public:
	CSspPlugin();
	~CSspPlugin();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ز��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-4 10:52
	// ����˵��:  @sPluginFileΪ�����̬���ļ�ȫ·���ļ���
	// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool Load(SString sPluginFile);

	SString m_sPluginFile;//����ļ�����ȫ·����
#ifdef WIN32
	HMODULE m_hInstance;
#else
	void* m_hInstance;
#endif
	pFun_SSP_Init				m_pSSP_Init;
	pFun_SSP_Exit				m_pSSP_Exit;
	pFun_SSP_GetPluginName		m_pSSP_GetPluginName;
	pFun_SSP_GetPluginVer		m_pSSP_GetPluginVer;
	pFun_SSP_FunPointSupported	m_pSSP_FunPointSupported;
	//pFun_SSP_SetGlobalPtr		m_pSSP_SetGlobalPtr;
	pFun_SSP_NewView			m_pSSP_NewView;
	pFun_SSP_GActSupported		m_pSSP_GActSupported;
};

class CSspPluginMgr
{
public:
	CSspPluginMgr();
	~CSspPluginMgr();
	
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ��������������������в��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-4 11:16
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ��ʼ���ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool Init();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �˳����в��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-9-14 13:46
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool Exit();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����µĴ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-4 13:05
	// ����˵��:  @sFunNameΪ���ܵ�����
	//         :  @parentΪ����������
	// �� �� ֵ:  CBaseView*��ʾ�´򿪵Ĵ���ָ��, NULL��ʾ�����û�ж�Ӧ�Ĺ��ܵ㴰��
	//////////////////////////////////////////////////////////////////////////
	CBaseView* NewView(SString sFunName,QWidget* parent);

	SPtrList<CSspPlugin> m_Plugins;
};

#endif//__SSP_PLUGIN_MGR_H__
