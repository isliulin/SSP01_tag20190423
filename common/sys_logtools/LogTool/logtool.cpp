/********************************************************************
	created:	2015/07/31
	filename: 	logtool.cpp
	author:		yws
	purpose:	������
*********************************************************************/
#include "logtool.h"
LogTool::LogTool(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);	
	showMaximized();
	setWindowTitle(tr("��־����"));
	connect(ui.openFileAction,SIGNAL(triggered()),this,SLOT(openFile()));
	connect(ui.tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(removeSubTab(int)));
	connect(ui.exitAction,SIGNAL(triggered()),this,SLOT(close()));
	connect(ui.actionAbout,SIGNAL(triggered()),this,SLOT(triggered_actionAbout()));
	m_hSocket=0;
	tabNameList=new QList<QString>();
	port=0;
	setServer(7001);
	
	actionGroup=new QActionGroup(this);//��ʽ
	actionGroup->addAction(ui.actionWindows);
	actionGroup->addAction(ui.actionWindowsXP);
	actionGroup->addAction(ui.actionWindowsVista);
	actionGroup->addAction(ui.actionMotif);
	actionGroup->addAction(ui.actionCDE);
	actionGroup->addAction(ui.actionPlastique);
	actionGroup->addAction(ui.actionCleanlooks);
	connect(ui.actionWindows,SIGNAL(triggered()),this,SLOT(triggered_actionWindows()));
	connect(ui.actionWindowsXP,SIGNAL(triggered()),this,SLOT(triggered_actionWindowsXP()));
	connect(ui.actionWindowsVista,SIGNAL(triggered()),this,SLOT(triggered_actionWindowsVista()));
	connect(ui.actionMotif,SIGNAL(triggered()),this,SLOT(triggered_actionMotif()));
	connect(ui.actionCDE,SIGNAL(triggered()),this,SLOT(triggered_actionCDE()));
	connect(ui.actionPlastique,SIGNAL(triggered()),this,SLOT(triggered_actionPlastique()));
	connect(ui.actionCleanlooks,SIGNAL(triggered()),this,SLOT(triggered_actionCleanlooks()));
}
// ============================================================================
// function:setServer
// date:2015/07/31
// input:port
// output:
// desc:��port������acceptThread
// ============================================================================
void LogTool::setServer(int port)
{
	if(this->port!=0)
	{
		QMessageBox::information(this, tr("����"), tr("�˿��Ѱ���%1").arg(QString::number(this->port)), tr("ȷ��"));
		return; 
	}
	this->port=port;
	m_hSocket = (AF_INET,SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in serv_addr; 
	#ifdef WIN32
		WORD wVersionRequested;//�汾�� 
		wVersionRequested = MAKEWORD(1,1);
		WSADATA wsaData; 
		int err; 
		err = WSAStartup( wVersionRequested, &wsaData ); 
	#endif
	if ((m_hSocket = ::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == SOCKET_ERROR)
	{ 
		QMessageBox::information(this, tr("����"), tr("socket��������"), tr("ȷ��"));
		return; 
	} 
	serv_addr.sin_family=AF_INET; 
	serv_addr.sin_port=htons(this->port); 
	serv_addr.sin_addr.s_addr =INADDR_ANY;
	memset(&(serv_addr.sin_zero),0,8); 
	if(bind(m_hSocket ,(sockaddr*)&serv_addr,sizeof(sockaddr))==-1)
	{
		QMessageBox::information(this, tr("����"), tr("�˿ڰ󶨳���"), tr("ȷ��"));
		return; 
	}
	listen(m_hSocket,5);

	aptThread=new acceptThread();
	aptThread->getpTool(this);
	aptThread->start();
	connect(aptThread, SIGNAL(finished()),aptThread, SLOT(deleteLater()),Qt::BlockingQueuedConnection); //�߳�ִ�����������
}
SOCKET LogTool::getSocket()
{
	return m_hSocket;
}
LogTool::~LogTool()
{
	delete tabNameList;
	delete actionGroup;
}
// ============================================================================
// function:addTabTable
// date:2015/07/31
// input:pTool, pTable, name
// output:
// desc:�ۺ�����������tab������pTableָ��
// ============================================================================
void LogTool::addTabTable(LogTool *pTool,logTable * &pTable,QString name)
{
	logTable *newLog=new logTable(pTool);
 	pTool->ui.tabWidget->addTab(newLog,name); 
	pTool->ui.tabWidget->setCurrentIndex(pTool->ui.tabWidget->count()-1);
 	pTable=newLog;
}
// ============================================================================
// function:removeSubTab
// date:2015/07/31
// input:index
// output:
// desc:�ۺ�������Ӧ��
// ============================================================================
void LogTool::removeSubTab(int index)
{
	logTable *log=(logTable *)ui.tabWidget->widget(index);
	ui.tabWidget->removeTab(index);
	tabNameList->removeAt(index);
	delete log;
}
// ============================================================================
// function:openFile
// date:2015/08/03
// input:
// output:
// desc:���ļ���������table������tab��,0803tab��ȥ��·��,���ͬ�ļ��ж�
// ============================================================================
void LogTool::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("file"), qApp->applicationDirPath (),tr("*.txt"));
	for(int i=0;i<tabNameList->size();i++)
	{
		if(tabNameList->at(i).contains(fileName))
		{
			if(QMessageBox::Yes==QMessageBox::information(NULL,tr("��ʾ"),tr("���ļ��Ѿ����ڣ��Ƿ�ת����ҳ�棿"), QMessageBox::Yes, QMessageBox::No))
			{
				ui.tabWidget->setCurrentIndex(i);
			}
			return;
		}
	}
	QFile file(fileName);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	logTable *newTable=new logTable(this);
	ui.tabWidget->addTab(newTable,file.fileName().split("/").at(file.fileName().split("/").size()-1));
	tabNameList->append(fileName);
	ui.tabWidget->setCurrentIndex(ui.tabWidget->count()-1);
	newTable->readFile(fileName);
	newTable->setName(file.fileName().split("/").at(file.fileName().split("/").size()-1));
}
void LogTool::closeEvent(QCloseEvent *event)
{
	#ifdef WIN32
		closesocket(m_hSocket);
	#else
		::close(m_hSocket);
	#endif
	m_hSocket=NULL;
	return QMainWindow::closeEvent(event);
}
void LogTool::triggered_actionAbout()
{
	logabout dlg(this);
	dlg.exec();
}
QList<QString> *LogTool::getTabNameList()
{
	return tabNameList;
}
logTable *LogTool::getTableFromTab(int count)
{
	logTable* table=static_cast<logTable*>(ui.tabWidget->widget(count));
	return table;
}
//��ʽ
void LogTool::triggered_actionWindows()
{
	QApplication::setStyle(QStyleFactory::create("Windows"));
}
void LogTool::triggered_actionWindowsXP()
{
	QApplication::setStyle(QStyleFactory::create("WindowsXP"));
}
void LogTool::triggered_actionWindowsVista()
{
	QApplication::setStyle(QStyleFactory::create("WindowsVista"));
}
void LogTool::triggered_actionMotif()
{
	QApplication::setStyle(QStyleFactory::create("Motif"));
}
void LogTool::triggered_actionCDE()
{
	QApplication::setStyle(QStyleFactory::create("CDE"));
}
void LogTool::triggered_actionPlastique()
{
	QApplication::setStyle(QStyleFactory::create("Plastique"));
}
void LogTool::triggered_actionCleanlooks()
{
	QApplication::setStyle(QStyleFactory::create("Cleanlooks"));
}
///////////////////////////////////////////////////////////////////////////////////////////

void acceptThread::getpTool(LogTool *point)
{
	pTool=point;
}

void acceptThread::run()
{
	sockaddr_in addrClient;
	#ifdef WIN32
		int len=sizeof(sockaddr);
	#else
		socklen_t len=sizeof(sockaddr);
	#endif
	while(1) 
	{ 
		if(pTool==NULL||pTool->getSocket()==NULL)//������closeʱ����
			break;
		SOCKET sockConn=accept(pTool->getSocket(),(sockaddr*)&addrClient,&len);
		if(sockConn==SOCKET_ERROR)
			continue;
		recvThread *recv=new recvThread();
		connect(recv, SIGNAL(finished()),recv,SLOT(deleteLater()),Qt::DirectConnection); //�߳�ִ�����������
		recv->getpTool(pTool);
		recv->getIp(QString(QLatin1String(inet_ntoa(addrClient.sin_addr))));
		recv->getSock(sockConn);
		recv->start();
		//recv��ͨ���źŴ�����table
		connect(recv,SIGNAL(createTab(LogTool*,logTable *&,QString)),pTool,SLOT(addTabTable(LogTool*,logTable *&,QString)),Qt::BlockingQueuedConnection);
		
	} 
}
acceptThread::~acceptThread()
{
	pTool=NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

void recvThread::getpTool(LogTool *point)
{
	pTool=point;
}
recvThread::~recvThread()
{
	#ifdef WIN32
		closesocket(sockConn);
	#else
		::close(sockConn);
	#endif 
}
void recvThread::getIp(QString Ip)
{
	ip=Ip;
}
void recvThread::getSock(SOCKET sock)
{
	sockConn=sock;
}
void recvThread::run()
{
	bool flag=false;
	logTable *pTable=NULL;
	do 
	{	
		char recvBuf[sizeof(stuLog)];
		QList<stuLog *> logs;
		int k=recv(sockConn,recvBuf,sizeof(recvBuf),0);
		if(k!=SOCKET_ERROR&&k!=0) 
		{
			unsigned int x=0;
			while(x<k)
			{
				unsigned int n=0;
				memcpy(&n,&recvBuf[x],sizeof(int));
				n=ntohl(n);
				stuLog *log=new stuLog();
				memcpy(log,&recvBuf[x],n);
				logs.append(log);
				x=x+n;
			}
		}
		if(flag==false)//��һ��ѭ���Ŵ���table��Ŀ����Ϊ�˻�ȡrecv��modulename�������ļ���
		{
			name=QString(logs.at(0)->modulename);
			name=name+"@"+ip;
			bool foundFlag=false;
			for(int i=0;i<pTool->getTabNameList()->size();i++)
			{
				if(pTool->getTabNameList()->at(i).contains(name))
				{
					pTable=pTool->getTableFromTab(i);
					foundFlag=true;
					disconnect(this,SIGNAL(setRow(QList<stuLog *>)),pTable,SLOT(setRow(QList<stuLog *>)));
				}
			}
			if(foundFlag==false)
			{
				pTool->getTabNameList()->append(name);
				emit createTab(pTool,pTable,name);			
				if(pTable==NULL)
					return;
				pTable->setName(name);
			}
			connect(this,SIGNAL(setRow(QList<stuLog *>)),pTable,SLOT(setRow(QList<stuLog *>)),Qt::BlockingQueuedConnection);//����table��ͨ��������ʽȷ�����tableָ��
			flag=true;
		}
		if(logs.size()!=0)
			emit setRow(logs);//�����ź������
		qDeleteAll(logs);
		logs.clear();
	}while(!pTable->isClosed());
//	emit finished();
}