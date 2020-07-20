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
#include "BuddyWidgetServer.h"
#include "ssp_uiframe.h"

#ifdef WIN32
#include <windows.h>
#endif


SBuddyWidgetServer::SBuddyWidgetServer()
{
	m_widgets.setAutoDelete(true);
	m_widgets.setShared(true);
}

SBuddyWidgetServer::~SBuddyWidgetServer()
{

}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ��������,�麯�������������б����ȵ��ô˺���
// ��    ��:  �ۿ���
// ����ʱ��:  2018-9-5 8:55
// ����˵��:  void
// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
//////////////////////////////////////////////////////////////////////////
bool SBuddyWidgetServer::Start()
{
	if(!SService::Start())
		return false;
	if(!m_udp.CreateUdp() || !m_udp.Bind(m_iUdpPort))
		return false;
	m_pMsgWidget = new BuddyMsgWidget();
	m_pMsgWidget->setServer(this);
	//m_pMsgWidget->show();
	m_udp.SetTimeout(1000,1000);
	SKT_CREATE_THREAD(ThreadUdp,this);
	LOGERROR("ע��ͨ�ö���:gact.svg.clicked.to_buddy_java");
	LOGERROR("ע��ͨ�ö���:gact.camera.linkage");
	SSP_GUI->RegisterGeneralAction("gact.svg.clicked.to_buddy_java",QAct_SvgClicked);
	SSP_GUI->RegisterGeneralAction("gact.camera.linkage",QAct_CameraLinkage);

	return true;
}

////////////////////////////////////////////////////////////////////////
// ��    ��:  ֹͣ�������������б����ȵ��ô˺���
// ��    ��:  �ۿ���
// ����ʱ��:  2018-9-5 8:55
// ����˵��:  void
// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
//////////////////////////////////////////////////////////////////////////
bool SBuddyWidgetServer::Stop()
{
	if(!SService::Stop())
		return false;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  SVG��������ͨ�ö���
// ��    ��:  �ۿ���
// ����ʱ��:  2018-9-11 20:58
// ����˵��:  void
// �� �� ֵ:  int
//////////////////////////////////////////////////////////////////////////
int SBuddyWidgetServer::QAct_SvgClicked()
{
	SString name,val,str = "act=buddy_qt.svg.clicked;";
	for(int i=0;i<SSP_GUI->GetSessionAttributeCount();i++)
	{
		SSP_GUI->GetIdSessionAttribute(i,name,val);
		str += name+"="+val+";";
	}
	SendMsgToJava(str);
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��Ƶ����ͨ�ö������Ự����:assdev_no/cam_pos/rec_secs/filename
// ��    ��:  �ۿ���
// ����ʱ��:  2018-9-11 20:58
// ����˵��:  void
// �� �� ֵ:  int
//////////////////////////////////////////////////////////////////////////
int SBuddyWidgetServer::QAct_CameraLinkage()
{
	int assdev_no = SSP_GUI->GetSessionAttributeValue("assdev_no").toInt();
	int cam_pos = SSP_GUI->GetSessionAttributeValue("cam_pos").toInt();
	int rec_secs = SSP_GUI->GetSessionAttributeValue("rec_secs").toInt();
	SString filename = SSP_GUI->GetSessionAttributeValue("filename");
	SString msg;
	msg.sprintf("act=camera.linkage;assdev_no=%d;cam_pos=%d;rec_secs=%d;filename=%s;",assdev_no,cam_pos,rec_secs,filename.data());
	SendMsgToJava(msg);
	
	return 0;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  �������ݵ�JAVA
// ��    ��:  �ۿ���
// ����ʱ��:  2018-9-13 16:58
// ����˵��:  @sMsgΪ��Ϣ���� 
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void SBuddyWidgetServer::SendMsgToJava(SString sMsg)
{
	SSocket skt;
	skt.CreateUdp();
	static int sn = 1;
	SString str;
	str.sprintf("oper_sn=%d;",sn++);
	str = sMsg+str;

	skt.SendTo("127.0.0.1",5679,str.data(),str.length());
	skt.SendTo("127.0.0.1",5679,str.data(),str.length());
	skt.SendTo("127.0.0.1",5679,str.data(),str.length());

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  UDP�����߳�
// ��    ��:  �ۿ���
// ����ʱ��:  2018-9-5 9:01
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void* SBuddyWidgetServer::ThreadUdp(void* lp)
{
	SBuddyWidgetServer *pThis = (SBuddyWidgetServer*)lp;
	sockaddr_in sa;
	int ret;
	SString str,last_str;
	char buf[1025];
	pThis->BeginThread();
	SApi::UsSleep(1000000);
	LOGERROR("ע��ͨ�ö���:gact.svg.clicked.to_buddy_java");
	LOGERROR("ע��ͨ�ö���:gact.camera.linkage");
	SSP_GUI->RegisterGeneralAction("gact.svg.clicked.to_buddy_java",QAct_SvgClicked);
	SSP_GUI->RegisterGeneralAction("gact.camera.linkage",QAct_CameraLinkage);

	memset(buf,0,sizeof(buf));
	while(pThis->IsQuit() == false)
	{
		ret = pThis->m_udp.RecvFrom(buf,1024,sa);
		if(ret > 0)
		{
			if(last_str == buf)
			{
				SApi::UsSleep(10000);
				continue;
			}
			SString str = buf;
			//pThis->ProcessMsg(str);
			pThis->m_pMsgWidget->onMsg(buf);
			last_str = str;
			SApi::UsSleep(1000000);
			memset(buf,0,sizeof(buf));
		}
	}
	pThis->EndThread();
	return NULL;
}

extern CSspUiFrame* g_SSpUiFrame;
void SBuddyWidgetServer::ProcessMsg(SString &str)
{
	LOGDEBUG("ProcessMsg:%s",str.data());
	/**
	 * �����µİ��洰��
	 * @param widget_name �������ƣ�������̬���ں͹��ܵ㴰�ڣ�cfgwnd:��������   funwnd:���ܵ���
	 * @param attr ���ٰ���x/y/width/height�ĸ�λ�����ԣ������԰���SSP�Ķ�̬�Ự���ԣ�ֱ�������Դ���ʽ������ֵ��
	 */
	SString act = SString::GetAttributeValue(str,"act");
	if(act == "general.act")
	{
		LOGDEBUG("�յ�ͨ�ö�������! ���� :%s",str.data());
		SString act_name = SString::GetAttributeValue(str,"act_name");
		if(act_name.length() != 0)
		{
			SSP_GUI->SetSessionAttributeValues(str);
			LOGDEBUG("׼��ִ��ͨ�ö���(%s):%s",act_name.data(),str.data());
			int ret = SSP_GUI->RunRegistedCustomAction(act_name);
			LOGDEBUG("ͨ�ö���(%s)ִ�з���:%d",act_name.data(),ret);
			SendMsgToJava(str+SString::toFormat("result=%d;",ret));
			return;
		}
		return;
	}
	if(act.left(7) != "widget.")
	{
		LOGERROR("��Ч����Ϣ����:%s",act.data());
		return;
	}
	SString name = SString::GetAttributeValue(str,"name");
	int hwnd = SString::GetAttributeValueI(str,"hwnd");
	int oid = SString::GetAttributeValueI(str,"oid");
	stuWidgetInfo *pWidget = SearchWidgetInfo(name);
	if(hwnd == NULL)
	{
		LOGERROR("��Ч�İ��洰�ھ��:0x%X",hwnd);
		return;
	}
	if(act != "widget.create" && pWidget == NULL)
	{
		LOGERROR("��Ч�İ��洰������:%s",name.data());
		return;
	}
	if(act == "widget.create" || act == "widget.resize")
	{
		if(act == "widget.create")
		{
			if(pWidget == NULL)
			{
				pWidget = new stuWidgetInfo();
				pWidget->widget_oid = oid;
				pWidget->widget_name = name;
				CFrameMenuL2 menu;
				menu.m_sFunName = name;
				if(name.left(7) == "cfgwnd:")
				{
					//��̬����
					menu.m_iType = 2;
					menu.m_sFunName = name.mid(7);
					menu.m_iRefSn = DB->SelectIntoI(SString::toFormat("select wnd_sn from t_ssp_uicfg_wnd where wnd_name='%s'",menu.m_sFunName.data()));
				}
				else if(name.left(7) == "funwnd:")
				{
					//���ܵ㴰��
					SRecordset rs;
					menu.m_sFunName = name.mid(7);
					DB->Retrieve(SString::toFormat("select type,ref_sn from t_ssp_fun_point where fun_key='%s'",menu.m_sFunName.data()),rs);
					if(rs.GetRows() <= 0)
					{
						LOGERROR("�������洰��ʱʧ�ܣ���Ч�Ĺ��ܵ㴰��:%s",name.data());
						return;
					}
					menu.m_iRefSn = rs.GetValueInt(0,1);
					if(rs.GetValueInt(0,0) == 2)
					{
						//��̬����
						menu.m_iType = 2;
					}
					else
					{
						menu.m_iType = 0;
					}
				}
				else if(name.left(3) == "rptwnd:")
				{
					//������
					menu.m_iType = 3;
					menu.m_sFunName = name.mid(7);
					menu.m_iRefSn = DB->SelectIntoI(SString::toFormat("select report_no from t_ssp_report_info where report_name='%s'",menu.m_sFunName.data()));
				}
				QDesktopWidget* pDesktopWidget = QApplication::desktop();
				//pDesktopWidget->winId();
				/*
				QWindow* native_wnd  = QWindow::fromWinId((WId)hwnd);
				QWidget* pWnd = QWidget::createWindowContainer(native_wnd);

// 				QWidget *pWnd = new QWidget();
// 				pWnd->create((WId)hwnd);
				pWidget->pWidget = pWnd;
				QVBoxLayout *m_verticalLayout_2 = new QVBoxLayout(this);
				m_verticalLayout_2->setSpacing(0);
				m_verticalLayout_2->setContentsMargins(0, 0, 0, 0);
				//m_verticalLayout_2->addWidget(pView);
				*/
				SSP_GUI->SetSessionAttributeValues(str);
#ifdef WIN32
				CBaseView *pView = g_SSpUiFrame->NewView(&menu,m_pMsgWidget);
#else
				CBaseView *pView = g_SSpUiFrame->NewView(&menu,pDesktopWidget);
#endif
				if(pView == NULL)
				{
					LOGERROR("�������洰��ʱʧ�ܣ�");
					delete pWidget;
					return;
				}
				pWidget->pWidget = pView;

				pView->OnPreShow();
				pView->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool| Qt::Dialog);

				pView->resize(400,400);
				pWidget->pWidget->show();
				pView->show();
				pView->OnAftShow();

#ifdef WIN32
				//SetWindowLong((HWND)pWidget->pWidget->winId())
				//pWidget->pWidget->setWindowFlags( Qt::Dialog | Qt::Tool | Qt::WindowStaysOnTopHint);
				SetParent((HWND)pWidget->pWidget->winId(),(HWND)hwnd);
#else
				//X11
#endif
				m_widgets.append(pWidget);
			}
			else
			{
#ifdef WIN32
				SetParent((HWND)pWidget->pWidget->winId(),(HWND)hwnd);
#else//X11
#endif
				((CBaseView*)pWidget->pWidget)->OnPreShow();
				SSP_GUI->SetSessionAttributeValues(str);
				((CBaseView*)pWidget->pWidget)->OnAftShow();
			}
			SString style = SString::GetAttributeValue(str,"stylesheet");
			if(style.length() == 0)
				style = "background-color: rgb(23, 28, 71);color: rgb(255, 255, 255);alternate-background-color: rgb(43, 48, 91);";
			if(style.length() > 0)
			{
				pWidget->pWidget->setStyleSheet(style.data());
			}
		}
 		int x = SString::GetAttributeValueI(str,"x");
 		int y = SString::GetAttributeValueI(str,"y");
               	int width = SString::GetAttributeValueI(str,"width");
                int height = SString::GetAttributeValueI(str,"height");

#ifndef WIN32
		y += 27;
		height -= 61;
#endif
		pWidget->x = x;
		pWidget->y = y;
		pWidget->width = width;
		pWidget->height = height;

#ifdef WIN32
 		MoveWindow((HWND)pWidget->pWidget->winId(),x,y,width,height,TRUE);
 		pWidget->pWidget->show();
		ShowWindow((HWND)pWidget->pWidget->winId(),SW_SHOW);
#else//X11
		pWidget->pWidget->move(x,y);
		pWidget->pWidget->resize(width,height);
		pWidget->pWidget->show();
#endif
	}
	else if(act == "widget.show")
	{
#ifdef WIN32
		MoveWindow((HWND)pWidget->pWidget->winId(),pWidget->x,pWidget->y,pWidget->width,pWidget->height,TRUE);
		pWidget->pWidget->show();
		ShowWindow((HWND)pWidget->pWidget->winId(),SW_SHOW);
#else//X11
		pWidget->pWidget->move(pWidget->x,pWidget->y);
		pWidget->pWidget->resize(pWidget->width,pWidget->height);
		pWidget->pWidget->show();
#endif
		SQt::ProcessUiEvents();
#ifdef WIN32
		ShowWindow((HWND)pWidget->pWidget->winId(),SW_SHOW);
#else//X11
#endif
	}
	else if(act == "widget.hide")
	{
#ifdef WIN32
#else//X11
		pWidget->pWidget->move(pWidget->x,pWidget->y-1);
		pWidget->pWidget->resize(pWidget->width,pWidget->height+1);
		pWidget->pWidget->show();
#endif
		pWidget->pWidget->hide();
#ifdef WIN32
		ShowWindow((HWND)pWidget->pWidget->winId(),SW_HIDE);
#else//X11
#endif
	}
	else if(act == "widget.close")
	{
		pWidget->pWidget->close();
		delete pWidget->pWidget;
		m_widgets.remove(pWidget);
	}
	SQt::ProcessUiEvents();
	SApi::UsSleep(10000);
}

SBuddyWidgetServer::stuWidgetInfo* SBuddyWidgetServer::SearchWidgetInfo(SString name)
{
	unsigned long pos;
	m_widgets.lock();
	stuWidgetInfo *p = m_widgets.FetchFirst(pos);
	while(p)
	{
		if(p->widget_name == name)
		{
			m_widgets.unlock();
			return p;
		}
		p = m_widgets.FetchNext(pos);
	}
	m_widgets.unlock();
	return NULL;
}

BuddyMsgWidget::BuddyMsgWidget(QWidget *parent):QWidget(parent)
{
	connect(this,SIGNAL(sigMsg(QString)),this,SLOT(sltMsg(QString)));
}
BuddyMsgWidget::~BuddyMsgWidget()
{

}

void BuddyMsgWidget::sltMsg(QString str)
{
	SString s = str.toStdString().data();
	m_pServer->ProcessMsg(s);
}


