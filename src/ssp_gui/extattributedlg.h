#ifndef EXTATTRIBUTEDLG_H
#define EXTATTRIBUTEDLG_H

#include <QDialog>
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QSpacerItem>
#include <QTableWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QMap>
#include <QVector>
#include <QItemDelegate>
#include "gui/qt/SSvgEditorWnd.h"
#include "gui/svg/SSvgObject.h"
#include "gui/qt/SQt.h"
#include "db/SDatabaseOper.h"
#include "ssp_datawindow.h"
#include "dlg_svg_datawindow.h"
class memSSvgEditorWnd;
class SpinBoxDelegate;
class DynamicSSvgEditorWnd:public QDialog
{
	Q_OBJECT
public:
	DynamicSSvgEditorWnd(QWidget *parent=NULL);
	~DynamicSSvgEditorWnd();
	void setData(SDatabaseOper * dboper,SString swndref,char * membuf);
	void getSaveBuff(SString & sbuff);
private slots:
	void OnOkPressed();
	void OnCancelPressed();
private:
	memSSvgEditorWnd * m_memwnd;
	QPushButton* m_btn_ok;
	QPushButton* m_btn_cancel;
	QGridLayout *m_gridLayout;
	QVBoxLayout *m_verticalLayout;
	QHBoxLayout *m_horizontalLayout;
	QSpacerItem *m_horizontalSpacer;
	SString saveBuff;
	bool m_save;
};
class memSSvgEditorWnd:public SSvgEditorWnd
{
	Q_OBJECT
public:
	memSSvgEditorWnd(QWidget *parent=NULL):SSvgEditorWnd(parent){m_pSvgDataWindow = NULL;}
	virtual ~memSSvgEditorWnd(){delete m_pSvgDataWindow;};
	bool EditExtAttribute(SSvgObject *pObj,SString &sExtAttr);
	void getBbOper(SDatabaseOper *dboper){m_dboper=dboper;};
	void getWndRef(SString swndRef){m_wndRef=swndRef;};
	void save(SString & sbuff);
public slots:
	void slotDatasetCell(const QString &dsname, int row,int col);
private:
	SDatabaseOper *m_dboper;
	SString m_wndRef;
	CDlgSvgDataWindow *m_pSvgDataWindow;

};
class ExtAttributeDlg : public QDialog
{
	Q_OBJECT

public:
	ExtAttributeDlg(QWidget *parent=0);
	ExtAttributeDlg(SSvgObject *pObj,SString &sExtAttr,SDatabaseOper *dboper,SString swndRef,QWidget *parent=0);
	~ExtAttributeDlg();
private:
	void init();
	void clearAttri();
	void initAttri(SString m_attriStr,QString type);//���ݴ��������ַ������ɲ�ͬ������
	void setAsText();
	void setAsState();
	void setAsJumpFun();
	void setAsJumpWnd();
	void setAsChartPie();
	void setAsChartCol();
	void setAsChartLine();
	void setAsDynRsRect();
	void setAsFloatWnd();
	void setAsFloatView();
	void setAsGenAction();
	void setAsChartPanel();
	SString QcorToSrgb(QColor cor);//��QColorת��Ϊrgb(255,0,0)
	QColor SrgbToAttri(SString srgb);//��rgb(255,0,0)��ʽת��ΪQColor��ʽ
	SString QcorToScor(QColor cor);//��QColorת��Ϊ255,0,0
	QColor ScorToQcor(SString srgb);//��255,0,0��ʽת��ΪQColor
	QColor AttriToQcor(QString sattri);//��#ff0000ת��ΪQColor
	QString QcorToAttri(QColor cor);//��QColorת��Ϊ#ff0000��ʽ
	SString getAttriStr();//���ݽ����������������ַ���
	void initTree1();
	void initTree2();
	void initAttri2(SString m_attriStr,QString act);//���ݴ��������ַ������ɲ�ͬ������
	void clearAttri2();
	void initTree3();
	void initAttri3(SString m_attriStr,QString act);//���ݴ��������ַ������ɲ�ͬ������
	void clearAttri3();
	int flashStrToInt(QString str);
	QString flashIntToStr(int num);
private slots:
	void OnOk();
	void OnCalcel();
	void OnChangeMap(int index);
	void OnTreeItemClicked(QTreeWidgetItem* item,int column);
	void OnTreeCustomContextMenuRequested(const QPoint& pos);
	void OnTreeItemChanged(QTreeWidgetItem* item,int column);

	void OnTree2ItemClicked(QTreeWidgetItem* item,int column);
	void OnTree2CustomContextMenuRequested(const QPoint& pos);
	void OnTree3ItemClicked(QTreeWidgetItem* item,int column);
	void OnMenuTriggered(QAction* action);
	void OnChoosed(QTableWidgetItem*item);//��ѡ�к���

	void onTree1CboChanged(QString type);
	void onTree2CboChanged(QString act);
private:
	SpinBoxDelegate *m_delegate;//�������
	QItemDelegate * m_stddelegate;//ԭʼ����
	SpinBoxDelegate *m_delegate2;//�������
	QItemDelegate * m_stddelegate2;//ԭʼ����
	SpinBoxDelegate *m_delegate3;//�������
	QItemDelegate * m_stddelegate3;//ԭʼ����
	SString m_wndRef;//��̬������������
	QDialog * m_choosedlg;
	SDatabaseOper *m_dboper;
	QPushButton* m_btn_ok;
	QPushButton* m_btn_cancel;
	QTreeWidget* m_tree;
	QTreeWidget* m_tree2;
	QTreeWidget* m_tree3;
	QTabWidget *m_tab;
	QGridLayout *m_gridLayout;
	QVBoxLayout *m_verticalLayout;
	QHBoxLayout *m_horizontalLayout;
	QSpacerItem *m_horizontalSpacer;
	SString m_attriStr;//�����ַ���
	SString * m_pattriStr;//�����ַ���ָ��
	SSvgObject * m_svgObject;
	QMenu * m_menu;
	QTreeWidgetItem * m_citem;
	int m_fcext,m_lcext;//fc��lc��չ
	SString m_stype,m_sds,m_srow,m_scol,m_sdime,m_sfc,m_slc,m_sto,m_suserdraw,m_scond,m_stcor,m_slcor,m_stitle,\
		m_scolcount,m_sheight,m_swidth,m_sact,m_salpha,m_sview,m_stitle1,m_flash,m_fds,m_frow,m_fcol,m_pRow,m_pCol,m_pStart,m_pEnd,m_pCor,m_pbakCor;
	QMap<QString,QString>m_typeMap, m_nameMap,m_showMap,m_actMap;
	QVector<SString> m_vfcext;//fc��չ
	QVector<SString> m_vlcext;//lc��չ
	QVector<SString> m_vpcorext;//p_cor��չ
	QVector<SString> m_vpcorcntext;//p_corcnt��չ
	QStringList m_retainElement;
};
class RefData//��ȡ��������
{
public:
	static void getPosData(SString refName,SRecordset &rs);//��ȡ�������ݼ�
	static void getCfgWndNo(SDatabaseOper *dboper,SRecordset &rs);//��ȡ��̬���ڱ��
	static void getFunPoint(SDatabaseOper *dboper,SRecordset &rs);//��ȡ���ܵ����ݼ�
	static void getGenAction(SDatabaseOper *dboper,SRecordset &rs);//��ȡͨ�ö�������
	static void getRefData(SDatabaseOper *dboper,SRecordset &rs,int type=0,QString cls="");//��ȡ�����������ݼ�//typr����̬��������
};


class SpinBoxDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	SpinBoxDelegate(QObject *parent = 0);
	~SpinBoxDelegate()
	{

	}
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
		const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
#endif // EXTATTRIBUTEDLG_H
