/**
 *
 * �� �� �� : view_doubleuser.h
 * �������� : 2015-7-30 16:42
 * ��    �� : �ұ���
 * �޸����� : 2015-7-30 16:42
 * ��ǰ�汾 : 1.0
 * �������� : ���û���¼����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30 	�ұ��������û���¼����
 *
 **/
#ifndef VIEW_SINGLEUSER_H
#define VIEW_SINGLEUSER_H

#include <QWidget>
#include "ui_view_singleuser.h"
#include "pwd_login.h"
class view_singleUser : public QWidget,public pwd_login
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
	view_singleUser(QWidget *parent = 0);
	virtual ~view_singleUser();
public slots:
	void on_pushButton_pressed();
	void on_pushButton_2_pressed()
	{
		((QWidget*)this->parent())->close();
	}
private:
	Ui::view_singleUser ui;
	SDatabase*m_db;
};

#endif // VIEW_SINGLEUSER_H
