/**
 *
 * �� �� �� : view_doubleuser.h
 * �������� : 2015-7-30 16:42
 * ��    �� : �ұ���
 * �޸����� : 2015-7-30 16:42
 * ��ǰ�汾 : 1.0
 * �������� : ˫�û���¼����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30 	�ұ�����˫�û���¼����
 *
 **/
#ifndef VIEW_DOUBLEUSER_H
#define VIEW_DOUBLEUSER_H

#include <QWidget>
#include "ui_view_doubleuser.h"
#include "pwd_login.h"
class view_doubleUser : public QWidget,public pwd_login
{
	Q_OBJECT
signals:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��¼��Ϣ�ź�
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-7-30 16:44
	// ����˵��:  @pwd_login::LOGSTATE ��¼״̬
	//         :  @SString ��¼��Ϣ
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void loginSucessed(pwd_login::LOGSTATE,SString);
public:
	view_doubleUser(QWidget *parent = 0);
	virtual ~view_doubleUser();
public slots:
	void on_pushButton_pressed();
	void on_pushButton_2_pressed()
	{
		((QWidget*)this->parent())->close();
	}
private:
	Ui::view_doubleUser ui;
	SDatabase*m_db;
	SString m_in[2];
};

#endif // VIEW_DOUBLEUSER_H
