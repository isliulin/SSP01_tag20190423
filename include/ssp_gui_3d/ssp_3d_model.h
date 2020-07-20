/**
 *
 * �� �� �� : ssp_3d_model.h
 * �������� : 2017-3-3 15:08
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : SSP 3Dģ��������װ
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2017-3-3	�ۿ�������ļ�
 *
 **/

#ifndef __SSP_3D_MODEL_H__
#define __SSP_3D_MODEL_H__

#include "ssp_base.h"
#include "ssp_3d_mdb.h"
#include "ssp_3d_scene.h"
#include "ssp_3d_predefine.h"
#include "ssp_3d_texture.h"
#include "ssp_3d_viewpoint.h"
#include "ssp_3d_vrml.h"
#include <QGLWidget>

class SSP_GUI_3D_EXPORT CSsp3DModel
{
public:
	CSsp3DModel();
	~CSsp3DModel();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ͨ�����ݿ���س�ʼģ��
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-3 15:12
	// ��    ��:  @pDbΪ���ݿ������ָ��
	//////////////////////////////////////////////////////////////////////////
	bool LoadByDb(SDatabaseOper *pDb,bool bLoadTexture = true,bool vboUse = false);

	void SetGlWidget(QGLWidget *pWnd){m_pGlWidget = pWnd;};

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���������Ų���ָ��������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-1 19:12
	// ��    ��:  CSsp3DTexture* ��NULL��ʾ�Ҳ���
	//////////////////////////////////////////////////////////////////////////
	inline CSsp3DTexture* SearchTexture(int tex_id)
	{
		unsigned long pos;
		CSsp3DTexture *p = m_Texures.FetchFirst(pos);
		while(p)
		{
			if(p->m_record.texture_id == tex_id)
				return p;
			p = m_Texures.FetchNext(pos);
		}
		return NULL;
	}
	inline CSsp3DScene* SearchScene(int scene_id)
	{
		unsigned long pos;
		CSsp3DScene *p = m_Scenes.FetchFirst(pos);
		while(p)
		{
			if(p->m_record.scene_id == scene_id)
				return p;
			p = m_Scenes.FetchNext(pos);
		}
		return NULL;
	}
	inline CSsp3DPreDefine* SearchPreDefine(int pre_id)
	{
		unsigned long pos;
		CSsp3DPreDefine *p = m_PreDefines.FetchFirst(pos);
		while(p)
		{
			if(p->m_record.pre_id == pre_id)
				return p;
			p = m_PreDefines.FetchNext(pos);
		}
		return NULL;
	}
	inline CSsp3DSceneTypeViewPoint* SearchSceneTypeVP(int scene_type)
	{
		unsigned long pos;
		CSsp3DSceneTypeViewPoint *p = m_SceneTypeViewPoints.FetchFirst(pos);
		while(p)
		{
			if(p->m_iSceneType == scene_type)
				return p;
			p = m_SceneTypeViewPoints.FetchNext(pos);
		}
		return NULL;
	}
	inline CSsp3DSceneViewPoint* SearchSceneVP(int scene_id)
	{
		unsigned long pos;
		CSsp3DSceneViewPoint *p = m_SceneViewPoints.FetchFirst(pos);
		while(p)
		{
			if(p->m_iSceneId == scene_id)
				return p;
			p = m_SceneViewPoints.FetchNext(pos);
		}
		return NULL;
	}
	inline CSsp3DVrml* SearchVrml(int rvml_id)
	{
		unsigned long pos;
		CSsp3DVrml *p = m_Vrmls.FetchFirst(pos);
		while(p)
		{
			if(p->m_record.vrml_id == rvml_id)
				return p;
			p = m_Vrmls.FetchNext(pos);
		}
		return NULL;
	}
protected:

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ����ڴ����ݿ�ʱ�ļ�¼���»ص���������������¼���¡����롢ɾ���ͱ�ض�
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-3 15:25
	// ����˵��:  @cbParamΪ��ǰ��¼���Ķ���ָ��
	//         :  @sTableΪ����
	//         :  @eTypeΪ��������
	//         :  @iTrgRowsΪ��������
	//         :  @iRowSizeΪÿ�еļ�¼����
	//         :  @pTrgDataΪ�����������ݣ�����Ч����ΪiTrgRows*iRowSize
	// �� �� ֵ:  NULL
	//////////////////////////////////////////////////////////////////////////
	static BYTE* OnMdbTrgCallback(void* cbParam, SString &sTable, eMdbTriggerType eType,int iTrgRows,int iRowSize,BYTE *pTrgData);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ݿ�仯������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-3 17:09
	// ����˵��:  @pRowΪ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void OnInsertTexture(t_ssp_3d_texture *pRow,bool bLoadTexture = true);
	void OnUpdateTexture(t_ssp_3d_texture *pRow);
	void OnDeleteTexture(t_ssp_3d_texture *pRow);
	void OnTruncateTexture();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  Ԥ�����������ݿ�仯������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-3 17:09
	// ����˵��:  @pRowΪ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void OnInsertPreDefine(t_ssp_3d_predefine *pRow);
	void OnUpdatePreDefine(t_ssp_3d_predefine *pRow);
	void OnDeletePreDefine(t_ssp_3d_predefine *pRow);
	void OnTruncatePreDefine();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  Ԥ����������ݿ�仯������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-3 17:09
	// ����˵��:  @pRowΪ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void OnInsertPreDefineObj(t_ssp_3d_predefine_obj *pRow);
	void OnUpdatePreDefineObj(t_ssp_3d_predefine_obj *pRow);
	void OnDeletePreDefineObj(t_ssp_3d_predefine_obj *pRow);
	void OnTruncatePreDefineObj();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �������ݿ�仯������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-3 17:09
	// ����˵��:  @pRowΪ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void OnInsertScene(t_ssp_3d_scene *pRow);
	void OnUpdateScene(t_ssp_3d_scene *pRow);
	void OnDeleteScene(t_ssp_3d_scene *pRow);
	void OnTruncateScene();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����������ݿ�仯������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-3 17:09
	// ����˵��:  @pRowΪ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void OnInsertSceneObj(t_ssp_3d_scene_object *pRow);
	void OnUpdateSceneObj(t_ssp_3d_scene_object *pRow);
	void OnDeleteSceneObj(t_ssp_3d_scene_object *pRow);
	void OnTruncateSceneObj();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ���������ӵ����ݿ�仯������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-3 17:09
	// ����˵��:  @pRowΪ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void OnInsertSceneTypeVP(t_ssp_3d_scene_type_viewpoint *pRow);
	void OnUpdateSceneTypeVP(t_ssp_3d_scene_type_viewpoint *pRow);
	void OnDeleteSceneTypeVP(t_ssp_3d_scene_type_viewpoint *pRow);
	void OnTruncateSceneTypeVP();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  �����ӵ����ݿ�仯������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-3 17:09
	// ����˵��:  @pRowΪ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void OnInsertSceneVP(t_ssp_3d_scene_viewpoint *pRow);
	void OnUpdateSceneVP(t_ssp_3d_scene_viewpoint *pRow);
	void OnDeleteSceneVP(t_ssp_3d_scene_viewpoint *pRow);
	void OnTruncateSceneVP();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  VRML���ݿ�仯������
	// ��    ��:  �ۿ���
	// ����ʱ��:  2017-3-3 17:09
	// ����˵��:  @pRowΪ������
	// �� �� ֵ:  void
	//////////////////////////////////////////////////////////////////////////
	void OnInsertVrml(t_ssp_3d_vrml *pRow,bool bLoadVrml,bool vboUse);
	void OnUpdateVrml(t_ssp_3d_vrml *pRow,bool bLoadVrml,bool vboUse);
	void OnDeleteVrml(t_ssp_3d_vrml *pRow);
	void OnTruncateVrml();

public:
	//������������
	SPtrList<CSsp3DScene> m_Scenes;
	//3D������
	SPtrList<CSsp3DTexture> m_Texures;
	//Ԥ�������弯��
	SPtrList<CSsp3DPreDefine> m_PreDefines;
	//���������ӵ㼯��
	SPtrList<CSsp3DSceneTypeViewPoint> m_SceneTypeViewPoints;
	//�����ӵ㼯��
	SPtrList<CSsp3DSceneViewPoint> m_SceneViewPoints;
	//VRML����
	SPtrList<CSsp3DVrml> m_Vrmls;

private:
	//�ڴ��ص��õĿͻ���ʵ��
	CMdbClient *m_pMdbClient;
	//��ǰ������ڹ������߳�����
	int m_iThreads;
	//ģ�ͷ����������ڶ�дģ�ͻ���
	SLock m_Lock;

	QGLWidget *m_pGlWidget;
	CSsp3DObjectMgr m_ObjectMgr;//�������������
};

#endif//__SSP_3D_MODEL_H__
