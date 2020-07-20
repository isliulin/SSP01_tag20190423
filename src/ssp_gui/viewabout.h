/**
 *
 * �� �� �� : viewabout.h
 * �������� : 2013-9-13 8:52
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ���ڴ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2013-9-13	�ۿ�������ļ�
 *
 **/

#ifndef VIEWABOUT_H
#define VIEWABOUT_H

#include <QWidget>
#include <QWebView>
#include "ui_viewabout.h"
#include "ssp_baseview.h"

class CAboutPic : public QWidget
{
	Q_OBJECT
public:
	CAboutPic(QWidget *parent = 0);
	virtual ~CAboutPic();
	void paintEvent(QPaintEvent *event);

	QPixmap m_bmpAbout;

};

class CViewAbout : public CBaseView
{
	Q_OBJECT

public:
	CViewAbout(QWidget *parent = 0);
	virtual ~CViewAbout();
	virtual void OnPreShow();
	virtual void OnRefresh();
private:
	Ui::CViewAbout ui;
	CAboutPic *m_pAboutPic;
};


class CViewHelp : public CBaseView
{
	Q_OBJECT

public:
	CViewHelp(QWidget *parent = 0);
	virtual ~CViewHelp();
	virtual void OnPreShow();
	virtual void OnRefresh();
private:
	QWebView *m_pWebView;
	QGridLayout *m_gridLayout;
};

#endif // VIEWABOUT_H
