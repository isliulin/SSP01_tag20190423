#include "ArpManager.h"


CArpManager::CArpManager(void)
{
}


CArpManager::~CArpManager(void)
{
}

bool CArpManager::openNetCard()
{
	///* ���豸*/
	//if ( (m_adhandle= pcap_open(d->name,          // �豸��
	//	65536,            // 65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������
	//	PCAP_OPENFLAG_PROMISCUOUS,    // ����ģʽ
	//	1000,             // ��ȡ��ʱʱ��
	//	NULL,             // Զ�̻�����֤
	//	errbuf            // ���󻺳��
	//	) ) == NULL)
	//{
	//	fprintf(stderr,"\n�޷���ȡ��������. ������%s ����WinPcap֧��\n", d->name);
	//	/* �ͷ��豸�б�*/  
	//	pcap_freealldevs(alldevs);
	//	return -1;
	//}
	return true;

}
