#include "dlg_login_bluestyle.h"

dlg_login_blueStyle::dlg_login_blueStyle(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowModality(Qt::ApplicationModal);
//#ifdef WIN32
	setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
//#else
//#endif
	connect(ui.toolButtonOK,SIGNAL(clicked()),this,SLOT(OnOk()));
	connect(ui.toolButtonCancel,SIGNAL(clicked()),this,SLOT(OnCancel()));
	connect(ui.lineEditName,SIGNAL(textChanged(QString)),this,SLOT(showErrorLabel(QString)));
	connect(ui.lineEditPwd,SIGNAL(textChanged(QString)),this,SLOT(showErrorLabel(QString)));
	m_bOk = false;

	//���ù��ڴ���ΪԲ��

	QBitmap bmp(this->size());

	bmp.fill();

	QPainter p(&bmp);

	p.setPen(Qt::NoPen);

	p.setBrush(Qt::black);

	p.drawRoundedRect(bmp.rect(),10,10);

	setMask(bmp);

	ui.lineEditName->setFocus();
	ui.labelError->setHidden(true);

	ui.toolButtonOK->setFocus(); //����Ĭ�Ͻ���

	ui.toolButtonOK->setShortcut( QKeySequence::InsertParagraphSeparator ); //���ÿ�ݼ�Ϊ���̵ġ��س�����

	ui.toolButtonOK->setShortcut(Qt::Key_Enter); //���ÿ�ݼ�Ϊenter��

	ui.toolButtonOK->setShortcut(Qt::Key_Return); //���ÿ�ݼ�ΪС�����ϵ�enter��

	//linux�ϱ�����û�����أ����ô��ڱ���Ϊ��¼
	setWindowTitle("Login");
}

dlg_login_blueStyle::~dlg_login_blueStyle()
{

}

void dlg_login_blueStyle::OnOk()
{
	m_sUser = ui.lineEditName->text().toStdString().data();
	SString pwd = ui.lineEditPwd->text().toStdString().data();
	SString sql;
	sql.sprintf("select usr_code,pwd from t_ssp_user where usr_code='%s' ",m_sUser.data());
	SRecordset rs;
	DB->Retrieve(sql,rs);
	if(rs.GetRows() <= 0)
	{
		//SQt::ShowErrorBox(tr("����"),tr("ָ�����û���������!"));
		ui.labelError->setHidden(false);
		return ;
	}
	if(rs.GetValue(0,0) != m_sUser || rs.GetValue(0,1) != pwd)
	{
		//SQt::ShowErrorBox(tr("����"),tr("��¼�������!"));
		ui.labelError->setHidden(false);
		return ;
	}
	m_bOk = true;
	accept();
}

void dlg_login_blueStyle::OnCancel()
{
	m_bOk = false;
	reject();
}

void dlg_login_blueStyle::showErrorLabel( QString text )
{
	ui.labelError->setHidden(true);
}
