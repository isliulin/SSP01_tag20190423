/**
 *
 * �� �� �� : view_user_mgr.h
 * �������� : 2015-7-8 17:30
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : �û�����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-8	�ۿ�������ļ�
 *
 **/

#ifndef __VIEW_USER_MGR_H__
#define __VIEW_USER_MGR_H__

#include <QWidget>
#include "ui_view_user_mgr.h"
#include "ssp_baseview.h"
class view_user_mgr : public CBaseView
{
	Q_OBJECT

public:
	view_user_mgr(QWidget *parent = 0);
	virtual ~view_user_mgr();

private:
	Ui::view_user_mgr ui;
};

#endif // __VIEW_USER_MGR_H__
