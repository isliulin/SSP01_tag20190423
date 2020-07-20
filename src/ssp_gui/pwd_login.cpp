#include "pwd_login.h"
pwd_login::pwd_login(int ncount)
	:m_nCount(ncount)
{
	m_nSucess=0;
}
pwd_login::~pwd_login(void)
{
}
pwd_login::LOGSTATE pwd_login::logInSys(SDatabaseOper* pdb,SString s_user,SString s_pwd)
{
	if (s_user=="")
	{
		m_slogInfo=QObject::tr("�������û���!").toStdString().data();
		return NOIN;
	}
	if (s_pwd=="")
	{
		m_slogInfo=QObject::tr("����������!").toStdString().data();
		return NOIN;
	}
	SString sSql;
	SRecordset rs;
	if(pdb==NULL)
	{
		m_slogInfo=QObject::tr("�������Ӵ���!").toStdString().data();
		return NOIN;
	}
	sSql=SString::toFormat("select usr_code,pwd from t_ssp_user where usr_code='%s'",s_user.data());
	pdb->Retrieve(sSql,rs);
	if (rs.GetRows()==1&&rs.GetValue(0,1)==s_pwd)
	{
			m_nSucess++;
	}else
	{
		m_slogInfo=QObject::tr("��¼ʧ��!").toStdString().data();
		m_nSucess=0;
		return NOIN;
	}
	if (m_nSucess==m_nCount)
	{
		m_slogInfo=QObject::tr("����ɹ�").toStdString().data();
		m_nSucess=0;
		return ALLIN;
	}
	m_slogInfo=QObject::tr("�����ѵ���").toStdString().data();
	return ONEIN;
}