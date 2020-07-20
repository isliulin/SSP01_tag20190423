/**
 *
 * �� �� �� : BuddyWidgetServer.h
 * �������� : 2018-9-5 8:55
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : QT��ΪJava���洰��ʹ��ʱ��QT��Ӧ�÷����,��������WIN32ƽ̨
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2018-9-5	�ۿ�������ļ�
 *
 **/

#ifndef __SKT_BUDDY_WIDGET_SERVER_H__
#define __SKT_BUDDY_WIDGET_SERVER_H__

#include "SService.h"
#include "SSocket.h"
#include "SString.h"
#include "SList.h"
#include <QtGui/QWidget>
#include "ssp_gui.h"
#include "ssp_gui_inc.h"
class BuddyMsgWidget;

class SSP_GUI_EXPORT SBuddyWidgetServer : public SService
{
public:
	struct stuWidgetInfo
	{
		SString widget_name;
		int widget_oid;
		int x,y,width,height;
		QWidget *pWidget;
	};

	SBuddyWidgetServer();
	~SBuddyWidgetServer();

	inline void setUdpListenPort(int port){m_iUdpPort = port;};


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������,�麯�������������б����ȵ��ô˺���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2018-9-5 8:55
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ�������������б����ȵ��ô˺���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2018-9-5 8:55
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	void ProcessMsg(SString &str);
	stuWidgetInfo* SearchWidgetInfo(SString name);

private:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  UDP�����߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2018-9-5 9:01
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadUdp(void* lp);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  SVG��������ͨ�ö���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2018-9-11 20:58
	// ����˵��:  void
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	static int QAct_SvgClicked();


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��Ƶ����ͨ�ö������Ự����:assdev_no/cam_pos/rec_secs/filename
	// ��    ��:  �ۿ���
	// ����ʱ��:  2018-9-11 20:58
	// ����˵��:  void
	// �� �� ֵ:  int
	//////////////////////////////////////////////////////////////////////////
	static int QAct_CameraLinkage();


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ݵ�JAVA
	// ��    ��:  �ۿ���
	// ����ʱ��:  2018-9-13 16:58
	// ����˵��:  @sMsgΪ��Ϣ���� 
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void SendMsgToJava(SString sMsg);

	int m_iUdpPort;//UDP�˿ںţ�ȱʡΪ5678
	SSocket m_udp;
	SPtrList<stuWidgetInfo> m_widgets;
	BuddyMsgWidget *m_pMsgWidget;//���ڽ�����Ϣ����Ĵ���
};

class SSP_GUI_EXPORT BuddyMsgWidget : public QWidget
{
	Q_OBJECT

public:
	BuddyMsgWidget(QWidget *parent = 0);
	~BuddyMsgWidget();
	void setServer(SBuddyWidgetServer *p){m_pServer = p;};
	void onMsg(QString str)
	{
		emit sigMsg(str);
	}
signals:
	void sigMsg(QString str);

	public slots:
		void sltMsg(QString str);

private:
	SBuddyWidgetServer *m_pServer;
};

#endif//__SKT_BUDDY_WIDGET_SERVER_H__
