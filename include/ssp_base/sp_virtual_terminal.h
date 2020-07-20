/**
 *
 * �� �� �� : sp_virtual_terminal.h
 * �������� : 2015-12-23 15:48
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ��˿���ض��弰���ַ�װ
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-12-23	�ۿ�������ļ�
 *
 **/

#ifndef __SP_VIRTUAL_TERMINAL_H__
#define __SP_VIRTUAL_TERMINAL_H__

#include "sp_sclparser.h"
#include "SBaseConfig.h"
#include "SApi.h"
#include "ssp_base_inc.h"

//����SCL��˿���ؽṹ����
class CSclVt;
struct stuSclVtIed;
struct stuSclVtIedLd;
struct stuSclVtIedDatasetFcda;
struct stuSclVtIedDataset;
struct stuSclVtIedSmvOut;
struct stuSclVtIedGooseOut;
struct stuSclVtIedSmvIn;
struct stuSclVtIedSmvInChannel;
struct stuSclVtIedGooseIn;
struct stuSclVtIedGooseInChannel;
struct stuSclVtIedUnknownInput;
struct stuSclVtSubNetwork;
class SSP_BASE_EXPORT CSclVt
{
public:
	SString m_sStationName;
	SPtrList<stuSclVtSubNetwork> m_SubNetwork;
	SPtrList<stuSclVtIed> m_Ied;
	CSclVt()
	{
		m_Ied.setAutoDelete(true);
		m_SubNetwork.setAutoDelete(true);
		m_fPrimaryVoltage = 220000;//һ�ζ��ѹ
		m_fSecondaryVoltage = 110;//���ζ��ѹ
		m_fPriaryCurrent=100;//һ�ζ����
		m_fSecondaryCurrent=1;//���ζ����
	}
	~CSclVt()
	{
		Clear();
	}
	void Clear()
	{
		m_Ied.clear();
		m_SubNetwork.clear();
		m_ConstString.Clear();
	}
	
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��SAX��SCL�������м����������Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-23 16:18
	// ����˵��:  pSclParser
	//         :  @pIeds=NULL��ʾ����ȫ��
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool LoadBySclParser(CSpSclParserEx *pSclParser);

	bool LoadCommunicationBySclParser(CSpSclParserEx *pSclParser);

	bool LoadIedBySclParser(CSpSclParserEx *pSclParser,stuSclVtIed *pVtIed,CSpSclParserEx::stuSCL_IED *pIed);

	bool LoadIedInputsBySclParser(CSpSclParserEx *pSclParser,stuSclVtIed *pVtIed,CSpSclParserEx::stuSCL_IED *pIed);

	stuSclVtIed* SearchIedByName(char *ied_name);

	bool LoadByBaseConfig(SBaseConfig *pParentNode);

	SConstBuffer m_ConstString;
	float m_fPrimaryVoltage;//һ�ζ��ѹ
	float m_fSecondaryVoltage;//���ζ��ѹ
	float m_fPriaryCurrent;//һ�ζ����
	float m_fSecondaryCurrent;//���ζ����
};

struct SSP_BASE_EXPORT stuSclVtSubNetworkConnectAPAddress
{
	const char* ip;
	const char* ip_subnet;
	const char* ip_gateway;
};
struct SSP_BASE_EXPORT stuSclVtSubNetworkConnectAPGSE
{
	const char* cb_name;
	const char* ld_inst;
	const char* mac_address;
	const char* appid;
	const char* vlan_priority;
	const char* vlan_id;
};
struct SSP_BASE_EXPORT stuSclVtSubNetworkConnectAPSMV
{
	const char* cb_name;
	const char* ld_inst;
	const char* mac_address;
	const char* appid;
	const char* vlan_priority;
	const char* vlan_id;
};
struct SSP_BASE_EXPORT stuSclVtSubNetworkConnectAP
{
	const char* ied_name;
	const char* ap_name;
	const char* desc;
	stuSclVtSubNetworkConnectAP()
	{
		m_Address.setAutoDelete(true);
		m_GSE.setAutoDelete(true);
		m_SMV.setAutoDelete(true);
	}
	~stuSclVtSubNetworkConnectAP()
	{
		m_Address.clear();
		m_GSE.clear();
		m_SMV.clear();
	}
	SPtrList<stuSclVtSubNetworkConnectAPAddress> m_Address;
	SPtrList<stuSclVtSubNetworkConnectAPGSE> m_GSE;
	SPtrList<stuSclVtSubNetworkConnectAPSMV> m_SMV;
};
struct SSP_BASE_EXPORT stuSclVtSubNetwork
{
	const char* desc;
	const char* name;
	const char* type;
	stuSclVtSubNetwork()
	{
		m_ConnectAP.setAutoDelete(true);
	}
	~stuSclVtSubNetwork()
	{
		m_ConnectAP.clear();
	}
	SPtrList<stuSclVtSubNetworkConnectAP> m_ConnectAP;
};


//<ied name="PT2201B" desc="1#��ѹ������B��" manufacturer="SAC" type="PST1200U" configVersion="1.30GIR.000" >
struct SSP_BASE_EXPORT stuSclVtIed
{
	const char* name;
	const char* desc;
	const char* manufacturer;
	const char* type;
	const char* configVersion;

	stuSclVtIed()
	{
		m_Ld.setAutoDelete(true);
		m_Dataset.setAutoDelete(true);
		m_SmvOut.setAutoDelete(true);
		m_GooseOut.setAutoDelete(true);
		m_SmvIn.setAutoDelete(true);
		m_GooseIn.setAutoDelete(true);
		m_UnknownInput.setAutoDelete(true);
	}
	~stuSclVtIed()
	{
		m_Ld.clear();
		m_Dataset.clear();
		m_SmvOut.clear();
		m_GooseOut.clear();
		m_SmvIn.clear();
		m_GooseIn.clear();
		m_UnknownInput.clear();
	}
	stuSclVtIedLd* SearchLdByName(char* ld_name);
	stuSclVtIedDataset* SearchDatasetByName(char* ld_name,char* ds_name);
	stuSclVtIedSmvOut* SearchSmvOutByNo(int smv_no);
	stuSclVtIedGooseOut* SearchGooseOutByNo(int goose_no);
	stuSclVtIedSmvOut* SearchSmvOutByDatasetName(char* ds_name);
	stuSclVtIedGooseOut* SearchGooseOutByDatasetName(char* ds_name);
	stuSclVtIedSmvOut* SearchSmvOutByAppid(int appid);
	stuSclVtIedGooseOut* SearchGooseOutByAppid(int appid);
	stuSclVtIedSmvIn* SearchSmvInByNo(int int_smv_no);
	stuSclVtIedGooseIn* SearchGooseInByNo(int int_goose_no);
	stuSclVtIedSmvIn* SearchSmvInByExtNo(const char* ext_iedname,int ext_smv_no);
	stuSclVtIedGooseIn* SearchGooseInByExtNo(const char* ext_iedname,int ext_goose_no);

	SPtrList<stuSclVtIedLd> m_Ld;
	SPtrList<stuSclVtIedDataset> m_Dataset;
	SPtrList<stuSclVtIedSmvOut> m_SmvOut;
	SPtrList<stuSclVtIedGooseOut> m_GooseOut;
	SPtrList<stuSclVtIedSmvIn> m_SmvIn;
	SPtrList<stuSclVtIedGooseIn> m_GooseIn;
	SPtrList<stuSclVtIedUnknownInput> m_UnknownInput;
};

//<ld inst="PROT" desc="����LD" /> 
struct SSP_BASE_EXPORT stuSclVtIedLd
{
	const char* inst;
	const char* desc;
};

enum eSclVtFcdaType
{
	SCL_VT_MU_DELAY=1,
	SCL_VT_MU_CHN_AI,
	SCL_VT_MU_CHN_AU,
	SCL_VT_MU_CHN_D,
};

//<fcda chn_no="1" ln_desc="" doi_desc="" dai_desc="" vtype="1" dime="" path="" value="v=1;angle=0;hz=50;q=0;t=0;" />
struct SSP_BASE_EXPORT stuSclVtIedDatasetFcda
{
	int chn_no;
	int chn_type;//�����ֶ�ͨ������
	const char* ln_desc;
	const char* doi_desc;
	const char* dai_desc;
	const char* ln_type;
	const char* do_type;
	const char* bType;
	int vtype;//0:δ֪  1:�ַ���  2������  3��������  4��bitstring  5:utctime  6:bool  7:Ʒ��  8:Struct ...
	const char* dime;
	SString path;
	SString value;
	float primary_rated;//һ�ζ
	float secondary_rated;//���ζ
	bool IsTest()
	{
		//DWORD q = (DWORD)SString::GetAttributeValueI(value,"q");
		//MEMO: TODO [2016-1-15 13:26 �ۿ���]
		return true;	
	}
	SString GetValue()
	{
		if(value.length())
			return "";
		SString v = SString::GetAttributeValue(value,"v");
		//DWORD q = (DWORD)SString::GetAttributeValueI(value,"q");
		//MEMO: TODO [2016-1-15 13:26 �ۿ���]
		return v;
	}
};

//<dataset ds_no="1" name="aa" desc="bb">
struct SSP_BASE_EXPORT stuSclVtIedDataset
{
	int ds_no;
	const char* ld_name;
	const char* name;
	const char* desc;
	stuSclVtIedDataset()
	{
		m_Fcda.setAutoDelete(true);
	}
	~stuSclVtIedDataset()
	{
		m_Fcda.clear();
	}
	stuSclVtIedDatasetFcda* SearchFcdaByChnNo(int chnno)
	{
		unsigned long pos;
		stuSclVtIedDatasetFcda *pFcda = m_Fcda.FetchFirst(pos);
		while(pFcda)
		{
			if(pFcda->chn_no == chnno)
				return pFcda;
			pFcda = m_Fcda.FetchNext(pos);
		}
		return NULL;
	}
	SPtrList<stuSclVtIedDatasetFcda> m_Fcda;
	const char* iedname;
};


//<smv-out smv_no="1" name="MSVCB01" datSet="dsSV1" confRev="1" nofASDU="1" smpRate="80" smvID="ML1103MU/LLN0$MS$MSVCB01" 
//multicast="true" desc="" appid="0x4001" mac="01-0C-CD-04-00-01" vlan="0" vlan_priority="0" refreshTime="false" 
//sampleRate="true" sampleSynchronized="true" security="false" dataRef="false" >
struct SSP_BASE_EXPORT stuSclVtIedSmvOut
{
	int smv_no;
	const char* name;
	const char* datSet;
	int confRev;
	int nofASDU;
	int smpRate;
	const char* smvID;
	bool multicast;
	const char* desc;
	int appid;
	const char* mac;
	int vlan;
	int vlan_priority;
	bool refreshTime;
	bool sampleRate;
	bool sampleSynchronized;//���������б����У��ɸ���SV�Ĳɼ�ͬ����Ǹ��¸��ֶ�
	bool security;
	bool dataRef;
	
	bool bActive;//��ʾ�ÿ��ƿ��ǻ�У�����ʵ�ʱ��ģ�,�������б��ĸ���
	bool bInvalid;//true��ʾ�ÿ��ƿ�������Чͨ��
	bool bTest;//true��ʾ�ÿ��ƿ����м���ͨ��
	stuSclVtIedDataset* m_pDataset;
	SString ied_name;
	SString ds_name;
	stuSclVtIedSmvOut()
	{
		m_pDataset = NULL;
		bActive = false;
		bInvalid = false;
		bTest = false;
	}
};

//<goose-out gse_no="1" gocb="ML1104RPIT/LLN0$GO$gocb1" datSet="dsGOOSE1" confRev="1" name="gocb1" appid="0x01" 
// mac="01-0C-CD-04-00-01" vlan="0" vlan_priority="0" min_time="2" max_time="5000" >
struct SSP_BASE_EXPORT stuSclVtIedGooseOut
{
	int gse_no;
	const char* gocb;
	const char* datSet;
	int confRev;
	const char* name;
	int appid;
	const char* mac;
	int vlan;
	int vlan_priority;
	int min_time;
	int max_time;

	bool bActive;//��ʾ�ÿ��ƿ��ǻ�У�����ʵ�ʱ��ģ�,�������б��ĸ���
	bool bTest;//true��ʾ�ÿ��ƿ鴦�ڼ���״̬
	stuSclVtIedDataset* m_pDataset;
	SString ied_name;
	SString ds_name;
	stuSclVtIedGooseOut()
	{
		m_pDataset = NULL;
		bActive = false;
		bTest = false;
	}
};

//δ���ã�����Ч���ã������������
struct SSP_BASE_EXPORT stuSclVtIedUnknownInput
{
	const char* intAddr;
	const char* iedName;
	const char* ldInst;
	const char* lnClass;
	const char* lnInst;
	const char* doName;
	const char* daName;
	const char* prefix;
};

//<smv-in int_smv_no="1" ext_iedname="" ext_smv_no="1">
struct SSP_BASE_EXPORT stuSclVtIedSmvIn
{
	int int_smv_no;
	const char* ext_iedname;
	int ext_smv_no;

	stuSclVtIed* m_pIed;//NULL��ʾδ�ҵ���Ӧ�ⲿ��Ϣ��δ���ã�����Ч���ã��������
	stuSclVtIedSmvOut *m_pSmvOut;
	stuSclVtIedSmvIn()
	{
		m_pIed = NULL;
		m_pSmvOut = NULL;
		m_Channel.setAutoDelete(true);
	}
	~stuSclVtIedSmvIn()
	{
		m_Channel.clear();
	}
	SPtrList<stuSclVtIedSmvInChannel> m_Channel;
};

//<channel int_chn_no="1" int_chn_desc="" int_path="" ext_chn_no="1" />
struct SSP_BASE_EXPORT stuSclVtIedSmvInChannel
{
	int int_chn_no;
	const char* int_chn_desc;
	const char* int_path;
	int ext_chn_no;
	stuSclVtIedDatasetFcda *m_pFcda;
};

//<goose-in int_gse_no="1" ext_iedname="" ext_ldname="" ext_gse_no="1" >
struct SSP_BASE_EXPORT stuSclVtIedGooseIn
{
	int int_gse_no;
	const char* ext_iedname;
	const char* ext_ldname;
	int ext_gse_no;

	stuSclVtIed* m_pIed;
	stuSclVtIedGooseOut *m_pGooseOut;
	stuSclVtIedGooseIn()
	{
		m_pIed = NULL;
		m_pGooseOut = NULL;
		m_Channel.setAutoDelete(true);
	}
	~stuSclVtIedGooseIn()
	{
		m_Channel.clear();
	}
	SPtrList<stuSclVtIedGooseInChannel> m_Channel;
};

//<channel int_chn_no="1" int_chn_desc="" int_path="" ext_chn_no="1" />
struct SSP_BASE_EXPORT stuSclVtIedGooseInChannel
{
	int int_chn_no;
	const char* int_chn_desc;
	const char* int_path;
	int ext_chn_no;
	stuSclVtIedDatasetFcda *m_pFcda;
};



#endif//__SP_VIRTUAL_TERMINAL_H__
