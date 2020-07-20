#include "panel_sim_icmp.h"

panel_sim_icmp::panel_sim_icmp(QWidget *parent)
	: CBaseView(parent)
{
	ui.setupUi(this);

	init();
	connect(ui.sendPushButton,SIGNAL(clicked()),this,SLOT(OnSlotSend()));
	connect(ui.stopPushButton,SIGNAL(clicked()),this,SLOT(OnSlotStop()));
	connect(this,SIGNAL(doExitSendThread()),this,SLOT(OnDoExitSendThread()));
}

panel_sim_icmp::~panel_sim_icmp()
{

}

void panel_sim_icmp::init()
{
	QRegExp rx("((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))");
	QValidator *validatorIp=new QRegExpValidator(rx,ui.IPLineEdit);
	ui.IPLineEdit->setValidator(validatorIp);
	ui.IPLineEdit->setToolTip("�������׼����IP��ַ��ʽ");
	ui.IPLineEdit->setText("172.18.162.119");

	QValidator *validatorinterval=new QIntValidator(0,500000,ui.intervalLineEdit);
	ui.intervalLineEdit->setValidator(validatorinterval);
	ui.intervalLineEdit->setToolTip("��Чֵ��Χ0-5000000");
	ui.intervalLineEdit->setText("50");
	m_iIntervalValue = 50;

	QValidator *validatorLen=new QIntValidator(32,1024,ui.lenLineEdit);
	ui.lenLineEdit->setValidator(validatorLen);
	ui.lenLineEdit->setToolTip("��Чֵ��Χ32-1024");
	ui.lenLineEdit->setText("32");
	m_iSendLen = 32;

	QValidator *validatorCount=new QIntValidator(0,1024,ui.sendNumLineEdit);
	ui.sendNumLineEdit->setValidator(validatorCount);
	ui.sendNumLineEdit->setToolTip("��Чֵ��Χ0-1024,0��ʾ����");
	ui.sendNumLineEdit->setText("10");
	m_iSendCount = 10;

	ui.flowLineEdit->setEnabled(false);
	int iFlow = countFlowValue();
	ui.flowLineEdit->setText(QString::number(iFlow));
	ui.stopPushButton->setEnabled(false);
	ui.sendPushButton->setEnabled(true);
}
int panel_sim_icmp::countFlowValue()
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

bool panel_sim_icmp::checkControlValid()
{
	QString strDestIP = ui.IPLineEdit->text();
	if (strDestIP.isEmpty())
	{
		QMessageBox::warning(this,"����","Ŀ��IP��ַ����Ϊ�գ������ã�");
		return false;
	} 

	QRegExp rx("((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))");
	bool bIsMatch = rx.exactMatch(strDestIP);
	if (!bIsMatch)
	{
		QMessageBox::warning(this,"����","Ŀ��IP��ʼ��ַ��ʽ����ȷ�������ã�");
		return false;
	} else
	{
		m_strDestIp = strDestIP;
	}
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
	if (ui.sendNumLineEdit->text().isEmpty())
	{
		m_iSendCount = 0;
	} else 
	{
		m_iSendCount = ui.sendNumLineEdit->text().toInt();
	}

	if (ui.intervalLineEdit->text().isEmpty())
	{
		m_iIntervalValue = 50*1000;
	} else
	{
		m_iIntervalValue = ui.intervalLineEdit->text().toInt()*1000;
	}

	m_iSendLen = ui.lenLineEdit->text().toInt();
	if (m_iSendLen < 32)
	{
		m_iSendLen = 32;
	}

	int iFlow = countFlowValue();
	ui.flowLineEdit->setText(QString::number(iFlow));
	return true;
}

void panel_sim_icmp::OnSlotSend()
{
	if (!checkControlValid())
	{
		return;
	}
	m_bSendTreadExit = false;
	ui.sendPushButton->setEnabled(false);
	ui.stopPushButton->setEnabled(true);
	SKT_CREATE_THREAD(ThreadSendRequest,this);
	
}

void panel_sim_icmp::OnSlotStop()
{
	m_bSendTreadExit = true;
	ui.sendPushButton->setEnabled(true);
	ui.stopPushButton->setEnabled(false);
}

void* panel_sim_icmp::ThreadSendRequest(void *lp)
{//����ARP����
	panel_sim_icmp* pThis = (panel_sim_icmp*)lp;
	if (pThis == NULL)
	{
		QMessageBox::warning(pThis,"����","�������Ϊ��");
		emit pThis->doExitSendThread();
		return NULL;
	}
	QString threadText = QString::number(quintptr(QThread::currentThreadId()));// QStringLiteral("@0x%1").arg(quintptr(QThread::currentThreadId()), 16, QLatin1Char('0'));
	pThis->m_sIcmp.doInitICMPBase(pThis->m_strDestIp.toStdString().data());
	int i = 0;

	while(!pThis->m_bSendTreadExit)//�ж��߳��Ƿ�Ӧ����ֹ
	{
		bool bResult = pThis->m_sIcmp.doInitICMPInfo(pThis->m_strDestIp.toStdString().data(),threadText.toInt(),pThis->m_iSendLen,i);
		if (!bResult)
		{
			QMessageBox::warning(pThis,"����","��ʼ��socketʧ�ܣ�");
			break;
		}
		if (!pThis->m_sIcmp.doIcmpPingSend())
		{
			break;
		}
		if (pThis->m_iSendCount != 0 && i >= pThis->m_iSendCount)
		{
			break;
		}
		if(pThis->m_iIntervalValue != 0)
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

void panel_sim_icmp::OnDoExitSendThread()
{
	ui.sendPushButton->setEnabled(true);
	ui.stopPushButton->setEnabled(false);
}