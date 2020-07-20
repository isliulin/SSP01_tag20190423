#include "dlgselectviewpoint.h"
#include <QMessageBox>

#define COLUMN_SCENE_ID		0
#define COLUMN_VP_ID		1
#define COLUMN_NAME			2

DlgSelectViewPoint::DlgSelectViewPoint(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	init();

	connect(ui.pushButton_ok,SIGNAL(clicked()),this,SLOT(slotOk()));
	connect(ui.pushButton_cancel,SIGNAL(clicked()),this,SLOT(close()));
	connect(ui.tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slotItemClicked(QTableWidgetItem*)));
}

DlgSelectViewPoint::~DlgSelectViewPoint()
{

}

void DlgSelectViewPoint::init()
{
	row = 0;
	initTableWidget();
}

void DlgSelectViewPoint::initTableWidget()
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
}

void DlgSelectViewPoint::start()
{
	CSsp3DScene *m_scene = m_3DModel->SearchScene(m_sceneId);
	if (!m_scene)
		return;

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

				item = new QTableWidgetItem(tr("%1").arg(p->vpoint_name));
				item->setTextAlignment(Qt::AlignCenter);
				item->setFlags(item->flags() & (~Qt::ItemIsEditable));
				ui.tableWidget->setItem(row-1,COLUMN_NAME,item);

				p = v->m_ViewPoints.FetchNext(pos1);
			}
		}
		v = m_3DModel->m_SceneViewPoints.FetchNext(pos);
	}

	m_sceneId = 0;
	m_viewPointId = 0;
}

void DlgSelectViewPoint::slotItemClicked(QTableWidgetItem *item)
{
	m_sceneId = ui.tableWidget->item(item->row(),COLUMN_SCENE_ID)->text().toInt();
	m_viewPointId = ui.tableWidget->item(item->row(),COLUMN_VP_ID)->text().toInt();
}

void DlgSelectViewPoint::slotOk()
{
	if (!m_sceneId || !m_viewPointId)
	{
		QMessageBox::warning(this,tr("�澯"),tr("��ѡ����Ч���ӵ㡣"));
		return;
	}

	accept();
}
