#include "panel_sim_arp.h"

panel_sim_arp::panel_sim_arp(QWidget *parent)
	: CBaseView(parent)
{
	ui.setupUi(this);
	m_pCurTreeItem = NULL;
	m_pOldTreeItem = NULL;
	m_pCurSubnetwork = NULL;
	m_adhandle = NULL;
	m_bSendTreadExit = false;
	m_bRecvTreadExit = false;
	memset(&m_curArpPacket,0,sizeof(m_curArpPacket));
	memset(&m_chCurMac,0,sizeof(m_chCurMac));
	initControl();
	m_addrList.setAutoDelete(true);
	connect(ui.startPushButton,SIGNAL(clicked()),this,SLOT(OnSlotStart()));
	connect(ui.stopPushButton,SIGNAL(clicked()),this,SLOT(OnSlotStop()));
	connect(this,SIGNAL(doExitSendThread()),this,SLOT(OnDoExitSendThread()));
}

panel_sim_arp::~panel_sim_arp()
{
	closeNetCard();
	m_addrList.clear();
}

void panel_sim_arp::OnRefresh()
{

	if (m_pCurTreeItem == NULL)
	{
		return;
	}
	if (m_pCurTreeItem == m_pOldTreeItem)
	{
		return;
	}
	m_bRecvTreadExit = true;
	m_bSendTreadExit = true;
	QString qstr = m_pCurTreeItem->data(0,Qt::UserRole+1).toString();
	m_pCurSubnetwork = (stuSimSubnetwork*)atol(qstr.toStdString().data());
	if (m_pCurSubnetwork == NULL)
	{
		QMessageBox::warning(this,"����","��������ָ��Ϊ��");
		return;
	}
	m_pOldTreeItem = m_pCurTreeItem;
	ui.startPushButton->setEnabled(true);
	ui.stopPushButton->setEnabled(false);
	//unsigned char mac[6]={0};
	//sscanf(m_pCurSubnetwork->mac.data(),"%2x:%2x:%2x:%2x:%2x:%2x",mac,mac+1,mac+2,mac+3,mac+4,mac+5);
	sscanf(m_pCurSubnetwork->mac.data(),"%2x:%2x:%2x:%2x:%2x:%2x",&m_chCurMac[0],&m_chCurMac[1],&m_chCurMac[2],&m_chCurMac[3],&m_chCurMac[4],&m_chCurMac[5]);

}
void panel_sim_arp::initControl()
{
	ui.label_5->hide();
	ui.destIPEndIPlineEdit->hide();
	QRegExp rx("((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))");
	QValidator *validatorIp=new QRegExpValidator(rx,ui.localIPLineEdit);
	ui.localIPLineEdit->setValidator(validatorIp);
	ui.localIPLineEdit->setToolTip("�������׼����IP��ַ��ʽ");
	ui.localIPLineEdit->setText("172.18.162.189");
	m_strLocalIp = "172.18.162.189";

	QValidator *validatorDestStartIp=new QRegExpValidator(rx,ui.destStartIPLineEdit);
	ui.destStartIPLineEdit->setValidator(validatorDestStartIp);
	ui.destStartIPLineEdit->setToolTip("�������׼����IP��ַ��ʽ");
	ui.destStartIPLineEdit->setText("172.18.162.119");
	m_strDestStartIp = "172.18.162.119";

	QValidator *validatorfrequency=new QIntValidator(0,500000,ui.intervalLineEdit);
	ui.intervalLineEdit->setValidator(validatorfrequency);
	ui.intervalLineEdit->setToolTip("��Чֵ��Χ0-500000");
	ui.intervalLineEdit->setText("50");
	m_iIntervalValue = 50;

	QValidator *validatorLen=new QIntValidator(64,1024,ui.lenLineEdit);
	ui.lenLineEdit->setValidator(validatorLen);
	ui.lenLineEdit->setToolTip("��Чֵ��Χ64-1024");
	ui.lenLineEdit->setText("64");
	m_iSendLen = 64;

	QValidator *validatorCount=new QIntValidator(0,1024,ui.sendNumLineEdit);
	ui.sendNumLineEdit->setValidator(validatorCount);
	ui.sendNumLineEdit->setToolTip("��Чֵ��Χ0-1024,0��ʾ����");
	ui.sendNumLineEdit->setText("10");
	m_iSendCount = 10;

	ui.flowLineEdit->setEnabled(false);
	int iFlow = countFlowValue();
	ui.flowLineEdit->setText(QString::number(iFlow));
	ui.stopPushButton->setEnabled(false);
	ui.startPushButton->setEnabled(true);
}

int panel_sim_arp::countFlowValue()
{
	int iFlow = 0;
	if (m_iIntervalValue == 0)
	{
		iFlow = (1000* 1000 * m_iSendLen);
	} else
	{
		iFlow = (1000*1000/m_iIntervalValue)*m_iSendLen;
	}
	return iFlow;
}
void panel_sim_arp::OnSlotStart()
{
	if (!checkControlValid())
	{
		return;
	}
	//��ʼ��Ҫ����arp��������
	if (m_pCurSubnetwork == NULL)
	{
		QMessageBox::warning(this,"����","��������ָ��Ϊ��");
		return;
	}
	//unsigned char mac[6]={0};
	//sscanf(m_pCurSubnetwork->mac.data(),"%2x:%2x:%2x:%2x:%2x:%2x",mac,mac+1,mac+2,mac+3,mac+4,mac+5);
	//sscanf(m_pCurSubnetwork->mac.data(),"%2x:%2x:%2x:%2x:%2x:%2x",&m_chCurMac[0],&m_chCurMac[1],&m_chCurMac[2],&m_chCurMac[3],&m_chCurMac[4],&m_chCurMac[5]);

	unsigned long ulLocalIp = inet_addr(m_strLocalIp.toStdString().data());//�õ�����ip
	unsigned long ulDestIp = inet_addr(m_strDestStartIp.toStdString().data());

	if(!initArpPacket(m_chCurMac,ulLocalIp,ulDestIp))
	{
		QMessageBox::warning(this,"����","��ʼ��arp��ʧ�ܣ�");
		return;
	}

	//if (m_iSendLen > 64)
	{
		memset(m_chSendBuf,0,sizeof(m_chSendBuf));
		memcpy(m_chSendBuf,(char*)&m_curArpPacket,sizeof(m_curArpPacket));
		
	}
	m_bSendTreadExit = false;
	m_bRecvTreadExit = false;
	setButtonState(true);
#ifdef WIN32
	HANDLE h; 
	h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(int)ThreadSendArpRequest, (LPVOID)this, 0, 0); 
	::SetThreadPriority(h,THREAD_PRIORITY_HIGHEST);  
	CloseHandle(h);
#else
	SKT_CREATE_THREAD(ThreadSendArpRequest,this);
#endif
	//SKT_CREATE_THREAD(ThreadSendArpRequest,this);

	
}
void panel_sim_arp::OnSlotStop()
{
	m_bSendTreadExit = true;
	m_bRecvTreadExit = true;
}

bool panel_sim_arp::openNetCard()
{
	if (m_pCurSubnetwork == NULL)
	{
		return false;
	}
	char errbuf[PCAP_ERRBUF_SIZE];
	/* ���豸*/
	if ( (m_adhandle= pcap_open(m_pCurSubnetwork->name.data(),          // �豸��
		65536,            // 65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������
		PCAP_OPENFLAG_PROMISCUOUS,    // ����ģʽ
		1000,             // ��ȡ��ʱʱ��
		NULL,             // Զ�̻�����֤
		errbuf            // ���󻺳��
		) ) == NULL)
	{
		//fprintf(stderr,"\n�޷���ȡ��������. ������%s ����WinPcap֧��\n", d->name);
		/* �ͷ��豸�б�*/  
		qDebug()<<tr("\n�޷���ȡ��������. ������%1 ����WinPcap֧��,������Ϣ��%2\n").arg(m_pCurSubnetwork->name.data()).arg(errbuf);
		m_adhandle = NULL;
		return false;
	}
	return true;
}

bool panel_sim_arp::closeNetCard()
{
	if (m_adhandle)
	{
		pcap_close(m_adhandle);
		m_adhandle = NULL;
	}
	return true;
}

bool panel_sim_arp::initArpPacket(unsigned char* source_mac, 
	unsigned long& srcIP,unsigned long& destIP)
{
	memset(&m_curArpPacket,0,sizeof(m_curArpPacket));
	memset(m_curArpPacket.eth.dest_mac,0xff,6);
	memcpy(m_curArpPacket.eth.source_mac,source_mac,6);
	memcpy(m_curArpPacket.arp.source_mac_add,source_mac,6);
	memset(m_curArpPacket.arp.dest_mac_add,0x00,6);
	m_curArpPacket.eth.eh_type = htons(ETH_ARP);
	m_curArpPacket.arp.hardware_type = htons(ARP_HARDWARE);
	m_curArpPacket.arp.protocol_type = htons(ETH_IP);
	m_curArpPacket.arp.hardware_add_len = 6;
	m_curArpPacket.arp.protocol_add_len = 4;
	m_curArpPacket.arp.source_ip_add = srcIP;//inet_addr(m_pCurSubnetwork->ip.data()); //���󷽵�IP��ַΪ�����IP��ַ
	m_curArpPacket.arp.operation_field = htons(ARP_REQUEST);
	m_curArpPacket.arp.dest_ip_add = destIP;
	//m_curArpPacket.arp.dest_ip_add = inet_addr(m_strDestStartIp.toStdString().data());
	return true;
}

void* panel_sim_arp::ThreadSendArpRequest(void *lp)
{//����ARP����
	panel_sim_arp* pThis = (panel_sim_arp*)lp;
	if (pThis == NULL)
	{
		QMessageBox::warning(pThis,"����","�������Ϊ��");
		return NULL;
	}

	if(!pThis->openNetCard())
	{
		QMessageBox::warning(pThis,"����","�򿪵�ǰ����ʧ�ܣ�");
		return NULL;
	}

	int i = 0;
	while(!pThis->m_bSendTreadExit)//�ж��߳��Ƿ�Ӧ����ֹ
	{
		int iresult = pcap_sendpacket(pThis->m_adhandle, (const u_char *)pThis->m_chSendBuf, pThis->m_iSendLen);//����ARP�����
		if(iresult==-1)
		{ 
			fprintf(stderr,"pcap_sendpacket error.\n"); 
			qDebug() << i << "---pcap_sendpacket error \n";
		} 
		if (pThis->m_iSendCount != 0 && i >=pThis->m_iSendCount)
		{
			break;
		}
		if (pThis->m_iIntervalValue != 0)
		{
			SApi::UsSleep(pThis->m_iIntervalValue);
		}
		i++;
	}
	if (!pThis->m_bSendTreadExit)
	{
		emit pThis->doExitSendThread();
	}
	
	return NULL;
}

void panel_sim_arp::setButtonState(bool bFlag)
{
	if (bFlag)
	{
		ui.stopPushButton->setEnabled(true);
		ui.startPushButton->setEnabled(false);
	} else
	{
		ui.stopPushButton->setEnabled(false);
		ui.startPushButton->setEnabled(true);
	}
	
}
void* panel_sim_arp::ThreadWaitForArpRepeatPacket(void *lp)
{
	panel_sim_arp* pThis = (panel_sim_arp*)lp;
	if (pThis == NULL)
	{
		QMessageBox::warning(pThis,"����","�������Ϊ��");
		return NULL;
	}
	pcap_t* pAdaptHandle;                                                        
	const char* pDevName = pThis->m_pCurSubnetwork->name.data();
	char errbuf[PCAP_ERRBUF_SIZE + 1]; 
	//������������
	if((pAdaptHandle = pcap_open_live(pDevName, 60, 0, 100, errbuf)) == NULL)
	{    
		QMessageBox::warning(pThis,"����","�޷�����������������֮������!");
		return NULL;
	}
	string ipWithMac;
	char* filter = "ether proto\\arp";
	bpf_program fcode;
	int res;
	unsigned short arp_op = 0;
	unsigned char arp_sha [6];
	unsigned long arp_spa = 0;
	struct pcap_pkthdr *header;
	const u_char *pkt_data;
	if (pcap_compile(pAdaptHandle, &fcode, filter, 1, (unsigned long)(0xFFFF0000)) < 0)
	{
		MessageBox(NULL,"���������﷨����!", "wait", MB_OK);
		return NULL;
	}
	//set the filter
	if (pcap_setfilter(pAdaptHandle, &fcode) < 0)
	{
		MessageBox(NULL,"���������������������!", "wait", MB_OK);
		return NULL;
	}
	
#if 1
	while(!pThis->m_bRecvTreadExit)
	{
		if (pThis->m_bRecvTreadExit) 
		{
			break;
		}
		int i = 0;
		ipWithMac = "";
		res = pcap_next_ex(pAdaptHandle, &header, &pkt_data);
		if (!res)
		{
			continue;
		}
		if (*(unsigned short *)(pkt_data+12)==htons(ETH_ARP))
		{
		
			struct arp_packet *recv=(arp_packet *)pkt_data;
		
			if(*(unsigned short *)(pkt_data+20)==htons(ARP_REPLY))
			{
				memcpy(&arp_op, pkt_data + 20, 2);//���Ĳ�������
				memcpy(arp_sha, pkt_data + 22, 6);//ԴMAC��ַ
				memcpy(&arp_spa, pkt_data + 28, 4);//ԴIP��ַ
				in_addr inaddr;
				inaddr.s_addr=arp_spa;
				SString strIP= inet_ntoa(inaddr);
				SString strSourceMac = SString::toFormat("%02x:%02x:%02x:%02x:%02x:%02x",
					arp_sha[0],arp_sha[1],arp_sha[2],arp_sha[3],arp_sha[4],arp_sha[5]);
				//�жϵ�ǰ��arp�ظ��е�Դmac�Ƿ��뱾����ͬ
#if 0
				if(strSourceMac != pThis->m_pCurSubnetwork->mac)
				{
					qDebug()<<"222222222 recvsource"<< ;
					continue;
				}
#endif
#if 0
				//���IP�Ƿ����б����ж�
				if (!pThis->checkIPIsExist(strIP.data()))
				{
					pThis->addRow(strIP.data(),strSourceMac.data());
				}
#endif
			}

		}
	} 
#endif
	return 0;
}

bool panel_sim_arp::getCurNetcardIpList()
{
	pcap_if_t *alldevs; //ȫ�������б� 
	pcap_if_t *d; //һ������ 
	char errbuf[PCAP_ERRBUF_SIZE + 1];
	/* ��ñ��������б� */ 
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) 
	{ 
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf); 
		//exit(1); 
		return false;
	} 

	for(d=alldevs; d; d=d->next) 
	{
		if (strcmp(d->name+8,m_pCurSubnetwork->name.data()) == 0)
		{
			break;
		}
	}
	if (d == NULL)
	{
		pcap_freealldevs(alldevs); 
		return false;
	}
	pcap_addr_t *pAddr; //������ַ 
	unsigned long ip; //IP��ַ 
	unsigned long netmask; //�������� 
	m_addrList.clear();
	for(pAddr=d->addresses; pAddr; pAddr=pAddr->next)
	{ 

		//�õ��û�ѡ���������һ��IP��ַ 
		ip = ((struct sockaddr_in *)pAddr->addr)->sin_addr.s_addr; 
		//�õ���IP��ַ��Ӧ���������� 
		netmask = ((struct sockaddr_in *)(pAddr->netmask))->sin_addr.s_addr; 
		if (!ip || !netmask){ 
			continue; 
		} 
		stAddrListInSelNetCard* pNewSt = new stAddrListInSelNetCard;
		pNewSt->ip = ip;
		pNewSt->netmask = netmask;
		m_addrList.append(pNewSt);
		SString strIP= inet_ntoa(((struct sockaddr_in *)pAddr->addr)->sin_addr);
		SString strNetMask = inet_ntoa(((struct sockaddr_in *)pAddr->netmask)->sin_addr);

#if 0
		unsigned long netsize = ntohl(~netmask); //������������
		unsigned long net = ip & netmask; //������ַ 

		for(unsigned long n=1; n<netsize; n++){ 
			//��į������IP��ַ�������ֽ�˳�� 
			unsigned long destIp = net | htonl(n); 
			//�����ٵ�ARP��������ﵽ����αװ�ɸ�����IP��ַ��Ŀ�� 
			packet = BuildArpPacket(mac,fakeIp,destIp); 
			if(pcap_sendpacket(adhandle, packet, 60)==-1){ 
				fprintf(stderr,"pcap_sendpacket error.\n"); 
			} 
		}
#endif
	}
	pcap_freealldevs(alldevs); 
	return true;
}

void panel_sim_arp::OnSlotSetting()
{
	if (!checkControlValid())
	{
		return;
	}
}

bool panel_sim_arp::checkControlValid()
{
	QString strLocalIP = ui.localIPLineEdit->text();
	if (strLocalIP.isEmpty())
	{
		m_strLocalIp = "127.0.0.1";
	} else
	{
		m_strLocalIp = strLocalIP;
	}

	QString strDestStartIP = ui.destStartIPLineEdit->text();
	if (strDestStartIP.isEmpty())
	{
		QMessageBox::warning(this,"����","Ŀ��IP��ַ����Ϊ�գ������ã�");
		return false;
	}
	QRegExp rx("((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))");
	bool bIsMatch = rx.exactMatch(strDestStartIP);
	if (!bIsMatch)
	{
		QMessageBox::warning(this,"����","Ŀ��IP��ʼ��ַ��ʽ����ȷ�������ã�");
		return false;
	}
	m_strDestStartIp = strDestStartIP;
#if 0
	if (ui.intervalLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this,"����","ʱ��������Ϊ�գ������ã�");
		return false;
	}
	if (ui.lenLineEdit->text().isEmpty())
	{
		QMessageBox::warning(this,"����","��Ϣ���Ȳ���Ϊ�գ������ã�");
		return false;
	}
#endif
	if (ui.intervalLineEdit->text().isEmpty())
	{
		m_iIntervalValue = 50*1000;
	} else 
	{
		m_iIntervalValue = ui.intervalLineEdit->text().toInt() * 1000;
	}
	if (ui.lenLineEdit->text().isEmpty() || ui.lenLineEdit->text().toInt() < 64)
	{
		m_iSendLen = 64;
	} else 
	{
		m_iSendLen = ui.lenLineEdit->text().toInt();
	}
	
	if (ui.sendNumLineEdit->text().isEmpty())
	{
		m_iSendCount = 0;
	} else 
	{
		m_iSendCount = ui.sendNumLineEdit->text().toInt();
	}
	int iFlow = countFlowValue();
	ui.flowLineEdit->setText(QString::number(iFlow));
	return true;
}

void panel_sim_arp::OnDoExitSendThread()
{
	closeNetCard();
	setButtonState(false);
}