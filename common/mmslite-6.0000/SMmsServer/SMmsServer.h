/**
 *
 * �� �� �� : SMmsServer.h
 * �������� : 2016-7-23 10:52
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : MMSͨѶ�������װ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2016-7-23	�ۿ�������ļ�
 *
 **/
#ifndef __SMMS_SERVER_H__
#define __SMMS_SERVER_H__

#include "SInclude.h"
#include "SService.h"
#include <map>

#ifdef _WIN32
  #ifdef  SMMS_SERVER_EXPORT_DLL
    #define SMMS_SERVER_EXPORT __declspec(dllexport)
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
    #pragma comment(lib,"ositcps_l.lib")

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
    #pragma comment(lib,"ositcps_ld.lib")
    #pragma comment(lib,"slog_ld.lib")
    #pragma comment(lib,"smpval_ld.lib")
    #pragma comment(lib,"utility_ld.lib")
    #pragma comment(lib,"ositcps_ld.lib")

    #pragma comment(linker,"/nodefaultlib:libcmtd.lib")
    #pragma comment(linker,"/nodefaultlib:libcmt.lib")
#endif//NDEBUG
  #else //SMMS_SERVER_EXPORT_DLL
    #define SMMS_SERVER_EXPORT __declspec(dllimport)
    #ifdef NDEBUG
      #pragma comment(lib,"SMmsServer.lib")
    #else//NDEBUG

      #pragma comment(lib,"SMmsServerd.lib")
    #endif//NDEBUG
  #endif//SMMS_SERVER_EXPORT_DLL
#else//WIN32
  #define SMMS_SERVER_EXPORT
#endif//WIN32

class SMmsServer;

struct SMMS_SERVER_EXPORT stuMmsServerIed
{
	stuMmsServerIed()
	{
		connected = false;
		scl_info = NULL;
	}
	~stuMmsServerIed()
	{
		if(scl_info != NULL)
		{
			delete scl_info;
			scl_info = NULL;
		}
	}
	static stuMmsServerIed* New(SString scl_file,int ied_no,SString ied_name,SString ap_name)
	{
		stuMmsServerIed *p = new stuMmsServerIed();
		p->scl_pathfile = scl_file;
		p->ied_no = ied_no;
		p->ied_name = ied_name;
		p->ap_name = ap_name;
		return p;
	}
	SString scl_pathfile;//SCL�ļ�����
	int ied_no;
	SString ied_name;
	SString ap_name;//���ʵ����ƣ�ȱʡΪS1���洢�ڽ�������豸��ͨѶ������
	void * /*SCL_INFO*/ scl_info;
	bool connected;//�Ƿ�������
	SPtrList<void* /*DATA_MAP*/> m_maps;//��Ϣ�㵽ģ����Ҷ��DATA_MAP��ӳ��
};

struct SMMS_SERVER_EXPORT stuLeafMap
{
	void* va_ptr;//MVL_VAR_ASSOC*���͵ı����ڴ�ָ�룬ָ��LN��Ӧ����
	void* map_ptr;//DATA_MAP*���͵�ָ�룬ָ��Ҷ�ӽڵ��ӳ���¼
	void* map_head_ptr;//DATA_MAP_HEAD*���͵�ָ�룬ָ��Ҷ�ӽڵ��ӳ��ͷָ���¼
	void* user_ptr;//�û��Զ���ָ��

};

#include "SSaxXml.h"
#include "SList.h"
class SMMS_SERVER_EXPORT CScdCommunication : public SSaxXmlEx
{
public:
	//�ڵ�ṹ����
	struct stuSCL_Communication;
	struct stuSCL_Communication_SubNetwork;
	struct stuSCL_Communication_SubNetwork_ConnectedAP;
	struct stuSCL_Communication
	{
		SConstPtrList<stuSCL_Communication_SubNetwork> m_SubNetwork;
		const char* node_value;
	};
	struct stuSCL_Communication_SubNetwork
	{
		const char* desc;
		const char* type;
		const char* name;
		SConstPtrList<stuSCL_Communication_SubNetwork_ConnectedAP> m_ConnectedAP;
		const char* node_value;
	};
	struct stuSCL_Communication_SubNetwork_ConnectedAP
	{
		const char* iedName;
		const char* apName;
		const char* desc;
		const char* node_value;
	};
	CScdCommunication();
	virtual ~CScdCommunication();
	static stuSaxChoice* CB_SCL(void* cbParam, SSaxAttribute *pAttribute,int iAttributeCount,char* sNodeValue);
	static stuSaxChoice* CB_SCL_Communication(void* cbParam, SSaxAttribute *pAttribute,int iAttributeCount,char* sNodeValue);
	static stuSaxChoice* CB_SCL_Communication_SubNetwork(void* cbParam, SSaxAttribute *pAttribute,int iAttributeCount,char* sNodeValue);
	static stuSaxChoice* CB_SCL_Communication_SubNetwork_ConnectedAP(void* cbParam, SSaxAttribute *pAttribute,int iAttributeCount,char* sNodeValue);
	SConstPtrList<stuSCL_Communication> m_Communication;
private:
	stuSCL_Communication* m_p_SCL_Communication;
	stuSCL_Communication_SubNetwork* m_p_SCL_Communication_SubNetwork;
	stuSCL_Communication_SubNetwork_ConnectedAP* m_p_SCL_Communication_SubNetwork_ConnectedAP;

};

//MMS�������������ͬʱ���ļ�����
#define C_MAX_MMS_SERVER_FILE_OPENED 64
class SMMS_SERVER_EXPORT SMmsServer : public SService
{
public:
	struct SMMS_SERVER_EXPORT stuSMmsFileInfo
	{
		SString filename;	//file name
		int fsize;			//file size (# bytes)
		bool mtimpres;		//last modified time present,mtime��Чʱ��true,һ�㵱��ʾĿ¼ʱΪfalse
		time_t mtime;		//last modified time
	};

	class SMMS_SERVER_EXPORT CMmsMemFile
	{
	public:
		CMmsMemFile()
		{
			m_iReadPos = 0;
			m_pBuffer = NULL;
			m_tOpenTime = m_iTimeoutTimes = 0;
			m_tFileModifyTime = 0;
		}
		~CMmsMemFile()
		{
			if(m_pBuffer != NULL)
				delete[] m_pBuffer;
		}

		BYTE *m_pBuffer;//�ļ�����
		int m_iLen;//�ļ�����
		int m_iReadPos;//��ȡ�ļ���ƫ��λ�ã���0��ʼ
		time_t m_tOpenTime;//��ʱ��
		time_t m_tFileModifyTime;//�ļ��޸�ʱ��
		int m_iTimeoutTimes;//��ʱ����
	};

	SMmsServer();
	virtual ~SMmsServer();
	
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������,�麯�������������б����ȵ��ô˺���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-23 11:11
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ�������������б����ȵ��ô˺���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-23 11:11
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ı��麯�������ط���ʱʹ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-4-13 15:10
	// ����˵��:  @sTipText
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnProgressText(SString sTipText){S_UNUSED(sTipText);};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����IED�ڵ�
	// ��    ��:  Ǯ����
	// ����ʱ��:  2016-7-23 11:11
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�����ɹ�,false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnParseIed(stuMmsServerIed *ied);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ӳ��Ҷ�ӽڵ��û�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-8-30 8:55
	// ����˵��:  @sIedLdName��ʾIED+LD����
	//         :  @sLeafPath��ʾҶ�ӽڵ�·��
	//         :  @ppLeafMap��ʾҶ�ӽڵ���û�ӳ��ָ��ĵ�ַ��ӳ��ָ��Ϊ�գ��û�������Ƿ���Ҫӳ�䣬��Ҫʱ����stuLeafMap������дָ�룬MMS�����ڲ��������ͷ�
	// �� �� ֵ:  true��ʾ�ɹ�ӳ�䣬false��ʾ�޷�ӳ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnMapLeafToUser(char* sIedLdName, char* sLeafPath, stuLeafMap **ppLeafMap)
	{
// 		static int cnt = 0;
// 		cnt ++;
// 		if(cnt % 1000 == 0)
// 			printf("%d \t LEAF MAP : %s    %s\n",cnt,sIedLdName,sLeafPath);
		return false;
	};
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ȡҶ�ӽڵ��ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-8-29 18:33
	// ����˵��:  @sIedLdName��ʾIED+LD����
	//         :  @sLeafPath��ʾҶ�ӽڵ�·��
	//         :  @pLeafMap��ʾҶ�ӽڵ���û�ӳ��ָ��,NULL��ʾδӳ��
	//         :  @mvluRdVaCtrl��ʾ��ȡ��������ָ�룬ʵ������ΪMVLU_RD_VA_CTRL*�� �������Ͳ���Ҫ�û�����������ڴ洢����ʹ��
	//         :  @reason��ʾ����ԭ��0����
	// �� �� ֵ:  true/false, true��ʾ��ȡ�ɹ������ѽ�ֵд��mvluRdVaCtrlָ����ֵ�ռ�,false��ʾʧ�� 
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnReadLeafValue(const char* sIedLdName, const char* sLeafPath, stuLeafMap *pLeafMap, void *mvluRdVaCtrl, int reason = 0){return false;};

// 	//////////////////////////////////////////////////////////////////////////
// 	// ��    ��:  OnReadLeafValue��ʹ�ã��Ա������и�ֵ
// 	// ��    ��:  �ۿ���
// 	// ����ʱ��:  2016-9-1 15:12
// 	// ����˵��:  @mvluRdVaCtrl��ʾ��ȡ��������ָ�룬ʵ������ΪMVLU_RD_VA_CTRL*����OnReadLeafValue��ͬ������һ��
// 	// �� �� ֵ:  true��ʾ��ֵ�ɹ���false��ʾ��ֵʧ��
// 	//////////////////////////////////////////////////////////////////////////
// 	bool FillReadVar(void *mvluRdVaCtrl,char *pValStr);
// 	bool FillReadVar(void *mvluRdVaCtrl,int val);
// 	bool FillReadVar(void *mvluRdVaCtrl,float val);
// 	bool FillReadVar(void *mvluRdVaCtrl,double val);
// 	bool FillReadVar(void *mvluRdVaCtrl,int soc,int usec);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ȡӳ���Ӧ��Ҷ�ӽڵ��ֵ�������ַ�����ʽ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-9-2 9:18
	// ����˵��:  @pLeafMap��ʾӳ��ָ��
	// �� �� ֵ:  SString��ʾֵ����
	//////////////////////////////////////////////////////////////////////////
	SString GetLeafVarValue(stuLeafMap *pLeafMap);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����DATA_MAPָ���ȡ�ڵ��ֵ�������ַ�����ʽ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-4-21 13:44
	// ����˵��:  @pDataMapΪ�ڵ��DATA_MAP*ָ��
	// �� �� ֵ:  SString��ʾֵ����
	//////////////////////////////////////////////////////////////////////////
	SString GetLeafVarValueByDM(void *pDataMap);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ȡָ��·���Ľڵ�ֵ������Ǹ��϶��󣬷��������ӽڵ�������ŵ�Ƕ��ֵ���ӽڵ�˳����ģ����˳��Ϊ׼��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-4-21 8:55
	// ����˵��:  @sMmsPathΪMMSȫ·����CL2223CTRL/GGIO26$ST$Alm2$stVal
	// �� �� ֵ:  SString��ʾֵ����
	//////////////////////////////////////////////////////////////////////////
	SString GetLeafVarValueByPath(SString sMmsPath);
	
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ȡָ��·���Ľڵ�ֵ������Ǹ��϶��󣬷��������ӽڵ�������ŵ�Ƕ��ֵ���ӽڵ�˳����ģ����˳��Ϊ׼��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-4-21 8:57
	// ����˵��:  @sIedLd��ʾIED��+�߼��ڵ㣺CL2223CTRL
	//         :  @sLeafPath��ʾҶ�ӽڵ�����GGIO26$ST$Alm2$stVal
	// �� �� ֵ:  SString��ʾֵ����
	//////////////////////////////////////////////////////////////////////////
	SString GetLeafVarValueByPath(SString sIedLd,SString sLeafPath);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ӳ���Ӧ��Ҷ�ӽڵ��ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-9-1 18:41
	// ����˵��:  @pLeafMap
	// �� �� ֵ:  true��ʾ��ֵ�ɹ���false��ʾ��ֵʧ��
	//////////////////////////////////////////////////////////////////////////
	bool SetLeafVarValue(stuLeafMap *pLeafMap,char *pValStr);
	bool SetLeafVarValue(stuLeafMap *pLeafMap,int val);
	bool SetLeafVarValue(stuLeafMap *pLeafMap,float val);
	bool SetLeafVarValue(stuLeafMap *pLeafMap,double val);
	bool SetLeafVarValue(stuLeafMap *pLeafMap,int soc,int usec);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����DATA_MAPָ���Ӧ��Ҷ�ӽڵ��ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-4-21 13:48
	// ����˵��:  @pDataMapΪDATA_MAP*ָ��
	//         :  @pValStrΪֵ�ַ�������ڵ�ͨ��'{}'��','�ָ�
	// �� �� ֵ:  true��ʾ��ֵ�ɹ���false��ʾ��ֵʧ��
	//////////////////////////////////////////////////////////////////////////
	bool SetLeafVarValueByDM(void* pDataMap,char *pValStr);


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ����·�������ýڵ��ֵ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-4-21 9:08
	// ����˵��:  @sMmsPath��ʾMMS�ڵ�ȫ·��
	//         :  @sVal��ʾֵ���ݣ��������Է�Ҷ�ӽڵ㣬ֵΪ�����żӶ��ŷָ���ֵ����˳��Ӧ���ȡ���ص�˳��һ��
	// �� �� ֵ:  true��ʾ��ֵ�ɹ���false��ʾ��ֵʧ��
	//////////////////////////////////////////////////////////////////////////
	bool SetLeafVarValueByPath(SString sMmsPath, SString sVal);

	bool SetLeafVarValueForDgt(SString sMmsPath, int val, SDateTime dt, SString &sErr);
	bool SetLeafVarValueForAna(SString sMmsPath, float val, SDateTime dt, SString &sErr);
	bool SetLeafVarValueForCtlDgt(SString sMmsPath, int val, SDateTime dt, SString &sErr);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  дһ���µı���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-9-2 9:10
	// ����˵��:  @pLeafMapΪҶ�ӽڵ�ӳ��ָ��
	//         :  @val��ʾֵ
	// �� �� ֵ:  true��ʾ����ڵ㸳ֵ�ɹ���false��ʾ��ֵʧ��
	//////////////////////////////////////////////////////////////////////////
	bool NewReportPush(stuLeafMap *pLeafMap,char *pValStr);
	bool NewReportPush(stuLeafMap *pLeafMap,int val);
	bool NewReportPush(stuLeafMap *pLeafMap,float val);
	bool NewReportPush(stuLeafMap *pLeafMap,double val);
	bool NewReportPush(stuLeafMap *pLeafMap,int soc,int usec);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����µı��棬ͨ��ɨ�豨�棬ʹ����NewReportPush���ĵı��淢��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-9-2 9:10
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void NewReportProc();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  дҶ�ӽڵ��ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-8-29 18:36
	// ����˵��:  @sIedLdName��ʾIED+LD����
	//         :  @sLeafPath��ʾҶ�ӽڵ�·��
	//         :  @pLeafMap��ʾҶ�ӽڵ���û�ӳ��ָ��,NULL��ʾδӳ��
	//         :  @mvluWrVaCtrl��ʾ��ȡ��������ָ�룬ʵ������ΪMVLU_WR_VA_CTRL*�� �������Ͳ���Ҫ�û�����������ڴ洢����ʹ��
	// �� �� ֵ:  true/false, true��ʾд�ɹ���false��ʾдʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnWriteLeafValue(const char* sIedLdName, const char* sLeafPath, stuLeafMap *pLeafMap, void *mvluWrVaCtrl){return true;};


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ������ļ�Ŀ¼����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-09-02 10:04
	// ����˵��:  @sPathΪĿ¼����
	//         :  @slFiles��Ŀ¼����
	//         :  @bNeedToSort��ʾ�Ƿ���Ҫ����true����false������
	// �� �� ֵ:  >0��ʾ����������<0��ʾʧ��, =0��ʾ����Ϊ��
	//////////////////////////////////////////////////////////////////////////
	virtual int OnServerDirectory(SString sPath,SPtrList<stuSMmsFileInfo> &slFiles,bool bNeedToSort=false);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ȡ������ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-09-02 10:10
	// ����˵��:  @sFileName��ʾ������ļ�����
	// �� �� ֵ:  CMmsMemFile*, ��ʾ���ص��ڴ���ļ����ݣ�NULL��ʾ�ļ��򿪻��ȡʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual CMmsMemFile* OnReadServerFile(SString sFileName);


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  д�������ļ��ɹ���Ļص�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-09-02 10:12
	// ����˵��:  @sFileName��ʾ������ļ�����
	//         :  @pBuf��ʾ�ļ����ݻ�����
	//         :  @iLen��ʾ���ݳ��ȣ��ֽڣ�
	// �� �� ֵ:  >0��ʾд��ɹ���=0��ʾд��0��<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual int OnWriteServerFile(SString sFileName/*,unsigned char* pBuf,int iLen*/);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��������ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-09-02 10:12
	// ����˵��:  @sFileName��ʾ������ļ����ƣ�������ļ������·����������Ŀ¼Ӧ������Ŀ¼
	// �� �� ֵ:  >0��ʾɾ���ɹ���=0��ʾ�ļ������ڣ�<0��ʾɾ��ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual int OnDeleteServerFile(SString sFileName);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �޸ķ�����ļ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-9-5 10:30
	// ����˵��:  @sOldFileName��ʾԭ�ļ����ƣ�������ļ������·��
	//         :  @sNewFileName��ʾ���ļ����ƣ�������ļ������·��
	// �� �� ֵ:  >0��ʾ�����ɹ���=0��ʾԭ�ļ�������,=-1��ʾ���ļ��Ѵ��ڣ�����<0��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual int OnRenameServerFile(SString sOldFileName,SString sNewFileName);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����������ȡCFG�ļ�ʱ�������ã�
	// ��    ��:  Ǯ����
	// ����ʱ��:  2016-9-5 10:30
	// ����˵��:  @key����
	//         : @value����ֵ
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnStartupCfgRead(SString key, SString value) {}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����SCD�ļ���δɾ��scl_infoǰ�����DAI�ڵ�
	// ��    ��:  Ǯ����
	// ����ʱ��:  2016-9-5 10:30
	// ����˵��:  @iedName IED����
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnMapDaiToUser(const char *iedName, const char *inst, const char *varName, const char *flattened, const char *val, const char *sAddr) {};

	virtual bool OnReturnSelect(char *ref, char *st, bool ret) { return false; };
	virtual bool OnReturnOper(char *ref, char *st, bool ret) { return false; };
	virtual bool OnReturnCancel(char *ref, char *st, bool ret) { return false; };
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ����Ҷ��ӳ��ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-9-1 14:09
	// ����˵��:  @sIedLdName��ʾIED+LD����
	//         :  @sLeafPath��ʾҶ�ӽڵ�·��
	// �� �� ֵ:  stuLeafMap*�� NULL��ʾδ�ҵ���Ч��ӳ���¼
	//////////////////////////////////////////////////////////////////////////
	stuLeafMap* SearchLeafMap(const char* sIedLdName, const char* sLeafPath);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ��·������ָ����Ҷ��ӳ��ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-4-20 15:19
	// ����˵��:  @sMmsPath��ʾMMS·��
	// �� �� ֵ:  stuLeafMap*�� NULL��ʾδ�ҵ���Ч��ӳ���¼
	//////////////////////////////////////////////////////////////////////////
	stuLeafMap* SearchLeafMap(const char* sMmsPath);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ��·������ָ����Ҷ��ӳ��ָ�룬���ָ��·���Ҳ����������������ӽڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-4-21 8:41
	// ����˵��:  @sMmsPath��ʾMMS·��
	//         :  @leafsΪ���÷��ص�Ҷ��ָ�����ö���,����ΪDATA_MAP*
	// �� �� ֵ:  ��ʾ���ؽڵ���ӽڵ�ָ���������0��ʾ�Ҳ�����<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int SearchLeafMapWithChild(const char* sMmsPath,SPtrList<void> &leafs);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ļ�����ĸ�Ŀ¼
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-8-29 18:39
	// ����˵��:  @sPathΪĿ¼��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetRootPath(SString sPath);

	inline void lock(){m_Lock.lock();};
	inline void unlock(){m_Lock.unlock();};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡӳ��ڵ��Ӧ��IED_LD����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-9-3 9:59
	// ����˵��:  @mapΪӳ��ָ��
	// �� �� ֵ:  char*, ""��ʾ��Чָ��
	//////////////////////////////////////////////////////////////////////////
	char* GetLeafMapIedLdName(stuLeafMap *map);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡӳ��ڵ��Ӧ�Ľڵ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-9-3 9:59
	// ����˵��:  @mapΪӳ��ָ��
	// �� �� ֵ:  char*, ""��ʾ��Чָ��
	//////////////////////////////////////////////////////////////////////////
	char* GetLeafMapLeaf(stuLeafMap *map);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���IED��Ϣ����Ӻ���SMmmsServer�ڲ������ͷţ�Ӧ��Start����֮ǰ�������IED��AP�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-8-29 18:39
	// ����˵��:  @pIed
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AddIed(stuMmsServerIed *pIed);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ��SCD�ļ����IED���ʵ㣬Ӧ��Start����֮ǰ��ɱ���������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-9-1 15:30
	// ����˵��:  @sScdFileΪSCD�ļ�ȫ·�� 
	// �� �� ֵ:  >0��ʾ����SCD�����IED���ʵ�ɹ���=0��ʾSCD�����ɹ�������û����Ч��MMS���ʵ�,<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int AddIedByScd(SString sScdFile);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �豸���Ʋ�����ȫ��ʹ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-4-7 15:36
	// ����˵��:  @bSelectEn��ʾѡ���ȫ��ʹ��
	//         :  @bOperEn��ʾִ�е�ȫ��ʹ��
	//         :  @bCancelEn��ʾȡ����ȫ��ʹ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetGlobalCtrl_En(bool bSelectEn,bool bOperEn,bool bCancelEn);


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��Ҷ�Ӽ��뱨���ݴ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-9-2 9:14
	// ����˵��:  @mapΪ�����Ӧ��Ҷ�ӽڵ�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void PushNewReport(stuLeafMap *map);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���÷���󶨵�IP��ַ��Ĭ��Ϊ0.0.0.0
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-9-2 9:14
	// ����˵��:  @ipΪ��ַ
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetBindIPAddr(SString ip);

private:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  MMS�������ѭ���߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-23 11:11
	// ����˵��:  this
	// �� �� ֵ:  void*
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadMainLoop(void* lp);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ӳ������Ҷ�ӽڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-9-2 9:13
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void MapLeaf();


private:
	int m_iPort;//����˶˿ں�
// 	bool m_bQuit;//�Ƿ�׼���˳�����
// 	int m_iThreads;//��ǰ���е��߳�����
	SLock m_Lock;//�߳���
	SPtrList<stuLeafMap> m_NewReportPush;//
protected:

public:
	SString m_sRootPath;//�ļ�����ĸ��ļ�ϵͳ·��
	SPtrList<stuMmsServerIed> m_Ieds;//׼���򿪵�IED+AP
	SPtrList<stuSMmsFileInfo> m_LastDirResult;//���һ�����ļ�Ŀ¼����
	SString m_sLastDirPath;//���һ�����ļ�Ŀ¼��Ŀ¼����ca_name��Чʱ���ִ����ͣ�ʹ���ϴεĻ���Ľ��
	CMmsMemFile *m_MemFilePtr[C_MAX_MMS_SERVER_FILE_OPENED];
	int m_iMemFileId;//��һ���ļ����0 ~ C_MAX_MMS_SERVER_FILE_OPENED-1
	std::map<std::string, SString> m_mapAddr;
};

#endif//__SMMS_SERVER_H__
