#include "clearData.h"


clearData::clearData(void)
{
}


clearData::~clearData(void)
{
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���������������
// ��    ��:  �ұ���
// ����ʱ��:  2015-7-30 10:30
// ����˵��:  @dbΪ���ݿ����Ӷ���ָ��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void clearData::ClearUnusefulData(/*SDatabase * db*/)
{
	//BOOLCHECKRETURN(db->TestConnect(),QObject::tr("���ݿ�����ʧ�ܣ�"),);
	FALSECHECKINFOSTART
	/**********����û����ж�������***********/
	SString sSql;
	sSql = "delete from t_ssp_user where grp_code not in(select grp_code from t_ssp_user_group)";
	FALSECHECKINFORECORD(DB->ExecuteSQL(sSql),"t_ssp_user");
	/**********����û����ж�������***********/


	/**********������ܵ���ж�������***********/
// 	sSql = "delete from t_ssp_fun_point where p_fun_key not in (select fun_key  from (select fun_key from t_ssp_fun_point)a) and p_fun_key is not null";
// 	FALSECHECKINFORECORD(DB->ExecuteSQL(sSql),"t_ssp_fun_point");
	/**********������ܵ���ж�������***********/


	/**********���Ⱥ��Ȩ�ޱ��ж�������***********/
	sSql = "delete from t_ssp_usergroup_auth where grp_code not in(select grp_code from t_ssp_user_group)";
	FALSECHECKINFORECORD(DB->ExecuteSQL(sSql),"t_ssp_usergroup_auth");
	sSql = "delete from t_ssp_usergroup_auth where fun_key not in(select fun_key from t_ssp_fun_point)";
	FALSECHECKINFORECORD(DB->ExecuteSQL(sSql),"t_ssp_usergroup_auth");
	/**********���Ⱥ��Ȩ�ޱ��ж�������***********/



	/**********����û�Ȩ�ޱ��ж�������***********/
	sSql = "delete from t_ssp_user_auth where usr_sn not in(select usr_sn from t_ssp_user)";
	FALSECHECKINFORECORD(DB->ExecuteSQL(sSql),"t_ssp_usergroup_auth");
	sSql = "delete from t_ssp_user_auth where fun_key not in(select fun_key from t_ssp_fun_point)";
	FALSECHECKINFORECORD(DB->ExecuteSQL(sSql),"t_ssp_usergroup_auth");
	/**********����û�Ȩ�ޱ��ж�������***********/
	// FALSECHECKINFOEND("����ʧ�ܣ�������ݿ����ӻ������ݿ��Ƿ����ı�");
}