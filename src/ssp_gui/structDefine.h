/**
 *
 *����Զ������ݽṹ
 *
 **/

#ifndef _STRUCT_DEFINE_H_
#define _STRUCT_DEFINE_H_

struct tableWidgetInfo
{
	int m_iFormerRow;//ԭ������
	int m_iInsertRow;//��������
	int m_iColumn;//����
	int m_iKey;//��������
	bool m_bInsert;//�Ƿ�����
};
struct fieldInfo//�ֶ���Ϣ
{
	SString m_sName;//�ֶ���
	SString m_sType;//�ֶ�����
	int m_iLen;//�ֶγ���
	int m_iPoint;//���㳤�� �Ǹ�������Ϊ0
	SString m_sReg;//ƥ���ֶ�ֵ��������ʽ
};

#endif