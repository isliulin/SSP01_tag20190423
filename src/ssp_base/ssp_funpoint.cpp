/**
 *
 * �� �� �� : ssp_funpoint.cpp
 * �������� : 2015-7-29 19:19
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ϵͳ���ܵ㶨��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-29	�ۿ�������ļ�
 *
 **/

#include "ssp_funpoint.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CSsp_FunPointConfig
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-29 19:20
// ��    ��:  ���ܵ�������
//////////////////////////////////////////////////////////////////////////

CSsp_FunPointConfig::CSsp_FunPointConfig()
{
	SetConfigType(SPCFG_FUN_POINT);
}

CSsp_FunPointConfig::~CSsp_FunPointConfig()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ���������ļ�,������LoadByDb����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-29 19:24
// ����˵��:  @sPathFileΪ�����ļ���
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CSsp_FunPointConfig::Load(SString sPathFile)
{
	CConfigBase::Load(sPathFile);
	SFile f(sPathFile);
	if(f.open(IO_ReadOnly) == false)
		return false;
	m_sFunPointXmlText = "";
	while(!f.atEnd())
		m_sFunPointXmlText += f.readLine();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����ݿ���ع��ܵ������ļ���������Load����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 16:37
// ����˵��:  @pDbOperΪ���ݿ������
//         :  @sUserCodeΪ�û����� 
// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool CSsp_FunPointConfig::LoadByDb(SDatabaseOper *pDbOper,SString sUserCode)
{
	SString sql;
	SRecordset rs1,rs2,rs3;
	int i,j,k,cnt1,cnt2,cnt3;
	SString pkey;
	sql = "select fun_key,type,ref_sn,name,dsc,path from t_ssp_fun_point where type<>4 and (p_fun_key='' or p_fun_key is null)";
	cnt1 = pDbOper->RetrieveRecordset(sql,rs1);
	SString sXml = "<?xml version='1.0' encoding='GBK'?>\r\n";
	if(cnt1 <= 0)
		return false;
	sXml += DbNodeToXml(rs1.GetRecord(0),false);
	pkey = rs1.GetValue(0,0);
	rs1.clear();
	SString sql_key;
	//��һ��
	if(sUserCode.length() == 0)
	{
		sql.sprintf("select fun_key,type,ref_sn,name,dsc,path from t_ssp_fun_point where type<>4 and (auth<>1 or auth is null) and p_fun_key='%s' order by idx",pkey.data());
	}		
	else
	{
		sql_key.sprintf("select fun_key from t_ssp_usergroup_auth where auth=1 and grp_code in (select grp_code from t_ssp_user where usr_code='%s') "
			"union select fun_key from t_ssp_user_auth where auth=1 and usr_sn in (select usr_sn from t_ssp_user where usr_code='%s')",
			sUserCode.data(),sUserCode.data());
		sql.sprintf("select fun_key,type,ref_sn,name,dsc,path from t_ssp_fun_point where type<>4 and ( auth<>1 or auth is null or fun_key in (%s)) and p_fun_key='%s' order by idx",sql_key.data(),pkey.data());
	}
	cnt1 = pDbOper->RetrieveRecordset(sql,rs1);
	for(i=0;i<cnt1;i++)
	{
		sXml += "\t"+DbNodeToXml(rs1.GetRecord(i),false);
		pkey = rs1.GetValue(i,0);

		//�ڶ���
		if(sUserCode.length() == 0)
			sql.sprintf("select fun_key,type,ref_sn,name,dsc,path from t_ssp_fun_point where type<>4 and (auth<>1 or auth is null) and p_fun_key='%s' order by idx",pkey.data());
		else
			sql.sprintf("select fun_key,type,ref_sn,name,dsc,path from t_ssp_fun_point where type<>4 and ( auth<>1 or auth is null or fun_key in (%s)) and p_fun_key='%s' order by idx",sql_key.data(),pkey.data());
		cnt2 = pDbOper->RetrieveRecordset(sql,rs2);
		for(j=0;j<cnt2;j++)
		{
			sXml += "\t\t"+DbNodeToXml(rs2.GetRecord(j),false);
			pkey = rs2.GetValue(j,0);

			//������
			if(sUserCode.length() == 0)
				sql.sprintf("select fun_key,type,ref_sn,name,dsc,path from t_ssp_fun_point where type<>4 and (auth<>1 or auth is null) and p_fun_key='%s' order by idx",pkey.data());
			else
				sql.sprintf("select fun_key,type,ref_sn,name,dsc,path from t_ssp_fun_point where type<>4 and (auth<>1 or auth is null or fun_key in (%s)) and p_fun_key='%s' order by idx",sql_key.data(),pkey.data());
			cnt3 = pDbOper->RetrieveRecordset(sql,rs3);
			for(k=0;k<cnt3;k++)
			{
				sXml += "\t\t"+DbNodeToXml(rs3.GetRecord(k),true);
			}
			sXml += "\t\t</fun_point>\r\n";
		}

		sXml += "\t</fun_point>\r\n";
	}
	sXml += "</fun_point>\r\n";

	m_sFunPointXmlText = sXml;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  һ�����ݿ⹦�ܵ��¼תΪXML�﷨�ڵ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-30 16:53
// ����˵��:  @pRecordΪ��
// �� �� ֵ:  SString
//////////////////////////////////////////////////////////////////////////
SString CSsp_FunPointConfig::DbNodeToXml(SRecord *pRecord,bool bLastLevel)
{
	return SString::toFormat("<fun_point key='%s' name='%s' type='%d' ref_sn='%d' desc='%s' path='%s' %s> \r\n",
			pRecord->GetValue(0).data(),
			pRecord->GetValue(3).data(),
			pRecord->GetValue(1).toInt(),
			pRecord->GetValue(2).toInt(),
			pRecord->GetValue(4).data(),
			pRecord->GetValue(5).data(),
			bLastLevel?"/":"");
	
}

