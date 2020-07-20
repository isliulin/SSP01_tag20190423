#ifndef VIEW_QUERY_H
#define VIEW_QUERY_H
/**
 *
 * �� �� �� : view_query.h
 * �������� : 2015-8-7 11:04
 * ��    �� : ������
 * �޸����� : 2015-8-7 11:04
 * ��ǰ�汾 : 1.0
 * �������� : ��ѯ����
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-7 	�����ࡡ��ѯ����
 *
 **/
#include <QWidget>
#include "ui_view_query.h"
#include "ssp_baseview.h"
#include "db/mysql/SMySQL.h"
#include "dbInteraction.h"
#include "include_macdef.h"
#include "structDefine.h"
#include "ssp_datawindow.h"
#include "mtreewidget.h"
#include "mcombobox.h"
#include "record_sql.h"
#include "clearData.h"
#include "qt/SQt.h"
#include "view_dw_condlg.h"

class view_query : public CBaseView
{
	Q_OBJECT

public:
	view_query(QWidget *parent = 0,SString sFunName="",bool isMDB=false);
	~view_query();
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ʼ�����ݴ���
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-7-30 11:13
	// ����˵��:  @sWname �����ļ���dataset������
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void InitDataWindow(SString sWname);
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ݲ�ͬ�˵����ʼ������
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-7-30 11:16
	// ����˵��:  @sWname �����ļ���dataset������
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	bool InitWidget(SString sWname);
	bool SetTableValue(int row,int col,QString value);//ͨ���������ñ��ֵ
	int getTableRowByFcol(QString colValue);//ͨ����һ�У��ٶ�Ψһ���ҵ��к�
	void OnPreShow();
	bool refreshReady;
private slots:
	void onPushButtonPressed();//��ѯ��ť��Ӧ
	void onPushButton2Pressed();//��ӡ��ť��Ӧ
	void onPushButton3Pressed();//������ť��Ӧ
	void queryTable();//�õ���ѯ������Ϣ����ʾ���
	void timerOut();
	void on_tableWidget_itemClicked(QTableWidgetItem * item);
private:
	Ui::view_query ui;
	SDatabasePool<SMySQL> *m_pPool;
	SDatabaseOper *m_pOper;
	SString m_tableName;//����
	SString m_refName;//������
	SString m_pre;//�޸�֮ǰ��ֵ
	bool m_bInsert;//�Ƿ���
	CSsp_DwColumn *m_fieldInfo;//�ֶ���Ϣ
	SPtrList<CSsp_DwColumn> *m_fieldlist;//�����ֶ���Ϣ
	vector<SString> m_fieldNamelist;//�ֶ�ԭ�������б�
	int m_nPkey;//������������
	int m_tableRow;//�����ϲ����е�����
	CSsp_DatawindowMgr *m_dataWindow;//���ݴ��ڹ������
	QWidget * m_widget;
	SRecordset* m_Record;
	record_sql m_sqlRecord;//���
	view_dw_condlg *dlg;
	bool isMDB;//�Ƿ����ڴ����ݿ�
	bool m_bNotPreShow;
	bool m_bCondConfirm;//�Ƿ������Ѿ���ȷ��������ʱΪtrue������ȷ����һ��ҲΪtrue������Ϊfalse
	QTimer *timer;
	QFont m_font_underline;

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ͨ�ñ��ʼ����ͨ�ô���
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-7-30 14:37
	// ����˵��:  @
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	SPtrList<CSsp_DwColumn> *getFieldInfo(SString sWname,SPtrList<CSsp_DwColumn> *cFinfo=NULL);
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��ͨ�ñ��ʼ����ͨ�ô���
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-7-30 14:37
	// ����˵��:  @��ѯ��Ϣ������Ϊ0����ѯ
	// �� �� ֵ:  
	// �� �� ��:  ������
	// �޸�ʱ��:  2015-8-8 11:15
	// �޸�����:  �޸�����ӷ�ʽ����ӹ��˲�ѯ���ݵ�
	//////////////////////////////////////////////////////////////////////////
	void SetCommonToWidget(CSsp_Dataset* dbSet);
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��sql����л�ȡ����
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-7-30 14:47
	// ����˵��:  @sql ��䣨ֻ֧�ֵ��ű���䣩
	// �� �� ֵ:  ����
	//////////////////////////////////////////////////////////////////////////
	SString GetTableFromSql(SString sSql);
	
	static BYTE* OnMdbDatasetTrgCallback(void* cbParam, CMdbClient *pMdbClient,CSsp_Dataset *pDataset, eMdbTriggerType eType,int iTrgRows,int iRowSize,BYTE *pTrgData);//�ص�����
};

#endif // VIEW_QUERY_H
