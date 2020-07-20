#include "dlgsaveviewpoint.h"
#include <QMessageBox>

#define COLUMN_SCENE_ID		0
#define COLUMN_VP_ID		1
#define COLUMN_DEFAULT		2
#define COLUMN_NAME			3

DlgSaveViewPoint::DlgSaveViewPoint(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	init();

	connect(ui.pushButton_ok,SIGNAL(clicked()),this,SLOT(slotOk()));
	connect(ui.pushButton_cancel,SIGNAL(clicked()),this,SLOT(close()));
	connect(ui.radioButton_add,SIGNAL(clicked()),this,SLOT(slotRadioButtonAdd()));
	connect(ui.radioButton_replace,SIGNAL(clicked()),this,SLOT(slotRadioButtonReplace()));
	connect(ui.tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slotItemClicked(QTableWidgetItem*)));
}

DlgSaveViewPoint::~DlgSaveViewPoint()
{

}

void DlgSaveViewPoint::init()
{
	row = 0;
	initTableWidget();
	
	ui.lineEdit->setEnabled(ui.radioButton_add->isChecked());
	ui.tableWidget->setEnabled(ui.radioButton_replace->isChecked());
}

void DlgSaveViewPoint::initTableWidget()
{
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);		//����ѡ��ģʽ
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);		//��ѡģʽ
	ui.tableWidget->setFocusPolicy(Qt::NoFocus);								//ȥ�����㣬�����
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);			//���ó�������
	ui.tableWidget->setStyleSheet("selection-background-color:lightblue;");		//����ѡ�б���ɫ
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(25);				//�����и�
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);			//�����ʱ���Ա�ͷ�й���
	ui.tableWidget->setAlternatingRowColors(true);								//���ý�����ɫ
	ui.tableWidget->verticalHeader()->setVisible(false);						//ȥ����ǰ��

	ui.tableWidget->setColumnWidth(COLUMN_SCENE_ID,80);
	ui.tableWidget->setColumnWidth(COLUMN_VP_ID,80);
	ui.tableWidget->setColumnWidth(COLUMN_DEFAULT,80);
}

void DlgSaveViewPoint::start()
{
	setDefaultEnable();

	unsigned long pos = 0;
	CSsp3DSceneViewPoint *v = m_3DModel->m_SceneViewPoints.FetchFirst(pos);
	while (v)
	{
		if (v->m_iSceneId == m_sceneId)
		{
			unsigned long pos1 = 0;
			t_ssp_3d_scene_viewpoint *p = v->m_ViewPoints.FetchFirst(pos1);
			while (p)
			{
				ui.tableWidget->setRowCount(++row);

				QTableWidgetItem *item = new QTableWidgetItem(tr("%1").arg(p->scene_id));
				item->setTextAlignment(Qt::AlignCenter);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				ui.tableWidget->setItem(row-1,COLUMN_SCENE_ID,item);

				item = new QTableWidgetItem(tr("%1").arg(p->vpoint_id));
				item->setTextAlignment(Qt::AlignCenter);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				ui.tableWidget->setItem(row-1,COLUMN_VP_ID,item);

				item = new QTableWidgetItem(tr("%1").arg(p->def_vpoint == 1 ? tr("Ĭ��") : tr("��Ĭ��")));
				item->setTextAlignment(Qt::AlignCenter);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				ui.tableWidget->setItem(row-1,COLUMN_DEFAULT,item);

				item = new QTableWidgetItem(tr("%1").arg(p->vpoint_name));
				item->setTextAlignment(Qt::AlignCenter);
				//item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				ui.tableWidget->setItem(row-1,COLUMN_NAME,item);

				p = v->m_ViewPoints.FetchNext(pos1);
			}
		}
		v = m_3DModel->m_SceneViewPoints.FetchNext(pos);
	}

	m_viewPointId = 0;
	m_viewPointDefault = 0;
}

void DlgSaveViewPoint::slotItemClicked(QTableWidgetItem *item)
{
	m_sceneId = ui.tableWidget->item(item->row(),COLUMN_SCENE_ID)->text().toInt();
	m_viewPointId = ui.tableWidget->item(item->row(),COLUMN_VP_ID)->text().toInt();
	m_viewPointDefault = ui.tableWidget->item(item->row(),COLUMN_DEFAULT)->text() == tr("Ĭ��") ? 1 : 0;
	m_viewPointName = ui.tableWidget->item(item->row(),COLUMN_NAME)->text().toStdString().data();
}

void DlgSaveViewPoint::slotOk()
{
	if (ui.radioButton_add->isChecked())
	{
		if (ui.lineEdit->text().isEmpty())
		{
			QMessageBox::warning(this,tr("�澯"),tr("�������ӵ����ơ�"));
			return;
		}

		int maxVPId = getMaxViewPointId();
		if (maxVPId == -1)
			return;

		SString sql;
		sql.sprintf("insert into t_ssp_3d_scene_viewpoint"
			"(scene_id,vpoint_id,vpoint_name,def_vpoint,eye_x,eye_y,eye_z,eye_forw_x,eye_forw_y,eye_forw_z,cent2eye,x_angle,z_angle) values"
			"(%d,%d,'%s',%d,%f,%f,%f,%f,%f,%f,%f,%f,%f)",m_sceneId,++maxVPId,ui.lineEdit->text().toStdString().data(),m_viewPointDefault,
			eyeX,eyeY,eyeZ,centerX,centerY,centerZ,eye2center,xAngle,zAngle);
		if (!DB->ExecuteSQL(sql))
		{
			QMessageBox::warning(this,tr("�澯"),tr("ʵʱ�������ӵ�ʧ�ܡ�"));
			return;
		}
		else
		{
			QMessageBox::information(this,tr("��ʾ"),tr("ʵʱ�������ӵ�ɹ���"));
			if (!MDB->ExecuteSQL(sql))
				QMessageBox::information(this,tr("��ʾ"),tr("�ڴ�������ӵ�ʧ�ܡ�"));
		}
	}
	else if (ui.radioButton_replace->isChecked())
	{
		if (!m_sceneId || !m_viewPointId)
		{
			QMessageBox::warning(this,tr("�澯"),tr("��ѡ����Ҫ�滻���ӵ㡣"));
			return;
		}

		SString sql;
		sql.sprintf("update t_ssp_3d_scene_viewpoint set "
			"vpoint_name='%s',eye_x=%f,eye_y=%f,eye_z=%f,eye_forw_x=%f,eye_forw_y=%f,eye_forw_z=%f,cent2eye=%f,x_angle=%f,z_angle=%f "
			"where scene_id=%d and vpoint_id=%d and def_vpoint=%d",m_viewPointName.data(),eyeX,eyeY,eyeZ,centerX,centerY,centerZ,eye2center,xAngle,zAngle,
			m_sceneId,m_viewPointId,m_viewPointDefault);
		if (!DB->ExecuteSQL(sql))
		{
			QMessageBox::warning(this,tr("�澯"),tr("ʵʱ���滻�ӵ�ʧ�ܡ�"));
			return;
		}
		else
		{
			QMessageBox::information(this,tr("��ʾ"),tr("ʵʱ���滻�ӵ�ɹ���"));
			if (!MDB->ExecuteSQL(sql))
				QMessageBox::information(this,tr("��ʾ"),tr("�ڴ���滻�ӵ�ʧ�ܡ�"));
		}
	}

	accept();
}

void DlgSaveViewPoint::slotRadioButtonAdd()
{
	ui.lineEdit->setEnabled(true);
	ui.tableWidget->setEnabled(false);
	setDefaultEnable();
}

void DlgSaveViewPoint::slotRadioButtonReplace()
{
	ui.lineEdit->setEnabled(false);
	ui.checkBox->setEnabled(false);
	ui.tableWidget->setEnabled(true);
}

void DlgSaveViewPoint::setDefaultEnable()
{
	SRecordset rs;
	int cnt = DB->Retrieve(SString::toFormat("select count(*) from t_ssp_3d_scene_viewpoint where scene_id=%d and def_vpoint=1",m_sceneId),rs);
	if (cnt <= 0)
	{
		ui.pushButton_ok->setEnabled(false);
		return;
	}
	else
	{
		int count = rs.GetValue(0,0).toInt();
		if (count)
			ui.checkBox->setEnabled(false);
		else
			ui.checkBox->setEnabled(true);
	}
}

int DlgSaveViewPoint::getMaxViewPointId()
{
	SRecordset rs;
	int cnt = DB->Retrieve(SString::toFormat("select max(vpoint_id) from t_ssp_3d_scene_viewpoint where scene_id=%d",m_sceneId),rs);
	if (cnt <= 0)
	{
		QMessageBox::warning(this,tr("�澯"),tr("��ȡ����������ӵ���ʧ�ܡ�"));
		return -1;
	}

	return rs.GetValue(0,0).toInt();
}
