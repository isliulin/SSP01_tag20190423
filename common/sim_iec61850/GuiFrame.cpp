/**
 *
 * �� �� �� : GuiFrame.cpp
 * �������� : 2015-7-30 15:55
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ͼ�ν������ʾ��������࣬���ڿ�ܴ��ڼ��������Ӵ��Ĺ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30	�ۿ�������ļ�
 *
 **/

#include "GuiFrame.h"
#include "view_home.h"
#include "view_sclimport.h"
#include "view_scl_edit.h"
#include "view_visual_ap.h"
#include "msgreplay.h"
CGuiFrame::CGuiFrame(QWidget *parent) : CSspUiFrame(parent)
{
	WaitForBackStart();
}


CGuiFrame::~CGuiFrame(void)
{
}

CBaseView* CGuiFrame::NewView(CFrameMenuL2 *pFunMenu,QWidget *parent)
{
	CBaseView *pView = CSspUiFrame::NewView(pFunMenu,parent);
	if(pView != NULL)
		return pView;
	//TODO:����Լ�����ͼ����
	SString sFunName = pFunMenu->m_sFunName;
 	if(sFunName == "home")
 		return new view_home(parent);
	else if(sFunName == "mod.scd.import")
		return new view_sclimport(parent);
	else if(sFunName == "mod.scd.edit")
		return new view_scl_edit(parent);
	else if(sFunName == "vis.ap")
		return new view_visual_ap(parent);
	else if(sFunName == "pcap.replay")
		return new MsgReplay(parent);
	return pView;
}