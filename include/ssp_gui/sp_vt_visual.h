/**
 *
 * �� �� �� : sp_vt_visual.h
 * �������� : 2016-1-8 10:13
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : �������ؿ��ӻ�ʵ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2016-1-8	�ۿ�������ļ�
 *
 **/

#ifndef __SP_VIRTUAL_TERMINAL_VISUAL_H__
#define __SP_VIRTUAL_TERMINAL_VISUAL_H__

#include "sp_virtual_terminal.h"
#include "SSvgWnd.h"
#include "ssp_gui_inc.h"

/*
URI���壺
home							��ʾ����ҳ��ť
subnet.<subnet_name>			��ʾһ��ͨѶ����
ied.<ied_name>					��ʾIED����
svcb.<ied_name>$0x<appid>		��ʾָ��IED��ָ��APPID��SV������ݼ�
gocb.<ied_name>$0x<appid>		��ʾָ��IED��ָ��APPID��GOOSE������ݼ�
svin.<out_ied_name>$<out_smv_no>$<in_ied_name>  ��ʾSV���ݼ������붩��
goin.<out_ied_name>$<out_gse_no>$<in_ied_name>  ��ʾGOOSE���ݼ������붩��
svchin.<out_ied_name>$<out_smv_no>$<in_ied_name>$<in_smv_no>  ��ʾSV���ݼ������붩��
gochin.<out_ied_name>$<out_gse_no>$<in_ied_name>$<in_smv_no>  ��ʾGOOSE���ݼ������붩��
v_line							��ʾ�л�Ϊ����·ͼ
v_terminal						��ʾ�л�Ϊ�����ͼ
*/

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CSpVtSvgGenerator
// ��    ��:  �ۿ���
// ����ʱ��:  2016-1-8 10:18
// ��    ��:  SVG������
//////////////////////////////////////////////////////////////////////////
class SSP_GUI_EXPORT CSpVtSvgGenerator
{
public:
	struct SSP_GUI_EXPORT stuHistoryLine
	{
		float x1,y1;
		float x2,y2;
	};
	struct stuSvgVtCbIedItem;
	struct SSP_GUI_EXPORT stuSvgVtCbIed
	{
		stuSvgVtCbIed()
		{
			items.setAutoDelete(true);
		}
		~stuSvgVtCbIed()
		{
			items.clear();
		}
		stuSclVtIed *pRelaIed;//����IED
		SPtrList<stuSvgVtCbIedItem> items;
		int ied_height;//ied�ܸ߶�
	};
	struct SSP_GUI_EXPORT stuSvgVtCbIedItem
	{
		stuSvgVtCbIedItem()
		{
			memset(this,0,sizeof(stuSvgVtCbIedItem));
		}
		int type;//1:smv in 2:gse in 3:smv out 4:gse out
		stuSclVtIed *pRelaIed;//����IED
		stuSclVtIedSmvOut *pSmvOut;
		stuSclVtIedGooseOut *pGooseOut;
		stuSclVtIedSmvIn *pSmvIn;
		stuSclVtIedGooseIn *pGooseIn;
		int center_y;//������ƿ����ĵ�Y������

	};
	stuSvgVtCbIed* SearchRelaIed(stuSclVtIed *pRelaIed);
	void SortRelaIed();

	//struct stuIed
	//���ͼ���п��ƹ�ϵ��IED���飬һ��������ܰ����������
	struct SSP_GUI_EXPORT stuBayIedGroup
	{
		SPtrList<stuSclVtIed> m_GroupIeds;
		SSvgObject::SVG_RECT m_GruopRect;//��ǰ������귶Χ
	};
	CSpVtSvgGenerator();
	virtual ~CSpVtSvgGenerator();

	void SetSclVt(CSclVt *p){m_pSclVt = p;};

	SString GetCBColor(stuSclVtIedSmvOut *pOut);

	SString GetCBColor(stuSclVtIedGooseOut *pOut);

	SString GetCBLineColor(stuSclVtIedSmvOut *pOut,stuSclVtIedSmvInChannel *pChn=NULL);

	SString GetCBLineColor(stuSclVtIedGooseOut *pOut,stuSclVtIedGooseInChannel *pChn=NULL);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ��������ͨѶ���ʵ���ӻ�SVGͼ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-1-11 8:59
	// ����˵��:  @sSubNetName��ʾ�������ƣ�""��ʾ��һ������
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool GeneratorSubNetSvg(SString sSubNetName, SXmlConfig &SvgXml);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ��IED�Ŀ��ƿ鷢�������Ĺ�ϵͼ����ָ��IEDΪ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-1-11 9:00
	// ����˵��:  @sIedName��ʾIED���ƣ�""��ʾ��һ��IED
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool GeneratorIedCBInOut(SString sIedName,SXmlConfig &SvgXml);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ָ��IED�ĳ�������ƿ��������������ͼ����ָ��IEDΪ����
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-1-13 17:00
	// ����˵��:  @sIedName��ʾIED���ƣ�""��ʾ��һ��IED
	// �� �� ֵ:  false/true
	//////////////////////////////////////////////////////////////////////////
	bool GeneratorIedVirtualTerminal(SString sIedName,SXmlConfig &SvgXml);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����߶�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-1-12 9:02
	// ����˵��:  
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void AddLine(SBaseConfig *pRootNode, int x1,int y1,int x2,int y2,bool bArrow,char* sAttrs);
	void AddBeeLine(SBaseConfig *pRootNode, int x1,int y1,int x2,int y2,bool bArrow,char* sAttrs);

	int NewHLineY(int y0)
	{
		unsigned long pos;
		int* p;
		int y=y0;
		int idx=1;
		again:
		p = m_iHLineY.FetchFirst(pos);
		while(p)
		{
			if(*p == y)
			{
//				if(idx&1==0)
					y = y0+idx*5;
// 				else
// 					y = y0-idx*10;
 				idx++;
				goto again;
			}
			p = m_iHLineY.FetchNext(pos);
		}
		m_iHLineY.append(y);
		return y;
	}

	int NewVLineX(int x0)
	{
		unsigned long pos;
		int* p;
		int x = x0;
		int idx=1;
		again:
		p = m_iVLineX.FetchFirst(pos);
		while(p)
		{
			if(*p == x)
			{
				if((idx&1)==0)
					x = x0+idx*10;
				else
					x = x0-idx*10;
				idx++;
				goto again;
			}
			p = m_iVLineX.FetchNext(pos);
		}
		m_iVLineX.append(x);
		return x;
	}

	void SortAndMoveIed(SPtrList<stuSclVtIed> &src_ied,SPtrList<stuSclVtIed> &dst_ied)
	{
		stuSclVtIed *pIedMin,*pIed2;
		unsigned long pos1;
		while(src_ied.count()>0)
		{
			pIedMin = pIed2 = src_ied.FetchFirst(pos1);
			while(pIed2)
			{
				if(strcmp(pIed2->name,pIedMin->name) < 0)
					pIedMin = pIed2;
				pIed2 = src_ied.FetchNext(pos1);
			}
			dst_ied.append(pIedMin);
			src_ied.remove(pIedMin);
		}

	}

	CSclVt *m_pSclVt;
	bool m_bShowVT;//�Ƿ���ʾ�����ͼ
	SString m_sStationName;//��վ����	
	SPtrList<stuHistoryLine> m_HisLines;
	SValueList<int> m_iHLineY;//ˮƽ��Y�����
	SValueList<int> m_iVLineX;//��ֱ��X�����
	SPtrList<stuSvgVtCbIed> m_RelaIeds;
};

class SSP_GUI_EXPORT CSpVtSvgAccessPoint : public SSvgWnd
{
public:
	CSpVtSvgAccessPoint(QWidget *parent);
	virtual ~CSpVtSvgAccessPoint();

	void GotoSubNet(SString sSubNetName);
	void GotoIed(SString sIedName);


	void SetSvgGenerator(CSpVtSvgGenerator *p){m_pGenerator = p;};
	virtual void OnClickObject(SSvgObject *pSvgObj);
	//ȡ������ʾ�ı��ص�
	virtual SString GetSvgObjectTipText(SSvgObject *pSvgObj);

	CSpVtSvgGenerator *m_pGenerator;
	SString m_sLastShowType;
	SString m_sLastSubNetName;
	SString m_sLastIedName;
};

#endif//__SP_VIRTUAL_TERMINAL_VISUAL_H__
