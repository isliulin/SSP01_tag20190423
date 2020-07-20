#include "dlgdelviewpoint.h"
#include <QMessageBox>

#define COLUMN_SCENE_ID		0
#define COLUMN_VP_ID		1
#define COLUMN_DEFAULT		2
#define COLUMN_NAME			3

DlgDelViewPoint::DlgDelViewPoint(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	init();

	connect(ui.pushButton_del,SIGNAL(clicked()),this,SLOT(slotOk()));
	connect(ui.pushButton_cancel,SIGNAL(clicked()),this,SLOT(close()));
	connect(ui.tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slotItemClicked(QTableWidgetItem*)));
}

DlgDelViewPoint::~DlgDelViewPoint()
{

}

void DlgDelViewPoint::init()
{
	row = 0;
	initTableWidget();
}

void DlgDelViewPoint::initTableWidget()
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

void DlgDelViewPoint::start()
{
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
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				ui.tableWidget->setItem(row-1,COLUMN_NAME,item);

				p = v->m_ViewPoints.FetchNext(pos1);
			}
		}
		v = m_3DModel->m_SceneViewPoints.FetchNext(pos);
	}
}

void DlgDelViewPoint::slotItemClicked(QTableWidgetItem *item)
{
	m_sceneId = ui.tableWidget->item(item->row(),COLUMN_SCENE_ID)->text().toInt();
	m_viewPointId = ui.tableWidget->item(item->row(),COLUMN_VP_ID)->text().toInt();
}

void DlgDelViewPoint::slotOk()
{
	SString sql;
	sql.sprintf("delete from t_ssp_3d_scene_viewpoint where scene_id=%d and vpoint_id=%d",m_sceneId,m_viewPointId);
	if (!DB->ExecuteSQL(sql))
	{
		QMessageBox::warning(this,tr("�澯"),tr("ʵʱ��ɾ���ӵ�ʧ�ܡ�"));
		return;
	}
	else
	{
		QMessageBox::information(this,tr("��ʾ"),tr("ʵʱ��ɾ���ӵ�ɹ���"));
		if (!MDB->ExecuteSQL(sql))
			QMessageBox::information(this,tr("��ʾ"),tr("�ڴ��ɾ���ӵ�ʧ�ܡ�"));
	}

	accept();
}
