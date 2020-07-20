/**
 *
 * �� �� �� : SimScript.cpp
 * �������� : 2017-3-27 10:51
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

#include "SimScript.h"
#include <math.h>

//������չ����
static const SScriptParser::stuExtFunction g_nam_eval_ext_fun[] = {
	{"$GooseChg",		CSimScriptParser::ExtFun_GooseChg,		CSimScriptParser::VAL_BOOL,	2,	"int appid,string vals",	"����һ��GOOSE��λ��valsΪch_name1=val1;ch_name2=val2;"	},//val����Ϊtrue/false��Ҳ����Ϊnow(��ʾ��ǰʱ��)
	{"$SvChg",			CSimScriptParser::ExtFun_SvChg,			CSimScriptParser::VAL_BOOL,	4,	"int appid,int ch_idx,float rms,int q",	"�޸�SVֵ��ch_idΪͨ����(��1��ʼ),rmsΪ��Чֵ,qΪƷ��"	},
	{"$SetLeaf",		CSimScriptParser::ExtFun_SetLeaf,		CSimScriptParser::VAL_BOOL,	4,	"string ied_ld,string leaf_path,int vt,string val",	"�޸�LNֵ��ied_ldIED��LD���ƣ�leaf_pathΪҶ�ӽڵ�·��,vtΪֵ����(1-bool,2-int,3-float,4-utctime)"	},
	{"$ReportProc",		CSimScriptParser::ExtFun_ReportProc,	CSimScriptParser::VAL_BOOL,	0,	"",	"ִ��һ��MMS����ɨ�����"	},
// 
// 	{"$��һ����ʷ״̬��",	CSimScriptParser::ExtFun_NextHisState,		CSimScriptParser::VAL_BOOL,	0,	"",	"ָ����һ����ʷ״̬����$��ʷ״̬��ͳ��()���õ�һ��ʹ�ã���ʾ�ƶ�����һ����¼����������bool(true��ʾ��¼���ڣ�false��ʾ��¼������)"	},
// 
// 	{"$��ʷ�¼���ͳ��",		CSimScriptParser::ExtFun_HisEventCnt,		CSimScriptParser::VAL_BOOL,	0,	"",	"��ȡ��ǰ��վ������δ������¼�����¼"	},
// 	{"$��һ����ʷ�¼���",	CSimScriptParser::ExtFun_NextHisEvent,		CSimScriptParser::VAL_BOOL,	0,	"",	"ָ����һ����ʷ�¼�����$��ʷ�¼���ͳ��()���õ�һ��ʹ�ã���ʾ�ƶ�����һ����¼����������bool(true��ʾ��¼���ڣ�false��ʾ��¼������)"	},
// 	
// 	{"$��ǰδ����״̬��",	CSimScriptParser::ExtFun_UnResumedStCnt,	CSimScriptParser::VAL_BOOL,	0,	"",	"��ȡ��ǰ��վ������δ�����״̬����¼"	},
// 	{"$��һ��δ����״̬��",	CSimScriptParser::ExtFun_NextUnResumeSt,	CSimScriptParser::VAL_BOOL,	0,	"",	"ָ����һ��δ����״̬����$��ǰδ����״̬��()���õ�һ��ʹ�ã���ʾ�ƶ�����һ����¼����������bool(true��ʾ��¼���ڣ�false��ʾ��¼������)"	},
// 	{"$ȡ��ǰ����ͨ��״̬",	CSimScriptParser::ExtFun_ResunCommSt,		CSimScriptParser::VAL_INT,		0,	"",	"ȡ��ǰδ����ͨ��״̬�����������µ�ͨ��״̬��0��ʾͨ����·�쳣����������������·��1��ʾͨ����·�쳣����������������·��2��ʾͨ����·����"	},
// 
// 	{"$�۵�ǰ�豸��",		CSimScriptParser::ExtFun_CurrDevDeduct,	CSimScriptParser::VAL_VOID,	2,	"float �۷�ֵ,string ��ѡ�۷�ԭ��",	"�Ե�ǰװ�ý��п۷�"	},//�������۷�ֵ(float),string �۷�ԭ��
// 	{"$�۵�ǰ��վ��",		CSimScriptParser::ExtFun_CurrSubDeduct,	CSimScriptParser::VAL_VOID,	2,	"float �۷�ֵ,string ��ѡ�۷�ԭ��",	"�Ե�ǰ��վ���п۷�"	},//�������۷�ֵ(float),string �۷�ԭ��

	{NULL,						NULL,									CSimScriptParser::VAL_UNKNOWN,	0,	NULL,	NULL	}
};

//�ⲿ����
static const SScriptParser::stuExtVariant g_nam_eval_ext_var[] = {
// 	{"@��վ���",			SScriptParser::VAL_INT,			"",		""	},
// 	{"@�豸���",			SScriptParser::VAL_INT,			"",		""	},
// 	{"@�豸����",			SScriptParser::VAL_STRING,		"",		""	},
// 	{"@Э������",			SScriptParser::VAL_INT,			"",		""	},
// 	{"@��������",			SScriptParser::VAL_STRING,		"",		""	},
// 	{"@��Ϣ����",			SScriptParser::VAL_STRING,		"",		""	},
// 	{"@�쳣״̬����",		SScriptParser::VAL_STRING,		"",		""	},
// 	{"@��ʷ״̬������",		SScriptParser::VAL_INT,			"",		""	},
// 	{"@��ʷ�¼�������",		SScriptParser::VAL_INT,			"",		""	},
// 	{"@δ����״̬ʱ��",		SScriptParser::VAL_INT,			"",		""	},

	{NULL,					SScriptParser::VAL_VOID,		NULL,	NULL}
};


CSimScriptParser::CSimScriptParser()
{
	m_pSimManager = NULL;
// 	m_vSubNo.SetName("@��վ���");
// 	m_vDevSn.SetName("@�豸���");
// 	m_vDevName.SetName("@�豸����");
// 	m_vProtType.SetName("@Э������");
// 	m_vClName.SetName("@��������");
// 	m_vInfName.SetName("@��Ϣ����");
// 	m_vInfWarnDesc.SetName("@�쳣״̬����");
// 	m_vHisStAmount.SetName("@��ʷ״̬������");
// 	m_vHisEvtAmount.SetName("@��ʷ�¼�������");
// 	m_vUnStSecs.SetName("@δ����״̬ʱ��");
// 	
// 	m_vSubNo.SetType(VAL_INT);
// 	m_vDevSn.SetType(VAL_INT);
// 	m_vDevName.SetType(VAL_STRING);
// 	m_vProtType.SetType(VAL_INT);
// 	m_vClName.SetType(VAL_STRING);
// 	m_vInfName.SetType(VAL_STRING);
// 	m_vInfWarnDesc.SetType(VAL_STRING);
// 	m_vHisStAmount.SetType(VAL_INT);
// 	m_vHisEvtAmount.SetType(VAL_INT);
// 	m_vUnStSecs.SetType(VAL_INT);
	
	int i,cnt;
	cnt = GetExtFunctionCount();
	for(i=0;i<cnt;i++)
	{
		const stuExtFunction *p = GetExtFunctionByIdx(i);
		this->RegisterFunction(p->sFunName,p->pFun,this);
	}
// 	RegisterInstVariant(&m_vSubNo);
// 	RegisterInstVariant(&m_vDevSn);
// 	RegisterInstVariant(&m_vDevName);
// 	RegisterInstVariant(&m_vProtType);
// 	RegisterInstVariant(&m_vClName);
// 	RegisterInstVariant(&m_vInfName);
// 	RegisterInstVariant(&m_vInfWarnDesc);
// 	RegisterInstVariant(&m_vHisStAmount);
// 	RegisterInstVariant(&m_vHisEvtAmount);
// 	RegisterInstVariant(&m_vUnStSecs);

}

CSimScriptParser::~CSimScriptParser()
{

}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��չ����������
// ��    ��:  �ۿ���
// ����ʱ��:  2016-12-9 10:52
// ����˵��:  void
// �� �� ֵ:  ������Ч��չ����������
//////////////////////////////////////////////////////////////////////////
int CSimScriptParser::GetExtFunctionCount()
{
	return (sizeof(g_nam_eval_ext_fun)/sizeof(stuExtFunction))-1;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ����ŵ���չ����ָ��
// ��    ��:  �ۿ���
// ����ʱ��:  2016-12-9 10:53
// ����˵��:  @idxΪ��ţ���0��ʼ
// �� �� ֵ:  stuExtFunction*, NULL��ʾ������
//////////////////////////////////////////////////////////////////////////
const CSimScriptParser::stuExtFunction* CSimScriptParser::GetExtFunctionByIdx(int idx)
{
	if(idx < 0 || idx >= (sizeof(g_nam_eval_ext_fun)/sizeof(stuExtFunction))-1)
		return NULL;
	return &g_nam_eval_ext_fun[idx];
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡ��չ���������� 
// ��    ��:  �ۿ���
// ����ʱ��:  2016-12-9 17:18
// ����˵��:  void
// �� �� ֵ:  ������Ч��չ����������
//////////////////////////////////////////////////////////////////////////
int CSimScriptParser::GetExtVariantCount()
{
	return (sizeof(g_nam_eval_ext_var)/sizeof(stuExtVariant))-1;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ȡָ����ŵ���չ������ָ��
// ��    ��:  �ۿ���
// ����ʱ��:  2016-12-9 17:18
// ����˵��:  @idxΪ��ţ���0��ʼ
// �� �� ֵ:  stuExtVariant*�� NULL��ʾ������
//////////////////////////////////////////////////////////////////////////
const CSimScriptParser::stuExtVariant* CSimScriptParser::GetExtVariantByIdx(int idx)
{
	if(idx < 0 || idx >= (sizeof(g_nam_eval_ext_var)/sizeof(stuExtVariant))-1)
		return NULL;
	return &g_nam_eval_ext_var[idx];
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����GOOSE��λ
// ��    ��:  �ۿ���
// ����ʱ��:  2017-03-27 16:30
// ����˵��:  @pCbParam��ʾ�ص�����(ע��ʱָ����)
//         :  @ReturnVal��ʾ����ֵ��������
//         :  @pParamValues��ʽ�����б�ָ�룬NULL��ʾû���βΣ������������÷�ʽ
// �� �� ֵ:  true��ʾ���гɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool CSimScriptParser::ExtFun_GooseChg(void *pCbParam,SScriptParser::stuValue &ReturnVal,SPtrList<SScriptParser::stuValue> *pParamValues)
{
	CSimScriptParser *pThis = (CSimScriptParser*)pCbParam;
	if(pThis->m_pSimManager == 0 || pParamValues == NULL || pParamValues->count()<2)
	{
		ReturnVal.Set(VAL_BOOL,"0");
		return false;
	}
	bool ret = pThis->m_pSimManager->DoGooseChg(pParamValues->at(0)->GetValue().toInt(), pParamValues->at(1)->GetValue().data());
	ReturnVal.Set(VAL_BOOL,ret?"1":"0");
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����SV�仯
// ��    ��:  �ۿ���
// ����ʱ��:  2017-03-27 16:30
// ����˵��:  @pCbParam��ʾ�ص�����(ע��ʱָ����)
//         :  @ReturnVal��ʾ����ֵ��������
//         :  @pParamValues��ʽ�����б�ָ�룬NULL��ʾû���βΣ������������÷�ʽ
// �� �� ֵ:  true��ʾ���гɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool CSimScriptParser::ExtFun_SvChg(void *pCbParam,SScriptParser::stuValue &ReturnVal,SPtrList<SScriptParser::stuValue> *pParamValues)
{
	CSimScriptParser *pThis = (CSimScriptParser*)pCbParam;
	if(pThis->m_pSimManager == 0 || pParamValues == NULL || pParamValues->count()<4)
	{
		ReturnVal.Set(VAL_BOOL,"0");
		return false;
	}
	bool ret = pThis->m_pSimManager->DoSvChg(  pParamValues->at(0)->GetValue().toInt(), pParamValues->at(1)->GetValue().toInt(), 
										pParamValues->at(2)->GetValue().toFloat(), pParamValues->at(3)->GetValue().toInt() );
	ReturnVal.Set(VAL_BOOL,ret?"1":"0");
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����Ҷ�ӽڵ�
// ��    ��:  �ۿ���
// ����ʱ��:  2017-03-27 16:30
// ����˵��:  @pCbParam��ʾ�ص�����(ע��ʱָ����)
//         :  @ReturnVal��ʾ����ֵ��������
//         :  @pParamValues��ʽ�����б�ָ�룬NULL��ʾû���βΣ������������÷�ʽ
// �� �� ֵ:  true��ʾ���гɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
bool CSimScriptParser::ExtFun_SetLeaf(void *pCbParam,SScriptParser::stuValue &ReturnVal,SPtrList<SScriptParser::stuValue> *pParamValues)
{
	CSimScriptParser *pThis = (CSimScriptParser*)pCbParam;
	if(pThis->m_pSimManager == 0 || pParamValues == NULL || pParamValues->count()<4)
	{
		ReturnVal.Set(VAL_BOOL,"0");
		return false;
	}
	bool ret = pThis->m_pSimManager->DoSetLeaf(  pParamValues->at(0)->GetValue().data(), pParamValues->at(1)->GetValue().data(), 
												 pParamValues->at(2)->GetValue().toInt(), pParamValues->at(3)->GetValue().data() );
	ReturnVal.Set(VAL_BOOL,ret?"1":"0");
	return true;
}

bool CSimScriptParser::ExtFun_ReportProc(void *pCbParam,SScriptParser::stuValue &ReturnVal,SPtrList<SScriptParser::stuValue> *pParamValues)
{
	CSimScriptParser *pThis = (CSimScriptParser*)pCbParam;
	if(pThis->m_pSimManager == 0 )
	{
		ReturnVal.Set(VAL_BOOL,"0");
		return false;
	}
	pThis->m_pSimManager->m_MmsServer.NewReportProc();
	ReturnVal.Set(VAL_BOOL,"1");
	return true;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��ǰ��վ����ʷ״̬��ͳ��
// ��    ��:  �ۿ���
// ����ʱ��:  2016-12-8 15:40
// ����˵��:  @pCbParam��ʾ�ص�����(ע��ʱָ����)
//         :  @ReturnVal��ʾ����ֵ��������
//         :  @pParamValues��ʽ�����б�ָ�룬NULL��ʾû���βΣ������������÷�ʽ
// �� �� ֵ:  true��ʾ���гɹ���false��ʾ����ʧ��
//////////////////////////////////////////////////////////////////////////
// bool CSimScriptParser::ExtFun_HisStateCnt(void *pCbParam,SScriptParser::stuValue &ReturnVal,SPtrList<SScriptParser::stuValue> *pParamValues)
// {
// 	CSimScriptParser *pThis = (CSimScriptParser*)pCbParam;
// 	SString sql;
// 	int soc_from;
// 	int soc_to = pThis->m_iBaseSoc;
// 	if(pThis->m_iEvalType == 1)
// 		soc_from = soc_to-300;
// 	else if(pThis->m_iEvalType == 2)
// 		soc_from = soc_to-3600;
// 	else if(pThis->m_iEvalType == 3)
// 		soc_from = soc_to-3600*24;
// 	else
// 	{
// 		ReturnVal.Set(VAL_BOOL,"0");
// 		return false;
// 	}
// 	SString sFrom = SDateTime::makeDateTime((time_t)soc_from).toString("yyyy-MM-dd hh:mm:ss");
// 	SString sTo = SDateTime::makeDateTime((time_t)soc_to).toString("yyyy-MM-dd hh:mm:ss");
// 	LOGDEBUG("���ڼ�����վ%dͳ������[%s ~ %s]�ڵ���ʷ״̬����Ϣ...",
// 		pThis->m_iSubNo,sFrom.data(),sTo.data());
// 	sql.sprintf("select v.dev_sn,v.dev_name,v.cl_name,v.inf_name,v.off_desc,h.cnt,v.prot_type from ( "
// 		"select stat_id,count(*)+sum(repeat_cnt) as cnt from t_nam_his_element_state "
// 		"where sub_no=%d and val=0 and rectime>=%s and  rectime<%s "
// 		"group by stat_id) h "
// 		"left join v_nam_real_state v on h.stat_id=v.stat_id",
// 		pThis->m_iSubNo,pThis->m_pEvalMgr->m_db_multi_sql.String2Date(sFrom).data(),pThis->m_pEvalMgr->m_db_multi_sql.String2Date(sTo).data());
// 	DB->Retrieve(sql,pThis->m_rsHisState);
// 	LOGDEBUG("��վ%dͳ������[%s ~ %s]�ڵ���ʷ״̬������Ϊ%d",pThis->m_iSubNo,sFrom.data(),sTo.data(),pThis->m_rsHisState.GetRows());
// 	pThis->m_iCurrHisStateRow = -1;
// 	pThis->m_pCurrHisStateRow = NULL;
// 	ReturnVal.Set(VAL_BOOL,"1");
// 	return true;
// }
