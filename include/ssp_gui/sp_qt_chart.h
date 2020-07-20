/**
 *
 * �� �� �� : sp_qt_chart.h
 * �������� : 2015-8-10 14:14
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ����QT��ͼ����ƺ����ӿ�
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-8-10	�ۿ�������ļ�
 *
 **/

#ifndef __SP_QT_CHART_H__
#define __SP_QT_CHART_H__

#include "SDatabase.h"
#include "qt/SQt.h"
#include "gui/SBaseDC.h"
#include "ssp_gui_inc.h"

class SSP_GUI_EXPORT CSpQtChartPainter
{
public:
	CSpQtChartPainter();
	~CSpQtChartPainter();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���Ʊ�ͼ
	// ��    ��:  
	// ����ʱ��:  2015-8-10 14:18
	// ����˵��:  @pPainterΪQT�������
	//         :  @rectΪ�������򣬻�����Ϊ���ɳ���������
	//         :  @pRsΪ��Ҫ��ʾ�����ݼ�
	//         :  @sTitleΪͼ�����ʾ���⣬��sTitleΪ��ʱȡ�ڶ��е��б���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void DrawChartPie(QPainter *pPainter,SRect &rect,SRecordset *pRs,SString sTitle="",QColor corText=Qt::black,QColor corLine=Qt::gray,SString sExtAttr="");

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������ͼ
	// ��    ��:  
	// ����ʱ��:  2015-8-10 14:18
	// ����˵��:  @pPainterΪQT�������
	//         :  @rectΪ�������򣬻�����Ϊ���ɳ���������
	//         :  @pRsΪ��Ҫ��ʾ�����ݼ�
	//         :  @sTitleΪͼ�����ʾ���⣬��sTitleΪ��ʱȡ�ڶ��е��б���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void DrawChartColumn(QPainter *pPainter,SRect &rect,SRecordset *pRs,SString sTitle="",QColor corText=Qt::black,QColor corLine=Qt::gray,SString sExtAttr="");

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������ͼ
	// ��    ��:  
	// ����ʱ��:  2015-8-10 14:18
	// ����˵��:  @pPainterΪQT�������
	//         :  @rectΪ�������򣬻�����Ϊ���ɳ���������
	//         :  @pRsΪ��Ҫ��ʾ�����ݼ�
	//         :  @sTitleΪͼ�����ʾ���⣬��sTitleΪ��ʱȡ�ڶ��е��б���
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void DrawChartLine(QPainter *pPainter,SRect &rect,SRecordset *pRs,SString sTitle="",QColor corText=Qt::black,QColor corLine=Qt::gray,SString sExtAttr="");


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����Ǳ�ͼ
	// ��    ��:  
	// ����ʱ��:  2015-12-11 14:18
	// ����˵��:  @pPainterΪQT�������
	//         :  @rectΪ�������򣬻�����Ϊ���ɳ���������
	//         :  @pRsΪ��Ҫ��ʾ�����ݼ�
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	static void DrawPanel(QPainter *pPainter,SRect &rect,float currentValue,SString sExtAttr);
private:
	static QColor ScorToQcor(SString srgb);
	static QColor StrToQcor(SString srgb);
};

#endif//__SP_QT_CHART_H__