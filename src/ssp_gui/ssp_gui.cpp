/**
 *
 * �� �� �� : ssp_gui.cpp
 * �������� : 2015-7-8 11:02
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SSPͼ���˻��Ի����򣨹���վ�����ƽ̨�ӿ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-8	�ۿ�������ļ�
 *
 **/

#include "ssp_gui.h"
#include "qt/SQt.h"
#include "view_login.h"
#include "ssp_uiframe.h"
#include "view_mdb_svg.h"
#include "view_common.h"
#include "view_query.h"
#include "dlg_login.h"
#include "dlg_operlogin.h"
#include "ssp_uiframe_bluestyle.h"
#include "dlg_login_bluestyle.h"
#include "dlg_login_substation.h"

ssp_gui g_ssp_gui;

ssp_gui::ssp_gui()
{
	LOGERROR("new ssp_gui!!!!!!");
	m_plabStatusTip = NULL;
	m_GeneralActions.setAutoDelete(true);
	m_ViewFloat.setAutoDelete(true);
}

ssp_gui::~ssp_gui()
{
	m_ViewFloat.clear();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��ǰ��ľ�̬ʵ����һ������ֻ��һ��ʵ����ƽ̨�Զ�ʵ����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-3 16:07
// ����˵��:  void
// �� �� ֵ:  ssp_gui*
//////////////////////////////////////////////////////////////////////////
ssp_gui* ssp_gui::GetPtr()
{
	return &g_ssp_gui;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ƽ̨ͼ�λ��������,ͼ�ι���վϵͳ����ʱһ���Ե���
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-22 17:14
// ����˵��:  @sGuiStyle��ʾͼ����ʽ,default:ȱʡ
//         :  @sExtAttribute��ʾ��չ����
// �� �� ֵ:  true��ʾ�����ɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool ssp_gui::StartGui(SString sGuiStyle,SString sExtAttribute)
{
	S_UNUSED(sGuiStyle);
	S_UNUSED(sExtAttribute);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ִ�е�¼������������¼����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-3 16:13
// ����˵��:  void
// �� �� ֵ:  true��ʾ��¼�ɹ���false��ʾ��¼ʧ��
//////////////////////////////////////////////////////////////////////////
bool ssp_gui::Login()
{
	//if(m_sLoginUser.length() > 0)
		//return true;

	//dlg_login dlg;
	//dlg_login_new dlg;
	dlg_login_blueStyle dlg;
	if(dlg.exec() == QDialog::Rejected)
		return false;

	m_sLoginUser = dlg.m_sUser;
	SRecordset rs;
	DB->Retrieve(SString::toFormat("select usr_sn,name,login_timeout from t_ssp_user where usr_code='%s'",m_sLoginUser.data()),rs);
	m_iLoginUserSn = rs.GetValue(0,0).toInt();
	m_sLoginUserName = rs.GetValue(0,1);
	m_iTimeoutSecs =  rs.GetValue(0,2).toInt();
	m_dtLoginTime = SDateTime::currentDateTime();
	NewSysLog(SSP_SYSLOG_TYPE_LOGIN,SSP_SYSLOG_LEVEL_TIP,
		SString::toFormat("�û�(%s:%s)������(%s)��¼����վHMI",m_sLoginUser.data(),m_sLoginUserName.data(),SSocket::GetLocalIp().data()).data());
	return true;
}

bool ssp_gui::LoginSubstation(bool forOper) {
	dlg_login_substation dlg(forOper);
	if(dlg.exec() == QDialog::Rejected)
		return false;

	if (forOper) {
		m_sLoginUser = dlg.m_sUser;
	}	
	else {
		m_sGuarderUser = dlg.m_sUser;
	}
	SRecordset rs;
	DB->Retrieve(SString::toFormat("select usr_sn,name,login_timeout from t_ssp_user where usr_code='%s'",m_sLoginUser.data()),rs);
	m_iLoginUserSn = rs.GetValue(0,0).toInt();
	m_sLoginUserName = rs.GetValue(0,1);
	if (forOper)
		m_iTimeoutSecs =  rs.GetValue(0,2).toInt();
	m_dtLoginTime = SDateTime::currentDateTime();
	/*
	NewSysLog(SSP_SYSLOG_TYPE_LOGIN,SSP_SYSLOG_LEVEL_TIP,
		SString::toFormat("�û�(%s:%s)������(%s)��¼����վHMI",m_sLoginUser.data(),m_sLoginUserName.data(),SSocket::GetLocalIp().data()).data());
	*/
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �˳���¼
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-3 16:18
// ����˵��:  @bOnlyGuarderΪtrue��ʾ���˳��໤�˵ĵ�¼
// �� �� ֵ:  true��ʾ�˳��ɹ���false��ʾ�˳�ʧ��
//////////////////////////////////////////////////////////////////////////
void ssp_gui::Logout()
{
	OperatorLogout();
	if(m_iLoginUserSn != 0)
	{
		SString sTip;
		sTip.sprintf("�û�(%s:%s)�˳�����վHMI�ĵ�¼",m_sLoginUser.data(),m_sLoginUserName.data());
		NewSysLog(SSP_SYSLOG_TYPE_OPERLOGOUT,SSP_SYSLOG_LEVEL_TIP,sTip.data());
	}
	m_iLoginUserSn = 0;
	m_sLoginUser = m_sLoginUserName = "";
}

void ssp_gui::LogoutSubstation()
{
	//OperatorLogout();
	if(m_iLoginUserSn != 0)
	{
		SString sTip;
		sTip.sprintf("�û�(%s:%s)�˳�����վHMI�ĵ�¼",m_sLoginUser.data(),m_sLoginUserName.data());
		NewSysLog(SSP_SYSLOG_TYPE_OPERLOGOUT,SSP_SYSLOG_LEVEL_TIP,sTip.data());
	}
	m_iLoginUserSn = 0;
	m_sLoginUser = m_sLoginUserName = "";

	m_sGuarderUser = m_sGuarderUserName = "";
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����Ա��ʽ��¼,���ڵ�¼�����˼��໤��
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-22 22:48
// ����˵��:  @sFunPointKey��ʾ���ܵ�����
//         :  @sOperTip��ʾ������ʾ�ı�
//         :  @bDoubleMode��ʾ��˫�û���¼�����Ƿ񺬼໤�ˣ�
// �� �� ֵ:  true��ʾȨ���Ѿ߱���false��ʾȨ�޲��߱�
//////////////////////////////////////////////////////////////////////////
bool ssp_gui::OperatorLogin(SString sFunPointKey, SString sOperTip, bool bDoubleMode/*=false*/)
{
begin:
	if(m_sOperUser.length() > 0)
	{
		if(!CheckUserFunPointAuth(m_sOperUser,sFunPointKey))
		{
			SQt::ShowInfoBox(tr("��ʾ"),tr("��ǰ�����˲��߱���ǰ������Ȩ��!\r\n������û�!"));
			OperatorLogout();
			goto begin;
		}
		if(bDoubleMode && !CheckUserFunPointAuth(m_sGuarderUser,sFunPointKey))
		{
			SQt::ShowInfoBox(tr("��ʾ"),tr("��ǰ�໤�˲��߱���ǰ������Ȩ��!\r\n������û�!"));
			OperatorLogout();
			goto begin;
		}
	}

	{
		dlg_operlogin dlg(bDoubleMode,sOperTip);
		if(dlg.exec() == QDialog::Rejected)
			return false;
		m_sOperUser = dlg.m_sUser;
		m_sGuarderUser = dlg.m_sMonUser;
	}
	if(!CheckUserFunPointAuth(m_sOperUser,sFunPointKey))
	{
		SQt::ShowInfoBox(tr("��ʾ"),tr("��ǰ�����˲��߱���ǰ������Ȩ��!\r\n������û�!"));
		OperatorLogout();
		goto begin;
	}
	if(bDoubleMode && !CheckUserFunPointAuth(m_sGuarderUser,sFunPointKey))
	{
		SQt::ShowInfoBox(tr("��ʾ"),tr("��ǰ�໤�˲��߱���ǰ������Ȩ��!\r\n������û�!"));
		OperatorLogout();
		goto begin;
	}
	SRecordset rs;
	DB->Retrieve(SString::toFormat("select usr_sn,name from t_ssp_user where usr_code='%s'",m_sOperUser.data()),rs);
	m_iOperUserSn = rs.GetValue(0,0).toInt();
	m_sOperUserName = rs.GetValue(0,1);
	if(m_sGuarderUser.length() > 0)
	{
		DB->Retrieve(SString::toFormat("select usr_sn,name from t_ssp_user where usr_code='%s'",m_sGuarderUser.data()),rs);
		m_iGuarderUserSn = rs.GetValue(0,0).toInt();
		m_sGuarderUserName = rs.GetValue(0,1);
	}
	SString sTip;
	sTip.sprintf("������(%s:%s)������(%s)ͨ��(%s:%s)���ܵ�Ȩ����֤",m_sOperUser.data(),m_sOperUserName.data(),SSocket::GetLocalIp().data(),
		sFunPointKey.data(),DB->SelectInto(SString::toFormat("select name from t_ssp_fun_point where fun_key='%s'",sFunPointKey.data())).data());
	if(m_sGuarderUser.length() > 0)
		sTip += SString::toFormat(",�໤��(%s:%s)",m_sGuarderUser.data(),m_sGuarderUserName.data());
	NewSysLog(SSP_SYSLOG_TYPE_OPERLOGIN,SSP_SYSLOG_LEVEL_TIP,sTip.data());

	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����˼��໤���˳���¼
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-22 22:50
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void ssp_gui::OperatorLogout()
{
	//�˳������˺ͼ໤�˵ĵ�¼
	if(m_iOperUserSn != 0)
	{
		SString sTip;
		sTip.sprintf("������(%s:%s)�˳����ܵ�Ȩ����֤",m_sOperUser.data(),m_sOperUserName.data());
		if(m_iGuarderUserSn != 0)
			sTip += SString::toFormat(",�໤��(%s:%s)",m_sGuarderUser.data(),m_sGuarderUserName.data());
		NewSysLog(SSP_SYSLOG_TYPE_OPERLOGOUT,SSP_SYSLOG_LEVEL_TIP,sTip.data());
	}
	m_iOperUserSn = m_iGuarderUserSn = 0;
	m_sOperUser = m_sOperUserName = "";
	m_sGuarderUser = m_sGuarderUserName = "";
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��鵱ǰ�û��Ƿ�߱�ָ�빦�ܵ��Ȩ�ޣ������û�����ʱ������¼������ʾ�û���¼
//            ����Ҫ˫�û���֤����δ��¼˫�û�ʱӦ����˫�û���¼��ʾ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-3 16:12
// ����˵��:  @sFunPointKey��ʾ����֤Ȩ�޵Ĺ��ܵ���
//         :  @bDoubleMode��ʾ�Ƿ�Ϊ˫�û�ģʽ��false��ʾ������Ա��¼��true��ʾ����Ա���໤��˫�û���¼
// �� �� ֵ:  true���߱�Ȩ�ޣ�false��ʾ���߱�Ȩ��
//////////////////////////////////////////////////////////////////////////
bool ssp_gui::CheckFunPointAuth(SString sFunPointKey,bool bDoubleMode/*=false*/)
{
	S_UNUSED(bDoubleMode);
	if(!IsLogin())
	{
		if(!Login())
			return false;
	}
	if(!CheckUserFunPointAuth(m_sLoginUser,sFunPointKey))
	{
		SQt::ShowInfoBox(tr("��ʾ"),SQt::qsprintf(tr("��ǰ��¼�û�[%s-%s]���߱������ܵ�ʹ��Ȩ��!"),m_sLoginUser.data(),m_sLoginUserName.data()));
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ָ���û��Ƿ����ָ�����ܵ��Ȩ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-3 16:39
// ����˵��:  @sUserCodeΪ�û�����
//         :  @sFunPointKeyΪ���ܵ����
// �� �� ֵ:  true��ʾ�߱�Ȩ��,false��ʾ���߱�Ȩ��
//////////////////////////////////////////////////////////////////////////
bool ssp_gui::CheckUserFunPointAuth(SString sUserCode,SString sFunPointKey)
{
	SString sql;
	sql.sprintf("select auth from t_ssp_usergroup_auth where fun_key='%s' and grp_code in (select grp_code from t_ssp_user where usr_code='%s')",
		sFunPointKey.data(),sUserCode.data());
	int auth = DB->SelectIntoI(sql);
	if(auth == 1)
		return true;
	sql.sprintf("select auth from t_ssp_user_auth where fun_key='%s' and usr_sn in (select usr_sn from t_ssp_user where usr_code='%s')",
		sFunPointKey.data(),sUserCode.data());
	auth = DB->SelectIntoI(sql);
	if(auth == 1)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��¼��Ϣ�����ַ��������ڽ�����ʾ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-3 16:04
// ����˵��:  void
// �� �� ֵ:  SString��������¼�ˡ���¼ʱ�䡢��ʱ�˳�ʱ�䣨�����ʱʱ�䲻Ϊ0��
//////////////////////////////////////////////////////////////////////////
SString ssp_gui::GetLoginInfoStr()
{
	SString str;
	if(m_sLoginUser.length() == 0)
		return Q2S(tr("��δ��¼! "));
	else
		str += Q2C(SQt::qsprintf(tr("��¼�û�:%s "),m_sLoginUserName.data()));
// 	if(m_sOperUser.length() != 0)
// 		str += Q2C(SQt::qsprintf(tr("������:%s "),m_sOperUserName.data()));
// 	if(m_sGuarderUser.length() != 0)
// 		str += Q2C(SQt::qsprintf(tr("�໤��:%s "),m_sGuarderUserName.data()));
	str += m_dtLoginTime.toString("��¼ʱ��:yyyy-MM-dd hh:mm:ss ");
	if(m_iTimeoutSecs != 0)
	{
		int sec = m_iTimeoutSecs - m_dtLoginTime.secsTo(SDateTime::currentDateTime());
		
		if(sec > 3600*24)
			str += Q2C(SQt::qsprintf(tr("%d����¼��ʱ! "),sec / 3600*24));
		else if(sec > 3600)
			str += Q2C(SQt::qsprintf(tr("%dСʱ���¼��ʱ! "),sec / 3600));
		else if(sec > 60)
			str += Q2C(SQt::qsprintf(tr("%d���Ӻ��¼��ʱ! "),sec / 60));
		else if(sec <= 0)
		{
			str += "�����Զ�ע��!";
		}
		else
			str += Q2C(SQt::qsprintf(tr("%d���Ӻ��¼��ʱ! "),sec));
	}
	return str;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �Ƿ��¼�ѳ�ʱ����ʱ�����ϲ�ע����¼
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-4 12:23
// ����˵��:  void
// �� �� ֵ:  true��ʾ��ʱ��false��ʾδ��ʱ
//////////////////////////////////////////////////////////////////////////
bool ssp_gui::IsLoginTimeout()
{
	if(m_iTimeoutSecs == 0 || m_sLoginUser.length() == 0)
		return false;
	return (m_iTimeoutSecs - m_dtLoginTime.secsTo(SDateTime::currentDateTime())) <= 0;	
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����״̬������ʾ��Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-10 8:50
// ����˵��:  @sText��ʾ��ʾ�ı�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void ssp_gui::SetStatusTip(SString sText)
{
	if(m_plabStatusTip == NULL)
		return;
	m_plabStatusTip->setText(sText.data());
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �򿪸�������̬����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-12-30 9:52
// ����˵��:  @wnd_sn�������
//         :  @w��ʾ���ڿ��
//         :  @h��ʾ���ڸ߶�
//         :  @alpha��ʾ͸���ȣ���alphaͨ����32λɫ�����ʱ����Ч��
//         :  @title��ʾ���ڱ���
// �� �� ֵ:  <0��ʾʧ��,>0��ʾ�ɹ������´��ڣ�=0��ʾ��ʾ�Ѵ򿪹��Ĵ��ڲ�ˢ��
//////////////////////////////////////////////////////////////////////////
int ssp_gui::OpenFloatWnd(int wnd_sn,int w,int h,int alpha,SString title)
{
	SString sql;
	int wnd_type=DB->SelectIntoI(SString::toFormat("select wnd_type from t_ssp_uicfg_wnd where wnd_sn=%d",wnd_sn));
	unsigned long pos=0;
	float falpha = ((float)alpha)/100.0f;
	if(falpha <= 0.0001)
		falpha = 1;
	else if(falpha < 0.1)
		falpha = 0.1;
	else if(falpha > 1)
		falpha = 1;
	CBaseView *pView = m_ViewFloat.FetchFirst(pos);
	while(pView)
	{
		if(pView->m_iWndSn == wnd_sn)
		{
			if(title.length() == 0)
				pView->setWindowTitle(title.data());
			pView->setWindowOpacity(falpha);
			pView->OnPreShow();
			pView->show();
			pView->OnAftShow();
			return 0;
		}
		pView = m_ViewFloat.FetchNext(pos);
	}

	if(wnd_type == 1)//1-	ӳ�䵽�ڴ����ݱ��SVGͼ�μ��Ӵ���
	{
		//ָ������̬���ڴ���
		//�ȴ��Ѵ����Ĵ����в���

		//�Ҳ������򴴽��µĴ���
		sql.sprintf("select dataset,ds_cond,wnd_name from t_ssp_uicfg_wnd where wnd_sn=%d",wnd_sn);
		CViewMdbSvg *pNew = new CViewMdbSvg(wnd_sn,NULL,true/*,&sExtAttr*/);
		pView = pNew;
		if(title.length() == 0)
			title = DB->SelectInto(sql,2);
		pNew->setWindowTitle(title.data());
		//pNew->m_pParentView = this;
		pNew->SetDatasetName(DB->SelectInto(sql));
		pNew->SetCondition(DB->SelectInto(sql,1));
	}
	else if(wnd_type == 2 || wnd_type == 4)//2-	ӳ�䵽�ڴ����ݱ�Ķ�̬���  4-	ӳ�䵽��ʷ���ݱ�����ݴ��ڣ���ѯ����
	{
		bool bMdb = DB->SelectIntoI("select count(*) from t_ssp_data_window where name='%s' and dbtype=3")>0;
		sql.sprintf("select dataset,ds_cond from t_ssp_uicfg_wnd where wnd_sn=%d",wnd_sn);
		SString ds = DB->SelectInto(sql);
		view_query *pNew = new view_query(NULL,ds,bMdb);
		pView = pNew;
		pNew->m_iWndSn = wnd_sn;
		if(title.length() == 0)
			title = DB->SelectInto(SString::toFormat("select dw_desc from t_ssp_data_window where name='%s' ",ds.data()));
		pNew->setWindowTitle(title.data());
	}
	else if(wnd_type == 3)//3-	ӳ�䵽��ʷ���ݱ�����ݴ��ڣ���Ӧ���ݴ��������е�ĳһ���ݼ��������������ɹ������������Էֱ�ָ����ѯ/�޸�/���/ɾ������
	{
		sql.sprintf("select dataset,ds_cond from t_ssp_uicfg_wnd where wnd_sn=%d",wnd_sn);
		SString ds = DB->SelectInto(sql);
		view_common *pNew = new view_common(NULL,ds);
		pView = pNew;
		pNew->m_iWndSn = wnd_sn;
		if(title.length() == 0)
			title = DB->SelectInto(SString::toFormat("select dw_desc from t_ssp_data_window where name='%s' ",ds.data()));
		pNew->setWindowTitle(title.data());
	}
	else
	{
		return -1;
	}

	//��ʾ�´���
	pView->setWindowOpacity(falpha);
	pView->OnPreShow();
	if(w == -1 && h == -1)
	{
		pView->showMaximized();
	}
	else
	{
#ifdef WIN32
		pView->setWindowFlags( Qt::Dialog | Qt::Tool/* | Qt::WindowStaysOnTopHint*/);
		pView->setWindowModality(Qt::ApplicationModal);
#else
		pView->setWindowFlags( Qt::Dialog );
#endif
// 		if(w <= 0)
// 			w = 300;
// 		if(h <= 0)
// 			h = 200;
		pView->resize(w,h);
		pView->show();
	}				
	pView->OnAftShow();
	pView->setFocus();
	m_ViewFloat.append(pView);

	return 1;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �򿪶Ի���ʽ����̬����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-12-31 8:50
// ����˵��:  @wnd_sn�������
//         :  @w��ʾ���ڿ��
//         :  @h��ʾ���ڸ߶�
//         :  @alpha��ʾ͸���ȣ���alphaͨ����32λɫ�����ʱ����Ч��
//         :  @title��ʾ���ڱ���
// �� �� ֵ:  <0��ʾʧ��,>0��ʾ�ɹ������´��ڣ�=0��ʾ��ʾ�Ѵ򿪹��Ĵ��ڲ�ˢ��
//////////////////////////////////////////////////////////////////////////
int ssp_gui::OpenDialogWnd(int wnd_sn,int w,int h,int alpha,SString title)
{
	SString sql;
	int wnd_type=DB->SelectIntoI(SString::toFormat("select wnd_type from t_ssp_uicfg_wnd where wnd_sn=%d",wnd_sn));
	//unsigned long pos=0;
	float falpha = ((float)alpha)/100.0f;
	if(falpha <= 0.0001)
		falpha = 1;
	else if(falpha < 0.1)
		falpha = 0.1;
	else if(falpha > 1)
		falpha = 1;
	CBaseView *pView;
	QDialog dlg;
	if(wnd_type == 1)//1-	ӳ�䵽�ڴ����ݱ��SVGͼ�μ��Ӵ���
	{
		//ָ������̬���ڴ���
		//�ȴ��Ѵ����Ĵ����в���

		//�Ҳ������򴴽��µĴ���
		sql.sprintf("select dataset,ds_cond,wnd_name from t_ssp_uicfg_wnd where wnd_sn=%d",wnd_sn);
		CViewMdbSvg *pNew = new CViewMdbSvg(wnd_sn,&dlg,true/*,&sExtAttr*/);
		pView = pNew;
		if(title.length() == 0)
			title = DB->SelectInto(sql,2);
		pNew->setWindowTitle(title.data());
		//pNew->m_pParentView = this;
		pNew->SetDatasetName(DB->SelectInto(sql));
		pNew->SetCondition(DB->SelectInto(sql,1));
	}
	else if(wnd_type == 2 || wnd_type == 4)//2-	ӳ�䵽�ڴ����ݱ�Ķ�̬���  4-	ӳ�䵽��ʷ���ݱ�����ݴ��ڣ���ѯ����
	{
		bool bMdb = DB->SelectIntoI("select count(*) from t_ssp_data_window where name='%s' and dbtype=3")>0;
		sql.sprintf("select dataset,ds_cond from t_ssp_uicfg_wnd where wnd_sn=%d",wnd_sn);
		SString ds = DB->SelectInto(sql);
		view_query *pNew = new view_query(&dlg,ds,bMdb);
		pView = pNew;
		pNew->m_iWndSn = wnd_sn;
		if(title.length() == 0)
			title = DB->SelectInto(SString::toFormat("select dw_desc from t_ssp_data_window where name='%s' ",ds.data()));
		pNew->setWindowTitle(title.data());
	}
	else if(wnd_type == 3)//3-	ӳ�䵽��ʷ���ݱ�����ݴ��ڣ���Ӧ���ݴ��������е�ĳһ���ݼ��������������ɹ������������Էֱ�ָ����ѯ/�޸�/���/ɾ������
	{
		sql.sprintf("select dataset,ds_cond from t_ssp_uicfg_wnd where wnd_sn=%d",wnd_sn);
		SString ds = DB->SelectInto(sql);
		view_common *pNew = new view_common(&dlg,ds);
		pView = pNew;
		pNew->m_iWndSn = wnd_sn;
		if(title.length() == 0)
			title = DB->SelectInto(SString::toFormat("select dw_desc from t_ssp_data_window where name='%s' ",ds.data()));
		pNew->setWindowTitle(title.data());
	}
	else
	{
		return -1;
	}
	QHBoxLayout *horizontalLayout = new QHBoxLayout(&dlg);
	horizontalLayout->setSpacing(1);
	horizontalLayout->setContentsMargins(11, 11, 11, 11);
	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
	horizontalLayout->setContentsMargins(1, 1, 1, 1);

	horizontalLayout->addWidget(pView);

	dlg.setWindowTitle(title.data());
	dlg.setWindowOpacity(falpha);
	//dlg.setWindowFlags(Qt::WindowMinMaxButtonsHint);
	if(w == -1 && h == -1)
		dlg.showMaximized();
	else
		dlg.resize(w,h);
	pView->OnPreShow();
	pView->OnAftShow();
#ifdef WIN32
	//dlg.setWindowFlags( Qt::Dialog | Qt::Tool | Qt::WindowStaysOnTopHint|Qt::WindowMinMaxButtonsHint);
	dlg.setWindowFlags(Qt::Dialog | Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);
#else
	//dlg.setWindowFlags( Qt::Dialog);
	dlg.setWindowFlags( Qt::Dialog  | Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);
#endif
	dlg.setWindowModality(Qt::ApplicationModal);
	dlg.exec();

	return 1;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ��Ƕ��ʽ����
// ��    ��:  �ۿ���
// ����ʱ��:  2017-7-26 10:33
// ����˵��:  @wnd_sn��̬�������
//         :  @parentΪ������ָ��
//         :  @title��ʾ���ڱ���
// �� �� ֵ:  QWidget*�� ��ʾ�´������Ӵ���ָ�룬NULL��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
QWidget* ssp_gui::NewEmbeddedWnd(int wnd_sn,QWidget *parent,SString title)
{
	SString sql;
	int wnd_type=DB->SelectIntoI(SString::toFormat("select wnd_type from t_ssp_uicfg_wnd where wnd_sn=%d",wnd_sn));
	CBaseView *pView;
	if(wnd_type == 1)//1-	ӳ�䵽�ڴ����ݱ��SVGͼ�μ��Ӵ���
	{
		//ָ������̬���ڴ���
		//�ȴ��Ѵ����Ĵ����в���

		//�Ҳ������򴴽��µĴ���
		sql.sprintf("select dataset,ds_cond,wnd_name from t_ssp_uicfg_wnd where wnd_sn=%d",wnd_sn);
		CViewMdbSvg *pNew = new CViewMdbSvg(wnd_sn,parent,true/*,&sExtAttr*/);
		pView = pNew;
		if(title.length() == 0)
			title = DB->SelectInto(sql,2);
		pNew->setWindowTitle(title.data());
		pNew->SetDatasetName(DB->SelectInto(sql));
		pNew->SetCondition(DB->SelectInto(sql,1));
	}
	else if(wnd_type == 2 || wnd_type == 4)//2-	ӳ�䵽�ڴ����ݱ�Ķ�̬���  4-	ӳ�䵽��ʷ���ݱ�����ݴ��ڣ���ѯ����
	{
		bool bMdb = DB->SelectIntoI("select count(*) from t_ssp_data_window where name='%s' and dbtype=3")>0;
		sql.sprintf("select dataset,ds_cond from t_ssp_uicfg_wnd where wnd_sn=%d",wnd_sn);
		SString ds = DB->SelectInto(sql);
		view_query *pNew = new view_query(parent,ds,bMdb);
		pView = pNew;
		pNew->m_iWndSn = wnd_sn;
		if(title.length() == 0)
			title = DB->SelectInto(SString::toFormat("select dw_desc from t_ssp_data_window where name='%s' ",ds.data()));
		pNew->setWindowTitle(title.data());
	}
	else if(wnd_type == 3)//3-	ӳ�䵽��ʷ���ݱ�����ݴ��ڣ���Ӧ���ݴ��������е�ĳһ���ݼ��������������ɹ������������Էֱ�ָ����ѯ/�޸�/���/ɾ������
	{
		sql.sprintf("select dataset,ds_cond from t_ssp_uicfg_wnd where wnd_sn=%d",wnd_sn);
		SString ds = DB->SelectInto(sql);
		view_common *pNew = new view_common(parent,ds);
		pView = pNew;
		pNew->m_iWndSn = wnd_sn;
		if(title.length() == 0)
			title = DB->SelectInto(SString::toFormat("select dw_desc from t_ssp_data_window where name='%s' ",ds.data()));
		pNew->setWindowTitle(title.data());
	}
	else
	{
		return NULL;
	}
	pView->OnPreShow();
	pView->OnAftShow();

	return (QWidget*)pView;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ���µĹ��ܵ㴰��
// ��    ��:  �ۿ���
// ����ʱ��:  2017-8-8 13:54
// ����˵��:  @sFunNameΪ���ܵ�����
//         :  @parentΪ������ָ��
//         :  @sExtAttrΪ��չ����
// �� �� ֵ:  CBaseView*, NULL��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
CBaseView* ssp_gui::NewView(SString sFunName,QWidget *parent,SString sExtAttr/*=""*/)
{
	CFrameMenuL2 l2;
	l2.m_iType = 0;
	l2.m_sFunName = sFunName;
	CBaseView *pView = ((CSspUiFrame*)m_pSspUiFrame)->NewView(&l2,parent);
	if(pView == NULL)
		return NULL;
	if(sExtAttr.length() > 0)
		pView->SetExtAttribute(sExtAttr);
	return pView;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ִ��ͨ�ö���
// ��    ��:  �ۿ���
// ����ʱ��:  2015-12-30 9:38
// ����˵��:  @act_snΪ�������
// �� �� ֵ:  <0��ʾʧ��(-1000��ʾ��δע��)��=0��ʾ����ȡ����>0��ʾ����ִ�гɹ�
//////////////////////////////////////////////////////////////////////////
int ssp_gui::RunGeneralAction(int act_sn)
{
	SString sql;
	sql.sprintf("select count(*) from t_ssp_general_action where act_sn=%d",act_sn);
	if(act_sn > 0 && DB->SelectIntoI(sql) > 0)
	{
		//ִ��ָ���Ķ����߼�
		sql.sprintf("select act_type,act_name,act_desc,act_tip,act_sql,succ_tip,fail_tip from t_ssp_general_action where act_sn=%d",act_sn);
		SRecordset rs;
		DB->Retrieve(sql,rs);
		if(rs.GetRows() <= 0)
			return -1001;
		int act_type = rs.GetValue(0,0).toInt();
		SString name = rs.GetValue(0,1);
		SString desc = rs.GetValue(0,2);
		SString act_tip = rs.GetValue(0,3);
		SString act_sql = rs.GetValue(0,4);
		SString succ_tip = rs.GetValue(0,5);
		SString fail_tip = rs.GetValue(0,6);
		if(act_tip.length() > 0)
		{
			if(!SQt::ShowQuestionBox(desc.data(),act_tip.data(),NULL))
				return 0;
		}
		bool ret = false;
		if(act_type == 1 || act_type == 2)
		{
			//����̬�Ự�������� {@SESSION:sub_no@}
			int p1,p2;
			SString sFldKey;
			p1 = act_sql.find("{@SESSION:");
			while(p1 >= 0)
			{
				p2 = act_sql.find("@}",p1);
				if(p2 < 0)
					break;
				sFldKey = act_sql.mid(p1+10,p2-p1-10);
				act_sql = act_sql.left(p1)+SSP_BASE->GetSessionAttributeValue(sFldKey.data())+act_sql.mid(p2+2);
				p1 = act_sql.find("{@SESSION:");
			}

			if(act_type == 1)
				ret = MDB->Execute(act_sql);
			else
				ret = DB->Execute(act_sql);
		}
		else if(act_type == 3)
		{
			int iret = SSP_GUI->RunRegistedCustomAction(name);
			if(iret < 0)
			{
				LOGERROR("ͨ�ö���(%s:%s)ִ��ʧ��:ret=%d",name.data(),desc.data(),iret);
			}
			if(iret == 0)
				return 0;//cancel
			ret = iret > 0;
		}
		if(ret)
		{
			if(succ_tip.length() > 0)
				SQt::ShowInfoBox(desc.data(),succ_tip.data(),NULL);
		}
		else
		{
			if(fail_tip.length() > 0)
				SQt::ShowErrorBox(desc.data(),fail_tip.data(),NULL);
		}
		return ret;
	}
	return -1002;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ִ��Ӧ�ò���ע����Զ��嶯��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-21 15:30
// ����˵��:  @sActNameΪ��������
// �� �� ֵ:  <0��ʾʧ��(-1000��ʾ��δע��)��=0��ʾ����ȡ����>0��ʾ����ִ�гɹ�
//////////////////////////////////////////////////////////////////////////
int ssp_gui::RunRegistedCustomAction(SString sActName)
{
	SSP_GeneralAction p = SearchGeneranActionFun(sActName);
	if(p == NULL)
	{
		LOGERROR("ָ����ͨ�ö���(%s)��δע��!��ע�ắ��%d��",sActName.data(),m_GeneralActions.count());
		unsigned long pos;
		stuGeneralActionParam *p0 = m_GeneralActions.FetchFirst(pos); 
		while(p0)
		{
			LOGERROR("��ע�ắ��:%s -> %p",p0->sActName.data(),p0->pFun);
			p0 = m_GeneralActions.FetchNext(pos);
		}
		LOGERROR("ָ����ͨ�ö���(%s)��δע��!��ע�ắ��%d��",sActName.data(),m_GeneralActions.count());
		return -1;
	}
	return p();
}
//////////////////////////////////////////////////////////////////////////
// ��    ��:  ע��ͨ�ö����������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-21 15:37
// ����˵��:  @sActNameΪȫ��Ψһ�Ķ����ؼ���
//         :  @pFunΪ������ڵľ�̬�ص�����
// �� �� ֵ:  true��ʾע��ɹ���false��ʾע��ʧ�ܣ���ؼ����Ѿ���ע�����
//////////////////////////////////////////////////////////////////////////
bool ssp_gui::RegisterGeneralAction(SString sActName,SSP_GeneralAction pFun)
{
	SSP_GeneralAction p = SearchGeneranActionFun(sActName);
	if(p != NULL)
	{
		LOGERROR("ע��ͨ�ö��������Ѵ���!%s -> %p",sActName.data(),pFun);
		return false;
	}
	LOGERROR("ע��ͨ�ö�������:%s -> %p, ����%d������!",sActName.data(),pFun,m_GeneralActions.count());
	stuGeneralActionParam *pNew = new stuGeneralActionParam();
	pNew->sActName = sActName;
	pNew->pFun = pFun;
	m_GeneralActions.append(pNew);
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ���ؼ�������ͨ�ö�����ڻص�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-21 15:41
// ����˵��:  @sActNameΪ�����ؼ���
// �� �� ֵ:  SSP_GeneralAction,NULL��ʾָ��������δע��
//////////////////////////////////////////////////////////////////////////
SSP_GeneralAction ssp_gui::SearchGeneranActionFun(SString sActName)
{
	unsigned long pos;
	stuGeneralActionParam *p = m_GeneralActions.FetchFirst(pos);
	while(p)
	{
		if(p->sActName == sActName)
			return p->pFun;
		p = m_GeneralActions.FetchNext(pos);
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ����SVG��ʾ�ı�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-12-8 14:50
// ����˵��:  @pSvgObjΪͼԪ����ָ��
//         :  @sTextΪ���÷��ص��ı�����
// �� �� ֵ:  true��ʾ��ȡ�ɹ���false��ʾ��ȡʧ��
//////////////////////////////////////////////////////////////////////////
bool ssp_gui::GetSvgObjectTipText(SSvgObject *pSvgObj,SString &sText)
{
	unsigned long pos=0;
	SSP_GetSvgObjectTipText* ppFun = m_GetSvgObjectTipTextFun.FetchFirst(pos);
	while(ppFun)
	{
		if((*ppFun)(pSvgObj,sText))
			return true;
		ppFun = m_GetSvgObjectTipTextFun.FetchNext(pos);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ע��ȡָ����SVG��ʾ�ı����ݵľ�̬�ص�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-12-8 14:56
// ����˵��:  @pFun
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void ssp_gui::RegisterGetSvgobjectTipCallback(SSP_GetSvgObjectTipText pFun)
{
	m_GetSvgObjectTipTextFun.append(pFun);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���������ܵ�״̬���Ҳ�����Զ���WIDGET�ؼ���Ӧ�ò���Կ��Ƹÿؼ��������Լ���ȡ����Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2016-1-11 11:21
// ����˵��:  @pWidget
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void ssp_gui::AddStatusWidget(QWidget *pWidget)
{
	((CSspUiFrame*)m_pSspUiFrame)->AddStatusWidget(pWidget);
	//pWidget->setParent(((CSspUiFrame*)m_pSspUiFrame)->m_pWndBottom);
	//((CSspUiFrame*)m_pSspUiFrame)->m_pWndBottom->horizontalLayout->addWidget(pWidget);

	
	//pWidget->setParent(((CSspUiFrameBlueStyle*)m_pSspUiFrame)->returnBottomWidget());
	//((CSspUiFrameBlueStyle*)m_pSspUiFrame)->returnBottomLayout()->addWidget(pWidget);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʾ���Ժ򴰿�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-1-20 16:35
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
void ssp_gui::ShowWaiting()
{
	((CSspUiFrame*)m_pSspUiFrame)->ShowWaiting();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �ر����Ժ򴰿�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-1-20 16:36
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
void ssp_gui::HideWaiting()
{
	((CSspUiFrame*)m_pSspUiFrame)->HideWaiting();
}

#include "view_mdb_svg.h"
//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����һ�ε����SVG��̬�����ض����µĴ��ں�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-3-1 18:19
// ����˵��:  @wnd_snΪ��̬���ں�
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void ssp_gui::JumpToSvgUiWnd(int wnd_sn)
{
	CViewMdbSvg::JumpWnd(wnd_sn);
}

