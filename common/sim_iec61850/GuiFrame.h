/**
 *
 * �� �� �� : GuiFrame.h
 * �������� : 2015-7-30 15:53
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

#ifndef __TEST_GUI_FRAME_H__
#define __TEST_GUI_FRAME_H__

#include "ssp_uiframe.h"
class CGuiFrame : public CSspUiFrame
{
public:
	CGuiFrame(QWidget *parent=0);
	virtual ~CGuiFrame(void);

	virtual CBaseView* NewView(CFrameMenuL2 *pFunMenu,QWidget *parent);
};

#endif//__TEST_GUI_FRAME_H__
