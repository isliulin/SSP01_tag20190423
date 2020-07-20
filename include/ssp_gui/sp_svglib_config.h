/**
 *
 * �� �� �� : sp_svglib_config.h
 * �������� : 2015-10-26 10:39
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SVGͼԪ�������ļ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-10-26	�ۿ�������ļ�
 *
 **/

#ifndef __SP_SVGLIB_CONFIG_H__
#define __SP_SVGLIB_CONFIG_H__

#include "sp_config_mgr.h"
#include "ssp_gui_inc.h"

class SSP_GUI_EXPORT CSp_SvgLibConfig : public CConfigBase
{
public:
	CSp_SvgLibConfig();
	virtual ~CSp_SvgLibConfig();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���������ļ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-1 13:04
	// ����˵��:  @sPathFile�����ļ���
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool Load(SString sPathFile);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ݿ����ͼԪ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-11-9 13:28
	// ����˵��:  void
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	virtual bool LoadByDb();
};

#endif//__SP_SVGLIB_CONFIG_H__
