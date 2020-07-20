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
#include "view_xml2ssaxex.h"
#include "view_mysql_mdbcfg.h"
#include "view_oracle_mdbcfg.h"
#include "view_prj_hmiplugin.h"

CGuiFrame::CGuiFrame(QWidget *parent) : CSspUiFrame(parent)
{
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
	if(sFunName == "code.xml2saxex")
		return new CViewXml2SSaxEx(parent);
	else if(sFunName == "code.mysql.helper")
		return new view_mysql_mdbcfg(parent);
	else if(sFunName == "code.oracle.helper")
		return new view_oracle_mdbcfg(parent);
	else if(sFunName == "prj.new.plugin")
		return new view_prj_hmiplugin(parent);
	return pView;
}