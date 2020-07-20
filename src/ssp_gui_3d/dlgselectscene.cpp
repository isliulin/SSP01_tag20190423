#include "dlgselectscene.h"
#include <QMessageBox>

#define COLUMN_NO		0
#define COLUMN_NAME		1

DlgSelectScene::DlgSelectScene(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	init();

	connect(ui.pushButton_ok,SIGNAL(clicked()),this,SLOT(slotOk()));
	connect(ui.pushButton_cancel,SIGNAL(clicked()),this,SLOT(close()));
	connect(ui.tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(slotItemClicked(QTableWidgetItem*)));
}

DlgSelectScene::~DlgSelectScene()
{

}

void DlgSelectScene::init()
{
	row = 0;
	initTableWidget();
}

void DlgSelectScene::initTableWidget()
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

	ui.tableWidget->setColumnWidth(COLUMN_NO,100);
}

void DlgSelectScene::start()
{
	CSsp3DScene *m_scene = m_3DModel->SearchScene(m_sceneId);
	if (!m_scene)
		return;

	unsigned long pos = 0;
	CSsp3DScene *s = m_3DModel->m_Scenes.FetchFirst(pos);
	while(s)
	{
		if ((m_sceneType == SCENE_TYPE_EQUAL && s->m_record.p_scene_id == m_scene->m_record.p_scene_id) ||
			(m_sceneType == SCENE_TYPE_CHILD && s->m_record.p_scene_id == m_scene->m_record.scene_id))
		{
			ui.tableWidget->setRowCount(++row);

			QTableWidgetItem *item = new QTableWidgetItem(tr("%1").arg(s->m_record.scene_id));
			item->setTextAlignment(Qt::AlignCenter);
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(row-1,COLUMN_NO,item);

			item = new QTableWidgetItem(tr("%1").arg(s->m_record.scene_name));
			item->setTextAlignment(Qt::AlignCenter);
			item->setFlags(item->flags() & (~Qt::ItemIsEditable));
			ui.tableWidget->setItem(row-1,COLUMN_NAME,item);
		}
		s = m_3DModel->m_Scenes.FetchNext(pos);
	}

	m_sceneId = 0;
}

void DlgSelectScene::slotItemClicked(QTableWidgetItem *item)
{
	m_sceneId = ui.tableWidget->item(item->row(),COLUMN_NO)->text().toInt();
}

void DlgSelectScene::slotOk()
{
	if (!m_sceneId)
	{
		QMessageBox::warning(this,tr("�澯"),tr("��ѡ����Ч�ĳ�����"));
		return;
	}

	accept();
}
