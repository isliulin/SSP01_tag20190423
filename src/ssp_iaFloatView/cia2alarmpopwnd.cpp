#include "cia2alarmpopwnd.h"
#include "ssp_database.h"
#include "ssp_iafloatview.h"
#include "ssp_gui.h"
#include "dlgmessage.h"
#include "dlgconfigure.h"

#define COLUMN_NUMBER			5

#define COLUMN_TIME				0
#define COLUMN_SEVERITY			1
#define COLUMN_CONTENT			2
#define COLUMN_DETAIL			3
#define COLUMN_CONFIRM			4

#define COLUMN_SCROLL_WIDTH		10
#define COLUMN_TIME_WIDTH		150
#define COLUMN_SEVERITY_WIDTH	60
#define COLUMN_DETAIL_WIDTH		60
#define COLUMN_CONFIRM_WIDTH	60

#define WND_INIT_WIDTH			900
#define WND_INIT_HIGHT			600
#define WND_EXT_WIDTH			250
#define VERTICAL_SPACING		2

#define CONFIRM_COLOR			"#cdcdcd"

#define TREE_TYPE_SUB			1
#define TREE_TYPE_BAY			2

CIa2AlarmPopWnd::CIa2AlarmPopWnd(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ssp_floatwnd = (ssp_iaFloatView *)parent;

	setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Tool);
	resize(QSize(WND_INIT_WIDTH,WND_INIT_HIGHT));
	setMinimumSize(WND_INIT_WIDTH,WND_INIT_HIGHT);
	
	Init();
	InitMenu();
	InitTableWidget();
	InitTreeWidget();

	connect(ui.tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(SlotTableWidgetItemClicked(QTableWidgetItem*)));
	connect(ui.tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(SlotTableWidgetItemDoubleClicked(QTableWidgetItem*)));
	connect(ui.tableWidget,SIGNAL(customContextMenuRequested(const QPoint)),this,SLOT(SlotTableWidgetClickedRightMenu(const QPoint)));

	connect(ui.treeWidget,SIGNAL(itemChanged(QTreeWidgetItem *,int)),this,SLOT(slotTreeWidgetItemChanged(QTreeWidgetItem *,int)));

	connect(ui.btn_all,SIGNAL(clicked()),this,SLOT(SlotBtnAllClicked()));
	connect(ui.btn_critrial,SIGNAL(clicked()),this,SLOT(SlotBtnCritrialClicked()));
	connect(ui.btn_important,SIGNAL(clicked()),this,SLOT(SlotBtnImportantClicked()));
	connect(ui.btn_secondary,SIGNAL(clicked()),this,SLOT(SlotBtnSecondaryClicked()));
	connect(ui.btn_general,SIGNAL(clicked()),this,SLOT(SlotBtnGeneralClicked()));
	connect(ui.btn_extend,SIGNAL(clicked()),this,SLOT(SlotBtnExtendClicked()));
	connect(ui.btn_setting,SIGNAL(clicked()),this,SLOT(SlotBtnSettingClicked()));
	connect(ui.btn_ok,SIGNAL(clicked()),this,SLOT(SlotBtnOkClicked()));
	connect(ui.btn_confirm_all,SIGNAL(clicked()),this,SLOT(SlotBtnConfirmAllClicked()));
	connect(ui.btn_start_stop,SIGNAL(clicked()),this,SLOT(SlotBtnStartStopClicked()));
	connect(ui.btn_save,SIGNAL(clicked()),this,SLOT(SlotBtnSaveClicked()));
	connect(ui.btn_query,SIGNAL(clicked()),this,SLOT(SlotBtnQueryClicked()));
	

	connect(m_actConfirm,SIGNAL(triggered()),this,SLOT(SlotActConfirm()));
	connect(m_actConfirmAll,SIGNAL(triggered()),this,SLOT(SlotActConfirmAll()));
	connect(m_actClear,SIGNAL(triggered()),this,SLOT(SlotActClear()));
	connect(m_actClearAll,SIGNAL(triggered()),this,SLOT(SlotActClearAll()));

	connect(m_iRestoreTimer,SIGNAL(timeout()),this,SLOT(SlotRestoreTimeout()));
}

CIa2AlarmPopWnd::~CIa2AlarmPopWnd()
{
	for (QMap<int,stuHisItem*>::const_iterator iter = m_mapHisItems.begin(); iter != m_mapHisItems.end(); ++iter)
		delete iter.value();

	m_mapSeveritys.clear();
	m_mapSubs.clear();
	m_mapBays.clear();
	m_mapHisItems.clear();

	delete m_itemDelegate;
}

void CIa2AlarmPopWnd::Init()
{
	m_itemDelegate = new ItemDelegate;
	m_iRestoreTimer = new QTimer(this);
	m_bIsExtend = false;
	m_bIsStart = true;
	ui.btn_extend->setToolTip(tr("����չ���ܴ�"));
	ui.widget_extend->setFixedWidth(WND_EXT_WIDTH);
	ui.widget_extend->setVisible(false);
	ui.tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	ui.gridLayout_main->setVerticalSpacing(VERTICAL_SPACING);

	SetPushButtonQss(ui.btn_all,5,8,1,"#000000","#FFFFFF","#000000","#646464","#FFFFFF","#646464","#FFFFFF");
	QString color = ssp_floatwnd->ReadSettings("/color/critical");
	SetPushButtonQss(ui.btn_critrial,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/important");
	SetPushButtonQss(ui.btn_important,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/secondary");
	SetPushButtonQss(ui.btn_secondary,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/general");
	SetPushButtonQss(ui.btn_general,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
}

void CIa2AlarmPopWnd::InitMenu()
{
	m_popMenu = new QMenu(ui.tableWidget);
	m_actConfirm = new QAction(QIcon(":/images/tag"),tr("ȷ��ѡ����Ϣ"),this);
	m_actConfirmAll = new QAction(QIcon(":/images/tag"),tr("ȷ��ȫ��δȷ����Ϣ"),this);
	m_actClear = new QAction(QIcon(":/images/coffee"),tr("���ѡ����Ϣ"),this);
	m_actClearAll = new QAction(QIcon(":/images/coffee"),tr("���ȫ����ȷ����Ϣ"),this);
	m_popMenu->addAction(m_actConfirm);
	m_popMenu->addAction(m_actConfirmAll);
	m_popMenu->addSeparator();
	m_popMenu->addAction(m_actClear);
	m_popMenu->addAction(m_actClearAll);

#ifdef WIN32
	m_popMenu->setStyleSheet(
		"QMenu{ background-color:white; border:1px solid rgb(214,214,214); }"
		"QMenu::item{ height:28px; padding:0px 20px 0px 40px; font-size:12px; color:rgb(0, 0, 0); font-family: Microsoft Yahei; }"
		"QMenu::item:focus{ padding: -1; }"
		"QMenu::item:selected:enabled{ background:rgb(22, 154, 243); color:white; }"
		"QMenu::item:selected:!enabled{ color:transparent; }"
		"QMenu::icon{ position:absolute;left: 12px; }"
		"QMenu::separator{ height:1px; background:rgb(209,209,209); margin:4px 0px 4px 0px; }");
#else
	m_popMenu->setStyleSheet(
		"QMenu{ background-color:white; border:1px solid rgb(214,214,214); }"
		"QMenu::item{ height:28px; padding:0px 20px 0px 40px; font-size:12px; color:rgb(0, 0, 0); font-family: ����; }"
		"QMenu::item:focus{ padding: -1; }"
		"QMenu::item:selected:enabled{ background:rgb(22, 154, 243); color:white; }"
		"QMenu::item:selected:!enabled{ color:transparent; }"
		"QMenu::icon{ position:absolute;left: 12px; }"
		"QMenu::separator{ height:1px; background:rgb(209,209,209); margin:4px 0px 4px 0px; }");
#endif
	m_popMenu->setContentsMargins(2,4,2,4);
}

void CIa2AlarmPopWnd::InitTableWidget()
{
	QStringList table_header;
	table_header << tr("ʱ��") << tr("����") << tr("�澯����") << tr("����") << tr("�Ƿ�ȷ��");
	ui.tableWidget->setColumnCount(COLUMN_NUMBER);
	ui.tableWidget->setHorizontalHeaderLabels(table_header);
	ui.tableWidget->setColumnWidth(COLUMN_TIME,COLUMN_TIME_WIDTH);
	ui.tableWidget->setColumnWidth(COLUMN_SEVERITY,COLUMN_SEVERITY_WIDTH);
	ui.tableWidget->setColumnWidth(COLUMN_CONTENT,this->width()-COLUMN_SCROLL_WIDTH-COLUMN_DETAIL_WIDTH-COLUMN_SEVERITY_WIDTH-COLUMN_TIME_WIDTH-COLUMN_CONFIRM_WIDTH);
	ui.tableWidget->setColumnWidth(COLUMN_DETAIL,COLUMN_DETAIL_WIDTH);
	ui.tableWidget->setColumnWidth(COLUMN_CONFIRM,COLUMN_CONFIRM_WIDTH);
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);		//����ѡ��ģʽ
	ui.tableWidget->setFocusPolicy(Qt::NoFocus);								//ȥ�����㣬�����
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(48);				//�����и�
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);			//�����ʱ���Ա�ͷ�й���
	ui.tableWidget->setStyleSheet("background-color:rgb(0,0,0);selection-background-color:rgb(80,80,80);");	//����ѡ�б���ɫ
	ui.tableWidget->setAlternatingRowColors(true);								//���ý�����ɫ
	ui.tableWidget->verticalHeader()->setVisible(false);						//ȥ����ǰ��
	ui.tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);		//�ɶ�ѡ��Ctrl��Shift��  Ctrl+A�����ԣ�
	ui.tableWidget->setFrameShape(QFrame::NoFrame);								//�����ޱ߿�
	ui.tableWidget->setShowGrid(false);											//���ò���ʾ������
	ui.tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //���ñ�ͷ����ɫ

	//����ˮƽ����ֱ��������ʽ
	ui.tableWidget->horizontalScrollBar()->setStyleSheet(
		"QScrollBar{background:transparent; height:10px;}"
		"QScrollBar::handle{background:gray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	ui.tableWidget->verticalScrollBar()->setStyleSheet(
		"QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:gray; border:20px solid transparent; border-radius:10px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");

	QStandardItemModel *model = (QStandardItemModel *)ui.tableWidget->model();
	ui.tableWidget->setItemDelegate(m_itemDelegate);
}

void CIa2AlarmPopWnd::InitTreeWidget()
{
	ui.treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);
	ui.treeWidget->header()->setStretchLastSection(false);

	//����ˮƽ����ֱ��������ʽ
	ui.treeWidget->horizontalScrollBar()->setStyleSheet(
		"QScrollBar{background:transparent; height:10px;}"
		"QScrollBar::handle{background:gray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
	ui.treeWidget->verticalScrollBar()->setStyleSheet(
		"QScrollBar{background:transparent; width: 10px;}"
		"QScrollBar::handle{background:gray; border:2px solid transparent; border-radius:5px;}"
		"QScrollBar::handle:hover{background:gray;}"
		"QScrollBar::sub-line{background:transparent;}"
		"QScrollBar::add-line{background:transparent;}");
}

void CIa2AlarmPopWnd::InitMap()
{
	SString sql;
	SRecordset rs;
	SRecordset rs1;

	sql.sprintf("select basesignal_severity,name from t_ia2_basesignal_severity");
	int cnt = DB->Retrieve(sql,rs);
	if (cnt > 0)
	{
		for (int i = 0; i < cnt; i++)
		{
			int basesignal_severity = rs.GetValue(i,0).toInt();
			QString name = rs.GetValue(i,1).data();
			m_mapSeveritys.insert(basesignal_severity,name);
		}
	}

	sql.sprintf("select sub_no,name from t_cim_substation order by name");
	cnt = DB->Retrieve(sql,rs);
	if (cnt > 0)
	{
		for (int i = 0; i < cnt; i++)
		{
			int sub_no = rs.GetValue(i,0).toInt();
			QString name = rs.GetValue(i,1).data();
			m_mapSubs.insert(sub_no,name);

			QTreeWidgetItem *subItem = new QTreeWidgetItem(ui.treeWidget,TREE_TYPE_SUB);
			subItem->setText(0,name);
			subItem->setData(0,Qt::UserRole,sub_no);
			subItem->setIcon(0,QIcon(":/images/home"));
			subItem->setCheckState(0,Qt::Checked);
			subItem->setExpanded(true);

			sql.sprintf("select bay_no,name from t_cim_bay where sub_no=%d order by name",sub_no);
			int cnt1 = DB->Retrieve(sql,rs1);
			if (cnt1 > 0)
			{
				for (int i = 0; i < cnt1; i++)
				{
					int bay_no = rs1.GetValue(i,0).toInt();
					QString name = rs1.GetValue(i,1).data();
					m_mapBays.insert(bay_no,name);
					m_mapFilterBays.insert(bay_no,name);

					QTreeWidgetItem * bayItem = new QTreeWidgetItem(subItem,TREE_TYPE_BAY);
					bayItem->setText(0,name);
					bayItem->setData(0,Qt::UserRole,bay_no);
					bayItem->setIcon(0,QIcon(":/images/data"));
					bayItem->setCheckState(0,Qt::Checked);
				}
			}
		}
	}

	unsigned int lastSn = 0;
	sql.sprintf("select ia_his,sub_no,bay_no,act_soc,act_usec,ia_conclusion,rule_severity,is_confirm,confirm_user,confirm_soc from t_ia2_history");
	cnt = DB->Retrieve(sql,rs);
	if (cnt > 0)
	{
		for (int i = 0; i < cnt; i++)
		{
			stuHisItem *item = new stuHisItem;
			item->sn =  rs.GetValue(i,0).toInt();
			item->subNo = rs.GetValue(i,1).toInt();
			item->bayNo = rs.GetValue(i,2).toInt();
			item->soc = rs.GetValue(i,3).toInt();
			item->usec = rs.GetValue(i,4).toInt();
			item->conclusion = rs.GetValue(i,5).data();
			item->severity = rs.GetValue(i,6).toInt();
			item->isConfirm = rs.GetValue(i,7).toInt() == 0 ? false : true;
			item->confirmUser = rs.GetValue(i,8).data();
			item->confirmSoc = rs.GetValue(i,9).toInt();
			m_mapHisItems.insert(item->sn,item);
			if (item->sn > lastSn)
				lastSn = item->sn;
		}
		ssp_floatwnd->SetLastHisSequence(lastSn);
	}
}

void CIa2AlarmPopWnd::SetPushButtonQss(QPushButton *btn, int radius, int padding,int width,const QString &borderColor,
	const QString &normalColor, const QString &normalTextColor,
	const QString &hoverColor, const QString &hoverTextColor,
	const QString &pressedColor, const QString &pressedTextColor)
{
	QStringList qss;
	qss.append(QString("QPushButton{border-style:solid;padding:%1px;border-radius:%2px;color:%3;background:%4;border-width:%5;border-color:%6;}")
		.arg(padding).arg(radius).arg(normalTextColor).arg(normalColor).arg(width).arg(borderColor));
	qss.append(QString("QPushButton:hover{color:%1;background:%2;}")
		.arg(hoverTextColor).arg(hoverColor));
	qss.append(QString("QPushButton:pressed{color:%1;background:%2;}")
		.arg(pressedTextColor).arg(pressedColor));
	btn->setStyleSheet(qss.join(""));
}

void CIa2AlarmPopWnd::InsertMapHisItem(stuHisItem *tItem)
{
	m_mapHisItems.insert(tItem->sn,tItem);
}

void CIa2AlarmPopWnd::PopupWnd(stuHisItem *tItem)
{
	if (ssp_floatwnd->ReadSettings("/pop/none").toInt() == 1) //������
		return;

	if (((ssp_floatwnd->ReadSettings("/pop/critical").toInt() == 1 && tItem->severity == SEVERITY_CRITICAL) ||
		(ssp_floatwnd->ReadSettings("/pop/important").toInt() == 1 && tItem->severity == SEVERITY_IMPORTANT) ||
		(ssp_floatwnd->ReadSettings("/pop/secondary").toInt() == 1 && tItem->severity == SEVERITY_SECONDARY) ||
		(ssp_floatwnd->ReadSettings("/pop/general").toInt() == 1 && tItem->severity == SEVERITY_GENEARL)) && this->isHidden())
	{
		this->show();
		ssp_floatwnd->hide();
	}
}

void CIa2AlarmPopWnd::InsertTableWidget(stuHisItem *tItem)
{
	if (m_mapFilterBays[tItem->bayNo].isEmpty()) //����ɸѡ�ļ����Χ��
		return;

	if (!m_bIsStart) //��̬ͣ
		return;

	QColor color;
	if (tItem->isConfirm)
	{
		color.setNamedColor(CONFIRM_COLOR);
	}
	else
	{
		switch (tItem->severity)
		{
		case 1:
			color.setNamedColor(ssp_floatwnd->ReadSettings("/color/general"));
			break;
		case 2:
			color.setNamedColor(ssp_floatwnd->ReadSettings("/color/secondary"));
			break;
		case 3:
			color.setNamedColor(ssp_floatwnd->ReadSettings("/color/important"));
			break;
		case 4:
			color.setNamedColor(ssp_floatwnd->ReadSettings("/color/critical"));
			break;
		default:
			color.setNamedColor("#ffffff");
			break;
		}
	}

	ui.tableWidget->insertRow(0);

	SDateTime dt(tItem->soc);
	QString s ;
	s.sprintf("%s.%03d",dt.toString("yyyy-MM-dd hh:mm:ss").data(),tItem->usec/1000);
	QTableWidgetItem *item = new QTableWidgetItem(s);
	item->setData(Qt::UserRole,tItem->sn);
	item->setBackgroundColor(QColor(0,0,0));
	item->setTextColor(color);
	item->setTextAlignment(Qt::AlignCenter);
	item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
	ui.tableWidget->setItem(0,COLUMN_TIME,item);

	item = new QTableWidgetItem(m_mapSeveritys[tItem->severity]);
	item->setBackgroundColor(QColor(0,0,0));
	item->setTextColor(color);
	item->setTextAlignment(Qt::AlignCenter);
	item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
	ui.tableWidget->setItem(0,COLUMN_SEVERITY,item);

	s = tr("��վ�������%1��-��%2��\n").arg(m_mapSubs[tItem->subNo]).arg(m_mapBays[tItem->bayNo]);
	s += tr("�澯���ݣ���%1��").arg(tItem->conclusion);
	item = new QTableWidgetItem(s);
	item->setBackgroundColor(QColor(0,0,0));
	item->setTextColor(color);
	item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
	ui.tableWidget->setItem(0,COLUMN_CONTENT,item);

	item = new QTableWidgetItem(tr("�鿴"));
	item->setBackgroundColor(QColor(0,0,0));
	item->setTextColor(color);
	QFont fontUnderLine;
	fontUnderLine.setUnderline(true);
	item->setFont(fontUnderLine);
	item->setTextAlignment(Qt::AlignCenter);
	item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
	ui.tableWidget->setItem(0,COLUMN_DETAIL,item);

	SDateTime dt1(tItem->confirmSoc);
	s.sprintf("%s",dt1.toString("yyyy-MM-dd hh:mm:ss").data());
	item = new QTableWidgetItem(tItem->isConfirm == false ? "δȷ��" : "��ȷ��");
	item->setBackgroundColor(QColor(0,0,0));
	item->setTextColor(color);
	item->setTextAlignment(Qt::AlignCenter);
	if (tItem->isConfirm)
		item->setToolTip(tr("��%1���û���%2��ȷ�ϸ澯").arg(s).arg(tItem->confirmUser));
	item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
	ui.tableWidget->setItem(0,COLUMN_CONFIRM,item);

	while(ui.tableWidget->rowCount() > ssp_floatwnd->ReadSettings("/count/maxshow").toInt())
		ui.tableWidget->removeRow(ui.tableWidget->rowCount()-1);

	ui.tableWidget->scrollToTop();
}

void CIa2AlarmPopWnd::resizeEvent(QResizeEvent *event)
{
	if (m_bIsExtend)
		ui.tableWidget->setColumnWidth(COLUMN_CONTENT,
		this->width()-COLUMN_SCROLL_WIDTH-COLUMN_DETAIL_WIDTH-COLUMN_SEVERITY_WIDTH-COLUMN_TIME_WIDTH-COLUMN_CONFIRM_WIDTH-WND_EXT_WIDTH-VERTICAL_SPACING);
	else
		ui.tableWidget->setColumnWidth(COLUMN_CONTENT,
		this->width()-COLUMN_SCROLL_WIDTH-COLUMN_DETAIL_WIDTH-COLUMN_SEVERITY_WIDTH-COLUMN_TIME_WIDTH-COLUMN_CONFIRM_WIDTH);
}

void CIa2AlarmPopWnd::closeEvent(QCloseEvent *event)
{
	ssp_floatwnd->show();
	this->hide();
}

void CIa2AlarmPopWnd::Start()
{
	InitMap();

	m_iCurrentSeverity = SEVERITY_ALL;
	RefreshTableWidget(m_iCurrentSeverity);
}

void CIa2AlarmPopWnd::ShowTableWidget(int severity)
{
	m_iRow = 0;
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(0);

	ssp_floatwnd->m_iLock.lock();
	QMapIterator<int,stuHisItem*> iter(m_mapHisItems);
	iter.toBack();
	while (iter.hasPrevious())	
	{
		iter.previous();
		stuHisItem *his = iter.value();

		if (m_mapFilterBays[his->bayNo].isEmpty())
			continue;

		if (severity == SEVERITY_CRITICAL && his->severity != SEVERITY_CRITICAL)
			continue;
		else if (severity == SEVERITY_IMPORTANT && his->severity != SEVERITY_IMPORTANT)
			continue;
		else if (severity == SEVERITY_SECONDARY && his->severity != SEVERITY_SECONDARY)
			continue;
		else if (severity == SEVERITY_GENEARL && his->severity != SEVERITY_GENEARL)
			continue;

		QColor color;
		if (his->isConfirm)
		{
			color .setNamedColor(CONFIRM_COLOR);
		}
		else
		{
			switch (his->severity)
			{
			case 1:
				color.setNamedColor(ssp_floatwnd->ReadSettings("/color/general"));
				break;
			case 2:
				color.setNamedColor(ssp_floatwnd->ReadSettings("/color/secondary"));
				break;
			case 3:
				color.setNamedColor(ssp_floatwnd->ReadSettings("/color/important"));
				break;
			case 4:
				color.setNamedColor(ssp_floatwnd->ReadSettings("/color/critical"));
				break;
			default:
				color.setNamedColor("#ffffff");
				break;
			}
		}

		ui.tableWidget->setRowCount(m_iRow+1);

		SDateTime dt(his->soc);
		QString s ;
		s.sprintf("%s.%03d",dt.toString("yyyy-MM-dd hh:mm:ss").data(),his->usec/1000);
		QTableWidgetItem *item = new QTableWidgetItem(s);
		item->setData(Qt::UserRole,his->sn);
		item->setBackgroundColor(QColor(0,0,0));
		item->setTextColor(color);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
		ui.tableWidget->setItem(m_iRow,COLUMN_TIME,item);

		item = new QTableWidgetItem(m_mapSeveritys[his->severity]);
		item->setBackgroundColor(QColor(0,0,0));
		item->setTextColor(color);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
		ui.tableWidget->setItem(m_iRow,COLUMN_SEVERITY,item);

		s = tr("��վ�������%1��-��%2��\n").arg(m_mapSubs[his->subNo]).arg(m_mapBays[his->bayNo]);
		s += tr("�澯���ݣ���%1��").arg(his->conclusion);
		item = new QTableWidgetItem(s);
		item->setBackgroundColor(QColor(0,0,0));
		item->setTextColor(color);
		item->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
		ui.tableWidget->setItem(m_iRow,COLUMN_CONTENT,item);

		item = new QTableWidgetItem(tr("�鿴"));
		item->setBackgroundColor(QColor(0,0,0));
		item->setTextColor(color);
		QFont fontUnderLine;
		fontUnderLine.setUnderline(true);
		item->setFont(fontUnderLine);
		item->setTextAlignment(Qt::AlignCenter);
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
		ui.tableWidget->setItem(m_iRow,COLUMN_DETAIL,item);

		SDateTime dt1(his->confirmSoc);
		s.sprintf("%s",dt1.toString("yyyy-MM-dd hh:mm:ss").data());
		item = new QTableWidgetItem(his->isConfirm == false ? "δȷ��" : "��ȷ��");
		item->setBackgroundColor(QColor(0,0,0));
		item->setTextColor(color);
		item->setTextAlignment(Qt::AlignCenter);
		if (his->isConfirm)
			item->setToolTip(tr("��%1���û���%2��ȷ�ϸ澯").arg(s).arg(his->confirmUser));
		item->setFlags(item->flags() & (~Qt::ItemIsEditable));	//����item��ɱ༭
		ui.tableWidget->setItem(m_iRow,COLUMN_CONFIRM,item);
		m_iRow++;

		if (m_iRow > ssp_floatwnd->ReadSettings("/count/maxshow").toInt())
			break;
	}
	ssp_floatwnd->m_iLock.unlock();
}

void CIa2AlarmPopWnd::RefreshTableWidget(int severity)
{
	switch (severity)
	{
	case SEVERITY_ALL:
		SlotBtnAllClicked();
		break;
	case SEVERITY_CRITICAL:
		SlotBtnCritrialClicked();
		break;
	case SEVERITY_IMPORTANT:
		SlotBtnImportantClicked();
		break;
	case SEVERITY_SECONDARY:
		SlotBtnSecondaryClicked();
		break;
	case SEVERITY_GENEARL:
		SlotBtnGeneralClicked();
		break;
	default:
		SlotBtnAllClicked();
		break;
	}
}

void CIa2AlarmPopWnd::SlotTableWidgetItemClicked(QTableWidgetItem *item)
{
	if (item->column() != COLUMN_DETAIL)
		return;

	int sn = ui.tableWidget->item(item->row(),COLUMN_TIME)->data(Qt::UserRole).value<int>();
	ssp_floatwnd->IAalarmDetail(sn);
}

void CIa2AlarmPopWnd::SlotTableWidgetItemDoubleClicked(QTableWidgetItem *item)
{
	int sn = ui.tableWidget->item(item->row(),COLUMN_TIME)->data(Qt::UserRole).value<int>();
	ssp_floatwnd->IAalarmDetail(sn);
}

void CIa2AlarmPopWnd::SlotTableWidgetClickedRightMenu(const QPoint &pos)
{
	m_popMenu->popup(QCursor::pos());
}

void CIa2AlarmPopWnd::slotTreeWidgetItemChanged(QTreeWidgetItem *item,int column)
{
	if (Qt::PartiallyChecked != item->checkState(column)) 
		SetChildCheckState(item,column,item->checkState(column));
}

void CIa2AlarmPopWnd::SlotRestoreTimeout()
{
	for (int i = 0; i < ui.treeWidget->topLevelItemCount(); i++) //�ָ�ɸѡ
	{
		QTreeWidgetItem *item = ui.treeWidget->topLevelItem(i);
		if (item->checkState(0) != Qt::Checked)
		{
			item->setCheckState(0,Qt::Checked);
			slotTreeWidgetItemChanged(item,0);
		}
	}

	if (m_bIsExtend) //������չ���ܴ�
		SlotBtnExtendClicked();

	if (!m_bIsStart) //��Ϊ��ʼ̬
		SlotBtnStartStopClicked();

	m_iRestoreTimer->stop(); //�رռ�ʱ
	SetMapFilterBays();
	RefreshTableWidget(m_iCurrentSeverity);
}

void CIa2AlarmPopWnd::SlotActClearAll()
{
	DlgMessage dlg(this);
	dlg.SetMessage(VIEW_QUESTION,"�����ȫ����ȷ�ϸ澯��\n\nͬ��ѡ���ǡ�����ͬ��ѡ���񡿣���ѡ��");
	if (dlg.exec() != QDialog::Accepted)
		return;

	ssp_floatwnd->m_iLock.lock();
	SString sql = SString::toFormat("delete from t_ia2_history where is_confirm=1");
	DB->Execute(sql); //������ݿ�

	bool refresh = false;
	QMapIterator<int,stuHisItem*> iter(m_mapHisItems);
	iter.toBack();
	while (iter.hasPrevious())	
	{
		iter.previous();
		stuHisItem *his = iter.value();
		if (his->isConfirm)
		{
			m_mapHisItems.remove(his->sn); //����ڴ�
			refresh = true;
		}
	}
	ssp_floatwnd->RefreshFloatWnd();
	ssp_floatwnd->m_iLock.unlock();

	if (refresh)
		RefreshTableWidget(m_iCurrentSeverity); //����ˢ�½���
}

void CIa2AlarmPopWnd::SlotActClear()
{
	ssp_floatwnd->m_iLock.lock();

	QMap<int,int> mapRow;
	QList<QTableWidgetSelectionRange> ranges = ui.tableWidget->selectedRanges();
	for(int i = 0; i < ranges.count(); i++)
	{
		int topRow = ranges.at(i).topRow();
		int bottomRow = ranges.at(i).bottomRow();
		for(int row = topRow; row <= bottomRow; row++)
		{
			int sn = ui.tableWidget->item(row,COLUMN_TIME)->data(Qt::UserRole).value<int>();
			m_mapHisItems.remove(sn); //����ڴ�
			SString sql = SString::toFormat("delete from t_ia2_history where ia_his=%d",sn);
			DB->Execute(sql); //������ݿ�
			mapRow.insert(sn,row); //��¼�����к�
		}
	}

	QMapIterator<int,int> iter(mapRow);
	while (iter.hasNext())	
	{
		iter.next();
		ui.tableWidget->removeRow(iter.value()); //�������
	}
	ssp_floatwnd->RefreshFloatWnd();
	ssp_floatwnd->m_iLock.unlock();
}

void CIa2AlarmPopWnd::SlotActConfirmAll()
{
	//ui.tableWidget->selectAll();
	//SlotActConfirm();

	QString warnText = QString::null;
	SString usr_code = SSP_GUI->GetLoginUser();
	SString usr_name = SSP_GUI->GetLoginUserName();
	if (usr_code.isEmpty() || usr_name.isEmpty())
	{
		DlgMessage dlg(this);
		dlg.SetMessage(VIEW_WARN,"���û���Ϣ���޷�ȷ�ϡ�\n\n���ȵ�½��");
		dlg.exec();
		return;
	}

	DlgMessage dlg(this);
	dlg.SetMessage(VIEW_QUESTION,"��ȷ��ȫ��δȷ�ϸ澯��\n\nͬ��ѡ���ǡ�����ͬ��ѡ���񡿣���ѡ��");
	if (dlg.exec() != QDialog::Accepted)
		return;

	int soc = (int)SDateTime::getNowSoc();
	SString sql = SString::toFormat("update t_ia2_history set is_confirm=1,confirm_user='%s',confirm_soc=%d where is_confirm=0",usr_name.data(),soc);
	DB->Execute(sql);

	QMapIterator<int,stuHisItem*> iter(m_mapHisItems);
	while (iter.hasNext())	
	{
		iter.next();
		stuHisItem *item = iter.value();
		if (item->isConfirm == false)
		{
			item->isConfirm = true;
			item->confirmSoc = soc;
			item->confirmUser = usr_name.data();
		}
	}

	RefreshTableWidget(m_iCurrentSeverity);
	ssp_floatwnd->RefreshFloatWnd();
}

void CIa2AlarmPopWnd::SlotActConfirm()
{
	QString warnText = QString::null;
	SString usr_code = SSP_GUI->GetLoginUser();
	SString usr_name = SSP_GUI->GetLoginUserName();
	if (usr_code.isEmpty() || usr_name.isEmpty())
	{
		DlgMessage dlg(this);
		dlg.SetMessage(VIEW_WARN,"���û���Ϣ���޷�ȷ�ϡ�\n\n���ȵ�½��");
		dlg.exec();
		return;
	}

	QList<QTableWidgetSelectionRange> ranges = ui.tableWidget->selectedRanges();
	for(int i = 0; i < ranges.count(); i++)
	{
		int topRow = ranges.at(i).topRow();
		int bottomRow = ranges.at(i).bottomRow();
		for(int row = topRow; row <= bottomRow; row++)
		{
			int sn = ui.tableWidget->item(row,COLUMN_TIME)->data(Qt::UserRole).value<int>();

			if (ui.tableWidget->item(row,COLUMN_TIME)->textColor().name() == CONFIRM_COLOR) //�Ѿ�ȷ�ϵĸ澯
				continue;

			int soc = (int)SDateTime::getNowSoc();
			SString sql = SString::toFormat("update t_ia2_history set is_confirm=1,confirm_user='%s',confirm_soc=%d where ia_his=%d",
				usr_name.data(),soc,sn);
			if (!DB->Execute(sql))
			{
				warnText += tr("��%1�и澯��Ϣ�����ݿ�ȷ��ʧ�ܡ�\n");
			}
			else
			{
				QMap<int,stuHisItem*>::iterator iter = m_mapHisItems.find(sn);
				if (iter != m_mapHisItems.end())
				{
					stuHisItem *item = iter.value();
					item->isConfirm = true;
					item->confirmSoc = soc;
					item->confirmUser = usr_name.data();

					for (int column = 0; column < COLUMN_NUMBER; column++)
					{
						ui.tableWidget->item(row,column)->setTextColor(CONFIRM_COLOR);
						if (column == COLUMN_CONFIRM)
						{
							SDateTime dt(item->confirmSoc);
							QString s;
							s.sprintf("%s",dt.toString("yyyy-MM-dd hh:mm:ss").data());
							ui.tableWidget->item(row,column)->setToolTip(tr("��%1���û���%2��ȷ�ϸ澯").arg(s).arg(item->confirmUser));
							ui.tableWidget->item(row,column)->setText(tr("��ȷ��"));
						}
					}
				}
				else
				{
					warnText += tr("��%1�и澯��Ϣ���ڴ�ȷ��ʧ�ܡ�\n");
				}
			}
		}
	}

	if (warnText.isEmpty())
	{
		DlgMessage dlg(this);
		dlg.SetMessage(VIEW_INFORMATION,"ȷ�ϳɹ���");
		dlg.exec();
	}
	else
	{
		DlgMessage dlg(this);
		dlg.SetMessage(VIEW_WARN,tr("ȷ��ʧ�ܡ�\n\nʧ�����ݣ�\n%1").arg(warnText));
		dlg.exec();
	}

	ssp_floatwnd->RefreshFloatWnd();
}

void CIa2AlarmPopWnd::SlotBtnAllClicked()
{
	SetPushButtonQss(ui.btn_all,5,8,1,"#000000","#646464","#FFFFFF","#646464","#FFFFFF","#646464","#FFFFFF");
	QString color = ssp_floatwnd->ReadSettings("/color/critical");
	SetPushButtonQss(ui.btn_critrial,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/important");
	SetPushButtonQss(ui.btn_important,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/secondary");
	SetPushButtonQss(ui.btn_secondary,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/general");
	SetPushButtonQss(ui.btn_general,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");

	ShowTableWidget();
	ui.tableWidget->scrollToTop();
	m_iCurrentSeverity = SEVERITY_ALL;
}

void CIa2AlarmPopWnd::SlotBtnCritrialClicked()
{
	SetPushButtonQss(ui.btn_all,5,8,1,"#000000","#FFFFFF","#000000","#646464","#FFFFFF","#646464","#FFFFFF");
	QString color = ssp_floatwnd->ReadSettings("/color/critical");
	SetPushButtonQss(ui.btn_critrial,5,8,1,"#000000",color,"#FFFFFF",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/important");
	SetPushButtonQss(ui.btn_important,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/secondary");
	SetPushButtonQss(ui.btn_secondary,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/general");
	SetPushButtonQss(ui.btn_general,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");

	ShowTableWidget(SEVERITY_CRITICAL);
	ui.tableWidget->scrollToTop();
	m_iCurrentSeverity = SEVERITY_CRITICAL;
}

void CIa2AlarmPopWnd::SlotBtnImportantClicked()
{
	SetPushButtonQss(ui.btn_all,5,8,1,"#000000","#FFFFFF","#000000","#646464","#FFFFFF","#646464","#FFFFFF");
	QString color = ssp_floatwnd->ReadSettings("/color/critical");
	SetPushButtonQss(ui.btn_critrial,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/important");
	SetPushButtonQss(ui.btn_important,5,8,1,"#000000",color,"#FFFFFF",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/secondary");
	SetPushButtonQss(ui.btn_secondary,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/general");
	SetPushButtonQss(ui.btn_general,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");

	ShowTableWidget(SEVERITY_IMPORTANT);
	ui.tableWidget->scrollToTop();
	m_iCurrentSeverity = SEVERITY_IMPORTANT;
}

void CIa2AlarmPopWnd::SlotBtnSecondaryClicked()
{
	SetPushButtonQss(ui.btn_all,5,8,1,"#000000","#FFFFFF","#000000","#646464","#FFFFFF","#646464","#FFFFFF");
	QString color = ssp_floatwnd->ReadSettings("/color/critical");
	SetPushButtonQss(ui.btn_critrial,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/important");
	SetPushButtonQss(ui.btn_important,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/secondary");
	SetPushButtonQss(ui.btn_secondary,5,8,1,"#000000",color,"#FFFFFF",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/general");
	SetPushButtonQss(ui.btn_general,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");

	ShowTableWidget(SEVERITY_SECONDARY);
	ui.tableWidget->scrollToTop();
	m_iCurrentSeverity = SEVERITY_SECONDARY;
}

void CIa2AlarmPopWnd::SlotBtnGeneralClicked()
{
	SetPushButtonQss(ui.btn_all,5,8,1,"#000000","#FFFFFF","#000000","#646464","#FFFFFF","#646464","#FFFFFF");
	QString color = ssp_floatwnd->ReadSettings("/color/critical");
	SetPushButtonQss(ui.btn_critrial,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/important");
	SetPushButtonQss(ui.btn_important,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/secondary");
	SetPushButtonQss(ui.btn_secondary,5,8,1,"#000000","#FFFFFF","#000000",color,"#FFFFFF",color,"#FFFFFF");
	color = ssp_floatwnd->ReadSettings("/color/general");
	SetPushButtonQss(ui.btn_general,5,8,1,"#000000",color,"#FFFFFF",color,"#FFFFFF",color,"#FFFFFF");

	ShowTableWidget(SEVERITY_GENEARL);
	ui.tableWidget->scrollToTop();
	m_iCurrentSeverity = SEVERITY_GENEARL;
}

void CIa2AlarmPopWnd::SlotBtnExtendClicked()
{
	m_bIsExtend = !m_bIsExtend;
	ui.widget_extend->setVisible(m_bIsExtend);
	
	if (m_bIsExtend)
	{
		this->resize(this->width()+WND_EXT_WIDTH+VERTICAL_SPACING,this->height());
		ui.btn_extend->setIcon(QIcon(":/images/hideleft"));
		ui.btn_extend->setText(tr("������չ"));
	}
	else
	{
		this->resize(this->width()-WND_EXT_WIDTH-VERTICAL_SPACING,this->height());
		ui.btn_extend->setIcon(QIcon(":/images/hideright"));
		ui.btn_extend->setText(tr("��չ����"));
	}
}

void CIa2AlarmPopWnd::SlotBtnSettingClicked()
{
	DlgConfigure dlg(ssp_floatwnd);
	if (dlg.exec() == QDialog::Accepted)
	{
		RefreshTableWidget(m_iCurrentSeverity);
		ssp_floatwnd->RefreshFloatWnd();
	}
}

void CIa2AlarmPopWnd::SlotBtnOkClicked()
{
	SetMapFilterBays();
	m_iRestoreTimer->stop();
	m_iRestoreTimer->setInterval(ssp_floatwnd->ReadSettings("/count/restoretime").toInt() * 1000);
	m_iRestoreTimer->start();
	
	RefreshTableWidget(m_iCurrentSeverity);
}

void CIa2AlarmPopWnd::SlotBtnConfirmAllClicked()
{
	SlotActConfirmAll();
}

void CIa2AlarmPopWnd::SlotBtnStartStopClicked()
{
	m_iRestoreTimer->stop();
	m_bIsStart = !m_bIsStart;
	if (m_bIsStart)
	{
		ui.btn_start_stop->setIcon(QIcon(":/images/pause"));
		ui.btn_start_stop->setText(tr("��ͣ"));
	}
	else
	{
		ui.btn_start_stop->setIcon(QIcon(":/images/start"));
		ui.btn_start_stop->setText(tr("��ʼ"));
		m_iRestoreTimer->setInterval(ssp_floatwnd->ReadSettings("/count/restoretime").toInt() * 1000);
		m_iRestoreTimer->start();
	}
}

void CIa2AlarmPopWnd::SlotBtnSaveClicked()
{
	SDateTime dt = SDateTime::makeDateTime(SDateTime::getNowSoc());
	QString saveName = tr("ʵʱ�澯���б�-%1").arg(dt.toString("yyyy��MM��dd��hhʱmm��ss��").data());

	QString filter = tr("EXCEL�ļ�(*.csv);;PDF�ļ�(*.pdf)");
	QString fileName = QFileDialog::getSaveFileName(this,tr("�����ļ�"),saveName,filter);
	if (!fileName.isEmpty())
	{
		ssp_floatwnd->m_iLock.lock();
		QFileInfo fileinfo(fileName);
		QString suffix = fileinfo.suffix();
		if (suffix == "csv")
			SaveCsvFile(fileName);
		else if (suffix == "pdf")
			SavePdfFile(fileName);
		ssp_floatwnd->m_iLock.unlock();
	}
}

void CIa2AlarmPopWnd::SlotBtnQueryClicked()
{
	ssp_floatwnd->IAalarmQuery();
}

void CIa2AlarmPopWnd::SetChildCheckState(QTreeWidgetItem *item,int column,Qt::CheckState state)
{
	for (int i = 0; i < item->childCount(); i++)
	{
		QTreeWidgetItem *it = item->child(i);
		it->setCheckState(column,state);
	}

	SetParentCheckState(item->parent());
}

void CIa2AlarmPopWnd::SetParentCheckState(QTreeWidgetItem *item)
{
	if (!item) 
		return;

	int selectedCount = 0;
	int selectedParCount = 0;
	int childCount = item->childCount();
	for (int i = 0; i < childCount; i++)  
	{  
		QTreeWidgetItem* child = item->child(i);  
		if (child->checkState(0) == Qt::Checked)   
			selectedCount++; 
		if (child->checkState(0) == Qt::PartiallyChecked)
			selectedParCount++;
	}  

	if (selectedParCount)
	{
		item->setCheckState(0,Qt::PartiallyChecked);
		return;
	}

	if (selectedCount == 0)
		item->setCheckState(0,Qt::Unchecked);
	else if (selectedCount == childCount)
		item->setCheckState(0,Qt::Checked);
	else
		item->setCheckState(0,Qt::PartiallyChecked);
}

void CIa2AlarmPopWnd::SetMapFilterBays()
{
	m_mapFilterBays.clear();
	QTreeWidgetItemIterator iter(ui.treeWidget);
	while (*iter) 
	{
		if ((*iter)->type() == TREE_TYPE_BAY && (*iter)->checkState(0) == Qt::Checked)
			m_mapFilterBays.insert((*iter)->data(0,Qt::UserRole).toInt(),(*iter)->text(0));
		++iter;
	}
}

void CIa2AlarmPopWnd::SaveCsvFile(QString fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		DlgMessage dlg(this);
		dlg.SetMessage(VIEW_WARN,"�ļ�д��ʽ�򿪴���");
		dlg.exec();
		return;
	}

	QTextStream strem(&file);
	for (int row = 0; row < ui.tableWidget->rowCount(); row++)
	{
		QString text = ui.tableWidget->item(row,COLUMN_TIME)->text().replace(" ","_") + ",";
		text += ui.tableWidget->item(row,COLUMN_SEVERITY)->text() + ",";
		text += ui.tableWidget->item(row,COLUMN_CONTENT)->text().replace("\n","-").replace(" ","_") + ",";
		text += ui.tableWidget->item(row,COLUMN_CONFIRM)->text();
		strem << text << endl; 
	}

	file.close();

	DlgMessage dlg(this);
	dlg.SetMessage(VIEW_INFORMATION,"�����ļ��ɹ�");
	dlg.exec();
}

void CIa2AlarmPopWnd::SavePdfFile(QString fileName)
{
	QTextBrowser browser;

	SXmlConfig report;
	report.SetNodeName("html");
	SBaseConfig *body = report.AddChildNode("body");
	SBaseConfig *h1 = body->AddChildNode("h1","align=center","ʵʱ�澯���б�");
	SBaseConfig *br = body->AddChildNode("br");

	SBaseConfig *table = body->AddChildNode("table",
		SString::toFormat("cellspacing=0;border=1;cellpadding=%d;width=100%%",ui.tableWidget->columnCount()));
	table->SetAttribute("style","border-style:solid;margin-top:-2px;margin-left:15px;margin-right:15px;");

	//�б���
	SBaseConfig *tr = table->AddChildNode("tr");
	for (int i = 0; i < ui.tableWidget->columnCount(); i++)
	{
		SBaseConfig *td = tr->AddChildNode("td",SString::toFormat("align=center;valign=middle;bgColor=#C0C0C0"));
		td->AddChildNode("b","",ui.tableWidget->horizontalHeaderItem(i)->text().toStdString().data());
	}
	//�����е���ֵ
	for (int i = 0; i < ui.tableWidget->rowCount(); i++)
	{
		tr = table->AddChildNode("tr");
		for (int j = 0; j < ui.tableWidget->columnCount(); j++)
		{
			SBaseConfig *td;
			if (j == COLUMN_CONTENT)
				td = tr->AddChildNode("td",SString::toFormat("align=left;valign=middle"));
			else
				td = tr->AddChildNode("td",SString::toFormat("align=center;valign=middle"));
			td->AddChildNode("b","",ui.tableWidget->item(i,j)->text().toStdString().data());
		}
	}

	SString html;
	report.SaveConfigToText(html);
	browser.setHtml(html.data());
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileName);
	browser.document()->print(&printer);

	DlgMessage dlg(this);
	dlg.SetMessage(VIEW_INFORMATION,"�����ļ��ɹ�");
	dlg.exec();
}

bool CIa2AlarmPopWnd::OnConfirmHistory(int sn)
{
	QString warnText = QString::null;
	SString usr_code = SSP_GUI->GetLoginUser();
	SString usr_name = SSP_GUI->GetLoginUserName();
	if (usr_code.isEmpty() || usr_name.isEmpty())
	{
		DlgMessage dlg(this);
		dlg.SetMessage(VIEW_WARN,"���û���Ϣ���޷�ȷ�ϡ�\n\n���ȵ�½��");
		dlg.exec();
		return false;
	}

	int soc = (int)SDateTime::getNowSoc();
	SString sql = SString::toFormat("update t_ia2_history set is_confirm=1,confirm_user='%s',confirm_soc=%d where ia_his=%d",
		usr_name.data(),soc,sn);
	if (!DB->Execute(sql))
	{
		return false;
	}
	else
	{
		QMap<int,stuHisItem*>::iterator iter = m_mapHisItems.find(sn);
		if (iter != m_mapHisItems.end())
		{
			stuHisItem *item = iter.value();
			item->isConfirm = true;
			item->confirmSoc = soc;
			item->confirmUser = usr_name.data();

			RefreshTableWidget(m_iCurrentSeverity);
		}
	}

	ssp_floatwnd->RefreshFloatWnd();

	return true;
}

void CIa2AlarmPopWnd::OnRefreshTableWidget()
{
	RefreshTableWidget(m_iCurrentSeverity);
}
