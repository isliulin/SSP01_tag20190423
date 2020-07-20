/********************************************************************
	created:	2015/07/31
	filename: 	logtable.cpp
	author:		yws
	purpose:	tabҳ��
*********************************************************************/
#include "logtable.h"

logTable::logTable(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	isClose=false;
	tmpLogs=NULL;
	ui.table->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.table->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.table->verticalHeader()->hide();
	connect(ui.filiterButton,SIGNAL(clicked()),this,SLOT(clicked_filiterButton()));
	connect(ui.forwardButton,SIGNAL(clicked()),this,SLOT(clicked_forwardButton()));
	connect(ui.backwardButton,SIGNAL(clicked()),this,SLOT(clicked_backwardButton()));
	connect(ui.exportButton,SIGNAL(clicked()),this,SLOT(clicked_exportButton()));
	connect(ui.table,SIGNAL(clicked( const QModelIndex &)),this,SLOT(sendMsg()));
	model=new Model(this);
	logs=new QList<myLog>();
	ui.table->setModel(model);
	model->setInfo(logs);
	ui.table->setColumnWidth(0,50);
	ui.table->setColumnWidth(1,90);
	ui.table->setColumnWidth(2,150);
	ui.table->setColumnWidth(3,60);
	ui.table->setColumnWidth(4,120);
	ui.table->setColumnWidth(5,50);
	ui.table->setColumnWidth(6,200);
	ui.table->horizontalHeader()->setStretchLastSection(true);
	//��ݼ�
	ui.backwardButton->setDefault(true);
	ui.backwardButton->setAutoDefault(true);
	//ui.filiterButton->setShortcut(tr("return")); 
	//ui.forwardButton->setShortcut(tr("return")); 
	//ui.backwardButton->setShortcut(tr("return")); 
	//ui.exportButton->setShortcut(tr("return")); 
}

logTable::~logTable()
{
	delete model;
	delete logs;

}
void logTable::setName(QString tabName)//�ļ���
{
	this->tabName=tabName;
}
// ============================================================================
// function:clicked_filiterButton
// date:2015/08/03
// input:
// output:
// desc:���˰�ť��8.3�����־���ݶ�����
// ============================================================================
void logTable::clicked_filiterButton()
{
	QString threadId=ui.threadIdText->text();
	QString LogTpe=ui.levelCombo->currentText();
	if(LogTpe=="all")
		LogTpe="";
	QStringList msgs=ui.filiterContext->text().split(QRegExp("\\s+"));//�Կո�ָ�����
	delete tmpLogs;
	tmpLogs=new QList <myLog>();
	for(int i=0;i<logs->size();i++)
	{
		if(threadId=="")//�̺߳�û��ʱ
		{
			if(logs->at(i).logType.contains(LogTpe))
			{
				bool flag=true;
				for(int j=0;j<msgs.size();j++)
				{
					if(logs->at(i).msg.contains(msgs.at(j))==false)//��
					{
						flag=false;
						break;
					}
				}
				if(flag==true)
					tmpLogs->append(logs->at(i));
			}
		}
		else//��д�̺߳�ʱ
		{
			if(logs->at(i).threadId==threadId&&
				logs->at(i).logType.contains(LogTpe))
			{
				bool flag=true;
				for(int j=0;j<msgs.size();j++)
				{
					if(logs->at(i).msg.contains(msgs.at(j))==false)//��
					{
						flag=false;
						break;
					}
				}
				if(flag==true)
					tmpLogs->append(logs->at(i));
			}
		}
	}
	model->setInfo(tmpLogs);
}
// ============================================================================
// function:clicked_forwardButton
// date:2015/08/03
// input:
// output:
// desc:��һ��,8.3����Ϊ����������һ��
// ============================================================================
void logTable::clicked_forwardButton()
{

	QList<myLog>* tmplog=model->getInfo();
	QString threadId=ui.threadIdText->text();
	QString LogTpe=ui.levelCombo->currentText();
	if(LogTpe=="all")
		LogTpe="";
	QStringList msgs=ui.filiterContext->text().split(QRegExp("\\s+"));//�Կո�ָ�����
	int currow=ui.table->currentIndex().row();
	if(currow==-1)//δѡ���д�ͷ��ʼ
	{
		currow=1;
		ui.table->setCurrentIndex(model->index(0,0));
	}
	if(currow==0)
	{	
		if(QMessageBox::Yes==QMessageBox::information(NULL,tr("��ʾ"),tr("�Ƿ��ĩ�˿�ʼ"), QMessageBox::Yes, QMessageBox::No))
			currow=tmplog->size();
	}

	L1://�����β�������ش˴�
	for(int i=currow-1;i>=0;i--)
	{
		if(threadId=="")//�̺߳�û��ʱ
		{
			if(tmplog->at(i).logType.contains(LogTpe))
			{
				bool flag=true;
				for(int j=0;j<msgs.size();j++)//�ж���־����
				{
					if(logs->at(i).msg.contains(msgs.at(j))==false)//��
					{
						flag=false;
						break;//for(int j=0;j<msgs.size();j++)
					}
				}
				if(flag==true)
				{
					ui.table->setCurrentIndex(model->index(i,0));
					sendMsg();//�Ҳ���Ӧ
					break;//for(int i=currow;i>0;i--)
				}
			}
		}
		else//��д�̺߳�ʱ
		{
			if(tmplog->at(i).threadId==threadId&&
				tmplog->at(i).logType.contains(LogTpe))
			{
				bool flag=true;
				for(int j=0;j<msgs.size();j++)//�ж���־����
				{
					if(logs->at(i).msg.contains(msgs.at(j))==false)//��
					{
						flag=false;
						break;//for(int j=0;j<msgs.size();j++)
					}
				}
				if(flag==true)
				{
					ui.table->setCurrentIndex(model->index(i,0));
					sendMsg();//�Ҳ���Ӧ
					break;//for(int i=currow;i>0;i--)
				}
			}
		}
		if(i==0)//����ͷ����δ������
		{
			if(QMessageBox::Yes==QMessageBox::information(NULL,tr("��ʾ"),tr("�Ƿ��ĩ�˿�ʼ"), QMessageBox::Yes, QMessageBox::No))
			{
				currow=tmplog->size();
				goto L1;
			}
		}
	}
}
// ============================================================================
// function:clicked_backwardButton
// date:2015/07/31
// input:
// output:
// desc:��һ��,8.3����Ϊ����������һ��
// ============================================================================
void logTable::clicked_backwardButton()
{
	QList<myLog>* tmplog=model->getInfo();
	QString threadId=ui.threadIdText->text();
	QString LogTpe=ui.levelCombo->currentText();
	if(LogTpe=="all")
		LogTpe="";
	QStringList msgs=ui.filiterContext->text().split(QRegExp("\\s+"));//�Կո�ָ�����
	int currow=ui.table->currentIndex().row();
	if(currow==-1)
	{
		ui.table->setCurrentIndex(model->index(0,0));
		currow=-1;
	}
	if(currow==tmplog->size()-1)//����ĩ����δ������
	{
		if(QMessageBox::Yes==QMessageBox::information(NULL,tr("��ʾ"),tr("�Ƿ��ĩ�˿�ʼ"), QMessageBox::Yes, QMessageBox::No))
			currow=-1;
	}
	L1://�����ͷ�������ش˴�
	for(int i=currow+1;i<tmplog->size();i++)
	{
		if(threadId=="")//�̺߳�û��ʱ
		{
			if(tmplog->at(i).logType.contains(LogTpe))
			{
				bool flag=true;
				for(int j=0;j<msgs.size();j++)//�ж���־����
				{
					if(logs->at(i).msg.contains(msgs.at(j))==false)//��
					{
						flag=false;
						break;//for(int j=0;j<msgs.size();j++)
					}
				}
				if(flag==true)
				{
					ui.table->setCurrentIndex(model->index(i,0));
					sendMsg();//�Ҳ���Ӧ
					break;//for(int i=currow;i>0;i--)
				}
			}
		}
		else//��д�̺߳�ʱ
		{
			if(tmplog->at(i).threadId==threadId&&
				tmplog->at(i).logType.contains(LogTpe))
			{
				bool flag=true;
				for(int j=0;j<msgs.size();j++)//�ж���־����
				{
					if(logs->at(i).msg.contains(msgs.at(j))==false)//��
					{
						flag=false;
						break;//for(int j=0;j<msgs.size();j++)
					}
				}
				if(flag==true)
				{
					ui.table->setCurrentIndex(model->index(i,0));
					sendMsg();//�Ҳ���Ӧ
					break;//for(int i=currow;i>0;i--)
				}
			}
		}
		if(i==tmplog->size()-1)//����ĩ����δ������
		{
			if(QMessageBox::Yes==QMessageBox::information(NULL,tr("��ʾ"),tr("�Ƿ��ĩ�˿�ʼ"), QMessageBox::Yes, QMessageBox::No))
			{
				currow=-1;
				goto L1;
			}
		}
	}
}
// ============================================================================
// function:clicked_exportButton
// date:2015/07/31
// input:
// output:
// desc:������ť
// ============================================================================
void logTable::clicked_exportButton()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("File"),tabName,(".txt"));
	if (fileName.isEmpty())
		return;
	QFile file(fileName);
	if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return;
	}
	QTextStream out(&file); 
	for(int i=0;i<logs->size();i++)
	{
		out<<logs->at(i).dateTime<<"["<<logs->at(i).threadId<<"] "
			<<logs->at(i).logType<<" at "<<logs->at(i).fileName<<"("<<logs->at(i).funName<<" line:"<<logs->at(i).line
			<<"):	"<<logs->at(i).msg<<"\n";
	}
	QMessageBox::information(this, tr("�������ݳɹ�"), tr("��Ϣ�ѱ�����%1��").arg(fileName), tr("ȷ��"));
	file.close();
}
bool logTable::isClosed()
{
	return isClose;
}
// ============================================================================
// function:sendMsg
// date:2015/07/31
// input:
// output:
// desc:�Ҳ��ı�����Ӧ
// ============================================================================
void logTable::sendMsg()
{
	int i=ui.table->currentIndex().row();
	if(i==-1)
		return;
	QString str=logs->at(i).dateTime+"["+logs->at(i).threadId+"] "
		+logs->at(i).logType+" at "+logs->at(i).fileName+"("+logs->at(i).funName+" line:"+logs->at(i).line
		+"):	"+logs->at(i).msg;
	ui.textEdit->setText(str);
}
// ============================================================================
// function:readFile
// date:2015/08/03
// input:fileName
// output:
// desc:��ȡ�ļ�����ӵ�logs������model
// ============================================================================
void logTable::readFile(QString fileName)
{
	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	setName(tabName);
	QStringList list;
	list.clear();
	QTextStream in(&file);
	int i =0;
	QString tmp;
	while(!in.atEnd())
	{
		//2015-07-30 13:38:33[00001938] fault at aaa(111 line:20):	12 345
		myLog log;
		QString fileLine = in.readLine();
		if(fileLine=="")
			continue;
		list = fileLine.split(QRegExp("\\s+"));//�Կո�ָ���
		if(!(list.at(2).contains("baseddebug")||list.at(2).contains("debug")||
			list.at(2).contains("warn")||list.at(2).contains("error")||
			list.at(2).contains("fault")||list.at(2).contains("none")))
			return;
		log.id=QString::number(++i);
		tmp=list.at(1).split("[").at(1);
		log.threadId=tmp.remove(QChar(']'));
		log.dateTime=list.at(0)+" "+list.at(1).split("[").at(0);
		log.logType=list.at(2);
		tmp=list.at(4);
		log.fileName=tmp.split("(").at(0);
		log.funName=tmp.split("(").at(1);
		tmp=list.at(5).split(":").at(1);
		log.line=tmp.remove(QChar(')'));
		log.msg=list.at(6);
		if(list.size()>6)//���ݿ��ܰ����ո�
			for(int i=7;i<list.size();i++)
				log.msg=log.msg+" "+list.at(i);
		logs->append(log);
		if(i%2000==0)//ÿ2000����ʾһ��
		{
			model->setInfo(logs);
			if(ui.rollingCheck->checkState()==Qt::Checked)//���������еײ�
			{
				ui.table->scrollToBottom();
			}
			qApp->processEvents();//����������
		}
	}
	file.close();
	model->setInfo(logs);
	if(ui.rollingCheck->checkState()==Qt::Checked)//���������еײ�
	{
		ui.table->scrollToBottom();
	}
}
// ============================================================================
// function:setRow
// date:2015/08/03
// input:logs
// output:
// desc:��ӵ�logs������model
// ============================================================================
void logTable::setRow(QList<stuLog *> tmpLogs)
{
	for(int i=0;i<tmpLogs.size();i++)
	{
		myLog tmplog;//����myLog����
		tmplog.id=QString::number(logs->size()+1);
		char* str=new char[20480];
		sprintf(str,"%p",tmpLogs.at(i)->threadid);//ת��Ϊ��ַ���ַ���
		tmplog.threadId=str;
		delete[] str;
		tmplog.dateTime=QString("%1-%2-%3 %4:%5:%6").arg(tmpLogs.at(i)->datetime.year+1900)
			.arg(tmpLogs.at(i)->datetime.month+1).arg(tmpLogs.at(i)->datetime.day+0).arg(tmpLogs.at(i)->datetime.hour+0).arg(tmpLogs.at(i)->datetime.minute+0).arg(tmpLogs.at(i)->datetime.second+0);
		QString tpe="";
		if(tmpLogs.at(i)->logtype+0==0)
			tpe="baseddebug";
		else if(tmpLogs.at(i)->logtype+0==1)
			tpe="debug";
		else if(tmpLogs.at(i)->logtype+0==2)
			tpe="warn";
		else if(tmpLogs.at(i)->logtype+0==3)
			tpe="error";
		else if(tmpLogs.at(i)->logtype+0==4)
			tpe="fault";
		else
			tpe="none";
		tmplog.logType=tpe;
		tmplog.fileName=tmpLogs.at(i)->filename;
		#ifdef LOG_HAS_FUNCTION_NAME
			tmplog.funName=tmpLogs.at(i)->funname;
		#else
			tmplog.funName="";
		#endif
		tmplog.line=QString::number(tmpLogs.at(i)->line);
		tmplog.msg=tmpLogs.at(i)->msg;
		logs->append(tmplog);
		if(i%2000==0)//ÿ2000����ʾһ��
		{
			model->setInfo(logs);
			if(ui.rollingCheck->checkState()==Qt::Checked)//���������еײ�
			{
				ui.table->scrollToBottom();
			}
			qApp->processEvents();//����������
		}
	}
	model->setInfo(logs);
	if(ui.rollingCheck->checkState()==Qt::Checked)//���������еײ�
	{
		ui.table->scrollToBottom();
	}
}

void logTable::closeEvent(QCloseEvent *event)
{
	isClose=true;
	return  QWidget::closeEvent(event);
}