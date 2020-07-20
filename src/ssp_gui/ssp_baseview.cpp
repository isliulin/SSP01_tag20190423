/**
 *
 * �� �� �� : ssp_baseview.cpp
 * �������� : 2015-7-8 11:07
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SSPͼ���˻��Ի����򣨹���վ����ܣ����ܴ��ڻ�����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-8	�ۿ�������ļ�
 *
 **/

#include "ssp_baseview.h"
#include "ssp_uiframe.h"
//#include "uiframe.h"
// class CUiFrame;
// extern CUiFrame *g_pUiFrame;

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CBaseView
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-8 11:11
// ��    ��:  ��������ͼ���࣬���й��ܴ�����Ҫ�Ӵ�����
//////////////////////////////////////////////////////////////////////////
CBaseView::CBaseView(QWidget *parent) : QWidget(parent)
{
	if(SApplication::GetPtr() != NULL)
	{
		LOGDEBUG("SApplication::GetPtr()->m_pUserPtr=%p",SApplication::GetPtr()->m_pUserPtr);
	}
	if(SApplication::GetPtr() != NULL && (SPtrList<CBaseView>*)(SApplication::GetPtr()->m_pUserPtr) != NULL)
		((SPtrList<CBaseView>*)(SApplication::GetPtr()->m_pUserPtr))->append(this);
}

CBaseView::~CBaseView()
{
	if(SApplication::GetPtr() != NULL && (SPtrList<CBaseView>*)(SApplication::GetPtr()->m_pUserPtr) != NULL)
		((SPtrList<CBaseView>*)(SApplication::GetPtr()->m_pUserPtr))->remove(this);
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������ʾ������麯��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-8 11:11
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CBaseView::OnAftShow()
{
	this->setEnabled(true);
	if(m_sFunCode.length() == 0)
		return;
	if(!CheckThisUserGrant(m_sFunCode))
	{
		this->setEnabled(false);
	}
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��鵱ǰ�û���ָ�����ܵ�Ȩ�ޣ�δ��¼ʱҪ�ȵ�¼
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-8 11:11
// ����˵��:  
// �� �� ֵ:  true��ʾ����false��ʾ�ܾ�
//////////////////////////////////////////////////////////////////////////
bool CBaseView::CheckThisUserGrant(SString sFunCode)
{
	if(!SSP_GUI->CheckFunPointAuth(sFunCode))
	{
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ������ʾ�ı�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-8 11:11
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
void CBaseView::SetTip(SString tip)
{
	SSP_GUI->SetStatusTip(tip);
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ˢ��ȫ���򿪵���ͼ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-8 11:11
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
void CBaseView::RefreshAllView()
{
	QWidget *p = parentWidget();
	if(p == NULL)
		return;
	p = p->parentWidget();
	if(p == NULL)
		return;
	CSspUiFrame *pFrame = (CSspUiFrame*)p;
	pFrame->RefreshAllView();
}

void CBaseView::slotGotoFunPoint()
{
	CSspUiFrame::GetPtr()->GotoView(CSspUiFrame::GetPtr()->m_sGotoFunPoint);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ת��ָ���Ĺ��ܵ㴰��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-5 14:35
// ����˵��:  @sFunKeyΪ���ܵ�����
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CBaseView::GotoFunPoint(SString sFunKey)
{
	CSspUiFrame::GetPtr()->m_sGotoFunPoint = sFunKey;
	emit slotGotoFunPoint();
	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ��¼�û�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-8 11:12
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
SString CBaseView::GetLoginUser()
{
	return SSP_GUI->GetOperUser();
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ��¼ʱ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-8 11:12
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
time_t CBaseView::GetLoginTime()
{
	return SSP_GUI->GetLoginTime();
}

