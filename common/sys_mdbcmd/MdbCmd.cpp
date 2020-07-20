/**
*
* �� �� �� : MdbCmd.cpp
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

#include "MdbCmd.h"

CMdbCmd::CMdbCmd()
{
}

CMdbCmd::~CMdbCmd()
{

}
SString TipAndInput(SString str,SString def)
{
	printf(str.data());
	if (def.length() > 0)
		printf("(ȱʡ:%s):", def.data());
	char buf[10240];
	memset(buf, 0, sizeof(buf));
#ifndef WIN32
	gets(buf);
#else
	gets_s(buf, sizeof(buf));
#endif
	SString ret = buf;
	if (ret == "")
	{
		ret = def;
	}
	return ret;
}
bool CMdbCmd::Start()
{
	//SKT_CREATE_THREAD(ThreadListen, this);
	SString cmd, sRet;
	again:
	{
		SString ip = TipAndInput("���������ݿ�IP:", "127.0.0.1");
		int port = TipAndInput("���������ݿ�˿�:", "1400").toInt();
		SString sParam = SString::toFormat("hostaddr=%s;port=%d;user=skt_mdb;password=skt001@163.com;", ip.data(), port);
		m_mdb.SetParams(sParam.data());
	}
	if (!m_mdb.Connect())
	{
		printf("�������ݿ�ʧ��!\n");
		goto again;
	}
	else
	{
		printf("�������ݿ�ɹ�!\n");
	}
	cmd = "help";
	cmd = ProcessTelnetCmd(&m_mdb, cmd);
	printf(cmd.data());
	SApi::UsSleep(10000);
	cmd = "";
	while (1)
	{
		cmd = TipAndInput("[s-mdb]#", "");
		if (cmd == "quit")
			break;
		if (cmd == "")
		{
			//printf("\n");
			continue;
		}
		cmd = ProcessTelnetCmd(&m_mdb, cmd);
		printf(cmd.data());
	}

	return true;
}

bool CMdbCmd::Stop()
{
	return true;
}

BYTE* CMdbCmd::OnMyMdbClientTrgCallback(void* cbParam, SString &sTable, eMdbTriggerType eType, int iTrgRows, int iRowSize, BYTE *pTrgData)
{
	CMdbCmd *pThis = (CMdbCmd*)cbParam;
	if (eType <=0 || eType >= 5)
	{
		printf("Trigger callback with invalid type:%d\n",eType);
		return NULL;
	}
	static char* trg_name[] = { "", "Insert", "Update", "Delete", "Truncate" };
	printf("\r\n[TRG] %s [%s %s Trigger] TrgRows=%d,RowSize=%d\r\n",
			SDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz").data(),
			sTable.data(),trg_name[(int)eType],iTrgRows,iRowSize);
	return NULL;
}

extern char g_ver[32];
SString CMdbCmd::ProcessTelnetCmd(SMdb *pMdb, SString &cmd)
{
	SString sRet = "";
	if (cmd == "welcome")
	{
		sRet += "��ӭʹ��MDB���ݿ��ն˹���\r\n�汾��:V" + SString::toString((char*)g_ver, strlen(g_ver)) + "\r\n��¼ʱ��:" + SDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "\r\n";
		sRet += "������'help'�鿴������������!\r\n";
	}
	else if (cmd == "help")
	{		
		sRet += "--------------------------------------------------------------\r\n";
		sRet += "MDB���ݿ��ն˹���(V" + SString::toString((char*)g_ver,strlen(g_ver)) + ")������Ϣ\r\n";
		sRet += "����:�ۿ���(skt001@163.com)\r\n";
		sRet += "--------------------------------------------------------------\r\n";
		sRet += "help                 ��ʾ������Ϣ\n";
		sRet += "trg_on <table_name>  ����ָ����Ĵ�����\n";
		sRet += "trg_off <table_name> �Ͽ�ָ����Ĵ�����\n";
		sRet += "run <sql_file>       ִ��SQL�ļ�\n";
		sRet += "blob_put             ���´��ֶ�\n";
		sRet += "blob_get             ��ȡ���ֶ�\n";
		sRet += "quit                 �˳�\n";
		sRet += "<SQL>                SQL�ṹ����ѯ���ԣ���֧�ּ��﷨��\n";
	}
	else if (cmd.find("trg_on") >= 0)
	{
		SString sTableName = SString::GetIdAttribute(2, cmd, " ");
		if (sTableName.length() == 0)
		{
			sRet += "��������ȷ�ı���!";
			return sRet;
		}
		//pMdb->GetMdbClient()->RemoveTriggerCallback(OnMyMdbClientTrgCallback, this, sTableName,0x0f);
		pMdb->GetMdbClient()->RegisterTriggerCallback(OnMyMdbClientTrgCallback, this, sTableName, 0x0f);
	}
	else if (cmd.find("trg_off") >= 0)
	{
		SString sTableName = SString::GetIdAttribute(2, cmd, " ");
		if (sTableName.length() == 0)
		{
			sRet += "��������ȷ�ı���!";
			return sRet;
		}
		pMdb->GetMdbClient()->RemoveTriggerCallback(OnMyMdbClientTrgCallback, this, sTableName, 0x0f);
	}
	else if (cmd.find("blob_put") >= 0)
	{
		SString table = TipAndInput("���������:", "");
		SString field = TipAndInput("�������ֶ���:", "");
		SString where = TipAndInput("����������:", "");
		SString file = TipAndInput("���������ȡ�ı����ļ�ȫ·����:", "");
		int ret = pMdb->UpdateLobFromFile(table, field, where, file);
		printf("���´��ֶη���:%d\n", ret);
	}
	else if (cmd.find("blob_get") >= 0)
	{
		SString table = TipAndInput("���������:", "");
		SString field = TipAndInput("�������ֶ���:", "");
		SString where = TipAndInput("����������:", "");
		SString file = TipAndInput("�����������ı����ļ�ȫ·����:", "");
		int ret = pMdb->ReadLobToFile(table, field, where, file);
		printf("��ȡ���ֶη���:%d\n", ret);
	}
	else if (cmd.find("run") == 0)
	{
		SString file = cmd.mid(4);
		if (file.length() == 0)
		{
			sRet += "��������ȷ���ļ���!";
			return sRet;
		}
		RunSqlFile(file);
	}
	else if (cmd == "")
	{
		sRet += "";
	}
	else
	{
		//ִ��SQL���
		SString sql = cmd.stripWhiteSpace();
		if (sql.left(6).toLower() == "select")
		{
			//��ѯ���
			SRecordset rs;
			int soc1, soc2, usec1, usec2;
			SDateTime::getSystemTime(soc1, usec1);
			int ret = pMdb->Retrieve(sql, rs);
			SDateTime::getSystemTime(soc2, usec2);
			if (ret < 0)
			{
				sRet += "SQLִ��ʧ��! ";
				sRet += pMdb->GetLastError() + "\r\n";
			}
			else
			{
				sRet += SString::toFormat("��ѯ����:%d��%d��,��ѯ��ʱ:%f��\r\n", rs.GetRows(), rs.GetColumns(), (soc2 - soc1) + ((float)usec2 - (float)usec1) / 1000000);
#if 1
				int row, rows = rs.GetRows();
				int col, cols = rs.GetColumns();
				if (cols <= 0)
				{
					return sRet;
				}
				SString text;
				int len;
				int *col_len = new int[cols];
				int rows_chars = 4;
				if (rows >= 10000000)
					rows_chars = 8;
				else if (rows >= 1000000)
					rows_chars = 7;
				else if (rows >= 100000)
					rows_chars = 6;
				else if (rows >= 10000)
					rows_chars = 5;
				for (col = 0; col<cols; col++)
				{
					col_len[col] = rs.GetColumnName(col).length();
				}
				//��ɨ��ȫ�����ݣ�ȷ���������ݳ���
				for (row = 0; row<rows; row++)
				{
					for (col = 0; col<cols; col++)
					{
						len = rs.GetValue(row, col).length();
						if (len > col_len[col])
							col_len[col] = len;
					}
				}

				//if (rows <= 30)
				{
					//���ɱ�ͷ
					sRet += SString::toFillString("�к�", rows_chars, ' ', false)+" ";
					for (col = 0; col<cols; col++)
					{
						sRet += SString::toFillString(rs.GetColumnName(col), col_len[col], ' ', false);
						sRet += " ";
					}
					sRet += "\r\n";
					sRet += SString::toFillString("-", rows_chars, '-', false)+" ";
					for (col = 0; col<cols; col++)
					{
						sRet += SString::toFillString("-", col_len[col], '-', false);
						sRet += " ";
					}
					sRet += "\r\n";
				}
				bool bPause = true;
				for (row = 0; row<rows; row++)
				{
					if (/*rows>30 &&*/ row> 0 && row % 30 == 0)
					{
						printf(sRet.data());
						sRet = "";
						if (bPause)
						{
							printf("�����⽡������ʾ(q�س��˳�,c�س�ȫ����ʾ)...\n");
							char ch = getchar();
							if (ch == 'q')
							{
								printf("��ֹ��ʾ������¼!\n");
								break;
							}
							if (ch == 'c')
								bPause = false;
						}
						//���ɱ�ͷ
						sRet += SString::toFillString("�к�", rows_chars, ' ', false) + " ";
						for (col = 0; col<cols; col++)
						{
							sRet += SString::toFillString(rs.GetColumnName(col), col_len[col], ' ', false);
							sRet += " ";
						}
						sRet += "\r\n";
						sRet += SString::toFillString("-", rows_chars, '-', false) + " ";
						for (col = 0; col<cols; col++)
						{
							sRet += SString::toFillString("-", col_len[col], '-', false);
							sRet += " ";
						}
						sRet += "\r\n";
					}
					sRet += SString::toFillString(SString::toString(row+1), rows_chars, ' ', false) + " ";
					for (col = 0; col<cols; col++)
					{
						sRet += SString::toFillString(rs.GetValue(row, col), col_len[col], ' ', false);
						sRet += " ";
					}
					sRet += "\r\n";
				}
				sRet += SString::toFillString("-", rows_chars, '-', false) + " ";
				for (col = 0; col<cols; col++)
				{
					sRet += SString::toFillString("-", col_len[col], '-', false);
					sRet += " ";
				}
				sRet += "\r\n";
				sRet += SString::toFormat("�ϼ�:%d\r\n", rows);
				delete[] col_len;
#endif
			}
		}
		else
		{
			//ִ�����
			if (pMdb->Execute(sql))
			{
				sRet += "SQLִ�гɹ�!\r\n";
			}
			else
			{
				sRet += "SQLִ��ʧ��! ";
				sRet += pMdb->GetLastError()+"\r\n";
			}
		}
	}
	return sRet;
}

void CMdbCmd::RunSqlFile(SString filename)
{
	SFile file(filename);
	if (file.open(IO_ReadOnly) == false)
	{
		printf("�ļ�[%s]�����ڣ�\n", filename.data());
		return;
	}
	char buf[10240];
	int lines = 0;
	SString sLine;
	SString sql = "";
	bool bEnd = false;
	bool bSepClosed = true;
	int pos = 0;
	bool bComment = false;
	SString msg;
	bool bSector = false;
	bool bOk = true;

	while (file.readLine(buf, sizeof(buf)) >= 0)
	{
		sLine = buf;
		if (sLine.length() == 0)
		{
			if (file.atEnd())
				break;
			continue;
		}

		if (sLine.Find("/*") >= 0)
		{
			bComment = true;
		}
		if (bComment == true)
		{
			if (sLine.Find("*/") >= 0)
				bComment = false;
			continue;
		}
		if (sLine.Find("--[BEGIN]") >= 0)
		{
			bSector = true;
			sql = "";
			continue;
		}
		if (bSector == true && sLine.Find("--[END]") >= 0)
		{
			bSector = false;
			//tfile.writeString("\r\nsql="+sql);
			if (!m_mdb.Execute(sql))
			{
				printf("SQL[%s]����:%s\n", sql.data(), m_mdb.GetLastError().data());
				bOk = false;
				break;
			}
			lines++;
			sql = "";
			bEnd = false;
			continue;
		}
		if (bSector == true)
		{
			sql += sLine + "\r\n";
			continue;
		}
		if (sLine.Find("--") >= 0)
		{
			sLine = sLine.Left(sLine.Find("--"));
		}
		pos = -1;
		while (1)
		{
			if ((pos = sLine.Find("'", pos + 1)) < 0)
				break;
			bSepClosed = !bSepClosed;
		}

		if (bSepClosed && sLine.Find(";") >= 0)
		{
			sLine = sLine.Left(sLine.Find(";"));
			bEnd = true;
		}
		sLine.trim();
		if (bEnd == false && (sLine == "" || sLine.Left(2) == "--" || sLine.Left(1) == "*"))
			continue;
		sql += sLine + " ";
		if (bEnd)
		{
			//tfile.writeString("\r\nsql="+sql);
			if (!m_mdb.Execute(sql))
			{
				printf("SQL[%s]����:%s\n", sql.data(), m_mdb.GetLastError().data());
				bOk = false;
				break;
			}
			lines++;
			sql = "";
			bEnd = false;
		}
		memset(buf, 0, sizeof(buf));
	}
	if (bOk)
		printf("SQL�ļ�ִ�����!��%d����䱻�ɹ�ִ��!\n", lines);
}