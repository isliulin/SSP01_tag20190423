/**
 *
 * �� �� �� : view_mdb_svg.h
 * �������� : 2015-8-4 13:09
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ӳ�䵽�ڴ����ݿ��SVGͼ�μ��Ӵ���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-4	�ۿ�������ļ�
 *
 **/
#ifndef __SKT_VIEW_MDB_SVG_H__
#define __SKT_VIEW_MDB_SVG_H__

#include <QtGui>
#include "ssp_baseview.h"
#include "ssp_database.h"
#include "ssp_datawindow.h"
#include "SSvgWnd.h"

class SSP_GUI_EXPORT CViewMdbSvg;

enum eSvgItemMapType
{
	SVGMAP_TEXT=1,		//�ı�ͼԪӳ��
	SVGMAP_STATE,		//��״̬ͼԪӳ��
	SVGMAP_CHART_PIE,	//��ͼͼԪӳ��
	SVGMAP_CHART_COL,	//��ͼͼԪӳ��
	SVGMAP_CHART_LINE,	//����ͼԪӳ��
	SVGMAP_CHART_PANEL,	//�Ǳ���ͼԪӳ��
	SVGMAP_DYN_RS_RECT,	//��̬��¼��ӳ������
	SVGMAP_AUTO_FLASH,	//�Զ���˸ͼԪ
};
struct stuSvgMapToRecord
{
	SSvgObject *pSvgObj;		//SVG����
	CSsp_Dataset *pDataset;		//��Ӧ���ݼ�
	int iRowIndex;				//��Ӧ���кţ���0��ʼ���
	int iColIndex;				//��Ӧ���кţ���0��ʼ���
	CSsp_Dataset *pFlashDataset;//�Զ���˸��Ӧ���ݼ�
	int iFlashRowIndex;			//�Զ���˸��Ӧ���кţ���0��ʼ���
	int iFlashColIndex;			//�Զ���˸��Ӧ���кţ���0��ʼ���
	//SString sRowPrimKey;		//��Ӧ�е�����ֵ���������ͨ��","�ָ�����β����","
	//SRecord *pRecord;			//ӳ��ͼԪ��Ӧ����
	SString sExtAttr;			//��չ����
	eSvgItemMapType m_iSvgMapType;//ͼԪӳ������
	//CSsp_DwColumn *pDsColumn;	//��Ӧ���ݼ��е���
};

class SSP_GUI_EXPORT CMdbSvgWnd : public SSvgWnd
{
public:
	friend class CViewMdbSvg;
	CMdbSvgWnd(QWidget *parent);
	virtual ~CMdbSvgWnd();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������Ӧ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-4 14:07
	// ����˵��:  @pSvgObj
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnClickObject(SSvgObject *pSvgObj);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡͼԪ����ʾ�ı�������ͨ��Ӧ�ò�ص�������ȡ��ȡ��������ͼԪ��title�ı�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-8 14:45
	// ����˵��:  @pSvgObj
	// �� �� ֵ:  SString
	//////////////////////////////////////////////////////////////////////////
	virtual SString GetSvgObjectTipText(SSvgObject *pSvgObj);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �û��Ի��麯�����Ի��������Ծ���ͼԪ��������չ�����к�userdraw=true;
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-10 14:07
	// ����˵��:  @pDCΪ����DC�����QTʱͨ��(QPainter*)pDC->GetHandle()ʹ��
	//         :  @rectΪָ��Ļ������򣬻��Ʋ��ɳ���ָ�������Χ
	//         :  @pObj��ʾ���ƵĶ���ָ�룬���û��������չ����ȷ������ģ��
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void DrawUser(SBaseDC *pDC,SRect &rect, SSvgObject *pObj);


	CViewMdbSvg *m_pViewMdbSvg;
};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CViewMdbSvg
// ��    ��:  �ۿ���
// ����ʱ��:  2015-8-10 14:08
// ��    ��:  ӳ��MDB���SVG��ͼ
//////////////////////////////////////////////////////////////////////////
class SSP_GUI_EXPORT CViewMdbSvg : public CBaseView,public SSvgDataModel
{
	Q_OBJECT
public:
	friend class CMdbSvgWnd;

	CViewMdbSvg(int wnd_sn,QWidget *parent,bool bFloat=false,SString *pExtAttr=NULL);
	virtual ~CViewMdbSvg();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������ʾ������麯��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-4 14:27
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	virtual void OnAftShow();

	virtual void OnClickObject(SSvgObject *pSvgObj);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡͼԪ����ʾ�ı�������ͨ��Ӧ�ò�ص�������ȡ��ȡ��������ͼԪ��title�ı�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-8 14:45
	// ����˵��:  @pSvgObj
	// �� �� ֵ:  SString
	//////////////////////////////////////////////////////////////////////////
	SString GetSvgObjectTipText(SSvgObject *pSvgObj);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ȱʡ���ݼ�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-6 15:58
	// ����˵��:  @strΪ���ݼ�����
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetDatasetName(SString str){m_sDatasetName = str;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ö�̬����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-6 16:16
	// ����˵��:  @strΪ�����ַ���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void SetCondition(SString str){m_sCondition = str;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ�����������ʾ�ı�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-18 17:23
	// ����˵��:  @pObjΪͼԪָ��
	//         :  @sTextΪ���÷��ص��ı�����
	// �� �� ֵ:  true��ʾȡ���������ݣ�false��ʾδȡ����������
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewText(SSvgObject *pObj,SString &sText);

	virtual bool GetViewTextColor(SSvgObject *pObj,SCOLOR &sColor);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ���������������ɫ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-18 17:24
	// ����˵��:  @pObjΪͼԪָ��
	//         :  @sColorΪ���÷��ص���ɫ
	// �� �� ֵ:  true��ʾȡ���������ݣ�false��ʾδȡ����������
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewLineColor(SSvgObject *pObj,SCOLOR &sColor);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��������������ɫ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-18 17:25
	// ����˵��:  @pObjΪͼԪָ��
	//         :  @bFillΪ���÷��ص��Ƿ����
	//         :  @sColorΪ���÷��ص���ɫ
	// �� �� ֵ:  true��ʾȡ���������ݣ�false��ʾδȡ����������
	//////////////////////////////////////////////////////////////////////////
	virtual bool GetViewBackColor(SSvgObject *pObj,bool &bFill,SCOLOR &sColor);

	virtual void hideEvent(QHideEvent *event);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ִ�е����SVG��̬�����ض�����һ����̬ҳ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-3-1 18:14
	// ����˵��:  @wnd_snΪ��̬�������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void JumpWnd(int wnd_sn);

public slots:
	void OnTimerReloadSvg();
	void OnRefreshSvg();
	void OnTimerRepaint();


signals:
	void DoRefSvg();
private:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ͼԪ�����ݵ�ӳ���ϵ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-6 15:56
	// ����˵��:  @pSvgObjΪͼԪָ��
	//         :  @sExtAttrΪ����չ����
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void AddObjectMap(SSvgObject *pSvgObj,SString &sExtAttr);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����̬���ݼ����򣬰����ݼ��������ɸ��е�Ԫ�ز��Զ�ӳ��ͼԪ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-11-16 8:36
	// ����˵��:  @pRootObjΪ���ڵ�
	//         :  @pRectObjΪ�������
	// �� �� ֵ:  true��ʾ�����ݴ���false��ʾ����Ϊ�գ����ֽڵ㱻ɾ������Ҫ���¿�ʼ�����ڵ�
	//////////////////////////////////////////////////////////////////////////
	bool ProcessDynRsRect(SSvgObject *pRootObj,SSvgObject *pRectObj);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ��ӳ��ͼԪ������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-6 16:18
	// ����˵��:  @pMapΪӳ���ϵ
	//         :  @pRΪ��Ӧ���У�NULL��ʾδ֪��Ӧ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void UpdateSvgState(stuSvgMapToRecord *pMap,SRecord *pR=NULL);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���ݼ����µĻص�����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-7 15:00
	// ����˵��:  @cbParamΪthisָ��
	//         :  @pMdbClientΪMDB�ͻ��˽ӿ�
	//         :  @pDatasetΪ��ǰ���ݼ�
	//         :  @...
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static BYTE* OnMdbDsTrgCallback(void* cbParam, CMdbClient *pMdbClient,CSsp_Dataset *pDataset, eMdbTriggerType eType,int iTrgRows,int iRowSize,BYTE *pTrgData);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ�����д����ص�������ע��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-7 15:02
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void RealeaseAllTrgCallback();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ʵʱˢ��SVG��ʾ����ֱ��ˢ�£�������Ϣ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-8-7 15:30
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void RefreshSvg();

	inline CSsp_Dataset* SearchDynSvgMapDataset(CSsp_Dataset* pDs)
	{
		unsigned long pos=0;
		CSsp_Dataset *p = m_DynSvgMapDataset.FetchFirst(pos);
		while(p)
		{
			if(p->GetName() == pDs->GetName())
				return p;
			p = m_DynSvgMapDataset.FetchNext(pos);
		}
		return NULL;
	}

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����SVG�̣߳����ں�̨���أ���Ӱ��GUI���߳�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2015-12-8 17:43
	// ����˵��:  @lp��this
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static void* ThreadReloadSvg(void *lp);

private:
	QVBoxLayout *m_verticalLayout_2;
	CMdbSvgWnd *m_pSvg;
	SSvgWnd *m_pTempSvg;//��ʱ�����õ�SVGʵ��
	QTimer *m_pTimer,*m_pTimer2;
	SString m_sDatasetName;//������ȱʡ���ݼ�����
	SString m_sCondition;//��̬����
	SPtrList<CSsp_Dataset> m_SvgMapDataset;//SVGͼԪ���õ����ݼ�����
	SPtrList<CSsp_Dataset> m_DynSvgMapDataset;//��̬SVGͼԪ���õ����ݼ����ϣ���Ҫ�ͷ�
	SPtrList<stuSvgMapToRecord> m_SvgItemMaps;//SVGͼԪ���¼����Ԫ�ص�ӳ���ϵ��
	SPtrList<stuSvgMapToRecord> m_SvgItemAutoFlashMaps;//�Զ���˸SVGͼԪ���¼����Ԫ�ص�ӳ���ϵ��
	int m_iSvgUpdateSn;//SVG�������
	int m_iSvgShowSn;//SVG��ʾ���
	time_t m_LastRefresgSvgTime;
	bool m_bFloatWnd;//�Ƿ�Ϊ��������
	//CViewMdbSvg *m_pParentView;//�����ڣ�������������Ч
	bool m_bLoadingSvg;//�Ƿ����ڼ���SVG
	SString m_sTempTip;
	char *m_pSvgText;
	int m_iSvgTextLen;
	int m_iDynRsRectCnt;
};

#endif//__SKT_VIEW_MDB_SVG_H__
