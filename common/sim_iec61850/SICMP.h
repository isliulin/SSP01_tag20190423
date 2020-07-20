#pragma once
#include "SSocket.h"
#include "ssp_baseview.h"
#define TIMEOUT 1000

#pragma pack(push)  
#pragma pack(1)

#define TYPE_ICMP_ECHOREPLY	0
#define TYPE_ICMP_ECHOREQ	8
#define TYPE_ICMP_DESTINATION_UNREACHABLE 3
#define TYPE_ICMP_SOURCE_QUENCH 4
#define TYPE_ICMP_REDIRECT 5
#define TYPE_ICMP_ROUTE 10
#define TYPE_ICMP_TIME_EXCEEDED 11
#define TYPE_ICMP_PARAMETER_PROBLEM 12
#define TYPE_ICMP_TIMESTAMP   13
#define TYPE_ICMP_TIMESTAMP_REPLY 14
#define TYPE_ICMP_INFORMATION_REQUEST 15
#define TYPE_ICMP_INFORMATION_REPLY 16
#define TYPE_ICMP_MASK_REQUEST 17
#define TYPE_ICMP_MASK_REPLY 18

#define ICMP_DEF_TIMEOUT    1000
#define ICMP_MAX_PACKET_SIZE 1024
#define ICMP_REQ_DATASIZE 32
#define ICMP_DEF_MAX_HOP 30
//����IP�ײ�
typedef struct _iphdr{
	unsigned char h_lenver; //4 λIP�汾��+4λ�ײ�����
	unsigned char tos; //8λ��������TOS
	unsigned short total_len; //16λIP���ܳ��ȣ��ֽڣ�
	unsigned short ident; //1 6λ��ʶ, ���ڸ���IP���Ĳ�װ
	unsigned short frag_and_flags; //3λ��־λ+13λƫ��λ, Ҳ������IP���Ĳ�װ
	unsigned char ttl; //8λIP������ʱ�� TTL
	unsigned char proto; //8λЭ�� (TCP, UDP ������)
	unsigned short checksum; //16λIP�ײ�У���,�������,�����а�ͷ����д��ȷ��,���㲢�滻.
	unsigned int sourceIP; //32λԴIP��ַ
	unsigned int destIP; //32λĿ��IP��ַ

	//struct	in_addr iaSrc;	// Internet Address - Source
	//struct	in_addr iaDst;	// Internet Address - Destination
}IP_HEADER;  

//����ICMP�ײ�
typedef struct icmp_hdr{
	unsigned char Type; //8λ����
	unsigned char Code; //8λ����
	unsigned short Checksum; //16λУ���, ��TYPE��ʼ,ֱ�����һλ�û�����,���Ϊ�ֽ���Ϊ�����򲹳�һλ
	unsigned short ID; //ʶ��ţ�һ���ý��̺���Ϊʶ��ţ�, ����ƥ��ECHO��ECHO REPLY��
	unsigned short Seq; //�������к�, ���ڱ��ECHO����˳��
	//unsigned int timestamp; //ʱ���
}ICMP_HEADER;

//����ICMP������Ի��ǻ���Ӧ��Echo or Echo Reply Message)
//Type(8)��������Ա���(Echo)��Type(0)�ǻ���Ӧ����(Echo Reply)��
//������Ի����Ӧ�������ڲ�ѯ���ġ�Ping���������ֱ��Ľ��в�ѯ�ͻ�Ӧ��
typedef struct _icmp_echo_quest{
	ICMP_HEADER icmpHdr;
	//unsigned int dwTime; //ʱ���
	//char	cData[ICMP_REQ_DATASIZE];
}ICMP_ECHOQUEST;

// ICMP Echo Reply
typedef struct _icmp_echoreply
{
	IP_HEADER	ipHdr;
	ICMP_ECHOQUEST	echoRequest;
	char    cFiller[256];
}ICMP_ECHO_REPLY,*ICMP_PECHO_REPLY;

//ʱ�����ʱ�������Timestamp or Timeestamp Reply Message)
typedef struct _icmp_timestamp
{
	ICMP_HEADER icmpHdr;
	unsigned int originate_time_stamp; //��ʼʱ���
	unsigned int receive_time_stamp; //����ʱ���
	unsigned int transmit_time_stamp; //����ʱ���
	//char	cData[ICMP_REQ_DATASIZE];
}ICMP_TIMESTAMP;


//ʱ�����ʱ���Ӧ��Timestamp or Timeestamp Reply Message)
typedef struct _icmp_timestamp_reply
{
	IP_HEADER	ipHdr;
	ICMP_TIMESTAMP	timestamp;
	char    cFiller[256];
}ICMP_TIMESTAMP_REPLY;

//��ַ��������Timestamp or Timeestamp Reply Message)
typedef struct _icmp_mask
{
	ICMP_HEADER icmpHdr;
	unsigned int imask; //����
}ICMP_MASK_QUEST;


//��ַ�������󷵻�
typedef struct _icmp_mask_reply
{
	IP_HEADER	ipHdr;
	ICMP_MASK_QUEST	maskquest;
	char    cFiller[256];
}ICMP_MASK_REPLY;
//·��ѯ�ʺͲ�ѯ
typedef struct _icmp_route_reply
{
	IP_HEADER	ipHdr;
	ICMP_HEADER icmp_header;
	unsigned ch[ICMP_REQ_DATASIZE];
}ICMP_ROUTE_Reply;

//������
typedef struct
{
	USHORT usSeqNo;   //�����к�
	DWORD dwRoundTripTime; //����ʱ��
	in_addr dwIPaddr;  //�Զ�IP��ַ
} DECODE_RESULT;

#pragma pack()
#pragma pack(pop)  


unsigned short checksum(unsigned short* buffer,int size);

class SICMP
{
public:
	SICMP(void);
	~SICMP(void);

	bool initSocket();
	void setIpAndPort(SString strIP, SString strPort)
	{
		m_strDestIp = strIP;
		m_strDestPort = strPort;
	}
	
	int doPing(char *sIp,int &ms,int &ttl);
	int testPing(char *sIp,int &ms,int &ttl);
	u_short in_cksum(u_short *addr, int len);
	int doTimestamp(char *sIp,int &ms,int &ttl);
	int doMask(char *sIp,int &ms,int &ttl);
	int doroute(char *sIp,int &ms,int &ttl);
	int doTraceRoute(char* sIP,int &ms,int &ttl);
	bool DecodeIcmpResponse(char* pBuf, int iPacketSize, DECODE_RESULT& stDecodeResult);
	bool DecodeIcmpEchoResponse(ICMP_ECHO_REPLY* pReply, int iPacketSize, DECODE_RESULT& stDecodeResult);
	bool DecodeIcmpTimestampResponse(ICMP_TIMESTAMP_REPLY* pReply, int iPacketSize, DECODE_RESULT& stDecodeResult);
	bool DecodeIcmpMaskResponse(ICMP_MASK_REPLY* pReply, int iPacketSize, DECODE_RESULT& stDecodeResult);
	bool DecodeIcmpRouteResponse(ICMP_ROUTE_Reply* pReply, int iPacketSize, DECODE_RESULT& stDecodeResult);
	BOOL DecodeIcmpTraceRouteResponse(char* pBuf, int iPacketSize, DECODE_RESULT& stDecodeResult);

	bool doIcmpPingSend();
	bool doInitICMPBase(SString strDestIp);
	bool doInitICMPInfo(SString strDestIp,int iThreadId,int iLen,int iSeq);
private:
	bool CreateSocket(int af, int type, int protocol);
	int CloseSocket();
private:
	SSocket m_socket;
	sockaddr_in m_sendDest;
	char  m_chSendBuf[1024];
	int   m_iSendLen;
	//SOCKET  m_socket;
	SString m_strDestIp;
	SString m_strDestPort;
	SOCKET m_hSocket;
	int m_iAF;
	bool m_bTcp;
	int m_iSocketType;
	int m_iLastError;

	char m_recvBuf[ICMP_MAX_PACKET_SIZE];
	static int nId;
	static int nSeq;
public:
	QString m_strResultInfo;
};

