/**
 *
 * �� �� �� : sp_unit_checkself.h
 * �������� : 2015-7-21 13:09
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ��Ԫ�Լ���Ϣ���弰���÷�����װ
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-21	�ۿ�������ļ�
 *
 **/

#ifndef __SP_UNIT_CHECK_SELF_H__
#define __SP_UNIT_CHECK_SELF_H__

#include "sp_unit_config.h"
#include "ssp_base_inc.h"

enum eUnitChkSelfAnalogId
{
	CS_ANALOG_CPU_USED=1,		//CPU������
	CS_ANALOG_ALLDISK=2,		//�洢����������
	CS_ANALOG_ALLDISK_USED=3,	//�洢������������
								//�洢����X����Y������
								//�洢����X����Y������
								//����洢��X����Y������
								//����洢��X����Y������
	CS_ANALOG_TEMP=501,			//װ���¶�
	CS_ANALOG_HUMI=502,			//װ��ʪ��
	CS_ANALOG_PWR1=503,			//װ�õ�Դ��ƽ1
	CS_ANALOG_PWR2=504,			//װ�õ�Դ��ƽ2
	CS_ANALOG_PWR3=505,			//װ�õ�Դ��ƽ3
	CS_ANALOG_PWR4=506,			//װ�õ�Դ��ƽ4
								//���X�Ĺ⹦��
								//װ��Ӳ��X���¶�

};

//////////////////////////////////////////////////////////////////////////
// ��    ��:  stuUnitCheckSelfAnalog
// ��    ��:  �ۿ���
// ����ʱ��:  2015-7-21 13:15
// ��    ��:  ��Ԫ�Լ���Ϣ��ģ����
//////////////////////////////////////////////////////////////////////////
struct SSP_BASE_EXPORT stuUnitCheckSelfAnalog
{
	stuUnitCheckSelfAnalog(int id, char* desc,int vt,char* dime,stuHwCheckSelfInfo *pMonitor=NULL)
	{
		m_iInfoId = id;
		strcpy(m_sInfoDesc, desc);
		m_iInfoValType = vt;
		strcpy(m_sInfoDime, dime);
		m_Value.iVal = 0;
		if(pMonitor != NULL)
			memcpy(&m_Monitor,pMonitor,sizeof(m_Monitor));
		else
		{
			m_Monitor.m_bOpen = false;
		}
	}
	int m_iInfoId;					//ģ������ţ���1��ʼ��ͬ��Ϣ��̶����
	char m_sInfoDesc[32];			//ģ������������
	int m_iInfoValType;				//ģ����ֵ���ͣ�0��ʾ��������1��ʾ����
	char m_sInfoDime[8];			//ģ����ֵ��λ
	union {
		float fVal;
		int iVal;
	}m_Value;						//ģ������ǰֵ
	stuHwCheckSelfInfo m_Monitor;	//��������
};

struct SSP_BASE_EXPORT stuUnitCheckSelfState
{
	stuUnitCheckSelfState(int id,char* desc,int state=0)
	{
		m_iInfoId = id;
		strcpy(m_sInfoDesc, desc);
		m_iState = state;
	}
	int m_iInfoId;					//״̬�����,��1��ʼ��ͬ��Ϣ��̶����
	char m_sInfoDesc[32];			//״̬����������
	int m_iState;					//״̬��1��ʾ������Ӧ�Ķ���������0��ʾ��������
};



#endif//__SP_UNIT_CHECK_SELF_H__
