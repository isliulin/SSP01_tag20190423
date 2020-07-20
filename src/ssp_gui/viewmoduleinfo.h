/**
 *
 * �� �� �� : viewmoduleinfo.h
 * �������� : 2013-9-13 12:58
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ִ��ģ��İ汾��Ϣ
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2013-9-13	�ۿ�������ļ�
 *
 **/

#ifndef VIEWMODULEINFO_H
#define VIEWMODULEINFO_H

#include <QWidget>
#include "ui_viewmoduleinfo.h"
#include "ssp_baseview.h"
#include "SApplication.h"
#include "ssp_database.h"

class CViewModuleInfo : public CBaseView
{
	Q_OBJECT

public:
	struct stuRunUnit
	{
		stuRunUnit()
		{
			m_ProcInfos.setAutoDelete(true);
		}
		~stuRunUnit()
		{
			m_ProcInfos.clear();
		}
		stuSpUnitAgentUnitInfo UnitInfo;
		SPtrList<stuSpUnitAgentProcInfo> m_ProcInfos;
	};
	struct stuRealModInfo
	{
		SString unit_name;
		SString mod_name;
		SString desc;
		SString create_time;
		SString version;
		SString crc;
		SString run_state;
	};

	CViewModuleInfo(QWidget *parent = 0);
	virtual ~CViewModuleInfo();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������׼����ʾǰ�����麯�������¼�������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013:9:13 13:03
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnPreShow();

	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���¼���ʵʱģ����Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013:10:23 10:03
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void ReLoad();

	virtual void OnRefresh(){OnPreShow();};
signals:
	void sigInfoUpdated();
public slots:
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʱ������������ʾ�̲߳�ѯ�����ļ�¼
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013:10:23 10:29
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void OnTimerUI();

	void OnQuery();
	void OnSave();
	void OnPrint();

private:
	////////////////////////////////////////////////////////////////////////
	// ��    ��:  �޸ļ����߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2013:10:23 10:02
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadLoadInfo(void *lp);

	bool m_bLoading;//�Ƿ����ڼ���
	SPtrList<stuRunUnit> m_RunUnits;//���е����е�Ԫ���Լ������е�Ԫ�ڵ�Ӧ�ý���
	Ui::CViewModuleInfo ui;
};

#endif // VIEWMODULEINFO_H
