/**
 *
 * �� �� �� : SSnmp.h
 * �������� : 2015-12-1 13:15
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SNMP�ͻ��˲����ӿ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-12-1	�ۿ�������ļ�
 *
 **/
#ifndef __SSNMP_CLIENT_H__
#define __SSNMP_CLIENT_H__

#ifdef WIN32
  #pragma comment(lib,"libeay32.lib")
  #pragma comment(lib,"ssleay32.lib")
  #pragma comment(lib,"ws2_32.lib")
  #ifdef  SSNMP_EXPORT_DLL
    #define SSNMP_EXPORT __declspec(dllexport)
  #else //SSNMP_EXPORT_DLL
    #define SSNMP_EXPORT __declspec(dllimport)
    #ifndef NDEBUG
      #pragma comment(lib,"ssnmpd.lib")
    #else
      #pragma comment(lib,"ssnmp.lib")
    #endif
  #endif //SSNMP_EXPORT_DLL
#else
  #define SSNMP_EXPORT
#endif//WIN32

#include "SApi.h"
#include "SDatabase.h"

enum SSnmpVersion
{
	SSNMP_VER1=1,
	SSNMP_VER2C,
	SSNMP_VER3,
};

struct SSNMP_EXPORT stuSnmpSwitchInfo
{
	SString desc;//����
	SString objid;//�����ʶ
	SString up_time;//�ϵ����ʱ��
	SString contact;//��ϵ��ʽ
	SString name;//����
	SString location;//λ��
	int if_number;//�˿�����
};

struct SSNMP_EXPORT stuSSnmpSwitchPortInfo
{
	stuSSnmpSwitchPortInfo()
	{
		port_id = 0;
		port_speed = 0;
		in_real_flow = 0;
		out_real_flow = 0;
		all_real_flow = 0;
		conn_stat = false;
		crc_error = false;
		short_error = false;
		big_error = false;
		storm_error = false;
		tmp_last_crc_error = 0;//
		tmp_last_short_error = 0;//
		tmp_last_big_error = 0;//
		tmp_last_storm_error = 0;//
	}
	int port_id;
	SString port_name;
	int port_speed;//�˿����ʣ�0��ʾδ֪
	float in_real_flow;//�������bps
	float out_real_flow;//��������bps
	float all_real_flow;//������bps
	bool conn_stat;//�Ƿ����������ӣ�true:up,false:down
	bool crc_error;//�Ƿ���CRC����
	bool short_error;//�Ƿ��г���֡����
	bool big_error;//�Ƿ��г���֡����
	bool storm_error;//�Ƿ�������籩

	int sum_in_bytes;//�ۼƽ����ֽ���
	int sum_in_pkgs;//�ۼƽ��հ���
	int sum_in_bcast_pkgs;//�ۼƹ㲥����
	int sum_in_mcast_pkgs;//�ۼƶಥ����
	int sum_in_pkts_64;//64�ֽ��ۼ��ֽ���
	int sum_in_pkts_65_127;//65-127�ֽ��ۼư���
	int sum_in_pkts_128_255;//128-255�ֽ��ۼư���
	int sum_in_pkts_256_511;//256-511�ֽ��ۼư���
	int sum_in_pkts_512_1023;//512-1023�ֽ��ۼư���
	int sum_in_pkts_1024_1518;//1024-1518�ֽ��ۼư���

	int tmp_last_crc_error;//
	int tmp_last_short_error;//
	int tmp_last_big_error;//
	int tmp_last_storm_error;//

};

struct SSNMP_EXPORT stuSSnmpSwitchPortMacInfo
{
	int port_id;
	SString mac;//:�ָ���MAC
	bool is_learned;//�Ƿ�Ϊѧϰ������MAC
	unsigned int ip;//0��ʾ��Ч�����޷���ȡ��ӦIP��
};

//����������
//.iso.org.dod.internet.mgmt.mib-2.system.sysDescr.0
#define OID_sysDescr "1.3.6.1.2.1.1.1"
//�豸��ʶ
//.iso.org.dod.internet.mgmt.mib-2.system.sysObjectID
#define OID_sysObjectID "1.3.6.1.2.1.1.2"
//�������ϵ����ʱ��
//.iso.org.dod.internet.mgmt.mib-2.system.sysUpTime
#define OID_sysUpTime "1.3.6.1.2.1.1.3"
//��ϵ
//.iso.org.dod.internet.mgmt.mib-2.system.sysContact
#define OID_sysContact "1.3.6.1.2.1.1.4"
//����
//.iso.org.dod.internet.mgmt.mib-2.system.sysName
#define OID_sysName "1.3.6.1.2.1.1.5"
//λ��
//.iso.org.dod.internet.mgmt.mib-2.system.sysLocation
#define OID_sysLocation "1.3.6.1.2.1.1.6"

//ȡ����˿�����
//.iso.org.dod.internet.mgmt.mib-2.interfaces.ifNumber
#define OID_ifNumber "1.3.6.1.2.1.2.1.0"

//ȡ����˿�������OID
//.iso.org.dod.internet.mgmt.mib-2.interfaces.ifTable.ifEntry.ifIndex
#define OID_ifIndex "1.3.6.1.2.1.2.2.1.1"

//ȡ����˿��������OID
//.iso.org.dod.internet.mgmt.mib-2.interfaces.ifTable.ifEntry.ifDescr
#define OID_ifDescr "1.3.6.1.2.1.2.2.1.2"

//ȡ����˿��������OID
//.iso.org.dod.internet.mgmt.mib-2.interfaces.ifTable.ifEntry.ifSpeed
#define OID_ifSpeed "1.3.6.1.2.1.2.2.1.5"

//ȡ����˿�����״̬���OID
//.iso.org.dod.internet.mgmt.mib-2.interfaces.ifTable.ifEntry.ifOperStatus
#define OID_ifOperStatus "1.3.6.1.2.1.2.2.1.8"


//�˿�MAC��ַ
//.iso.org.dod.internet.mgmt.mib-2.dot1dBridge.dot1dTp.dot1dTpFdbTable.dot1dTpFdbEntry.dot1dTpFdbAddress
#define OID_MACAddr "1.3.6.1.2.1.17.4.3.1.1"
//�˿�MACַ��˿ںŹ���
//.iso.org.dod.internet.mgmt.mib-2.dot1dBridge.dot1dTp.dot1dTpFdbTable.dot1dTpFdbEntry.dot1dTpFdbPort
#define OID_MACAddrPort "1.3.6.1.2.1.17.4.3.1.2"
//�˿�MAC��Դ��ѧϰor��̬��
//.iso.org.dod.internet.mgmt.mib-2.dot1dBridge.dot1dTp.dot1dTpFdbTable.dot1dTpFdbEntry.dot1dTpFdbStatus
#define OID_MACAddrPortSrc "1.3.6.1.2.1.17.4.3.1.3"

//�˿�CRC�澯����
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsCRCAlignErrors
#define OID_CrcError "1.3.6.1.2.1.16.1.1.1.8"

//�˿ھ�֡����
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsJabbers
#define OID_BigFrameError "1.3.6.1.2.1.16.1.1.1.12"

//�˿ڶ�֡����
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsUndersizePkts
#define OID_ShortFrameError "1.3.6.1.2.1.16.1.1.1.9"

//�˿��ۼƽ����ֽ���
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsOctets
#define OID_etherStatsOctets "1.3.6.1.2.1.16.1.1.1.4"

//�˿��ۼƽ��հ���
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsPkts
#define OID_etherStatsPkts "1.3.6.1.2.1.16.1.1.1.5"

//�㲥�ۼư���
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsBroadcastPkts
#define OID_etherStatsBroadcastPkts "1.3.6.1.2.1.16.1.1.1.6"

//�ಥ�ۼư���
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsMulticastPkts
#define OID_etherStatsMulticastPkts "1.3.6.1.2.1.16.1.1.1.7"

//64�ֽ��ۼư���
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsPkts64Octets
#define OID_etherStatsPkts64Octets "1.3.6.1.2.1.16.1.1.1.14"

//65-127�ֽ��ۼư���
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsPkts65to127Octets
#define OID_etherStatsPkts65to127Octets "1.3.6.1.2.1.16.1.1.1.15"

//128-255�ֽ��ۼư���
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsPkts128to255Octets
#define OID_etherStatsPkts128to255Octets "1.3.6.1.2.1.16.1.1.1.16"

//256-511�ֽ��ۼư���
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsPkts256to511Octets
#define OID_etherStatsPkts256to511Octets "1.3.6.1.2.1.16.1.1.1.17"

//512-1023�ֽ��ۼư���
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsPkts512to1023Octets
#define OID_etherStatsPkts512to1023Octets "1.3.6.1.2.1.16.1.1.1.18"

//1024-1518�ֽ��ۼư���
//.iso.org.dod.internet.mgmt.mib-2.rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsPkts1024to1518Octets
#define OID_etherStatsPkts1024to1518Octets "1.3.6.1.2.1.16.1.1.1.19"

class SSNMP_EXPORT SSnmpClient
{
public:
	SSnmpClient();
	virtual ~SSnmpClient();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȫ�ֳ�ʼ����̬����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-1 13:54
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void GlobalInit();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȫ�ֻ��վ�̬����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-1 13:54
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void GlobalExit();

	void SetVersion(SSnmpVersion ver){m_Version = ver;};
	void SetCommunity(SString sCommunityName);
	void SetRetryTimes(int times){m_iRetryTimes = times;};
	void SetTimeoutMs(int ms){m_iTimeoutMs = ms;};
	void SetServerAddress(SString ip,int port=161);
	SString GetServerIp(){return m_sServerIp;};
	int GetServerPort(){return m_iPort;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����������Ʒ��ض�Ӧ��SNMP����ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-9 15:03
	// ����˵��:  @nameΪ�������ƣ�ȫ��Сд���磺int/octets/ipaddr/oid/timeticks/gauge32/cntr32/cntr64/bits/...
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	int GetSynTaxByName(SString name);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ָ����10����Ϊ��Ԫ��ʱ�������ת��Ϊ���ӵ��ַ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-11 14:09
	// ����˵��:  @hsec_ticks��ʾװ���ϵ��λ��ʼ������ʱ�䣬��0��ʼ
	// �� �� ֵ:  SString
	//////////////////////////////////////////////////////////////////////////
	static SString HSecondsTickets2Str(unsigned long hsec_ticks);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �½��Ự
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-1 14:10
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool BeginSession();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Ự
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-1 14:10
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool EndSession();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  SNMP Get����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-1 14:12
	// ����˵��:  oidsΪ���OID�ַ�����ͨ��','�ָ�
	//         :  @sValues��ʾ���÷��صĽ��ֵ�����Դ���ʽ,oid=value;�����ʱ��';'�ָ�
	//         :  @pRowHexList��ʾ�Ƿ񽫶�Ӧ��ֵ��ȡΪԭʼ��HEX�ַ����б�������oid����һ��
	// �� �� ֵ:  >0��ʾ�ɹ���=0��ʾOID�����ڣ�<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int Get(SString oids,SString &sValues,SValueList<bool> *pRowHexList=NULL);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  SNMP Get���������ص���¼������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-8 10:52
	// ����˵��:  oidsΪ���OID�ַ�����ͨ��','�ָ�
	//         :  @recΪ��¼����
	//         :  @pRowHexList��ʾ�Ƿ񽫶�Ӧ��ֵ��ȡΪԭʼ��HEX�ַ����б�������oid����һ��
	// �� �� ֵ:  >0��ʾ�ɹ���=0��ʾOID�����ڣ�<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int Get(SString oids,SRecord &rec,SValueList<bool> *pRowHexList=NULL);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  SNMP Setд����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-8 15:27
	// ����˵��:  oidsΪ���OID�ַ�����ͨ��','�ָ�
	//         :  @sValues��ʾ���÷��صĽ��ֵ��������ʱ��';'�ָ���������oids��Ӧ
	// �� �� ֵ:  >0��ʾ�ɹ���=0��ʾOID�����ڣ�<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int Set(SString oids,SString sValues);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ݸ�����ֵ����дһ�����ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-9 14:56
	// ����˵��:  @oid��ʾ·��
	//         :  @sValue��ʾֵ����
	//         :  @iValType��ʾֵ����
	// �� �� ֵ:  >0��ʾ�ɹ���=0��ʾOID�����ڣ�<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int SetOneWithType(SString oid,SString sValue,int iValType);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  SNMP GetNext����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-1 14:12
	// ����˵��:  oidΪ����OID�ַ���
	//         :  @sValues��ʾ���÷��صĽ��ֵ�����Դ���ʽ,oid=value;
	//         :  @bRawHexStr��ʾ�Ƿ���ʾ��16���Ƶĸ�ʽ���ַ���
	// �� �� ֵ:  >0��ʾ�ɹ���=0��ʾOID�����ڣ�<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetNext(SString oid,SString &sValues,bool bRawHexStr=false);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  SNMP���ֶ� GetNext����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-8 9:13
	// ����˵��:  oidsΪ���OID�ַ�����ͨ��','�ָ�
	//         :  @reΪ�����¼
	//         :  @pRowHexList��ʾ�Ƿ񽫶�Ӧ��ֵ��ȡΪԭʼ��HEX�ַ����б�������oid����һ��
	// �� �� ֵ:  >0��ʾ�ɹ����صļ�¼������=0��ʾOID�����ڣ�<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetNextMulti(SString oids,SRecord &rec,SValueList<bool> *pRowHexList=NULL);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  SNMP����ָ����OID������OID����ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-2 8:58
	// ����˵��:  @oidΪ����OID�ַ���
	//         :  @sValues��ʾ���÷��صĽ��ֵ�����Դ���ʽ,oid=value;oid2=value2;...
	// �� �� ֵ:  >0��ʾֵ������=0��ʾOID�����ڣ�<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int FetchAll(SString oid,SString &sValues);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  SNMP����ָ����OID��ֵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-2 8:58
	// ����˵��:  @oidΪ����OID�ַ���
	//         :  @sValues��ʾ���÷��صĽ��ֵ�����ֵ����';'�ָ������һ��Ԫ�غ󲻷�';'
	// �� �� ֵ:  >0��ʾֵ������=0��ʾOID�����ڣ�<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int FetchValue(SString oid,SString &sValues);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ݸ����Ĺ㲥IP��ַ��̽�����������е�֧��SNMPЭ����ն�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-11 8:47
	// ����˵��:  @bcast_ipΪ�㲥��ַ
	//         :  @slResultΪ���IP��ַ����,ÿ����ַ�ĸ�ʽΪ:IP��ַ/�˿ں�
	//         :  @ver��ʾ�汾�ţ�V1/V2C/V3
	// �� �� ֵ:  >=0��ʾ���ص��ն�������<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int SnmpDiscover(SString bcast_ip,SStringList &slResult,SSnmpVersion ver);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ȡ�豸������1.3.6.1.2.1.1.1
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-20 20:02
	// ����˵��:  void
	// �� �� ֵ:  SString
	//////////////////////////////////////////////////////////////////////////
	SString GetSwitchDescr();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��������Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-2 10:21
	// ����˵��:  @pInfoΪ��Ϣָ��
	// �� �� ֵ:  >0��ʾ�ɹ���=0��ʾû����Ч�˿ڣ�<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetSwitchInfo(stuSnmpSwitchInfo *pInfo);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�������˿���Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-1 20:44
	// ����˵��:  @pSwitchPortInfo��ʾ���÷��صĽ���б�ָ��
	// �� �� ֵ:  >0��ʾ���ض˿�������=0��ʾû����Ч�˿ڣ�<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetSwitchPortInfo(SPtrList<stuSSnmpSwitchPortInfo>* &pSwitchPortInfo);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�������˿ڶ�Ӧ��MAC��Ϣ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-1 20:47
	// ����˵��:  @SwitchPortMacInfo��ʾ���÷��صĽ���б�
	// �� �� ֵ:  >0��ʾ����MAC������=0��ʾû����ЧMAC��<0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int GetSwitchPortMacInfo(SPtrList<stuSSnmpSwitchPortMacInfo> &SwitchPortMacInfo);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰ�Ự�Ƿ����ӣ������һ�ζ�д�������Ϊ���ݣ��ɹ���ʾ�����ӣ�ʧ�ܱ�ʾ�Ͽ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-13 8:37
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ�����ӣ�false��ʾ�ѶϿ�
	//////////////////////////////////////////////////////////////////////////
	bool IsConnected(){return m_iConnected==1;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����������ϵĻص��麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-20 18:47
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	virtual void OnConnected(){};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������Ͽ����ӵĻص��麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-20 18:47
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	virtual void OnDisconnected(){};
private:
	void SetConnected(bool b);

	SString m_sServerIp;
	int m_iPort;
	SString m_sCommunity;//Ĭ��Ϊpublic
	SSnmpVersion m_Version;
	int m_iRetryTimes;//���Դ���
	int m_iTimeoutMs;//��ʱʱ�䣬��λms
	void *m_pUdpAddress;//UdpAddress*
	void *m_pCommunity;// OctetStr* community
	void *m_pSnmp;//Snmp*
	void *m_pCTarget;//CTarget*
	SPtrList<stuSSnmpSwitchPortInfo> m_SwitchPortInfo;
	int m_portinfo_last_soc;//������ʱ��,��
	int m_portinfo_last_usec;//������ʱ�䣬΢��
	SLock m_lock;
	int m_iConnected;//�Ƿ���������װ�ã� 0��ʾδ���ӣ�1��ʾ�����ӣ�-1��ʾδ֪
};


//////////////////////////////////////////////////////////////////////////
// ��    ��:  Trap�¼��ص�����
// ��    ��:  �ۿ���
// ����ʱ��:  2016-7-11 9:27
// ����˵��:  @pCbParam��ʾ�ص�����
//         :  @hsec_ticks��ʾ��10����Ϊ��Ԫ��ʱ���������װ���ϵ��λ��ʼ��0�ۼ�
//         :  @from��ʾ��Ϣ��Դ�Ŀ����ı�����ʽΪ:IP��ַ/�˿ں�
//         :  @notify_id��ʾ��Ϣ֪ͨ��OID
//         :  @oids��ʾ��ϢЯ����oids�����oidͨ��','�ָ�
//         :  @sValues��ʾ��oids��Ӧ��ֵ�����ͨ��';'�ָ�
// �� �� ֵ:  NULL
/*
Trap:
hseconds_ticks:256976888l  29 days, 17:49:28.88
from:193.110.25.70/161
notify_id:1.3.6.1.6.3.1.1.5.3.2
enterprise_id:
oids:1.3.6.1.2.1.2.2.1.1,1.3.6.1.2.1.2.2.1.7,1.3.6.1.2.1.2.2.1.8
values:5;2;2
TRAP PARSE OK: event_desc=�˿�5�ж�   values=5
*/
//////////////////////////////////////////////////////////////////////////
typedef void (*CB_SSnmp_Trap)(void* pCbParam,unsigned long hsec_ticks,const char *from,const char *notify_id,const char *enterprise_id,SString oids,SString sValues);

//trap �б�׼trap����չtrap֮��
//��ν��׼trap���� notify_idΪ����5�ֵ�trap
//trapֵ  notify_id��OID        ����
#define C_Trap_coldStart				"1.3.6.1.6.3.1.1.5.1"
#define C_Trap_warmStart				"1.3.6.1.6.3.1.1.5.2"
#define C_Trap_linkDown					"1.3.6.1.6.3.1.1.5.3"//�˿����ӶϿ����������׺
#define C_Trap_linkUp					"1.3.6.1.6.3.1.1.5.4"//�˿����ӻָ����������׺
#define C_Trap_LinkUpDownChnVal			"1.3.6.1.2.1.2.2.1.1"//��Ӧ�˿�Up��Down�Ķ˿ںţ��������׺
#define C_Trap_authenticationFailure	"1.3.6.1.6.3.1.1.5.5"
#define C_Trap_egpNeighborloss			"1.3.6.1.6.3.1.1.5.6"

//�������notify_id�������ϵ�5����ô������չtrap
//////////////////////////////////////////////////////////////////////////
// ��    ��:  SSnmpTrapListenner
// ��    ��:  �ۿ���
// ����ʱ��:  2016-7-11 9:46
// ��    ��:  ����SNMP��Trap�¼�֡����
//////////////////////////////////////////////////////////////////////////
class SSNMP_EXPORT SSnmpTrapListenner
{
public:
	SSnmpTrapListenner();
	~SSnmpTrapListenner();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼTrap����������һ������ֻ��ʹ��һ��Trap֡��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-11 9:39
	// ����˵��:  @pCallbackFun��ʾ�ص�����ָ��
	//         :  @pCbParam��ʾ�ص�����ָ��
	// �� �� ֵ:  >0��ʾ�ɹ���<=0��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	int StartTrapListen(CB_SSnmp_Trap pCallbackFun,void* pCbParam,int trap_port=162);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����Trap�¼�֡��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-7-11 9:42
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void StopTrapListen();

public:
	CB_SSnmp_Trap m_pTrapFun;
	void* m_pTrapCbParam;
	void* m_pSnmp;

};

#endif//__SSNMP_CLIENT_H__
