/**
 *
 * �� �� �� : ssp_3d_scene.h
 * �������� : 2017-3-2 9:48
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : 3D������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2017-3-2	�ۿ�������ļ�
 *
 **/

#ifndef __SSP_3D_SCENE_H__
#define __SSP_3D_SCENE_H__

#include "SString.h"
#include "SList.h"
#include "ssp_3d_inc.h"
#include "ssp_3d_predefine.h"
#include "ssp_3d_texture.h"



class SSP_GUI_3D_EXPORT CSsp3DSceneObj
{
public:
	CSsp3DSceneObj(t_ssp_3d_scene_object *p=NULL);
	~CSsp3DSceneObj();

	t_ssp_3d_scene_object m_record;
	CSsp3DTexture *m_pTexture;
	CSsp3DObject *m_p3DObject;//3D���������ָ�룬NULL��ʾ����������

	SString color;

public:
	bool LoadTexture();

public:
	bool LoadRGBATexture(SString path);
	unsigned char* LoadBMPFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader,unsigned char *backgroundColor,bool bAlpha = false);
};

class SSP_GUI_3D_EXPORT CSsp3DScene
{
public:
	CSsp3DScene(t_ssp_3d_scene *p=NULL);
	~CSsp3DScene();
	inline CSsp3DSceneObj* SearchObj(int obj_id)
	{
		unsigned long pos;
		CSsp3DSceneObj *p = m_Objs.FetchFirst(pos);
		while(p)
		{
			if(p->m_record.obj_id == obj_id)
				return p;
			p = m_Objs.FetchNext(pos);
		}
		return NULL;
	}

	t_ssp_3d_scene m_record;
	/*
	int m_iSceneId;
	int m_iPSceneId;
	SString m_sSceneName;
	int m_iSceneType;
	SString m_sSceneDesc;
	int m_iId1;
	int m_iId2;
	int m_iId3;
	int m_iId4;
	float m_fCenter[3];
	float m_fRotate[3];
	float m_fScale;
	float m_fEyeRange[3][2];//x/y/z��Ӧ����С�����ֵ
	*/

	//
	CSsp3DScene *m_pParentScene;
	CSsp3DObject *m_p3DObject;//3D���������ָ�룬NULL��ʾ����������
	SPtrList<CSsp3DSceneObj> m_Objs;
};


#endif//__SSP_3D_SCENE_H__
