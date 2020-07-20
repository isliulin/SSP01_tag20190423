/**
 *
 * �� �� �� : viewmoduleinfo.cpp
 * �������� : 2013-9-13 12:58
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ִ��ģ��İ汾��Ϣ
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2013-9-13	�ۿ�������ļ�
 *
 **/

#include "viewmoduleinfo.h"

CViewModuleInfo::CViewModuleInfo(QWidget *parent) : CBaseView(parent)
{
	ui.setupUi(this);
	QStringList labels;
	ui.tableWidget->setColumnCount(8);
	labels << tr("���е�Ԫ") << tr("���̺�") << tr("ģ������") << tr("����") << tr("�汾") << tr("����ʱ��") << tr("CRCУ����") << tr("��¼����ʱ��") << tr("����IP") ; 
	ui.tableWidget->setHorizontalHeaderLabels(labels);
	ui.tableWidget->setColumnWidth(0,150);
	ui.tableWidget->setColumnWidth(1,70);
	ui.tableWidget->setColumnWidth(2,120);
	ui.tableWidget->setColumnWidth(3,200);
	ui.tableWidget->setColumnWidth(4,80);
	ui.tableWidget->setColumnWidth(5,140);
	ui.tableWidget->setColumnWidth(6,80);
	ui.tableWidget->setColumnWidth(7,140);
	ui.tableWidget->setColumnWidth(8,100);
	//������ʽ
	SQt::SetTableWidgetStyle(ui.tableWidget);
	m_bLoading = false;
	m_RunUnits.setAutoDelete(true);
	m_RunUnits.setShared(true);

	connect(this,SIGNAL(sigInfoUpdated()),this,SLOT(OnTimerUI()));
	connect(ui.pushButtonQuery,SIGNAL(clicked()),this,SLOT(OnQuery()));
	connect(ui.pushButtonSave,SIGNAL(clicked()),this,SLOT(OnSave()));
	connect(ui.pushButtonPrint,SIGNAL(clicked()),this,SLOT(OnPrint()));
	ReLoad();
}

CViewModuleInfo::~CViewModuleInfo()
{
	m_RunUnits.clear();
}


////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ʱ������������ʾ�̲߳�ѯ�����ļ�¼
// ��    ��:  �ۿ���
// ����ʱ��:  2013:10:23 10:29
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
void CViewModuleInfo::OnTimerUI()
{
	int idx;
	QTableWidgetItem *pItem;
	if(m_bLoading == false)
	{
		SetTip("��Ԫʵʱģ����Ϣ��ȡ��ϣ�");
	}
	unsigned long pos = 0,pos2=0;
	stuSpUnitAgentProcInfo *pProc;
	stuRunUnit *pRunUnit = m_RunUnits.FetchFirst(pos);
	while(pRunUnit)
	{
		SString sUnitInfo;
		SString ip;
		for(int i=0;i<8;i++)
		{
			if(pRunUnit->UnitInfo.m_iUnitIp[i] == 0)
				break;
			if(ip.length() > 0)
				ip += "/";
			ip += SSocket::Ipv4ToIpStr(pRunUnit->UnitInfo.m_iUnitIp[i]).data();
		}
		sUnitInfo.sprintf("%d-%s(%s %s)",pRunUnit->UnitInfo.m_iUnitId,pRunUnit->UnitInfo.m_sUnitName,ip.data(),pRunUnit->UnitInfo.m_sUnitDesc);
		if(pRunUnit->m_ProcInfos.count() == 0)
		{
			sUnitInfo.sprintf("�ӵ�Ԫ%s��ȡģ����Ϣʧ��!",sUnitInfo.data());
			idx = ui.tableWidget->rowCount();
			ui.tableWidget->insertRow(idx);
			ui.tableWidget->setRowHeight(idx,C_TABLE_ROW_HEIGHT);
			ui.tableWidget->setItem(idx,0,(pItem=new QTableWidgetItem(sUnitInfo.data())));

		}
		pProc = pRunUnit->m_ProcInfos.FetchFirst(pos2);
		while(pProc)
		{
			//TODO:
			idx = ui.tableWidget->rowCount();
			ui.tableWidget->insertRow(idx);
			ui.tableWidget->setRowHeight(idx,C_TABLE_ROW_HEIGHT);
			ui.tableWidget->setItem(idx,0,(pItem=new QTableWidgetItem(sUnitInfo.data())));
			ui.tableWidget->setItem(idx,1,(pItem=new QTableWidgetItem(SString::toString((int)pProc->pid).data())));	pItem->setTextAlignment(Qt::AlignCenter);
			ui.tableWidget->setItem(idx,2,(pItem=new QTableWidgetItem(pProc->name)));
			ui.tableWidget->setItem(idx,3,(pItem=new QTableWidgetItem(pProc->desc)));
			ui.tableWidget->setItem(idx,4,(pItem=new QTableWidgetItem(pProc->version)));		pItem->setTextAlignment(Qt::AlignCenter);
			ui.tableWidget->setItem(idx,5,(pItem=new QTableWidgetItem(pProc->build_time)));		pItem->setTextAlignment(Qt::AlignCenter);
			ui.tableWidget->setItem(idx,6,(pItem=new QTableWidgetItem(pProc->crc)));			pItem->setTextAlignment(Qt::AlignCenter);
			if(pProc->login_time != 0)
			{
				ui.tableWidget->setItem(idx,7,(pItem=new QTableWidgetItem(SDateTime::makeDateTime((time_t)pProc->login_time).toString("yyyy-MM-dd hh:mm:ss").data())));	pItem->setTextAlignment(Qt::AlignCenter);
			}
			ui.tableWidget->setItem(idx,8,(pItem=new QTableWidgetItem(pProc->login_ip)));	pItem->setTextAlignment(Qt::AlignCenter);
			pProc = pRunUnit->m_ProcInfos.FetchNext(pos2);
		}
		pRunUnit = m_RunUnits.FetchNext(pos);
	}
	m_RunUnits.clear();
}

void CViewModuleInfo::OnQuery()
{
	ReLoad();
}

void CViewModuleInfo::OnSave()
{
	SQt::TableExort(Q2S(tr("ģ��汾��Ϣ")), ui.tableWidget);
}

void CViewModuleInfo::OnPrint()
{
	SQt::TablePrint(Q2S(tr("ģ��汾��Ϣ")), ui.tableWidget);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ������׼����ʾǰ�����麯�������¼�������
// ��    ��:  �ۿ���
// ����ʱ��:  2013:9:13 13:03
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CViewModuleInfo::OnPreShow()
{
	
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ���¼���ʵʱģ����Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2013:10:23 10:03
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
void CViewModuleInfo::ReLoad()
{
	if(m_bLoading)
		return;
	SetTip("������ȡ��Ԫʵʱģ����Ϣ�����Ժ�...");
	while(ui.tableWidget->rowCount() > 0)
		ui.tableWidget->removeRow(0);
	m_bLoading = true;
	SKT_CREATE_THREAD(ThreadLoadInfo,this);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �޸ļ����߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2013:10:23 10:02
// ����˵��:  
// �� �� ֵ:  
//////////////////////////////////////////////////////////////////////////
void* CViewModuleInfo::ThreadLoadInfo(void *lp)
{
	SKT_INTO_THREAD
	CViewModuleInfo *pThis = (CViewModuleInfo*)lp;
	pThis->m_RunUnits.clear();
	SPtrList<stuSpUnitAgentUnitInfo> units;
	if(SBASE_SAPP->GetUnitInfo(units) < 0)
	{
		pThis->SetTip("��ȡ��Ԫ��Ϣʱʧ�ܣ���ȷ�ϱ���Ԫ�Ĵ������sys_unit_agent�Ƿ�����?");
		return NULL;
	}
	unsigned long pos;
	stuSpUnitAgentUnitInfo *pUnit = units.FetchFirst(pos);
	while(pUnit)
	{
		stuRunUnit *pRunUnit = new stuRunUnit();
		memcpy(&pRunUnit->UnitInfo,pUnit,sizeof(stuSpUnitAgentUnitInfo));
		if(SBASE_SAPP->GetProcessInfo(pUnit->m_iUnitId,pRunUnit->m_ProcInfos) < 0)
		{
			LOGERROR("��Ԫ%d������Ϣ��ȡʧ��!",pUnit->m_iUnitId);
		}
		pThis->m_RunUnits.append(pRunUnit);
		pUnit = units.FetchNext(pos);
	}

#if 0
	SString sql,unit_name,unit_ip;
	SRecordset rsUnit;
	int i,cnt;
	SApplication *pApp = SApplication::GetPtr();
	if(pApp)
	{
		SString ip = "127.0.0.1";
		if(DB->GetDatabasePool() != NULL && DB->GetDatabasePool()->GetPoolSize() > 0)
		{
			SString sParams = DB->GetDatabasePool()->GetParams();
			ip = SString::GetAttributeValue(sParams,"hostaddr");
			if(ip == "localhost")
				ip = "127.0.0.1";
		}
		bool bReConn = false;
		if(ip != "127.0.0.1")
		{
			pApp->ReConnectAgentSvr(ip);
			SPtrList<stuModuleInfo> module_list;
			module_list.setAutoDelete(true);
			for(int i=0;i<100&& !pApp->m_bLogined;i++)
				SApi::UsSleep(10000);
			bReConn = true;
		}
		SPtrList<stuModuleInfo> module_list;
		if(pApp->GetModuleList(module_list) < 0)
		{
			stuModuleInfo *pNew = new stuModuleInfo();
			pNew->pid = 0;
			pNew->name	= "*";
			pNew->desc		= "δ֪(�ӵ�Ԫ�����ȡģ����Ϣʧ��!)";
			pThis->m_RealModInfo.append(pNew);
		}
		else
		{
			module_list.copyto(pThis->m_RealModInfo);
		}
		if(bReConn)
			pApp->ReConnectAgentSvr("127.0.0.1");
	}
#endif
#if 0
	//�������е�Ԫ�е�����ģ��
	int port = SelectIntoI("select cfg_value from t_config where cfg_name='unitagent.port'");
	if(port == 0)
		port = 9502;
	sql = "select unit_name,unit_cmdip from t_substationunit";
	RetrieveRecordset(sql,rsUnit);
	cnt = rsUnit.GetRows();
	for(i=0;i<cnt;i++)
	{
		unit_name = rsUnit.GetValue(i,0);
		unit_ip = rsUnit.GetValue(i,1);

		YsSocket skt;
		stuAgentCmdHeader header;
		header.flag = 0xEB90;
		header.command = AGENT_QUERY_MODULE;
		header.length = 0;
		header.hton();
		skt.Create();
		skt.SetTimeout(1000*60,5000);
		if(	skt.Connect(unit_ip.data(),port) &&
			skt.Send(&header,sizeof(header)) == sizeof(header) && 
			skt.Receive(&header,sizeof(header)) == sizeof(header))
		{
			header.ntoh();
			if(header.length < 0 || header.length > 10000000)
			{
				stuRealModInfo *pNew = new stuRealModInfo();
				pNew->unit_name = unit_name;
				pNew->mod_name	= "*";
				pNew->desc		= "δ֪(�ӵ�Ԫ��ȡģ����Ϣʧ��!)";
				pThis->m_RealModInfo.append(pNew);
				continue;
			}
			char *pStr = new char[header.length + 1];
			memset(pStr,0,header.length + 1);
			skt.Receive(pStr,header.length );
			
			SString str = pStr;
			delete[] pStr;
			SString strtmp,pathfile,module,desc,created,version,libver,libtime,crc,run,pid,dbcrc;
			int i,cnt = SString::GetAttributeCount(str,"\n") - 1;
			int index = 0;
			SStringList slModules,slModulesSort;
			for(i=1;i<=cnt;i++)
			{
				strtmp = SString::GetIdAttribute(i,str,"\n");
				slModules.append(strtmp);
			}
			while(slModules.count() > 0)
			{
				int min=0;
				strtmp = SString::GetAttributeValue(*slModules[0],"module");
				for(int j=1;j<slModules.count();j++)
				{
					SString mod = SString::GetAttributeValue(*slModules[j],"module");
					if(mod < strtmp)
					{
						strtmp = mod;
						min = j;
					}
				}
				strtmp = *slModules[min];
				slModulesSort.append(strtmp);
				slModules.remove(min);
			}
			for(i=0;i<cnt;i++)
			{
				strtmp = *slModulesSort[i];//SString::GetIdAttribute(i,str,"\n");
				pathfile = SString::GetAttributeValue(strtmp,"pathfile");
				module = SString::GetAttributeValue(strtmp,"module");
				desc = SString::GetAttributeValue(strtmp,"desc");
				created = SString::GetAttributeValue(strtmp,"created");
				version = SString::GetAttributeValue(strtmp,"version");
				libver = SString::GetAttributeValue(strtmp,"libver");
				libtime = SString::GetAttributeValue(strtmp,"libtime");
				crc = SString::GetAttributeValue(strtmp,"crc");
				run = SString::GetAttributeValue(strtmp,"run");
				pid = SString::GetAttributeValue(strtmp,"pid");

				stuRealModInfo *pNew = new stuRealModInfo();
				pNew->unit_name		= unit_name;
				pNew->mod_name		= module;
				pNew->desc			= desc;
				pNew->create_time	= created;
				pNew->version		= version;
				pNew->crc	= crc;
				if(run=="1")
					pNew->run_state.sprintf("��������(Pid=%d)",pid.toInt());
				else
					pNew->run_state = "��ֹͣ";
				pThis->m_RealModInfo.append(pNew);
				index ++;
			}
		}
		else
		{
			stuRealModInfo *pNew = new stuRealModInfo();
			pNew->unit_name = unit_name;
			pNew->mod_name	= "*";
			pNew->desc.sprintf("�ӵ�Ԫ[%s(%s)]��ȡʵʱģ����Ϣʧ�ܣ�",unit_name.data(),unit_ip.data());
			pThis->m_RealModInfo.append(pNew);
		}
	}
#endif
	pThis->m_bLoading = false;
	emit pThis->sigInfoUpdated();
	return NULL;
}
