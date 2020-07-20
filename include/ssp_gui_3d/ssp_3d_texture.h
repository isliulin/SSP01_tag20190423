/**
 *
 * �� �� �� : ssp_3d_texture.h
 * �������� : 2017-3-1 18:46
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : 3D������ض���
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2017-3-1	�ۿ�������ļ�
 *
 **/

#ifndef __SSP_3D_TEXTURE_H__
#define __SSP_3D_TEXTURE_H__

#include "SString.h"
#include "SList.h"
#include "ssp_3d_inc.h"
#include <QGLWidget>

class SSP_GUI_3D_EXPORT CSsp3DTexture
{
public:

	CSsp3DTexture(t_ssp_3d_texture *p=NULL);
	~CSsp3DTexture();

	void SetGlWidget(QGLWidget *pWnd){m_pGlWidget = pWnd;};

	bool LoadTexture(bool bLoadTexture = true);
	bool RemoveTexture();
	bool LoadRGBTexture(SString path);
	bool LoadRGBATexture(SString path);
	bool LoadColorTexture();
	unsigned char* LoadBMPFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader,unsigned char *backgroundColor,bool bAlpha = false);

	t_ssp_3d_texture m_record;
	QGLWidget *m_pGlWidget;
	GLuint m_Texture;
};



#endif//__SSP_3D_TEXTURE_H__
