/**
 *
 * �� �� �� : UKQtRccBuilder.cpp
 * �������� : 2017-10-10 10:55
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2017-10-10	�ۿ�������ļ�
 *
 **/

#include "UKQtRccBuilder.h"
#include "SXmlConfig.h"
#include <QString>
#include <QChar>
#include <QHash>

CUkQtRccBuilder::CUkQtRccBuilder()
{
	m_pRccBuffer = NULL;
	m_iRccBufferLen = 0;
}

CUkQtRccBuilder::~CUkQtRccBuilder()
{

}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ��qrc�ļ�����RCC
// ��    ��:  �ۿ���
// ����ʱ��:  2017-10-10 11:15
// ����˵��:  qrc_fileΪqrc�����ļ���
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CUkQtRccBuilder::LoadByQrc(SString qrc_file)
{
	SXmlConfig xml;
	if(!xml.ReadConfig(qrc_file))
		return false;
	SBaseConfig *qresource = xml.SearchChild("qresource");
	if(qresource == NULL)
		return false;
	SetLang(qresource->GetAttribute("lang"));
	SetPrefix(qresource->GetAttribute("prefix"));
	unsigned long pos;
	SString alias,filename;
	SBaseConfig *file = qresource->GetChildPtr()->FetchFirst(pos);
	while(file)
	{
		alias = file->GetAttribute("alias");
		filename = file->GetNodeValue();
		if(alias.length() == 0)
			alias = filename;
		if(!AddImage(alias,filename))
			return false;
		file = qresource->GetChildPtr()->FetchNext(pos);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �ӱ����ļ����һ��ͼƬ
// ��    ��:  �ۿ���
// ����ʱ��:  2017-10-10 11:06
// ����˵��:  @aliasΪͼƬ�ļ�����
//         :  @sFileNameΪ�����ļ�����
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CUkQtRccBuilder::AddImage(SString alias,SString sFileName)
{
	SFile f(sFileName);
	if(!f.open(IO_ReadOnly))
		return false;
	int len = f.size();
	if(len == 0)
		return false;
	unsigned char *buf = new unsigned char[len+1];
	if(f.readBlock(buf,len) != len)
	{
		delete[] buf;
		return false;
	}
	AddImage(alias,buf,len,true);
	f.close();
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  �ӻ��������һ��ͼƬ
// ��    ��:  �ۿ���
// ����ʱ��:  2017-10-10 11:06
// ����˵��:  @aliasΪͼƬ�ļ�����
//         :  @bufΪ�ļ�������ָ�� 
//         :  @lenΪ�ļ����ݳ���
//         :  @clone_buf��ʾ�Ƿ���Ҫ�ͷ�ָ���Ļ�����
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CUkQtRccBuilder::AddImage(SString alias,unsigned char *buf,int len,bool free_buf/*=true*/)
{
	if(len <= 0)
		return false;
	unsigned long pos;
	stuFile *p = m_Files.FetchFirst(pos);
	while(p)
	{
		if(p->img_alias == alias)
			return false;
		p = m_Files.FetchNext(pos);
	}
// 	if(m_prefix.length() > 0)
// 		alias = m_prefix+"/"+alias;
	m_Files.append(new stuFile(alias,buf,len,free_buf));
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����RCC���ݵ�������
// ��    ��:  �ۿ���
// ����ʱ��:  2017-10-10 11:13
// ����˵��:  void
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CUkQtRccBuilder::SaveRccToBuffer()
{
	if(m_Files.count() == 0)
		return false;
	if(m_pRccBuffer != NULL)
		delete[] m_pRccBuffer;
	while(m_prefix.left(1) == "/")
		m_prefix = m_prefix.mid(1);

	//���㳤��
	unsigned long p1;
	stuFile *pFile;
	m_iRccBufferLen = 20+(2+4+m_prefix.length()*2)+14*2;

	//����HASH��ֵ��������
	SPtrList<stuFile> files;
	files.setAutoDelete(false);

	while(m_Files.count() > 0)
	{
		QString s1,s2;
		stuFile *pMin = pFile = m_Files.FetchFirst(p1);
		pFile = m_Files.FetchNext(p1);
		while(pFile)
		{
			s1 = pMin->img_alias.data();
			s2 = pFile->img_alias.data();
			if( qHash(s2) < qHash(s1))
				pMin = pFile;
			pFile = m_Files.FetchNext(p1);
		}
		files.append(pMin);
		m_Files.remove(pMin);
	}
	files.copyto(m_Files);

	pFile = m_Files.FetchFirst(p1);
	while(pFile)
	{
		m_iRccBufferLen += 4 + pFile->img_len;
		m_iRccBufferLen += 2 + 4 + pFile->img_alias.length()*2;
		m_iRccBufferLen += 14;
		pFile = m_Files.FetchNext(p1);
	}
	m_pRccBuffer = new unsigned char[m_iRccBufferLen+32];
	memset(m_pRccBuffer,0,m_iRccBufferLen+32);
	int pos = 0;
	//write header
	//qres
	//int * 4;
	int i32 = 0;
	int i16;

	//////////////////////////////////////////////////////////////////////////
	//header 
	write(m_pRccBuffer,pos,(void*)"qres",4);
	write(m_pRccBuffer,pos,&i32,4);
	write(m_pRccBuffer,pos,&i32,4);
	write(m_pRccBuffer,pos,&i32,4);
	write(m_pRccBuffer,pos,&i32,4);

	//////////////////////////////////////////////////////////////////////////
	//data 
	int datasOffset = pos;
	pFile = m_Files.FetchFirst(p1);
	while(pFile)
	{
		pFile->dataOffset = pos - datasOffset;
		i32 = htonl(pFile->img_len);
		write(m_pRccBuffer,pos,&i32,4);
		write(m_pRccBuffer,pos,pFile->img_buf,pFile->img_len);
		pFile = m_Files.FetchNext(p1);
	}

	//////////////////////////////////////////////////////////////////////////
	//names
	int namesOffset = pos;
	//root
	{
		i16 = ntohs(m_prefix.length());
		write(m_pRccBuffer,pos,&i16,2);
		QString name = m_prefix.data();
		i32 = htonl(qHash(name));
		write(m_pRccBuffer,pos,&i32,4);
		const QChar *unicode = name.unicode();
		for (int i = 0; i < name.length(); ++i)
		{
			i16 = htons(unicode[i].unicode());
			write(m_pRccBuffer,pos,&i16,2);
		}
	}
	pFile = m_Files.FetchFirst(p1);
	while(pFile)
	{
		pFile->nameOffset = pos - namesOffset;
		i16 = ntohs(pFile->img_alias.length());
		write(m_pRccBuffer,pos,&i16,2);
		QString name = pFile->img_alias.data();
		i32 = htonl(qHash(name));
		write(m_pRccBuffer,pos,&i32,4);
		const QChar *unicode = name.unicode();
		for (int i = 0; i < name.length(); ++i)
		{
			i16 = htons(unicode[i].unicode());
			write(m_pRccBuffer,pos,&i16,2);
		}
		pFile = m_Files.FetchNext(p1);
	}

	//////////////////////////////////////////////////////////////////////////
	//struct tree
	int treeOffset = pos;
	int idx = 0;
	//root
	{
		i32 = htonl(0/*namesOffset*/);
		write(m_pRccBuffer,pos,&i32,4);//nameOffset
		i16 = htons(02);
		write(m_pRccBuffer,pos,&i16,2);//flags
		i32 = htonl(1);
		write(m_pRccBuffer,pos,&i32,4);//child count
		i32 = htonl(1);//TODO
		write(m_pRccBuffer,pos,&i32,4);//first child offset
	}
	//prefix
	{
		i32 = htonl(0/*namesOffset*/);
		write(m_pRccBuffer,pos,&i32,4);//nameOffset
		i16 = htons(02);
		write(m_pRccBuffer,pos,&i16,2);//flags
		i32 = htonl(m_Files.count());
		write(m_pRccBuffer,pos,&i32,4);//child count
		i32 = htonl(2);//TODO
		write(m_pRccBuffer,pos,&i32,4);//first child offset
	}
	
	pFile = m_Files.FetchFirst(p1);
	while(pFile)
	{
		i32 = htonl(pFile->nameOffset);
		write(m_pRccBuffer,pos,&i32,4);//nameOffset
		i16 = 0;
		write(m_pRccBuffer,pos,&i16,2);//flags
		write(m_pRccBuffer,pos,&i16,2);//country
		i16 = htons(1);
		write(m_pRccBuffer,pos,&i16,2);//language

		i32 = htonl(pFile->dataOffset);
		write(m_pRccBuffer,pos,&i32,4);//dataoffset
		pFile = m_Files.FetchNext(p1);
	}

	//init header
	((int*)m_pRccBuffer)[1] = htonl(1);
	((int*)m_pRccBuffer)[2] = htonl(treeOffset);
	((int*)m_pRccBuffer)[3] = htonl(datasOffset);
	((int*)m_pRccBuffer)[4] = htonl(namesOffset);
	return true;
}

void CUkQtRccBuilder::write(unsigned char* pBuf,int &pos,void* pBlockBuf,int iBlockLen)
{
	memcpy(pBuf+pos,pBlockBuf,iBlockLen);
	pos += iBlockLen;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����RCC���ݵ������ļ�
// ��    ��:  �ۿ���
// ����ʱ��:  2017-10-10 11:13
// ����˵��:  sPathFileΪ����RCC�ļ���ȫ·��
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CUkQtRccBuilder::SaveRccToFile(SString sPathFile)
{
	if(!SaveRccToBuffer())
		return false;
	SFile f(sPathFile);
	if(!f.open(IO_Truncate))
		return false;
	if(f.writeBlock(m_pRccBuffer,m_iRccBufferLen) != m_iRccBufferLen)
		return false;
	return true;
}
