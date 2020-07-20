/**
 *
 * �� �� �� : sp_log_config.h
 * �������� : 2015-8-1 13:02
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ��־�����ļ�������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-1	�ۿ�������ļ�
 *
 **/

#ifndef __SP_LOG_CONFIG_H__
#define __SP_LOG_CONFIG_H__

#include "sp_config_mgr.h"
#include "ssp_base_inc.h"

class SSP_BASE_EXPORT CLogConfig : public CConfigBase
{
public:
	CLogConfig();
	virtual ~CLogConfig();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���������ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-1 13:04
	// ����˵��:  @sPathFile�����ļ���
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Load(SString sPathFile);
};

#endif//__SP_LOG_CONFIG_H__
