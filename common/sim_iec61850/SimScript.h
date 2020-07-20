/**
 *
 * �� �� �� : SimScript.h
 * �������� : 2017-3-27 10:52
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ģ�����ű���װ
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2017-3-27	�ۿ�������ļ�
 *
 **/

#ifndef __NAM_SIM_SCRIPT_H__
#define __NAM_SIM_SCRIPT_H__

#include "ssp_base.h"
#include "SScriptParser.h"
#include <map>
#include "sim_manager.h"

//////////////////////////////////////////////////////////////////////////
// ��    ��:  CEvalScriptParser
// ��    ��:  �ۿ���
// ����ʱ��:  2016-10-27 11:16
// ��    ��:  ��ά�����ű��ⷭ������
//////////////////////////////////////////////////////////////////////////
class CSimScriptParser : public SScriptParser
{
public:

	CSimScriptParser();
	virtual ~CSimScriptParser();


	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��չ����������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-9 10:52
	// ����˵��:  void
	// �� �� ֵ:  ������Ч��չ����������
	//////////////////////////////////////////////////////////////////////////
	static int GetExtFunctionCount();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ����ŵ���չ����ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-9 10:53
	// ����˵��:  @idxΪ��ţ���0��ʼ
	// �� �� ֵ:  stuExtFunction*, NULL��ʾ������
	//////////////////////////////////////////////////////////////////////////
	static const stuExtFunction* GetExtFunctionByIdx(int idx);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡ��չ���������� 
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-9 17:18
	// ����˵��:  void
	// �� �� ֵ:  ������Ч��չ����������
	//////////////////////////////////////////////////////////////////////////
	static int GetExtVariantCount();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ȡָ����ŵ���չ������ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2016-12-9 17:18
	// ����˵��:  @idxΪ��ţ���0��ʼ
	// �� �� ֵ:  stuExtVariant*�� NULL��ʾ������
	//////////////////////////////////////////////////////////////////////////
	static const stuExtVariant* GetExtVariantByIdx(int idx);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����GOOSE��λ
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-03-27 16:30
	// ����˵��:  @pCbParam��ʾ�ص�����(ע��ʱָ����)
	//         :  @ReturnVal��ʾ����ֵ��������
	//         :  @pParamValues��ʽ�����б�ָ�룬NULL��ʾû���βΣ������������÷�ʽ
	// �� �� ֵ:  true��ʾ���гɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool ExtFun_GooseChg(void *pCbParam,SScriptParser::stuValue &ReturnVal,SPtrList<SScriptParser::stuValue> *pParamValues);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����SV�仯
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-03-27 16:30
	// ����˵��:  @pCbParam��ʾ�ص�����(ע��ʱָ����)
	//         :  @ReturnVal��ʾ����ֵ��������
	//         :  @pParamValues��ʽ�����б�ָ�룬NULL��ʾû���βΣ������������÷�ʽ
	// �� �� ֵ:  true��ʾ���гɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool ExtFun_SvChg(void *pCbParam,SScriptParser::stuValue &ReturnVal,SPtrList<SScriptParser::stuValue> *pParamValues);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����Ҷ�ӽڵ�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-03-27 16:30
	// ����˵��:  @pCbParam��ʾ�ص�����(ע��ʱָ����)
	//         :  @ReturnVal��ʾ����ֵ��������
	//         :  @pParamValues��ʽ�����б�ָ�룬NULL��ʾû���βΣ������������÷�ʽ
	// �� �� ֵ:  true��ʾ���гɹ���false��ʾ����ʧ��
	//////////////////////////////////////////////////////////////////////////
	static bool ExtFun_SetLeaf(void *pCbParam,SScriptParser::stuValue &ReturnVal,SPtrList<SScriptParser::stuValue> *pParamValues);
	static bool ExtFun_ReportProc(void *pCbParam,SScriptParser::stuValue &ReturnVal,SPtrList<SScriptParser::stuValue> *pParamValues);


public:
	void SetSimManagerPtr(CSimManager *p){m_pSimManager = p;};
protected:
	CSimManager *m_pSimManager;
};

#endif//__NAM_SIM_SCRIPT_H__
