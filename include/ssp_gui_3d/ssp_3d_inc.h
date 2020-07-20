/**
 *
 * �� �� �� : ssp_3d_inc.h
 * �������� : 2017-3-2 10:35
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SSP 3D ��������ͷ�ļ�
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2017-3-2	�ۿ�������ļ�
 *
 **/

#ifndef __SSP_3D_INCLUDE_H__
#define __SSP_3D_INCLUDE_H__

#include "ssp_3d_mdb.h"
#include "ssp_3d_object.h"
#include "ssp_gui_3d_inc.h"

enum eSsp3DPickActionMode
{
	SSP_3D_PICK_MOD_NONE=0,		//0�������ڳ���ʰȡ
	SSP_3D_PICK_MOD_MOVE=1,		//1���л��ӽǵ��ӳ������ӵ�
	SSP_3D_PICK_MOD_INTO=2,		//2�������ӳ�����ʾ
	SSP_3D_PICK_MOD_CALLBACK=3,	//3���ص�Ӧ�ò㴦��
};

enum eSsp3DPickHoverMode
{
	SSP_3D_PICK_HOVER_NONE=0,		//0���޶������
	SSP_3D_PICK_HOVER_BOX=1,		//1�����ư�͸��ʰȡ����
	SSP_3D_PICK_HOVER_NAME=2,		//2�����λ�û���ʰȡ��������
	SSP_3D_PICK_HOVER_CALLBACK=3,	//3���ص�Ӧ�ò㴦��
};


enum eSsp3DDrawType
{
	SSP_3D_DRAW_LINE=1,		//1-ֱ�߶�
	SSP_3D_DRAW_RECT=2,		//2-ƽ�����
	SSP_3D_DRAW_TRIANGLE=3,	//3-ƽ��������
	SSP_3D_DRAW_CIRCULAR=4,	//4-ƽ��Բ�� 
	SSP_3D_DRAW_TEXT=5,		//5-ƽ������
	SSP_3D_DRAW_BOX=6,		//6-������
	SSP_3D_DRAW_CYLINDER=7,	//7-Բ����
	SSP_3D_DRAW_CONE=8,		//8-Բ׶��
	SSP_3D_DRAW_BALL=9,		//9-����
	SSP_3D_DRAW_PREDEF=101,	//101-Ԥ�������
	SSP_3D_DRAW_CALLBACK=102,//102-ʵʱ�ص���������
};

enum eSsp3DTextureType
{
	SSP_3D_TEXTURE_BMP=1,
	SSP_3D_TEXTURE_COLOR=2,
};



#endif//__SSP_3D_INCLUDE_H__
