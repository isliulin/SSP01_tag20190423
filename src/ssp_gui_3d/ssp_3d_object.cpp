/**
 *
 * �� �� �� : ssp_3d_object.cpp
 * �������� : 2017-3-7 14:05
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : ����ID��ʶ�ඨ��
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2017-3-7	�ۿ�������ļ�
 *
 **/

#include "ssp_3d_object.h"

CSsp3DObject::CSsp3DObject(int sn,int id1,int id2,int id3,int id4)
{
	m_iObjSn = sn;
	m_iId1 = id1;
	m_iId2 = id2;
	m_iId3 = id3;
	m_iId4 = id4;
}

CSsp3DObject::~CSsp3DObject()
{

}


CSsp3DObjectMgr::CSsp3DObjectMgr()
{
	m_lst3DObj.setAutoDelete(true);
	m_iLastObjSn = 1;
}

CSsp3DObjectMgr::~CSsp3DObjectMgr()
{

}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ��Ψһ��ſ��ٲ��Ҷ���
// ��    ��:  �ۿ���
// ����ʱ��:  2017-3-7 14:22
// ����˵��:  @obj_sn
// �� �� ֵ:  CSsp3DObject*
//////////////////////////////////////////////////////////////////////////
CSsp3DObject* CSsp3DObjectMgr::SearchObjectBySn(int obj_sn)
{
	SSP3D_MAP_OBJECT_SN::iterator it = m_map3DObj.find(obj_sn);
	if(it == m_map3DObj.end())
	{
		return NULL;
	}
	return it->second;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ͨ��ģ��ӳ���õ�4��IDֵ���Ҷ���
// ��    ��:  �ۿ���
// ����ʱ��:  2017-3-7 14:23
// ����˵��:  @id1/id2/id3/id4
// �� �� ֵ:  CSsp3DObject*
//////////////////////////////////////////////////////////////////////////
CSsp3DObject* CSsp3DObjectMgr::SearchObjectById(int id1,int id2,int id3,int id4)
{
	unsigned long pos;
	CSsp3DObject *p = m_lst3DObj.FetchFirst(pos);
	while(p)
	{
		if(p->m_iId1 == id1 || p->m_iId2 == id2 || p->m_iId3 == id3 || p->m_iId4 == id4)
			return p;
		p = m_lst3DObj.FetchNext(pos);
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ����һ���µĶ��󣬲����ض���ָ��
// ��    ��:  �ۿ���
// ����ʱ��:  2017-3-7 14:24
// ����˵��:  @id1/id2/id3/id4
// �� �� ֵ:  CSsp3DObject*
//////////////////////////////////////////////////////////////////////////
CSsp3DObject* CSsp3DObjectMgr::NewObject(int id1,int id2,int id3,int id4)
{
	CSsp3DObject *pObj = SearchObjectById(id1,id2,id3,id4);
	if(pObj != NULL)
		return pObj;
	pObj = new CSsp3DObject(m_iLastObjSn,id1,id2,id3,id4);
	m_lst3DObj.append(pObj);
	m_map3DObj[m_iLastObjSn] = pObj;
	m_iLastObjSn ++;
	return pObj;
}

//////////////////////////////////////////////////////////////////////////
// ��    ��:  ɾ��һ��ָ����ʶ�Ķ���
// ��    ��:  �ۿ���
// ����ʱ��:  2017-3-7 14:39
// ����˵��:  @id1/id2/id3/id4
// �� �� ֵ:  true/false
//////////////////////////////////////////////////////////////////////////
bool CSsp3DObjectMgr::RemoveObject(int id1,int id2,int id3,int id4)
{
	CSsp3DObject *pObj = SearchObjectById(id1,id2,id3,id4);
	if(pObj == NULL)
		return false;
	m_map3DObj.erase(pObj->m_iObjSn);
	m_lst3DObj.remove(pObj);
	return true;
}


//////////////////////////////////////////////////////////////////////////
// ��    ��:  ������ж�������,������SN��
// ��    ��:  �ۿ���
// ����ʱ��:  2017-3-7 14:37
// ����˵��:  void
// �� �� ֵ:  void
//////////////////////////////////////////////////////////////////////////
void CSsp3DObjectMgr::Clear()
{
	m_map3DObj.clear();
	m_lst3DObj.clear();
	m_iLastObjSn = 1;
}
