/**
 *
 * �� �� �� : view_login.h
 * �������� : 2015-7-30 16:46
 * ��    �� : �ұ���
 * �޸����� : 2015-7-30 16:46
 * ��ǰ�汾 : 1.0
 * �������� : ��¼����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30 	�ұ�������¼����
 *
 **/
#ifndef VIEW_LOGIN_H
#define VIEW_LOGIN_H
#include "SApi.h"
#include "db/SDatabase.h"
#include "pwd_login.h"
#include "include_macdef.h"
#include <QDialog>

class view_login : public QDialog
{
	Q_OBJECT
//signals:
//	//////////////////////////////////////////////////////////////////////////
//	// ��    ��:  ��¼��Ϣ�ź�
//	// ��    ��:  �ұ���
//	// ����ʱ��:  2015-7-30 16:44
//	// ����˵��:  @pwd_login::LOGSTATE ��¼״̬
//	//         :  @SString ��¼��Ϣ
//	// �� �� ֵ:  
//	//////////////////////////////////////////////////////////////////////////
//	void loginSucessed(pwd_login::LOGSTATE,SString);
public:
	enum LOGINWINDOWTYPE
	{
		LOGIN_WND_TYPE_LOGIN,//�û���¼
		LOGIN_WND_TYPE_CHECK//Ȩ����֤
	};
public:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  view_login����
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-7-30 16:47
	// ����˵��:  @pdb ���ݿ����ָ��
	//         :  @usertype ��¼ģʽĬ��Ϊ����
	//		   :  @parent ������
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	view_login(LOGINWINDOWTYPE usertype=LOGIN_WND_TYPE_LOGIN,QWidget *parent=0);
	virtual ~view_login();
	bool exec(SString &sUser,SString &sMonitor)
	{
		QDialog::exec();

		if (m_bLogin)
		{
			sUser=m_sUser;
			sMonitor=m_sMonitor;
		}
		return m_bLogin;
	}
public slots:
	void onLoginSucessed(pwd_login::LOGSTATE state,SString sInfo);
private:
	QWidget * m_logWidget;//��¼����
	bool m_bLogin;//�Ƿ��¼�ɹ�
	SString m_sUser;//�û���
	SString m_sMonitor;//�໤����
	bool m_bSingle;//���˻��Ƕ���
};

#endif // VIEW_LOGIN_H
