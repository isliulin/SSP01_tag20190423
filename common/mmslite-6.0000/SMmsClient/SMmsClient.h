/**
 *
 * �� �� �� : SMmsClient.h
 * �������� : 2015-9-16 19:22
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : sbase-MMS�ͻ��˷�װ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-9-16	�ۿ�������ļ�
 *
 **/

#include "SInclude.h"
#include <map>

#ifdef _WIN32
  #ifdef  SMMS_CLIENT_EXPORT_DLL
    #define SMMS_CLIENT_EXPORT __declspec(dllexport)
  #ifdef NDEBUG
    #pragma comment(lib,"winmm.lib")
    #pragma comment(lib,"ws2_32.lib")
    #pragma comment(lib,"Packet.lib")
#ifdef WIN64
	#pragma comment(lib,"sbase64.lib")
#else
	#pragma comment(lib,"sbase.lib")
#endif
    #pragma comment(lib,"advapi32.lib")
    #pragma comment(lib,"asn1_l.lib")
    #pragma comment(lib,"gse_mgmt_l.lib")
    #pragma comment(lib,"mem_l.lib")
    #pragma comment(lib,"mlog_l.lib")
    #pragma comment(lib,"mmsl_l.lib")
    #pragma comment(lib,"mmsle_l.lib")
    #pragma comment(lib,"mvlu_l.lib")
    #pragma comment(lib,"ositpxs_l.lib")
    #pragma comment(lib,"slog_l.lib")
    #pragma comment(lib,"smpval_l.lib")
    #pragma comment(lib,"utility_l.lib")
    
    #pragma comment(linker,"/nodefaultlib:libcmt.lib")
    #pragma comment(linker,"/nodefaultlib:libcmt.lib")
  #else//NDEBUG
    #pragma comment(lib,"winmm.lib")
    #pragma comment(lib,"ws2_32.lib")
    #pragma comment(lib,"Packet.lib")
#ifdef WIN64
	#pragma comment(lib,"sbase64d.lib")
#else
	#pragma comment(lib,"sbased.lib")
#endif
    #pragma comment(lib,"advapi32.lib")
    #pragma comment(lib,"asn1_ld.lib")
    #pragma comment(lib,"gse_mgmt_ld.lib")
    #pragma comment(lib,"mem_ld.lib")
    #pragma comment(lib,"mlog_ld.lib")
    #pragma comment(lib,"mmsl_ld.lib")
    #pragma comment(lib,"mmsle_ld.lib")
    #pragma comment(lib,"mvlu_ld.lib")
    #pragma comment(lib,"ositpxs_ld.lib")
    #pragma comment(lib,"slog_ld.lib")
    #pragma comment(lib,"smpval_ld.lib")
    #pragma comment(lib,"utility_ld.lib")    
    
    #pragma comment(linker,"/nodefaultlib:libcmtd.lib")
    #pragma comment(linker,"/nodefaultlib:libcmt.lib")
  #endif//NDEBUG
  #else //SMMS_CLIENT_EXPORT_DLL
    #define SMMS_CLIENT_EXPORT __declspec(dllimport)
    #ifdef NDEBUG
      #pragma comment(lib,"SMmsClient.lib")
    #else//NDEBUG
      #pragma comment(lib,"SMmsClientd.lib")
    #endif//NDEBUG
  #endif//SMMS_CLIENT_EXPORT_DLL
#else//WIN32
  #define SMMS_CLIENT_EXPORT
#endif//WIN32

//���津������
#define C_SMMS_TRGOPT_DATA	0x40	//���ݱ仯
#define C_SMMS_TRGOPT_Q		0x20	//Ʒ�ʱ仯
#define C_SMMS_TRGOPT_REF	0x10	//����ˢ��
#define C_SMMS_TRGOPT_ZQ	0x08	//����
#define C_SMMS_TRGOPT_ZZ	0x04	//����
#define C_SMMS_TRGOPT_ALL	0x7C	//ȫ����������

//ԭ����
#define REASON_RESERVED					0	//����ԭ��
#define REASON_DATA_CHANGE				1	//���ݱ仯ԭ��
#define REASON_QUALITY_CHANGE			2	//Ʒ�ʱ仯ԭ��
#define REASON_DATA_UPDATE				3	//���ݸ���ԭ��
#define REASON_INTEGRITY				4	//��������ԭ��
#define REASON_GENERAL_INTERROGATION	5	//���ٻ�ԭ��

#define QUALITY_GOOD			0
#define QUALITY_INVALID			1
#define QUALITY_RESERVED		2
#define QUALITY_QUESTIONABLE	3
typedef struct MMS_QUALITY MMS_QUALITY;
struct SMMS_CLIENT_EXPORT MMS_QUALITY 
{
	unsigned short oldData:1;			//�����ݣ���ʾֵ�ڸ�����ʱ���ڲ�����	
	unsigned short failure:1;			//���ϣ���ʾ���ӹ����Ѿ����һ���ڲ����ⲿ����
	unsigned short oscillatory:1;		//����
	unsigned short badReference:1;		//����׼ֵ����ʾ���ڻ�׼ֵ�޷�У׼��ֵ���ܲ���ȷ
	unsigned short outOfRange:1;		//��ֵ�򣬱�ʾ�����Ʒ������������Գ�����Ԥ�ȶ����ֵ��
	unsigned short overFlow:1;			//�������ʾ�����Ʒ������������Գ�����������ȷ��ʾֵ������
	unsigned short validity:2;			//��Ч��
	unsigned short res:3;				//����
	unsigned short operatorBlocked:1;	//����Ա��������ʾֵ��ˢ���Ѿ���������ͬʱ������ҲӦ��λ
	unsigned short test:1;				//���ԣ���ʾֵΪ���ڲ��ԣ�������������Ŀ��
	unsigned short substituted:1;		//Դ(����0/��ȡ��1)������ֵ����Դ��Ϣ
	unsigned short inaccurate:1;		//����ȷ����ʾֵ������ԴҪ��ľ���
	unsigned short inconsistent:1;		//��һ�£���ʾ���������Ѿ������һ��
};
typedef union mms_quality_t mms_quality_t;
union mms_quality_t 
{         
	unsigned char byte[2];
	MMS_QUALITY MQ;
}; 

#define TIME_ACCURACY_UNKNOWN	31
#define TIME_ACCURACY_T0		7
#define TIME_ACCURACY_T1		10
#define TIME_ACCURACY_T2		14
#define TIME_ACCURACY_T3		16
#define TIME_ACCURACY_T4		18
#define TIME_ACCURACY_T5		20
typedef struct MMS_TIME_QUALITY MMS_TIME_QUALITY;
struct SMMS_CLIENT_EXPORT MMS_TIME_QUALITY 
{
	unsigned char timeAccuracy:5;	//ʱ��׼ȷ��
	unsigned char clockNotSync:1;	//ʱ��δͬ��
	unsigned char clockFailue:1;	//ʱ�ӹ���
	unsigned char leapSecondKnown:1;//��֪����
};
typedef union mms_time_quality_t mms_time_quality_t;
union mms_time_quality_t 
{         
	unsigned char byte;
	MMS_TIME_QUALITY MTQ;
}; 

class SMmsClient;

//////////////////////////////////////////////////////////////////////////
// ��    ��:  stuSMmsReportInfo
// ��    ��:  �ۿ���
// ����ʱ��:  2015-9-22 20:10
// ��    ��:  ������Ϣ�ṹ����
//////////////////////////////////////////////////////////////////////////
struct SMMS_CLIENT_EXPORT stuSMmsReportInfo
{
	SString dom_name;
	SString rcb_name;
	SString rptID;//����ID, example:PROT/LLN0.brcbWarning
	SString buffered;//�Ƿ��ǻ��汨��
	int report_en_state;//0:δʹ�ܣ�1:ʹ�ܳɹ���2:ʹ��ʧ��
	SString report_en_error;//����ʹ�ܴ�������
	int report_id;//�����,��1��ʼ��ţ�һ��Ϊ1~8
	unsigned int IntgPd;//����������(ms)
	unsigned char OptFlds[2];
	unsigned char TrgOps[1];
	unsigned char EntryID[8];
	void* pRCB_INFO;//RCB_INFO*
};

struct SMMS_CLIENT_EXPORT stuSMmsFileInfo
{
	SString filename;	//file name
	int fsize;			//file size (# bytes)
	bool mtimpres;		//last modified time present
	time_t mtime;		//last modified time
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  stuSMmsReportItem
// ��    ��:  �ۿ���
// ����ʱ��:  2015-10-9 9:12
// ��    ��:  MMSͻ������ص���ϸ��
//////////////////////////////////////////////////////////////////////////
struct SMMS_CLIENT_EXPORT stuSMmsReportItem
{
	void* dataRefName;//����ΪMVL_VAR_ASSOC*
	void* dataValue;//����ΪMVL_VAR_ASSOC*
	void* Reason;//����ΪMVL_VAR_ASSOC*
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  stuSMmsReport
// ��    ��:  �ۿ���
// ����ʱ��:  2015-10-9 9:11
// ��    ��:  MMSͻ������ص��ṹ
//////////////////////////////////////////////////////////////////////////
struct SMMS_CLIENT_EXPORT stuSMmsReport
{
	void*  RptID;//����ΪMVL_VAR_ASSOC*����ͬ
	void*  OptFlds;
	void*  SqNum;
	void*  TimeOfEntry;
	void*  DatSetNa;
	void*  BufOvfl;
	void*  SubSeqNum;
	void*  MoreSegmentsFollow;
	void*  EntryID;
	void*  ConfRev;
	void*  Inclusion;
	SString sRptID;
	SPtrList<stuSMmsReportItem> Items;

	stuSMmsReport()
	{
		memset(&RptID,0,sizeof(RptID)*11);
		Items.setAutoDelete(true);
	}
	void AddItem(void* ref,void* val,void* q)
	{
		stuSMmsReportItem *p = new stuSMmsReportItem();
		p->dataRefName = ref;
		p->dataValue = val;
		p->Reason = q;
		Items.append(p);
	}
};

typedef enum
{
	TYPE_STRUCT,
	TYPE_ARRAY,
	TYPE_BOOL,
	TYPE_INT8,	
	TYPE_INT16,	
	TYPE_INT32,
	TYPE_INT64,
	TYPE_UINT8,
	TYPE_UINT16,
	TYPE_UINT32,
	TYPE_UINT64,
	TYPE_FLOAT,
	TYPE_DOUBLE,
	TYPE_BIT_STRING,
	TYPE_UTC_TIME,
	TYPE_UTF8_STRING,
	TYPE_BINARY_TIME,
	TYPE_UNKNOWN
} VALUE_TYPE;

class SMMS_CLIENT_EXPORT MMSValue
{
public:
	MMSValue()
	{
		type = TYPE_UNKNOWN;
		strValue = "";
		bValue = false;
		i8Value = 0;
		i16Value = 0;
		i32Value = 0;
		i64Value = 0;
		ui8Value = 0;
		ui16Value = 0;
		ui32Value = 0;
		ui64Value = 0;
		fValue = 0.0;
		dValue = 0.0;
	}

	~MMSValue() {};

	void setName(SString nm) { name = nm; }
	void setType(int m_type) { type = m_type; }
	void setStrValue(SString str_val) { strValue = str_val; }
	void setBoolValue(bool bool_val) { bValue = bool_val; }
	void setInt8Value(signed char i8_val) { i8Value = i8_val; }
	void setInt16Value(signed short i16_val) { i16Value = i16_val; }
	void setInt32Value(signed int i32_val) { i32Value = i32_val; }
	void setInt64Value(INT64 i64_val) { i64Value = i64_val; }
	void setUint8Value(unsigned char ui8_val) { ui8Value = ui8_val; }
	void setUint16Value(unsigned short ui16_val) { ui16Value = ui16_val; }
	void setUint32Value(unsigned int ui32_val) { ui32Value = ui32_val; }
	void setUint64Value(UINT64 ui64_val) { ui64Value = ui64_val; }
	void setFloatValue(float float_val) { fValue = float_val; }
	void setDoubleValue(double double_val) { dValue = double_val; }

	SString			getName() { return name; }
	int				getType() { return type; }
	SString			getStrValue() { return strValue; }
	bool			getBoolValue() { return bValue; }
	signed char		getInt8Value() { return i8Value; }
	signed short	getInt16Value() { return i16Value; }
	signed int		getInt32Value() { return i32Value; }
	INT64			getInt64Value() { return i64Value; }
	unsigned char	getUint8Value() { return ui8Value; }
	unsigned short	getUint16Value() { return ui16Value; }
	unsigned int	getUint32Value() { return ui32Value; }
	UINT64			getUint64Value() { return ui64Value; }
	float			getFloatValue() { return fValue; }
	double			getDoubleValue() { return dValue; }

private:
	SString			name;
	int				type;
	SString			strValue;
	bool			bValue;
	signed char		i8Value;
	signed short	i16Value;
	signed int		i32Value;
	INT64			i64Value;
	unsigned char	ui8Value;
	unsigned short	ui16Value;
	unsigned int	ui32Value;
	UINT64			ui64Value;
	float			fValue;
	double			dValue;
};

struct SMMS_CLIENT_EXPORT stuJournalData
{
	SString ref;
	char	entry_id[8];
	SPtrList<MMSValue> value;
};

struct SMMS_CLIENT_EXPORT stuSMmsDataNode
{
	SString sDesc;//����
	SString sMmsPath;//LLN0$BR$brcbWarning01$RptID
	void* pValue;//MVL_VAR_ASSOC*, NULL��ʾΪ�գ���һ��ʹ��ʱ�����Ҳ����ͷ�,����ʱ�Զ��ͷ�
	int iMmsOperId;//�������ͺţ�MMSOP_READ��MMSOP_WRITE��MMSOP_INFO_RPT��MMSOP_RD_USR_HANDLED ......
	void	*user;	//�û��Զ���

	stuSMmsDataNode();
	~stuSMmsDataNode();
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CSMmsDataset
// ��    ��:  �ۿ���
// ����ʱ��:  2015-10-9 17:32
// ��    ��:  MMS���ݼ���װ��
//////////////////////////////////////////////////////////////////////////
class SMMS_CLIENT_EXPORT CSMmsDataset
{
public:
	CSMmsDataset();
	~CSMmsDataset();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���һ���µ���������Ŀ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-10-9 17:29
	// ����˵��:  @mmspathΪ�ڵ�ȫ·��(��:PL2202APROT/LLN0$BR$brcbWarning01$RptID)
	// �� �� ֵ:  stuSMmsDataNode*
	//////////////////////////////////////////////////////////////////////////
	stuSMmsDataNode* AddItem(SString desc,SString mmspath);

	SString m_sIedName;//װ��mms����
	SString m_sDsPath;//���ݼ�·��
	SString m_sDsName;//���ݼ����ƣ���������
	int m_iDsType;//���ݼ�����:1-ң��,2-ң��,3-�¼�,4-�澯,5-��ѹ��,6-����,7-��ֵ,8-������,9-ͨѶ���� ......
	void	*user;//�û��Զ���
	SPtrList<stuSMmsDataNode> m_Items;
};

/********************************************  
* @brief CSMmsLogicDevice
* @author �ο�
* @date 2016/01/04
* @return MMS�߼�װ�÷�װ��
*********************************************/ 
class SMMS_CLIENT_EXPORT CSMmsLogicDevice
{
public:
	CSMmsLogicDevice(SMmsClient*);
	~CSMmsLogicDevice();
 
	/********************************************  
	* @brief ����������Ŀ�б�
	* @author �ο�
	* @date 2016/01/04
	* @param int iTimeout_s ��ʱʱ�䣨Ĭ��10�룩
	* @return true��ʾ���óɹ�,false��ʾ����ʧ��
	*********************************************/ 
	bool SetVarNameList(int iTimeout_s=10);

public:
	SMmsClient	*m_client;	//����ʵ��,MVL_NET_INFO*,NULL��ʾδ����
	SString		m_sLdPath;		//�߼�װ��·����PL2202APROT��
	SString		m_sLdName;		//�߼�װ�����ƣ���������	
	void		*user;			//�û��Զ���
	SPtrList<SString> m_VarNames;

private:
	/********************************************  
	* @brief ��ȡ������Ŀ�б�
	* @author �ο�
	* @date 2016/01/04
	* @param void * req_info ������Ϣ��NAMELIST_REQ_INFO*
	* @param void * resp_info �ظ���Ϣ��NAMELIST_RESP_INFO*
	* @param int iTimeout_s ��ʱʱ��
	* @return int �Ƿ��ȡ�ɹ���0��ʾ�ɹ�����0��ʾ���ɹ�
	*********************************************/ 
	int getVarName(void *req_info,void *resp_info,int iTimeout_s);
};

class SMMS_CLIENT_EXPORT SMmsClient
{
public:
	SMmsClient();
	virtual ~SMmsClient();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����MMS��������ַ������������ǰ��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-16 19:36
	// ����˵��:  @sServerIpAΪ�����A��IP��ַ
	//         :  @sServerIpBΪ�����B��IP��ַ
	//         :  @iPortΪ����TCP�˿ںţ�ȱʡΪ102
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetMmsServerAddr(char *sServerIpA,char *sServerIpB="",int iPort=102);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ��MMS�ͻ������л���,һ�����̽�����һ�μ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-21 18:20
	// ����˵��:  @bSingleThreadMode��ʾ����ͻ���ʵ��ʱ�Ƿ���õ��߳�����ģʽ��true��ʾ���߳�ģʽ��false��ʾ���߳�ģʽ
	// �� �� ֵ:  true��ʾ��ʼ���ɹ���false��ʾ��ʼ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool InitGlobal(bool bSingleThreadMode=false);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȫ���˳�ʱ�����ٺ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-22 18:37
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	static bool ExitGlobal();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������,�麯�������������б����ȵ��ô˺���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-16 19:24
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ�������������б����ȵ��ô˺���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-16 19:24
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ���ַ���˿ڵ�MMS������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-16 19:24
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ���ӳɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool Connect();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ͽ������˵�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-16 19:26
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�Ͽ��ɹ�,false��ʾ�Ͽ�ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool DisConnect();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰʵ���Ƿ����ӵ��ͻ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-5-26 14:18
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool IsConnected(){return (m_pMVL_NET_INFO_A != NULL && (m_pMVL_NET_INFO_B != NULL || m_sServerIpB[0] == '\0'));}
	bool IsConnected_A(){return m_pMVL_NET_INFO_A != NULL;};
	bool IsConnected_B(){return m_pMVL_NET_INFO_B != NULL;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ͽ�A��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-22 14:21
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�Ͽ��ɹ�,false��ʾ�Ͽ�ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool DisConnect_A();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ͽ�B��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-22 14:21
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�Ͽ��ɹ�,false��ʾ�Ͽ�ʧ��
	//////////////////////////////////////////////////////////////////////////
	bool DisConnect_B();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ɹ����ӷ����֮��Ļص��麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-16 19:29
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnConnected(){};

	virtual void OnConnected_A(){};
	virtual void OnConnected_B(){};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ӷ���˶Ͽ��Ļص��麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-16 19:30
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnDisConnected(){};

	virtual void OnDisConnected_A(){};
	virtual void OnDisConnected_B(){};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ÿ����һ�εĻص�����Ӧ�ò����ִ�ж�ʱ������ͨ��ȡ����˱�ʶȷ��ͨ���Ƿ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-5-26 10:30
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnSecondTimer(){};

	inline void* GetMVL_NET_INFO_A(){return m_pMVL_NET_INFO_A;};
	inline void* GetMVL_NET_INFO_B(){return m_pMVL_NET_INFO_B;};
	inline void* GetUserInfo_A(){return m_pUserInfo_A;};
	inline void* GetUserInfo_B(){return m_pUserInfo_B;};


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����µı�����Ϣ,����ӵ����������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-24 8:24
	// ����˵��:  @dom_name��ʾIED+LD����
	//		   :  @rptID��ʾ����ID,example:PROT/LLN0.brcbWarning
	//		   :  @buffered��ʾ�Ƿ��ǻ��汨��
	//         :  @rcb_name��ʾ������ƿ�����
	//         :  @report_id��ʾ������ţ���1��ʼ��ţ�0��ʾ��ʵ�����棨����Ҫ����ţ�
	//         :  @IntgPd��ʾ���������ڣ���λms
	//         :  @TrgOpt��ʾ����ѡ��
	// �� �� ֵ:  stuSMmsReportInfo*
	//////////////////////////////////////////////////////////////////////////
	stuSMmsReportInfo* AddReportInfo(SString dom_name,SString rptID,SString buffered,
		SString rcb_name,int report_id,SString entry_id,int IntgPd=5000,unsigned char TrgOpt=C_SMMS_TRGOPT_ALL);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��������ƿ飬���ӱ����б�ɾ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-24 9:00
	// ����˵��:  @pRptInfo��ʾ����ָ��
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool RemoveReportInfo(stuSMmsReportInfo *pRpt);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ������ƿ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-22 20:21
	// ����˵��:  void
	// �� �� ֵ:  int, Ϊ������ƿ������
	//////////////////////////////////////////////////////////////////////////
	int GetReportInfoCount(){return m_ReportInfos.count();};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ����ŵı���ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-22 20:24
	// ����˵��:  @idxΪ��ţ���0��ʼ���
	// �� �� ֵ:  stuSMmsReportInfo*,NULL��ʾָ����ű��治����
	//////////////////////////////////////////////////////////////////////////
	stuSMmsReportInfo* GetReportInfo(int idx){return m_ReportInfos[idx];};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ʹ�����б���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-23 16:38
	// ����˵��:  @iTimeout_s��ʾ��ʱ������
	// �� �� ֵ:  <=0��ʾʹ��ʧ�ܣ�>0��ʾʹ�ܳɹ���������=GetReportInfoCount()��ʾȫ��ʹ�ܳɹ�
	//////////////////////////////////////////////////////////////////////////
	int EnableReport(int iTimeout_s=10);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ر����б���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-23 16:39
	// ����˵��:  void
	// �� �� ֵ:  <=0��ʾ�ر�ʧ�ܣ�>0��ʾʹ�ܳɹ���������=GetReportInfoCount()��ʾȫ��ʹ�ܳɹ�
	//////////////////////////////////////////////////////////////////////////
	int DisableReport(bool bNeedLock=true);

	inline void lock(){m_Lock.lock();};
	inline void unlock(){m_Lock.unlock();};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ��MVL����ת��Ϊ�ַ���������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-24 17:02
	// ����˵��:  @pValΪMVL_VAR_ASSOC*
	// �� �� ֵ:  SString
	//////////////////////////////////////////////////////////////////////////
	SString GetMvlVarText(void *pVal);

	/********************************************  
	* @brief ��ָ��MVL����ת��Ϊֵ����в�����
	* @author �ο�
	* @date 2015/12/25
	* @param void * pVal MVL_VAR_ASSOC*����
	* @param SPtrList<MMSValue> & valueList ֵ��ת������
	* @return int =0��ʾת���ɹ���>0��<0��ʾת��ʧ��
	*********************************************/ 
	int GetMvlVarValueList(void *pVal,SPtrList<MMSValue> &valueList);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡMVLĿ¼����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-24 18:43
	// ����˵��:  @sPathΪĿ¼����
	//         :  @slFiles��Ŀ¼����
	//         :  @bNeedToSort��ʾ�Ƿ���Ҫ����true����false������
	// �� �� ֵ:  >0��ʾ����������<0��ʾʧ��, =0��ʾ����Ϊ��
	//////////////////////////////////////////////////////////////////////////
	int GetMvlDirectory(SString sPath,SPtrList<stuSMmsFileInfo> &slFiles,bool bNeedToSort=false);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����MMS�����ָ�����ļ����ļ�ϵͳ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-24 18:47
	// ����˵��:  @sPathFile��ʾ����˵�Դ�ļ���
	//         :  @sLocalFile��ʾĿǰ�ļ���
	// �� �� ֵ:  true��ʾ�ɹ�,false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool DownMvlFile(SString sPathFile, SString sLocalFile);
	
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����MMS�����ָ�����ļ����ڴ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-24 18:48
	// ����˵��:  @sPathFile��ʾ����˵�Դ�ļ���
	//         :  @pBuffer��ʾĿ�껺������������Ϊ��ʱ�����ڲ�����ռ䲢���÷��أ������߸���delete�ͷ�
	//         :  @iLen��ʾĿ�껺�������ȣ���pBuffer�ǿ�ʱ��ʾ���������ȣ�����ʱ�ᱻ��ΪĿ�����ݵ��ֽ���
	// �� �� ֵ:  true��ʾ�ɹ�,false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool DownMvlFileToMem(SString sPathFile, BYTE* &pBuffer, int &iLen);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  MMSͻ�����洦��Ļص��麯����ʹ���걨��ṹ����Ҫͨ��delete�ͷű���ָ��
	//            ����������������غ�����ͬ������Ҳ���Խ�����뱨����н��л��洦��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-10-9 9:14
	// ����˵��:  @pMmsRpt��ʾ��ǰ���Ӷ�Ӧ��ͻ����������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnMmsReportProcess(stuSMmsReport* pMmsRpt){delete pMmsRpt;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����µ����ݼ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-10-9 17:42
	// ����˵��:  @sDsPathΪ���ݼ�ȫ·��
	//         :  @sDsNameΪ���ݼ�����
	//         :  @iDsTypeΪ���ݼ����ͣ��μ�CSMmsDataset::m_iDsType����)
	// �� �� ֵ:  CSMmsDataset*
	//////////////////////////////////////////////////////////////////////////
	CSMmsDataset* AddDataset(SString sIedPath,SString sDsPath,SString sDsName,int iDsType);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��ָ�������ݼ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-10-9 17:45
	// ����˵��:  @sDsPathΪ���ݼ�ȫ·����
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool RemoveDataset(SString sDsPath);

	CSMmsLogicDevice* AddLogicDevice(SString sLdPath,SString sLdName);

	/********************************************  
	* @brief ���ٻ����б�����ƿ�����
	* @author �ο�
	* @date 2015/12/28
	* @param int iTimeout_s ��ʱ��������Ĭ��10��
	* @return void
	*********************************************/ 
	void ProcessGi(int iTimeout_s=10);

	/********************************************  
	* @brief ���ٻ�ָ��������ƿ�����
	* @author �ο�
	* @date 2015/12/28
	* @param SString rcbName ������ƿ����� ��ʱ������
	* @param int iTimeout_s ��ʱ��������Ĭ��10��
	* @return void
	*********************************************/ 
	void ProcessGi(SString rcbName,int iTimeout_s=10);

	/********************************************  
	* @brief �ж�ԭ�����Ƿ�������ݱ仯
	* @author �ο�
	* @date 2015/12/29
	* @param SString reason
	* @return -true��ʾ���� -false��ʾ������
	*********************************************/ 
	bool ReasonIsDchg(SString reason);

	/********************************************  
	* @brief �ж�ԭ�����Ƿ����Ʒ�ʱ仯
	* @author �ο�
	* @date 2015/12/29
	* @param SString reason
	* @return -true��ʾ���� -false��ʾ������
	*********************************************/ 
	bool ReasonIsQchg(SString reason);

	/********************************************  
	* @brief �ж�ԭ�����Ƿ�������ݸ���
	* @author �ο�
	* @date 2015/12/29
	* @param SString reason
	* @return -true��ʾ���� -false��ʾ������
	*********************************************/ 
	bool ReasonIsDupd(SString reason);

	/********************************************  
	* @brief �ж�ԭ�����Ƿ�������ڱ仯
	* @author �ο�
	* @date 2015/12/29
	* @param SString reason
	* @return -true��ʾ���� -false��ʾ������
	*********************************************/ 
	bool ReasonIsPeriod(SString reason);

	/********************************************  
	* @brief �ж�ԭ�����Ƿ�������ٻ��仯
	* @author �ο�
	* @date 2015/12/29
	* @param SString reason
	* @return -true��ʾ���� -false��ʾ������
	*********************************************/ 
	bool ReasonIsGi(SString reason);

	/********************************************  
	* @brief ������Ŀ��ʶ��
	* @author �ο�
	* @date 2015/12/30
	* @param SString ied_name װ��MMS���ƣ��磺PT1101
	* @param stuSMmsReport * report �ص��ı���
	* @return void
	*********************************************/ 
	void SetReportEntryID(stuSMmsReport *report,stuSMmsReportInfo **rptInfo,SString &entry_id);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ��
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsGood(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ���Ч
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsInvalid(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ����
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsQuestionable(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ����
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsOverFlow(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ�ֵ��
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsOutOfRange(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ񻵻�׼ֵ
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsBadReference(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ񶶶�
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsOscillatory(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ����
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsFailure(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ������
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsOldData(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ�һ��
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsInconsistent(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ񲻾�ȷ
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsInaccurate(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ�ȡ��
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsSubstituted(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ����
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsTest(unsigned short quality);

	/********************************************  
	* @brief �ж�Ʒ���Ƿ����Ա����
	* @author �ο�
	* @date 2015/12/31
	* @param unsigned short quality Ʒ��ֵ
	* @return -true��ʾ�� -false��ʾ��
	*********************************************/ 
	bool QualityIsOperatorBlocked(unsigned short quality);

	/********************************************  
	* @brief ��ȡ��ֵ������
	* @author �ο�
	* @date 2016/01/05
	* @param char * dom_name ������PT1101LD0��
	* @param int & areaNum ������ķ�����ֵ
	* @param int timeOut ��ʱʱ�䣬Ĭ��10��
	* @return -true��ʾ��ȡ�ɹ� -false��ʾ��ȡʧ��
	*********************************************/ 
	bool ReadSettingAreaNumber(char *dom_name,int &areaNum,int timeOut=10);

	/********************************************  
	* @brief ��ȡ��ǰ��ֵ������
	* @author �ο�
	* @date 2016/01/11
	* @param char * dom_name ������PT1101LD0��
	* @param int & currentArea �����󷵻صĵ�ǰ��ֵ������
	* @param int timeOut ��ʱʱ�䣬Ĭ��10��
	* @return -true��ʾ��ȡ�ɹ� -false��ʾ��ȡʧ�� 
	*********************************************/ 
	bool ReadCurrentSettingArea(char *dom_name,int &currentArea,int timeOut=10);

	/********************************************  
	* @brief ��ȡ�༭��ֵ������
	* @author �ο�
	* @date 2016/01/11
	* @param char * dom_name ������PT1101LD0��
	* @param int & currentArea �����󷵻صı༭��ֵ������
	* @param int timeOut ��ʱʱ�䣬Ĭ��10��
	* @return -true��ʾ��ȡ�ɹ� -false��ʾ��ȡʧ�� 
	*********************************************/ 
	bool ReadEditSettingArea(char *dom_name,int &editArea,int timeOut=10);

	/********************************************  
	* @brief �޸ĵ�ǰ��ֵ������
	* @author �ο�
	* @date 2016/02/02
	* @param char * dom_name ������PT1101LD0��
	* @param int currentArea ���޸ĵĶ�ֵ������
	* @param int timeOut ��ʱʱ�䣬Ĭ��10��
	* @return -true��ʾ�޸ĳɹ� -false��ʾ�޸�ʧ�� 
	*********************************************/ 
	bool WriteCurrentSettingArea(char *dom_name,int currentArea,int timeOut=10);

	/********************************************  
	* @brief �޸ı༭��ֵ������
	* @author �ο�
	* @date 2016/02/02
	* @param char * dom_name ������PT1101LD0��
	* @param int currentArea ���޸ĵĶ�ֵ������
	* @param int timeOut ��ʱʱ�䣬Ĭ��10��
	* @return -true��ʾ�޸ĳɹ� -false��ʾ�޸�ʧ�� 
	*********************************************/ 
	bool WriteEditSettingArea(char *dom_name,int editArea,int timeOut=10);

	/********************************************  
	* @brief ��ȡ������ֵ
	* @author �ο�
	* @date 2016/01/05
	* @param char * dom_name ������PT1101LD0��
	* @param char * var_name ֵ����LLN0$SG$Enable$setVal��
	* @param SPtrList<MMSValue> & lstValue ������ķ�����ֵ����
	* @param int timeOut ��ʱʱ�䣬Ĭ��10��
	* @return -true��ʾ��ȡ�ɹ� -false��ʾ��ȡʧ��
	*********************************************/ 
	bool ReadValue(char *dom_name,char *var_name,SPtrList<MMSValue> &lstValue,int timeOut=10);

#define MAX_READ_VALUES_NUMBER	50	//������ֵ���������ֵ
	/********************************************  
	* @brief ��ȡ�����ֵ����ֵ��������(MAX_READ_VALUES_NUMBER=50)
	* @author �ο�
	* @date 2016/01/06
	* @param SPtrList<SString> & dom_name �������У�PT1101LD0��
	* @param SPtrList<SString> & var_name ֵ�����У�LLN0$SG$Enable$setVal��
	* @param SPtrList<SPtrList<MMSValue>> & lstValue ������ķ�����ֵ���еĶ���
	* @param int timeOut ��ʱʱ�䣬Ĭ��10��
	* @return -true��ʾ��ȡ�ɹ� -false��ʾ��ȡʧ��
	*********************************************/ 
	bool ReadValues(SPtrList<SString> &dom_name,SPtrList<SString> &var_name,
		SPtrList< SPtrList<MMSValue> > &lstValue,int timeOut=10);

	/********************************************  
	* @brief д������ֵ
	* @author �ο�
	* @date 2016/02/03
	* @param char * dom_name ������PT1101LD0��
	* @param char * var_name ֵ����LLN0$SG$Enable$setVal��
	* @param char * data д����ֵ
	* @param int timeOut ��ʱʱ�䣬Ĭ��10��
	* @return -true��ʾдֵ�ɹ� -false��ʾдֵʧ��
	*********************************************/ 
	bool WriteValue(char *dom_name,char *var_name,char *data,int timeOut=10);

	/********************************************  
	* @brief �̻���ֵ
	* @author �ο�
	* @date 2016/02/03
	* @param char * dom_name ������PT1101LD0��
	* @param int timeOut ��ʱʱ�䣬Ĭ��10��
	* @return -true��ʾ�̻��ɹ� -false��ʾ�̻�ʧ��
	*********************************************/ 
	bool CureSetting(char *dom_name,int timeOut=10);

	/********************************************  
	* @brief ң�ز���������ѡ�غ�ֱ��
	* @author �ο�
	* @date 2016/02/01
	* @param char * dom_name ������PT1101LD0��
	* @param char * var_name ֵ����CSWI1$CO$Pos��
	* @param int ctrlValue ң��ֵ��0�֣�1�ϣ�
	* @param bool check ң�ؼ��
	* @param int timeOut ��ʱʱ�䣬Ĭ��10��
	* @return -true��ʾң�سɹ� -false��ʾң��ʧ��
	*********************************************/ 
	bool ProcessControl(char *dom_name,char *var_name,int ctrlValue,bool check,int timeOut=10);

	/********************************************  
	* @brief �źŸ���/�豸��λ
	* @author �ο�
	* @date 2016/02/01
	* @param char * dom_name ������PT1101LD0��
	* @param char * var_name ֵ����LLN0$CO$LEDRs��
	* @return -true��ʾ����ɹ� -false��ʾ����ʧ��
	*********************************************/ 
	bool Reset(char *dom_name,char *var_name);

	/********************************************  
	* @brief ��ȡװ��ȷ�ϣ����ڶ��ڲ�������
	* @author �ο�
	* @date 2016/01/26
	* @param int iTimeout_s ��ʱʱ�䣬Ĭ��10��
	* @return -true��ʾ��ȡ�ɹ� -false��ʾ��ȡʧ��
	*********************************************/ 
	bool GetIdentify(int iTimeout_s=10);

	bool InitJournal(char *dom_name,char *var_name,unsigned long &delEntries,int timeOut=10);

	bool GetJournalState(char *dom_name,char *var_name,unsigned long &entries,bool &deletable,int timeOut=10);

	bool ReadJournal(char *dom_name,char *var_name,SDateTime &start,SDateTime &end,unsigned char *entryID,
		SPtrList<stuJournalData> &lstJData,bool &follow,int timeOut=10);

	/********************************************  
	* @brief ����װ����ȫ��LD��������Ŀ��
	* @author �ο�
	* @date 2016/01/05
	* @return -true��ʾ�ɹ� -false��ʾʧ��
	*********************************************/ 
	bool SetIedVarName();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ö��߳�����ģʽ����һ��MMSClient�Ự�������һ���߳�,һ�����̿��Ժ��ж��MMSClient�Ự�����߳���һ������ֻ����һ�����߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-5-26 10:17
	// ����˵��:  b:true���̣߳�false���̣߳�Ĭ�ϵ��߳�
	// �� �� ֵ:  NULl
	//////////////////////////////////////////////////////////////////////////
	void SetMultiThreadMode(bool b);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ͨ�Żظ����ĺ󴥷�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-5-26 10:17
	// ����˵��:  void
	// �� �� ֵ:  NULl
	virtual void onRecvNewPacket(SDateTime dt) {};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ���������Ӧ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-5-26 10:17
	// ����˵��:  void
	// �� �� ֵ:  NULl
	virtual void OnRequestDenial() {};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ������ʱ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-5-26 10:17
	// ����˵��:  void
	// �� �� ֵ:  NULl
	virtual void OnRequestTimeout() {};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ƿ����û��·��������������ڴ��������ٻ���ֵ�ȣ���ִ��ʱ����������ȼ���ʹ�ܱ��潫�ȴ���������Ϻ��ټ�����
	// ��    ��:  Ǯ����
	// ����ʱ��:  2018-11-16 10:17
	// ����˵��:  void
	// �� �� ֵ:  �����û���ִ�еĸ����ȼ����񷵻�true��û���û����񷵻�false
	bool IsUserTaskProcessing();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �ȴ�ϵͳ����ִ����ɡ�������ʹ�ܱ�����ƿ飬��ʱ�����û�������Ϊtrue�����õ�ǰ������ʹ�ܹ��̽�ֹͣ��������Ȩ�����û�
	// ��    ��:  Ǯ����
	// ����ʱ��:  2018-11-16 10:17
	// ����˵��:  timeout �ȴ���ʱ����ʱ�䣬��λ����
	// �� �� ֵ:  ��ϵͳ����ִ����ɷ���true������ʱϵͳ������δִ����ɷ���false
	bool WaitSystemTaskFinish(int timeout = 5000);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ʹ�ܱ���ص���ÿһ����������ʹ�ܳɹ���ʧ�ܶ��ᴥ���ص���������Լ̳�ʵ�ֱ������ٻ��ȹ���
	// ��    ��:  Ǯ����
	// ����ʱ��:  2019-01-16 10:22
	// ����˵��:  rpt ��ǰʹ�ܵı������
	//           result ʹ�ܽ����true�ɹ���falseʧ��
	//           isLastRpt �Ƿ������һ������
	// �� �� ֵ:  void
	virtual void OnEnabledRpt(stuSMmsReportInfo *rpt, bool result, bool isLastRpt) {}

private:

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  MMS�ͻ������߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-9-16 19:31
	// ����˵��:  this
	// �� �� ֵ:  void*
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadMain(void* lp);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȫ��MMS�ͻ������̣߳�����ͨ��ʵ����ʹ��ͬһ��ͨ��ʵ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-5-26 10:17
	// ����˵��:  NULL
	// �� �� ֵ:  NULl
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadGlobalMmsMain(void *lp);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȫ��MMS�ͻ��˶�ʱ�̣߳����������豸�����ӡ�������ȫ�ܱ��桢��ʱ�����Ȳ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-5-26 10:26
	// ����˵��:  NULL
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadGlobalMmsTimer(void *lp);

	/********************************************  
	* @brief ���ٻ�������ƿ�����
	* @author �ο�
	* @date 2015/12/28
	* @param stuSMmsReportInfo * rptInfo ������ƿ�
	* @param int iTimeout_s ��ʱ������
	* @return void
	*********************************************/ 
	void ProcessReportInfoGi(stuSMmsReportInfo *rptInfo,int iTimeout_s);

	void SDateTimeToMmsBtod(SDateTime *time,void *btod);

private:
	char m_sServerIpA[32];//�����A��IP��ַ
	char m_sServerIpB[32];//�����B��IP��ַ
	int m_iPort;//����˶˿ں�
	bool m_bQuit;//�Ƿ�׼���˳�����
	int m_iThreads;//��ǰ���е��߳�����
	void* m_pMVL_NET_INFO_A;//A������ʵ��,MVL_NET_INFO*,NULL��ʾδ����
	void* m_pMVL_NET_INFO_B;//B������ʵ��,MVL_NET_INFO*,NULL��ʾδ����
	void* m_pMVL_NET_INFO_RptEn;//ʹ�ܱ����õ�����ʵ��,MVL_NET_INFO*,NULL��ʾδ����
	void* m_pDIB_ENTRY_A;//A��������DIB��Ŀָ�룬DIB_ENTRY*,NULL��ʾδ����
	void* m_pDIB_ENTRY_B;//B��������DIB��Ŀָ�룬DIB_ENTRY*,NULL��ʾδ����
	void* m_pUserInfo_A;//A���������û���Ϣָ��
	void* m_pUserInfo_B;//B���������û���Ϣָ��
	SLock m_Lock;//�߳���
	bool m_isSystemTaskProcessing;				// �Ƿ���ϵͳ�������ڴ�����ʹ�ܱ���

protected:
	bool m_isUserTaskProcessing;				// �Ƿ����û��������ڴ���
	SPtrList<stuSMmsReportInfo> m_ReportInfos;//��ʹ�ܵı�����Ϣ�б�
	SPtrList<CSMmsDataset> m_Datasets;//���е����ݼ�����
	SPtrList<CSMmsLogicDevice> m_LogicDevices;//�����߼�װ�ö���
};
