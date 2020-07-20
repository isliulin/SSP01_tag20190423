#include "panel_ntp_client.h"

panel_ntp_client::panel_ntp_client(QWidget *parent,int iType)
	: CBaseView(parent)
{
	ui.setupUi(this);
	m_strCurSetServerIp = "";
	m_strCurSetServerPort = "123";
	m_strCurSetClientIp = "182.92.12.11";
	m_strCurSetClientPort = "123";
	m_iNtpType = iType;
	init();
	refreshInfo();

	connect(ui.timeSynPushButton,SIGNAL(clicked()),this,SLOT(OnSlotTimeSyn()));
	connect(ui.startPushButton,SIGNAL(clicked()),this,SLOT(OnSlotStartServer()));
	connect(ui.stopPushButton,SIGNAL(clicked()),this,SLOT(OnSlotStopServer()));
	ui.timeSynPushButton->setToolTip("��Ҫ����ԱȨ�����г���");
}

panel_ntp_client::~panel_ntp_client()
{

}


void panel_ntp_client::init()
{
	QRegExp rx("((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))");
	QValidator *validatorIp=new QRegExpValidator(rx,ui.ipLineEdit);
	ui.ipLineEdit->setValidator(validatorIp);
	ui.ipLineEdit->setToolTip("�������׼����IP��ַ��ʽ");
	

	QValidator *validatorPort=new QIntValidator(0,65535,ui.portLineEdit);
	ui.portLineEdit->setValidator(validatorPort);
	ui.portLineEdit->setToolTip("��Чֵ��Χ0-65535");
}
void panel_ntp_client::refreshInfo()
{
	if (m_iNtpType == NTP_CLIENT)
	{
		ui.label->show();
		ui.ipLineEdit->show();
		ui.startPushButton->hide();
		ui.stopPushButton->hide();
		ui.timeSynPushButton->show();
		ui.ipLineEdit->setText(m_strCurSetClientIp);
		ui.portLineEdit->setText(m_strCurSetClientPort);
	} else
	{
		ui.label->hide();
		ui.ipLineEdit->hide();
		ui.startPushButton->show();
		ui.stopPushButton->show();
		ui.timeSynPushButton->hide();
		ui.ipLineEdit->setText(m_strCurSetServerIp);
		ui.portLineEdit->setText(m_strCurSetServerPort);
		ui.startPushButton->setEnabled(true);
		ui.stopPushButton->setEnabled(false);
	}
}
void panel_ntp_client::OnSlotTimeSyn()
{
	QString strIp = ui.ipLineEdit->text();
	QString strPort = ui.portLineEdit->text();
	if (strIp.isEmpty())
	{
		QMessageBox::warning(this,"����","������IP��ַ����Ϊ�գ�");
		return;
	}
	QRegExp rx("((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))");
	bool bIsMatch = rx.exactMatch(strIp);
	if (!bIsMatch)
	{
		QMessageBox::warning(this,"����","������IP��ַ����ȷ������IP��");
		return;
	}
	if (strPort.isEmpty())
	{
		QMessageBox::warning(this,"����","�������˿ںŲ���Ϊ�գ�");
		return;
	}
	m_strCurSetClientIp = strIp;
	m_strCurSetClientPort = strPort;
	//182.92.12.11 ����ʱ��
	m_ntpClent.SetServer(strIp.toStdString().data(),strPort.toInt());
	SDateTime strNowTime;
	if (m_ntpClent.GetTime(strNowTime))
	{
		SString strTime = strNowTime.toString("yyyy-MM-dd hh:mm:ss");
	} else 
	{
		QMessageBox::warning(this,"����",tr("��ȡ������[%1]ʱ��ʧ�ܣ�").arg(strIp));
		return;
	}

}

void panel_ntp_client::OnSlotStartServer()
{
	QString strIp = ui.ipLineEdit->text();
	QString strPort = ui.portLineEdit->text();
	if (!strIp.isEmpty())
	{
		QRegExp rx("((?:(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(?:25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d))))");
		bool bIsMatch = rx.exactMatch(strIp);
		if (!bIsMatch)
		{
			QMessageBox::warning(this,"����","������IP��ַ����ȷ������IP��");
			return;
		}
	}
	
	if (strPort.isEmpty())
	{
		QMessageBox::warning(this,"����","�������˿ںŲ���Ϊ�գ�");
		return;
	}
	m_strCurSetServerIp = strIp;
	m_strCurSetServerPort = strPort;
	m_ntpServer.SetServer(strIp.toStdString().data(),strPort.toInt());
	if (m_ntpServer.StartServer())
	{
		ui.startPushButton->setEnabled(false);
		ui.stopPushButton->setEnabled(true);
	} else 
	{
		QMessageBox::warning(this,"����",tr("��ȡNTP������ʧ�ܣ�"));
		return;
	}
}

void panel_ntp_client::OnSlotStopServer()
{
	if (m_ntpServer.StopServer())
	{

	} else 
	{
		QMessageBox::warning(this,"����",tr("ֹͣNTP������ʧ�ܣ�"));
		return;
	}
	ui.startPushButton->setEnabled(true);
	ui.stopPushButton->setEnabled(false);
}

void panel_ntp_client::OnRefresh()
{
	refreshInfo();
}