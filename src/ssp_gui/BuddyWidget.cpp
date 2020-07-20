/**
 *
 * �� �� �� : BuddyWidget.cpp
 * �������� : 2018-9-4 19:27
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : �����ⲿ���洰�ڣ�ͨ��HWND���ǿ�ƽٳִ���
 *            Ŀ��Ӧ�ó�������ʱ��ȡ����Ĵ��ھ����ͨ��UDP������Ϣ��������5678�˿ڣ�����Ϊ��hwnd����Ϊ10���ƺ�16���ƣ�16����ʱ��Ҫ0x��ͷ����
 *            String str = "type=buddy_wnd;app_name="+app_name+";app_desc="+app_desc+";hwnd="+hwnd+";";
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2018-9-4	�ۿ�������ļ�
 *
 **/
#include "BuddyWidget.h"

#ifdef WIN32

SSocket *g_pBuddyUdpListener = NULL;
SPtrList<SBuddyWidget> g_BuddyWidgetList;
SBuddyWidget::SBuddyWidget(QWidget *parent)	: QWidget(parent)
{
	g_BuddyWidgetList.append(this);
	if(g_pBuddyUdpListener == NULL)
	{
		g_BuddyWidgetList.setShared(true);
		g_pBuddyUdpListener = new SSocket();
		g_pBuddyUdpListener->CreateUdp();
		g_pBuddyUdpListener->SetTimeout(10,100);
		if(!g_pBuddyUdpListener->Bind(5678))
		{
			LOGERROR("�޷������ڰ��洰��������UDP�˿�[5678]!");
		}
		SKT_CREATE_THREAD(ThreadUdp,0);
	}
}

SBuddyWidget::~SBuddyWidget()
{
	cleanBuddyWnd();
	g_BuddyWidgetList.remove(this);
	if(g_BuddyWidgetList.count() == 0)
	{
		SApi::UsSleep(100000);
		delete g_pBuddyUdpListener;
		g_pBuddyUdpListener = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ð���Ӧ�õ�����
// ��    ��:  �ۿ���
// ����ʱ��:  2018-9-4 20:14
// ����˵��:  @app_nameӦ�ó������ƣ�������Ŀ�������ȫһ��
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBuddyWidget::setBuddyAppName(char *app_name)
{
	m_sAppName = app_name;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ð��洰�ھ��
// ��    ��:  �ۿ���
// ����ʱ��:  2018-9-4 20:15
// ����˵��:  @hwndΪ���
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBuddyWidget::setBuddyWnd(int hwnd)
{
	if(m_iAppHWND == hwnd)
		return;
	if(m_iAppHWND != 0)
		cleanBuddyWnd();
	m_iAppHWND = hwnd;
	SetParent((HWND)m_iAppHWND,(HWND)winId());
	onResize();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ���ڴ�С�ı�ʱ���¼�����ʹ���洰�ڸ��Ÿı�
// ��    ��:  �ۿ���
// ����ʱ��:  2018-9-4 20:17
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBuddyWidget::onResize()
{
	if(m_iAppHWND == 0)
		return;
	RECT r;
	GetClientRect((HWND)winId(),&r);
	MoveWindow((HWND)m_iAppHWND,r.left,r.top,r.right,r.bottom,TRUE);
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ������洰����Ϣ
// ��    ��:  �ۿ���
// ����ʱ��:  2018-9-4 20:17
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBuddyWidget::cleanBuddyWnd()
{
	SetParent((HWND)m_iAppHWND,NULL);
	ShowWindow((HWND)m_iAppHWND,SW_MAXIMIZE);
	m_iAppHWND = 0;
}

void SBuddyWidget::resizeEvent( QResizeEvent * event )
{
	onResize();
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  UDP���������߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2018-9-4 20:18
// ����˵��:  void*
// �� �� ֵ:  void*
//////////////////////////////////////////////////////////////////////////
void* SBuddyWidget::ThreadUdp(void *lp)
{
	char buf[1025];
	int ret;
	sockaddr_in sa;
	memset(buf,0,sizeof(buf));
	SString str;
	SString type,app_name,app_desc,hwnd;
	while(g_BuddyWidgetList.count() > 0)
	{
		ret = g_pBuddyUdpListener->RecvFrom(buf,1024,sa);
		if(ret > 0)
		{
			LOGDEBUG("RECV UDP:%s",buf);
			//String str = "type=buddy_wnd;app_name="+app_name+";app_desc="+app_desc+";hwnd="+hwnd+";";
			str = buf;
			type = SString::GetAttributeValue(str,"type");
			app_name = SString::GetAttributeValue(str,"app_name");
			app_desc = SString::GetAttributeValue(str,"app_desc");
			hwnd = SString::GetAttributeValue(str,"hwnd");
			if(type != "buddy_wnd")
			{
				SApi::UsSleep(10000);
				continue;
			}
			g_BuddyWidgetList.lock();
			unsigned long pos;
			SBuddyWidget *p = g_BuddyWidgetList.FetchFirst(pos);
			while(p)
			{
				if(p->m_sAppName == app_name)
				{
					p->setBuddyWnd(hwnd.toInt());
					break;
				}
				p = g_BuddyWidgetList.FetchNext(pos);
			}
			g_BuddyWidgetList.unlock();

			memset(buf,0,sizeof(buf));
		}
		SApi::UsSleep(10000);
	}
	return 0;
}

// 
// void test_java_inqt::onpushButton()
// {
// 	QString str = ui.lineEdit->displayText();
// 	int hwnd = str.toInt(0,16);
// 	SetParent((HWND)hwnd,(HWND)ui.widget->winId());
// }


#endif