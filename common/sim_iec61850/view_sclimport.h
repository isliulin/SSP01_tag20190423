/**
 *
 * �� �� �� : view_sclimport.h
 * �������� : 2015-12-23 13:17
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SCL�ļ�����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-12-23	�ۿ�������ļ�
 *
 **/

#ifndef VIEW_SCLIMPORT_H
#define VIEW_SCLIMPORT_H

#include <QWidget>
#include "ui_view_sclimport.h"
#include "ssp_baseview.h"
#include "sim_config.h"
#include "sp_sclparser.h"
#include "sp_virtual_terminal.h"

class view_sclimport : public CBaseView
{
	Q_OBJECT

public:
	view_sclimport(QWidget *parent = 0);
	~view_sclimport();

public slots:
	void OnOpen();
	void OnSelAll();
	void OnSelNone();
	void OnSelRev();
	void OnImport();

private:
	static void* ThreadLoadScl(void *lp);
	static void* ThreadImportScl(void *lp);

	SString m_sSclFile;
	int m_iSclLoadState;//0:loading 1:load error, 2:load failed

	SPtrList<CSpSclParser::stuSCL_IED> m_ImportIed;
	int m_iSclImportState;//0:importing, >0��������, <0��ʾʧ��
	int m_iSclImportPercent;

	Ui::view_sclimport ui;
	CSpSclParserEx* m_pScl;
	CSimConfig *m_pSimConfig;
	float m_fPrimaryVoltage;//һ�ζ��ѹ
	float m_fSecondaryVoltage;//���ζ��ѹ
	float m_fPriaryCurrent;//һ�ζ����
	float m_fSecondaryCurrent;//���ζ����
};

#endif // VIEW_SCLIMPORT_H
