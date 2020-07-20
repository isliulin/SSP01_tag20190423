/**
 *
 * �� �� �� : ssp_3d_object.h
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

#ifndef __SSP_3D_OBJECT_H__
#define __SSP_3D_OBJECT_H__

#include "SList.h"
#include "ssp_gui_3d_inc.h"
#include <map>

class SSP_GUI_3D_EXPORT CSsp3DObject
{
public:
	CSsp3DObject(int sn,int id1,int id2,int id3,int id4);
	~CSsp3DObject();

	int m_iObjSn;//����Ψһ���

	int m_iId1;//����ģ�Ͷ�Ӧ�ı��
	int m_iId2;
	int m_iId3;
	int m_iId4;
};

typedef std::map<int,CSsp3DObject*> SSP3D_MAP_OBJECT_SN;

class SSP_GUI_3D_EXPORT CSsp3DObjectMgr
{
public:
	CSsp3DObjectMgr();
	~CSsp3DObjectMgr();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ��Ψһ��ſ��ٲ��Ҷ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-7 14:22
	// ����˵��:  @obj_sn
	// �� �� ֵ:  CSsp3DObject*
	//////////////////////////////////////////////////////////////////////////
	CSsp3DObject* SearchObjectBySn(int obj_sn);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ��ģ��ӳ���õ�4��IDֵ���Ҷ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-7 14:23
	// ����˵��:  @id1/id2/id3/id4
	// �� �� ֵ:  CSsp3DObject*
	//////////////////////////////////////////////////////////////////////////
	CSsp3DObject* SearchObjectById(int id1,int id2,int id3,int id4);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����һ���µĶ��󣬲����ض���ָ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-7 14:24
	// ����˵��:  @id1/id2/id3/id4
	// �� �� ֵ:  CSsp3DObject*
	//////////////////////////////////////////////////////////////////////////
	CSsp3DObject* NewObject(int id1,int id2,int id3,int id4);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ɾ��һ��ָ����ʶ�Ķ���
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-7 14:39
	// ����˵��:  @id1/id2/id3/id4
	// �� �� ֵ:  true/false
	//////////////////////////////////////////////////////////////////////////
	bool RemoveObject(int id1,int id2,int id3,int id4);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������ж�������,������SN��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-7 14:37
	// ����˵��:  void
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void Clear();
private:
	SPtrList<CSsp3DObject> m_lst3DObj;
	SSP3D_MAP_OBJECT_SN m_map3DObj;
	int m_iLastObjSn;
};

#endif//__SSP_3D_OBJECT_H__
