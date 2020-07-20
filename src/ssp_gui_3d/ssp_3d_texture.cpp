/**
 *
 * �� �� �� : ssp_3d_texture.cpp
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

#include "ssp_3d_texture.h"
#include "SApplication.h"

#define BMP_TYPE		1
#define COLOR_TYPE		2
#define BITMAP_ID		0x4D42

CSsp3DTexture::CSsp3DTexture(t_ssp_3d_texture *p/*=NULL*/)
{
	m_pGlWidget = NULL;
	if(p == NULL)
		memset(&m_record,0,sizeof(m_record));
	else
		memcpy(&m_record,p,sizeof(m_record));
	m_Texture = 0;
}

CSsp3DTexture::~CSsp3DTexture()
{
	RemoveTexture();
}

bool CSsp3DTexture::LoadTexture(bool bLoadTexture)
{
	if(m_pGlWidget == NULL)
		return false;
	if(m_Texture != 0)
		RemoveTexture();
	SString sPath = SBASE_SAPP->GetHomePath()+"textures\\"+m_record.tex_filename;

	if(bLoadTexture && !DB->ReadLobToFile("t_ssp_3d_texture","tex_image",SString::toFormat("texture_id=%d",m_record.texture_id),sPath))
	{
		LOGERROR("���������ļ�%d-%sʧ��!",m_record.texture_id,m_record.tex_filename);
		return false;
	}

	if (m_record.tex_type == BMP_TYPE)
	{
		if (m_record.tex_color[0] == 0x00)
		{
			if (!LoadRGBTexture(sPath))
				return false;
		}
		else if (!LoadRGBATexture(sPath))
			return false;

	}
	else if (m_record.tex_type == COLOR_TYPE)
	{
		if (!LoadColorTexture())
			return false;
	}
	else 
	{
		LOGERROR("�������[%d]������[%d]����Ҫ��Χ[1-2]��!",m_record.texture_id,m_record.tex_type);
		return false;
	}

	return true;
}

bool CSsp3DTexture::LoadRGBTexture(SString path)
{
	QString s = QString("%1").arg(path.data());
	QImage image(s);
	if (!image.width())
		return false;

	image = image.convertToFormat(QImage::Format_RGB888);
	image = image.mirrored();

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);      
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.bits());

	return true;
}

bool CSsp3DTexture::LoadRGBATexture(SString path)
{
	BITMAPINFOHEADER bitmapInfoHeader;    
	unsigned char*   bitmapData = NULL;
	unsigned char color[3] = { 0,0,0 };

	if (m_record.tex_color[0] == 0)
	{
		bitmapData = LoadBMPFile(path.data(),&bitmapInfoHeader,color);
	}
	else
	{
		QColor c(QObject::tr("%1").arg(m_record.tex_color));
		color[0] = c.red(); 
		color[1] = c.green(); 
		color[2] = c.blue();
		bitmapData = LoadBMPFile(path.data(),&bitmapInfoHeader,color,true);
	}
	if (!bitmapData)
		return false;
	
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);      
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
	glTexImage2D(GL_TEXTURE_2D,  
				 0,							//mipmap���(ͨ��Ϊ0����ʾ���ϲ�)       
				 GL_RGBA,					//�졢�̡�����alpha����      
				 bitmapInfoHeader.biWidth,	//��������������n�����б߿�+2       
				 bitmapInfoHeader.biHeight, //����߶ȣ�������n�����б߿�+2       
				 0,							//�߿�(0=�ޱ߿�, 1=�б߿�)       
				 GL_RGBA,					//bitmap���ݵĸ�ʽ      
				 GL_UNSIGNED_BYTE,			//ÿ����ɫ���ݵ�����      
				 bitmapData);				//bitmap����ָ�� 
	delete bitmapData;
	return true;
}

unsigned char* CSsp3DTexture::LoadBMPFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader,unsigned char *backgroundColor,bool bAlpha)
{
	FILE *filePtr;
	int imageIdx = 0;   
	BITMAPFILEHEADER bitmapFileHeader;    //bitmap�ļ�ͷ      
	unsigned char    *bitmapImage;        //bitmapͼ������    

	filePtr = fopen(filename, "rb");  
	if (filePtr == NULL) 
	{  
		LOGERROR("�����ļ�[%s]��ʧ�ܡ�",filename);
		return NULL;  
	}  
    
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);        
	if (bitmapFileHeader.bfType != BITMAP_ID) 
	{  
		LOGERROR("�����ļ�[%s]����BMP�ļ���",filename);
		return NULL;  
	}  
	     
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (bitmapInfoHeader->biSizeImage == 0 || bitmapInfoHeader->biSizeImage+54 != bitmapFileHeader.bfSize)
		bitmapInfoHeader->biSizeImage = bitmapFileHeader.bfSize-54;

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);       
	bitmapImage = new unsigned char[bitmapInfoHeader->biSizeImage];     
	if (!bitmapImage) 
	{  
		LOGERROR("���������ļ�[%s]ʧ�ܣ�ԭ���ڴ治�㡣",filename); 
		return NULL;  
	}  
    
	int ret = fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);      
	if (ret != bitmapInfoHeader->biSizeImage) 
	{  
		delete[] bitmapImage;
		LOGERROR("��ȡ�����ļ�[%s]ʧ�ܣ�ԭ���ڴ治�㡣",filename); 
		return NULL;  
	} 

	unsigned char* bitmapData = NULL;
	bitmapData = new unsigned char[bitmapInfoHeader->biSizeImage / 3 * 4+32];  
	if (!bitmapData) 
	{  
		delete[] bitmapImage;
		LOGERROR("����alpha����[%s]�ڴ�ʧ�ܣ�ԭ���ڴ治�㡣",filename); 
		return NULL;  
	}

	int count = 0;  
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx += 3) 
	{  
		bitmapData[count] = bitmapImage[imageIdx + 2];  
		bitmapData[count + 1] = bitmapImage[imageIdx + 1];  
		bitmapData[count + 2] = bitmapImage[imageIdx];  
		if (bAlpha && bitmapData[count] == backgroundColor[0] && bitmapData[count + 1] == backgroundColor[1] && bitmapData[count + 2] == backgroundColor[2])
		{  
			bitmapData[count + 3] = 30;  
		}
		else 
		{
			bitmapData[count + 3] = 255; 
		}

		count += 4;  
	}  
    
	delete bitmapImage;
	fclose(filePtr);

	return bitmapData;  
}

bool CSsp3DTexture::LoadColorTexture()
{
	return true;
}

bool CSsp3DTexture::RemoveTexture()
{
	if(m_pGlWidget == NULL)
		return false;

	if(m_Texture != 0)
		glDeleteTextures(1,&m_Texture);

	m_Texture = 0;
	return true;
}
