/**
 *
 * �� �� �� : ssp_3d_predefine.h
 * �������� : 2017-3-2 8:56
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : Ԥ��������װ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2017-3-2	�ۿ�������ļ�
 *
 **/

#ifndef __SSP_3D_PRE_DEFINE_H__
#define __SSP_3D_PRE_DEFINE_H__

#include "SString.h"
#include "SList.h"
#include "ssp_3d_inc.h"
#include "ssp_3d_texture.h"


class SSP_GUI_3D_EXPORT CSsp3DPreDefineObj
{
public:
	CSsp3DPreDefineObj(t_ssp_3d_predefine_obj *p=NULL);
	~CSsp3DPreDefineObj();

	t_ssp_3d_predefine_obj m_record;
	CSsp3DTexture *m_pTexture;

public:
	bool LoadTexture();

public:
	bool LoadRGBATexture(SString path);
	unsigned char* LoadBMPFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader,unsigned char *backgroundColor,bool bAlpha = false);
};

class SSP_GUI_3D_EXPORT CSsp3DPreDefine
{
public:
	CSsp3DPreDefine(t_ssp_3d_predefine *p=NULL);
	~CSsp3DPreDefine();
	inline CSsp3DPreDefineObj* SearchObj(int obj_id)
	{
		unsigned long pos;
		CSsp3DPreDefineObj *p = m_PreObjs.FetchFirst(pos);
		while(p)
		{
			if(p->m_record.obj_id == obj_id)
				return p;
			p = m_PreObjs.FetchNext(pos);
		}
		return NULL;
	}

	t_ssp_3d_predefine m_record;
	SPtrList<CSsp3DPreDefineObj> m_PreObjs;
};

#endif//__SSP_3D_PRE_DEFINE_H__
