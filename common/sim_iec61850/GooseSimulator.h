/**
 *
 * �� �� �� : GooseSimulator.h
 * �������� : 2016-1-6 9:17
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : GOOSEģ����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2016-1-6	�ۿ�������ļ�
 *
 **/

#if !defined(AFX_GOOSESIMULATOR_H__038486BD_7706_423C_A600_91A3B616B8ED__INCLUDED_)
#define AFX_GOOSESIMULATOR_H__038486BD_7706_423C_A600_91A3B616B8ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SApi.h"
#include "SRawSocket.h"
#include <math.h>
#include "sp_virtual_terminal.h"

//һ�����ĵ���󳤶�
#define C_MAX_GOOSE_FRAME_LEN 1400

#pragma pack(1)
struct stuGoose_Head1
{
	BYTE DstMac[6];
	BYTE SrcMac[6];
	BYTE VLan[2];//0x81 00
	BYTE VLan_Val[2];//0x80 08
	WORD Type;//0x88B8
	WORD AppID;
	WORD AppLen;
	WORD Res1;
	WORD Res2;
	BYTE PDU_Len[4];//61 82 xx xx
// 	BYTE NumberOfAsdu[3];//80 01 01
// 	BYTE SeqOfAsdu[4];//A2 82 xx xx
// 	//BYTE Asdu_Head[4];//30 82 00 AC
// 	BYTE Asdu_Head[4];//30 82 XX xx
// 	BYTE SvId_Empty[2];//80 0? ���SVID�ַ���
};

// struct stuSV92_Head2
// {
// 	BYTE SampCnt[4];//82 02 00 00
// 	BYTE ConfRev[6];//83 04 00 00 00 01
// 	BYTE Sync[3];//85 01 01
// 	//BYTE Data_Head[4];//87 82 00 90
// 	BYTE Data_Head[4];//87 82 xx xx
// };
#pragma pack(4)

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CGooseSimulator
// ��    ��:  �ۿ���
// ����ʱ��:  2016-01-07 09:42
// ��    ��:  GOOSEģ����
//////////////////////////////////////////////////////////////////////////
class CGooseSimulator  
{
public:
	/*
	{   1, "array" },
	{   2, "structure" },
	{   3, "boolean" },
	{   4, "bit-string" },
	{   5, "integer" },
	{   6, "unsigned" },
	{   7, "floating-point" },
	{   9, "octet-string" },
	{  10, "visible-string" },
	{  12, "binary-time" },
	{  13, "bcd" },
	{  14, "booleanArray" },
	{  15, "objId" },
	{  16, "mMSString" },
	{  17, "utc-time" },
	};

	*/
	enum eGooseValType
	{
		GOOSE_CHN_UNKNOWN = 0,
		GOOSE_CHN_BOOL    = 3,
		GOOSE_CHN_INT     = 5,
		GOOSE_CHN_UINT    = 6,
		GOOSE_CHN_UTC     = 17,
		GOOSE_CHN_FLOAT   = 7,
		GOOSE_CHN_BITSTR  = 4,
		GOOSE_CHN_STRING  = 10,
		GOOSE_CHN_Q		  = 12,//?
	};
	class CChannel
	{
	public:
		CChannel();
		~CChannel();
		int m_iOffsetInFrameBuffer;//�ڷ��ͱ��Ļ������е�ƫ��λ��
		eGooseValType m_ValType;
		SString m_sChnName;
		SString m_sValue;
		SString m_sNewValue;
	};

	CGooseSimulator(stuSclVtIedGooseOut *pGooseOut,SString srcmac);
	virtual ~CGooseSimulator();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���һ���µ�ͨ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-01-07 09:42
	// ����˵��:  @eTypeΪ����
	//            @iValueΪ��ǰֵ
	//            @qΪƷ��
	// �� �� ֵ:  ��ͨ��ָ��
	//////////////////////////////////////////////////////////////////////////
	CChannel* AddChannel(SString chnname,eGooseValType vtype,SString sValue);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���¹���������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-01-07 09:42
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void RebuildBuffer();

	void AddNextSendMs(int msec)
	{
		m_iNextSendTimeUSec += msec*1000;
		if(m_iNextSendTimeUSec >= 1000000)
		{
			m_iNextSendTimeSoc += m_iNextSendTimeUSec/1000000;
			m_iNextSendTimeUSec %= 1000000;
		}
	}
protected:


public:
	stuSclVtIedGooseOut *m_pGooseOut;

	BYTE m_SrcMac[6];//ԴMAC��ַ
	BYTE m_DstMac[6];//Ŀ��MAC��ַ
	int m_iVLanId,m_iVLanP;
	WORD m_iAppid;//APPID
	WORD m_iReserver1,m_iReserver2;//����������
	SString m_gocb,m_dataset,m_goID;
	int m_iTTL;
	int m_iMaxSendMsec;
	int m_iEventTimeSoc,m_iEventTimeNSec;
	int m_iEventTimeQ;
	int m_iST;
	int m_iSQ;
	bool m_bTest;
	int m_iConfRev;
	int m_iNdsCom;
	int m_iChns;
	SPtrList<CChannel> m_Chns;//ȫ����ͨ��

	BYTE *m_pBuffer;//���ͻ�������С
	int m_iFrameLen;//һ�����ĵĳ��ȣ���λ�ֽ�
	int m_iEventTimePos;//�¼�ʱ������ƫ��λ��
	int m_iStPos;//STƫ��λ�ã���ʼΪ1
	int m_iSqPos;//SQƫ��λ�ã���ʼΪ1
	int m_iTestPos;//����λƫ��λ��
	int m_iReSendTimes;//�ط���������λʱ��0
	int m_iNextSendTimeSoc,m_iNextSendTimeUSec;//��һ�εķ���ʱ��
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CGooseSimPort
// ��    ��:  �ۿ���
// ����ʱ��:  2016-01-07 09:57
// ��    ��:  MUģ�����˿ڣ�һ���˿ڿ��Է��Ͷ��MU
//////////////////////////////////////////////////////////////////////////
class CGooseSimPort : public SService
{
public:
	CGooseSimPort(SString sDevName);
	~CGooseSimPort();
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������,�麯�������������б����ȵ��ô˺���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-01-07 09:57
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ�������������б����ȵ��ô˺���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-01-07 09:57
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	static void* ThreadSend(void *lp);

	bool CheckGooseOutIsExist(stuSclVtIedGooseOut* pCur);
	SString m_sDevName;//����������
	SRawSocketSend m_Sender;
	SPtrList<CGooseSimulator> m_Gooses;//Goose�б�
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CGooseSimManager
// ��    ��:  �ۿ���
// ����ʱ��:  2016-01-07 10:01
// ��    ��:  GOOSEģ����������
//////////////////////////////////////////////////////////////////////////
class CGooseSimManager
{
public:
	CGooseSimManager();
	virtual ~CGooseSimManager();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ���������Ķ˿�ʵ�������������´���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-01-07 10:01
	// ����˵��:  @sDevNameΪ������
	// �� �� ֵ:  CGooseSimPort*
	//////////////////////////////////////////////////////////////////////////
	CGooseSimPort* SearchPort(SString sDevName);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���һ���µ�GOOSE
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-01-07 10:01
	// ����˵��:  sDevNameΪ������
	//            @pGooseΪ��ʼ�����GOOSEʵ��
	// �� �� ֵ:  CGooseSimulator*
	//////////////////////////////////////////////////////////////////////////
	CGooseSimulator* AddGoose(SString sDevName,CGooseSimulator *pGoose);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣȫ�����ͣ��ͷŶ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-01-07 10:01
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void Clear();

	SPtrList<CGooseSimPort> m_Ports;
};

#endif // !defined(AFX_GOOSESIMULATOR_H__038486BD_7706_423C_A600_91A3B616B8ED__INCLUDED_)
