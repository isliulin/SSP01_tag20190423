/**
 *
 * �� �� �� : SimClient.cpp
 * �������� : 2018-8-22 9:59
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ����ͻ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2018-8-22	�ۿ�������ļ�
 *
 **/

#include "SimClient.h"
#include "view_home.h"
#include <QDebug>
CSimClient::CSimClient()
{
	SetStartWord(0x35E7);
	m_strModuleName = "sim_iec61850_server";
}
CSimClient::~CSimClient()
{

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �Է�����ʽ�����ͻ��ˣ�������Connect����ͬʱʹ��
// ��    ��:  �ۿ���
// ����ʱ��:  2018-8-22 9:58
// ����˵��:  @ip��ʾ�����IP��ַ��@port��ʾ�˿�
// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
//////////////////////////////////////////////////////////////////////////
bool CSimClient::Start(SString module_name,SString module_desc,SString ip,int port)
{
	SString str;
	str.sprintf("module_name=%s;module_desc=%s;",module_name.data(),module_desc.data());
	SetLoginInfo(str);
	qDebug() << str << "ip"<<ip.data()<<"port"<<port;
	return STcpClientBase::Start(ip,port);
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ֹͣ����
// ��    ��:  �ۿ���
// ����ʱ��:  2018-8-22 9:58
// ����˵��:  void
// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
//////////////////////////////////////////////////////////////////////////
bool CSimClient::Stop()
{
	qDebug() << "ֹͣ����";
	return STcpClientBase::Stop();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��¼�Ϸ���˺�Ļص�
// ��    ��:  �ۿ���
// ����ʱ��:  2018-8-22 9:58
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSimClient::OnLogin()
{
	LOGDEBUG("��¼����˳ɹ�!");
	//����ע������
	sendRegisterInfo();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �Ͽ�����˺�Ļص�
// ��    ��:  �ۿ���
// ����ʱ��:  2018-8-22 9:58
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSimClient::OnClose()
{

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  �Ự�߳̽��յ����ĺ�Ļص��麯����������ͨ���˺�������ʱ��Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2018-8-22 9:58
// ����˵��:  @pSessionΪ�Ựʵ��ָ�룬����ͨ��ָ����Զ˷�������
//            @pPackageΪ�ս��յ������ݰ�
// �� �� ֵ:  true��ʾ�Ѿ�������ϣ��˱��Ŀ����ͷ��ˣ�false��ʾδ�����˱���Ӧ������ն���
//////////////////////////////////////////////////////////////////////////
bool CSimClient::OnRecvFrame(stuSTcpPackage *pPackage)
{
	if(pPackage->m_wFrameType == PKG_RUN_FUN_REQ)
	{
		SString func = SString::GetAttributeValue(pPackage->m_sHead,"func");
		SString args = SString::GetAttributeValue(pPackage->m_sHead,"args");

		int ret = RunStep(func,args);
		SString sHead = SString::toFormat("result=%d;",ret) + pPackage->m_sHead;
		qDebug() << "1111111111--:" << sHead.data();
		SendFrame(sHead,PKG_RUN_FUN_RES);
	}
#if 0
	else if (pPackage->m_wFrameType == PKG_REGISTER_RES)
	{
		return ProcessSimPackageType2(pPackage->m_sHead);
	} else if (pPackage->m_wFrameType == PKG_ABILITY_REQ)
	{
		return ProcessSimPackageType3(pPackage->m_sHead);
	}else if (pPackage->m_wFrameType == PKG_START_REQ)
	{
		return ProcessSimPackageType5(pPackage->m_sHead);
	}else if (pPackage->m_wFrameType == PKG_STOP_REQ)
	{
		return ProcessSimPackageType7(pPackage->m_sHead);
	}else if (pPackage->m_wFrameType == PKG_JOB_REQ)
	{
		return ProcessSimPackageType9(pPackage->m_sHead);
	}
#endif
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ִ��ָ���Ĺ����������ִ�н��
// ��    ��:  �ۿ���
// ����ʱ��:  2018-11-6 16:46
// ����˵��:  @func��ʾ����������
//         :  @args��ʾִ�в������������ʹ�ö��ŷָ�
// �� �� ֵ:  >0��ʾִ��ʧ�ܣ�  1:���������
//								2:������������
//								3:������ʽ����ȷ
//								4:������Ч
//								5:����δ����
//								6:�����쳣
//								7:ִ��ʧ��
//			  =0��ʾִ�гɹ�
//////////////////////////////////////////////////////////////////////////
int CSimClient::RunStep(SString func,SString args)
{
	LOGDEBUG("RunStep(%s, %s)",func.data(),args.data());
	//TODO:�ڴ˽��в���ִ�У����ظ����
	if (func == "61850_Ied_Param_Change")
	{
		return m_pViewHome->doExecCmd(args);
	}else if (func == "start")
	{
		return m_pViewHome->startServer()?0:7;
	} else if (func == "stop")
	{
		return m_pViewHome->stopServer()? 0:7;
	} else if (func == "func_info")
	{
		return ProcessSimPackageType3(args)?0:7;
	}

	return 1;
}


void CSimClient::setOptObj(view_home* pViewHome,ConfigFileMgr* pConfileMgr)
{
	m_pViewHome = pViewHome;
	m_pConfigFileMgr = pConfileMgr;
}


bool CSimClient::ProcessSimPackageType2(SString& strHead)
{
	//���ܵ�����˷��ص�ע���������ݽ�������߼�����
	int iRest = SString::GetAttributeValue(strHead,"result").toInt();
	if (iRest == 0)
	{
		//socket�����ӵģ�û�жϿ��������·���ע����Ϣ
		if (IsConnected())
		{
			return sendRegisterInfo();
		} else
		{
			return m_pViewHome->startSimClient();
		}
	} else
	{
		LOGDEBUG("ע��ɹ�!");
		return true;
	}
}

bool CSimClient::ProcessSimPackageType3(SString& strHead)
{
	//�ͻ��˻ظ���������������Ϣ(PKG_ABILITY_RES=4)
	stModule* pCurModule = m_pConfigFileMgr->getModuleByName(m_strModuleName);
	if (pCurModule == NULL)
	{
		return false;
	}
	//	sprintf(regCommand.name,"%s",pCurModule->strName.data());
	QVector<stuSimAbility> tmpList;

	for(int j = 0; j < pCurModule->vFuncList.count(); j++)
	{
		stFunc* pCurFunc = pCurModule->vFuncList.at(j);
		if (pCurFunc == NULL)
		{
			continue;
		}
		stuSimAbility tmp;
		tmp.ctrl_id = pCurFunc->strIdx.toInt();
		sprintf(tmp.ctrl_name,"%s",pCurFunc->strName.data());
		if (pCurFunc->strArgc.isEmpty())
		{
			tmp.param_num = 0;
			sprintf(tmp.param_names,"%s","");
		} else
		{
			tmp.param_num = pCurFunc->strName.find(";")+1;
			sprintf(tmp.param_names,"%s",pCurFunc->strArgcFmt.data());
		}
		tmpList.append(tmp);
	}

	int iNum = tmpList.count();
	stuSTcpPackage* pSend = new stuSTcpPackage;
	pSend->m_wFrameType = PKG_ABILITY_RES;
	pSend->m_sHead = strHead;
	pSend->m_iAsduLen = iNum;
	pSend->m_iAsduBufferLen = iNum * sizeof(stuSimAbility);
	char* pNewBuf = new char[pSend->m_iAsduBufferLen];
	memset(pNewBuf,0,pSend->m_iAsduBufferLen);
	char* pMemStart=pNewBuf;
	for (int i = 0; i < tmpList.count(); i++)
	{
		stuSimAbility tmp = tmpList.at(i);
		memcpy(pMemStart,&tmp,sizeof(tmp));
		pMemStart = pMemStart + sizeof(tmp);
	}
	return SendFrame(pSend);
}

bool CSimClient::ProcessSimPackageType5(SString& strHead)
{
	//�ͻ��˻ظ���������Ӧ(PKG_START_RES=6)
	bool bRst = m_pViewHome->startServer();
	SString sRstHead;
	if (bRst)
	{
		sRstHead = SString::toFormat("result=%d;",1) + strHead;
	} else
	{
		sRstHead = SString::toFormat("result=%d;",0) + strHead;
	}
	return SendFrame(sRstHead,PKG_START_RES);
}

bool CSimClient::ProcessSimPackageType7(SString& strHead)
{
	//�ͻ��˻ظ�����ֹͣ��Ӧ(PKG_STOP_RES=8)
	bool bRst = m_pViewHome->stopServer();
	SString sRstHead;
	if (bRst)
	{
		sRstHead = SString::toFormat("result=%d;",1) + strHead;
	} else
	{
		sRstHead = SString::toFormat("result=%d;",0) + strHead;
	}
	return SendFrame(sRstHead,PKG_STOP_RES);
}

bool CSimClient::ProcessSimPackageType9(SString& strHead)
{
	//�ͻ���ִ����ָ������ҵ��ظ�ִ�н��(PKG_JOB_RES=10)
	SString func = SString::GetAttributeValue(strHead,"func");
	SString args = SString::GetAttributeValue(strHead,"args");
	int ret = RunStep(func,args);
	SString sHead = SString::toFormat("result=%d;",ret) + strHead;
	return SendFrame(sHead,PKG_JOB_RES);
}

bool CSimClient::sendRegisterInfo()
{
	SString strHead;
	strHead.sprintf("cluster_id=%d,sim_code=%d,sim_name=%s",1,1,"sim_iec61850_server");
	return SendFrame(strHead,PKG_REGISTER_REQ);
}