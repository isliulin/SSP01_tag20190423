#include "view_reportshow.h"
#include "sp_qt_chart.h"
#include "ssp_gui.h"
QString static getAtu(QString info,QString name);
void  static drawText(QRectF rect,QString txt,QPainter *painter,QString info);
void  static drawLine(QRectF rect,int type,QPainter *painter,QString info);
const int DEVHEIGHT=20;//18+2px
view_reportshow::view_reportshow(int reportNo,QWidget *parent)
	: CBaseView(parent)
{
	ui.setupUi(this);
	m_pOper=DB;
	condlg=NULL;
	timer=NULL;
	dsNo=m_pOper->SelectIntoI(SString::toFormat("select ds_no from t_ssp_report_info where report_no=%d",reportNo));
	if(dsNo<=0)
		return;
	paintWidget = new PaintWidget(this);  
	paintWidget->setreportNo(reportNo);
	topWidget=new QWidget(this); 
	
	LargeGap=40;//��ҳ��Ĭ�Ͼ���
	this->reportNo=reportNo;
	conButton=new QPushButton(this);
	printViewButton=new QPushButton(this);
	printPDFButton=new QPushButton(this);
	QHBoxLayout *layouttop = new QHBoxLayout();
	layouttop->addWidget(conButton);
	conButton->setText("��ѯ(&Q)");
	QIcon icon;
	icon.addFile(QString::fromUtf8(":/ssp_gui/Resources/search.gif"), QSize(), QIcon::Normal, QIcon::Off);
	conButton->setIcon(icon);

	layouttop->addWidget(printViewButton);
	printViewButton->setText("��ӡ(&P)");
	QIcon icon1;
	icon1.addFile(QString::fromUtf8(":/ssp_gui/Resources/print.gif"), QSize(), QIcon::Normal, QIcon::Off);
	printViewButton->setIcon(icon1);

	layouttop->addWidget(printPDFButton);
	printPDFButton->setText("����PDF(&E)");
	QIcon icon2;
	icon2.addFile(QString::fromUtf8(":/ssp_gui/Resources/export.gif"), QSize(), QIcon::Normal, QIcon::Off);
	printPDFButton->setIcon(icon2);

	layouttop->addStretch();
	layouttop->setContentsMargins(0,0,0,0);
	topWidget->setLayout(layouttop);
	QVBoxLayout *layout = new QVBoxLayout();
	//layout->setSpacing(0);
	layout->addWidget(topWidget);
	
	QScrollArea *scrollArea=new QScrollArea();
	scrollArea->setWidget(paintWidget);
	SRecordset reports;
	m_pOper->RetrieveRecordset(SString::toFormat("select report_no,report_name,ds_no,rpt_size from t_ssp_report_info where report_no=%d order by report_no",reportNo),reports);
	SString s03=reports.GetValue(0,3);
	width=SString::GetIdAttributeI(1,s03,",");
	height=SString::GetIdAttributeI(2,s03,",");
	paintWidget->resize(width,height);
	layout->addWidget(scrollArea);	
	//layout->addStretch();
	setLayout(layout);

	dsName=m_pOper->SelectInto(SString::toFormat("select name from t_ssp_data_window where dw_sn=%d",dsNo));
	m_fieldlist = new SPtrList<CSsp_DwColumn>;
	m_dataWindow = GET_DW_CFG;
	getFieldInfo(dsName.data());
	
	dbSet =  m_dataWindow->SearchDataset(dsName);
	paintWidget->setDbSet(dbSet);
	paintWidget->update();

	//ͼ�������Ի���
	SRecordset reportItemSets;
	m_pOper->RetrieveRecordset(SString::toFormat("select item_no,report_no,pos,type,seg,rect,info from t_ssp_report_item where report_no=%d order by seg+0",reportNo),reportItemSets);
	for(int i=0;i<reportItemSets.GetRows();i++)
	{
		int type=reportItemSets.GetValue(i,3).toInt();
		int itemNo=reportItemSets.GetValue(i,0).toInt();
		if(type==8)//ͼ��
		{
			QString info=reportItemSets.GetValue(i,6).data();
			QString txt=getAtu(info,"txt");
			//int type=getAtu(info,"charttype").toInt();
			int ds=getAtu(info,"chartds").toInt();
			SString dsName=m_pOper->SelectInto(SString::toFormat("select name from t_ssp_data_window where dw_sn=%d",ds));
			CSsp_Dataset* dbSet= m_dataWindow->SearchDataset(dsName);
			dbSetMap.insert(itemNo,dbSet);
			//chartconCbo->addItem(reportItemSets.GetValue(i,0).data());
			view_dw_condlg *newcondlg=new view_dw_condlg();
			QString err;
			newcondlg->refreshTableByName(dsName,err);
			condlgMap.insert(itemNo,newcondlg);
		}
	}

	condlg=new view_dw_condlg();
	QString err="";
	if(!condlg->refreshTableByName(dsName,err)){}
	connect(conButton,SIGNAL(clicked()),this,SLOT(onPushButton_con()));
	connect(printViewButton,SIGNAL(clicked()),this,SLOT(onPushButton_printview()));
	connect(printPDFButton,SIGNAL(clicked()),this,SLOT(onPushButton_printPDF()));

	timer = new QTimer(this);
	connect(timer,SIGNAL(timeout()),this,SLOT(timerOut()));
	timer->start(200);
}

view_reportshow::~view_reportshow()
{
	if(condlg!=NULL)
		delete condlg;
	if(timer!=NULL)
		delete timer;
	qDeleteAll(condlgMap);
}
void view_reportshow::timerOut()
{
	int dsNo=m_pOper->SelectIntoI(SString::toFormat("select ds_no from t_ssp_report_info where report_no=%d",reportNo));
	if(dsNo<=0)
		return;
	dsName=m_pOper->SelectInto(SString::toFormat("select name from t_ssp_data_window where dw_sn=%d",dsNo));
	dbSet=m_dataWindow->SearchDataset(dsName);
	//�Ƿ���Ҫͼ��ˢ�£�
// 	SRecordset reportItemSets;
// 	m_pOper->RetrieveRecordset(SString::toFormat("select item_no,report_no,pos,type,seg,rect,info from t_ssp_report_item where report_no=%d order by seg+0",reportNo),reportItemSets);
// 	for(int i=0;i<reportItemSets.GetRows();i++)
// 	{
// 		int type=reportItemSets.GetValue(i,3).toInt();
// 		if(type==8)//ͼ��
// 		{
// 			QString info=reportItemSets.GetValue(i,6).data();
// 			QString txt=getAtu(info,"txt");
// 			int type=getAtu(info,"charttype").toInt();
// 			int ds=getAtu(info,"chartds").toInt();
// 			SString dsName=m_pOper->SelectInto(SString::toFormat("select name from t_ssp_data_window where dw_sn=%d",ds));
// 			CSsp_Dataset* dbSet= m_dataWindow->SearchDataset(dsName);
// 			dbSetMap.value(reportItemSets.GetValue(i,0).toInt())=dbSet;
// 		}
// 	}
	paintWidget->setDbSet(dbSet);
	paintWidget->update();
}
//���ò�ѯ����
void view_reportshow::onPushButton_con()
{
	if(condlg->isConditionEmpty())
		return;
	condlg->exec();
	dbSet=m_dataWindow->SearchDataset(dsName);
	if(dbSet==NULL)
		return;
	//ͼ��
	SRecordset reportItemSets;
	m_pOper->RetrieveRecordset(SString::toFormat("select item_no,report_no,pos,type,seg,rect,info from t_ssp_report_item where report_no=%d order by seg+0",reportNo),reportItemSets);
	QString dsName=m_pOper->SelectInto(SString::toFormat("select name from t_ssp_data_window where dw_sn=%d",dsNo)).data();
	for(int i=0;i<reportItemSets.GetRows();i++)
	{
		int type=reportItemSets.GetValue(i,3).toInt();
		if(type==8)//ͼ��
		{
			QString info=reportItemSets.GetValue(i,6).data();
			QString txt=getAtu(info,"txt");
			//int type=getAtu(info,"charttype").toInt();
			int ds=getAtu(info,"chartds").toInt();
			//SString dsName1=m_pOper->SelectInto(SString::toFormat("select name from t_ssp_data_window where dw_sn=%d",ds));
			SString atu=m_pOper->SelectInto(SString::toFormat("select ext_attr from t_ssp_data_window where dw_sn=%d",ds));
			QString condRef=getAtu(atu.data(),"cond_ref");
			int itemNo=reportItemSets.GetValue(i,0).toInt();
			if(condRef!=dsName)//����������µ����Ի���
			{
				view_dw_condlg *newcondlg=condlgMap.value(itemNo);
				if(newcondlg!=NULL)
				{
					newcondlg->exec();
					CSsp_Dataset* dbSet=dbSetMap.value(itemNo);
					dbSet->SetCondition(*(newcondlg->getListDwCon()));
				}
			}
			else
			{
				SPtrList<stuDwCondition> cpyList;
				cpyList.setAutoDelete(false);
//				condlg->getListDwCon()->copyto(cpyList);
				SPtrList<stuDwCondition> *cpyFromList=condlg->getListDwCon();
				for(int i=0;i<cpyFromList->count();i++)//��������
				{
					stuDwCondition *newNode=new stuDwCondition();
					newNode->iConditionSn=cpyFromList->at(i)->iConditionSn;
					newNode->sCmpValue=cpyFromList->at(i)->sCmpValue;
					cpyList.append(newNode);
				}
				CSsp_Dataset* dbSet=dbSetMap.value(itemNo);
				dbSet->SetCondition(cpyList);
			}
		}
	}

	dbSet->SetCondition(*(condlg->getListDwCon()));
	paintWidget->setDbSet(dbSet);
	paintWidget->update();
	con="��ѯ����:"+condlg->getConStr();
	paintWidget->setCon(con);
}
//nouse
void view_reportshow::onPushButton_chartcon()
{
	//qdialogΪ�������ڲ�����Ƕ
// 	QString cboTxt=chartconCbo->currentText();
// 	if(cboTxt=="")
// 		return;
// 	view_dw_condlg *dlg=condlgMap.value(cboTxt.toInt());
// 	dlg->exec();
// 	CSsp_Dataset* dbset=dbSetMap.value(cboTxt.toInt());
// 	dbset->SetCondition(*(dlg->getListDwCon()));
}
//nouse
void view_reportshow::onPushButton_print()
{
	QPrintDialog printDialog(&printer, this);
	if (printDialog.exec()) 
	{
		printPages(printer);
	}
}
//��ӡ
void view_reportshow::onPushButton_printview()
{
	//QPrinter printer(QPrinter::HighResolution);
	QPrintPreviewDialog preview(&printer,this);
	connect(&preview, SIGNAL(paintRequested(QPrinter *)),this,SLOT(printPages(QPrinter *)));
	printer.setOutputFormat(QPrinter::NativeFormat);
	preview.exec();//��ӡԤ������
}
//���pdf
void view_reportshow::onPushButton_printPDF()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("����PDF�ļ�"), QString(), "*.pdf");
	if (!fileName.isEmpty()) 
	{
		if (QFileInfo(fileName).suffix().isEmpty())
			fileName.append(".pdf");		
		printer.setOutputFormat(QPrinter::PdfFormat);
		printer.setOutputFileName(fileName);
		printer.setPaperSize(width>=800?(QPrinter::A3):(QPrinter::A4));
		printPages(printer);
	}
}
//��ӡpdf,��ͨ��Ԥ������
void view_reportshow::printPDF(QPrinter &printer)
{
	QPainter painter(&printer);
	SRecordset reportItemSets;
	m_pOper->RetrieveRecordset(SString::toFormat("select item_no,report_no,pos,type,seg,rect,info from t_ssp_report_item where report_no=%d order by seg+0",reportNo),reportItemSets);//������seg����
	if(reportItemSets.GetRows()==0)
		return;
	QList<divStruct>divList;
	int divHeight=0;
	for(int i=0;i<reportItemSets.GetRows();i++)
	{
		SString si2=reportItemSets.GetValue(i,2);
		if(reportItemSets.GetValue(i,3).toInt()==5)//�ָ���
		{
			divStruct div1(reportItemSets.GetValue(i,4).data(),reportItemSets.GetValue(i,0).toInt(),SString::GetIdAttributeI(2,si2,",")-divHeight-DEVHEIGHT);
			divHeight=SString::GetIdAttributeI(2,si2,",");
			divList.append(div1);//˳������
		}
	}
	/*SRecordset *record =*/dbSet->GetRecordset();
	//int expCount=record->GetRows();
	QList<QStringList> pages;
	printPage(&painter,divList,reportItemSets);
	painter.end();
}
void view_reportshow::printPages(QPrinter *printer)
{
	printPages(*printer);
}
//��ӡ�������ݻ��Ƶ�printer��
void view_reportshow::printPages(QPrinter &printer)
{
	QPainter painter(&printer);
	SRecordset reportItemSets;
	m_pOper->RetrieveRecordset(SString::toFormat("select item_no,report_no,pos,type,seg,rect,info from t_ssp_report_item where report_no=%d order by seg+0",reportNo),reportItemSets);//������seg����
	if(reportItemSets.GetRows()==0)
		return;
	QList<divStruct>divList;
	int divHeight=0;
	for(int i=0;i<reportItemSets.GetRows();i++)
	{
		SString si2=reportItemSets.GetValue(i,2);
		if(reportItemSets.GetValue(i,3).toInt()==5)//�ָ���
		{
			divStruct div1(reportItemSets.GetValue(i,4).data(),reportItemSets.GetValue(i,0).toInt(),SString::GetIdAttributeI(2,si2,",")-divHeight-DEVHEIGHT);
			divHeight=SString::GetIdAttributeI(2,si2,",");
			divList.append(div1);//˳������
		}
	}
	/*SRecordset *record =*/dbSet->GetRecordset();
	//int expCount=record->GetRows();
	QList<QStringList> pages;
	printPage(&painter,divList,reportItemSets);
}
//��ӡÿҳҳͷ��ҳβ
void view_reportshow::initPage(QPainter *painter,const QList<divStruct> &divList,SRecordset &reportItemSets,int pageNum,int firstHeight)
{
	//firstHeight+=DEVHEIGHT;
	if(pageNum==1)
		firstHeight=0;
	for(int i1=0;i1<divList.size();i1++)
	{
		divStruct div1=divList.at(i1);
		if(div1.no=="0"&&pageNum==1)
		{
			for(int i=0;i<reportItemSets.GetRows();i++)
			{
				int segNum=reportItemSets.GetValue(i,4).toInt();
				int type=reportItemSets.GetValue(i,3).toInt();
				if(segNum==0)
				{
					SString si2=reportItemSets.GetValue(i,2);
					SString si5=reportItemSets.GetValue(i,5);
					if(type!=5)
					{
						QRectF rect(SString::GetIdAttributeI(1,si2,",")+SString::GetIdAttributeI(1,si5,","),
							SString::GetIdAttributeI(2,si2,",")+SString::GetIdAttributeI(2,si5,",")-0*DEVHEIGHT,
							SString::GetIdAttributeI(3,si5,","),SString::GetIdAttributeI(4,si5,","));
						if(type==2||type==3||type==7)//�߶�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							drawLine(rect,type,painter,info);
						}
						else if(type==4)//�ı�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							drawText(rect,txt,painter,info);
						}
						else if(type==1)//����
						{
							painter->drawRect(rect);
						}
						else if(type==6)//���ʽ��ֻ������
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString fun=getAtu(info,"fun");
							if(fun!="")
							{
								if(fun=="no")//��ţ�ȡҳ��
									drawText(rect,QString::number(pageNum),painter,info);
								else if(fun=="sum")//��ͣ���Ҫ�к�
								{
									QString exp=getAtu(info,"exp");
									QString txt=getAtu(info,"txt");
									int k=exp.toInt();
									if(k>=dbSet->GetColumnCount())
										break;
									SRecordset *record =dbSet->GetRecordset();
									int iRow=record->GetRows();
									int sum=0;
									for(int i=0;i<iRow;i++)
										sum+=record->GetValue(i,k).toInt();
									drawText(rect,txt+QString::number(sum),painter,info);
								}
								else if(fun=="time")//ʱ��
								{
									drawText(rect,"��ǰʱ��:"+QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"),painter,info);
								}
								else if(fun=="selcon")//��ʾ��ѯ����
								{
									if(con=="")
										con="��ѯ����:��";
									drawText(rect,con,painter,info);
								}
								else if(fun=="count")//��ʾ����
								{
									int rowCount=dbSet->GetRecordset()->GetRows();
									drawText(rect,"������:"+QString::number(rowCount),painter,info);
								}
								else if(fun=="author")//��ʾ����
								{
									QString author=SSP_GUI->GetLoginUserName().data();
									drawText(rect,QString("��ǰ����:")+author,painter,info);
								}
							}//fun
						}//type6
						else if(type==8)//ͼ��
						{
							int chartNo=reportItemSets.GetValue(i,0).toInt();
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							int type=getAtu(info,"charttype").toInt();
							QString extAtu=getAtu(info,"extatu");
							extAtu.replace(":","=");//�滻���ţ������ͻ
							extAtu.replace(".",";");
							int pointRow=getAtu(extAtu,"pointrow").toInt();
							int pointCol=getAtu(extAtu,"pointcol").toInt();
							//int ds=getAtu(info,"chartds").toInt();
							//SString dsName=m_pOper->SelectInto(SString::toFormat("select name from t_ssp_data_window where dw_sn=%d",ds));
							//CSsp_Dataset* dbSet= m_dataWindow->SearchDataset(dsName);
							CSsp_Dataset* dbSet=dbSetMap.value(chartNo);
							if(dbSet==NULL)
								continue;
							SRect srect;
							srect.left=rect.x();
							srect.right=rect.x()+rect.width();
							srect.top=rect.y();
							srect.bottom=rect.y()+rect.height();
							if(type==0)//��ͼ
								CSpQtChartPainter::DrawChartPie(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==1)//��ͼ
								CSpQtChartPainter::DrawChartColumn(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==2)//����ͼ
								CSpQtChartPainter::DrawChartLine(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==3)//�Ǳ���
								CSpQtChartPainter::DrawPanel(painter,srect,dbSet->GetRecordset()->GetValue(pointRow,pointCol).toInt(),extAtu.toStdString().data());
						}//type8
					}
				}
			}
		}
		else if(div1.no=="1")
		{
			for(int i=0;i<reportItemSets.GetRows();i++)
			{
				int segNum=reportItemSets.GetValue(i,4).toInt();
				int type=reportItemSets.GetValue(i,3).toInt();
				if(segNum==1)
				{
					SString si2=reportItemSets.GetValue(i,2);
					SString si5=reportItemSets.GetValue(i,5);
					if(type!=5)
					{
						QRectF rect(SString::GetIdAttributeI(1,si2,",")+SString::GetIdAttributeI(1,si5,","),
							SString::GetIdAttributeI(2,si2,",")+SString::GetIdAttributeI(2,si5,",")-firstHeight-1*DEVHEIGHT,
							SString::GetIdAttributeI(3,si5,","),SString::GetIdAttributeI(4,si5,","));
						if(type==2||type==3||type==7)//�߶�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							drawLine(rect,type,painter,info);
						}
						else if(type==4)//�ı�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							drawText(rect,txt,painter,info);
						}
						else if(type==1)//����
						{
							painter->drawRect(rect);
						}
						else if(type==6)//���ʽ��ֻ������
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString fun=getAtu(info,"fun");
							if(fun!="")
							{
								if(fun=="no")//��ţ�ȡҳ��
									drawText(rect,QString::number(pageNum),painter,info);
								else if(fun=="sum")//��ͣ���Ҫ�к�
								{
									QString exp=getAtu(info,"exp");
									QString txt=getAtu(info,"txt");
									int k=exp.toInt();
									if(k>=dbSet->GetColumnCount())
										break;
									SRecordset *record =dbSet->GetRecordset();
									int iRow=record->GetRows();
									int sum=0;
									for(int i=0;i<iRow;i++)
										sum+=record->GetValue(i,k).toInt();
									drawText(rect,txt+QString::number(sum),painter,info);
								}
								else if(fun=="time")//ʱ��
								{
									drawText(rect,QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"),painter,info);
								}
								else if(fun=="selcon")//��ʾ��ѯ����
								{
									if(con=="")
										con="��ѯ����:��";
									drawText(rect,con,painter,info);
								}
								else if(fun=="count")//��ʾ����
								{
									int rowCount=dbSet->GetRecordset()->GetRows();
									drawText(rect,"������:"+QString::number(rowCount),painter,info);
								}
								else if(fun=="author")//��ʾ����
								{
									QString author=SSP_GUI->GetLoginUserName().data();
									drawText(rect,QString("��ǰ����:")+author,painter,info);
								}
							}//fun
						}//type6
						else if(type==8)//ͼ��
						{
							int chartNo=reportItemSets.GetValue(i,0).toInt();
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							int type=getAtu(info,"charttype").toInt();

							QString extAtu=getAtu(info,"extatu");
							extAtu.replace(":","=");//�滻���ţ������ͻ
							extAtu.replace(".",";");
							int pointRow=getAtu(extAtu,"pointrow").toInt();
							int pointCol=getAtu(extAtu,"pointcol").toInt();

							CSsp_Dataset* dbSet=dbSetMap.value(chartNo);
							if(dbSet==NULL)
								continue;
							SRect srect;
							srect.left=rect.x();
							srect.right=rect.x()+rect.width();
							srect.top=rect.y();
							srect.bottom=rect.y()+rect.height();
							if(type==0)//��ͼ
								CSpQtChartPainter::DrawChartPie(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==1)//��ͼ
								CSpQtChartPainter::DrawChartColumn(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==2)//����ͼ
								CSpQtChartPainter::DrawChartLine(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==3)//�Ǳ���
								CSpQtChartPainter::DrawPanel(painter,srect,dbSet->GetRecordset()->GetValue(pointRow,pointCol).toInt(),extAtu.toStdString().data());
						}//type8
					}
				}
			}
		}
		else if(div1.no=="4")
		{
			for(int i=0;i<reportItemSets.GetRows();i++)
			{
				int segNum=reportItemSets.GetValue(i,4).toInt();
				int type=reportItemSets.GetValue(i,3).toInt();
				if(segNum==4)
				{
					SString si2=reportItemSets.GetValue(i,2);
					SString si5=reportItemSets.GetValue(i,5);
					if(type!=5)
					{
						int bTop=painter->window().height()-LargeGap-height+(SString::GetIdAttributeI(2,si5,",")
							+SString::GetIdAttributeI(2,si2,","));

						QRectF rect(SString::GetIdAttributeI(1,si2,",")+SString::GetIdAttributeI(1,si5,","),bTop,
							SString::GetIdAttributeI(3,si5,","),SString::GetIdAttributeI(4,si5,","));
						if(type==2||type==3||type==7)//�߶�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							drawLine(rect,type,painter,info);
						}
						else if(type==4)//�ı�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							drawText(rect,txt,painter,info);
						}
						else if(type==1)//����
						{
							painter->drawRect(rect);
						}
						else if(type==6)//���ʽ��ֻ������
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString fun=getAtu(info,"fun");
							if(fun!="")
							{
								if(fun=="no")//��ţ�Ϊ1
								{
									drawText(rect,QString::number(1),painter,info);
								}
								else if(fun=="sum")//��ͣ���Ҫ�к�
								{
									QString exp=getAtu(info,"exp");
									QString txt=getAtu(info,"txt");
									int k=exp.toInt();
									if(k>=dbSet->GetColumnCount())
										break;
									SRecordset *record =dbSet->GetRecordset();
									int iRow=record->GetRows();
									int sum=0;
									for(int i=0;i<iRow;i++)
										sum+=record->GetValue(i,k).toInt();
									drawText(rect,txt+QString::number(sum),painter,info);
								}
								else if(fun=="time")//ʱ��
								{
									drawText(rect,QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"),painter,info);
								}
								else if(fun=="selcon")//��ʾ��ѯ����
								{
									if(con=="")
										con="��ѯ����:��";
									drawText(rect,con,painter,info);
								}
								else if(fun=="count")//��ʾ����
								{
									int rowCount=dbSet->GetRecordset()->GetRows();
									drawText(rect,"������:"+QString::number(rowCount),painter,info);
								}
								else if(fun=="author")//��ʾ����
								{
									QString author=SSP_GUI->GetLoginUserName().data();
									drawText(rect,QString("��ǰ����:")+author,painter,info);
								}
							}
						}//type6
						else if(type==8)//ͼ��
						{
							int chartNo=reportItemSets.GetValue(i,0).toInt();
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							int type=getAtu(info,"charttype").toInt();
							CSsp_Dataset* dbSet=dbSetMap.value(chartNo);

							QString extAtu=getAtu(info,"extatu");
							extAtu.replace(":","=");//�滻���ţ������ͻ
							extAtu.replace(".",";");
							int pointRow=getAtu(extAtu,"pointrow").toInt();
							int pointCol=getAtu(extAtu,"pointcol").toInt();

							if(dbSet==NULL)
								continue;
							SRect srect;
							srect.left=rect.x();
							srect.right=rect.x()+rect.width();
							srect.top=rect.y();
							srect.bottom=rect.y()+rect.height();
							if(type==0)//��ͼ  
								CSpQtChartPainter::DrawChartPie(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==1)//��ͼ
								CSpQtChartPainter::DrawChartColumn(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==2)//����ͼ
								CSpQtChartPainter::DrawChartLine(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==3)//�Ǳ���
								CSpQtChartPainter::DrawPanel(painter,srect,dbSet->GetRecordset()->GetValue(pointRow,pointCol).toInt(),extAtu.toStdString().data());
						}//type8
					}//!type5
				}
			}
		}
	}//for div
}

QColor GetColorByAttr(SString sExtAttr,SString sAttrName)
{
	SString sColor = SString::GetAttributeValue(sExtAttr,sAttrName);
	int r = SString::GetIdAttributeI(1,sColor,",");
	int g = SString::GetIdAttributeI(2,sColor,",");
	int b = SString::GetIdAttributeI(3,sColor,",");
	return QColor(r,g,b);
}

void view_reportshow::printPage(QPainter *painter,const QList<divStruct> &divList,SRecordset &reportItemSets)
{	
	QFont font;
	font.setPointSize(8);
	painter->setFont(font);
	painter->save();
	int pageNumber=1;
	painter->drawText(painter->window(),Qt::AlignHCenter|Qt::AlignBottom,QString::number(pageNumber));
	painter->translate(0,LargeGap);
	int pageHeight=painter->window().height()-2*LargeGap;
	int height1=0;

	QTextOption o;
	o.setAlignment(Qt::AlignCenter);
	int y=0;//ƫ��
	//int y1=0;
	int divH=0;
	int pageTopH=0;
	int pageEndH=0;
	int topPageH=0;
	for(int i1=0;i1<divList.size();i1++)
	{
		divStruct div1=divList.at(i1);
		if(div1.no=="0")//��ҳ�߶�
		{
			topPageH=div1.height;
		}
		if(div1.no=="1")//ҳͷ�߶�
		{
			pageTopH=div1.height;
		}
		if(div1.no=="4")//ҳβ�߶�
		{
			pageEndH=div1.height;
		}
	}
	pageHeight=pageHeight-pageTopH-pageEndH-topPageH;//��ȥҳͷҳβ��ͷ�߶ȣ����ڴ˻��ƣ�
	initPage(painter,divList,reportItemSets,1,topPageH);//���Ƶ�һҳ��ҳͷҳβ��ͷ
	painter->translate(0,pageTopH+LargeGap+topPageH-DEVHEIGHT*2);//��λ��ҳͷ��
	height1=pageTopH+topPageH;
	for(int i1=0;i1<divList.size();i1++)
	{
		divStruct div1=divList.at(i1);
		if(div1.no=="2")
		{
			QString grp;
			QString grpInfo;
			QList<QString>grpList;
			bool bAutoTrans = dbSet->GetAutoColumnTransform();
			for(int j=0;j<reportItemSets.GetRows();j++)
			{
				if(reportItemSets.GetValue(j,3).toInt()==5)//�ָ���
				{
					if(reportItemSets.GetValue(j,4).toInt()==100)//��ʱֻ����һ�ط���
					{
						SString info=reportItemSets.GetValue(j,6);
						grpInfo=info;
						grp=getAtu(info.data(),"grp");
					}
				}
			}
			SRecordset *record =dbSet->GetRecordset();
			int col=0;
			for(col=0;col<record->GetColumns();col++)
			{
				SString colName=record->GetColumnName(col);
				if(colName==record->GetColumnName(grp.toInt()))
					break;
			}
			int iRow = record->GetRows();
			for(int row=0;row<iRow;row++)
			{
				if(grp=="")//�����ڷ���ʱ
				{
					for(int j=0;j<reportItemSets.GetRows();j++)
					{
						int segNum=reportItemSets.GetValue(j,4).toInt();
						int type=reportItemSets.GetValue(j,3).toInt();
						if(segNum==2)
						{
							if(type!=5)
							{
								SString sj2=reportItemSets.GetValue(j,2);
								SString sj5=reportItemSets.GetValue(j,5);
								if(SString::GetIdAttributeI(2,sj2,",")+SString::GetIdAttributeI(2,sj5,",")+y
									-height1+SString::GetIdAttributeI(4,sj5,",")>pageHeight)//�ܸ߶ȼ�ȥǰ��ҳ�߶Ⱥ�
								{
									pageNumber++;
									if(pageNumber==2)
									{
										pageHeight=pageHeight+topPageH;
										height1=pageTopH;
									}
									painter->restore();
									printer.newPage();//��ҳ
									painter->drawText(painter->window(),Qt::AlignHCenter|Qt::AlignBottom,QString::number(pageNumber));
									painter->save();
									painter->translate(0,LargeGap);
									initPage(painter,divList,reportItemSets,pageNumber,topPageH);
									painter->translate(0,pageTopH+LargeGap+DEVHEIGHT);//��λ��ҳͷ��
									height1=SString::GetIdAttributeI(2,sj2,",")+SString::GetIdAttributeI(2,sj5,",")+y;
								}
								QRectF rect(SString::GetIdAttributeI(1,sj2,",")
									+SString::GetIdAttributeI(1,sj5,","),SString::GetIdAttributeI(2,sj2,",")
									+SString::GetIdAttributeI(2,sj5,",")+y-height1-2*DEVHEIGHT,
									SString::GetIdAttributeI(3,sj5,","),SString::GetIdAttributeI(4,sj5,","));
								if(type==2||type==3||type==7)//�߶�
								{
									QString info=reportItemSets.GetValue(j,6).data();
									drawLine(rect,type,painter,info);
								}
								else if(type==4)//�ı�
								{
									QString info=reportItemSets.GetValue(j,6).data();
									QString txt=getAtu(info,"txt");
									drawText(rect,txt,painter,info);
								}
								else if(type==6)//���ʽ
								{
									QString info=reportItemSets.GetValue(j,6).data();
									QString fun=getAtu(info,"fun");
									if(fun=="")//�Ǻ���
									{
										QString txt;
										QString exp=getAtu(info,"exp");
										int k=exp.toInt();
										if(k>=dbSet->GetColumnCount())
											break;									
										if(grp!=""&&k==grp.toInt())
											continue;
										if (dbSet->GetColumn(k)->m_pRef==NULL)
										{
											if(!bAutoTrans && dbSet->GetColumn(k)->m_sConv!="")
												txt=dbSet->GetColumn(k)->GetConvDb2Sys(record->GetValue(row,k)).data();
											else
												txt=record->GetValue(row,k).data();
											drawText(rect,txt,painter,info);
										}
										else
										{
											SRecordset *pRef  = dbSet->GetColumn(k)->m_pRef->GetRecordset();
											//int iIndex = 0;
											for (int k1 = 0;k1<pRef->GetRows();k1++)
											{
												if (record->GetValue(row,k)==pRef->GetValue(k1,0))
												{
													txt=tr(pRef->GetValue(k1,1).data());
													drawText(rect,txt,painter,info);
													break;
												}
												else if(k1==pRef->GetRows()-1)//û�ҵ���Ӧ��ȡĬ��ֵ
												{
													//txt=(tr(pRef->GetValue(0,1).data()));
													txt="";
													drawText(rect,txt,painter,info);
												}
											}
										}
									}
									else//����
									{
										if(fun=="no")//��ţ�ȡ�к�
											drawText(rect,QString::number(row+1),painter,info);
										else if(fun=="sum")//��ͣ���Ҫ�к�
										{
											QString exp=getAtu(info,"exp");
											QString txt=getAtu(info,"txt");
											int k=exp.toInt();
											if(k>=dbSet->GetColumnCount())
												break;
											SRecordset *record =dbSet->GetRecordset();
											int iRow=record->GetRows();
											int sum=0;
											for(int i=0;i<iRow;i++)
												sum+=record->GetValue(i,k).toInt();
											drawText(rect,txt+QString::number(sum),painter,info);
										}
										else if(fun=="time")//ʱ��
										{
											drawText(rect,"��ǰʱ��:"+QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"),painter,info);
										}
										else if(fun=="selcon")//��ʾ��ѯ����
										{
											if(con=="")
												con="��ѯ����:��";
											drawText(rect,con,painter,info);
										}
										else if(fun=="count")//��ʾ����
										{
											//int rowCount=dbSet->GetRecordset()->GetRows();
											//drawText(rect,"������:"+QString::number(rowCount),painter,info);
										}
										else if(fun=="author")//��ʾ����
										{
											QString author=SSP_GUI->GetLoginUserName().data();
											drawText(rect,QString("��ǰ����:")+author,painter,info);
										}
									}
								}
								else if(type==1)//����
								{
									painter->drawRect(rect);
								}
								else if(type==8)//ͼ��
								{
									int chartNo=reportItemSets.GetValue(j,0).toInt();
									QString info=reportItemSets.GetValue(j,6).data();
									QString txt=getAtu(info,"txt");
									int type=getAtu(info,"charttype").toInt();

									QString extAtu=getAtu(info,"extatu");
									extAtu.replace(":","=");//�滻���ţ������ͻ
									extAtu.replace(".",";");
									//int pointRow=getAtu(extAtu,"pointrow").toInt();
									int pointCol=getAtu(extAtu,"pointcol").toInt();

									CSsp_Dataset* dbSet=dbSetMap.value(chartNo);
									if(dbSet==NULL)
										continue;
									SRect srect;
									srect.left=rect.x();
									srect.right=rect.x()+rect.width();
									srect.top=rect.y();
									srect.bottom=rect.y()+rect.height();
									if(type==0)//��ͼ
										CSpQtChartPainter::DrawChartPie(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
									else if(type==1)//��ͼ
										CSpQtChartPainter::DrawChartColumn(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
									else if(type==2)//����ͼ
										CSpQtChartPainter::DrawChartLine(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
									else if(type==3)//�Ǳ���
										CSpQtChartPainter::DrawPanel(painter,srect,dbSet->GetRecordset()->GetValue(row,pointCol).toInt(),extAtu.toStdString().data());
								}//type8
							}
						}//segNum==2
					}
					if(row!=iRow-1)//���һ�β��Ӿ���
						y=y+div1.height;
				}
				else//���ڷ���
				{
					SString grpValue=record->GetValue(row,col);
					QString grpValueStr=grpValue.data();						
					if(grpList.contains(grpValueStr))
						continue;
					grpList.append(grpValueStr);
					QString transName;
					if (dbSet->GetColumn(col)->m_pRef==NULL)//ȡ��ת�����ı�
					{
						if(!bAutoTrans && dbSet->GetColumn(col)->m_sConv!="")
							transName=dbSet->GetColumn(col)->GetConvDb2Sys(record->GetValue(row,col)).data();
						else
							transName=record->GetValue(row,col).data();
					}
					else
					{
						SRecordset *pRef  = dbSet->GetColumn(col)->m_pRef->GetRecordset();
						//int iIndex = 0;
						for (int k1 = 0;k1<pRef->GetRows();k1++)
						{
							if (record->GetValue(row,col)==pRef->GetValue(k1,0))
							{
								transName=tr(pRef->GetValue(k1,1).data());
								break;
							}
							else if(k1==pRef->GetRows()-1)//û�ҵ���Ӧ��ȡĬ��ֵ
							{
								transName="";
							}
						}
					}
// 					int fontSize=painter->font().pointSize();//���ʶ
// 					painter->fillRect(0,y-fontSize+pageTopH-height1,painter->window().width(),fontSize,Qt::yellow);
// 					painter->drawText(QPointF(50,y+pageTopH-height1),"����:"+transName);

					int grpFontSize=getAtu(grpInfo,"fontsize").toInt();
					QString txt;
					QStringList tmpTxtLst=getAtu(grpInfo,"txt").split("@");
					txt=tmpTxtLst.at(0)+transName;
					if(tmpTxtLst.size()>1)
						txt=txt+tmpTxtLst.at(1);
// 					if(y>0)
// 						y+=div1.height;
					drawText(QRect(10,y+pageTopH-height1+topPageH+DEVHEIGHT,painter->window().width()-20,2*grpFontSize),txt,painter,grpInfo);
					y=2*grpFontSize+y;

					for(int row1=0;row1<iRow;row1++)
					{
						if(grpValue==record->GetValue(row1,col))
						{						
							//y+=20;
							for(int j=0;j<reportItemSets.GetRows();j++)
							{
								int segNum=reportItemSets.GetValue(j,4).toInt();
								int type=reportItemSets.GetValue(j,3).toInt();
								if(segNum==2)
								{
									if(type!=5)
									{
										SString sj2=reportItemSets.GetValue(j,2);
										SString sj5=reportItemSets.GetValue(j,5);
										if(SString::GetIdAttributeI(2,sj2,",")+SString::GetIdAttributeI(2,sj5,",")+y
											-height1+SString::GetIdAttributeI(4,sj5,",")>pageHeight)//�ܸ߶ȼ�ȥǰ��ҳ�߶Ⱥ�
										{
											pageNumber++;
											if(pageNumber==2)
											{
												pageHeight=pageHeight+topPageH;
												height1=pageTopH;
											}
											painter->restore();
											painter->drawText(painter->window(),Qt::AlignHCenter | Qt::AlignBottom,QString::number(pageNumber));
											printer.newPage();//��ҳ
											painter->save();
											painter->translate(0, LargeGap);
											initPage(painter,divList,reportItemSets,pageNumber,topPageH);
											painter->translate(0,pageTopH+LargeGap+DEVHEIGHT);//��λ��ҳͷ�£���ȥ�ָ����߶�
											height1=SString::GetIdAttributeI(2,sj2,",")+SString::GetIdAttributeI(2,sj5,",")+y;
										}
										QRectF rect(SString::GetIdAttributeI(1,sj2,",")
											+SString::GetIdAttributeI(1,sj5,","),SString::GetIdAttributeI(2,sj2,",")
											+SString::GetIdAttributeI(2,sj5,",")+y-height1-2*DEVHEIGHT,
											SString::GetIdAttributeI(3,sj5,","),SString::GetIdAttributeI(4,sj5,","));
										if(type==2||type==3||type==7)//�߶�
										{
											QString info=reportItemSets.GetValue(j,6).data();
											drawLine(rect,type,painter,info);
										}
										else if(type==4)//�ı�
										{
											QString info=reportItemSets.GetValue(j,6).data();
											QString txt=getAtu(info,"txt");
											drawText(rect,txt,painter,info);
										}
										else if(type==6)// ���ʽ
										{
											QString info=reportItemSets.GetValue(j,6).data();
											QString fun=getAtu(info,"fun");
											if(fun=="")//�Ǻ���
											{
												QString txt;
												QString exp=getAtu(info,"exp");
												int k=exp.toInt();
												if(k>=dbSet->GetColumnCount())
													break;											
// 												if(grp!=""&&k==grp.toInt())
// 													continue;
												if (dbSet->GetColumn(k)->m_pRef==NULL)
												{
													if(!bAutoTrans && dbSet->GetColumn(k)->m_sConv!="")
														txt=dbSet->GetColumn(k)->GetConvDb2Sys(record->GetValue(row1,k)).data();
													else
														txt=record->GetValue(row1,k).data();
													drawText(rect,txt,painter,info);
												}
												else
												{
													SRecordset *pRef  = dbSet->GetColumn(k)->m_pRef->GetRecordset();
													//int iIndex = 0;
													for (int k1 = 0;k1<pRef->GetRows();k1++)
													{
														if (record->GetValue(row1,k)==pRef->GetValue(k1,0))
														{
															txt=tr(pRef->GetValue(k1,1).data());
															drawText(rect,txt,painter,info);
															break;
														}
														else if(k1==pRef->GetRows()-1)//û�ҵ���Ӧ��ȡĬ��ֵ
														{
															//txt=(tr(pRef->GetValue(0,1).data()));
															txt="";
															drawText(rect,txt,painter,info);
														}
													}
												}
											}
											else//����
											{
												if(fun=="no")//���
													drawText(rect,QString::number(row1+1),painter,info);
												else if(fun=="sum")//���
												{
													QString exp=getAtu(info,"exp");
													QString txt=getAtu(info,"txt");
													int k=exp.toInt();
													if(k>=dbSet->GetColumnCount())
														break;
													SRecordset *record =dbSet->GetRecordset();
													int iRow=record->GetRows();
													int sum=0;
													for(int i=0;i<iRow;i++)
														sum+=record->GetValue(i,k).toInt();
													drawText(rect,txt+QString::number(sum),painter,info);
												}
												else if(fun=="time")//ʱ��
												{
													drawText(rect,"��ǰʱ��:"+QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"),painter,info);
												}
												else if(fun=="selcon")//��ʾ��ѯ����
												{
													if(con=="")
														con="��ѯ����:��";
													drawText(rect,con,painter,info);
												}
												else if(fun=="count")//��ʾ����
												{
													//int rowCount=dbSet->GetRecordset()->GetRows();
													//drawText(rect,"������:"+QString::number(rowCount),painter,info);
												}
												else if(fun=="author")//��ʾ����
												{
													QString author=SSP_GUI->GetLoginUserName().data();
													drawText(rect,QString("��ǰ����:")+author,painter,info);
												}
											}
										}
										else if(type==1)//����
										{
											painter->drawRect(rect);
										}
										else if(type==8)//ͼ��
										{
											int chartNo=reportItemSets.GetValue(j,0).toInt();
											QString info=reportItemSets.GetValue(j,6).data();
											QString txt=getAtu(info,"txt");
											int type=getAtu(info,"charttype").toInt();

											QString extAtu=getAtu(info,"extatu");
											extAtu.replace(":","=");//�滻���ţ������ͻ
											extAtu.replace(".",";");
											//int pointRow=getAtu(extAtu,"pointrow").toInt();
											int pointCol=getAtu(extAtu,"pointcol").toInt();

											CSsp_Dataset* dbSet=dbSetMap.value(chartNo);
											if(dbSet==NULL)
												continue;
											SRect srect;
											srect.left=rect.x();
											srect.right=rect.x()+rect.width();
											srect.top=rect.y();
											srect.bottom=rect.y()+rect.height();

											if(type==0)//��ͼ
												CSpQtChartPainter::DrawChartPie(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
											else if(type==1)//��ͼ
												CSpQtChartPainter::DrawChartColumn(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
											else if(type==2)//����ͼ
												CSpQtChartPainter::DrawChartLine(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
											else if(type==3)//�Ǳ���
												CSpQtChartPainter::DrawPanel(painter,srect,dbSet->GetRecordset()->GetValue(row,pointCol).toInt(),extAtu.toStdString().data());
										}//type8
									}

								}//segNum==2
							}
							if(row1!=iRow-1)//���һ�β��Ӿ���
								y=y+div1.height;//�߶��ۼ�
						}//if(grpValue==record->GetValue(row1,col))
					}//for row1
				}
			}//for row
		}
		else if(div1.no=="3")
		{
			for(int i=0;i<reportItemSets.GetRows();i++)
			{
				int segNum=reportItemSets.GetValue(i,4).toInt();
				int type=reportItemSets.GetValue(i,3).toInt();
				if(segNum==3)
				{
					if(type!=5)
					{
						SString si2=reportItemSets.GetValue(i,2);
						SString si5=reportItemSets.GetValue(i,5);
						if(SString::GetIdAttributeI(2,si2,",")+SString::GetIdAttributeI(2,si5,",")+y
							-height1+SString::GetIdAttributeI(4,si5,",")>pageHeight)//��ҳ
						{
							pageNumber++;
							if(pageNumber==2)
							{
								pageHeight=pageHeight+topPageH;
								height1=pageTopH;
							}
							painter->restore();
							painter->drawText(painter->window(),Qt::AlignHCenter | Qt::AlignBottom,QString::number(pageNumber));
							printer.newPage();
							painter->save();
							painter->translate(0, LargeGap);
							initPage(painter,divList,reportItemSets,pageNumber,topPageH);
							painter->translate(0,pageTopH+LargeGap+DEVHEIGHT);
							height1=SString::GetIdAttributeI(2,si2,",")+SString::GetIdAttributeI(2,si5,",")+y;
						}

						QRectF rect(SString::GetIdAttributeI(1,si2,",")+SString::GetIdAttributeI(1,si5,","),
							SString::GetIdAttributeI(2,si2,",")+SString::GetIdAttributeI(2,si5,",")+y-height1-3*DEVHEIGHT,
							SString::GetIdAttributeI(3,si5,","),SString::GetIdAttributeI(4,si5,","));
						if(type==2||type==3||type==7)//�߶�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							drawLine(rect,type,painter,info);
						}
						else if(type==4)//�ı�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							drawText(rect,txt,painter,info);
						}
						else if(type==1)//����
						{
							painter->drawRect(rect);
						}
						else if(type==6)//���ʽ��ֻ������
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString fun=getAtu(info,"fun");
							if(fun!="")
							{
								if(fun=="no")
								{
									int rowCount=dbSet->GetRecordset()->GetRows();
									drawText(rect,QString::number(rowCount),painter,info);
								}
								else if(fun=="sum")
								{
									QString exp=getAtu(info,"exp");
									QString txt=getAtu(info,"txt");
									int k=exp.toInt();
									if(k>=dbSet->GetColumnCount())
										break;
									SRecordset *record =dbSet->GetRecordset();
									int iRow=record->GetRows();
									int sum=0;
									for(int i=0;i<iRow;i++)
										sum+=record->GetValue(i,k).toInt();
									drawText(rect,txt+QString::number(sum),painter,info);
								}
								else if(fun=="time")//ʱ��
								{
									drawText(rect,"��ǰʱ��:"+QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"),painter,info);
								}
								else if(fun=="selcon")//��ʾ��ѯ����
								{
									if(con=="")
										con="��ѯ����:��";
									drawText(rect,con,painter,info);
								}
								else if(fun=="count")//��ʾ����
								{
									int rowCount=dbSet->GetRecordset()->GetRows();
									drawText(rect,"������:"+QString::number(rowCount),painter,info);
								}
								else if(fun=="author")//��ʾ����
								{
									QString author=SSP_GUI->GetLoginUserName().data();
									drawText(rect,QString("��ǰ����:")+author,painter,info);
								}
							}
						}//type6
						else if(type==8)//ͼ��
						{
							int chartNo=reportItemSets.GetValue(i,0).toInt();
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							int type=getAtu(info,"charttype").toInt();

							QString extAtu=getAtu(info,"extatu");
							extAtu.replace(":","=");//�滻���ţ������ͻ
							extAtu.replace(".",";");
							int pointRow=getAtu(extAtu,"pointrow").toInt();
							int pointCol=getAtu(extAtu,"pointcol").toInt();

							CSsp_Dataset* dbSet=dbSetMap.value(chartNo);
							if(dbSet==NULL)
								continue;
							SRect srect;
							srect.left=rect.x();
							srect.right=rect.x()+rect.width();
							srect.top=rect.y();
							srect.bottom=rect.y()+rect.height();

							if(type==0)//��ͼ
								CSpQtChartPainter::DrawChartPie(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==1)//��ͼ
								CSpQtChartPainter::DrawChartColumn(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==2)//����ͼ
								CSpQtChartPainter::DrawChartLine(painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==3)//�Ǳ���
								CSpQtChartPainter::DrawPanel(painter,srect,dbSet->GetRecordset()->GetValue(pointRow,pointCol).toInt(),extAtu.toStdString().data());
						}//type8
					}
				}
			}
		}
		divH+=divList.at(i1).height;//�ۼӾ��Ը߶�
	}//for div
	painter->restore();
	//painter->drawText(painter->window(),Qt::AlignHCenter | Qt::AlignBottom,QString::number(pageNumber+1));
}
//��ȡͼ���õ����ݼ�map
QMap<int,CSsp_Dataset*> view_reportshow::getdbSetMap()
{
	return dbSetMap;
}
SPtrList<CSsp_DwColumn> *view_reportshow::getFieldInfo(SString sWname,SPtrList<CSsp_DwColumn> *cFinfo)
{
	CSsp_Dataset* dbSet=NULL;
	dbSet =  m_dataWindow->SearchDataset(sWname);
	if (dbSet==NULL)
	{
		return m_fieldlist;
	}
	int iCount = dbSet->GetColumnCount();
	for(int i = 0;i<iCount;i++)
	{
		m_fieldlist->append(dbSet->GetColumn(i));
		if (cFinfo!=NULL)
		{
			cFinfo->append(dbSet->GetColumn(i));
		}
	}
	return m_fieldlist;
}
//����info���Ի����ı�
void drawText(QRectF rect,QString txt,QPainter *painter,QString info)
{
	QString fontSize=getAtu(info,"fontsize");
	QString fontFamily=getAtu(info,"fontfamily");
	QString fontType=getAtu(info,"fonttype");
	QFont oldFont=painter->font();
	QPen oldPen=painter->pen();
	QTextOption o;
	QFont newfont;
	newfont.setPointSize(fontSize.toInt());
	newfont.setFamily(fontFamily);
	if(fontType=="0")
	{
		newfont.setBold(false);
		newfont.setItalic(false);
	}
	else if(fontType=="1")
	{
		newfont.setBold(true);
		newfont.setItalic(false);
	}
	else if(fontType=="2")
	{
		newfont.setBold(false);
		newfont.setItalic(true);
	}
	else if(fontType=="3")
	{
		newfont.setBold(true);
		newfont.setItalic(true);
	}
	QString delLine=getAtu(info,"delline");
	QString undLine=getAtu(info,"undline");
	if(delLine=="true")
		newfont.setStrikeOut(true);
	else
		newfont.setStrikeOut(false);
	if(undLine=="true")
		newfont.setUnderline(true);
	else
		newfont.setUnderline(false);
	painter->setFont(newfont);
	QPen newPen;
	QString colorStr=getAtu(info,"color");
	QStringList lstColorStr=colorStr.split(",");
	if(lstColorStr.size()>=3)
	{
		QColor color(lstColorStr.at(0).toInt(),lstColorStr.at(1).toInt(),lstColorStr.at(2).toInt());
		newPen.setColor(color);
	}
	painter->setPen(newPen);
	int txtOption=getAtu(info,"txtop").toInt();
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

	painter->drawText(rect,txt,o);
	painter->setFont(oldFont);
	QString isRect=getAtu(info,"isrect");
	if(isRect=="1")
	{
		QString rectPx=getAtu(info,"rectpx");
		QString rectColorStr=getAtu(info,"rectcor");
		QStringList lstRectColorStr=rectColorStr.split(",");
		QColor rectColor(lstRectColorStr.at(0).toInt(),lstRectColorStr.at(1).toInt(),lstRectColorStr.at(2).toInt());
		QPen newRtPen;
		newRtPen.setColor(rectColor);
		newRtPen.setWidth(rectPx.toInt());
		painter->setPen(newRtPen);
		painter->drawRect(rect);
	}
	painter->setPen(oldPen);
}
//����info���Ի�������
void drawLine(QRectF rect,int type,QPainter *painter,QString info)
{	
	QPen oldpen=painter->pen();
	QPen pen;
	QString lineType=getAtu(info,"type");
	QString colorStr=getAtu(info,"color");
	QStringList lstColorStr=colorStr.split(",");	
	if(lstColorStr.size()>=3)
	{
		QColor color(lstColorStr.at(0).toInt(),lstColorStr.at(1).toInt(),lstColorStr.at(2).toInt());
		pen.setColor(color);
	}
	QString linePx=getAtu(info,"px");
	pen.setStyle(Qt::SolidLine);
	if(lineType=="1")
		pen.setStyle(Qt::DashLine);
	else if(lineType=="2")
		pen.setStyle(Qt::DotLine);
	else if(lineType=="3")
		pen.setStyle(Qt::DashDotLine);
	pen.setWidth(linePx.toInt());
	painter->setPen(pen);
	if(type==2)
		painter->drawLine(rect.x(),rect.y(),rect.x()+rect.width(),rect.y());
	else if(type==3)
		painter->drawLine(rect.x(),rect.y(),rect.x(),rect.y()+rect.height());
	else if(type==7)
		painter->drawLine(rect.x(),rect.y(),rect.x()+rect.width(),rect.y()+rect.height());
	painter->setPen(oldpen);
}
///////////////////////////////////////////////////
//��Ϊ����
PaintWidget::PaintWidget(QWidget *parent)
{
	m_pOper=DB;
	QPalette bgpal = palette();
	bgpal.setColor(QPalette::Background,Qt::white);
	m_dataWindow = GET_DW_CFG;
	setPalette(bgpal);
	this->parent=(view_reportshow *)parent;
}
void PaintWidget::setDbSet(CSsp_Dataset*dbSet)
{
	this->dbSet=dbSet;
}
void PaintWidget::setreportNo(int reportNo)
{
	this->reportNo=reportNo;
}
void PaintWidget::paintEvent(QPaintEvent *event) 
{ 
	S_UNUSED(event);
	QPainter painter(this); 
	QRegion rg=visibleRegion();
	int redrawTop=rg.boundingRect().top();//��ǰ���½�
	int redrawBottom=rg.boundingRect().bottom();
	QFont font;
	font.setPointSize(20);
	painter.setFont(font);
	//painter.setBrush(Qt::white); 

	QTextOption o;
	o.setAlignment(Qt::AlignCenter);
	SRecordset reportItemSets;
	m_pOper->RetrieveRecordset(SString::toFormat("select item_no,report_no,pos,type,seg,rect,info from t_ssp_report_item where report_no=%d order by seg+0",reportNo),reportItemSets);//������seg����
	if(reportItemSets.GetRows()==0)
		return;
	QList<divStruct> divList;
	int divHeight=0;
	for(int i=0;i<reportItemSets.GetRows();i++)
	{
		SString si2=reportItemSets.GetValue(i,2);
		if(reportItemSets.GetValue(i,3).toInt()==5)//�ָ���
		{
			divStruct div1(reportItemSets.GetValue(i,4).data(),reportItemSets.GetValue(i,0).toInt(),SString::GetIdAttributeI(2,si2,",")-divHeight-DEVHEIGHT);
			divHeight=SString::GetIdAttributeI(2,si2,",");
			divList.append(div1);//˳������
		}
	}
	if(dbSet == NULL)
	{
		QString txt="���ݼ���Ч��";
		painter.drawText(10,100,txt);
		return ;
	}
	int y=0;//ƫ��
	//int y1=0;
	int divH=0;
	for(int i1=0;i1<divList.size();i1++)
	{
		divStruct div1=divList.at(i1);
		if(div1.no=="0")
		{
			for(int i=0;i<reportItemSets.GetRows();i++)
			{
				int segNum=reportItemSets.GetValue(i,4).toInt();
				int type=reportItemSets.GetValue(i,3).toInt();
				if(segNum==0)
				{
					SString si2=reportItemSets.GetValue(i,2);
					SString si5=reportItemSets.GetValue(i,5);
					if(type!=5)
					{
						int rX=SString::GetIdAttributeI(1,si2,",")+SString::GetIdAttributeI(1,si5,",");
						int rY=SString::GetIdAttributeI(2,si2,",")+SString::GetIdAttributeI(2,si5,",")+y-0*DEVHEIGHT;
						int rWidth=SString::GetIdAttributeI(3,si5,",");
						int rHeight=SString::GetIdAttributeI(4,si5,",");

						QRectF rect(rX,rY,rWidth,rHeight);
						if(rY<redrawTop-rHeight)//������ǰ�Ͻ粻����,��Ӹ߶ȱ�֤��������
							continue;
						if(rY>redrawBottom)//������ǰ�½�����������
							break;
						if(type==2||type==3||type==7)//����
						{
							QString info=reportItemSets.GetValue(i,6).data();
							drawLine(rect,type,&painter,info);
						}
						else if(type==4)//�ı�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							drawText(rect,txt,&painter,info);
						}
						else if(type==1)//����
						{
							painter.drawRect(rect);
						}
						else if(type==6)//���ʽ��ֻ������
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString fun=getAtu(info,"fun");
							if(fun!="")
							{
								if(fun=="no")
									drawText(rect,QString::number(1),&painter,info);
								else if(fun=="sum")
								{
									QString exp=getAtu(info,"exp");
									QString txt=getAtu(info,"txt");
									int k=exp.toInt();
									if(k>=dbSet->GetColumnCount())
										break;
									SRecordset *record =dbSet->GetRecordset();
									int iRow=record->GetRows();
									int sum=0;
									for(int i=0;i<iRow;i++)
										sum+=record->GetValue(i,k).toInt();
									drawText(rect,txt+QString::number(sum),&painter,info);
								}
								else if(fun=="time")//ʱ��
								{
									drawText(rect,"��ǰʱ��:"+QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"),&painter,info);
								}
								else if(fun=="selcon")//��ʾ��ѯ����
								{
									if(con=="")
										con="��ѯ����:��";
									drawText(rect,con,&painter,info);
								}
								else if(fun=="count")//��ʾ����
								{
									int rowCount=dbSet->GetRecordset()->GetRows();
									drawText(rect,"������:"+QString::number(rowCount),&painter,info);
								}
								else if(fun=="author")//��ʾ����
								{
									QString author=SSP_GUI->GetLoginUserName().data();
									drawText(rect,author,&painter,info);
								}
							}
						}//type6
						else if(type==8)//ͼ��
						{
							int chartNo=reportItemSets.GetValue(i,0).toInt();
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							int type=getAtu(info,"charttype").toInt();

							QString extAtu=getAtu(info,"extatu");
							extAtu.replace(":","=");//�滻���ţ������ͻ
							extAtu.replace(".",";");
							int pointRow=getAtu(extAtu,"pointrow").toInt();
							int pointCol=getAtu(extAtu,"pointcol").toInt();

							CSsp_Dataset* dbSet=parent->getdbSetMap().value(chartNo);//ͨ��item�ŵõ����ݼ�
							if(dbSet==NULL)
								continue;
							SRect srect;
							srect.left=rect.x();
							srect.right=rect.x()+rect.width();
							srect.top=rect.y();
							srect.bottom=rect.y()+rect.height();

							if(type==0)//��ͼ
								CSpQtChartPainter::DrawChartPie(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==1)//��ͼ
								CSpQtChartPainter::DrawChartColumn(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==2)//����ͼ
								CSpQtChartPainter::DrawChartLine(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==3)//�Ǳ���
								CSpQtChartPainter::DrawPanel(&painter,srect,dbSet->GetRecordset()->GetValue(pointRow,pointCol).toInt(),extAtu.toStdString().data());
						}//type8
					}
				}
			}
		}
		else if(div1.no=="1")
		{
			for(int i=0;i<reportItemSets.GetRows();i++)
			{
				int segNum=reportItemSets.GetValue(i,4).toInt();
				int type=reportItemSets.GetValue(i,3).toInt();
				if(segNum==1)
				{
					SString si2=reportItemSets.GetValue(i,2);
					SString si5=reportItemSets.GetValue(i,5);
					if(type!=5)
					{
						int rX=SString::GetIdAttributeI(1,si2,",")+SString::GetIdAttributeI(1,si5,",");
						int rY=SString::GetIdAttributeI(2,si2,",")+SString::GetIdAttributeI(2,si5,",")+y-1*DEVHEIGHT;
						int rWidth=SString::GetIdAttributeI(3,si5,",");
						int rHeight=SString::GetIdAttributeI(4,si5,",");
						
						QRectF rect(rX,rY,rWidth,rHeight);
						if(rY<redrawTop-rHeight)//������ǰ�Ͻ粻����,��Ӷ���߶ȱ�֤��������
							continue;
						if(rY>redrawBottom)//������ǰ�½�����������
							break;
						if(type==2||type==3||type==7)//����
						{
							QString info=reportItemSets.GetValue(i,6).data();
							drawLine(rect,type,&painter,info);
						}
						else if(type==4)//�ı�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							drawText(rect,txt,&painter,info);
						}
						else if(type==1)//����
						{
							painter.drawRect(rect);
						}
						else if(type==6)//���ʽ��ֻ������
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString fun=getAtu(info,"fun");
							if(fun!="")
							{
								if(fun=="no")//��ţ�����
								{
									int rowCount=dbSet->GetRecordset()->GetRows();
									drawText(rect,QString::number(rowCount),&painter,info);
								}
								else if(fun=="sum")//���
								{
									QString exp=getAtu(info,"exp");
									QString txt=getAtu(info,"txt");
									int k=exp.toInt();
									if(k>=dbSet->GetColumnCount())
										break;
									SRecordset *record =dbSet->GetRecordset();
									int iRow=record->GetRows();
									int sum=0;
									for(int i=0;i<iRow;i++)
										sum+=record->GetValue(i,k).toInt();
									drawText(rect,txt+QString::number(sum),&painter,info);
								}
								else if(fun=="time")//ʱ��
								{
									drawText(rect,"��ǰʱ��:"+QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"),&painter,info);
								}
								else if(fun=="selcon")//��ʾ��ѯ����
								{
									if(con=="")
										con="��ѯ����:��";
									drawText(rect,con,&painter,info);
								}
							}
						}//type6
						else if(type==8)//ͼ��
						{
							int chartNo=reportItemSets.GetValue(i,0).toInt();
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							int type=getAtu(info,"charttype").toInt();

							QString extAtu=getAtu(info,"extatu");
							extAtu.replace(":","=");//�滻���ţ������ͻ
							extAtu.replace(".",";");
							int pointRow=getAtu(extAtu,"pointrow").toInt();
							int pointCol=getAtu(extAtu,"pointcol").toInt();

							CSsp_Dataset* dbSet=parent->getdbSetMap().value(chartNo);
							if(dbSet==NULL)
								continue;
							SRect srect;
							srect.left=rect.x();
							srect.right=rect.x()+rect.width();
							srect.top=rect.y();
							srect.bottom=rect.y()+rect.height();

							if(type==0)//��ͼ
								CSpQtChartPainter::DrawChartPie(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==1)//��ͼ
								CSpQtChartPainter::DrawChartColumn(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==2)//����ͼ
								CSpQtChartPainter::DrawChartLine(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==3)//�Ǳ���
								CSpQtChartPainter::DrawPanel(&painter,srect,dbSet->GetRecordset()->GetValue(pointRow,pointCol).toInt(),extAtu.toStdString().data());
						}//type8
					}
				}
			}
		}
		else if(div1.no=="2")
		{
			QString grp;
			QString grpInfo;
			QList<QString> grpList;
			bool bAutoTrans = dbSet->GetAutoColumnTransform();
			for(int j=0;j<reportItemSets.GetRows();j++)
			{
				if(reportItemSets.GetValue(j,3).toInt()==5)//�ָ���
				{
					if(reportItemSets.GetValue(j,4).toInt()==100)//Ŀǰֻ����һ�ط���
					{
						SString info=reportItemSets.GetValue(j,6);
						grpInfo=reportItemSets.GetValue(j,6).data();
						grp=getAtu(info.data(),"grp");
						break;
					}
				}
			}
			SRecordset *record =dbSet->GetRecordset();
			int col=0;
			for(col=0;col<record->GetColumns();col++)
			{
				SString colName=record->GetColumnName(col);
				if(colName==record->GetColumnName(grp.toInt()))
					break;
			}
			int iRow = record->GetRows();
			for(int row=0;row<iRow;row++)
			{
				if(grp=="")//û�з�������
				{
					for(int j=0;j<reportItemSets.GetRows();j++)
					{
						int segNum=reportItemSets.GetValue(j,4).toInt();
						int type=reportItemSets.GetValue(j,3).toInt();
						if(segNum==2)
						{
							SString sj2=reportItemSets.GetValue(j,2);
							SString sj5=reportItemSets.GetValue(j,5);
							if(type!=5)
							{
								int rX=SString::GetIdAttributeI(1,sj2,",")+SString::GetIdAttributeI(1,sj5,",");
								int rY=SString::GetIdAttributeI(2,sj2,",")+SString::GetIdAttributeI(2,sj5,",")+y-2*DEVHEIGHT;
								int rWidth=SString::GetIdAttributeI(3,sj5,",");
								int rHeight=SString::GetIdAttributeI(4,sj5,",");
								QRectF rect(rX,rY,rWidth,rHeight);
 								if(rY<redrawTop-rHeight)//������ǰ�Ͻ粻����,��Ӷ���߶ȱ�֤��������
 									continue;
								if(rY>redrawBottom)//������ǰ�½�����������
									break;
								if(type==2||type==3||type==7)//�߶�
								{
									QString info=reportItemSets.GetValue(j,6).data();
									drawLine(rect,type,&painter,info);
								}
								else if(type==4)//�ı�
								{
									QString info=reportItemSets.GetValue(j,6).data();
									QString txt=getAtu(info,"txt");
									drawText(rect,txt,&painter,info);
								}
								else if(type==6)//���ʽ
								{
									QString info=reportItemSets.GetValue(j,6).data();
									QString fun=getAtu(info,"fun");
									if(fun=="")//�Ǻ���
									{
										QString txt;
										QString exp=getAtu(info,"exp");
										int k=exp.toInt();
										if(k>=dbSet->GetColumnCount())
											break;											
										if(grp!=""&&k==grp.toInt())//�������ʱ�ų�����
											continue;
										if (dbSet->GetColumn(k)->m_pRef==NULL)
										{
											if(!bAutoTrans && dbSet->GetColumn(k)->m_sConv!="")
												txt=dbSet->GetColumn(k)->GetConvDb2Sys(record->GetValue(row,k)).data();
											else
												txt=record->GetValue(row,k).data();
											drawText(rect,txt,&painter,info);
										}
										else
										{
											SRecordset *pRef  = dbSet->GetColumn(k)->m_pRef->GetRecordset();
											//int iIndex = 0;
											for (int k1 = 0;k1<pRef->GetRows();k1++)
											{
												if (record->GetValue(row,k)==pRef->GetValue(k1,0))
												{
													txt=tr(pRef->GetValue(k1,1).data());
													drawText(rect,txt,&painter,info);
													break;
												}
												else if(k1==pRef->GetRows()-1)//û�ҵ���Ӧ��ȡĬ��ֵ
												{
													//txt=(tr(pRef->GetValue(0,1).data()));
													txt="";
													drawText(rect,txt,&painter,info);
												}
											}
										}
									}
									else//����
									{
										if(fun=="no")//���
											drawText(rect,QString::number(row+1),&painter,info);
										else if(fun=="sum")//���
										{
											QString exp=getAtu(info,"exp");
											QString txt=getAtu(info,"txt");
											int k=exp.toInt();
											if(k>=dbSet->GetColumnCount())
												break;
											SRecordset *record =dbSet->GetRecordset();
											int iRow=record->GetRows();
											int sum=0;
											for(int i=0;i<iRow;i++)
												sum+=record->GetValue(i,k).toInt();
											drawText(rect,txt+QString::number(sum),&painter,info);
										}
										else if(fun=="time")//ʱ��
										{
											drawText(rect,"��ǰʱ��:"+QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"),&painter,info);
										}
										else if(fun=="selcon")//��ʾ��ѯ����
										{
											if(con=="")
												con="��ѯ����:��";
											drawText(rect,con,&painter,info);
										}
									}
								}
								else if(type==1)//����
								{
									painter.drawRect(rect);
								}
								else if(type==8)//ͼ��
								{
									int chartNo=reportItemSets.GetValue(j,0).toInt();
									QString info=reportItemSets.GetValue(j,6).data();
									QString txt=getAtu(info,"txt");
									int type=getAtu(info,"charttype").toInt();

									QString extAtu=getAtu(info,"extatu");
									extAtu.replace(":","=");//�滻���ţ������ͻ
									extAtu.replace(".",";");
									//int pointRow=getAtu(extAtu,"pointrow").toInt();
									int pointCol=getAtu(extAtu,"pointcol").toInt();

									CSsp_Dataset* dbSet=parent->getdbSetMap().value(chartNo);
									if(dbSet==NULL)
										continue;
									SRect srect;
									srect.left=rect.x();
									srect.right=rect.x()+rect.width();
									srect.top=rect.y();
									srect.bottom=rect.y()+rect.height();

									if(type==0)//��ͼ
										CSpQtChartPainter::DrawChartPie(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
									else if(type==1)//��ͼ
										CSpQtChartPainter::DrawChartColumn(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
									else if(type==2)//����ͼ
										CSpQtChartPainter::DrawChartLine(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
									else if(type==3)//�Ǳ���
										CSpQtChartPainter::DrawPanel(&painter,srect,dbSet->GetRecordset()->GetValue(row,pointCol).toInt(),extAtu.toStdString().data());
								}//type8
							}


						}//segNum==2
					}
					if(row!=iRow-1)//���һ�β��Ӿ���
						y=y+div1.height;
				}
				else//������
				{
					SString grpValue=record->GetValue(row,col);
					QString grpValueStr=grpValue.data();						
					if(grpList.contains(grpValueStr))
						continue;
					grpList.append(grpValueStr);

					QString transName;//ȡ����ת���ı�
					if (dbSet->GetColumn(col)->m_pRef==NULL)
					{
						if(!bAutoTrans && dbSet->GetColumn(col)->m_sConv!="")
							transName=dbSet->GetColumn(col)->GetConvDb2Sys(record->GetValue(row,col)).data();
						else
							transName=record->GetValue(row,col).data();
					}
					else
					{
						SRecordset *pRef  = dbSet->GetColumn(col)->m_pRef->GetRecordset();
						//int iIndex = 0;
						for (int k1 = 0;k1<pRef->GetRows();k1++)
						{
							if (record->GetValue(row,col)==pRef->GetValue(k1,0))
							{
								transName=tr(pRef->GetValue(k1,1).data());
								break;
							}
							else if(k1==pRef->GetRows()-1)//û�ҵ���Ӧ��ȡĬ��ֵ
							{
								transName="";
							}
						}
					}
					//int fontSize=painter.font().pointSize();//����ͷ
					int grpFontSize=getAtu(grpInfo,"fontsize").toInt();
					int width=parent->getWidth();
					if(width<1)
						width=5000;
					QString txt;
					QStringList tmpTxtLst=getAtu(grpInfo,"txt").split("@");
					txt=tmpTxtLst.at(0)+transName;
					if(tmpTxtLst.size()>1)
						txt=txt+tmpTxtLst.at(1);
// 					if(y>0)
// 						y+=div1.height;
					drawText(QRect(10,divH+y+DEVHEIGHT,width,grpFontSize*2),txt,&painter,grpInfo);
					//painter.fillRect(0,divH+y-fontSize,5000,fontSize,Qt::yellow);
					//painter.drawText(QPointF(50,divH+y),"����:"+transName);
					y=grpFontSize*2+y;
					for(int row1=0;row1<iRow;row1++)
					{
						if(grpValue==record->GetValue(row1,col))
						{
							//y+=20;
							for(int j=0;j<reportItemSets.GetRows();j++)
							{
								int segNum=reportItemSets.GetValue(j,4).toInt();
								int type=reportItemSets.GetValue(j,3).toInt();
								if(segNum==2)
								{
									SString sj2=reportItemSets.GetValue(j,2);
									SString sj5=reportItemSets.GetValue(j,5);
									if(type!=5)
									{
										int rX=SString::GetIdAttributeI(1,sj2,",")+SString::GetIdAttributeI(1,sj5,",");
										int rY=SString::GetIdAttributeI(2,sj2,",")+SString::GetIdAttributeI(2,sj5,",")+y-2*DEVHEIGHT;
										int rWidth=SString::GetIdAttributeI(3,sj5,",");
										int rHeight=SString::GetIdAttributeI(4,sj5,",");
										QRectF rect(rX,rY,rWidth,rHeight);
										if(rY<redrawTop-rHeight)//������ǰ�Ͻ粻����,��Ӹ߶ȱ�֤��������
											continue;
										if(rY>redrawBottom)//������ǰ�½�����������
											break;

										if(type==2||type==3||type==7)//����
										{
											QString info=reportItemSets.GetValue(j,6).data();
											drawLine(rect,type,&painter,info);
										}
										else if(type==4)//�ı�
										{
											QString info=reportItemSets.GetValue(j,6).data();
											QString txt=getAtu(info,"txt");
											drawText(rect,txt,&painter,info);
										}
										else if(type==6)//���ʽ
										{
											QString info=reportItemSets.GetValue(j,6).data();
											QString fun=getAtu(info,"fun");
											if(fun=="")//�Ǻ���
											{
												QString txt;
												
												QString exp=getAtu(info,"exp");
												int k=exp.toInt();
												if(k>=dbSet->GetColumnCount())
													break;											
// 												if(grp!=""&&k==grp.toInt())
// 													continue;
												if (dbSet->GetColumn(k)->m_pRef==NULL)
												{
													if(!bAutoTrans && dbSet->GetColumn(k)->m_sConv!="")
														txt=dbSet->GetColumn(k)->GetConvDb2Sys(record->GetValue(row1,k)).data();
													else
														txt=record->GetValue(row1,k).data();
													drawText(rect,txt,&painter,info);
												}
												else
												{
													SRecordset *pRef  = dbSet->GetColumn(k)->m_pRef->GetRecordset();
													//int iIndex = 0;
													for (int k1 = 0;k1<pRef->GetRows();k1++)
													{
														if (record->GetValue(row1,k)==pRef->GetValue(k1,0))
														{
															txt=tr(pRef->GetValue(k1,1).data());
															drawText(rect,txt,&painter,info);
															break;
														}
														else if(k1==pRef->GetRows()-1)//û�ҵ���Ӧ��ȡĬ��ֵ
														{
															//txt=(tr(pRef->GetValue(0,1).data()));
															txt="";
															drawText(rect,txt,&painter,info);
														}
													}
												}
											}
											else//����
											{
												if(fun=="no")//���
													drawText(rect,QString::number(row1+1),&painter,info);
												else if(fun=="sum")//���
												{
													QString exp=getAtu(info,"exp");
													QString txt=getAtu(info,"txt");
													int k=exp.toInt();
													if(k>=dbSet->GetColumnCount())
														break;
													SRecordset *record =dbSet->GetRecordset();
													int iRow=record->GetRows();
													int sum=0;
													for(int i=0;i<iRow;i++)
														sum+=record->GetValue(i,k).toInt();
													drawText(rect,txt+QString::number(sum),&painter,info);
												}
												else if(fun=="time")//ʱ��
												{
													drawText(rect,"��ǰʱ��:"+QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"),&painter,info);
												}
												else if(fun=="selcon")//��ʾ��ѯ����
												{
													if(con=="")
														con="��ѯ����:��";
													drawText(rect,con,&painter,info);
												}
												else if(fun=="count")//��ʾ����
												{
													int rowCount=dbSet->GetRecordset()->GetRows();
													drawText(rect,"������:"+QString::number(rowCount),&painter,info);
												}
												else if(fun=="author")//��ʾ����
												{
													QString author=SSP_GUI->GetLoginUserName().data();
													drawText(rect,author,&painter,info);
												}
											}
										}
										else if(type==1)//����
										{
											painter.drawRect(rect);
										}
										else if(type==8)//ͼ��
										{
											int chartNo=reportItemSets.GetValue(j,0).toInt();
											QString info=reportItemSets.GetValue(j,6).data();
											QString txt=getAtu(info,"txt");
											int type=getAtu(info,"charttype").toInt();

											QString extAtu=getAtu(info,"extatu");
											extAtu.replace(":","=");//�滻���ţ������ͻ
											extAtu.replace(".",";");
											//int pointRow=getAtu(extAtu,"pointrow").toInt();
											int pointCol=getAtu(extAtu,"pointcol").toInt();

											CSsp_Dataset* dbSet=parent->getdbSetMap().value(chartNo);
											if(dbSet==NULL)
												continue;
											SRect srect;
											srect.left=rect.x();
											srect.right=rect.x()+rect.width();
											srect.top=rect.y();
											srect.bottom=rect.y()+rect.height();
											if(type==0)
												CSpQtChartPainter::DrawChartPie(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
											else if(type==1)
												CSpQtChartPainter::DrawChartColumn(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
											else if(type==2)
												CSpQtChartPainter::DrawChartLine(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
											else if(type==3)//�Ǳ���
												CSpQtChartPainter::DrawPanel(&painter,srect,dbSet->GetRecordset()->GetValue(row,pointCol).toInt(),extAtu.toStdString().data());
										}//type8
									}

								}//segNum==2
							}
						if(row1!=iRow-1)//���һ�β��Ӿ���
							y=y+div1.height;

						}//if(grpValue==record->GetValue(row1,col))
					}//for row1
				}
			}//for row
		}
		else if(div1.no=="3")
		{
			for(int i=0;i<reportItemSets.GetRows();i++)
			{
				int segNum=reportItemSets.GetValue(i,4).toInt();
				int type=reportItemSets.GetValue(i,3).toInt();
				if(segNum==3)
				{
					SString si2=reportItemSets.GetValue(i,2);
					SString si5=reportItemSets.GetValue(i,5);
					if(type!=5)
					{
						int rX=SString::GetIdAttributeI(1,si2,",")+SString::GetIdAttributeI(1,si5,",");
						int rY=SString::GetIdAttributeI(2,si2,",")+SString::GetIdAttributeI(2,si5,",")+y-3*DEVHEIGHT;
						int rWidth=SString::GetIdAttributeI(3,si5,",");
						int rHeight=SString::GetIdAttributeI(4,si5,",");
						QRectF rect(rX,rY,rWidth,rHeight);

						if(rY>redrawBottom)//������ǰ�½�����������
							break;

						if(type==2||type==3||type==7)//�߶�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							drawLine(rect,type,&painter,info);
						}
						else if(type==4)//�ı�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							drawText(rect,txt,&painter,info);
						}
						else if(type==1)//����
						{
							painter.drawRect(rect);
						}
						else if(type==6)//���ʽ��ֻ������
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString fun=getAtu(info,"fun");
							if(fun!="")
							{
								if(fun=="no")//��ţ�����
								{
									int rowCount=dbSet->GetRecordset()->GetRows();
									drawText(rect,QString::number(rowCount),&painter,info);
								}
								else if(fun=="sum")//���
								{
									QString exp=getAtu(info,"exp");
									QString txt=getAtu(info,"txt");
									int k=exp.toInt();
									if(k>=dbSet->GetColumnCount())
										break;
									SRecordset *record =dbSet->GetRecordset();
									int iRow=record->GetRows();
									int sum=0;
									for(int i=0;i<iRow;i++)
										sum+=record->GetValue(i,k).toInt();
									drawText(rect,txt+QString::number(sum),&painter,info);
								}
								else if(fun=="time")//ʱ��
								{
									drawText(rect,"��ǰʱ��:"+QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"),&painter,info);
								}
								else if(fun=="selcon")//��ʾ��ѯ����
								{
									if(con=="")
										con="��ѯ����:��";
									drawText(rect,con,&painter,info);
								}
								else if(fun=="count")//��ʾ����
								{
									int rowCount=dbSet->GetRecordset()->GetRows();
									drawText(rect,"������:"+QString::number(rowCount),&painter,info);
								}
								else if(fun=="author")//��ʾ����
								{
									QString author=SSP_GUI->GetLoginUserName().data();
									drawText(rect,author,&painter,info);
								}
							}
						}//type6
						else if(type==8)//ͼ��
						{
							int chartNo=reportItemSets.GetValue(i,0).toInt();
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							int type=getAtu(info,"charttype").toInt();

							QString extAtu=getAtu(info,"extatu");
							extAtu.replace(":","=");//�滻���ţ������ͻ
							extAtu.replace(".",";");
							int pointRow=getAtu(extAtu,"pointrow").toInt();
							int pointCol=getAtu(extAtu,"pointcol").toInt();

							CSsp_Dataset* dbSet=parent->getdbSetMap().value(chartNo);
							if(dbSet==NULL)
								continue;
							SRect srect;
							srect.left=rect.x();
							srect.right=rect.x()+rect.width();
							srect.top=rect.y();
							srect.bottom=rect.y()+rect.height();

							if(type==0)//��ͼ
								CSpQtChartPainter::DrawChartPie(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==1)//��ͼ
								CSpQtChartPainter::DrawChartColumn(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==2)//����ͼ
								CSpQtChartPainter::DrawChartLine(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==3)//�Ǳ���
								CSpQtChartPainter::DrawPanel(&painter,srect,dbSet->GetRecordset()->GetValue(pointRow,pointCol).toInt(),extAtu.toStdString().data());
						}//type8
					}
				}
			}
		}
		else if(div1.no=="4")
		{
			for(int i=0;i<reportItemSets.GetRows();i++)
			{
				int segNum=reportItemSets.GetValue(i,4).toInt();
				int type=reportItemSets.GetValue(i,3).toInt();
				if(segNum==4)
				{
					SString si2=reportItemSets.GetValue(i,2);
					SString si5=reportItemSets.GetValue(i,5);
					if(type!=5)
					{
						int rX=SString::GetIdAttributeI(1,si2,",")+SString::GetIdAttributeI(1,si5,",");
						int rY=SString::GetIdAttributeI(2,si2,",")+SString::GetIdAttributeI(2,si5,",")+y-4*DEVHEIGHT;
						int rWidth=SString::GetIdAttributeI(3,si5,",");
						int rHeight=SString::GetIdAttributeI(4,si5,",");
						QRectF rect(rX,rY,rWidth,rHeight);
						if(rY>redrawBottom)//������ǰ�½�����������
							break;
						if(type==2||type==3||type==7)//����
						{
							QString info=reportItemSets.GetValue(i,6).data();
							drawLine(rect,type,&painter,info);
						}
						else if(type==4)//�ı�
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							drawText(rect,txt,&painter,info);
						}
						else if(type==1)//����
						{
							painter.drawRect(rect);
						}
						else if(type==6)//���ʽ��ֻ������
						{
							QString info=reportItemSets.GetValue(i,6).data();
							QString fun=getAtu(info,"fun");
							if(fun!="")
							{
								if(fun=="no")//��ţ�����
								{
									int rowCount=dbSet->GetRecordset()->GetRows();
									drawText(rect,QString::number(rowCount),&painter,info);
								}
								else if(fun=="sum")//���
								{
									QString exp=getAtu(info,"exp");
									QString txt=getAtu(info,"txt");
									int k=exp.toInt();
									if(k>=dbSet->GetColumnCount())
										break;
									SRecordset *record =dbSet->GetRecordset();
									int iRow=record->GetRows();
									int sum=0;
									for(int i=0;i<iRow;i++)
										sum+=record->GetValue(i,k).toInt();
									drawText(rect,txt+QString::number(sum),&painter,info);
								}
								else if(fun=="time")//ʱ��
								{
									drawText(rect,"��ǰʱ��:"+QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"),&painter,info);
								}
								else if(fun=="selcon")//��ʾ��ѯ����
								{
									if(con=="")
										con="��ѯ����:��";
									drawText(rect,con,&painter,info);
								}
								else if(fun=="count")//��ʾ����
								{
									int rowCount=dbSet->GetRecordset()->GetRows();
									drawText(rect,"������:"+QString::number(rowCount),&painter,info);
								}
								else if(fun=="author")//��ʾ����
								{
									QString author=SSP_GUI->GetLoginUserName().data();
									drawText(rect,author,&painter,info);
								}
							}
						}//type6
						else if(type==8)//ͼ��
						{
							int chartNo=reportItemSets.GetValue(i,0).toInt();
							QString info=reportItemSets.GetValue(i,6).data();
							QString txt=getAtu(info,"txt");
							int type=getAtu(info,"charttype").toInt();

							QString extAtu=getAtu(info,"extatu");
							extAtu.replace(":","=");//�滻���ţ������ͻ
							extAtu.replace(".",";");
							int pointRow=getAtu(extAtu,"pointrow").toInt();
							int pointCol=getAtu(extAtu,"pointcol").toInt();

							CSsp_Dataset* dbSet=parent->getdbSetMap().value(chartNo);
							if(dbSet==NULL)
								continue;
							SRect srect;
							srect.left=rect.x();
							srect.right=rect.x()+rect.width();
							srect.top=rect.y();
							srect.bottom=rect.y()+rect.height();

							if(type==0)
								CSpQtChartPainter::DrawChartPie(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==1)
								CSpQtChartPainter::DrawChartColumn(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==2)
								CSpQtChartPainter::DrawChartLine(&painter,srect,dbSet->GetRecordset(),txt.toStdString().data(),GetColorByAttr(extAtu.toStdString().data(),"tcor"),GetColorByAttr(extAtu.toStdString().data(),"lcor"),extAtu.toStdString().data());
							else if(type==3)//�Ǳ���
								CSpQtChartPainter::DrawPanel(&painter,srect,dbSet->GetRecordset()->GetValue(pointRow,pointCol).toInt(),extAtu.toStdString().data());
						}//type8
					}
				}
			}
		}
	divH+=divList.at(i1).height;//���Ը߶�
	if(i1==divList.size()-1)
	{
		resize(this->width(),divH+y);
	}
	}//for div
}
//��info��ȡ����,��ʽΪ "name=atu;"
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
