#include "view_reportsetting.h"
static void qt_graphicsItem_highlightSelected(QGraphicsItem *item, QPainter *painter, const QStyleOptionGraphicsItem *option);
static QString setValue(QString info,QString name,QString value);
static QString getAtu(QString info,QString name);
view_reportsetting::view_reportsetting(QWidget *parent)
	: CBaseView(parent)
{
	ui.setupUi(this);
	sql_dlg=new settingSQL_dlg(this);
	group_dlg=NULL;
	page_dlg=NULL;
	topWidget=new QWidget(this);
	leftWidget=new QWidget(this);
	m_dataWindow=GET_DW_CFG;
	width=0;height=0;
	m_scene=new MyGraphicsScene(this);
	connect(m_scene,SIGNAL(changeHW(int,int)),this,SLOT(changeHW(int,int)));
	m_pOper=DB;
	dsNo=-1;
 	m_view = new QGraphicsView(m_scene); 
	m_view->centerOn(0,0);
	QHBoxLayout *layouttop = new QHBoxLayout();
	newreport=new QPushButton(tr("�½�����(&N)"),this);
	chgName=new QPushButton(tr("����������(&R)"),this);
	selreport=new QComboBox(this);
	selreport->addItem("δѡ��");
	delreport=new QPushButton(tr("ɾ����ǰ����"),this);
	delElement=new QPushButton(tr("ɾ��Ԫ��(&D)"),this);
	sqlElement=new QPushButton(tr("����Դ����(&S)"),this);
	groupElement=new QPushButton(tr("����(&G)"),this);
	pageSetting=new QPushButton(tr("����ҳ����(&P)"),this);
	//pageSetting->hide();
	layouttop->addWidget(newreport);
	layouttop->addWidget(selreport);
	layouttop->addWidget(chgName);
	layouttop->addWidget(delreport);
	layouttop->addWidget(delElement);
	layouttop->addWidget(sqlElement);
	layouttop->addWidget(groupElement);
	layouttop->addWidget(pageSetting);
	layouttop->addStretch();
	layouttop->setMargin(2);
	topWidget->setLayout(layouttop);
	
	QVBoxLayout *layoutleft = new QVBoxLayout();
	rectDraw=new QPushButton(this);
	rectDraw->setText("����");
	rectDraw->hide();
	lineHDraw=new QPushButton(this);
	lineHDraw->setToolTip("����");
	lineHDraw->setMinimumHeight(32);
	lineHDraw->setMaximumHeight(32);
	lineHDraw->setMinimumWidth(32);
	lineHDraw->setMaximumWidth(32);
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/ssp_gui/Resources/rpt_lineH.png"), QSize(), QIcon::Normal, QIcon::Off);
	lineHDraw->setIcon(icon1);

	lineVDraw=new QPushButton(this);
	lineVDraw->setToolTip("����");
	lineVDraw->setMinimumHeight(32);
	lineVDraw->setMaximumHeight(32);
	lineVDraw->setMinimumWidth(32);
	lineVDraw->setMaximumWidth(32);
	QIcon icon2;
	icon2.addFile(QString::fromUtf8(":/ssp_gui/Resources/rpt_lineV.png"), QSize(), QIcon::Normal, QIcon::Off);
	lineVDraw->setIcon(icon2);

	textDraw=new QPushButton(this);
	textDraw->setToolTip("�ı�");
	textDraw->setMinimumHeight(32);
	textDraw->setMaximumHeight(32);
	textDraw->setMinimumWidth(32);
	textDraw->setMaximumWidth(32);
	QIcon icon3;
	icon3.addFile(QString::fromUtf8(":/ssp_gui/Resources/rpt_txt.png"), QSize(), QIcon::Normal, QIcon::Off);
	textDraw->setIcon(icon3);

	lineDraw=new QPushButton(this);
	lineDraw->setToolTip("б��");
	lineDraw->setMinimumHeight(32);
	lineDraw->setMaximumHeight(32);
	lineDraw->setMinimumWidth(32);
	lineDraw->setMaximumWidth(32);
	QIcon icon4;
	icon4.addFile(QString::fromUtf8(":/ssp_gui/Resources/rpt_line.png"), QSize(), QIcon::Normal, QIcon::Off);
	lineDraw->setIcon(icon4);

	expDraw=new QPushButton(this);
	expDraw->setToolTip("���ʽ");
	expDraw->setMinimumHeight(32);
	expDraw->setMaximumHeight(32);
	expDraw->setMinimumWidth(32);
	expDraw->setMaximumWidth(32);
	QIcon icon5;
	icon5.addFile(QString::fromUtf8(":/ssp_gui/Resources/rpt_grp.png"), QSize(), QIcon::Normal, QIcon::Off);
	expDraw->setIcon(icon5);

	chartDraw=new QPushButton(this);
	chartDraw->setToolTip("ͼ��");
	chartDraw->setMinimumHeight(32);
	chartDraw->setMaximumHeight(32);
	chartDraw->setMinimumWidth(32);
	chartDraw->setMaximumWidth(32);
	QIcon icon6;
	icon6.addFile(QString::fromUtf8(":/ssp_gui/Resources/rpt_chart.png"), QSize(), QIcon::Normal, QIcon::Off);
	chartDraw->setIcon(icon6);

	Halign=new QPushButton(this);
	Halign->setToolTip("�����");
	Halign->setMinimumHeight(32);
	Halign->setMaximumHeight(32);
	Halign->setMinimumWidth(32);
	Halign->setMaximumWidth(32);
	QIcon icon7;
	icon7.addFile(QString::fromUtf8(":/ssp_gui/Resources/rpt_Halign.png"), QSize(), QIcon::Normal, QIcon::Off);
	Halign->setIcon(icon7);

	Valign=new QPushButton(this);
	Valign->setToolTip("������");
	Valign->setMinimumHeight(32);
	Valign->setMaximumHeight(32);
	Valign->setMinimumWidth(32);
	Valign->setMaximumWidth(32);
	QIcon icon8;
	icon8.addFile(QString::fromUtf8(":/ssp_gui/Resources/rpt_Valign.png"), QSize(), QIcon::Normal, QIcon::Off);
	Valign->setIcon(icon8);

	sizeAlign=new QPushButton(this);
	sizeAlign->setToolTip("��С����");
	sizeAlign->setMinimumHeight(32);
	sizeAlign->setMaximumHeight(32);
	sizeAlign->setMinimumWidth(32);
	sizeAlign->setMaximumWidth(32);
	QIcon icon9;
	icon9.addFile(QString::fromUtf8(":/ssp_gui/Resources/rpt_Salign.png"), QSize(), QIcon::Normal, QIcon::Off);
	sizeAlign->setIcon(icon9);

	layoutleft->addWidget(rectDraw);
	layoutleft->addWidget(lineHDraw);
	layoutleft->addWidget(lineVDraw);
	layoutleft->addWidget(lineDraw);
	layoutleft->addWidget(textDraw);
	layoutleft->addWidget(expDraw);
	layoutleft->addWidget(chartDraw);
	layoutleft->addWidget(Halign);
	layoutleft->addWidget(Valign);
	layoutleft->addWidget(sizeAlign);
	layoutleft->addStretch();
	layoutleft->setMargin(2);
	leftWidget->setLayout(layoutleft);

	QHBoxLayout *layout1 = new QHBoxLayout();
	layout1->addWidget(leftWidget);
	layout1->addWidget(m_view);
	//layout1->addStretch();
	layout1->setMargin(2);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(topWidget);
	layout->addLayout(layout1);
	layout->addStretch();
	setLayout(layout);

	SRecordset reports;
	m_pOper->RetrieveRecordset("select report_no,report_name from t_ssp_report_info order by report_no",reports);

	reportCount=reports.GetRows();
	for(int i=0;i<reportCount;i++)
	{
		selreport->addItem(QString((reports.GetValue(i,0)+":"+reports.GetValue(i,1)).data()));
	}

	connect(newreport,SIGNAL(clicked()),this,SLOT(onPushButton_new()));
	connect(selreport,SIGNAL(currentIndexChanged(int)),this,SLOT(initPaint(int)));
	connect(chgName,SIGNAL(clicked()),this,SLOT(onPushButton_chgName()));
	connect(delreport,SIGNAL(clicked()),this,SLOT(onPushButton_delreport()));
	connect(delElement,SIGNAL(clicked()),this,SLOT(onPushButton_delElm()));
	connect(sqlElement,SIGNAL(clicked()),this,SLOT(onPushButton_sql()));
	connect(groupElement,SIGNAL(clicked()),this,SLOT(onPushButton_group()));
	connect(pageSetting,SIGNAL(clicked()),this,SLOT(onPushButton_page()));

	connect(rectDraw,SIGNAL(clicked()),this,SLOT(onPushButton_rect()));
	connect(lineHDraw,SIGNAL(clicked()),this,SLOT(onPushButton_lineH()));
	connect(lineVDraw,SIGNAL(clicked()),this,SLOT(onPushButton_lineV()));
	connect(lineDraw,SIGNAL(clicked()),this,SLOT(onPushButton_line()));
	connect(textDraw,SIGNAL(clicked()),this,SLOT(onPushButton_text()));
	connect(chartDraw,SIGNAL(clicked()),this,SLOT(onPushButton_chart()));
	connect(expDraw,SIGNAL(clicked()),this,SLOT(onPushButton_exp()));
	connect(Halign,SIGNAL(clicked()),this,SLOT(onPushButton_Halign()));
	connect(Valign,SIGNAL(clicked()),this,SLOT(onPushButton_Valign()));
	connect(sizeAlign,SIGNAL(clicked()),this,SLOT(onPushButton_setSize()));
	connect(m_scene,SIGNAL(itemInserted(QGraphicsRectItem*)),this,SLOT(itemInserted(QGraphicsRectItem*)));
	
}

view_reportsetting::~view_reportsetting()
{

}
//�ı�ҳ��߶�
void view_reportsetting::changeHW(int height,int width)
{
	this->height=height;
	this->width=width;
	m_view->setSceneRect(QRectF(0,0,width,height));
	int realX=m_view->size().width();
	int realY=m_view->size().height();
	int tranX=0;int tranY=0;
	if(realX>width)
		tranX=realX-width;
	if(realY>height)
		tranY=realY-height;
	m_view->translate(tranX,tranY);
	m_scene->setSceneRect(QRectF(0,0,width,height));
}
//�½�����
void view_reportsetting::onPushButton_new()
{
	bool ok;

	QString name=QInputDialog::getText(0,tr("��������"),tr("����������:"),QLineEdit::Normal,"",&ok,0);  
	int newReportNo=m_pOper->SelectInto("select max(report_no) from t_ssp_report_info").toInt()+1;
	if(!name.isEmpty()) 
	{
		reportCount++;
		selreport->addItem(QString::number(newReportNo)+QString(":")+name);
	}
	else
		return;
	m_scene->clear();
	selreport->setCurrentIndex(selreport->count()-1);
	
	QString size="1200,800";
	width=1200;
	height=800;
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_info (report_no,report_name,rpt_size) \
										  values (%d,'%s','%s')",newReportNo,name.toStdString().data(),size.toStdString().data()));
	m_view->setSceneRect(QRectF(0,0,width,height));
	reportNo=newReportNo;
	initNewPage(reportNo);
}
void view_reportsetting::initNewPage(int reportNo)
{
	m_scene->initScreen();
	m_scene->setNo(reportNo);
	m_scene->initNewPage(selreport->currentText());
}
void view_reportsetting::onPushButton_chgName()
{
	if(reportNo<1)
		return;
	QString curName=m_pOper->SelectInto(SString::toFormat("select report_name from t_ssp_report_info where report_no=%d",reportNo)).data();
	bool ok;
	QString name=QInputDialog::getText(0,tr("�޸��ı�"),tr("���������ı�:"),QLineEdit::Normal,curName,&ok,0);  
	if(!name.isEmpty())
	{
		m_pOper->Execute(SString::toFormat("update t_ssp_report_info set report_name='%s'where report_no=%d",name.toStdString().data(),reportNo));
		selreport->setItemText(selreport->currentIndex(),QString::number(reportNo)+QString(":")+name);
	}
}
void view_reportsetting::initPaint(int cont)
{
	SRecordset reports,items;
	m_scene->clear();
	if(cont==0)
	{
		reportNo=0;
		return;
	}
	m_pOper->RetrieveRecordset("select report_no,report_name,ds_no,rpt_size from t_ssp_report_info order by report_no",reports);
	reportNo=reports.GetValue(cont-1,0).toInt();//��һ����Ĭ�Ͽ�
	dsNo=reports.GetValue(cont-1,2).toInt();
	SString s13=reports.GetValue(cont-1,3);
	width=SString::GetIdAttributeI(1,s13,",");
	height=SString::GetIdAttributeI(2,s13,",");
// 	m_scene->setSceneRect(QRectF(0,0,width,height));
// 	m_view->setSceneRect(QRectF(0,0,width,height));

	int realX=m_view->size().width();
	int realY=m_view->size().height();
	int tranX=width;int tranY=height;
	if(realX>width)
		tranX=realX;
	if(realY>height)
		tranY=realY;
	m_scene->setSceneRect(QRectF(0,0,tranX,tranY));
	m_view->setSceneRect(QRectF(0,0,tranX,tranY));

	m_pOper->RetrieveRecordset(SString::toFormat("select item_no,report_no,pos,type,rect,info from t_ssp_report_item where report_no=%d order by seg+0",reportNo),items);
	m_scene->setNo(reportNo);

	CSsp_Dataset* dbSet=NULL;
	SString dsName=m_pOper->SelectInto(SString::toFormat("select name from t_ssp_data_window where dw_sn=%d",dsNo));
	dbSet=m_dataWindow->SearchDataset(dsName);

	m_scene->setDsNo(dbSet);
	m_scene->saveAllDs(dbSet);
	for(int i=0;i<items.GetRows();i++)
	{
		m_scene->initPaint(items.GetValue(i,0).toInt(),items.GetValue(i,3).toInt(),items.GetValue(i,2).data(),items.GetValue(i,4).data(),items.GetValue(i,5).data());
	}
	
}
//ɾ����ǰ����
void view_reportsetting::onPushButton_delreport()
{
	if (QMessageBox::No==QMessageBox::warning(this,tr("����"),tr("�Ƿ�ɾ����ǰͼ��"),QMessageBox::Yes,QMessageBox::No))
		return;
	if(selreport->currentIndex()==0)
		return;
	int tmpNo=reportNo;
	int selNo=selreport->currentIndex();
	selreport->setCurrentIndex(0);
	selreport->removeItem(selNo);
	m_scene->initScreen();
	m_pOper->Execute(SString::toFormat("delete from t_ssp_report_info where report_no=%d",tmpNo));
	m_pOper->Execute(SString::toFormat("delete from t_ssp_report_item where report_no=%d",tmpNo));

}
//ɾ��ѡ��Ԫ��
void view_reportsetting::onPushButton_delElm()
{
	if (QMessageBox::No==QMessageBox::warning(this,tr("����"),tr("�Ƿ�ɾ����ѡԪ�أ�"),QMessageBox::Yes,QMessageBox::No))
		return;
	m_scene->delSelElm();
}
//ѡ�����ݼ�
void view_reportsetting::onPushButton_sql()
{
	if(selreport->currentIndex()==0)
		return;
	sql_dlg->setDsNo(dsNo);
	sql_dlg->exec();
	dsNo=sql_dlg->getNo();
	if(dsNo!=0)
	{
		m_pOper->Execute(SString::toFormat("update t_ssp_report_info set ds_no=%d where report_no=%d",dsNo,reportNo));
		CSsp_Dataset* dbSet=NULL;//�Ƿ�Ҫdelete
		SString dsName=m_pOper->SelectInto(SString::toFormat("select name from t_ssp_data_window where dw_sn=%d",dsNo));
		dbSet=m_dataWindow->SearchDataset(dsName);
		m_scene->setDsNo(dbSet);
		m_scene->saveAllDs(dbSet);
	}

}
//���飨ֻ֧��һ�أ�
void view_reportsetting::onPushButton_group()
{
	if(selreport->currentIndex()==0)
		return;
	if(dsNo==-1)
		return;
	if(group_dlg==NULL)
	{
		group_dlg=new settingGroup_dlg(dsNo,this);
		connect(group_dlg,SIGNAL(deleteGrp()),m_scene,SLOT(deleteGrp()));
	}
	SRecordset grpSets;
	m_pOper->RetrieveRecordset("select item_no,info from t_ssp_report_item where seg='100'",grpSets);//��ʱֻ����һ������

	QString info=grpSets.GetValue(0,1).data();
	QString fontSize=getAtu(info,"fontsize");//�����С
	group_dlg->setFontSize(fontSize);

	QString fontFamily=getAtu(info,"fontfamily");//����
	group_dlg->setFontFamily(fontFamily);

	QString colorStr=getAtu(info,"color");//������ɫ
	QStringList lstColorStr=colorStr.split(",");
	if(lstColorStr.size()>=3)
	{
		QColor color(lstColorStr.at(0).toInt(),lstColorStr.at(1).toInt(),lstColorStr.at(2).toInt());
		group_dlg->setColor(color);
	}
	else
	{
		group_dlg->setColor(QColor(0,0,0));
	}

	QString fontType=getAtu(info,"fonttype");//�������ͣ��Ӵ�б�壩
	group_dlg->setFontType(fontType.toInt());

	QString txtop=getAtu(info,"txtop");//������
	group_dlg->setTxtOp(txtop.toInt());

	QString delLine=getAtu(info,"delline");//ɾ����
	if(delLine=="true")
		group_dlg->setFontDelLine(true);
	else
		group_dlg->setFontDelLine(false);

	QString undLine=getAtu(info,"undline");//�»���
	if(undLine=="true")
		group_dlg->setFontUndLine(true);
	else
		group_dlg->setFontUndLine(false);

	QString isRect=getAtu(info,"isrect");//���
	group_dlg->setIsRect(isRect.toInt());

	QString rectPx=getAtu(info,"rectpx");//����ϸ
	group_dlg->setRectpx(rectPx.toInt());

	QString rectColorStr=getAtu(info,"rectcor");//�����ɫ��û��color���ܱ���
	QStringList lstRectColorStr=rectColorStr.split(",");
	if(lstRectColorStr.size()>=3)
	{
		QColor rectColor(lstRectColorStr.at(0).toInt(),lstRectColorStr.at(1).toInt(),lstRectColorStr.at(2).toInt());
		group_dlg->setRectColor(rectColor);
	}
	else
	{
		group_dlg->setRectColor(QColor(0,0,0));
	}
	QString txt=getAtu(info,"txt");//����ϸ
	group_dlg->setTxt(txt);

	QString grp=getAtu(info,"grp");//����ϸ
	group_dlg->setGrp(grp);

	group_dlg->setDs(dsNo);
	group_dlg->initDlg();
	group_dlg->exec();
	//�ӶԻ����ȡ����
	QString newInfo;
	if(group_dlg->getTxt()!="")
	{
		QString txt=group_dlg->getTxt();
		QString newInfo=setValue(info,"txt",txt);
		info=newInfo;
	}
	fontSize=group_dlg->getFontSize();
	newInfo=setValue(info,"fontsize",fontSize);
	info=newInfo;

	fontFamily=group_dlg->getFontFamily();
	newInfo=setValue(info,"fontfamily",fontFamily);
	info=newInfo;

	QColor color=group_dlg->getColor();
	colorStr=QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue());
	newInfo=setValue(info,"color",colorStr);
	info=newInfo;

	fontType=QString::number(group_dlg->getFontType());
	newInfo=setValue(info,"fonttype",fontType);
	info=newInfo;

	txtop=QString::number(group_dlg->getTxtOp());
	newInfo=setValue(info,"txtop",txtop);
	info=newInfo;

	bool delline=group_dlg->getFontDelLine();
	if(delline)
		newInfo=setValue(info,"delline","true");
	else
		newInfo=setValue(info,"delline","false");
	info=newInfo;

	bool undline=group_dlg->getFontUndLine();
	if(undline)
		newInfo=setValue(info,"undline","true");
	else
		newInfo=setValue(info,"undline","false");
	info=newInfo;

	isRect=QString::number(group_dlg->getIsRect());
	newInfo=setValue(info,"isrect",isRect);
	info=newInfo;

	rectPx=QString::number(group_dlg->getRectpx());
	newInfo=setValue(info,"rectpx",rectPx);
	info=newInfo;

	QColor rectColor=group_dlg->getRectColor();
	rectColorStr=QString::number(rectColor.red())+","+QString::number(rectColor.green())+","+QString::number(rectColor.blue());
	newInfo=setValue(info,"rectcor",rectColorStr);
	info=newInfo;

	if(group_dlg->getGrp()!="")
	{
		QString grp=group_dlg->getGrp();
		m_scene->drawDiv(grp,dsNo,info);
	}
}
//���ø߿�
void view_reportsetting::onPushButton_page()
{
	if(selreport->currentIndex()==0)
		return;
	if(page_dlg==NULL)
		page_dlg=new pagesetting_dlg(this);
	page_dlg->setWH(width,height);
	page_dlg->exec();
	page_dlg->getWH(width,height);
	//m_scene->setSceneRect(QRectF(0,0,width,height));
	QString size=QString::number(width)+","+QString::number(height);
	m_pOper->Execute(SString::toFormat("update t_ssp_report_info set rpt_size='%s' where report_no=%d",size.toStdString().data(),reportNo));
	//m_view->setSceneRect(QRectF(0,0,width,height));
	int realX=m_view->size().width();
	int realY=m_view->size().height();
	int tranX=width;int tranY=height;
	if(realX>width)
		tranX=realX;
	if(realY>height)
		tranY=realY;
	m_scene->setSceneRect(QRectF(0,0,tranX,tranY));
	m_view->setSceneRect(QRectF(0,0,tranX,tranY));
	//m_view->translate(tranX*2,tranY);
	//m_view->centerOn(0,0);
}
//�����Σ�unuse��
void view_reportsetting::onPushButton_rect()
{
	if(selreport->currentIndex()!=0)
		m_scene->setMode(MyGraphicsScene::InsertRect);
}
//����
void view_reportsetting::onPushButton_lineH()
{	
	if(selreport->currentIndex()!=0)
		m_scene->setMode(MyGraphicsScene::InsertHLine);
}
//����
void view_reportsetting::onPushButton_lineV()
{
	if(selreport->currentIndex()!=0)
		m_scene->setMode(MyGraphicsScene::InsertVLine);
}
//�ı�
void view_reportsetting::onPushButton_text()
{
	if(selreport->currentIndex()!=0)
		m_scene->setMode(MyGraphicsScene::InsertText);
}
//����
void view_reportsetting::onPushButton_line()
{
	if(selreport->currentIndex()!=0)
		m_scene->setMode(MyGraphicsScene::InsertLine);
}
//���ʽ
void view_reportsetting::onPushButton_exp()
{
	if(selreport->currentIndex()!=0)
		m_scene->setMode(MyGraphicsScene::InsertExp);
}
//ͼ��
void view_reportsetting::onPushButton_chart()
{
	if(selreport->currentIndex()!=0)
		m_scene->setMode(MyGraphicsScene::InsertChart);
}
//�����
void view_reportsetting::onPushButton_Halign()
{
	if(selreport->currentIndex()!=0)
		m_scene->setHalign();
}
//������
void view_reportsetting::onPushButton_Valign()
{
	if(selreport->currentIndex()!=0)
		m_scene->setValign();
}
//��С����
void view_reportsetting::onPushButton_setSize()
{
	if(selreport->currentIndex()!=0)
		m_scene->setSameSize();
}
///////////////////////////////////////////////////////////////////////////////////
MyGraphicsScene::MyGraphicsScene(QObject *parent)
	: QGraphicsScene(parent)
{
	setSceneRect(QRectF(0,0,1200,700));
	m_pOper=DB;
	myMode = MoveItem;
	line = 0;
	textItem = 0;
	divList=new QList<divStruct>();
	itemCount=0;
	myItemColor = Qt::white;
	myTextColor = Qt::black;
	myLineColor = Qt::black;
	dbSet=NULL;
	connect(this,SIGNAL(selectionChanged()),this,SLOT(selListChanged()));
}
MyGraphicsScene::~MyGraphicsScene()
{
	divList->clear();
	delete divList;
}
void MyGraphicsScene::initScreen()
{
	clear();
	itemCount=0;
	divList->clear();
}
//���÷���
void MyGraphicsScene::drawDiv(QString dsName,int dsNo,QString info)
{
	itemCount++;int count=0;
	m_pOper->ExecuteSQL("delete from t_ssp_report_item where seg='100'");
	int height=divList->at(count).height;
	QList<QGraphicsItem *>items=this->items();

	SString dsDesc=m_pOper->SelectInto(SString::toFormat("select col_name from t_ssp_data_window_col where col_sn=%d and dw_sn=%d",dsName.toInt()+1,dsNo));
/*	drawGraphics(QPointF(0,height+18),QRectF(0,0,5000,18),5,reportNo,itemCount,SString::toFormat("txt=����:%s;fontsize=10;",dsDesc.data()).data());*/
	QString pos="0,"+QString::number(height+18);
	QString rect="0,0,5000,18";
	SString divNum="100";
	
	QString newInfo=setValue(info,"grp",dsName);

	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),5,divNum.data(),rect.toStdString().data(),newInfo.toStdString().data()));
	QString sInfo=m_pOper->SelectInto(SString::toFormat("select info from t_ssp_report_item where type='5' and seg='2' and report_no=%d",reportNo)).data();
	QString newsInfo=setValue(sInfo,"txt",QString("���ݼ�+�����У�")+dsName);
	m_pOper->ExecuteSQL(SString::toFormat("update t_ssp_report_item set info='%s' where type='5' and seg='2'and report_no=%d",newsInfo.toStdString().data(),reportNo));

	int itemNo1=m_pOper->SelectIntoI(SString::toFormat("select item_no from t_ssp_report_item where type='5' and seg='2' and report_no=%d",reportNo));
	for(int i=0;i<items.size();i++)
	{
		int reportNo;int itemNo;
		((MyGraphicsItem*)items.at(i))->getNo(reportNo,itemNo);
		if(itemNo==itemNo1)
		{
			((MyGraphicsItem*)items.at(i))->setTxt(QString("���ݼ�+�����У�")+dsName);
			break;
		}
	}
// 	divStruct div2(divNum.data(),itemCount,height+18);
// 	divList->insert(count+1,div2);
}
//���Ƴ�ʼҳ���Ԫ��
void MyGraphicsScene::initNewPage(QString curName)
{
	itemCount++;
	drawGraphics(QPointF(0,0),QRectF(0,0,5000,18),5,reportNo,itemCount,"txt=��ͷ;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;");
	QString pos="0,0";
	QString rect="0,0,5000,18";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),5,"0",rect.toStdString().data(),"txt=��ͷ;fontsize=10;"));
	divStruct div0("0",itemCount,0);
	divList->append(div0);

	itemCount++;
	drawGraphics(QPointF(406,18),QRectF(-62,1,217,44),4,reportNo,itemCount,"txt=���վ;fontsize=18;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;fonttype=1;");
	pos="406,18";
	rect="-62,1,217,44";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),4,"1",rect.toStdString().data(),"txt=���վ;fontsize=20;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;fonttype=1;"));

	itemCount++;
	drawGraphics(QPointF(462,63),QRectF(-117,2,217,37),4,reportNo,itemCount,QString("txt=%1;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;fonttype=1;").arg(curName));
	pos="462,63";
	rect="-117,2,217,37";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),4,"1",rect.toStdString().data(),QString("txt=%1;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;fonttype=1;").arg(curName).toStdString().data()));

	itemCount++;
	drawGraphics(QPointF(0,139),QRectF(0,0,5000,18),5,reportNo,itemCount,"txt=ҳͷ;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;");
	pos="0,139";
	rect="0,0,5000,18";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),5,"1",rect.toStdString().data(),"txt=ҳͷ;fontsize=10;"));
	divStruct div1("1",itemCount,139);
	divList->append(div1);

	itemCount++;
	drawGraphics(QPointF(12,101),QRectF(0,16,82,21),4,reportNo,itemCount,"txt=��1;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;");
	pos="12,101";
	rect="0,16,82,21";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),4,"1",rect.toStdString().data(),"txt=��1;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;"));

	itemCount++;
	drawGraphics(QPointF(117,101),QRectF(0,16,82,21),4,reportNo,itemCount,"txt=��2;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;");
	pos="117,101";
	rect="0,16,82,21";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),4,"1",rect.toStdString().data(),"txt=��2;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;"));

	itemCount++;
	drawGraphics(QPointF(222,101),QRectF(0,16,82,21),4,reportNo,itemCount,"txt=��3;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;");
	pos="222,101";
	rect="0,16,82,21";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),4,"1",rect.toStdString().data(),"txt=��3;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;"));

	itemCount++;
	drawGraphics(QPointF(0,182),QRectF(0,0,5000,18),5,reportNo,itemCount,"txt=���ݼ�;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;");
	pos="0,182";
	rect="0,0,5000,18";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),5,"2",rect.toStdString().data(),"txt=���ݼ�;fontsize=10;"));
	divStruct div2("2",itemCount,182);
	divList->append(div2);

	itemCount++;
	drawGraphics(QPointF(13,144),QRectF(0,16,82,21),6,reportNo,itemCount,"txt=���ʽ1;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;");
	pos="13,144";
	rect="0,16,82,21";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),6,"2",rect.toStdString().data(),"txt=���ʽ1;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;"));

	itemCount++;
	drawGraphics(QPointF(116,143),QRectF(0,16,82,21),6,reportNo,itemCount,"txt=���ʽ2;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;");
	pos="116,143";
	rect="0,16,82,21";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),6,"2",rect.toStdString().data(),"txt=���ʽ2;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;"));

	itemCount++;
	drawGraphics(QPointF(222,144),QRectF(0,16,82,21),6,reportNo,itemCount,"txt=���ʽ3;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;");
	pos="222,144";
	rect="0,16,82,21";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),6,"2",rect.toStdString().data(),"txt=���ʽ3;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;"));

	itemCount++;
	drawGraphics(QPointF(0,236),QRectF(0,0,5000,18),5,reportNo,itemCount,"txt=�ܼ�;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;");
	pos="0,236";
	rect="0,0,5000,18";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),5,"3",rect.toStdString().data(),"txt=�ܼ�;fontsize=10;"));
	divStruct div3("3",itemCount,236);
	divList->append(div3);

	itemCount++;
	drawGraphics(QPointF(14,210),QRectF(0,0,81,20),6,reportNo,itemCount,"txt=�ܼ�;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;");
	pos="14,210";
	rect="0,0,81,20";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),4,"3",rect.toStdString().data(),"txt=�ܼ�;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;"));

	itemCount++;
	drawGraphics(QPointF(0,479),QRectF(0,0,5000,18),5,reportNo,itemCount,"txt=ҳβ;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;");
	pos="0,479";
	rect="0,0,5000,18";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),5,"4",rect.toStdString().data(),"txt=ҳβ;fontsize=10;"));
	divStruct div4("4",itemCount,479);
	divList->append(div4);

	itemCount++;
	drawGraphics(QPointF(9,110),QRectF(0,0,566,1),2,reportNo,itemCount,"px=2;type=0;color=0,0,0;");
	pos="9,110";
	rect="0,0,566,1";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),1,"4",rect.toStdString().data(),"px=2;type=0;color=0,0,0;"));

	itemCount++;
	drawGraphics(QPointF(9,206),QRectF(0,0,566,1),2,reportNo,itemCount,"px=2;type=0;color=0,0,0;");
	pos="9,206";
	rect="0,0,566,1";
	m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,seg,rect,info) values (%d,%d,'%s',%d,'%s','%s','%s')",itemCount,reportNo,pos.toStdString().data(),1,"4",rect.toStdString().data(),"px=2;type=0;color=0,0,0;"));

	//dsNo=-1;
	m_pOper->ExecuteSQL(SString::toFormat("update t_ssp_report_info set ds_no=%d where report_no=%d",-1,reportNo));
}
//ɾ��Ԫ��
void MyGraphicsScene::delSelElm()
{

	QList<QGraphicsItem *>items=this->selectedItems();
	for(int i=0;i<items.size();i++)
	{
		
		int reportNo;int itemNo;
		((MyGraphicsItem*)items.at(i))->getNo(reportNo,itemNo);
		if(((MyGraphicsItem*)items.at(i))->getType()==5)//����ɾ���ָ���
		{
			continue;
// 			for(int j=0;j<divList->size();j++)
// 			{
// 				if(itemNo==divList->at(j).itemNo)
// 				{
// 					divList->removeAt(j);
// 					break;
// 				}
// 			}
		}
		removeItem(items.at(i));
		delete items.at(i);
		m_pOper->Execute(SString::toFormat("delete from t_ssp_report_item where report_no=%d and item_no=%d",reportNo,itemNo));
	}
	update();
}
//���ú����
void MyGraphicsScene::setHalign()
{
	//QList<QGraphicsItem *>items=this->selectedItems();
	if(selItems.size()>0)
	{
		QPointF point=selItems.at(0)->pos();
		qreal y=point.y();
		for(int i=0;i<selItems.size();i++)
		{
			selItems.at(i)->setPos(selItems.at(i)->pos().x(),y);
			int itemNo=0;int reportNo=0;
			((MyGraphicsItem*)selItems.at(i))->getNo(reportNo,itemNo);
			QString pos=QString::number(selItems.at(i)->pos().x())+","+QString::number(y);
			m_pOper->Execute(SString::toFormat("update t_ssp_report_item set pos='%s' where item_no=%d and report_no=%d",pos.toStdString().data(),itemNo,reportNo));
		}
	}
	update();
}
//������
void MyGraphicsScene::setValign()
{
	//QList<QGraphicsItem *>items=this->selectedItems();
	if(selItems.size()>0)
	{
		QPointF point=selItems.at(0)->pos();
		qreal x=point.x();
		for(int i=0;i<selItems.size();i++)
		{
			selItems.at(i)->setPos(x,selItems.at(i)->pos().y());
			int itemNo=0;int reportNo=0;
			((MyGraphicsItem*)selItems.at(i))->getNo(reportNo,itemNo);
			QString pos=QString::number(x)+","+QString::number(selItems.at(i)->pos().y());
			m_pOper->Execute(SString::toFormat("update t_ssp_report_item set pos='%s' where item_no=%d and report_no=%d",pos.toStdString().data(),itemNo,reportNo));
		}
	}
	update();
}
//��С����
void MyGraphicsScene::setSameSize()
{
	//QList<QGraphicsItem *>items=this->selectedItems();
	if(selItems.size()>0)
	{
		QRectF rect=((MyGraphicsItem*)selItems.at(0))->rect();
		for(int i=0;i<selItems.size();i++)
		{
			((MyGraphicsItem*)selItems.at(i))->ResetRect(rect);
			int itemNo=0;int reportNo=0;
			((MyGraphicsItem*)selItems.at(i))->getNo(reportNo,itemNo);
			QString rect1=QString::number(rect.x())+","+QString::number(rect.y())+","+QString::number(rect.width())+","+QString::number(rect.height());
			m_pOper->Execute(SString::toFormat("update t_ssp_report_item set rect='%s' where item_no=%d and report_no=%d",rect1.toStdString().data(),itemNo,reportNo));
		}
	}
	update();
}
//���书�ܰ���һ��Ԫ��Ϊ��׼���Դ˺���ȷ����һ��Ԫ��
void MyGraphicsScene::selListChanged()
{
	QList<QGraphicsItem *>items=this->selectedItems();//������Ҫȷ����һ��
	for(int i=0;i<items.size();i++)
	{
		MyGraphicsItem* item=(MyGraphicsItem*)items.at(i);
		int itemNo=0;int reportNo=0;
		item->getNo(reportNo,itemNo);
		bool isExist=false;
		for(int j=0;j<selItems.size();j++)
		{
			MyGraphicsItem* selItem=(MyGraphicsItem*)selItems.at(j);
			int selItemNo=0;int selReportNo=0;
			selItem->getNo(selReportNo,selItemNo);
			if(selItemNo==itemNo)
			{
				isExist=true;
				break;
			}
		}
		if(isExist==false)
			selItems.append(item);
	}
	for(int i=0;i<selItems.size();i++)
	{			
		MyGraphicsItem* selItem=(MyGraphicsItem*)selItems.at(i);
		int selItemNo=0;int selReportNo=0;
		selItem->getNo(selReportNo,selItemNo);
		bool isExist=false;
		for(int j=0;j<items.size();j++)
		{
			MyGraphicsItem* item=(MyGraphicsItem*)items.at(j);
			int itemNo=0;int reportNo=0;
			item->getNo(reportNo,itemNo);
			if(selItemNo==itemNo)
			{
				isExist=true;
				break;
			}
		}
		if(isExist==false)
		{
			selItems.removeOne(selItem);
			i=0;
		}
	}
}
//����ͼ��
void MyGraphicsScene::drawGraphics(QPointF pos,QRectF rect,int type,int reportNo,int itemNo,QString info)
{
	MyGraphicsItem *item= new MyGraphicsItem();
	addItem(item);
	QFont myFont;
	item->setType(type);
	item->setNo(reportNo,itemNo);
	item->setPos(pos);
	item->setDivList(divList);
	item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	item->setFlag(QGraphicsItem::ItemIsSelectable, true);
	connect(item,SIGNAL(refresh()),this,SLOT(refresh()));
	connect(item,SIGNAL(saveAllPos()),this,SLOT(saveAllPos()));
	connect(item,SIGNAL(moveItems(float,float)),this,SLOT(moveItems(float,float)));
	if(type==2||type==3||type==7)
	{
		item->setLinePx(getAtu(info,"px").toInt());
		QString colorStr=getAtu(info,"color");
		QStringList lstColorStr=colorStr.split(",");
		if(lstColorStr.size()>=3)
		{
			QColor color(lstColorStr.at(0).toInt(),lstColorStr.at(1).toInt(),lstColorStr.at(2).toInt());
			item->setColor(color);
			item->setLineType(getAtu(info,"type").toInt());
		}
	}
	else if(type==4)
	{
		item->setTxt(getAtu(info,"txt"));
		QString colorStr=getAtu(info,"color");
		QStringList lstColorStr=colorStr.split(",");
		if(lstColorStr.size()>=3)
		{
			QColor color(lstColorStr.at(0).toInt(),lstColorStr.at(1).toInt(),lstColorStr.at(2).toInt());
			item->setColor(color);
		}
		QString txtOp=getAtu(info,"txtop");
		item->setTxtop(txtOp.toInt());
		myFont.setPointSize(getAtu(info,"fontsize").toInt());
		myFont.setFamily(getAtu(info,"fontfamily"));
		QString fontType=getAtu(info,"fonttype");
		if(fontType=="0")
		{
			myFont.setBold(false);
			myFont.setItalic(false);
		}
		else if(fontType=="1")
		{
			myFont.setBold(true);
			myFont.setItalic(false);
		}
		else if(fontType=="2")
		{
			myFont.setBold(false);
			myFont.setItalic(true);
		}
		else if(fontType=="3")
		{
			myFont.setBold(true);
			myFont.setItalic(true);
		}
		if(getAtu(info,"delline")=="true")
			myFont.setStrikeOut(true);
		else
			myFont.setStrikeOut(false);
		if(getAtu(info,"undline")=="true")
			myFont.setUnderline(true);
		else
			myFont.setUnderline(false);
		item->setFont(myFont);
	}
	else if(type==5)
	{
		item->setTxt(getAtu(info,"txt"));
		myFont.setPointSize(getAtu(info,"fontsize").toInt());
		myFont.setFamily(getAtu(info,"family"));
		item->setFont(myFont);
		connect(item,SIGNAL(divChanged(QPointF,int)),this,SLOT(divChanged(QPointF,int)));
		connect(item,SIGNAL(changeHeight(int)),this,SLOT(changeHeight(int)));
	}
	else if(type==6)
	{
		item->setTxt(getAtu(info,"txt"));
		QString colorStr=getAtu(info,"color");
		QStringList lstColorStr=colorStr.split(",");
		if(lstColorStr.size()>=3)
		{
			QColor color(lstColorStr.at(0).toInt(),lstColorStr.at(1).toInt(),lstColorStr.at(2).toInt());
			item->setColor(color);
		}
		QString txtOp=getAtu(info,"txtop");
		item->setTxtop(txtOp.toInt());
		myFont.setPointSize(getAtu(info,"fontsize").toInt());
		myFont.setFamily(getAtu(info,"fontfamily"));
		QString fontType=getAtu(info,"fonttype");
		if(fontType=="0")
		{
			myFont.setBold(false);
			myFont.setItalic(false);
		}
		else if(fontType=="1")
		{
			myFont.setBold(true);
			myFont.setItalic(false);
		}
		else if(fontType=="2")
		{
			myFont.setBold(false);
			myFont.setItalic(true);
		}
		else if(fontType=="3")
		{
			myFont.setBold(true);
			myFont.setItalic(true);
		}
		if(getAtu(info,"delline")=="true")
			myFont.setStrikeOut(true);
		else
			myFont.setStrikeOut(false);
		if(getAtu(info,"undline")=="true")
			myFont.setUnderline(true);
		else
			myFont.setUnderline(false);
		item->setFont(myFont);
	}
	else if(type==8)
	{
		item->setTxt(getAtu(info,"txt"));
		int type=getAtu(info,"charttype").toInt();
		item->setChartType(type);
		QString chartDs=getAtu(info,"chartds");
		item->setChartDs(chartDs);
	}
	item->setAtu(info);
	if(type!=5)
		item->ResetRect(rect);
	else
	{
		QRectF tmpRect;
		tmpRect.setX(rect.x());
		tmpRect.setWidth(width());
		tmpRect.setHeight(rect.height());
		tmpRect.setY(rect.y());
		item->ResetRect(tmpRect);
	}
	item->setDs(dbSet);
	myMode=MoveItem;
}
void MyGraphicsScene::deleteGrp()
{
	m_pOper->ExecuteSQL("delete from t_ssp_report_item where seg='100'");
	QString sInfo=m_pOper->SelectInto(SString::toFormat("select info from t_ssp_report_item where type='5' and seg='2' and report_no=%d",reportNo)).data();
	QString newsInfo=setValue(sInfo,"txt",QString("���ݼ�"));
	m_pOper->ExecuteSQL(SString::toFormat("update t_ssp_report_item set info='%s'where where type='5' and seg='2'and report_no=%d",newsInfo.toStdString().data(),reportNo));

	int itemNo1=m_pOper->SelectIntoI(SString::toFormat("select item_no from t_ssp_report_item where type='5' and seg='2' and report_no=%d",reportNo));
	for(int i=0;i<this->items().size();i++)
	{
		int reportNo;int itemNo;
		((MyGraphicsItem*)this->items().at(i))->getNo(reportNo,itemNo);
		if(itemNo==itemNo1)
		{
			((MyGraphicsItem*)this->items().at(i))->setTxt(QString("���ݼ�"));
			break;
		}
	}
}

//�ƶ�item����ѡ�ƶ��ã�
void MyGraphicsScene::moveItems(float dx,float dy)
{
	QList<QGraphicsItem *>items=this->selectedItems();
	for(int i=0;i<items.size();i++)
	{
		((MyGraphicsItem*)items.at(i))->moveItem(dx,dy);
	}
}
//���ø߶�
void MyGraphicsScene::changeHeight(int height)
{
	QRectF rectf=sceneRect();
	rectf.setHeight(height);
	
	emit changeHW(height,rectf.width());
	//setSceneRect(rectf);
}
//������Ӧ�����Ԫ���ã�
void MyGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return;
	
	QString pos;
	QString rect;
	switch (myMode) {

	case InsertRect:
		itemCount++;
		drawGraphics(mouseEvent->scenePos(),QRectF(0,0,50,50),1,reportNo,itemCount,"");
		pos=QString::number(mouseEvent->scenePos().x())+","+QString::number(mouseEvent->scenePos().y());
		rect="0,0,50,50";
		m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,rect,info) values (%d,%d,'%s',%d,'%s','%s')",itemCount,reportNo,pos.toStdString().data(),1,rect.toStdString().data(),""));
		break;

	case InsertHLine:
		itemCount++;
		drawGraphics(mouseEvent->scenePos(),QRectF(0,0,100,1),2,reportNo,itemCount,"px=2;type=0;color=0,0,0;");
		pos=QString::number(mouseEvent->scenePos().x())+","+QString::number(mouseEvent->scenePos().y());
		rect="0,0,100,1";
		m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,rect,info) values (%d,%d,'%s',%d,'%s','%s')",itemCount,reportNo,pos.toStdString().data(),2,rect.toStdString().data(),"px=2;type=0;color=0,0,0;"));
		break;

	case InsertVLine:
		itemCount++;
		drawGraphics(mouseEvent->scenePos(),QRectF(0,0,1,100),3,reportNo,itemCount,"px=2;type=0;color=0,0,0;");
		pos=QString::number(mouseEvent->scenePos().x())+","+QString::number(mouseEvent->scenePos().y());
		rect="0,0,1,100";
		m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,rect,info) values (%d,%d,'%s',%d,'%s','%s')",itemCount,reportNo,pos.toStdString().data(),3,rect.toStdString().data(),"px=2;type=0;color=0,0,0;"));
		break;

 	case InsertText:
		itemCount++;
		drawGraphics(mouseEvent->scenePos(),QRectF(0,0,80,20),4,reportNo,itemCount,"txt=txt;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;");
		pos=QString::number(mouseEvent->scenePos().x())+","+QString::number(mouseEvent->scenePos().y());
		rect="0,0,80,20";
		m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,rect,info) values (%d,%d,'%s',%d,'%s','%s')",itemCount,reportNo,pos.toStdString().data(),4,rect.toStdString().data(),"txt=txt;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;"));
		break;

// 	case InsertDiv:
// 		itemCount++;
// 		drawGraphics(QPointF(0,mouseEvent->scenePos().y()),QRectF(0,0,5000,18),5,reportNo,itemCount,"txt=�ָ�;");
// 		pos=QString::number(0)+","+QString::number(mouseEvent->scenePos().y());
// 		rect="0,0,5000,18";
// 		m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,rect,info) values (%d,%d,'%s',%d,'%s','%s')",itemCount,reportNo,pos.toStdString().data(),5,rect.toStdString().data(),"txt=�ָ�;fontsize=10;"));
// 		break;

	case InsertExp:
		itemCount++;
		drawGraphics(mouseEvent->scenePos(),QRectF(0,0,65,20),6,reportNo,itemCount,"txt=���ʽ;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;");
		pos=QString::number(mouseEvent->scenePos().x())+","+QString::number(mouseEvent->scenePos().y());
		rect="0,0,80,20";
		m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,rect,info) values (%d,%d,'%s',%d,'%s','%s')",itemCount,reportNo,pos.toStdString().data(),6,rect.toStdString().data(),"txt=���ʽ;fontsize=10;fontfamily=����;color=0,0,0;fonttype=0;txtop=0;delline=false;undline=false;isrect=0;rectpx=1;rectcor=0,0,0;"));
		break;

	case InsertLine:
		itemCount++;
		drawGraphics(mouseEvent->scenePos(),QRectF(0,0,50,50),7,reportNo,itemCount,"px=2;type=0;color=0,0,0;");
		pos=QString::number(mouseEvent->scenePos().x())+","+QString::number(mouseEvent->scenePos().y());
		rect="0,0,1,50";
		m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,rect,info) values (%d,%d,'%s',%d,'%s','%s')",itemCount,reportNo,pos.toStdString().data(),7,rect.toStdString().data(),"px=2;type=0;color=0,0,0;"));
		break;

	case InsertChart:
		itemCount++;
		drawGraphics(mouseEvent->scenePos(),QRectF(0,0,100,100),8,reportNo,itemCount,"txt=ͼ��;fontsize=10;");
		pos=QString::number(mouseEvent->scenePos().x())+","+QString::number(mouseEvent->scenePos().y());
		rect="0,0,100,100";
		m_pOper->ExecuteSQL(SString::toFormat("insert into t_ssp_report_item (item_no,report_no,pos,type,rect,info) values (%d,%d,'%s',%d,'%s','%s')",itemCount,reportNo,pos.toStdString().data(),8,rect.toStdString().data(),"txt=ͼ��;fontsize=10;"));
		break;
	default:
		;
	}
	QGraphicsScene::mousePressEvent(mouseEvent);
}
//��info������ԣ���ʽ"name=atu;"
QString getAtu(QString info,QString name)
{
	QList <QString> strList=info.split(";");
	for(int i=0;i<strList.size();i++)
	{
		QString tmp=strList.at(i);
		if(tmp.split("=").at(0)==name)
		{
			if(tmp.split("=").size()>1)
				return tmp.split("=").at(1);
			else
				return "";
		}
	}
	return "";
}
//�������ݿ����ݻ���
void MyGraphicsScene::initPaint(int itemNo,int type,QString pos,QString rect,QString info)
{
	if(itemNo>itemCount)
		itemCount=itemNo;
	QPointF point;
	point.setX(pos.split(",").at(0).toInt());
	point.setY(pos.split(",").at(1).toInt());
	QString txt;
	QRectF rect1(rect.split(",").at(0).toInt(),rect.split(",").at(1).toInt(),rect.split(",").at(2).toInt(),rect.split(",").at(3).toInt());
	SString div;
	divStruct div1;
	switch (type) 
	{
	case 1:
		drawGraphics(point,rect1,1,reportNo,itemNo,info);
		break;
	case 2:
		drawGraphics(point,rect1,2,reportNo,itemNo,info);
		break;
	case 3:
		drawGraphics(point,rect1,3,reportNo,itemNo,info);
		break;
	case 4:
		drawGraphics(point,rect1,4,reportNo,itemNo,info);
		break;
	case 5:
		{
			div=m_pOper->SelectInto(SString::toFormat("select seg from t_ssp_report_item where item_no=%d and report_no=%d",itemNo,reportNo));
			if(div!="100")
			{
				drawGraphics(point,rect1,5,reportNo,itemNo,info);
				div1.setDiv(div.data(),itemNo,point.y());
				divList->append(div1);
			}
		}
		break;
	case 6:
		drawGraphics(point,rect1,6,reportNo,itemNo,info);
		break;
	case 7:
		drawGraphics(point,rect1,7,reportNo,itemNo,info);
		break;
	case 8:
		drawGraphics(point,rect1,8,reportNo,itemNo,info);
		break;
	default:
		;
	}
}
void MyGraphicsScene::setMode(Mode mode)
{
	myMode=mode;
}
void MyGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void MyGraphicsScene::refresh()
{
	update();
}
//�ָ����ı�߶�ʱ��������Ԫ��ͬ���ı�
void MyGraphicsScene::divChanged(QPointF divR,int y)
{
	QList<QGraphicsItem *>items=this->items();
	int orgH=divR.y();
	for(int i=0;i<items.size();i++)
	{
		if(orgH<items.at(i)->pos().y())
		{
			((MyGraphicsItem*)items.at(i))->moveItem(0,y);

		}
	}
}
//����λ��
void MyGraphicsScene::saveAllPos()
{
	QList<QGraphicsItem *>items=this->items();
	for(int i=0;i<items.size();i++)
	{
		if(((MyGraphicsItem*)items.at(i))->getType()==5)
			((MyGraphicsItem*)items.at(i))->savePos();
	}
	for(int i=0;i<items.size();i++)
	{
		if(((MyGraphicsItem*)items.at(i))->getType()!=5)
			((MyGraphicsItem*)items.at(i))->savePos();
	}
}
//Ϊitem�������ݼ�
void MyGraphicsScene::saveAllDs(CSsp_Dataset *dbSet)
{
	QList<QGraphicsItem *>items=this->items();
	for(int i=0;i<items.size();i++)
	{
		((MyGraphicsItem*)items.at(i))->setDs(dbSet);
	}
}
void MyGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
//������Ӧ�¼�
void MyGraphicsScene::keyPressEvent(QKeyEvent *keyEvent)
{
	if(keyEvent->key()==Qt::Key_Up)
	{
		moveItems(0,-1);
		return;
	}
	if(keyEvent->key()==Qt::Key_Down)
	{
		moveItems(0,1);
		return;
	}
	if(keyEvent->key()==Qt::Key_Left)
	{
		moveItems(-1,0);
		return;
	}
	if(keyEvent->key()==Qt::Key_Right)
	{
		moveItems(1,0);
		return;
	}
	if(keyEvent->key()==Qt::Key_Delete)
	{
		if (QMessageBox::No==QMessageBox::warning(NULL,tr("����"),tr("�Ƿ�ɾ����ѡԪ�أ�"),QMessageBox::Yes,QMessageBox::No))
			return;
		delSelElm();
	}
	QGraphicsScene::keyPressEvent(keyEvent);
}

/////////////////////////////////////////////////////////////////////////////
MyGraphicsItem::MyGraphicsItem()
{
	setZValue(0);
	setAcceptDrops(true);
	setFlags(ItemIsSelectable);
	setAcceptsHoverEvents(true);
	cScale=false;
	m_cursor=new QCursor;
	direction=0;
	txt="txt";
	expDlg=NULL;
	txtDlg=NULL;
	lineDlg=NULL;
	chartDlg=NULL;
	m_pOper=DB;
	dbSet=NULL;
	
}
MyGraphicsItem::~MyGraphicsItem()
{
	delete m_cursor;
	delete expDlg;
	delete txtDlg;
	delete lineDlg;
	delete chartDlg;
	
}
void MyGraphicsItem::ResetRect(QRectF rect)
{
	m_rect=rect;
	update(boundingRect());
	//setPos(rect.x(),rect.y());

}
//��÷ָ����б��ı䷽λʱ�����ָ������ܸı�
void MyGraphicsItem::setDivList(QList<divStruct>*divList)
{
	this->divList=divList;
}
//����
void MyGraphicsItem::setType(int type)
{
	this->type=type;
}
//��ʾ���ı�
void MyGraphicsItem::setTxt(QString txt)
{
	this->txt=txt;
}
QRectF MyGraphicsItem::boundingRect()const
{
	return QRectF(m_rect.x()-2,m_rect.y()-2,m_rect.width()+4,m_rect.height()+4);
}
QRectF MyGraphicsItem::rect()
{
	return m_rect;
}
void MyGraphicsItem::setFont(QFont font)
{
	if(type==4)
		m_font=font;
	if(type==5)
		m_font=font;
	if(type==6)
		m_font=font;
}
void MyGraphicsItem::setColor(QColor color)
{
	m_color=color;
}
//��Ӧ����
QPainterPath MyGraphicsItem::shape()const
{
	QPainterPath path;
	if(type==1||type==7)
		path.addRect(m_rect);
	else if(type==2)//����
	{
		//path.moveTo(m_rect.x(),m_rect.y());
		//path.lineTo(m_rect.x()+m_rect.width(),m_rect.y());
		path.addRect(m_rect.x(),m_rect.y()-1,m_rect.width(),2);//��Ϊ���Σ�����ѡ��
	}
	else if(type==3)//����
	{
		path.addRect(m_rect.x()-1,m_rect.y(),2,m_rect.height());//��Ϊ���Σ�����ѡ��
		//path.moveTo(m_rect.x(),m_rect.y());
		//path.lineTo(m_rect.x(),m_rect.y()+m_rect.height());
	}
	else if(type==4)//�ı�
	{
		path.addText(m_rect.x(),m_rect.y()+m_font.pointSize(),m_font,txt);
		path.addRect(m_rect);
	}
	else if(type==5)//����ָ�
	{
		path.addText(m_rect.x(),m_rect.y()+m_font.pointSize(),m_font,txt);
		path.addRect(m_rect);
	}
	else if(type==6)//���ʽ
	{
		path.addText(m_rect.x(),m_rect.y()+m_font.pointSize(),m_font,txt);
		path.addRect(m_rect);
	}
	else if(type==8)//ͼ��
	{
		path.addText(m_rect.x(),m_rect.y()+m_font.pointSize(),m_font,txt);
		path.addRect(m_rect);
	}
	return path;
}
//����Ԫ��
void MyGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	QStyleOptionGraphicsItem op(*option ); 
	op.state=QStyle::State_HasFocus;
	QTextOption o;
	o.setAlignment(Qt::AlignCenter);
	if(txtOption==0)
	{
		o.setAlignment(Qt::AlignCenter);
	}
	else if(txtOption==1)
	{
		o.setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	}
	else if(txtOption==2)
	{
		o.setAlignment(Qt::AlignRight|Qt::AlignVCenter);
	}

	if(type==1)//����
		painter->drawRect(m_rect);

	else if(type==2)//����
	{
		QPen oldpen=painter->pen();
		QPen pen;
		pen.setColor(m_color);
		pen.setStyle(Qt::SolidLine);
		if(lineType==1)
			pen.setStyle(Qt::DashLine);
		else if(lineType==2)
			pen.setStyle(Qt::DotLine);
		else if(lineType==3)
			pen.setStyle(Qt::DashDotLine);
		pen.setWidth(linePx);
		painter->setPen(pen);
		painter->drawLine(m_rect.x(),m_rect.y(),m_rect.x()+m_rect.width(),m_rect.y());
		painter->setPen(oldpen);
	}
	else if(type==3)//����
	{
		QPen oldpen=painter->pen();
		QPen pen;
		pen.setColor(m_color);
		pen.setStyle(Qt::SolidLine);
		if(lineType==1)
			pen.setStyle(Qt::DashLine);
		else if(lineType==2)
			pen.setStyle(Qt::DotLine);
		else if(lineType==3)
			pen.setStyle(Qt::DashDotLine);
		pen.setWidth(linePx);
		painter->setPen(pen);
		painter->drawLine(m_rect.x(),m_rect.y(),m_rect.x(),m_rect.y()+m_rect.height());
		painter->setPen(oldpen);
	}
	else if(type==7)//б��
	{
		QPen oldpen=painter->pen();
		QPen pen;
		pen.setColor(m_color);
		pen.setStyle(Qt::SolidLine);
		if(lineType==1)
			pen.setStyle(Qt::DashLine);
		else if(lineType==2)
			pen.setStyle(Qt::DotLine);
		else if(lineType==3)
			pen.setStyle(Qt::DashDotLine);
		pen.setWidth(linePx);
		painter->setPen(pen);
		painter->drawLine(m_rect.x(),m_rect.y(),m_rect.x()+m_rect.width(),m_rect.y()+m_rect.height());
		painter->setPen(oldpen);
	}
	else if(type==4)//�ı�
	{
		painter->setFont(m_font);
		QPen oldpen=painter->pen();
		QPen pen;
		pen.setColor(m_color);
		painter->setPen(pen);
		painter->drawText(m_rect,txt,o);
		painter->setPen(oldpen);
		painter->drawRect(m_rect);
	}
	else if(type==5)//�ָ�
	{
		painter->setFont(m_font);
		painter->setBrush(Qt::yellow);
		painter->drawRect(m_rect);
		QPen oldpen=painter->pen();
		QPen pen;
		pen.setColor(m_color);
		painter->setPen(pen);
		//painter->drawText(m_rect.x(),m_rect.y()+m_font.pointSize(),txt,o);
		QTextOption o;
		o.setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
		painter->drawText(m_rect,txt,o);
		painter->setPen(oldpen);
	}
	else if(type==6)//���ʽ
	{
		painter->setFont(m_font);
		painter->drawRect(m_rect);
		QPen oldpen=painter->pen();
		QPen pen;
		pen.setColor(m_color);
		painter->setPen(pen);
		painter->drawText(m_rect,txt,o);
		painter->setPen(oldpen);
	}
	else if(type==8)//ͼ��
	{
		QFont font;
		font.setPointSize(18);
		painter->setFont(font);
		painter->drawText(m_rect,txt,o);
		painter->drawRect(m_rect);
	}
	if (option->state & QStyle::State_Selected)//ѡ�������������߿�
	{
		qt_graphicsItem_highlightSelected(this, painter, option);
		//setFlags(flags()|QGraphicsItem::ItemIsFocusable);
	}
	//emit refresh();
}
//��꽹�����item����״�ı�
void MyGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	QPointF pos1=event->scenePos();
	QPointF lt=this->scenePos()+QPointF(m_rect.x(),m_rect.y());
	QPointF lb=this->scenePos()+QPointF(m_rect.x(),m_rect.y()+m_rect.height());
	QPointF rt=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y());
	QPointF rb=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y()+m_rect.height());
	if((pos1.x()<=lt.x()+2&&pos1.y()<=lt.y()+2)||((pos1.x()>=rb.x()-2&&pos1.y()>=rb.y()-2)&&type!=2&&type!=3))
	{
		m_cursor->setShape(Qt::SizeFDiagCursor);
	}
	else if((pos1.x()<=lb.x()+2&&pos1.y()>=lb.y()-2)||((pos1.x()>=rt.x()-2&&pos1.y()<=rt.y()+2)&&type!=2&&type!=3))
	{
		m_cursor->setShape(Qt::SizeBDiagCursor);
	}
	else if((pos1.x()<=lt.x()+2||pos1.x()>=rt.x()-2)&&(pos1.y()<=lb.y()&&pos1.y()>=lt.y())&&type!=3)
	{
		m_cursor->setShape(Qt::SizeHorCursor);
	}
	else if((pos1.y()<=lt.y()+2||pos1.y()>=lb.y()-2)&&(pos1.x()>=lt.x()&&pos1.x()<=rt.x())&&type!=2)
	{
		m_cursor->setShape(Qt::SizeVerCursor);
	}
	else
	{
		m_cursor->setShape(Qt::OpenHandCursor);
	}
	this->setCursor(*m_cursor);
	update();
	QGraphicsItem::hoverEnterEvent(event);
}
//��꽹���ƶ�����״�ı�
void MyGraphicsItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	QPointF pos1=event->scenePos();
	QPointF lt=this->scenePos()+QPointF(m_rect.x(),m_rect.y());
	QPointF lb=this->scenePos()+QPointF(m_rect.x(),m_rect.y()+m_rect.height());
	QPointF rt=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y());
	QPointF rb=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y()+m_rect.height());
	if(((pos1.x()<=lt.x()+2&&pos1.y()<=lt.y()+2)||(pos1.x()>=rb.x()-2&&pos1.y()>=rb.y()-2))&&type!=2&&type!=3)//����������
	{
		m_cursor->setShape(Qt::SizeFDiagCursor);
	}
	else if(((pos1.x()<=lb.x()+2&&pos1.y()>=lb.y()-2)||(pos1.x()>=rt.x()-2&&pos1.y()<=rt.y()+2))&&type!=2&&type!=3)
	{
		m_cursor->setShape(Qt::SizeBDiagCursor);
	}
	else if((pos1.x()<=lt.x()+2||pos1.x()>=rt.x()-2)&&(pos1.y()<=lb.y()&&pos1.y()>=lt.y())&&type!=3)
	{
		m_cursor->setShape(Qt::SizeHorCursor);
	}
	else if((pos1.y()<=lt.y()+2||pos1.y()>=lb.y()-2)&&(pos1.x()>=lt.x()&&pos1.x()<=rt.x())&&type!=2)
	{
		m_cursor->setShape(Qt::SizeVerCursor);
	}
	else
	{
		m_cursor->setShape(Qt::ClosedHandCursor);
	}
	this->setCursor(*m_cursor);
	update();
	QGraphicsItem::hoverMoveEvent(event);
}

void MyGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	m_cursor->setShape(Qt::ArrowCursor);
	this->setCursor(*m_cursor);
	update();
	QGraphicsItem::hoverLeaveEvent(event);
}

void MyGraphicsItem::keyPressEvent(QKeyEvent *event)
{
	QGraphicsItem::keyPressEvent(event);
}
void MyGraphicsItem::keyReleaseEvent(QKeyEvent *event)
{
	QGraphicsItem::keyReleaseEvent(event);
}
//812
//7 3
//654
//�����item���ж��ĸ���������
void MyGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->button()==Qt::LeftButton){
 		start=event->scenePos();
		cScale=true;
		cMove=false;
		setFlag(QGraphicsItem::ItemIsMovable,false);
		QPointF pos1=event->scenePos();
		QPointF lt=this->scenePos()+QPointF(m_rect.x(),m_rect.y());
		QPointF lb=this->scenePos()+QPointF(m_rect.x(),m_rect.y()+m_rect.height());
		QPointF rt=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y());
		QPointF rb=this->scenePos()+QPointF(m_rect.x()+m_rect.width(),m_rect.y()+m_rect.height());
		if(((pos1.x()<=lt.x()+2&&pos1.y()<=lt.y()+2)||(pos1.x()>=rb.x()-2&&pos1.y()>=rb.y()-2))&&type!=2&&type!=3)
		{
			if(pos1.x()<=lt.x()+2)
				direction=8;
			else 
				direction=4;
		}else if(((pos1.x()<=lb.x()+2&&pos1.y()>=lb.y()-2)||(pos1.x()>=rt.x()-2&&pos1.y()<=rt.y()+2))&&type!=2&&type!=3)
		{
			if(pos1.x()<=lb.x()+2)
				direction=6;
			else 
				direction=2;
		}
		else if((pos1.x()<=lt.x()+2||pos1.x()>=rt.x()-2)&&(pos1.y()<=lb.y()&&pos1.y()>=lt.y())&&type!=3)
		{
			if(pos1.x()<=lt.x()+2)
				direction=7;
			else 
				direction=3;
		}
		else if((pos1.y()<=lt.y()+2||pos1.y()>=lb.y()-2)&&(pos1.x()>=lt.x()&&pos1.x()<=rt.x())&&type!=2)
		{
			if(pos1.y()<=lt.y()+2)
				direction=1;
			else 
				direction=5;
		}
		else
		{
			cScale=false;
			cMove=true;
			m_cursor->setShape(Qt::ArrowCursor);
		}
	}
	if(type==2)//���ߴ�ֱ��������
	{
		if(direction==1||direction==5)
		{
			cScale=false;
			cMove=true;
		}
	}
	if(type==3)//����ˮƽ��������
	{
		if(direction==3||direction==7)
		{
			cScale=false;
			cMove=true;
		}
	}
	if(type==5)//�ָ���ʹ���Զ���move
	{
		cScale=false;
		cMove=false;
		setFlag(QGraphicsItem::ItemIsMovable,false);
	}
	update();
	QGraphicsItem::mousePressEvent(event);
}
//���˫��item�������ÿ�
void MyGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)//˫��
{
	S_UNUSED(event);
	if(type==2||type==3||type==7)//3����
	{
		if(lineDlg==NULL)
			lineDlg=new linesetting_dialog(0);
		QString info=m_pOper->SelectInto(SString::toFormat("select info from t_ssp_report_item where item_no=%d and report_no=%d",itemNo,reportNo)).data();
		//��ȡ�������ý��Ի���
		QString px=getAtu(info,"px");//��ϸ
		lineDlg->setpx(px.toInt());

		QString colorStr=getAtu(info,"color");//��ɫ
		QStringList lstColorStr=colorStr.split(",");
		if(lstColorStr.size()>=3)
		{
			QColor color(lstColorStr.at(0).toInt(),lstColorStr.at(1).toInt(),lstColorStr.at(2).toInt());
			lineDlg->setColor(color);
		}

		QString type=getAtu(info,"type");//���ͣ�ʵ�����ߣ�
		lineDlg->setType(type.toInt());

		lineDlg->exec();//�����ԶԻ���

		//�ӶԻ����ȡ����
		QColor color=lineDlg->getColor();
		colorStr=QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue());
		QString newInfo=setValue(atu,"color",colorStr);
		m_color=color;
		atu=newInfo;

		type=QString::number(lineDlg->getType());
		newInfo=setValue(atu,"type",type);
		lineType=type.toInt();
		atu=newInfo;

		px=QString::number(lineDlg->getpx());
		newInfo=setValue(atu,"px",px);
		linePx=px.toInt();
		atu=newInfo;

		m_pOper->Execute(SString::toFormat("update t_ssp_report_item set info='%s' where item_no=%d and report_no=%d",newInfo.toStdString().data(),itemNo,reportNo));

		emit refresh();
	}
	else if(type==4)//�ı�
	{
		if(txtDlg==NULL)
			txtDlg=new txtsetting_dialog(0);
		QString info=m_pOper->SelectInto(SString::toFormat("select info from t_ssp_report_item where item_no=%d and report_no=%d",itemNo,reportNo)).data();
		//��ȡ�������ý��Ի���
		QString fontSize=getAtu(info,"fontsize");//�����С
		txtDlg->setFontSize(fontSize);

		QString fontFamily=getAtu(info,"fontfamily");//����
		txtDlg->setFontFamily(fontFamily);

		QString colorStr=getAtu(info,"color");//��ɫ
		QStringList lstColorStr=colorStr.split(",");
		if(lstColorStr.size()>=3)
		{
			QColor color(lstColorStr.at(0).toInt(),lstColorStr.at(1).toInt(),lstColorStr.at(2).toInt());
			txtDlg->setColor(color);
		}

		QString fontType=getAtu(info,"fonttype");//�������ͣ���б�Ӵ֣�
		txtDlg->setFontType(fontType.toInt());

		QString txtop=getAtu(info,"txtop");//������
		txtDlg->setTxtOp(txtop.toInt());

		QString delLine=getAtu(info,"delline");//ɾ����
		if(delLine=="true")
			txtDlg->setFontDelLine(true);
		else
			txtDlg->setFontDelLine(false);

		QString undLine=getAtu(info,"undline");//�»���
		if(undLine=="true")
			txtDlg->setFontUndLine(true);
		else
			txtDlg->setFontUndLine(false);

		QString isRect=getAtu(info,"isrect");//���
		txtDlg->setIsRect(isRect.toInt());

		QString rectPx=getAtu(info,"rectpx");//����ϸ
		txtDlg->setRectpx(rectPx.toInt());

		QString rectColorStr=getAtu(info,"rectcor");//�����ɫ
		QStringList lstRectColorStr=rectColorStr.split(",");
		if(lstRectColorStr.size()>=3)
		{
			QColor rectColor(lstRectColorStr.at(0).toInt(),lstRectColorStr.at(1).toInt(),lstRectColorStr.at(2).toInt());
			txtDlg->setRectColor(rectColor);
		}

		txtDlg->setTxt(txt);
		//txtDlg->initDlg();
		txtDlg->exec();//�����ԶԻ���
		//�ӶԻ����ȡ����
		if(txtDlg->getTxt()!="")
		{
			txt=txtDlg->getTxt();
			QString newInfo=setValue(atu,"txt",txt);
			m_pOper->Execute(SString::toFormat("update t_ssp_report_item set info='%s' where item_no=%d and report_no=%d",newInfo.toStdString().data(),itemNo,reportNo));
			atu=newInfo;
		}
		fontSize=txtDlg->getFontSize();
		QString newInfo=setValue(atu,"fontsize",fontSize);
		atu=newInfo;

		fontFamily=txtDlg->getFontFamily();
		newInfo=setValue(atu,"fontfamily",fontFamily);
		atu=newInfo;


		QColor color=txtDlg->getColor();
		colorStr=QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue());
		newInfo=setValue(atu,"color",colorStr);
		m_color=color;
		atu=newInfo;

		fontType=QString::number(txtDlg->getFontType());
		newInfo=setValue(atu,"fonttype",fontType);
		atu=newInfo;

		txtop=QString::number(txtDlg->getTxtOp());
		newInfo=setValue(atu,"txtop",txtop);
		txtOption=txtop.toInt();
		atu=newInfo;

		bool delline=txtDlg->getFontDelLine();
		if(delline)
			newInfo=setValue(atu,"delline","true");
		else
			newInfo=setValue(atu,"delline","false");
		atu=newInfo;

		bool undline=txtDlg->getFontUndLine();
		if(undline)
			newInfo=setValue(atu,"undline","true");
		else
			newInfo=setValue(atu,"undline","false");
		atu=newInfo;

		isRect=QString::number(txtDlg->getIsRect());
		newInfo=setValue(atu,"isrect",isRect);
		isrect=isRect.toInt();
		atu=newInfo;

		rectPx=QString::number(txtDlg->getRectpx());
		newInfo=setValue(atu,"rectpx",rectPx);
		rectpx=rectPx.toInt();
		atu=newInfo;

		QColor rectColor=txtDlg->getRectColor();
		rectColorStr=QString::number(rectColor.red())+","+QString::number(rectColor.green())+","+QString::number(rectColor.blue());
		newInfo=setValue(atu,"rectcor",rectColorStr);
		rect_color=rectColor;
		atu=newInfo;

		m_pOper->Execute(SString::toFormat("update t_ssp_report_item set info='%s' where item_no=%d and report_no=%d",newInfo.toStdString().data(),itemNo,reportNo));

		QFont curfont=m_font;
		curfont.setPointSize(fontSize.toInt());
		curfont.setFamily(fontFamily);
		if(fontType=="0")
		{
			curfont.setBold(false);
			curfont.setItalic(false);
		}
		else if(fontType=="1")
		{
			curfont.setBold(true);
			curfont.setItalic(false);
		}
		else if(fontType=="2")
		{
			curfont.setBold(false);
			curfont.setItalic(true);
		}
		else if(fontType=="3")
		{
			curfont.setBold(true);
			curfont.setItalic(true);
		}
		if(delline)
			curfont.setStrikeOut(true);
		else
			curfont.setStrikeOut(false);
		if(undline)
			curfont.setUnderline(true);
		else
			curfont.setUnderline(false);
		m_font=curfont;
		setFont(curfont);

 		emit refresh();
	}
	else if(type==5)//�ָ���
	{
// 		bool ok;
// 		QString name=QInputDialog::getText(0,tr("�޸��ı�"),tr("���������ı�:"),QLineEdit::Normal,"",&ok,0);  
// 		if(!name.isEmpty()) 
// 			txt=name; 
// 		QString newInfo=setValue(atu,"txt",txt);
// 		m_pOper->Execute(SString::toFormat("update t_ssp_report_item set info='%s' where item_no=%d and report_no=%d",newInfo.toStdString().data(),itemNo,reportNo));
// 		atu=newInfo;
	}
	else if(type==6)//���ʽ
	{
		if(dbSet==NULL)
			return;
		if(expDlg==NULL)
			expDlg=new settingExpress_dlg(txt,exp,0);
		expDlg->setDs(dbSet);
		//��ȡ�������ý�Ի���
		QString info=m_pOper->SelectInto(SString::toFormat("select info from t_ssp_report_item where item_no=%d and report_no=%d",itemNo,reportNo)).data();
		QString fontSize=getAtu(info,"fontsize");//�����С
		expDlg->setFontSize(fontSize);

		QString fontFamily=getAtu(info,"fontfamily");//����
		expDlg->setFontFamily(fontFamily);

		QString colorStr=getAtu(info,"color");//������ɫ
		QStringList lstColorStr=colorStr.split(",");
		if(lstColorStr.size()>=3)
		{
			QColor color(lstColorStr.at(0).toInt(),lstColorStr.at(1).toInt(),lstColorStr.at(2).toInt());
			expDlg->setColor(color);
		}

		QString fontType=getAtu(info,"fonttype");//�������ͣ��Ӵ�б�壩
		expDlg->setFontType(fontType.toInt());
		
		QString txtop=getAtu(info,"txtop");//������
		expDlg->setTxtOp(txtop.toInt());

		QString delLine=getAtu(info,"delline");//ɾ����
		if(delLine=="true")
			expDlg->setFontDelLine(true);
		else
			expDlg->setFontDelLine(false);

		QString undLine=getAtu(info,"undline");//�»���
		if(undLine=="true")
			expDlg->setFontUndLine(true);
		else
			expDlg->setFontUndLine(false);

		QString isRect=getAtu(info,"isrect");//���
		expDlg->setIsRect(isRect.toInt());

		QString rectPx=getAtu(info,"rectpx");//����ϸ
		expDlg->setRectpx(rectPx.toInt());

		QString rectColorStr=getAtu(info,"rectcor");//�����ɫ��û��color���ܱ���
		QStringList lstRectColorStr=rectColorStr.split(",");
		if(lstRectColorStr.size()>=3)
		{
			QColor rectColor(lstRectColorStr.at(0).toInt(),lstRectColorStr.at(1).toInt(),lstRectColorStr.at(2).toInt());
			expDlg->setRectColor(rectColor);
		}

		QString exp=getAtu(info,"exp");//���ݼ�
		expDlg->setExp(exp);

		QString fun=getAtu(info,"fun");//����
		expDlg->setFun(fun);

		expDlg->setTxt(txt);

		expDlg->initDlg();
		expDlg->exec();//�����ԶԻ���
		//�ӶԻ����ȡ����
		if(expDlg->getTxt()!="")
		{
			txt=expDlg->getTxt();
			QString newInfo=setValue(atu,"txt",txt);
			m_pOper->Execute(SString::toFormat("update t_ssp_report_item set info='%s' where item_no=%d and report_no=%d",newInfo.toStdString().data(),itemNo,reportNo));
			atu=newInfo;
		}
		if(expDlg->getExp()!="")
		{
			exp=expDlg->getExp();
			QString newInfo=setValue(atu,"exp",exp);
			m_pOper->Execute(SString::toFormat("update t_ssp_report_item set info='%s' where item_no=%d and report_no=%d",newInfo.toStdString().data(),itemNo,reportNo));
			atu=newInfo;
		}
		fun=expDlg->getFun();
		QString newInfo=setValue(atu,"fun",fun);
		atu=newInfo;

		fontSize=expDlg->getFontSize();
		newInfo=setValue(atu,"fontsize",fontSize);
		atu=newInfo;

		fontFamily=expDlg->getFontFamily();
		newInfo=setValue(atu,"fontfamily",fontFamily);
		atu=newInfo;
		
		QColor color=expDlg->getColor();
		colorStr=QString::number(color.red())+","+QString::number(color.green())+","+QString::number(color.blue());
		newInfo=setValue(atu,"color",colorStr);
		m_color=color;
		atu=newInfo;

		fontType=QString::number(expDlg->getFontType());
		newInfo=setValue(atu,"fonttype",fontType);
		atu=newInfo;

		txtop=QString::number(expDlg->getTxtOp());
		newInfo=setValue(atu,"txtop",txtop);
		txtOption=txtop.toInt();
		atu=newInfo;

		bool delline=expDlg->getFontDelLine();
		if(delline)
			newInfo=setValue(atu,"delline","true");
		else
			newInfo=setValue(atu,"delline","false");
		atu=newInfo;

		bool undline=expDlg->getFontUndLine();
		if(undline)
			newInfo=setValue(atu,"undline","true");
		else
			newInfo=setValue(atu,"undline","false");
		atu=newInfo;

		isRect=QString::number(expDlg->getIsRect());
		newInfo=setValue(atu,"isrect",isRect);
		isrect=isRect.toInt();
		atu=newInfo;

		rectPx=QString::number(expDlg->getRectpx());
		newInfo=setValue(atu,"rectpx",rectPx);
		rectpx=rectPx.toInt();
		atu=newInfo;

		QColor rectColor=expDlg->getRectColor();
		rectColorStr=QString::number(rectColor.red())+","+QString::number(rectColor.green())+","+QString::number(rectColor.blue());
		newInfo=setValue(atu,"rectcor",rectColorStr);
		rect_color=rectColor;
		atu=newInfo;

		m_pOper->Execute(SString::toFormat("update t_ssp_report_item set info='%s' where item_no=%d and report_no=%d",newInfo.toStdString().data(),itemNo,reportNo));

		QFont curfont=m_font;
		curfont.setPointSize(fontSize.toInt());
		curfont.setFamily(fontFamily);
		if(fontType=="0")
		{
			curfont.setBold(false);
			curfont.setItalic(false);
		}
		else if(fontType=="1")
		{
			curfont.setBold(true);
			curfont.setItalic(false);
		}
		else if(fontType=="2")
		{
			curfont.setBold(false);
			curfont.setItalic(true);
		}
		else if(fontType=="3")
		{
			curfont.setBold(true);
			curfont.setItalic(true);
		}
		if(delline)
			curfont.setStrikeOut(true);
		else
			curfont.setStrikeOut(false);
		if(undline)
			curfont.setUnderline(true);
		else
			curfont.setUnderline(false);
		m_font=curfont;
		setFont(curfont);
		emit refresh();
	}
	else if(type==8)//ͼ��
	{
		if(chartDlg==NULL)
			chartDlg=new chartsetting_dialog(0);
		QString info=m_pOper->SelectInto(SString::toFormat("select info from t_ssp_report_item where item_no=%d and report_no=%d",itemNo,reportNo)).data();
		QString txt=getAtu(info,"txt");//��ʾ�ı���ͼ����⣩
		chartDlg->setTxt(txt);

		QString chartDs=getAtu(info,"chartds");//���ݼ���
		chartDlg->setDs(chartDs.toInt());

		QString extAtu=getAtu(info,"extatu");//��չ����
		chartDlg->setExtAtu(extAtu);

		QString chartType=getAtu(info,"charttype");//ͼ������
		chartDlg->setType(chartType.toInt());

		//chartDlg->initDlg();
		chartDlg->exec();//�򿪶Ի���

		txt=chartDlg->getTxt();
		QString newInfo=setValue(atu,"txt",txt);
		atu=newInfo;

		chartDs=QString::number(chartDlg->getDs());
		newInfo=setValue(atu,"chartds",chartDs);
		atu=newInfo;

		extAtu=chartDlg->getExtAtu();
		newInfo=setValue(atu,"extatu",extAtu);
		atu=newInfo;

		chartType=QString::number(chartDlg->getType());
		newInfo=setValue(atu,"charttype",chartType);
		atu=newInfo;

		m_pOper->Execute(SString::toFormat("update t_ssp_report_item set info='%s' where item_no=%d and report_no=%d",newInfo.toStdString().data(),itemNo,reportNo));

		emit refresh();
	}
}
void MyGraphicsItem::setDs(CSsp_Dataset* dbSet)
{
	this->dbSet=dbSet;
}
//��������ƶ�
void MyGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(cScale)
	{
		QPointF dis;
		end=event->scenePos();
		dis=end-start;
		start=end;
		switch(direction)
		{
		case 1://��
			{
				QRectF tem=QRectF(m_rect.x(),m_rect.y()+dis.y(),m_rect.width(),m_rect.height()-dis.y());
				if(tem.height()>0)
					this->ResetRect(tem);
				break;
			}
		case 2://����
			{
				QRectF tem=QRectF(m_rect.x(),m_rect.y()+dis.y(),
					m_rect.width()+dis.x(),m_rect.height()-dis.y());
				if(tem.height()>0&&tem.width()>0)
					this->ResetRect(tem);
				break;
			}
		case 3://��
			{
				QRectF tem=QRectF(m_rect.x(),m_rect.y(),m_rect.width()+dis.x(),m_rect.height());
				if(tem.width()>0)
					this->ResetRect(tem);
				break;
			}
		case 4://����
			{
				QRectF tem=QRectF(m_rect.x(),m_rect.y(),m_rect.width()+dis.x(),m_rect.height()+dis.y());
				if(tem.height()>0&&tem.width()>0)
					this->ResetRect(tem);
				break;
			}
		case 5://��
			{
				QRectF tem=QRectF(m_rect.x(),m_rect.y(),m_rect.width(),m_rect.height()+dis.y());
				if(tem.height()>0)
					this->ResetRect(tem);
				break;
			}
		case 6://����
			{
				QRectF tem=QRectF(m_rect.x()+dis.x(),m_rect.y(),m_rect.width()-dis.x(),m_rect.height()+dis.y());
				if(tem.height()>0&&tem.width()>0)
					this->ResetRect(tem);
				break;
			}
		case 7://��
			{
				QRectF tem=QRectF(m_rect.x()+dis.x(),m_rect.y(),m_rect.width()-dis.x(),m_rect.height());
				if(tem.width()>0)
					this->ResetRect(tem);
				break;
			}
		case 8://����
			{
				QRectF tem=QRectF(m_rect.x()+dis.x(),m_rect.y()+dis.y(),m_rect.width()-dis.x(),m_rect.height()-dis.y());
				if(tem.height()>0&&tem.width()>0)
					this->ResetRect(tem);
				break;
			}
		}
	}
	else if(cMove==true)//�����ƶ�
	{
		QPointF dis;
		end=event->scenePos();
 		dis=end-start;
//		start=end;
// 		setPos(pos().x()+dis.x(),pos().y()+dis.y());
		emit moveItems(dis.x(),dis.y());
	}
	else if(type==5)//�ָ���ʱ�·�Ԫ��һ��ı�λ��
	{
		QPointF dis;
		end=event->scenePos();
		dis=end-start;
		start=end;
		setPos(pos().x(),pos().y()+dis.y());
		//QRectF tem=QRectF(m_rect.x(),m_rect.y()+dis.y(),
		//	m_rect.width(),m_rect.height());
		//this->ResetRect(tem);
		emit divChanged(pos(),dis.y());
	}
	QGraphicsItem::mouseMoveEvent(event);
}
void MyGraphicsItem::moveItem(int rx,int ry)
{
	QPointF tmpS=start;
	start.setX(tmpS.x()+rx);
	start.setY(tmpS.y()+ry);
	setPos(pos().x()+rx,pos().y()+ry);
}
void MyGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	cScale=false;
	cMove=false;
	update();
// 	if(type==5)
// 	{
// 		for(int i=0;i<divList->size();i++)
// 		{
// 			if(divList->at(i).itemNo==itemNo)
// 			{
// 				(*divList)[i].height=pos().y();
// 			}
// 		}
// 	}
// 	else
// 	{
// 		for(int i=0;i<divList->size();i++)
// 		{
// 			if(divList->at(i).height>=pos().y())
// 			{
// 				m_pOper->Execute(SString::toFormat("update t_ssp_report_item set seg='%s' where item_no=%d and report_no=%d",divList->at(i).no.data(),itemNo,reportNo));
// 				break;
// 			}
// 		}
// 	}
	emit saveAllPos();//����λ��
	QGraphicsItem::mouseReleaseEvent(event);
}
//����λ�ã����ݷָ���λ�øı�ָ�������
void MyGraphicsItem::savePos()
{
	QRectF rect=m_rect;
	QString pos1=QString::number(pos().x())+","+QString::number(pos().y());
	QString rectStr=QString::number(rect.x())+","+QString::number(rect.y())+","+QString::number(rect.width())+","+QString::number(rect.height());
	if(type==5)
	{
		for(int i=0;i<divList->size();i++)
		{
			if(divList->at(i).itemNo==itemNo)
			{
				(*divList)[i].height=pos().y();
			}
		}
	}
	else
	{
		for(int i=0;i<divList->size();i++)
		{
			if(divList->at(i).height>=pos().y())
			{
				m_pOper->Execute(SString::toFormat("update t_ssp_report_item set seg='%s' where item_no=%d and report_no=%d",divList->at(i).no.data(),itemNo,reportNo));
				break;
			}
		}
	}
	m_pOper->Execute(SString::toFormat("update t_ssp_report_item set pos='%s',rect='%s' where item_no=%d and report_no=%d",pos1.toStdString().data(),rectStr.toStdString().data(),itemNo,reportNo));
}
//��info�������
QString setValue(QString info,QString name,QString value)
{
	QList <QString> strList=info.split(";");
	for(int i=0;i<strList.size();i++)
	{
		QString tmp=strList.at(i);
		if(tmp=="")
			continue;
		if(tmp.split("=").at(0)==name)
		{
			if(tmp.split("=").size()>1)
			{
				QString newInfo=name+"="+value+";";
				for(int j=0;j<strList.size();j++)
				{
					if(i!=j&&strList.at(j)!="")
					{
						newInfo+=strList.at(j)+";";
					}
				}
				return newInfo;
			}
			else
				return info;
		}
	}
	return name+"="+value+";"+info;//Ϊ��
}
/////////////////////////////////////////////////////////////////////////////////////////
settingSQL_dlg::settingSQL_dlg(QWidget *parent)
	: QDialog(parent)
{
	//��������Դ����
	setWindowTitle(tr("���ݴ���ѡ��"));
	dsLabel=new QLabel(this);
	dsLabel->setText(tr("ѡ�����ݴ���"));
	curDsLabel=new QLabel(this);
	tableWidget=new QTableWidget(this);

	Ok=new QPushButton(this);
	Ok->setText(tr("ȷ��(&O)"));
	Ok->setMaximumHeight(24);
	Ok->setDefault(true);
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/image/ok"), QSize(), QIcon::Normal, QIcon::Off);
	Ok->setIcon(icon);
	cancel=new QPushButton(this);
	cancel->setText(tr("ȡ��(&C)"));
	cancel->setMaximumHeight(24);
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/image/cancel"), QSize(), QIcon::Normal, QIcon::Off);
	cancel->setIcon(icon1);

	m_pOper=DB;
	QVBoxLayout *labelsqllayout=new QVBoxLayout();
	QVBoxLayout *sqllayout=new QVBoxLayout();
	QHBoxLayout *btnlayout=new QHBoxLayout();

	labelsqllayout->addWidget(dsLabel);
	labelsqllayout->addStretch();
	labelsqllayout->addWidget(curDsLabel);
	labelsqllayout->setMargin(2);

	sqllayout->addLayout(labelsqllayout);
	sqllayout->addWidget(tableWidget);
	sqllayout->setMargin(2);

	btnlayout->addStretch();
	btnlayout->addWidget(Ok);
	btnlayout->addWidget(cancel);
	btnlayout->setMargin(2);

	QVBoxLayout *layout=new QVBoxLayout();
	layout->addLayout(sqllayout);
	layout->addLayout(btnlayout);
	layout->setMargin(2);

	setLayout(layout);
	initDlg();
}
settingSQL_dlg::~settingSQL_dlg()
{

}
void settingSQL_dlg::initDlg()//���ñ������
{
	tableWidget->clear();
	QStringList header;
	header<<tr("���")<<tr("����")<<tr("����");
	tableWidget->setColumnCount(3);
	tableWidget->setRowCount(0);
	tableWidget->setHorizontalHeaderLabels(header);
	tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	tableWidget->setColumnWidth(0,50);
	tableWidget->setColumnWidth(1,100);
	tableWidget->setColumnWidth(2,100);
	tableWidget->setMinimumWidth(350);
	tableWidget->verticalHeader()->setDefaultSectionSize(17);
	tableWidget->horizontalHeader()->setStretchLastSection(true);
	tableWidget->horizontalHeader()->setAutoScroll(true);
	tableWidget->verticalHeader()->setAutoScroll(true);
	SRecordset dsSet;
	m_pOper->RetrieveRecordset("select dw_sn,dstype,name,dw_desc from t_ssp_data_window order by cls_name desc,dw_desc",dsSet);

	for(int i=0;i<dsSet.GetRows();i++)
	{
		if(dsSet.GetValue(i,1).toInt()!=1)
		{
			int rowCount=tableWidget->rowCount();
			tableWidget->insertRow(rowCount);

			SString tmp=dsSet.GetValue(i,0).data();
			QTableWidgetItem *item=new QTableWidgetItem();
			item->setText(tmp.data());
			item->setFlags(item->flags()&~Qt::ItemIsEditable);
			tableWidget->setItem(rowCount,0,item);

			tmp=dsSet.GetValue(i,2).data();
			item=new QTableWidgetItem();
			item->setText(tmp.data());
			item->setFlags(item->flags()&~Qt::ItemIsEditable);
			tableWidget->setItem(rowCount,1,item);

			tmp=dsSet.GetValue(i,3).data();
			item=new QTableWidgetItem();
			item->setText(tmp.data());
			item->setFlags(item->flags()&~Qt::ItemIsEditable);
			tableWidget->setItem(rowCount,2,item);
		}
	}
	connect(Ok,SIGNAL(clicked()),this,SLOT(onOk()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(onCancel()));
}
void settingSQL_dlg::onOk()
{
	if(tableWidget->selectedItems().size()>0)
	{
		int row=tableWidget->selectedItems().at(0)->row();
		dsNo=tableWidget->item(row,0)->text().toInt();
	}
	hide();
}
void settingSQL_dlg::onCancel()
{
	hide();
}
int settingSQL_dlg::getNo()
{
	return dsNo;
}
void settingSQL_dlg::setDsNo(int ds)
{
	dsNo=ds;
	curDsLabel->setText(QString("��ǰ���:%1").arg(dsNo));
}

/////////////////////////////////////////////////////////////////////////////////////////
settingExpress_dlg::settingExpress_dlg(QString txt,QString exp,QWidget *parent)
	: QDialog(parent)
{
	S_UNUSED(exp);
	//���ʽԪ����������
	setWindowTitle(tr("���ʽ����"));
	funLabel=new QLabel(this);
	funLabel->setText("��������");
	curFunLabel=new QLabel(this);
	funList=new QTableWidget(this);

	colLabel=new QLabel(this);
	colLabel->setText("������");
	curColLabel=new QLabel(this);
	colList=new QTableWidget(this);

	tabWidget=new QTabWidget(this);
	tab1=new QWidget(this);
	tab2=new txtsetting_widget(this);

	Ok=new QPushButton(this);
	Ok->setText(tr("ȷ��(&O)"));
	Ok->setMaximumHeight(24);
	Ok->setDefault(true);
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/image/ok"), QSize(), QIcon::Normal, QIcon::Off);
	Ok->setIcon(icon);
	cancel=new QPushButton(this);
	cancel->setText(tr("ȡ��(&C)"));
	cancel->setMaximumHeight(24);
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/image/cancel"), QSize(), QIcon::Normal, QIcon::Off);
	cancel->setIcon(icon1);

	QHBoxLayout *labelfunlayout=new QHBoxLayout();
	QVBoxLayout *funlayout=new QVBoxLayout();
	QHBoxLayout *labelcollayout=new QHBoxLayout();
	QVBoxLayout *collayout=new QVBoxLayout();
	QHBoxLayout *bottomlayout=new QHBoxLayout();
	QVBoxLayout *tab1layout=new QVBoxLayout();
	QHBoxLayout *btnlayout=new QHBoxLayout();

	labelfunlayout->addWidget(funLabel);
	labelfunlayout->addStretch();
	labelfunlayout->addWidget(curFunLabel);
	labelfunlayout->setMargin(2);

	funlayout->addLayout(labelfunlayout);
	funlayout->addWidget(funList);
	funlayout->setMargin(2);

	labelcollayout->addWidget(colLabel);
	labelcollayout->addStretch();
	labelcollayout->addWidget(curColLabel);
	labelcollayout->setMargin(2);

	collayout->addLayout(labelcollayout);
	collayout->addWidget(colList);
	collayout->setMargin(2);

	bottomlayout->addLayout(funlayout);
	bottomlayout->addLayout(collayout);
	bottomlayout->setMargin(2);

	tab1layout->addLayout(bottomlayout);
	tab1layout->setMargin(2);
	tab1->setLayout(tab1layout);
	tabWidget->addTab(tab1,"���ʽ����");
	tabWidget->addTab(tab2,"�ı�����");

	btnlayout->addStretch();
	btnlayout->addWidget(Ok);
	btnlayout->addWidget(cancel);
	btnlayout->setMargin(2);

	m_pOper=DB;
	this->txt=txt;
	this->col=col;

	QVBoxLayout *layout=new QVBoxLayout();
	layout->addWidget(tabWidget);
	layout->addLayout(btnlayout);
	layout->setMargin(2);
	setLayout(layout);

	//initDlg();
	connect(Ok,SIGNAL(clicked()),this,SLOT(onOk()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(onCancel()));
}
settingExpress_dlg::~settingExpress_dlg()
{

}
void settingExpress_dlg::initDlg()
{
	colList->clear();
	funList->clear();
	
	funList->setColumnCount(1);
	funList->setRowCount(0);
	funList->insertRow(0);
	QTableWidgetItem *item=new QTableWidgetItem();
	item->setText("��");
	item->setFlags(item->flags()&~Qt::ItemIsEditable);
	funList->setItem(0,0,item);

	funList->insertRow(1);
	item=new QTableWidgetItem();
	item->setText("���");
	item->setFlags(item->flags()&~Qt::ItemIsEditable);
	funList->setItem(1,0,item);

	funList->insertRow(2);
	item=new QTableWidgetItem();
	item->setText("���");
	item->setFlags(item->flags()&~Qt::ItemIsEditable);
	funList->setItem(2,0,item);

	funList->insertRow(3);
	item=new QTableWidgetItem();
	item->setText("ʱ��");
	item->setFlags(item->flags()&~Qt::ItemIsEditable);
	funList->setItem(3,0,item);

	funList->insertRow(4);
	item=new QTableWidgetItem();
	item->setText("��ѯ����");
	item->setFlags(item->flags()&~Qt::ItemIsEditable);
	funList->setItem(4,0,item);

	funList->insertRow(5);
	item=new QTableWidgetItem();
	item->setText("����");
	item->setFlags(item->flags()&~Qt::ItemIsEditable);
	funList->setItem(5,0,item);

	funList->insertRow(6);
	item=new QTableWidgetItem();
	item->setText("����");
	item->setFlags(item->flags()&~Qt::ItemIsEditable);
	funList->setItem(6,0,item);

	if(fun!="")
		curFunLabel->setText(QString("��ǰ����:")+fun);
	else
		curFunLabel->setText("");
	funList->verticalHeader()->setDefaultSectionSize(17);
	funList->horizontalHeader()->setHidden(true);
	funList->verticalHeader()->setHidden(true);
	funList->horizontalHeader()->setStretchLastSection(true);
	funList->setSelectionMode(QAbstractItemView::SingleSelection);

	colList->setColumnCount(2);
	colList->setRowCount(0);
	colList->horizontalHeader()->setHidden(true);
	colList->verticalHeader()->setHidden(true);
	colList->setColumnHidden(1,true);
	colList->verticalHeader()->setDefaultSectionSize(17);
	colList->horizontalHeader()->setStretchLastSection(true);
	colList->setSelectionMode(QAbstractItemView::SingleSelection);
	//int count=dbSet->GetColumnCount();
	for(int i=0;i<dbSet->GetColumnCount();i++)
	{
		colList->insertRow(i);
		QTableWidgetItem *item=new QTableWidgetItem();
		item->setText(dbSet->GetColumn(i)->m_sName.data());
		item->setFlags(item->flags()&~Qt::ItemIsEditable);
		colList->setItem(i,0,item);

		item=new QTableWidgetItem();
		item->setText(dbSet->GetRecordset()->GetColumnName(i).data());
		item->setFlags(item->flags()&~Qt::ItemIsEditable);
		colList->setItem(i,1,item);
	}
	if(col.toInt()>=0)
	{
		QTableWidgetItem *item = colList->item(col.toInt(),0);
		if(item == NULL)
			curColLabel->setText(QString("��ǰ��:δ����"));
		else
			curColLabel->setText(QString("��ǰ��:")+item->text());
	}
	else
		curColLabel->setText("");
}
void settingExpress_dlg::onOk()
{
	QString sColText="";
	if(colList->currentRow()>=0)
	{
		int row=colList->currentItem()->row();
		col=QString::number(row);
		sColText = colList->item(row,0)->text();
		//tab2->setTxt(col+":"+colList->currentItem()->text());
	}
	else
		col="";
	//0 �� 1 ��� 2 ��� 3 ��ǰʱ�� 4 ��ѯ���� 5 ��Ŀ���� 6 ����
	if(funList->selectedRanges().size()!=0&&funList->currentRow()>=0)
	{
		int row=funList->currentItem()->row();
		switch (row)
		{
		case 0:
			fun="";
			break;
		case 1:
			fun="no";
			break;
		case 2:
			fun="sum";
			break;
		case 3:
			fun="time";
			break;
		case 4:
			fun="selcon";
			break;
		case 5:
			fun="count";
			break;
		case 6:
			fun="author";
			break;
		default:
			break;
		}
	}
// 	else
// 		fun="";
	txt=tab2->getTxt();
	if(sColText.length()>0)
		txt = sColText;
	font=tab2->getFont();
	color=tab2->getColor();
	txtop=tab2->getTxtOp();
	isRect=tab2->getIsRect();
	rectPx=tab2->getRectpx();
	rectColor=tab2->getRectColor();
	hide();
}
void settingExpress_dlg::onCancel()
{
	col="";
	hide();
}
void settingExpress_dlg::setTxt(QString txt)
{
	this->txt=txt;
	tab2->setTxt(this->txt);
}
QString settingExpress_dlg::getTxt()
{
	return txt;
}
void settingExpress_dlg::setDs(CSsp_Dataset* dbSet)
{
	this->dbSet=dbSet;
}
void settingExpress_dlg::setFun(QString fun)
{
	this->fun=fun;
}
QString settingExpress_dlg::getFun()
{
	return fun;
}
void settingExpress_dlg::setExp(QString col)
{
	this->col=col;
}
QString settingExpress_dlg::getExp()
{
	return col;
}
void settingExpress_dlg::setFontSize(QString fontSize)
{
	font.setPointSize(fontSize.toInt());
	tab2->setFont(font);
}
QString settingExpress_dlg::getFontSize()
{
	int tmp=font.pointSize();
	return QString::number(tmp);
}
void settingExpress_dlg::setFontFamily(QString family)
{
	font.setFamily(family);
	tab2->setFont(font);
}
QString settingExpress_dlg::getFontFamily()
{
	return font.family();
}
void settingExpress_dlg::setColor(QColor color)
{
	this->color=color;
	tab2->setColor(color);
}
QColor settingExpress_dlg::getColor()
{
	return color;
}
void settingExpress_dlg::setFontType(int type)//0 normal 1 bold 2 italic 3 bolditalic
{
	if(type==0)
	{
		font.setBold(false);
		font.setItalic(false);
	}
	else if(type==1)
	{
		font.setBold(true);
		font.setItalic(false);
	}
	else if(type==2)
	{
		font.setBold(false);
		font.setItalic(true);
	}
	else if(type==3)
	{
		font.setBold(true);
		font.setItalic(true);
	}
}
int settingExpress_dlg::getFontType()
{
	if(!font.bold()&&!font.italic())
		return 0;
	else if(font.bold()&&!font.italic())
		return 1;
	else if(!font.bold()&&font.italic())
		return 2;
	else if(font.bold()&&font.italic())
		return 3;
	return 0;
}
void settingExpress_dlg::setTxtOp(int op)//0 center 1 left 2 right
{
	txtop=op;
	tab2->setTxtOp(op);
}
int settingExpress_dlg::getTxtOp()
{
	return txtop;
}
void settingExpress_dlg::setFontDelLine(bool delLine)
{
	if(delLine)
		font.setStrikeOut(true);
	else
		font.setStrikeOut(false);
	tab2->setFont(font);
}
bool settingExpress_dlg::getFontDelLine()
{
	return font.strikeOut();
}
void settingExpress_dlg::setFontUndLine(bool undLine)
{
	if(undLine)
		font.setUnderline(true);
	else
		font.setUnderline(false);
	tab2->setFont(font);
}
bool settingExpress_dlg::getFontUndLine()
{
	return font.underline();
}
int settingExpress_dlg::getIsRect()
{
	return isRect;
}
void settingExpress_dlg::setIsRect(int isRect)
{
	this->isRect=isRect;
	tab2->setIsRect(isRect);
}
int settingExpress_dlg::getRectpx()
{
	return rectPx;
}
void settingExpress_dlg::setRectpx(int px)
{
	rectPx=px;
	tab2->setRectpx(rectPx);
}
QColor settingExpress_dlg::getRectColor()
{
	return rectColor;
}
void settingExpress_dlg::setRectColor(QColor color)
{
	rectColor=color;
	tab2->setRectColor(color);
}
////////////////////////////////////////////////////////////////////////////////////
//���߿�
static void qt_graphicsItem_highlightSelected(QGraphicsItem *item, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
	const QRectF murect = painter->transform().mapRect(QRectF(0, 0, 1, 1));
	if (qFuzzyIsNull(qMax(murect.width(), murect.height())))
		return;

	const QRectF mbrect = painter->transform().mapRect(item->boundingRect());
	if (qMin(mbrect.width(), mbrect.height()) < qreal(1.0))
		return;

	qreal itemPenWidth=1.0;

	const qreal pad = itemPenWidth / 2;

	//const qreal penWidth = 0; // cosmetic pen

	const QColor fgcolor = option->palette.windowText().color();
	const QColor bgcolor( // ensure good contrast against fgcolor
		fgcolor.red()   > 127 ? 0 : 255,
		fgcolor.green() > 127 ? 0 : 255,
		fgcolor.blue()  > 127 ? 0 : 255);

// 	painter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
// 	painter->setBrush(Qt::NoBrush);
// 	painter->drawRect(item->boundingRect().adjusted(pad, pad, -pad, -pad));

	painter->setPen(QPen(option->palette.windowText(), 0, Qt::DashLine));
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(item->boundingRect().adjusted(pad, pad, -pad, -pad));
}
///////////////////////////////////////////////////////////////////////////////////////
settingGroup_dlg::settingGroup_dlg(int dsNo,QWidget *parent)
	: QDialog(parent)
{
	//�������öԻ���
	setWindowTitle(tr("������"));

	colLabel=new QLabel(this);
	curColLabel=new QLabel(this);
	colLabel->setText("������");
	colList=new QTableWidget(this);

	tabWidget=new QTabWidget(this);
	tab1=new QWidget(this);
	tab2=new txtsetting_widget(this);
	tab2->setTwoTxt(true);

	delGrp=new QPushButton(this);
	delGrp->setText(tr("ɾ������(&D)"));
	QIcon icon2;
	icon2.addFile(QString::fromUtf8(":/ssp_gui/Resources/20070402165933865.gif"), QSize(), QIcon::Normal, QIcon::Off);
	delGrp->setIcon(icon2);

	delGrp->setMaximumHeight(24);
	Ok=new QPushButton(this);
	Ok->setText(tr("ȷ��(&O)"));
	Ok->setMaximumHeight(24);
	Ok->setDefault(true);
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/image/ok"), QSize(), QIcon::Normal, QIcon::Off);
	Ok->setIcon(icon);
	cancel=new QPushButton(this);
	cancel->setText(tr("ȡ��(&C)"));
	cancel->setMaximumHeight(24);
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/image/cancel"), QSize(), QIcon::Normal, QIcon::Off);
	cancel->setIcon(icon1);

	QHBoxLayout *labelcollayout=new QHBoxLayout();
	QVBoxLayout *collayout=new QVBoxLayout();
	QHBoxLayout *bottomlayout=new QHBoxLayout();
	QVBoxLayout *tablayout=new QVBoxLayout();
	QHBoxLayout *btnlayout=new QHBoxLayout();
	//QHBoxLayout *toplayout=new QHBoxLayout();

	labelcollayout->addWidget(colLabel);
	labelcollayout->addWidget(curColLabel);
	labelcollayout->addStretch();
	labelcollayout->setMargin(2);

	collayout->addLayout(labelcollayout);
	collayout->addWidget(colList);
	collayout->setMargin(2);

	bottomlayout->addLayout(collayout);
	bottomlayout->setMargin(2);

	btnlayout->addStretch();
	btnlayout->addWidget(delGrp);
	btnlayout->addWidget(Ok);
	btnlayout->addWidget(cancel);
	btnlayout->setMargin(2);

// 	tablayout->addStretch();
// 	tablayout->addLayout(btnlayout);
// 	tablayout->setMargin(2);

	tablayout->addLayout(bottomlayout);
	tablayout->setMargin(2);
	tab1->setLayout(tablayout);

	tabWidget->addTab(tab1,"������");
	tabWidget->addTab(tab2,"�ı�����");

	m_fieldlist = new SPtrList<CSsp_DwColumn>;
	m_dataWindow = GET_DW_CFG;
	m_pOper=DB;
	this->dsNo=dsNo;
	this->col=col;

	QVBoxLayout *layout=new QVBoxLayout();
	layout->addWidget(tabWidget);
	layout->addLayout(btnlayout);
	layout->setMargin(2);
	setLayout(layout);

	//initDlg();
}
settingGroup_dlg::~settingGroup_dlg()
{

}
void settingGroup_dlg::initDlg()
{
	SString dsName=m_pOper->SelectInto(SString::toFormat("select name from t_ssp_data_window where dw_sn=%d",dsNo));
	CSsp_Dataset* dbSet=NULL;
	colList->setColumnCount(2);
	colList->setRowCount(0);
	colList->horizontalHeader()->setHidden(true);
	colList->verticalHeader()->setHidden(true);
	colList->setColumnHidden(1,true);
	colList->verticalHeader()->setDefaultSectionSize(17);
	colList->horizontalHeader()->setStretchLastSection(true);
	colList->setSelectionMode(QAbstractItemView::SingleSelection);
	dbSet=m_dataWindow->SearchDataset(dsName);
	for(int i=0;i<dbSet->GetColumnCount();i++)
	{
		colList->insertRow(i);
		QTableWidgetItem *item=new QTableWidgetItem();
		item->setText(dbSet->GetColumn(i)->m_sName.data());
		item->setFlags(item->flags()&~Qt::ItemIsEditable);
		colList->setItem(i,0,item);

		item=new QTableWidgetItem();
		item->setText(dbSet->GetRecordset()->GetColumnName(i).data());
		item->setFlags(item->flags()&~Qt::ItemIsEditable);
		colList->setItem(i,1,item);
		//colList->addItem(dbSet->GetColumn(i)->m_sName.data());
	}

	connect(delGrp,SIGNAL(clicked()),this,SLOT(onDelGrp()));
	connect(Ok,SIGNAL(clicked()),this,SLOT(onOk()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(onCancel()));
}
void settingGroup_dlg::onDelGrp()
{
	emit deleteGrp();
	col="";
	hide();
}
void settingGroup_dlg::onOk()
{
	if(colList->currentRow()>=0)
		col=QString::number(colList->currentRow());

	txt=tab2->getTxt();
	font=tab2->getFont();
	color=tab2->getColor();
	txtop=tab2->getTxtOp();
	isRect=tab2->getIsRect();
	rectPx=tab2->getRectpx();
	rectColor=tab2->getRectColor();

	hide();
}
void settingGroup_dlg::onCancel()
{
	col="";
	hide();
}
void settingGroup_dlg::setDs(int dsNo)
{
	this->dsNo=dsNo;
}

void settingGroup_dlg::setGrp(QString grp)
{
	this->col=grp;
	if(grp!="")
	{
		curColLabel->setText("��ǰ��"+grp);
		colList->setCurrentCell(grp.toInt(),0);
	}
	else
		curColLabel->setText("");

}
QString settingGroup_dlg::getGrp()
{
	return col;
}

void settingGroup_dlg::setTxt(QString txt)
{
	this->txt=txt;
	tab2->setTxt(this->txt);
}
QString settingGroup_dlg::getTxt()
{
	return txt;
}
void settingGroup_dlg::setFontSize(QString fontSize)
{
	font.setPointSize(fontSize.toInt());
	tab2->setFont(font);
}
QString settingGroup_dlg::getFontSize()
{
	int tmp=font.pointSize();
	return QString::number(tmp);
}
void settingGroup_dlg::setFontFamily(QString family)
{
	font.setFamily(family);
	tab2->setFont(font);
}
QString settingGroup_dlg::getFontFamily()
{
	return font.family();
}
void settingGroup_dlg::setColor(QColor color)
{
	this->color=color;
	tab2->setColor(color);
}
QColor settingGroup_dlg::getColor()
{
	return color;
}
void settingGroup_dlg::setFontType(int type)//0 normal 1 bold 2 italic 3 bolditalic
{
	if(type==0)
	{
		font.setBold(false);
		font.setItalic(false);
	}
	else if(type==1)
	{
		font.setBold(true);
		font.setItalic(false);
	}
	else if(type==2)
	{
		font.setBold(false);
		font.setItalic(true);
	}
	else if(type==3)
	{
		font.setBold(true);
		font.setItalic(true);
	}
}
int settingGroup_dlg::getFontType()
{
	if(!font.bold()&&!font.italic())
		return 0;
	else if(font.bold()&&!font.italic())
		return 1;
	else if(!font.bold()&&font.italic())
		return 2;
	else if(font.bold()&&font.italic())
		return 3;
	return 0;
}
void settingGroup_dlg::setTxtOp(int op)//0 center 1 left 2 right
{
	txtop=op;
	tab2->setTxtOp(op);
}
int settingGroup_dlg::getTxtOp()
{
	return txtop;
}
void settingGroup_dlg::setFontDelLine(bool delLine)
{
	if(delLine)
		font.setStrikeOut(true);
	else
		font.setStrikeOut(false);
	tab2->setFont(font);
}
bool settingGroup_dlg::getFontDelLine()
{
	return font.strikeOut();
}
void settingGroup_dlg::setFontUndLine(bool undLine)
{
	if(undLine)
		font.setUnderline(true);
	else
		font.setUnderline(false);
	tab2->setFont(font);
}
bool settingGroup_dlg::getFontUndLine()
{
	return font.underline();
}
int settingGroup_dlg::getIsRect()
{
	return isRect;
}
void settingGroup_dlg::setIsRect(int isRect)
{
	this->isRect=isRect;
	tab2->setIsRect(isRect);
}
int settingGroup_dlg::getRectpx()
{
	return rectPx;
}
void settingGroup_dlg::setRectpx(int px)
{
	rectPx=px;
	tab2->setRectpx(rectPx);
}
QColor settingGroup_dlg::getRectColor()
{
	return rectColor;
}
void settingGroup_dlg::setRectColor(QColor color)
{
	rectColor=color;
	tab2->setRectColor(color);
}
//////////////////////////////////////////////////////////////////////
pagesetting_dlg::pagesetting_dlg(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("ҳ������"));

	widthLabel=new QLabel(this);
	widthLabel->setText("�������");
	widthLine=new QLineEdit(this);

	heightLabel=new QLabel(this);
	heightLabel->setText("�߶�����");
	heightLine=new QLineEdit(this);

	Ok=new QPushButton(this);
	Ok->setText(tr("ȷ��(&O)"));
	Ok->setMaximumHeight(24);
	Ok->setDefault(true);
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/image/ok"), QSize(), QIcon::Normal, QIcon::Off);
	Ok->setIcon(icon);
	cancel=new QPushButton(this);
	cancel->setText(tr("ȡ��(&C)"));
	cancel->setMaximumHeight(24);
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/image/cancel"), QSize(), QIcon::Normal, QIcon::Off);
	cancel->setIcon(icon1);

	QHBoxLayout *widthlayout=new QHBoxLayout();
	QHBoxLayout *heightlayout=new QHBoxLayout();
	QVBoxLayout *bottomlayout=new QVBoxLayout();
	QHBoxLayout *btnlayout=new QHBoxLayout();

	widthlayout->addWidget(widthLabel);
	widthlayout->addWidget(widthLine);
	widthlayout->addStretch();
	widthlayout->setMargin(2);

	//widthLabel->hide();
	//widthLine->hide();
	heightlayout->addWidget(heightLabel);
	heightlayout->addWidget(heightLine);
	heightlayout->addStretch();
	heightlayout->setMargin(2);

	bottomlayout->addLayout(widthlayout);
	bottomlayout->addLayout(heightlayout);
	bottomlayout->addStretch();
	bottomlayout->setMargin(2);

	btnlayout->addStretch();
	btnlayout->addWidget(Ok);
	btnlayout->addWidget(cancel);
	btnlayout->setMargin(2);

	bottomlayout->addLayout(btnlayout);
	setLayout(bottomlayout);
	initDlg();
	connect(Ok,SIGNAL(clicked()),this,SLOT(onOk()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(onCancel()));
}
pagesetting_dlg::~pagesetting_dlg()
{

}
void pagesetting_dlg::initDlg()
{
	widthLine->setText(QString::number(width));
	heightLine->setText(QString::number(height));

}
void pagesetting_dlg::onOk()
{
	height=heightLine->text().toInt();
	width=widthLine->text().toInt();
	hide();
}
void pagesetting_dlg::onCancel()
{
	hide();
}
void pagesetting_dlg::getWH(int &width,int &height)
{
	width=this->width;
	height=this->height;
}
void pagesetting_dlg::setWH(int width,int height)
{
	this->width=width;
	this->height=height;
	initDlg();
}
//////////////////////////////////////////////////////////////////////
txtsetting_widget::txtsetting_widget(QWidget *parent)
	: QWidget(parent)
{
	setWindowTitle(tr("�ı�����"));
	//�ı�����ҳ��
	txtLabel=new QLabel(this);
	txtLabel->setText("�ı�");
	txtLine=new QLineEdit(this);

	txtLabel2=new QLabel(this);
	txtLabel2->setText("��׺");
	txtLine2=new QLineEdit(this);

	fontExampleLabel=new QLabel(this);
	fontExampleLabel->setText("ʾ��");

	fontButton=new QPushButton(this);
	fontButton->setText("��������");
	colorButton=new QPushButton(this);
	colorButton->setText("��ɫ����");

	txtOpCbo=new QComboBox(this);
	txtOpCbo->addItem("���ж���");
	txtOpCbo->addItem("�����");
	txtOpCbo->addItem("�Ҷ���");
	txtOpLabel=new QLabel(this);
	txtOpLabel->setText("�ı�λ��");

	isRect=new QCheckBox("��ʾ����",this);
	pxLabel=new QLabel("�����߿�",this);
	pxLine=new QLineEdit(this);
	pxcolorButton=new QPushButton("������ɫ",this);

	QHBoxLayout *txtlayout=new QHBoxLayout();
	QHBoxLayout *fontlayout=new QHBoxLayout();
	QHBoxLayout *txtOplayout=new QHBoxLayout();
	QHBoxLayout *pxlayout=new QHBoxLayout();
	QHBoxLayout *rectlayout=new QHBoxLayout();
	QVBoxLayout *bottomlayout=new QVBoxLayout();
	QHBoxLayout *btnlayout=new QHBoxLayout();

	txtlayout->addWidget(txtLabel);
	txtlayout->addWidget(txtLine);
	txtlayout->addWidget(txtLabel2);
	txtlayout->addWidget(txtLine2);
	txtlayout->addStretch();
	txtlayout->setMargin(2);

	fontlayout->addWidget(fontExampleLabel);
	fontlayout->addWidget(fontButton);
	fontlayout->addWidget(colorButton);
	fontlayout->addStretch();
	fontlayout->setMargin(2);

	txtOplayout->addWidget(txtOpLabel);
	txtOplayout->addWidget(txtOpCbo);
	txtOplayout->addStretch();
	txtOplayout->setMargin(2);

	pxlayout->addWidget(pxLabel);
	pxlayout->addWidget(pxLine);
	pxlayout->setMargin(2);

	rectlayout->addWidget(isRect);
	rectlayout->addLayout(pxlayout);
	rectlayout->addWidget(pxcolorButton);
	rectlayout->addStretch();
	rectlayout->setMargin(2);

	bottomlayout->addLayout(txtlayout);
	bottomlayout->addLayout(fontlayout);
	bottomlayout->addLayout(txtOplayout);
	bottomlayout->addLayout(rectlayout);
	bottomlayout->addStretch();
	bottomlayout->addLayout(btnlayout);
	bottomlayout->setMargin(2);
	setLayout(bottomlayout);
	txtLabel2->hide();
	txtLine2->hide();
	twoTxt=false;
	initDlg();	
	connect(fontButton,SIGNAL(clicked()),this,SLOT(onfontButton()));
	connect(colorButton,SIGNAL(clicked()),this,SLOT(oncolorButton()));

	connect(isRect,SIGNAL(stateChanged(int)),this,SLOT(onisRectCheck()));
	connect(pxLine,SIGNAL(textChanged (QString)),this,SLOT(onpxLine(QString)));
	connect(pxcolorButton,SIGNAL(clicked()),this,SLOT(onpxcolorButton()));
}
txtsetting_widget::~txtsetting_widget()
{

}
void txtsetting_widget::initDlg()
{
	txtLine->setText(txt);
	fontExampleLabel->setFont(font);
	QPalette palette;
	palette.setColor(QPalette::WindowText, color);  
	fontExampleLabel->setPalette(palette);
	if(isRect->checkState()==Qt::Checked)
	{
		QString srgb=QString::number(rectColor.red())+","+QString::number(rectColor.green())+","+QString::number(rectColor.blue());
		QString srgb2=QString::number(this->color.red())+","+QString::number(this->color.green())+","+QString::number(this->color.blue());
		QString ss;
		ss.sprintf("border:%dpx solid rgb(%s);color:rgb(%s);",px,srgb.toStdString().data(),srgb2.toStdString().data());
		fontExampleLabel->setStyleSheet(ss);
	}
	else
	{
		QString srgb2=QString::number(this->color.red())+","+QString::number(this->color.green())+","+QString::number(this->color.blue());
		QString ss;
		ss.sprintf("color:rgb(%s);",srgb2.toStdString().data());
		fontExampleLabel->setStyleSheet(ss);
	}
}
void txtsetting_widget::setTwoTxt(bool isTwo)
{
	twoTxt=isTwo;
	if(isTwo==true)
	{
		txtLabel->setText("ǰ׺");
		txtLabel2->setHidden(false);
		txtLine2->setHidden(false);
	}
	else
	{
		txtLabel->setText("�ı�");
		txtLabel2->hide();
		txtLine2->hide();
	}
}
void txtsetting_widget::onfontButton()
{
	bool ok;  
	font=QFontDialog::getFont(&ok,font,this,tr("����Ի���")); 
	fontExampleLabel->setFont(font);
}
void txtsetting_widget::oncolorButton()
{
	color = QColorDialog::getColor(color, this);  
	if(isRect->checkState()==Qt::Checked)
	{
		QString srgb=QString::number(rectColor.red())+","+QString::number(rectColor.green())+","+QString::number(rectColor.blue());
		QString srgb2=QString::number(this->color.red())+","+QString::number(this->color.green())+","+QString::number(this->color.blue());
		QString ss;
		ss.sprintf("border:%dpx solid rgb(%s);color:rgb(%s);",px,srgb.toStdString().data(),srgb2.toStdString().data());
		fontExampleLabel->setStyleSheet(ss);
	}
	else
	{
		QString srgb2=QString::number(this->color.red())+","+QString::number(this->color.green())+","+QString::number(this->color.blue());
		QString ss;
		ss.sprintf("color:rgb(%s);",srgb2.toStdString().data());
		fontExampleLabel->setStyleSheet(ss);
	}
}
void txtsetting_widget::onisRectCheck()
{
	if(((QCheckBox*)sender())->checkState()==Qt::Checked)
	{
		QString srgb=QString::number(rectColor.red())+","+QString::number(rectColor.green())+","+QString::number(rectColor.blue());
		QString srgb2=QString::number(this->color.red())+","+QString::number(this->color.green())+","+QString::number(this->color.blue());
		QString ss;
		ss.sprintf("border:%dpx solid rgb(%s);color:rgb(%s);",px,srgb.toStdString().data(),srgb2.toStdString().data());
		fontExampleLabel->setStyleSheet(ss);
	}
	else
	{
		QString srgb2=QString::number(this->color.red())+","+QString::number(this->color.green())+","+QString::number(this->color.blue());
		QString ss;
		ss.sprintf("color:rgb(%s);",srgb2.toStdString().data());
		fontExampleLabel->setStyleSheet(ss);
	}
}
void txtsetting_widget::onpxLine(QString txt)
{
	px=txt.toInt();
	if(isRect->checkState()==Qt::Checked)
	{
		QString srgb=QString::number(rectColor.red())+","+QString::number(rectColor.green())+","+QString::number(rectColor.blue());
		QString srgb2=QString::number(this->color.red())+","+QString::number(this->color.green())+","+QString::number(this->color.blue());
		QString ss;
		ss.sprintf("border:%dpx solid rgb(%s);color:rgb(%s);",px,srgb.toStdString().data(),srgb2.toStdString().data());
		fontExampleLabel->setStyleSheet(ss);
	}
	else
	{
		QString srgb2=QString::number(this->color.red())+","+QString::number(this->color.green())+","+QString::number(this->color.blue());
		QString ss;
		ss.sprintf("color:rgb(%s);",srgb2.toStdString().data());
		fontExampleLabel->setStyleSheet(ss);
	}
}
void txtsetting_widget::onpxcolorButton()
{
	rectColor = QColorDialog::getColor(rectColor, this);  
	if(rectColor.isValid()) 
	{  
		QString srgb=QString::number(rectColor.red())+","+QString::number(rectColor.green())+","+QString::number(rectColor.blue());
		if(isRect->checkState()==Qt::Checked)
		{
			QString srgb=QString::number(rectColor.red())+","+QString::number(rectColor.green())+","+QString::number(rectColor.blue());
			QString srgb2=QString::number(this->color.red())+","+QString::number(this->color.green())+","+QString::number(this->color.blue());
			QString ss;
			ss.sprintf("border:%dpx solid rgb(%s);color:rgb(%s);",px,srgb.toStdString().data(),srgb2.toStdString().data());
			fontExampleLabel->setStyleSheet(ss);
		}
		else
		{
			QString srgb2=QString::number(this->color.red())+","+QString::number(this->color.green())+","+QString::number(this->color.blue());
			QString ss;
			ss.sprintf("color:rgb(%s);",srgb2.toStdString().data());
			fontExampleLabel->setStyleSheet(ss);
		}
	}  
}
QString txtsetting_widget::getTxt()
{
	if(twoTxt==false)
		txt=txtLine->text();
	else
	{
		txt=txtLine->text()+"@"+txtLine2->text();
	}
	return txt;
}
QFont txtsetting_widget::getFont()
{
	return font;
}
QColor txtsetting_widget::getColor()
{
	return color;
}
int txtsetting_widget::getTxtOp()
{
	int txtOp=txtOpCbo->currentIndex();
	return txtOp;
}
void txtsetting_widget::setTxt(QString txt)
{
	this->txt=txt;
	if(twoTxt==false)
		txtLine->setText(txt);
	else
	{
		QStringList tmpTxt=txt.split("@");
		txtLine->setText(tmpTxt.at(0));
		if(tmpTxt.size()>1)
			txtLine2->setText(tmpTxt.at(1));
		else
			txtLine2->setText("");
	}
	
}
void txtsetting_widget::setColor(QColor color)
{
	this->color=color;
}
void txtsetting_widget::setFont(QFont font)
{
	this->font=font;
	fontExampleLabel->setFont(font);
}
void txtsetting_widget::setTxtOp(int txtOp)
{
	txtOpCbo->setCurrentIndex(txtOp);
}
int txtsetting_widget::getIsRect()
{
	if(isRect->checkState()==Qt::Checked)
		return 1;
	else
		return 0;
}
int txtsetting_widget::getRectpx()
{
	return px;
}
QColor txtsetting_widget::getRectColor()
{
	return rectColor;
}
void txtsetting_widget::setIsRect(int isRect)
{
	if(isRect==1)
		this->isRect->setCheckState(Qt::Checked);
	else
		this->isRect->setCheckState(Qt::Unchecked);
}
void txtsetting_widget::setRectpx(int px)
{
	this->px=px;
	pxLine->setText(QString::number(px));
}
void txtsetting_widget::setRectColor(QColor color)
{
	rectColor=color;
	pxLine->setText(QString::number(px));

	//QString srgb=QString::number(rectColor.red())+","+QString::number(rectColor.green())+","+QString::number(rectColor.blue());
	if(isRect->checkState()==Qt::Checked)
	{
		QString srgb=QString::number(rectColor.red())+","+QString::number(rectColor.green())+","+QString::number(rectColor.blue());
		QString srgb2=QString::number(this->color.red())+","+QString::number(this->color.green())+","+QString::number(this->color.blue());
		QString ss;
		ss.sprintf("border:%dpx solid rgb(%s);color:rgb(%s);",px,srgb.toStdString().data(),srgb2.toStdString().data());
		fontExampleLabel->setStyleSheet(ss);
	}
	else
	{
		QString srgb2=QString::number(this->color.red())+","+QString::number(this->color.green())+","+QString::number(this->color.blue());
		QString ss;
		ss.sprintf("color:rgb(%s);",srgb2.toStdString().data());
		fontExampleLabel->setStyleSheet(ss);
	}
}
////////////////////////////////////////////////////////////////////////////////////
txtsetting_dialog::txtsetting_dialog(QWidget *parent)
	: QDialog(parent)
{
	//�ı����öԻ���
	setWindowTitle(tr("�ı�����"));
	wig=new txtsetting_widget(this);

	Ok=new QPushButton(this);
	Ok->setText(tr("ȷ��(&O)"));
	Ok->setMaximumHeight(24);
	Ok->setDefault(true);
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/image/ok"), QSize(), QIcon::Normal, QIcon::Off);
	Ok->setIcon(icon);
	cancel=new QPushButton(this);
	cancel->setText(tr("ȡ��(&C)"));
	cancel->setMaximumHeight(24);
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/image/cancel"), QSize(), QIcon::Normal, QIcon::Off);
	cancel->setIcon(icon1);

	QVBoxLayout *bottomlayout=new QVBoxLayout();
	QHBoxLayout *btnlayout=new QHBoxLayout();

	btnlayout->addStretch();
	btnlayout->addWidget(Ok);
	btnlayout->addWidget(cancel);
	
	btnlayout->setMargin(2);

	bottomlayout->addWidget(wig);
	bottomlayout->addStretch();
	bottomlayout->addLayout(btnlayout);
	bottomlayout->setMargin(2);

	setLayout(bottomlayout);
	initDlg();	
	connect(Ok,SIGNAL(clicked()),this,SLOT(onOk()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(onCancel()));

}
txtsetting_dialog::~txtsetting_dialog()
{

}
void txtsetting_dialog::initDlg()
{
	
}
void txtsetting_dialog::onOk()
{
	txt=wig->getTxt();
	font=wig->getFont();
	color=wig->getColor();
	txtop=wig->getTxtOp();
	isRect=wig->getIsRect();
	rectPx=wig->getRectpx();
	rectColor=wig->getRectColor();
	hide();
}
void txtsetting_dialog::onCancel()
{
	hide();
}
void txtsetting_dialog::setTxt(QString txt)
{
	this->txt=txt;
	wig->setTxt(this->txt);
}
QString txtsetting_dialog::getTxt()
{
	return txt;
}
void txtsetting_dialog::setFontSize(QString fontSize)
{
	font.setPointSize(fontSize.toInt());
	wig->setFont(font);
}
QString txtsetting_dialog::getFontSize()
{
	int tmp=font.pointSize();
	return QString::number(tmp);
}
void txtsetting_dialog::setFontFamily(QString family)
{
	font.setFamily(family);
	wig->setFont(font);
}
QString txtsetting_dialog::getFontFamily()
{
	return font.family();
}
void txtsetting_dialog::setColor(QColor color)
{
	this->color=color;
	wig->setColor(color);
}
QColor txtsetting_dialog::getColor()
{
	return color;
}
void txtsetting_dialog::setFontType(int type)//0 normal 1 bold 2 italic 3 bolditalic
{
	if(type==0)
	{
		font.setBold(false);
		font.setItalic(false);
	}
	else if(type==1)
	{
		font.setBold(true);
		font.setItalic(false);
	}
	else if(type==2)
	{
		font.setBold(false);
		font.setItalic(true);
	}
	else if(type==3)
	{
		font.setBold(true);
		font.setItalic(true);
	}
	wig->setFont(font);
}
int txtsetting_dialog::getFontType()
{
	if(!font.bold()&&!font.italic())
		return 0;
	else if(font.bold()&&!font.italic())
		return 1;
	else if(!font.bold()&&font.italic())
		return 2;
	else if(font.bold()&&font.italic())
		return 3;
	return 0;
}
void txtsetting_dialog::setTxtOp(int op)//0 center 1 left 2 right
{
	txtop=op;
	wig->setTxtOp(op);
}
int txtsetting_dialog::getTxtOp()
{
	return txtop;
}
void txtsetting_dialog::setFontDelLine(bool delLine)
{
	if(delLine)
		font.setStrikeOut(true);
	else
		font.setStrikeOut(false);
	wig->setFont(font);
}
bool txtsetting_dialog::getFontDelLine()
{
	return font.strikeOut();
}
void txtsetting_dialog::setFontUndLine(bool undLine)
{
	if(undLine)
		font.setUnderline(true);
	else
		font.setUnderline(false);
	wig->setFont(font);
}
bool txtsetting_dialog::getFontUndLine()
{
	return font.underline();
}
int txtsetting_dialog::getIsRect()
{
	return isRect;
}
void txtsetting_dialog::setIsRect(int isRect)
{
	this->isRect=isRect;
	wig->setIsRect(isRect);
}
int txtsetting_dialog::getRectpx()
{
	return rectPx;
}
void txtsetting_dialog::setRectpx(int px)
{
	rectPx=px;
	wig->setRectpx(rectPx);
}
QColor txtsetting_dialog::getRectColor()
{
	return rectColor;
}
void txtsetting_dialog::setRectColor(QColor color)
{
	rectColor=color;
	wig->setRectColor(color);
}
////////////////////////////////////////////////////////////////////////////////////
linesetting_dialog::linesetting_dialog(QWidget *parent)
	: QDialog(parent)
{
	//�߶����öԻ���
	setWindowTitle(tr("�ı�����"));
	
	colorButton=new QPushButton(this);
	colorButton->setText("��ɫ����");
	pxLabel=new QLabel(this);
	pxLabel->setText("�߿�");
	pxLine=new QLineEdit(this);
	typeLabel=new QLabel(this);
	typeLabel->setText("����");
	typeCbo=new QComboBox(this);

	Ok=new QPushButton(this);
	Ok->setText(tr("ȷ��(&O)"));
	Ok->setMaximumHeight(24);
	Ok->setDefault(true);
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/image/ok"), QSize(), QIcon::Normal, QIcon::Off);
	Ok->setIcon(icon);
	cancel=new QPushButton(this);
	cancel->setText(tr("ȡ��(&C)"));
	cancel->setMaximumHeight(24);
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/image/cancel"), QSize(), QIcon::Normal, QIcon::Off);
	cancel->setIcon(icon1);

	QHBoxLayout *colorlayout=new QHBoxLayout();
	QHBoxLayout *pxlayout=new QHBoxLayout();
	QHBoxLayout *typelayout=new QHBoxLayout();
	QVBoxLayout *bottomlayout=new QVBoxLayout();
	QHBoxLayout *btnlayout=new QHBoxLayout();

	colorlayout->addWidget(colorButton);
	colorlayout->addStretch();
	colorlayout->setMargin(2);

	pxlayout->addWidget(pxLabel);
	pxlayout->addWidget(pxLine);
	pxlayout->addStretch();
	pxlayout->setMargin(2);

	typelayout->addWidget(typeLabel);
	typelayout->addWidget(typeCbo);
	typelayout->setMargin(2);

	typeCbo->addItem("ʵ��");typeCbo->addItem("����");
	typeCbo->addItem("����");typeCbo->addItem("������");
	typelayout->addStretch();

	btnlayout->addWidget(Ok);
	btnlayout->addWidget(cancel);
	btnlayout->addStretch();
	btnlayout->setMargin(2);

	bottomlayout->addLayout(colorlayout);
	bottomlayout->addLayout(pxlayout);
	bottomlayout->addLayout(typelayout);
	bottomlayout->addStretch();
	bottomlayout->addLayout(btnlayout);
	bottomlayout->setMargin(2);
	setLayout(bottomlayout);
	initDlg();	
	connect(Ok,SIGNAL(clicked()),this,SLOT(onOk()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(onCancel()));
	connect(colorButton,SIGNAL(clicked()),this,SLOT(onColorButton()));
}
linesetting_dialog::~linesetting_dialog()
{

}
void linesetting_dialog::initDlg()
{

}
void linesetting_dialog::onOk()
{
	color=tmpColor;
	px=pxLine->text().toInt();
// 	if(px<2)
// 		px=2;
	type=typeCbo->currentIndex();
	hide();
}
void linesetting_dialog::onCancel()
{
	hide();
}
void linesetting_dialog::onColorButton()
{
	tmpColor=QColorDialog::getColor(tmpColor,this);  
}
void linesetting_dialog::setType(int type)
{
	this->type=type;
	typeCbo->setCurrentIndex(type);
}
int linesetting_dialog::getType()
{
	return type;
}
void linesetting_dialog::setColor(QColor color)
{
	this->color=color;
	tmpColor=color;
}
QColor linesetting_dialog::getColor()
{
	return color;
}
int linesetting_dialog::getpx()
{
	return px;
}
void linesetting_dialog::setpx(int px)
{
	this->px=px;
	pxLine->setText(QString::number(px));
}
/////////////////////////////////////////////////////////////////////////////////////////
chartsetting_dialog::chartsetting_dialog(QWidget *parent)
	: QDialog(parent)
{
	//ͼ�����öԻ���
	setWindowTitle(tr("���ݴ���ѡ��"));
	dsLabel=new QLabel(this);
	dsLabel->setText(tr("ѡ�����ݴ���"));
	curDsLabel=new QLabel(this);
	tableWidget=new QTableWidget(this);
	m_pOper=DB;
	chartType=0;

	Ok=new QPushButton(this);
	Ok->setText(tr("ȷ��(&O)"));
	Ok->setMaximumHeight(24);
	Ok->setDefault(true);
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/image/ok"), QSize(), QIcon::Normal, QIcon::Off);
	Ok->setIcon(icon);
	cancel=new QPushButton(this);
	cancel->setText(tr("ȡ��(&C)"));
	cancel->setMaximumHeight(24);
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/image/cancel"), QSize(), QIcon::Normal, QIcon::Off);
	cancel->setIcon(icon1);

	txtLabel=new QLabel(this);
	txtLabel->setText(tr("�ı�"));
	txtLine=new QLineEdit(this);

	typeLabel=new QLabel(this);
	typeLabel->setText(tr("ͼ������"));
	typeCbo=new QComboBox(this);
	typeCbo->addItem("��ͼ");
	typeCbo->addItem("��ͼ");
	typeCbo->addItem("����ͼ");
	typeCbo->addItem("�Ǳ���");//pointRow.pointCol��ʶָ��̶�

	extatuLabel=new QLabel(this);
	extatuLabel->setText("��չ����");
	extatuEdit=new QTextEdit(this);

	QVBoxLayout *labelsqllayout=new QVBoxLayout();
	QVBoxLayout *sqllayout=new QVBoxLayout();
	QHBoxLayout *txtlayout=new QHBoxLayout();
	QHBoxLayout *typelayout=new QHBoxLayout();
	QHBoxLayout *atulayout=new QHBoxLayout();
	QVBoxLayout *rightlayout=new QVBoxLayout();
	QHBoxLayout *toplayout=new QHBoxLayout();
	QHBoxLayout *btnlayout=new QHBoxLayout();

	labelsqllayout->addWidget(dsLabel);
	labelsqllayout->addStretch();
	labelsqllayout->addWidget(curDsLabel);
	labelsqllayout->setMargin(2);

	sqllayout->addLayout(labelsqllayout);
	sqllayout->addWidget(tableWidget);
	sqllayout->setMargin(2);

	txtlayout->addWidget(txtLabel);
	txtlayout->addWidget(txtLine);
	txtlayout->addStretch();
	txtlayout->setMargin(2);

	typelayout->addWidget(typeLabel);
	typelayout->addWidget(typeCbo);
	typelayout->addStretch();
	typelayout->setMargin(2);

	atulayout->addWidget(extatuLabel);
	atulayout->addWidget(extatuEdit);

	rightlayout->addLayout(txtlayout);
	rightlayout->addLayout(typelayout);
	rightlayout->addLayout(atulayout);
	rightlayout->addStretch();
	rightlayout->setMargin(2);

	toplayout->addLayout(sqllayout);
	toplayout->addLayout(rightlayout);
	toplayout->setMargin(2);

	btnlayout->addStretch();
	btnlayout->addWidget(Ok);
	btnlayout->addWidget(cancel);
	btnlayout->setMargin(2);
	QVBoxLayout *layout=new QVBoxLayout();
	layout->addLayout(toplayout);
	layout->addLayout(btnlayout);
	layout->setMargin(2);
	setLayout(layout);
	initDlg();
}
chartsetting_dialog::~chartsetting_dialog()
{

}
void chartsetting_dialog::initDlg()//���ñ������
{
	tableWidget->clear();
	QStringList header;
	header<<tr("���")<<tr("����")<<tr("����");
	tableWidget->setColumnCount(3);
	tableWidget->setRowCount(0);
	tableWidget->setHorizontalHeaderLabels(header);
	tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	tableWidget->setColumnWidth(0,50);
	tableWidget->setColumnWidth(1,100);
	tableWidget->setColumnWidth(2,200);
	tableWidget->setMinimumWidth(350);
	tableWidget->verticalHeader()->setDefaultSectionSize(17);
	tableWidget->horizontalHeader()->setStretchLastSection(true);
	tableWidget->horizontalHeader()->setAutoScroll(true);
	tableWidget->verticalHeader()->setAutoScroll(true);
	SRecordset dsSet;
	m_pOper->RetrieveRecordset("select dw_sn,dstype,name,dw_desc from t_ssp_data_window order by cls_name desc,dw_desc",dsSet);

	for(int i=0;i<dsSet.GetRows();i++)
	{
		if(dsSet.GetValue(i,1).toInt()!=1)
		{
			int rowCount=tableWidget->rowCount();
			tableWidget->insertRow(rowCount);

			SString tmp=dsSet.GetValue(i,0).data();
			QTableWidgetItem *item=new QTableWidgetItem();
			item->setText(tmp.data());
			item->setFlags(item->flags()&~Qt::ItemIsEditable);
			tableWidget->setItem(rowCount,0,item);

			tmp=dsSet.GetValue(i,2).data();
			item=new QTableWidgetItem();
			item->setText(tmp.data());
			item->setFlags(item->flags()&~Qt::ItemIsEditable);
			tableWidget->setItem(rowCount,1,item);

			tmp=dsSet.GetValue(i,3).data();
			item=new QTableWidgetItem();
			item->setText(tmp.data());
			item->setFlags(item->flags()&~Qt::ItemIsEditable);
			tableWidget->setItem(rowCount,2,item);
		}
	}
	txtLine->setText(txt);
	typeCbo->setCurrentIndex(chartType);
	
	connect(Ok,SIGNAL(clicked()),this,SLOT(onOk()));
	connect(cancel,SIGNAL(clicked()),this,SLOT(onCancel()));
}
void chartsetting_dialog::onOk()
{
	if(tableWidget->selectedItems().size()>0)
	{
		int row=tableWidget->selectedItems().at(0)->row();
		chartDs=tableWidget->item(row,0)->text().toInt();
	}
	txt=txtLine->text();
	extAtu=extatuEdit->toPlainText();
	extAtu.replace("=",":");//�滻���ţ������ͻ
	extAtu.replace(";",".");
	chartType=typeCbo->currentIndex();
	hide();
}
void chartsetting_dialog::onCancel()
{
	extAtu.replace("=",":");//�滻���ţ������ͻ
	extAtu.replace(";",".");
	hide();
}
void chartsetting_dialog::setExtAtu(QString extAtu)
{
	extAtu.replace(":","=");//�滻���ţ������ͻ
	extAtu.replace(".",";");
	this->extAtu=extAtu;
	extatuEdit->setText(extAtu);
}
QString chartsetting_dialog::getExtAtu()
{
	return extAtu;
}
void chartsetting_dialog::setTxt(QString txt)
{
	this->txt=txt;
	txtLine->setText(txt);
}
QString chartsetting_dialog::getTxt()
{
	return txt;
}

void chartsetting_dialog::setType(int type)
{
	chartType=type;
	typeCbo->setCurrentIndex(chartType);
}
int chartsetting_dialog::getType()
{
	return chartType;
}

int chartsetting_dialog::getDs()
{
	return chartDs;
}
void chartsetting_dialog::setDs(int ds)
{
	chartDs=ds;
	curDsLabel->setText(QString("��ǰ���:%1").arg(chartDs));
}