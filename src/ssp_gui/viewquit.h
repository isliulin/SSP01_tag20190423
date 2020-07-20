/**
 *
 * �� �� �� : viewquit.h
 * �������� : 2013-9-22 13:22
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : �˳�ϵͳ�ļ�������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2013-9-22	�ۿ�������ļ�
 *
 **/

#ifndef VIEWQUIT_H
#define VIEWQUIT_H

#include <QWidget>
#include "ui_viewquit.h"
#include "ssp_baseview.h"

class CViewQuit : public CBaseView
{
	Q_OBJECT

public:
	CViewQuit(QWidget *parent = 0);
	virtual ~CViewQuit();

	virtual void OnRefresh(){};
public slots:
	void OnResetHmi();
	void OnQuitHmi();
	void OnResetSys();
	void OnHaltSys();

private:
	Ui::CViewQuit ui;
};

#endif // VIEWQUIT_H
