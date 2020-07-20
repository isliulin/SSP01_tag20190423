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

#include "ssp_3d_scene.h"
#include "SApplication.h"

#define BITMAP_ID		0x4D42

CSsp3DScene::CSsp3DScene(t_ssp_3d_scene *p/*=NULL*/)
{
	if(p != NULL)
		memcpy(&m_record,p,sizeof(m_record));
	else
		memset(&m_record,0,sizeof(m_record));
	m_pParentScene = NULL;
	m_p3DObject = NULL;
	m_Objs.setAutoDelete(true);
}

CSsp3DScene::~CSsp3DScene()
{
	
}

CSsp3DSceneObj::CSsp3DSceneObj(t_ssp_3d_scene_object *p/*=NULL*/)
{
	if(p != NULL)
		memcpy(&m_record,p,sizeof(m_record));
	else
		memset(&m_record,0,sizeof(m_record));
	m_pTexture = NULL;
	m_p3DObject = NULL;
	color = "";
}

CSsp3DSceneObj::~CSsp3DSceneObj()
{
	if(m_record.draw_type == 5)
	{
		GLuint m_Texures = m_record.texture_id;
		glDeleteTextures(1,&m_Texures);
	}
}

bool CSsp3DSceneObj::LoadTexture()
{
	SString sPath = SBASE_SAPP->GetHomePath()+"textures\\"+m_record.obj_name+".bmp";
	if (!LoadRGBATexture(sPath))
		return false;

	return true;
}

bool CSsp3DSceneObj::LoadRGBATexture(SString path)
{
	BITMAPINFOHEADER bitmapInfoHeader;    
	unsigned char*   bitmapData = NULL;
	unsigned char color[3] = { 0xc8,0xc8,0xc8 };

	bitmapData = LoadBMPFile(path.data(),&bitmapInfoHeader,color,true);
	if (!bitmapData)
		return false;

	GLuint m_Texture;
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
	m_record.texture_id = m_Texture;

	return true;
}

unsigned char* CSsp3DSceneObj::LoadBMPFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader,unsigned char *backgroundColor,bool bAlpha)
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

	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);      
	if (bitmapImage == NULL) 
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
			bitmapData[count + 3] = 0;  
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
