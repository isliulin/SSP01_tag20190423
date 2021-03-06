/**
 *
 * 文 件 名 : BridgeSvr.h
 * 创建日期 : 2018-10-12 16:37
 * 作    者 : 邵凯田(skt001@163.com)
 * 修改日期 : $Date: $
 * 当前版本 : $Revision: $
 * 功能描述 : 桥接服务程序
 * 修改记录 : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2018-10-12	邵凯田　创建文件
 *
 **/

#ifndef __SSP_BRIDGE_SERVER_H__
#define __SSP_BRIDGE_SERVER_H__

#include "SService.h"
#include "SApi.h"
#include "SSocket.h"
#include "SString.h"
#include "STcpServerBase.h"
#include "../public/bridge_inc.h"

class CSession : public SService
{
public:
	enum eChannelType
	{
		CHN_NONE=0,
		CHN_TCP_SERVER=1,
	};
	struct stuTcpSvrParam
	{
		CSession *pThis;
		SSocket *pSessionSock;
		SString session_id;
	};
	CSession(STcpServerSession *pServerSession);
	~CSession();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  启动服务,虚函数，派生函数中必须先调用此函数
	// 作    者:  邵凯田
	// 创建时间:  2018-10-12 18:38
	// 参数说明:  void
	// 返 回 值:  true表示服务启动成功,false表示服务启动失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  停止服务，派生函数中必须先调用此函数
	// 作    者:  邵凯田
	// 创建时间:  2018-10-12 18:38
	// 参数说明:  void
	// 返 回 值:  true表示服务停止成功,false表示服务停止失败
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  创建TCP服务通道
	// 作    者:  邵凯田
	// 创建时间:  2018-10-12 18:38
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	bool CreateTcpSvr(SString &sExtAttr);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  处理从客户端接收到的数据包
	// 作    者:  邵凯田
	// 创建时间:  2018-10-12 20:51
	// 参数说明:  @pPaakage
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	void ProcessClientPkg(stuSTcpPackage *pPackage);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  TCP转发服务通道侦听线程
	// 作    者:  邵凯田
	// 创建时间:  2018-10-12 19:05
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadTcpSvrListen(void *lp);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  TCP转发服务通道收发线程
	// 作    者:  邵凯田
	// 创建时间:  2018-10-12 19:05
	// 参数说明:  
	// 返 回 值:  
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadTcpSvrSession(void *lp);

	STcpServerSession *m_pSession;
	eChannelType m_ChannelType;
	SSocket m_ListenSock;
	SPtrList<stuTcpSvrParam> m_TcpSvrParams;
};

class CBridgeSvr : public STcpServerBase
{
public:

	CBridgeSvr();
	virtual ~CBridgeSvr();

	////////////////////////////////////////////////////////////////////////
	// 描    述:  登录回调虚函数
	// 作    者:  邵凯田
	// 创建时间:  2018-10-12 16:37
	// 参数说明:  @ip登录客户端的IP
	//            @port登录客户端的端口
	//            @sLoginHead登录字符串
	// 返 回 值:  true表示允许登录,false表示拒绝登录
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnLogin(SString ip,int port,SString &sLoginHead);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  新会话回调虚函数
	// 作    者:  邵凯田
	// 创建时间:  2018-10-12 16:37
	// 参数说明:  @pSession为新创建的会话实例
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnNewSession(STcpServerSession *pSession);

	//////////////////////////////////////////////////////////////////////////
	// 描    述:  删除会话的回调虚函数
	// 作    者:  邵凯田
	// 创建时间:  2018-10-12 16:37
	// 参数说明:  @pSession为即将将删除的会话实例
	// 返 回 值:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnDeleteSession(STcpServerSession *pSession);

	////////////////////////////////////////////////////////////////////////
	// 描    述:  会话线程接收到报文后的回调虚函数，派生类通过此函数处理即时消息
	// 作    者:  邵凯田
	// 创建时间:  2018-10-12 16:37
	// 参数说明:  @pSession为会话实例指针，可以通过指针向对端发送数据
	//            @pPackage为刚接收到的数据包
	// 返 回 值:  true表示已经处理完毕，此报文可以释放了，false表示未处理，此报文应放入接收队列
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnRecvFrame(STcpServerSession *pSession,stuSTcpPackage *pPackage);



};

#endif//__SSP_BRIDGE_SERVER_H__
