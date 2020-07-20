#pragma once
#ifdef WIN32
#include <stdio.h> 
#define HAVE_REMOTE
#define WPCAP
#include <pcap.h>
#pragma comment(lib,"wpcap.lib")
#else
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <netinet/ip.h> 
#include <string.h> 
#include <netdb.h> 
#include <netinet/tcp.h> 
#include <netinet/udp.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h> 
#include <net/if.h> 
#include <sys/ioctl.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <linux/if_ether.h> 
#include <net/ethernet.h> 

/* ssl_ifindex��ʾ�������*/
//struct sockaddr_ll 
//{ 
//	u_short sll_family; 
//	u_short sll_protocol; 
//	int sll_ifindex; //�����ֽ��򣬣�3Ϊeth1,5Ϊeth3��
//	u_short sll_hatype; 
//	u_char sll_pkttype; 
//	u_char sll_halen; 
//	u_char sll_addr[8]; 
//};

#endif
//�ֽڶ��������1 
#pragma pack(push)  
#pragma pack(1)
#define IPTOSBUFFERS    12
#define ETH_ARP         0x0806  //��̫��֡���ͱ�ʾ�������ݵ����ͣ�����ARP�����Ӧ����˵�����ֶε�ֵΪx0806
#define ARP_HARDWARE    1  //Ӳ�������ֶ�ֵΪ��ʾ��̫����ַ
#define ETH_IP          0x0800  //Э�������ֶα�ʾҪӳ���Э���ַ����ֵΪx0800��ʾIP��ַ
#define ARP_REQUEST     1
#define ARP_REPLY       2
#define HOSTNUM         255

struct ethernet_head 
{ 
	unsigned char dest_mac[6]; //Ŀ������MAC��ַ 
	unsigned char source_mac[6]; //Դ��MAC��ַ 
	unsigned short eh_type; //��̫������ 
}; 

struct arp_head 
{ 
	unsigned short hardware_type; //Ӳ�����ͣ���̫���ӿ�����Ϊ1 
	unsigned short protocol_type; //Э�����ͣ�IPЭ������Ϊ0X0800 
	unsigned char hardware_add_len; //Ӳ����ַ���ȣ�MAC��ַ����Ϊ6B 
	unsigned char protocol_add_len; //Э���ַ���ȣ�IP��ַ����Ϊ4B 
	unsigned short operation_field; //������ARP����Ϊ1��ARPӦ��Ϊ2 
	unsigned char source_mac_add[6]; //ԴMAC��ַ�����ͷ���MAC��ַ 
	unsigned long source_ip_add; //ԴIP��ַ�����ͷ���IP��ַ 
	unsigned char dest_mac_add[6]; //Ŀ��MAC��ַ��ARP�����и��ֶ�û�����壻ARP��Ӧ��Ϊ���շ���MAC��ַ 
	unsigned long dest_ip_add; //Ŀ��IP��ַ��ARP������Ϊ���������IP��ַ��ARP��Ӧ��Ϊ���շ���IP��ַ
	unsigned char padding[18]; 
}; 

struct arp_packet //����arp���ṹ 
{ 
	ethernet_head eth; //��̫��ͷ�� 
	arp_head arp; //arp���ݰ�ͷ�� 
}; 

#pragma pack()
#pragma pack(pop)  

class CArpManager
{
public:
	CArpManager(void);
	~CArpManager(void);
private:
	bool openNetCard();
private:
	pcap_t *m_adhandle;
};

