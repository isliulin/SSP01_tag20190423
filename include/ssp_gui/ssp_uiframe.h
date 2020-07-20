/**
 *
 * �� �� �� : ssp_uiframe.h
 * �������� : 2015-07-08 13:51
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ��������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-07-08	�ۿ�������ļ�
 *
 **/

#ifndef __UIFRAME_H__
#define __UIFRAME_H__

#include <QtGui>
#include "SLog.h"
#include "SList.h"
#include "SString.h"
#include "ssp_baseview.h"
#include "ssp_gui.h"
#include "ssp_gui_inc.h"
#include "BuddyWidgetServer.h"

class CModuleMgr;
class CSspUiFrame;
class CSspPluginMgr;

class SSP_GUI_EXPORT CUiFrameLoading : public QWidget
{
	Q_OBJECT
public:
	CUiFrameLoading(QWidget *parent = 0);
	virtual ~CUiFrameLoading();

	void SetProgress(SString sTip);
	void Stop();
	void postSetTipText()
	{
		emit sigSetTipText();
	}
signals:
	void sigSetTipText();
public slots:
	void OnSetTipText();
public:
	SString m_sTipText;
	SString m_sSysName;
private:
	QLabel *m_label;
	QMovie *m_movie;
	QLabel *m_labelSysName;
	//QLabel *m_labelTipWait;
	QLabel *m_labelTip;
};


class SSP_GUI_EXPORT CUiFrameWaiting : public QWidget
{
	Q_OBJECT
public:
	CUiFrameWaiting(QWidget *parent = 0);
	virtual ~CUiFrameWaiting();
	void ShowWaiting();
	void HideWaiting();
private:
	QLabel *m_label;
	QMovie *m_movie;
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUiFrameTop
// ��    ��:  �ۿ���
// ����ʱ��:  2015-07-08 14:02
// ��    ��:  ��ܶ�������
//////////////////////////////////////////////////////////////////////////
class SSP_GUI_EXPORT CUiFrameTop : public QWidget
{
	Q_OBJECT
public:
	friend class ssp_gui;
	friend class CSspUiFrame;
	CUiFrameTop(QWidget *parent = 0);
	virtual ~CUiFrameTop();

	void paintEvent(QPaintEvent *event);

	virtual void  mouseMoveEvent ( QMouseEvent * event ) ;
	virtual void  mousePressEvent ( QMouseEvent * event ) ;
	virtual void  mouseReleaseEvent ( QMouseEvent * event ) ;
public slots:
	void OnTimerUI();
	void OnLogin();
public:
	SString m_sCurrModDesc;//��ǰ����ģ�������
private:
	QPixmap m_bmpTop,m_bmpTopNoMenu,m_bmpTopFull,m_bmpTopRight,m_bmpButton,m_bmpButtonSel;
	QPen m_penTextMod,m_penLine;
	QFont m_ModFont;
	CSspUiFrame *m_pUiFrame;
	QTimer *m_ptimerUI;
	QVBoxLayout *m_verticalLayout_2;
	QSpacerItem *m_verticalSpacer;
	QWidget *m_pTopWidget;
	QHBoxLayout *m_horizontalLayout;
	QSpacerItem *m_horizontalSpacer;
	QLabel *m_plabLoginIfo;
	SQLabel *m_plabDoLogin;
// 	QLabel *m_plabTime;
// 	QLabel *m_plabTime2;
	int m_iModLeft;
};

class CFrameMenuL1;
class CFrameFunModule;


//////////////////////////////////////////////////////////////////////////
// ��    ��:  CFrameMenuL2
// ��    ��:  �ۿ���
// ����ʱ��:  2015-07-08 14:58
// ��    ��:  �����˵�
//////////////////////////////////////////////////////////////////////////
class SSP_GUI_EXPORT CFrameMenuL2
{
public:
	CFrameMenuL2();
	~CFrameMenuL2();

	SString m_sFunName;//������ͼ���ƣ�����ʵ�����ܴ���ʱʹ��
	SString m_sTitle;//�˵�����
	SString m_sDesc;//�˵�����
	int m_iType;//���ܵ����ͣ�
				// 		0-	��ʾ��̬���ܵ�
				// 		1-	��ʾ��̬�˵����������ӽڵ㣬����Ӧʵ�ʹ��ܣ�
				// 		2-	��ʾ��̬���湦�ܵ�
				// 		3-	��ʾ��̬�����ܵ�
				//		4-	��ʽ���ܵ�(�����ڽ�����ʾ��������ʽ�Ĺ��ܵ�Ȩ��)
				//		5-WEBҳ��(path��ΪURL��URL�п��԰�����̬�Ự����{@SESSION:������@}��ȫ������{@SETUP:������@})
				//		6-JAVA��ά���������ࣨ��path�������������������������ƣ�
	int m_iRefSn;//���ܵ�������ţ�
				// 		type=0/1ʱ��Ч
				// 		type=2ʱ��ӦT_UICFG_WND��wnd_sn
				// 		type=3ʱ��ӦT_REPORT��rpt_sn
	SString m_sPath;//����·�������ã�
	CBaseView *m_pFunView;//��ǰ�˵���Ӧ�Ĺ��ܴ���
	CFrameMenuL1 *m_pL1;//�ϼ��˵�
	unsigned char* img_normal_buff;
	int img_normal_len;
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CFrameMenuL1
// ��    ��:  �ۿ���
// ����ʱ��:  2015-07-08 14:59
// ��    ��:  һ���˵�
//////////////////////////////////////////////////////////////////////////
class SSP_GUI_EXPORT CFrameMenuL1
{
public:
	CFrameMenuL1();
	~CFrameMenuL1();

	SPtrList<CFrameMenuL2> m_SubMenus;
	SString m_sTitle;//�˵�����
	bool m_bOpened;//�����˵��Ƿ��Ѿ�չ��
	CFrameFunModule *m_pMod;//�ϼ�ģ��
	unsigned char* img_normal_buff;
	int img_normal_len;
	SString m_sFunName;
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CFrameFunModule
// ��    ��:  �ۿ���
// ����ʱ��:  2015-07-08 15:13
// ��    ��:  ����ģ�鶨�壬һ������ģ���ж�Ӧ���ɸ�һ���˵�
//////////////////////////////////////////////////////////////////////////
class SSP_GUI_EXPORT CFrameFunModule
{
public:
	CFrameFunModule();
	~CFrameFunModule();
	bool IsExistL1(CFrameMenuL1 *pL1);
	bool IsExistL2(CFrameMenuL2 *pL2);
	void CheckLastMenuL2Valid();
	SPtrList<CFrameMenuL1> m_Menus;
	CFrameMenuL2 *m_pLastMenuL2;//�����ʾ�Ķ����˵�����

	int m_iViewCount;//��ǰ����ģ���µ���ͼ����
	SString m_sTitle;//ģ�����

	unsigned char* img_normal_buff;
	int img_normal_len;
	unsigned char* img_hover_buff;
	int img_hover_len;
	unsigned char* img_down_buff;
	int img_down_len;
	SString m_sFunName;
};


//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUiFrameLeft
// ��    ��:  �ۿ���
// ����ʱ��:  2015-07-08 14:49
// ��    ��:  ��߲˵�����
//////////////////////////////////////////////////////////////////////////
class SSP_GUI_EXPORT CUiFrameLeft : public QWidget
{
	Q_OBJECT
public:
	friend class ssp_gui;
	friend class CSspUiFrame;
	CUiFrameLeft(QWidget *parent = 0);
	virtual ~CUiFrameLeft();

	void paintEvent(QPaintEvent *event);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���Ʋ˵�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-07-08 13:15
	// ����˵��:  @pModuleΪ��ǰģ��ָ��
	//         :  @painterΪ����
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void paintMenu(CFrameFunModule *pModule,QPainter &painter);

	virtual void  mouseMoveEvent ( QMouseEvent * event ) ;
	virtual void  mousePressEvent ( QMouseEvent * event ) ;
	virtual void  mouseReleaseEvent ( QMouseEvent * event ) ;
	virtual void  wheelEvent ( QWheelEvent * event );

private:
	QPixmap m_bmpMenuTop,m_bmpMenuL1Open,m_bmpMenuL1Close,m_bmpMenuBody,m_bmpMenuL2,m_bmpMenuL2Act,m_bmpMenuBottom;
	int m_iOffsetY;//Y�����ƫ������
	int m_iNeedHeight;//Y������Ҫ���ܸ߶�
	CSspUiFrame *m_pUiFrame;
	QPen m_penTextL1,m_penTextL1Sel,m_penTextL2,m_penTextL2Sel;
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUiFrameBottom
// ��    ��:  �ۿ���
// ����ʱ��:  2015-07-08 15:45
// ��    ��:  ��ܵײ�
//////////////////////////////////////////////////////////////////////////
class SSP_GUI_EXPORT CUiFrameBottom : public QFrame
{
	Q_OBJECT
public:
	friend class ssp_gui;
	friend class CSspUiFrame;
	CUiFrameBottom(QWidget *parent = 0);
	virtual ~CUiFrameBottom();

	//void paintEvent(QPaintEvent *event);
	void SetTip(SString &tip);

private:
	QPixmap m_bmpBottom,m_bmpConnStation;
	CSspUiFrame *m_pUiFrame;
	QSpacerItem *horizontalSpacer_2;
	QHBoxLayout *horizontalLayout;
	QLabel *m_labTip/*,*m_labTipRight,*m_labNull*/;
	//QFrame *m_frmPicture;
};


//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUiFrameBody
// ��    ��:  �ۿ���
// ����ʱ��:  2013:9:2 15:25
// ��    ��:  ��ܵײ�
//////////////////////////////////////////////////////////////////////////
class SSP_GUI_EXPORT CUiFrameBody : public QWidget
{
	Q_OBJECT
public:
	friend class ssp_gui;
	friend class CSspUiFrame;
	CUiFrameBody(QWidget *parent = 0);
	virtual ~CUiFrameBody();
	void AddWidget(QWidget *p);
private:
	CSspUiFrame *m_pUiFrame;
	QHBoxLayout *horizontalLayout;
};


//////////////////////////////////////////////////////////////////////////
// ��    ��:  CUiFrame
// ��    ��:  �ۿ���
// ����ʱ��:  2015-07-08 14:02
// ��    ��:  ��ܴ���
//////////////////////////////////////////////////////////////////////////
class SSP_GUI_EXPORT CSspUiFrame : public QWidget
{
	Q_OBJECT

public:
	friend class ssp_gui;
	friend class CUiFrameLeft;
	CSspUiFrame(QWidget *parent = 0);
	virtual ~CSspUiFrame();
	virtual void Start();
	bool WaitForBackStart();
	bool HideStartWnd();
	void SetStartSysName(SString sSysName);
	void SetStartTipText(SString sTipText);
	static CSspUiFrame* GetPtr();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��̨������������������������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-1-19 17:07
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	virtual bool BackStart()
	{
		LOGDEBUG("into CSspUiFrame::BackStart()");
		SApplication::GetPtr()->BackStart();
		return true;
	};

	virtual void resizeEvent(QResizeEvent *e);
	void frame_resize();

	virtual void AddStatusWidget(QWidget *pWidget){m_pWndBottom->horizontalLayout->addWidget(pWidget);};

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ִ��ģ��ĵ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013:9:3 9:23
	// ����˵��:  @pModΪģ��ָ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void OnFunModule(CFrameFunModule *pMod);


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ִ�ж����˵��ĵ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013:9:2 15:00
	// ����˵��:  pMenuL2Ϊ������ָ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void OnFunView(CFrameMenuL2 *pMenuL2);

	void paintall()
	{
		m_pWndLeft->repaint();
		m_pWndTop->repaint();
	}

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �豸״̬����ʾ��Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013:9:10 14:53
	// ����˵��:  @tipΪ��ʾ�ı�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetTip(SString &tip);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ˢ��ȫ����ͼ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013:10:9 14:12
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void RefreshAllView();


	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ��ģ�鼰һ�����˵�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-07-08 13:12
	// ����˵��:  mods�洢ģ������ñ���
	// �� �� ֵ:  true��ʾ�ɹ���false��ʾʧ��
	//////////////////////////////////////////////////////////////////////////
	bool Init(const char* sFunctionPointXml,SPtrList<CFrameFunModule> &mods);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ݹ������ƴ�������ͼ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-07-08 14:56
	// ����˵��:  @pFunMenu��ʾ���ܲ˵�ָ��
	//         :  @parentΪ������ָ��
	// �� �� ֵ:  CBaseView*
	//////////////////////////////////////////////////////////////////////////
	virtual CBaseView* NewView(CFrameMenuL2 *pFunMenu,QWidget *parent);


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ��ģ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-8 16:26
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	virtual void InitModule(const char* sFunctionPointXml);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ�������Ʋ��Ҷ����˵�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-7-30 16:30
	// ����˵��:  @sNameΪ���ܵ�����
	// �� �� ֵ:  CFrameMenuL2*
	//////////////////////////////////////////////////////////////////////////
	CFrameMenuL2* SearchMenul2(SString sName);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ת��ָ�����ܵ�Ľ�����ͼ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-5 14:41
	// ����˵��:  @sFunNameΪ���ܵ����ƣ���ӦCFrameMenuL2������
	// �� �� ֵ:  true��ʾ��ת�ɹ�,false��ʾ��תʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool GotoView(SString sFunName);
	void ShowWaiting(){m_Waiting.ShowWaiting();};
	void HideWaiting(){m_Waiting.HideWaiting();};
	CUiFrameWaiting* GetWaitingWidget(){return &m_Waiting;};
	CUiFrameLoading* GetLoadingWidget(){return &m_Loading;};

	CUiFrameBody* GetBodyWidget(){return m_pWndBody;};
private:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ���߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-11-22 17:05
	// ����˵��:  @lpΪthisָ��
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadLoading(void* lp);
	bool m_bLoading;//�Ƿ����ڼ���
	bool m_bLoadSuccessed;//�Ƿ���سɹ�

	CUiFrameTop *m_pWndTop;
	CUiFrameLeft *m_pWndLeft;
	CUiFrameBottom *m_pWndBottom;
	CUiFrameBody *m_pWndBody;
	CUiFrameLoading m_Loading;
	QTimer m_TimerLoadHide;
	int m_iStartWndAlpha;
	CUiFrameWaiting m_Waiting;
	bool m_bOpenningView;
	SBuddyWidgetServer m_BuddyWidgetServer;
	int m_iStartState;//0:��δ���� 1:��ʼ��̨���� 2����̨�������
public slots:
	void OnTimerLoadHide();
public:
	CFrameFunModule *m_pThisFunModule;//��ǰ����򿪵�ģ��ָ��
	CFrameFunModule *m_pHotFunModule;//��ǰ����ƶ�����ģ��
	SPtrList<CFrameFunModule> m_FunMods;//������ģ��
	CFrameMenuL2 *m_pLastMenuL2;//�����ʾ�Ķ����˵�����
	SPtrList<CFrameMenuL2> m_MenuHistory;//�򿪽������ʷ��¼��ͨ��������ǰ������
//	BaseUpModule *m_pUpModule;//���л���

	SString m_sUserCode;//��ǰ��¼���û���
	time_t m_iLoginTime;//����¼ʱ��
	bool m_bFullScreen;//�Ƿ�ȫ����ʾ��true��ʾȫ����false��ʾ��ȫ��
	SString m_sSysName;
	SString m_sGotoFunPoint;//��ת�õ���ʱ���ܵ�
	CSspPluginMgr *m_pluginMgr;
	unsigned char* img_normal_buff;
	int img_normal_len;
};

#endif // __UIFRAME_H__
