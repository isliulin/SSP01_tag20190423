/**
 *
 * �� �� �� : SimComm.h
 * �������� : 2018-8-21 17:20
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ������ͨ�Ż�������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2018-8-21	�ۿ�������ļ�
 *
 **/

#ifndef __SIM_COMMUNICATION_H__
#define __SIM_COMMUNICATION_H__

enum eSimPackageType
{
	PKG_REGISTER_REQ=1,
	PKG_REGISTER_RES=2,
	PKG_ABILITY_REQ=3,
	PKG_ABILITY_RES=4,
	PKG_START_REQ=5,
	PKG_START_RES=6,
	PKG_STOP_REQ=7,
	PKG_STOP_RES=8,
	PKG_JOB_REQ=9,
	PKG_JOB_RES=10,

	PKG_RUN_FUN_REQ=21,
	PKG_RUN_FUN_RES=22,
};

struct stuSimAbility
{
	int ctrl_id;//֧�ֵĿ��Ʊ��
	char ctrl_name[32];//��������
	int param_num;//������Ŀ
	char param_names[128];//���������б���ͬ�����÷ֺż��
};



#endif//__SIM_COMMUNICATION_H__
