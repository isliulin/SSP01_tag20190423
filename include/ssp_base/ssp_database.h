/**
 *
 * �� �� �� : ssp_database.h
 * �������� : 2015-7-29 19:28
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ���ݿ�������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-29	�ۿ�������ļ�
 *
 **/

#ifndef __SSP_DATABASE_CONFIG_H__
#define __SSP_DATABASE_CONFIG_H__

#include "sp_config_mgr.h"
#include "SDatabase.h"
#include "SDatabaseOper.h"

#include "SMdb.h"
#include "SApi.h"
#include "ssp_base_inc.h"

//ȡ���ݿ�����ʵ��
#define GET_DB_CFG ((CSsp_Database*)CConfigMgr::GetConfig(SPCFG_DB_CONFIG))

//ȡ��ʷ�������
#define DB GET_DB_CFG->GetHisDbOper()

//ȡ�ڴ�������
#define MDB GET_DB_CFG->GetMdbOper()

typedef e_SDatabase_Type eSsp_Database_Type;
//#define eSsp_Database_Type e_SDatabase_Type

class SSP_BASE_EXPORT CSsp_Database : public CConfigBase
{
public:
	CSsp_Database();
	virtual ~CSsp_Database();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �豸���ӳش�С��Ӧ��Load֮ǰ�������ã��ò��������������ڴ��ͬʱ��Ч��Ĭ��Ϊ3
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 13:47
	// ����˵��:  @poolsizeΪ�ش�С�����������ݿ�ĻỰ����
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetPoolSize(int poolsize){m_iPoolSize = poolsize;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ���������ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-29 19:31
	// ����˵��:  @sPathFileΪ���ݿ������ļ���
	// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Load(SString sPathFile);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���������ڴ����ݿ�������ַ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-29 20:04
	// ����˵��:  void
	// �� �� ֵ:  SString,����SDatabaseʵ�������Ӳ���
	//////////////////////////////////////////////////////////////////////////
	SString GetMasterConnectString();
	SString GetSlaveConnectString();
	SString GetMdbConnectString();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ʷ���ݿ������ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 14:46
	// ����˵��:  void
	// �� �� ֵ:  SDatabaseOper*,NULL��ʾ������
	//////////////////////////////////////////////////////////////////////////
	inline SDatabaseOper* GetHisDbOper(){return &m_HisDbOper;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�ڴ����ݿ������ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 14:47
	// ����˵��:  void
	// �� �� ֵ:  SDatabaseOper*,NULL��ʾ������
	//////////////////////////////////////////////////////////////////////////
	inline SDatabaseOper* GetMdbOper(){return &m_MdbOper;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������MDB���ڴ津����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-5-11 16:49
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void RemoveAllMdbTrigger();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�����ݿ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-11-7 15:46
	// ����˵��:  void
	// �� �� ֵ:  void
	inline eSsp_Database_Type GetMasterType(){return m_MasterDbType;}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�������ݿ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-11-7 15:46
	// ����˵��:  void
	// �� �� ֵ:  void
	inline eSsp_Database_Type GetSlaveType(){return  m_SlaveDbType;}

private:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ӳ���Դ����ʱ�ͷ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 14:05
	// ����˵��:  @lpΪthis
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadDelayFree(void *lp);

	bool	m_bMaster;
	SString m_sMasterType;//mysql/pgsql/oracle/dameng/mdb
	eSsp_Database_Type m_MasterDbType;
	SString m_sMasterHostAddr;
	int		m_iMasterPort;
	SString m_sMasterUser;
	SString m_sMasterPassword;
	SString m_sMasterDbName;

	bool	m_bSlave;
	SString m_sSlaveType;//mysql/pgsql/oracle/dameng/mdb
	eSsp_Database_Type m_SlaveDbType;
	SString m_sSlaveHostAddr;
	int		m_iSlavePort;
	SString m_sSlaveUser;
	SString m_sSlavePassword;
	SString m_sSlaveDbName;

	bool	m_bMdb;
	SString m_sMdbHostAddr;
	int		m_iMdbPort;
	SString m_sMdbUser;
	SString m_sMdbPassword;
	SString m_sMdbDbName;

	SDatabaseOper m_HisDbOper;						 //��ʷ���ݿ������
	SDatabaseOper m_MdbOper;						 //�ڴ����ݿ������
	SDatabasePool<SDatabase> *m_pHisMasterDbPools;	 //����ʷ���ݿ����ӳ�
	SDatabasePool<SDatabase> *m_pHisSlaveDbPools;	 //����ʷ���ݿ����ӳ�
	SDatabasePool<SDatabase> *m_pMdbDbPools;		 //�ڴ����ݿ����ӳ�
	SDatabasePool<SDatabase> *m_pOldHisMasterDbPools;//��ɾ���Ĺ�������ʷ���ݿ����ӳ�
	SDatabasePool<SDatabase> *m_pOldHisSlaveDbPools; //��ɾ���Ĺ��ڱ���ʷ���ݿ����ӳ�
	SDatabasePool<SDatabase> *m_pOldMdbDbPools;		 //��ɾ���Ĺ����ڴ����ݿ����ӳ�
	int m_iPoolSize;								 //���ӳش�С
};


#define C_BATCH_DML_MAX_BUFFERSIZE 65536
//���ݿ�ִ���첽ͬ��������ʱ�ı��
#define C_SQL_ASYNC_RUN "/*async*/"

class SSP_BASE_EXPORT CSsp_DmlHelper
{
public:
	CSsp_DmlHelper();
	~CSsp_DmlHelper();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰʵ����Ӧ�����ݿ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-15 16:14
	// ����˵��:  void
	// �� �� ֵ:  eSsp_Database_Type
	//////////////////////////////////////////////////////////////////////////
	inline eSsp_Database_Type GetDbType(){return m_eDbType;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��SQL�������������תΪ�ַ��������ֶ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-15 16:14
	// ����˵��:  @sFieldΪ�ֶ�����
	// �� �� ֵ:  ���ݲ�ͬ�����ݿ����Ͷ���ͬ
	//////////////////////////////////////////////////////////////////////////
	SString Date2String(SString sField);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��SQL������ַ�����Ϊ��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-15 16:14
	// ����˵��:  @sDateTime��ʽΪ:yyyy-MM-dd hh:mm:ss
	// �� �� ֵ:  ���ݲ�ͬ�����ݿ����Ͷ���ͬ
	//////////////////////////////////////////////////////////////////////////
	SString String2Date(SString sDateTime);


protected:
	SDatabaseOper *m_pDbOper;//���ݿ�ָ��
	SDatabase *m_pDb;
	eSsp_Database_Type m_eDbType;//���ݿ�����
	bool m_bSqlAsyncRun;//���ݿ�SQL���첽����ִ�е�SQLǰ����Ƿ�,true��ʾ���첽,false��ʾ�����첽
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CSsp_BatchDmlInsert
// ��    ��:  �ۿ���
// ����ʱ��:  2016-12-15 10:51
// ��    ��:  ���������
//////////////////////////////////////////////////////////////////////////
class SSP_BASE_EXPORT CSsp_BatchDmlInsert : public CSsp_DmlHelper
{
public:
	CSsp_BatchDmlInsert();
	~CSsp_BatchDmlInsert();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-15 13:14
	// ����˵��:  @pDbΪ���ݿ������ָ��
	//         :  @dbTypeΪ���ݿ�����
	//         :  @sTableName��ʾ������
	//         :  @sFields��ʾ�ֶ��������ŷָ�����:col1,col2,col3
	//         :  @iTimeoutSec��ʾ��ʱʱ�䣬��λ:��
	//         :  @bAsyncΪ���ݿ�SQL���첽����ִ�е�SQLǰ����Ƿ�,true��ʾ���첽,false��ʾ�����첽
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void Init(SDatabaseOper *pDb,eSsp_Database_Type dbType,char* sTableName,char* sFields,int iTimeoutSec=5,bool bAsync=true);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ʹ��SDatabase��ʼ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-24 19:14
	// ����˵��:  @pDbΪ���ݿ���ָ��
	//         :  @iTimeoutSec��ʾ��ʱʱ�䣬��λ:��
	//         :  @bAsyncΪ���ݿ�SQL���첽����ִ�е�SQLǰ����Ƿ�,true��ʾ���첽,false��ʾ�����첽
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void Init(SDatabase *pDb,char* sTableName,char* sFields,int iTimeoutSec=5,bool bAsync=true);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���һ����ֵ���ݣ��绺���С�������ִ�����л���SQL
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-15 13:15
	// ����˵��:  @sValues,Ϊ��Ӧ�����ֶε�ֵ�����ŷָ�����:1,2,'xx'
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AddInsertValues(SString sValues);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����һ�γ�ʱ��飬�絽�ﳬʱʱ�䣬����ִ�л���SQL
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-15 13:16
	// ����˵��:  @now_soc��ʾ��ǰʱ�䣬0��ʾ������ʱ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void CheckTimeOut(int now_soc=0);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ִ�л���SQL
	// ��    ��:  Ǯ����
	// ����ʱ��:  2018-01-19 10:02
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void Flush();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����insert SQL��β���ַ���������MYSQL�Ĳ���ת���£�
	//         :  ��:on duplicate key update current_val=values(current_val),soc=values(soc) ��Ϊβ��
	//         :        �������[ insert into t_oe_element_state (st_sn,current_val,soc) values (1,0,100),(2,1,101) ]�ĺ���ʵ�ֲ���ת���£��ҿ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2019-4-12 8:28
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void SetSqlTail(SString tail){m_sSqlTail = tail;}

private:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ִ��һ��SQL�����������л���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-15 13:18
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void ExecuteSql();

	SString m_sTableName;//��Ӧ�ı���
	SString m_sFields;//��Ӧ���ֶ��������ŷָ�����:col1,col2,col3
	int m_iFirstSqlSoc;//��һ������ʱ�䣬0��ʾ����δִ�е�SQL
	int m_iTimeoutSecs;//��ʱʱ�䣬��λ:��
	SString m_sFullSql;
	SString m_sSqlTail;//insert SQL��β���ַ����������MYSQl���ݿ⣬�����ڲ���������ͻʱ��Ϊupdate���������ܽ�������insert/update��insert into t_oe_element_state (st_sn,current_val,soc) values (1,0,100),(2,1,101) on duplicate key update current_val=values(current_val),soc=values(soc)
	SLock m_Lock;
};

class SSP_BASE_EXPORT CSsp_BatchDml : public CSsp_DmlHelper
{
public:
	CSsp_BatchDml();
	~CSsp_BatchDml();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ʹ��SDatabaseOper��ʼ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-15 13:14
	// ����˵��:  @pDbΪ���ݿ������ָ��
	//         :  @dbTypeΪ���ݿ�����
	//         :  @iTimeoutSec��ʾ��ʱʱ�䣬��λ:��
	//         :  @bAsyncΪ���ݿ�SQL���첽����ִ�е�SQLǰ����Ƿ�,true��ʾ���첽,false��ʾ�����첽
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void Init(SDatabaseOper *pDb,eSsp_Database_Type dbType,int iTimeoutSec=5,bool bAsync=true);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ʹ��SDatabase��ʼ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-8-24 19:14
	// ����˵��:  @pDbΪ���ݿ���ָ��
	//         :  @iTimeoutSec��ʾ��ʱʱ�䣬��λ:��
	//         :  @bAsyncΪ���ݿ�SQL���첽����ִ�е�SQLǰ����Ƿ�,true��ʾ���첽,false��ʾ�����첽
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void Init(SDatabase *pDb,int iTimeoutSec=5,bool bAsync=true);


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���һ��SQL��䵽���������绺����������ִ�в���ջ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-15 13:22
	// ����˵��:  @sqlΪ��һSQL��䣬һ��Ϊdelete update
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AddSql(SString sql);


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����һ�γ�ʱ��飬�絽�ﳬʱʱ�䣬����ִ�л���SQL
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-15 13:16
	// ����˵��:  @now_soc��ʾ��ǰʱ�䣬0��ʾ������ʱ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void CheckTimeOut(int now_soc=0);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ִ�л���SQL
	// ��    ��:  Ǯ����
	// ����ʱ��:  2018-01-19 10:02
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void Flush();

private:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ִ��һ��SQL�����������л���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-15 13:18
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void ExecuteSql();

	int m_iFirstSqlSoc;//��һ������ʱ�䣬0��ʾ����δִ�е�SQL
	int m_iTimeoutSecs;//��ʱʱ�䣬��λ:��
	SString m_sFullSql;
	SLock m_Lock;
};


#endif//__SSP_DATABASE_CONFIG_H__
