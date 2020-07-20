/**
 *
 * �� �� �� : viewquit.cpp
 * �������� : 2013-9-22 13:22
 * ��    �� : �ۿ���(skt001@163.com)
 * �޸����� : $Date: $
 * ��ǰ�汾 : $Revision: $
 * �������� : �˳�ϵͳ�ļ�������
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2013-9-22	�ۿ�������ļ�
 *
 **/

#include "viewquit.h"
#include "SApplication.h"

CViewQuit::CViewQuit(QWidget *parent) : CBaseView(parent)
{
	ui.setupUi(this);
	//SetFunCode("sys.quit");

	connect(ui.pushButtonResetHmi,SIGNAL(clicked()),this,SLOT(OnResetHmi()));
	connect(ui.pushButtonQuitHmi,SIGNAL(clicked()),this,SLOT(OnQuitHmi()));
	connect(ui.pushButtonResetSys,SIGNAL(clicked()),this,SLOT(OnResetSys()));
	connect(ui.pushButtonHaltSys,SIGNAL(clicked()),this,SLOT(OnHaltSys()));


#ifdef WIN32
//	ui.pushButtonResetHmi->setEnabled(false);
	ui.pushButtonResetSys->setEnabled(false);
	ui.pushButtonHaltSys->setEnabled(false);
#endif
}

CViewQuit::~CViewQuit()
{

}

void CViewQuit::OnResetHmi()
{
	if(QMessageBox::question(NULL,"��ʾ","��ȷ��Ҫ����HMI���������?",QDialogButtonBox::Yes,QDialogButtonBox::No) != QDialogButtonBox::Yes)
		return ;
	SApplication *pApp = SApplication::GetPtr();
	if(pApp != NULL)
	{
#ifdef WIN32
		SString sLautch = "launch.exe";
#else
		SString sLautch = "launch";
#endif
		SString sBinPath = pApp->GetBinPath()+pApp->GetModule();
		SApi::SystemExec(pApp->GetBinPath()+sLautch,sLautch,sBinPath);
	}
	if(pApp != NULL)
	{
		pApp->Stop();
		pApp->Quit();
	}
	exit(0);
	abort();
}

void CViewQuit::OnQuitHmi()
{
	if(QMessageBox::question(NULL,"��ʾ","��ȷ��Ҫ�˳�HMI���������?",QDialogButtonBox::Yes,QDialogButtonBox::No) != QDialogButtonBox::Yes)
		return ;
#ifndef WIN32
	SFile::touchFile("/hmi_stop.flg");
#endif
	SApplication *pApp = SApplication::GetPtr();
	if(pApp != NULL)
	{
		pApp->Stop();
		pApp->Quit();
	}
	exit(0);
	abort();
}

void CViewQuit::OnResetSys()
{
	if(QMessageBox::question(NULL,"��ʾ","��ȷ��Ҫ��������ϵͳ��?",QDialogButtonBox::Yes,QDialogButtonBox::No) != QDialogButtonBox::Yes)
		return ;

	SApi::SystemReboot();
}

void CViewQuit::OnHaltSys()
{
	if(QMessageBox::question(NULL,"��ʾ","��ȷ��Ҫ�رղ���ϵͳ��?",QDialogButtonBox::Yes,QDialogButtonBox::No) != QDialogButtonBox::Yes)
		return ;

	SApi::SystemHalt();
}




