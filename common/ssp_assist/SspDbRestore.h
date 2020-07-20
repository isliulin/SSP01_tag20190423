/**
*
* �� �� �� : SspDbRestore.h
* �������� : 2016-04-23 17:07
* ��    �� : SspAssist(skt001@163.com)
* �޸����� : $Date: 2016-04-23 17:07$
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

#ifndef __SAXEX_CSSPDBRESTORE_H__
#define __SAXEX_CSSPDBRESTORE_H__

#include "SSaxXml.h"
#include "SDatabase.h"
#include "ssp_gui.h"
#include "SQt.h"

class CSspDbRestore : public SSaxXml
{
public:
	enum eNodeLevel
	{
		LEV_NONE=1,
		LEV_ROOT,
		LEV_TABLE,
		LEV_TABLE_COL,
		LEV_TABLE_ROW,
		LEV_TABLE_ROW_BLOB,
	};

	CSspDbRestore();
	virtual ~CSspDbRestore();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ�µ�XML�ĵ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-04-23 17:07
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void StartDocument();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����XML�ļ��Ľ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-04-23 17:07
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void EndDocument();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼһ���µ�XML�ڵ�Ԫ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-04-23 17:07
	// ����˵��:  @sNodeName��ʾ�ڵ�����
	//         :  @pAttribute��ʾ�ڵ�����ָ�룬�ɺ�0~N������
	//         :  @iAttributeCount��ʾ�ڵ���������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void StartElement(char* sNodeName,SSaxAttribute *pAttribute,int iAttributeCount);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����һ��XML�ڵ�Ԫ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-04-23 17:07
	// ����˵��:  @sNodeName��ʾ�ڵ�����
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void EndElement(char* sNodeName);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����¼�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-04-23 17:07
	// ����˵��:  @iErrCode��ʾ�������
	//         :  @sErrText��ʾ��������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void Error(int iErrCode,SString sErrText);

	virtual void ProcessEndSql()=0;

	SDatabase *m_pDb;
	eNodeLevel m_NodeLevel;
	SString m_sTableName;
	SString m_sql;
	SString m_sBlobColName;
	SString m_sBlobWhere;
	int m_iBlobSize;
	BYTE *m_pBlobBuffer;
	int m_iBlobBufferLen;
	bool m_bTrunkTab;//�Ƿ�ԭǰ�������

	int m_iSaveTabs;
	int m_iSaveRows;

	int m_iDbErrors;
	SString m_sTempVal;
};

class CSspDbRestoreMysql : public CSspDbRestore
{
public:
	CSspDbRestoreMysql(){};
	virtual ~CSspDbRestoreMysql(){};
	virtual void Characters(char* sNodeValue);
	virtual void ProcessEndSql();
};

class CSspDbRestoreOracle : public CSspDbRestore
{
public:
	CSspDbRestoreOracle(){};
	virtual ~CSspDbRestoreOracle(){};
	virtual void Characters(char* sNodeValue);
	virtual void ProcessEndSql();
};


#endif
