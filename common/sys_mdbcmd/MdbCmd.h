/**
*
* �� �� �� : MdbCmd.h
* �������� : 2014-11-28 13:38
* ��    �� : �ۿ���(skt001@163.com)
* �޸����� : $Date: $
* ��ǰ�汾 : $Revision: $
* �������� : MDB���ݿ����̨�ͻ���
* �޸ļ�¼ :
*            $Log: $
*
* Ver  Date        Author  Comments
* ---  ----------  ------  -------------------------------------------
* 001	2014-11-28	�ۿ�������ļ�
*
**/

#include "db/mdb/SMdb.h"
#include "SApi.h"
#include "SFile.h"
#include "db/mdb/SMdb.h"
class CMdbCmd
{
public:

	CMdbCmd();
	virtual ~CMdbCmd();

	bool Start();
	bool Stop();

	SString ProcessTelnetCmd(SMdb *pMdb,SString &cmd);
	void RunSqlFile(SString filename);

	static BYTE* OnMyMdbClientTrgCallback(void* cbParam, SString &sTable, eMdbTriggerType eType, int iTrgRows, int iRowSize, BYTE *pTrgData);
	SMdb m_mdb;
};
