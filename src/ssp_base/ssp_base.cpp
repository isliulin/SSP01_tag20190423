/**
 *
 * �� �� �� : ssp_base.cpp
 * �������� : 2015-11-9 19:14
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SSP���ƽ̨������ͷ�ļ�
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-11-9	�ۿ�������ļ�
 *
 **/

#include "ssp_base.h"
#include "SDateTime.h"

ssp_base* g_p_ssp_base = NULL;
ssp_base::ssp_base()
{
	m_iOperUserSn = m_iLoginUserSn = m_iGuarderUserSn = 0;
	m_SessionAttributes.setAutoDelete(true);
	m_SessionAttributes.setShared(true);
	g_p_ssp_base = this;
}


ssp_base::~ssp_base()
{
	m_SessionAttributes.clear();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡΨһʵ��ָ��
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-10 15:14
// ����˵��:  void
// �� �� ֵ:  ssp_base*
//////////////////////////////////////////////////////////////////////////
ssp_base* ssp_base::GetPtr()
{
	if(g_p_ssp_base == NULL)
	{
		static ssp_base base;
		g_p_ssp_base = &base;
	}
	return g_p_ssp_base;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �豸Ψһʵ��ָ�루��̬����ʹ�ã�
// ��    ��:  �ۿ���
// ����ʱ��:  2017-8-4 17:04
// ����˵��:  @ptr��ʾʵ��ָ��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void ssp_base::SetPtr(ssp_base* ptr)
{
	g_p_ssp_base = ptr;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����µ�ϵͳ��־��¼����ʷ���ݿ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-9 19:45
// ����˵��:  @iLogType��ʾ��־����
//         :  @iLogLevel��ʾ��־����
//         :  @sLogText��ʾ��־�����ַ���
//         :  @id1~id4��ʾ4����ʶ��
// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
//////////////////////////////////////////////////////////////////////////
bool ssp_base::NewSysLog(int iLogType,int iLogLevel,const char* sLogText,int id1,int id2,int id3,int id4)
{
	int soc,usec;
	SDateTime::getSystemTime(soc,usec);
	SString sql;
	int oper_sn = m_iOperUserSn;
	if(oper_sn == 0)
		oper_sn = m_iLoginUserSn;
	sql.sprintf("insert into t_ssp_syslog (soc,usec,usr_sn,monitor_sn,log_type,log_level,id1,id2,id3,id4,log_text) "
		"values(%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s')",
		soc,usec,oper_sn,m_iGuarderUserSn,iLogType,iLogLevel,id1,id2,id3,id4,sLogText);
	if(!DB->Execute(sql))
	{
		LOGERROR("����ϵͳ��־ʱʧ��!sql=%s",sql.data());
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡȫ������ֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-22 9:48
// ����˵��:  @sCfgKey��ʾȫ��Ψһ�����ùؼ���
//         :  @sDefault��ʾȱʡֵ
// �� �� ֵ:  ����ֵ������������ʱ����ȱʡֵ
//////////////////////////////////////////////////////////////////////////
SString ssp_base::GetGlobalConfig(SString sCfgKey,SString sDefault/*=""*/)
{
	SString sql;
	SRecordset rs;
	sql.sprintf("select set_value from t_ssp_setup where set_key='%s'",sCfgKey.data());
	DB->Retrieve(sql,rs);
	if(rs.GetRows() <= 0)
		return sDefault;
	else
		return rs.GetValue(0,0);
}

int ssp_base::GetGlobalConfigI(SString sCfgKey,int iDefault/*=0*/)
{
	SString sql;
	SRecordset rs;
	sql.sprintf("select set_value from t_ssp_setup where set_key='%s'",sCfgKey.data());
	DB->Retrieve(sql,rs);
	if(rs.GetRows() <= 0)
		return iDefault;
	else
		return rs.GetValue(0,0).toInt();
}

float ssp_base::GetGlobalConfigF(SString sCfgKey,float fDefault/*=0*/)
{
	SString sql;
	SRecordset rs;
	sql.sprintf("select set_value from t_ssp_setup where set_key='%s'",sCfgKey.data());
	DB->Retrieve(sql,rs);
	if(rs.GetRows() <= 0)
		return fDefault;
	else
		return rs.GetValue(0,0).toFloat();
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ȫ������ֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2016-2-22 9:54
// ����˵��:  @sCfgKey��ʾȫ��Ψһ�����ùؼ���
//         :  @sValue��ʾֵ����
// �� �� ֵ:  true��ʾ���óɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool ssp_base::SetGlobalConfig(SString sCfgKey,SString sValue)
{
	SString sql;
	sql.sprintf("update t_ssp_setup set set_value='%s' where set_key='%s'",sValue.data(),sCfgKey.data());
	return DB->Execute(sql);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �жϵ�Ȼ�Ự�����Ƿ����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-10 11:30
// ����˵��:  @sAttrNameΪ�Ự����
// �� �� ֵ:  true��ʾ���Դ��ڣ�false��ʾ���Բ�����
//////////////////////////////////////////////////////////////////////////
bool ssp_base::IsSessionAttribute(const char* sAttrName)
{
	unsigned long pos;
	SString sName = sAttrName;
	sName.toLower();
	stuSessionAttribute *p = m_SessionAttributes.FetchFirst(pos);
	while(p)
	{
		if(p->m_sName == sName)
			return true;
		p = m_SessionAttributes.FetchNext(pos);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��idx���Ự���Ե�����
// ��    ��:  �ۿ���
// ����ʱ��:  2017-12-13 17:10
// ����˵��:  @idx
// �� �� ֵ:  SString
//////////////////////////////////////////////////////////////////////////
SString ssp_base::GetIdSessionAttributeName(int idx)
{
	stuSessionAttribute *p = m_SessionAttributes[idx];
	if(p == NULL)
		return "";
	return p->m_sName;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��idx���Ự���Ե����ƺ�ֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2017-12-13 17:12
// ����˵��:  @idx
//         :  @name
//         :  @value
// �� �� ֵ:  true��ʾ���ڣ�false��ʾ������
//////////////////////////////////////////////////////////////////////////
bool ssp_base::GetIdSessionAttribute(int idx,SString &name,SString &value)
{
	stuSessionAttribute *p = m_SessionAttributes[idx];
	if(p == NULL)
		return false;
	name = p->m_sName;
	value = p->m_sValue;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ���Ự���Ե�ֵ
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-10 11:31
// ����˵��:  @sAttrNameΪ�Ự��������
// �� �� ֵ:  SString
//////////////////////////////////////////////////////////////////////////
SString ssp_base::GetSessionAttributeValue(const char* sAttrName)
{
	unsigned long pos;
	SString sName = sAttrName;
	sName.toLower();
	stuSessionAttribute *p = m_SessionAttributes.FetchFirst(pos);
	while(p)
	{
		if(p->m_sName == sName)
			return p->m_sValue;
		p = m_SessionAttributes.FetchNext(pos);
	}
	return "";
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ���ĻỰ����ֵ����������ڸ��������½�����
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-10 11:32
// ����˵��:  @sAttrNameΪ�������� 
//         :  @sValueΪ����ֵ
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void ssp_base::SetSessionAttributeValue(const char* sAttrName,SString sValue)
{
	unsigned long pos;
	SString sName = sAttrName;
	sName.toLower();
	stuSessionAttribute *p = m_SessionAttributes.FetchFirst(pos);
	while(p)
	{
		if(p->m_sName == sName)
		{
			p->m_sValue = sValue;
			return;
		}
		p = m_SessionAttributes.FetchNext(pos);
	}
	p = new stuSessionAttribute();
	p->m_sName = sName;
	p->m_sValue = sValue;
	m_SessionAttributes.append(p);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ûỰ���Լ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-10 15:31
// ����˵��:  @sAttrsΪ���Լ����ݣ���ʽ�磺name1=value1;name2=value2;
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void ssp_base::SetSessionAttributeValues(SString sAttrs)
{
	SString sLine,name,value;
	int i,cnt = SString::GetAttributeCount(sAttrs,";");
	for(i=1;i<=cnt;i++)
	{
		sLine = SString::GetIdAttribute(i,sAttrs,";");
		name  = SString::GetIdAttribute(1,sLine,"=");
		//���Բ����ĵ����ԣ��磺������SVGͼԪЭ��������
		if(name == "type" ||
			name == "ds" ||
			name == "to" ||
			name == "row" ||
			name == "col" ||
			name == "dime" ||
			name == "fc" ||
			name == "lc" ||
			name.left(3) == "fc_" ||
			name.left(3) == "lc_" ||
			name == "userdraw" ||
			name == "t" ||
			name == "tcor" ||
			name == "lcor" 
			)
			continue;
		value = SString::GetIdAttribute(2,sLine,"=");
		SetSessionAttributeValue(name.data(),value);
	}
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ��ָ���ĻỰ������
// ��    ��:  �ۿ���
// ����ʱ��:  2015-11-10 14:54
// ����˵��:  @sAttrNameΪ��ɾ����������
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool ssp_base::RemoveSessionAttribute(const char* sAttrName)
{
	unsigned long pos;
	SString sName = sAttrName;
	sName.toLower();
	stuSessionAttribute *p = m_SessionAttributes.FetchFirst(pos);
	while(p)
	{
		if(p->m_sName == sName)
		{
			m_SessionAttributes.remove(p);
			return true;
		}
		p = m_SessionAttributes.FetchNext(pos);
	}
	return false;
}


