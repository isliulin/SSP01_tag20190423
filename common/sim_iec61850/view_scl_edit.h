/**
 *
 * �� �� �� : view_scl_edit.h
 * �������� : 2015-12-29 17:23
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-12-29	�ۿ�������ļ�
 *
 **/

#ifndef VIEW_SCL_EDIT_H
#define VIEW_SCL_EDIT_H

#include <QWidget>
#include "ui_view_scl_edit.h"
#include "ssp_baseview.h"
#include "sim_config.h"
#include "sp_sclparser.h"

class view_scl_edit : public CBaseView
{
	Q_OBJECT

public:
	view_scl_edit(QWidget *parent = 0);
	~view_scl_edit();

private:
	Ui::view_scl_edit ui;
};

#endif // VIEW_SCL_EDIT_H
