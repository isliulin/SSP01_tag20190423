/**
 *
 * �� �� �� : sp_unitconfig.h
 * �������� : 2015-7-20 18:43
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

#ifndef __SP_UNIT_CONFIG_H__
#define __SP_UNIT_CONFIG_H__

#include "sp_config_mgr.h"
#include "SInclude.h"
#include "ssp_base_inc.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  eHardwareType
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 19:31
// ��    ��:  Ӳ������ö�٣���ͬ����Ӳ����Ӳ������Լ�֧�̶ֳȺͷ���Ҳ��ͬ
//////////////////////////////////////////////////////////////////////////
enum eHardwareType
{
	HW_COMMON_IPC=1,	//��ͨ���ػ�
	HW_COMMON_PC,		//��ͨPC��
	HW_COMMON_SERVER,	//��ͨ������
	HW_UK21007,			//UK21007��������
	HW_UK21008,			//UK21008��������
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  eFileSysType
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 19:32
// ��    ��:  �ļ�ϵͳ���ͣ����NFS�����̣��޷��������������ȡ
//////////////////////////////////////////////////////////////////////////
enum eFileSysType
{
	FS_EXT2=1,			//EXT2
	FS_EXT3,			//EXT3
	FS_EXT4,			//EXT4
	FS_YAFFS,			//YAFFS
	FS_FAT,				//FAT
	FS_NTFS,			//NTFS
	FS_RAW,				//ԭʼ����
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  eNasFileSysType
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 19:45
// ��    ��:  ����洢���ļ�ϵͳ���ͣ����������ϵ������ļ�ϵͳ�������뱾װ�õ��������ͣ�
//////////////////////////////////////////////////////////////////////////
enum eNasFileSysType
{
	NFS_NFS=1,			//NFS
	NFS_FTP,			//FTP
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  stuHwCheckSelfInfo
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 18:58
// ��    ��:  Ӳ�����Ԥ�����澯�Լ�
//////////////////////////////////////////////////////////////////////////
struct SSP_BASE_EXPORT stuHwCheckSelfInfo
{
	bool m_bOpen;						//�Ƿ������
	float m_fAlertThreshold;			//Ԥ���ż�
	float m_fWarnThreshold;				//�澯�ż�

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ������ýڵ㣬��ָ����������ƶ�ȡ�Լ�������Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-20 20:03
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void Read(SBaseConfig *p,SString open_attr,SString alert_attr,SString warn_attr)
	{
		m_bOpen = (p->GetAttribute(open_attr).toLower() == "true");
		m_fAlertThreshold = p->GetAttributeF(alert_attr);
		m_fWarnThreshold = p->GetAttributeF(warn_attr);
	}
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUnitNetInterface
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 19:34
// ��    ��:  ����ӿڶ��壬һ��ʵ����Ӧһ���������ڣ�ÿ�����ڿ�0����IP������
//////////////////////////////////////////////////////////////////////////
class SSP_BASE_EXPORT CUnitNetInterface
{
public:
	CUnitNetInterface();
	~CUnitNetInterface();
	struct stuAddr
	{
		SString m_sIp;						//IP��ַ
		int m_iMaskSize;					//����λ��
	};
	struct stuGateway
	{
		SString m_sNetIp;					//����IP���ձ�ʾȱʡ(default)
		SString m_sGwIp;					//����IP
	};
//private:
	SString m_sName;						//����ӿ�����
	SString m_sDesc;						//����ӿ�����
	bool m_bMonitor;						//�Ƿ��������״̬
	bool m_bOptical;						//�Ƿ���˽ӿ�
	stuHwCheckSelfInfo m_ChkSelfOptPower;	//�⹦���Լ죨m_bOpticalΪtrueʱ��Ч��
	SPtrList<stuAddr> m_Addrs;				//IP��ַ
	SPtrList<stuGateway> m_Gateways;		//����
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUnitDisk
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 19:35
// ��    ��:  �洢�����̣���Ӧ���̡�SSD��Flash
//////////////////////////////////////////////////////////////////////////
class SSP_BASE_EXPORT CUnitDisk
{
public:
	CUnitDisk();
	~CUnitDisk();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  stuPartition
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-20 19:36
	// ��    ��:  ��������
	//////////////////////////////////////////////////////////////////////////
	struct stuPartition
	{
		SString m_sName;					//��������
		SString m_sDesc;					//��������
		eFileSysType m_FsType;				//�ļ�ϵͳ����
		stuHwCheckSelfInfo m_ChkSelfFree;	//�������������Լ�
	};

	SString m_sName;						//�洢��������
	SString m_sDesc;						//�洢��������
	SPtrList<stuPartition> m_Parts;			//���з���
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUnitNas
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 19:38
// ��    ��:  ����洢��
//////////////////////////////////////////////////////////////////////////
class SSP_BASE_EXPORT CUnitNas
{
public:
	CUnitNas();
	~CUnitNas();

	struct stuPartition
	{
		SString m_sName;					//��������
		SString m_sDesc;					//��������
		SString m_sExtAttr;					//��չ����
		eNasFileSysType m_FsType;			//�ļ�ϵͳ����
		stuHwCheckSelfInfo m_ChkSelfFree;	//�������������Լ�
	};

	SString m_sName;						//����洢������
	SString m_sDesc;						//����洢������
	SPtrList<stuPartition> m_Parts;			//���з���
};

class SSP_BASE_EXPORT CSpUnit
{
public:
	CSpUnit()
	{
	};
	~CSpUnit()
	{
	};

	int m_iUnitId;//��ԪΨһ��ʶ��
	SString m_sUnitCommIp;//��Ԫ��ͨѶIP
	SString m_sUnitName;//��Ԫ����
	SString m_sUnitDesc;//��Ԫ����
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUnitConfig
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-20 19:50
// ��    ��:  ��Ԫ������
//////////////////////////////////////////////////////////////////////////
class SSP_BASE_EXPORT CUnitConfig : public CConfigBase
{
public:
	CUnitConfig();
	virtual ~CUnitConfig();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ���ļ����Ƶ������ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-20 19:51
	// ����˵��:  @sPathFileΪ�����ļ�ȫ·��
	// �� �� ֵ:  true��ʾ���سɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Load(SString sPathFile);

//protected:
	stuHwCheckSelfInfo m_ChkSelfTemp;	//�¶��Լ�
	stuHwCheckSelfInfo m_ChkSelfHumi;	//�¶��Լ�
	stuHwCheckSelfInfo m_ChkSelfCpu;	//CPU�������Լ�
	stuHwCheckSelfInfo m_ChkSelfPwr[4];	//װ�õ�Դ�Լ�
	stuHwCheckSelfInfo m_ChkSelfAllDisk;//�����ܿط����������Լ�

//private:
	SString m_sVersion;					//�����ļ��汾��
	int m_iUnitId;						//��ԪID
	SString m_sSystemType;				//ϵͳ�ͺ�
	SString m_sHmiStyle;				//HMI������
	SString m_sUnitName;				//��Ԫ����
	SString m_sUnitDesc;				//��Ԫ����
	SString m_sThisIp;					//����Ԫ��IP��ַ
	eHardwareType m_HwType;				//��ԪӲ������
	SPtrList<CUnitNetInterface> m_Nets;	//��������
	SPtrList<CUnitDisk> m_Disks;		//���д���
	SPtrList<CUnitNas> m_Nass;			//��������洢��
	SPtrList<CSpUnit> m_Units;			//���еĵ�Ԫ
	SString m_sUsbStartDev;				//U����ʼ�豸�����磺sdd
	SStringList m_slAutoRunCmd;			//�����Զ����е�����
};

#endif//__SP_UNIT_CONFIG_H__
