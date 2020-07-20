/**
 *
 * �� �� �� : GuiApplication.h
 * �������� : 2015-7-30 15:51
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ͼ�ν���Ӧ�ó�����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30	�ۿ�������ļ�
 *
 **/

#ifndef __TEST_GUI_APPLICATION_H__
#define __TEST_GUI_APPLICATION_H__

#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include "SApplication.h"
#include "GuiFrame.h"
#include "ssp_gui.h"
#include "ssp_database.h"
#include "ssp_funpoint.h"
#include "ssp_datawindow.h"
#include "sp_svglib_config.h"
#include "sim_config.h"
#include "sim_manager.h"

enum eEventLevel
{
	EV_TIP=1,
	EV_WARN,
	EV_ERROR,
	EV_FAULT,
};

enum eEventType
{
	ET_SVOUT=1,
	ET_GSEOUT,
	ET_MMSSVR,
	ET_MMSCLIENT,
	ET_NTPSVR,
	ET_NTPCLIENT,
	ET_TCPSVR,
	ET_TCPCLIENT,
	ET_UDP,
	ET_ICMP,
	ET_ARP,
	ET_STORM,
	ET_SUBNET,
	ET_SYS,
	ET_UNKNOWN,
};

struct stuRealEvent
{
	SString sEventType;
	int soc;
	int usec;
	SString sIedDesc;
	SString sEventLevel;
	SString sEventInfo;
};
void InitConsoleWindow();
class CFloatWnd_RealEvent : public QWidget
{
	Q_OBJECT
public:
	CFloatWnd_RealEvent(QWidget *p=NULL) : QWidget(p)
	{
		setWindowTitle(tr("ʵʱ�¼�����"));
		m_pGridLayout = new QGridLayout(this);
		m_pGridLayout->setSpacing(6);
		m_pGridLayout->setContentsMargins(5, 5, 5, 5);
		m_pGridLayout->setObjectName(QString::fromUtf8("m_pGridLayout"));

		m_pTableWidget = new QTableWidget(p);
		m_pGridLayout->addWidget(m_pTableWidget);
		m_pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


		QStringList labels;
		m_pTableWidget->setColumnCount(5);
		labels << "�¼�����" << "ʱ��" << "IED����" << "�¼�����" << "�¼�����"; 
		m_pTableWidget->setHorizontalHeaderLabels(labels);
		m_pTableWidget->setColumnWidth(0,80);
		m_pTableWidget->setColumnWidth(1,150);
		m_pTableWidget->setColumnWidth(2,200);		;
		m_pTableWidget->setColumnWidth(3,70);
		m_pTableWidget->setColumnWidth(4,300);
		SQt::SetTableWidgetStyle(m_pTableWidget);

		m_pPushButton = new QPushButton(NULL);
		m_pPushButton->setObjectName(QString::fromUtf8("m_pPushButton"));
		QIcon icon;
		icon.addFile(QString::fromUtf8(":/ssp_gui/Resources/MSGBOX04.ICO"), QSize(), QIcon::Normal, QIcon::Off);
		m_pPushButton->setIcon(icon);
		m_pPushButton->setCheckable(true);
		m_pPushButton->setChecked(false);
		m_pPushButton->setFlat(true);
		m_pPushButton->setText(tr("ʵʱ�¼�"));
		SSP_GUI->AddStatusWidget(m_pPushButton);
		m_RealEvents.setAutoDelete(true);
		connect(m_pPushButton,SIGNAL(clicked()),this,SLOT(OnBtnRealEvent()));
		m_pTimer = new QTimer();
		connect(m_pTimer,SIGNAL(timeout()),this,SLOT(OnTimer()));
		m_pTimer->start(100);

	}
	virtual ~CFloatWnd_RealEvent()
	{
		delete m_pTimer;
	}
	virtual void hideEvent ( QHideEvent * event )
	{
		QWidget::hideEvent ( event );
		if(isVisible())
			m_pPushButton->setChecked(true);
		else
			m_pPushButton->setChecked(false);
	}

	public slots:
		void OnTimer()
		{
			stuRealEvent *pEvent;
			while(m_RealEvents.count()>0)
			{
				pEvent = m_RealEvents[0];
				if(m_pTableWidget->rowCount() >= 10000)
					m_pTableWidget->removeRow(0);
				int row = m_pTableWidget->rowCount();
				m_pTableWidget->insertRow(row);
				m_pTableWidget->setItem(row,0,new QTableWidgetItem(pEvent->sEventType.data()));
				SDateTime dt = SDateTime::makeDateTime((time_t)pEvent->soc,pEvent->usec/1000,pEvent->usec%1000);
				m_pTableWidget->setItem(row,1,new QTableWidgetItem(dt.toString("yyyy-MM-dd hh:mm:ss.zzzuuu").data()));
				m_pTableWidget->setItem(row,2,new QTableWidgetItem(pEvent->sIedDesc.data()));
				m_pTableWidget->setItem(row,3,new QTableWidgetItem(pEvent->sEventLevel.data()));
				m_pTableWidget->setItem(row,4,new QTableWidgetItem(pEvent->sEventInfo.data()));
				m_RealEvents.remove(0);
			}
			m_pPushButton->setText(m_pTableWidget->rowCount()==0?"ʵʱ�¼�":SString::toFormat("ʵʱ�¼�(%d)",m_pTableWidget->rowCount()).data());
		}
		void OnBtnRealEvent()
		{
			if(isVisible())
			{
				m_pPushButton->setChecked(false);
				hide();
			}
			else
			{
				m_pPushButton->setChecked(true);
				show();
				setFocus();
			}
		}
public:
	SPtrList<stuRealEvent> m_RealEvents;
private:

	QGridLayout *m_pGridLayout;
	QTableWidget *m_pTableWidget;
	QPushButton *m_pPushButton;
	QTimer *m_pTimer;
};

class CGuiApplication : public SApplication
{
public:
	CGuiApplication(void);
	virtual ~CGuiApplication(void);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡAPPӦ��ʵ��ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 15:59
	// ����˵��:  void
	// �� �� ֵ:  CHmiApplication*
	//////////////////////////////////////////////////////////////////////////
	static CGuiApplication* GetApp();
	static QApplication* GetQApp();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �豸QTӦ����ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 15:59
	// ����˵��:  @pApp
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetQApplication(QApplication *pApp);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������񣬿�������Ӧ�õķ��񣬸ú���������������ʵ�֣�������������ɺ���뷵��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 15:59
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ���񣬹ر�����Ӧ�õķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 15:59
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��ǰģ�������״̬��Ϣ����Ӧ����ģ��Ӧ�г����н����豸��״̬��ϸ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 15:59
	// ����˵��:  void
	// �� �� ֵ:  SString
	//////////////////////////////////////////////////////////////////////////
	virtual SString GetNowModInfo();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������Ϣ����ӿڣ���������ʵ�֣����������뾡����С�����ٷ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-2-3 15:20
	// ����˵��:  @wMsgType��ʾ��Ϣ����
	//         :  @pMsgHeadΪ��Ϣͷ
	//         :  @sHeadStr��Ϣͷ�ַ���
	//         :  @pBufferΪ��ϢЯ���Ķ������������ݣ�NULL��ʾ�޶���������
	//         :  @iLengthΪ���������ݳ���
	// �� �� ֵ:  true��ʾ����ɹ���false��ʾ����ʧ�ܻ�δ����
	//////////////////////////////////////////////////////////////////////////
	virtual bool ProcessAgentMsg(WORD wMsgType,stuSpUnitAgentMsgHead *pMsgHead,SString &sHeadStr,BYTE* pBuffer=NULL,int iLength=0){return true;};

	void NewEvent(SString sEventType,int soc,int usec,SString sIedDesc,SString sEventLevel,SString sEventInfo);

	CSimConfig m_SimConfig;
	CSimManager m_SimManager;
protected:
	QApplication *m_pQApp;//QTӦ��ָ��
	CGuiFrame *m_pMainWnd;//������
	CSsp_FunPointConfig *m_pFunpoint;	//���ܵ������ļ�

	CFloatWnd_RealEvent *m_pWndRealEvent;//ʵʱͻ���¼��Ĵ���ָ��
	CSsp_Database *m_pDatabase;			//���ݿ������ļ�

private:
	//�ر�SCD����ʱ����Ҫ�����ڴ��
	void closeScdToMDB();
};

#endif//__TEST_GUI_APPLICATION_H__
