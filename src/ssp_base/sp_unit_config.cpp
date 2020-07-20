/**
 *
 * �� �� �� : sp_unit_config.cpp
 * �������� : 2015-7-20 18:44
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ��Ԫ�����ļ������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-20	�ۿ�������ļ�
 *
 **/

#include "sp_unit_config.h"
#include "SXmlConfig.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUnitNetInterface
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 19:34
// ��    ��:  ����ӿڶ��壬һ��ʵ����Ӧһ���������ڣ�ÿ�����ڿ�0����IP������
//////////////////////////////////////////////////////////////////////////

CUnitNetInterface::CUnitNetInterface()
{
	m_Addrs.setAutoDelete(true);
	m_Gateways.setAutoDelete(true);
}

CUnitNetInterface::~CUnitNetInterface()
{

}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUnitDisk
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 19:35
// ��    ��:  �洢�����̣���Ӧ���̡�SSD��Flash
//////////////////////////////////////////////////////////////////////////
CUnitDisk::CUnitDisk()
{
	m_Parts.setAutoDelete(true);
}

CUnitDisk::~CUnitDisk()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUnitNas
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 19:38
// ��    ��:  ����洢��
//////////////////////////////////////////////////////////////////////////
CUnitNas::CUnitNas()
{
	m_Parts.setAutoDelete(true);
}

CUnitNas::~CUnitNas()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUnitConfig
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 19:50
// ��    ��:  ��Ԫ������
//////////////////////////////////////////////////////////////////////////
CUnitConfig::CUnitConfig()
{
	SetConfigType(SPCFG_UNIT_CONFIG);
	m_Nets.setAutoDelete(true);
	m_Disks.setAutoDelete(true);
	m_Nass.setAutoDelete(true);
	m_Units.setAutoDelete(true);
}

CUnitConfig::~CUnitConfig()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����ָ���ļ����Ƶ������ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 19:51
// ����˵��:  @sPathFileΪ�����ļ�ȫ·��
// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool CUnitConfig::Load(SString sPathFile)
{
	CConfigBase::Load(sPathFile);
	m_Nets.clear();
	m_Disks.clear();
	m_Nass.clear();
	m_slAutoRunCmd.clear();
	SXmlConfig xml;
	if(!xml.ReadConfig(sPathFile))
	{
		LOGFAULT("���ص�Ԫ�����ļ�ʧ��:%s",sPathFile.data());
		return false;
	}
	m_sVersion = xml.SearchNodeAttribute("header","version");
	m_sSystemType = xml.SearchNodeAttribute("system","soft_type");
	m_sHmiStyle = xml.SearchNodeAttribute("system","hmi_style");

	SBaseConfig *pCfg;
	pCfg = xml.SearchChild("unit");
	if(pCfg == NULL)
	{
		LOGFAULT("ȱ��[unit]�ڵ��ڵ�Ԫ�����ļ�:%s",sPathFile.data());
		return false;
	}
	m_iUnitId = pCfg->GetAttributeI("id");
	m_sUnitName = pCfg->GetAttribute("name");
	m_sUnitDesc = pCfg->GetAttribute("desc");
	m_ChkSelfTemp.Read(pCfg,"temp_monitor","temp_alert","temp_warn");
	m_ChkSelfHumi.Read(pCfg,"humi_monitor","humi_alert","humi_warn");
	m_ChkSelfCpu.Read(pCfg,"cpu_monitor","cpu_alert","cpu_warn");
	m_ChkSelfPwr[0].Read(pCfg,"pwr1_monitor","pwr1_alert","pwr1_warn");
	m_ChkSelfPwr[1].Read(pCfg,"pwr2_monitor","pwr2_alert","pwr2_warn");
	m_ChkSelfPwr[2].Read(pCfg,"pwr3_monitor","pwr3_alert","pwr3_warn");
	m_ChkSelfPwr[3].Read(pCfg,"pwr4_monitor","pwr4_alert","pwr4_warn");

	// 	<unit-list>
	// 		<unit id="1" ip="172.18.162.208" name="cu1" desc="�ɼ���Ԫ1" />
	// 		<unit id="2" ip="172.18.162.205" name="hmi" desc="�˻�����վ1" />
	// 	</unit-list>
	SBaseConfig *pUnitList = xml.SearchChild("unit-list");
	if(pUnitList == NULL)
	{
		LOGFAULT("ȱ��[unit-list]�ڵ��ڵ�Ԫ�����ļ�:%s",sPathFile.data());
		return false;
	}
	unsigned long pos = 0;
	pCfg = pUnitList->GetChildPtr()->FetchFirst(pos);
	while(pCfg)
	{
		if(pCfg->GetNodeName() == "unit")
		{
			CSpUnit *pUnit = new CSpUnit();
			pUnit->m_iUnitId = pCfg->GetAttributeI("id");
			pUnit->m_sUnitCommIp = pCfg->GetAttribute("ip");
			pUnit->m_sUnitName = pCfg->GetAttribute("name");
			pUnit->m_sUnitDesc = pCfg->GetAttribute("desc");
			if(pUnit->m_iUnitId == m_iUnitId)
			{
				if(pUnit->m_sUnitName.length() > 0)
					m_sUnitName = pUnit->m_sUnitName;
				if(pUnit->m_sUnitDesc.length() > 0)
					m_sUnitDesc = pUnit->m_sUnitDesc;
				m_sThisIp = pUnit->m_sUnitCommIp;
			}
			m_Units.append(pUnit);
		}
		pCfg = pUnitList->GetChildPtr()->FetchNext(pos);
	}	

	pCfg = xml.SearchChild("network");
	int i,cnt,j,cnt2;
	SBaseConfig *p;
	SString sTemp;
	if(pCfg != NULL)
	{
		cnt = pCfg->GetChildCount("interface");
		for(i=0;i<cnt;i++)
		{
			p = pCfg->GetChildNode(i,"interface");
			if(p == NULL)
				continue;
			CUnitNetInterface *pNet = new CUnitNetInterface;
			pNet->m_sName = p->GetAttribute("name");
			pNet->m_sDesc = p->GetAttribute("desc");
			pNet->m_bOptical = (p->GetAttribute("is_optical").toLower() == "true");
			pNet->m_bMonitor = (p->GetAttribute("monitor").toLower() == "true");
			pNet->m_ChkSelfOptPower.Read(p,"opt_pwr_monitor","opt_pwr_alert","opt_pwr_warn");
			
			cnt2 = p->GetChildCount("addr");
			for(j=0;j<cnt2;j++)
			{
				SBaseConfig *p2 = p->GetChildNode(j,"addr");
				if(p2 == NULL)
					continue;
				CUnitNetInterface::stuAddr *pAddr = new CUnitNetInterface::stuAddr;
				pAddr->m_sIp = p2->GetAttribute("ip");
				unsigned int netmask = ntohl(SSocket::IpStrToIpv4(p2->GetAttribute("netmask").data()));
				int k;
				for(k=31;k>=0;k--)
				{
					if(((netmask >> k) & 0x01) == 0)
						break;
				}
				pAddr->m_iMaskSize = 32-(k+1);
				pNet->m_Addrs.append(pAddr);
			}

			cnt2 = p->GetChildCount("gateway");
			for(j=0;j<cnt2;j++)
			{
				SBaseConfig *p2 = p->GetChildNode(j,"gateway");
				if(p2 == NULL)
					continue;
				CUnitNetInterface::stuGateway *pGate = new CUnitNetInterface::stuGateway;
				pGate->m_sNetIp = p2->GetAttribute("netip");
				pGate->m_sGwIp = p2->GetAttribute("gwip");
				pNet->m_Gateways.append(pGate);
			}
			m_Nets.append(pNet);
		}
	}

	pCfg = xml.SearchChild("storage");
	if(pCfg != NULL)
	{
		cnt = pCfg->GetChildCount("disk");
		for(i=0;i<cnt;i++)
		{
			p = pCfg->GetChildNode(i,"disk");
			if(p == NULL)
				continue;
			CUnitDisk *pDisk = new CUnitDisk();
			pDisk->m_sName = p->GetAttribute("name");
			pDisk->m_sDesc = p->GetAttribute("desc");
			cnt2 = p->GetChildCount("partition");
			for(j=0;j<cnt2;j++)
			{
				SBaseConfig *p2 = p->GetChildNode(j,"partition");
				if(p2 == NULL)
					continue;
				CUnitDisk::stuPartition *pPart = new CUnitDisk::stuPartition();
				pPart->m_sName = p2->GetAttribute("name");
				pPart->m_sDesc = p2->GetAttribute("desc");
				sTemp = p2->GetAttribute("fs_type").toLower();
				if(sTemp == "ext2")
					pPart->m_FsType = FS_EXT2;
				else if(sTemp == "ext3")
					pPart->m_FsType = FS_EXT3;
				else if(sTemp == "ext4")
					pPart->m_FsType = FS_EXT4;
				else if(sTemp == "yaffs")
					pPart->m_FsType = FS_NTFS;
				else if(sTemp == "fat")
					pPart->m_FsType = FS_FAT;
				else if(sTemp == "ntfs")
					pPart->m_FsType = FS_NTFS;
				else if(sTemp == "raw")
					pPart->m_FsType = FS_RAW;
				pDisk->m_Parts.append(pPart);
			}
			m_Disks.append(pDisk);
		}

		cnt = pCfg->GetChildCount("nas");
		for(i=0;i<cnt;i++)
		{
			p = pCfg->GetChildNode(i,"nas");
			if(p == NULL)
				continue;
			CUnitNas *pNas = new CUnitNas;
			pNas->m_sName = p->GetAttribute("name");
			pNas->m_sDesc = p->GetAttribute("desc");
			cnt2 = p->GetChildCount("partition");
			for(j=0;j<cnt2;j++)
			{
				SBaseConfig *p2 = p->GetChildNode(j,"partition");
				if(p2 == NULL)
					continue;
				CUnitNas::stuPartition *pPart = new CUnitNas::stuPartition();
				pPart->m_sName = p2->GetAttribute("name");
				pPart->m_sDesc = p2->GetAttribute("desc");
				pPart->m_sExtAttr = p2->GetAttribute("extattr");
				sTemp = p2->GetAttribute("fs_type").toLower();
				if(sTemp == "nfs")
					pPart->m_FsType = NFS_NFS;
				else if(sTemp == "ftp")
				pPart->m_FsType = NFS_FTP;

				pNas->m_Parts.append(pPart);
			}
			m_Nass.append(pNas);
		}
		m_sUsbStartDev = pCfg->SearchNodeAttribute("usbdisk","start_dev");
	}

	pCfg = xml.SearchChild("autorun");
	if(pCfg != NULL)
	{
		cnt = pCfg->GetChildCount("cmd");
		for(i=0;i<cnt;i++)
		{
			p = pCfg->GetChildNode(i,"cmd");
			if(p == NULL)
				continue;
			m_slAutoRunCmd.append(p->GetNodeValue());
		}
	}
	return true;
}
