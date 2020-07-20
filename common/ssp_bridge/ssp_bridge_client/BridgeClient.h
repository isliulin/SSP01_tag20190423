/**
 *
 * �� �� �� : BridgeSvr.h
 * �������� : 2018-10-12 16:37
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : �Žӷ������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2018-10-12	�ۿ�������ļ�
 *
 **/

#ifndef __SSP_BRIDGE_SERVER_H__
#define __SSP_BRIDGE_SERVER_H__

#include "SService.h"
#include "SApi.h"
#include "SSocket.h"
#include "SString.h"
#include "STcpClientBase.h"
#include "../public/bridge_inc.h"

class CBridgeClient : public STcpClientBase
{
public:
	struct stuTcpClientParam
	{
		stuTcpClientParam()
		{
			pSock = NULL;
			in_bytes = out_bytes = 0;
		}
		~stuTcpClientParam()
		{
			if(pSock != NULL)
				delete pSock;
		}
		CBridgeClient *pThis;
		SSocket *pSock;
		SString session_id;
		unsigned int in_bytes,out_bytes;
	};
	CBridgeClient();
	virtual ~CBridgeClient();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Է�����ʽ�����ͻ��ˣ�������Connect����ͬʱʹ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2018-10-12 22:05
	// ����˵��:  @ip��ʾ�����IP��ַ��@port��ʾ�˿�
	// �� �� ֵ:  true��ʾ���������ɹ�,false��ʾ��������ʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Start(SString ip,int port,int outside_port,SString inside_ip,int inside_port);

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ֹͣ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2018-10-12 22:05
	// ����˵��:  void
	// �� �� ֵ:  true��ʾ����ֹͣ�ɹ�,false��ʾ����ֹͣʧ��
	//////////////////////////////////////////////////////////////////////////
	virtual bool Stop();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��¼�Ϸ���˺�Ļص�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2018-10-12 22:05
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnLogin();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �Ự�߳̽��յ����ĺ�Ļص��麯����������ͨ���˺�������ʱ��Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2018-10-12 22:05
	// ����˵��:  @pSessionΪ�Ựʵ��ָ�룬����ͨ��ָ����Զ˷�������
	//            @pPackageΪ�ս��յ������ݰ�
	// �� �� ֵ:  true��ʾ�Ѿ�������ϣ��˱��Ŀ����ͷ��ˣ�false��ʾδ�����˱���Ӧ������ն���
	//////////////////////////////////////////////////////////////////////////
	virtual bool OnRecvFrame(stuSTcpPackage *pPackage);

	static SString GetInput(SString tip="",SString def="");

	static void* ThreadTcpSvrInnerClient(void *lp);

	SString m_sBrgIp;
	int m_iBrgPort;
	SString m_sInnerIp;
	int m_iInnerPort;
	SString m_sName;
	SPtrList<stuTcpClientParam> m_TcpClientParams;
};

#endif//__SSP_BRIDGE_SERVER_H__
