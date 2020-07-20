/**
 *
 * �� �� �� : GuiApplication.h
 * �������� : 2015-7-30 15:51
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ͼ�ν���Ӧ�ó�����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30	�ۿ�������ļ�
 *
 **/

#ifndef __TEST_GUI_APPLICATION_H__
#define __TEST_GUI_APPLICATION_H__

#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include "SApplication.h"
#include "GuiFrame.h"
#include "ssp_gui.h"
#include "ssp_database.h"
#include "ssp_funpoint.h"
#include "ssp_datawindow.h"
#include "sp_svglib_config.h"

class CGuiApplication : public SApplication
{
public:
	CGuiApplication(void);
	virtual ~CGuiApplication(void);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡAPPӦ��ʵ��ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 15:59
	// ����˵��:  void
	// �� �� ֵ:  CHmiApplication*
	//////////////////////////////////////////////////////////////////////////
	static CGuiApplication* GetApp();
	static QApplication* GetQApp();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �豸QTӦ����ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 15:59
	// ����˵��:  @pApp
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetQApplication(QApplication *pApp);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������񣬿�������Ӧ�õķ��񣬸ú���������������ʵ�֣�������������ɺ���뷵��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 15:59
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ���񣬹ر�����Ӧ�õķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 15:59
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰģ�������״̬��Ϣ����Ӧ����ģ��Ӧ�г����н����豸��״̬��ϸ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 15:59
	// ����˵��:  void
	// �� �� ֵ:  SString
	//////////////////////////////////////////////////////////////////////////
	virtual SString GetNowModInfo();

	bool CSpAgentClient::ProcessAgentMsg(WORD,stuSpUnitAgentMsgHead *,SString &,BYTE *,int){return false;};

protected:
	QApplication *m_pQApp;//QTӦ��ָ��
	CGuiFrame *m_pMainWnd;//������

	CSsp_Database *m_pDatabase;			//���ݿ������ļ�
	CSsp_DatawindowMgr *m_pDatawindow;	//���ݴ��������ļ�
	CSsp_FunPointConfig *m_pFunpoint;	//���ܵ������ļ�
	CSp_SvgLibConfig	*m_pSvgLibCfg;	//SVGͼԪ��
};

#endif//__TEST_GUI_APPLICATION_H__
