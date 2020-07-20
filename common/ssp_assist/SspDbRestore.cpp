/**
*
* �� �� �� : SspDbRestore.cpp
* �������� : 2016-04-23 17:07
* ��    �� : SspAssist(skt001@163.com)
* �޸����� : $Date: 2016-04-23$
* ��ǰ�汾 : $Revision: 1.0.0$
* �������� : ����SSaxXmlEx���XML���͹��ܷ�װ
* �޸ļ�¼ : 
*            $Log: $
*
* Ver  Date        Author  Comments
* ---  ----------  ------  -------------------------------------------
* 001  2016-04-23        �������ļ�
*
**/

#include "SspDbRestore.h"

CSspDbRestore::CSspDbRestore()
{
	m_iBlobBufferLen = 0;
	m_pBlobBuffer = NULL;
}

CSspDbRestore::~CSspDbRestore()
{
	if(m_pBlobBuffer != NULL)
		delete[] m_pBlobBuffer;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʼ�µ�XML�ĵ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2016-04-23 17:07
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSspDbRestore::StartDocument()
{
	m_iSaveTabs = m_iSaveRows = m_iDbErrors = 0;
	m_NodeLevel = LEV_NONE;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����XML�ļ��Ľ���
// ��    ��:  �ۿ���
// ����ʱ��:  2016-04-23 17:07
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSspDbRestore::EndDocument()
{
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʼһ���µ�XML�ڵ�Ԫ��
// ��    ��:  �ۿ���
// ����ʱ��:  2016-04-23 17:07
// ����˵��:  @sNodeName��ʾ�ڵ�����
//         :  @pAttribute��ʾ�ڵ�����ָ�룬�ɺ�0~N������
//         :  @iAttributeCount��ʾ�ڵ���������
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSspDbRestore::StartElement(char* sNodeName,SSaxAttribute *pAttribute,int iAttributeCount)
{
	switch(m_NodeLevel)
	{
	case LEV_TABLE_ROW:
		if(SString::equals(sNodeName,"blob"))
		{
			m_NodeLevel = LEV_TABLE_ROW_BLOB;
			for(int i=0;i<iAttributeCount;i++)
			{
				register char *pStr = pAttribute[i].m_sAttrName.data();
				if(SString::equals(pStr,"name"))
					m_sBlobColName = pAttribute[i].m_sAttrValue;
				else if(SString::equals(pStr,"size"))
					m_iBlobSize = pAttribute[i].m_sAttrValue.toInt();
				else if(SString::equals(pStr,"where"))
					m_sBlobWhere = pAttribute[i].m_sAttrValue;
			}
		}
		break;
	case LEV_TABLE:
		if(SString::equals(sNodeName,"r"))
			m_NodeLevel = LEV_TABLE_ROW;
		else if(SString::equals(sNodeName,"column"))
			m_NodeLevel = LEV_TABLE_COL;
		break;
	case LEV_ROOT:
		if(SString::equals(sNodeName,"table"))
		{
			m_NodeLevel = LEV_TABLE;
			m_sTableName = "";
			for(int i=0;i<iAttributeCount;i++)
			{
				register char *pStr = pAttribute[i].m_sAttrName.data();
				if(SString::equals(pStr,"name"))
				{
					m_sTableName = pAttribute[i].m_sAttrValue;
					break;
				}
			}
			ProcessEndSql();
			if(m_bTrunkTab)
			{
				if(!m_pDb->Execute(SString::toFormat("truncate table %s",m_sTableName.data())))
					m_iDbErrors++;
			}
		}
		break;
	case LEV_NONE:
		if(SString::equals(sNodeName,"ssp_db"))
			m_NodeLevel = LEV_ROOT;
		break;
	}

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ��XML�ڵ�Ԫ��
// ��    ��:  �ۿ���
// ����ʱ��:  2016-04-23 17:07
// ����˵��:  @sNodeName��ʾ�ڵ�����
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSspDbRestore::EndElement(char* sNodeName)
{
	switch(m_NodeLevel)
	{
	case LEV_TABLE_ROW:
		m_NodeLevel = LEV_TABLE;
		m_iSaveRows ++;
		if(m_iSaveRows % 100 == 0)
		{
			SSP_GUI->SetStatusTip(SString::toFormat("�ѵ���%d�ű�,��%d����¼...",m_iSaveTabs,m_iSaveRows));
			SQt::ProcessUiEvents();
		}
		break;
	case LEV_TABLE_COL:
		m_NodeLevel = LEV_TABLE;
		break;
	case LEV_TABLE_ROW_BLOB:
		m_NodeLevel = LEV_TABLE_ROW;
		break;
	case LEV_TABLE:
		m_NodeLevel = LEV_ROOT;
		m_iSaveTabs ++;
		SSP_GUI->SetStatusTip(SString::toFormat("�ѵ���%d�ű�,��%d����¼...",m_iSaveTabs,m_iSaveRows));
		ProcessEndSql();
		SQt::ProcessUiEvents();
		break;
	case LEV_ROOT:
		m_NodeLevel = LEV_NONE;
		break;
	}

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �����¼�
// ��    ��:  �ۿ���
// ����ʱ��:  2016-04-23 17:07
// ����˵��:  @iErrCode��ʾ�������
//         :  @sErrText��ʾ��������
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSspDbRestore::Error(int iErrCode,SString sErrText)
{
	printf("error in saxxml!\n");
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �µĽڵ�ֵ����
// ��    ��:  �ۿ���
// ����ʱ��:  2016-04-23 17:07
// ����˵��:  @sNodeValue��ʾָ�����ַ����׵�ַ
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSspDbRestoreMysql::Characters(char* sNodeValue)
{
	//INSERT INTO t_ia_base_signal VALUES (3,'�غ�բ����',1,'�غ�բ.*����',1,NULL,NULL,NULL),(4,'��·��λ��',1,'(��·��)|(����)��λ',NULL,NULL,NULL,NULL)
	if(m_NodeLevel == LEV_TABLE_ROW)
	{
		register char *psVal = sNodeValue;
		if(m_sql.length() == 0)
			m_sql.sprintf("insert into %s values (%s)",m_sTableName.data(),psVal);
		else
			m_sql += SString::toFormat(",(%s)",psVal);
		if(m_sql.length() > 65536)
		{
			if(!m_pDb->Execute(m_sql))
			{
				m_iDbErrors ++;
			}
			m_sql = "";
		}
	}
	else if(m_NodeLevel == LEV_TABLE_ROW_BLOB)
	{
		if(m_iBlobSize>m_iBlobBufferLen)
		{
			if(m_pBlobBuffer != NULL)
				delete[] m_pBlobBuffer;
			m_iBlobBufferLen = m_iBlobSize;
			m_pBlobBuffer = new BYTE[m_iBlobBufferLen];
		}
		SString::StrToHex(sNodeValue,strlen(sNodeValue),m_pBlobBuffer);
		ProcessEndSql();
		if(!m_pDb->UpdateLobFromMem(m_sTableName,m_sBlobColName,m_sBlobWhere,m_pBlobBuffer,m_iBlobSize))
			m_iDbErrors ++;
	}
}

void CSspDbRestoreMysql::ProcessEndSql()
{
	if(m_sql.length() == 0)
		return;
	if(!m_pDb->Execute(m_sql))
		m_iDbErrors ++;
	m_sql = "";
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �µĽڵ�ֵ����
// ��    ��:  �ۿ���
// ����ʱ��:  2016-04-23 17:07
// ����˵��:  @sNodeValue��ʾָ�����ַ����׵�ַ
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSspDbRestoreOracle::Characters(char* sNodeValue)
{
	//insert all into t_oe_run_unit values(4,'a','111') into t_oe_run_unit values(5,'b','222') into t_oe_run_unit values(6,'c','333') select 1 from dual
	if(m_NodeLevel == LEV_TABLE_ROW)
	{
		register char *psVal = sNodeValue;
//		m_sql.sprintf("insert into %s values (%s)",m_sTableName.data(),psVal);
		if(m_sql.length() == 0)
			m_sql.sprintf("insert all into %s values (%s)",m_sTableName.data(),psVal);
		else
			m_sql += SString::toFormat(" into %s values (%s)",m_sTableName.data(),psVal);

		if(m_sql.length() > 10240)
		{
			m_sql += " select 1 from dual";
			if(!m_pDb->Execute(m_sql))
				m_iDbErrors ++;
			m_sql = "";
		}
	}
	else if(m_NodeLevel == LEV_TABLE_ROW_BLOB)
	{
		if(m_iBlobSize>m_iBlobBufferLen)
		{
			if(m_pBlobBuffer != NULL)
				delete[] m_pBlobBuffer;
			m_iBlobBufferLen = m_iBlobSize;
			m_pBlobBuffer = new BYTE[m_iBlobBufferLen];
		}
		SString::StrToHex(sNodeValue,strlen(sNodeValue),m_pBlobBuffer);
		ProcessEndSql();
		if(!m_pDb->UpdateLobFromMem(m_sTableName,m_sBlobColName,m_sBlobWhere,m_pBlobBuffer,m_iBlobSize))
			m_iDbErrors ++;
	}
}

void CSspDbRestoreOracle::ProcessEndSql()
{
	if(m_sql.length() == 0)
		return;
	m_sql += " select 1 from dual";
	if(!m_pDb->Execute(m_sql))
		m_iDbErrors ++;
	m_sql = "";
}
