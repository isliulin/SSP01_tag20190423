#ifndef VIEW_REPORTSETTING_H
#define VIEW_REPORTSETTING_H

#include <QWidget>
#include <QtCore>  
#include <QtGui>  
#include "ssp_baseview.h"
#include "ssp_database.h"
#include "SSvgWnd.h"
#include "ssp_datawindow.h"
#include "ui_view_reportsetting.h"

class MyGraphicsScene;
class MyGraphicsItem;
class settingSQL_dlg;
class settingExpress_dlg;
class settingGroup_dlg;
class pagesetting_dlg;
class txtsetting_widget;
class txtsetting_dialog;
class linesetting_dialog;
class chartsetting_dialog;
struct divStruct
{
	divStruct(){};
	divStruct(QString no,int itemNo,int height){this->no=no;this->itemNo=itemNo;this->height=height;}
	void setDiv(QString no,int itemNo,int height){this->no=no;this->itemNo=itemNo;this->height=height;}
	QString no;
	int itemNo;
	int height;
};
class view_reportsetting : public CBaseView
{
	Q_OBJECT

public:
	view_reportsetting(QWidget *parent = 0);
	~view_reportsetting();

private:
	Ui::view_reportsetting ui;
	QWidget *topWidget;
	QWidget *leftWidget;
	int dsNo;
	CSsp_DatawindowMgr *m_dataWindow;
	QGraphicsView *m_view;
	MyGraphicsScene *m_scene;
	QPushButton *newreport;
	QComboBox *selreport;
	QPushButton *chgName;//�ı䵱ǰ����
	QPushButton *delreport;
	QPushButton *delElement;//ɾ��ͼԪ��
	QPushButton *sqlElement;//����Ԫ��dql
	QPushButton *groupElement;//����Ԫ��
	QPushButton *pageSetting;//ҳ������
	SDatabaseOper *m_pOper;
	int reportCount;
	int reportNo;
	int height;
	int width;

	QPushButton *rectDraw;//����
	QPushButton *lineHDraw;//����
	QPushButton *lineVDraw;//����
	QPushButton *lineDraw;//б��
	QPushButton *textDraw;//�ı�
	QPushButton *chartDraw;//ͼ��
	QPushButton *expDraw;//���ʽ
	QPushButton *Halign;//�����
	QPushButton *Valign;//������
	QPushButton *sizeAlign;//���ô�Сһ��
	settingSQL_dlg *sql_dlg;
	settingGroup_dlg *group_dlg;
	pagesetting_dlg *page_dlg;
private slots:
	void onPushButton_new();
	void initPaint(int cont);
	void onPushButton_delreport();
	void onPushButton_delElm();
	void onPushButton_sql();
	void onPushButton_group();
	void onPushButton_page();
	void onPushButton_chgName();

	void onPushButton_rect();
	void onPushButton_lineH();
	void onPushButton_lineV();
	void onPushButton_text();
	void onPushButton_line();
	void onPushButton_exp();
	void onPushButton_chart();
	void onPushButton_Halign();
	void onPushButton_Valign();
	void onPushButton_setSize();
	void initNewPage(int reportNo);
	void changeHW(int height,int width);
};
//scene
class MyGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	enum Mode {InsertRect,InsertHLine,InsertVLine,InsertText,InsertDiv,InsertExp,InsertLine,InsertChart,MoveItem};
	void setMode(Mode mode);
	MyGraphicsScene(QObject *parent = 0);
	~MyGraphicsScene();
	void setNo(int reportNo){this->reportNo=reportNo;itemCount=0;}
	void setDsNo(CSsp_Dataset *dbSet){this->dbSet=dbSet;}
	void delSelElm();
	void setHalign();
	void setValign();
	void setSameSize();
	void initPaint(int itemNo,int type,QString pos,QString rect,QString info);
	void initNewPage(QString curName);
	void saveAllDs(CSsp_Dataset *dbSet);
	void initScreen();
	void drawDiv(QString dsName,int dsNo,QString info);
private:
	Mode myMode;
	CSsp_Dataset* dbSet;
	SDatabaseOper *m_pOper;
	bool leftButtonDown;
	QPointF startPoint;
	MyGraphicsItem *line;
	QFont myFont;
	MyGraphicsItem *textItem;
	QColor myTextColor;
	QColor myItemColor;
	QColor myLineColor;
	//QString getAtu(QString info,QString name);
	int reportNo;
	int itemCount;
	//int dsNo;
	QList<divStruct>*divList;
	QList<MyGraphicsItem *>selItems;

	void drawGraphics(QPointF pos,QRectF rect,int type,int reportNo,int itemNo,QString info);
private slots:
	void refresh();
	void divChanged(QPointF divR,int y);
	void saveAllPos();
	void moveItems(float dx,float dy);
	void changeHeight(int height);
	void selListChanged();
	void deleteGrp();
signals:
	void itemInserted(MyGraphicsItem &item);
	void changeHW(int height,int width);
	
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

	void keyPressEvent(QKeyEvent *keyEvent); 
};
///////////////////////////////////////////////////////////////////////
//item
class MyGraphicsItem : public QObject,public QGraphicsItem
{
	
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)
public:
	MyGraphicsItem();
	~MyGraphicsItem();
	SDatabaseOper *m_pOper;
	void setType(int type);
	QRectF boundingRect()const;
	QRectF rect();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QPainterPath shape()const;
	void ResetRect(QRectF rect);
	int getType(){return type;}
	void setDs(CSsp_Dataset *dbSet);
	void setFont(QFont font);
	void setTxt(QString txt);
	void setColor(QColor color);
	void setLinePx(int px){this->linePx=px;}
	void setLineType(int type){this->lineType=type;}
	void setAtu(QString atu){this->atu=atu;}

	void getNo(int &reportNo,int &itemNo){reportNo=this->reportNo;itemNo=this->itemNo;}
	void setNo(int reportNo,int itemNo){this->reportNo=reportNo;this->itemNo=itemNo;}

	void setTxtop(int txtOp){txtOption=txtOp;}
	int getTxtop(){return txtOption;}

	void setChartType(int chartType){this->chartType=chartType;}
	void setChartDs(QString chartDs){this->chartDs=chartDs;}

	void savePos();
	void setDivList(QList<divStruct>*divList);
	void moveItem(int rx,int ry);
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
private:
	
private:
	int type;//1���� 4�ı� 2���� 3���� 5�ָ��� 6���ʽ 7 б�� 8 ͼ��
	QFont m_font;//����
	QRectF m_rect;//λ��
	QColor m_color;//��ɫ
	//�ı���������
	int isrect;//�Ƿ�ӱ߿�
	int rectpx;//�߿��ϸ
	QColor rect_color;//�߿���ɫ
	//�߶�����
	int linePx;//�߿�
	int lineType;//�����ͣ�ʵ�����ߣ�
	int txtOption;//λ�ã��������ң�
	//ͼ������
	int chartType;//0 ��ͼ 1 ��ͼ 2 ����ͼ
	QString chartDs;//ͼ���Ӧ���ݼ���

	bool cScale;
	QCursor* m_cursor;
	bool cMove;
	QPointF start;//��һ��λ��
	QPointF end;//��һ��λ��
	int direction;//��λ
	QString txt;//�ı�
	QString atu;//info
	int reportNo;
	int itemNo;
	CSsp_Dataset* dbSet;
	QString exp;
	QList<divStruct>*divList;
	//QString setValue(QString info,QString name,QString value);
	settingExpress_dlg *expDlg;
	txtsetting_dialog *txtDlg;
	linesetting_dialog *lineDlg;
	chartsetting_dialog *chartDlg;
signals:
	void refresh();
	void divChanged(QPointF divR,int y);
	void saveAllPos();
	void moveItems(float x,float y);
	void changeHeight(int height);

};
////////////////////////////////////////////////////////////////////////////
//ѡ�����ݼ�
class settingSQL_dlg: public QDialog
{
	Q_OBJECT
public:
	settingSQL_dlg(QWidget *parent = 0);
	~settingSQL_dlg();
	int getNo();
	void setDsNo(int ds);
private:
	QTableWidget *tableWidget;
	QLabel *dsLabel;
	QLabel *curDsLabel;
	QPushButton *Ok;
	QPushButton *cancel;
	int dsNo;
	SDatabaseOper *m_pOper;
	
private:
	void initDlg();
	private slots:
		void onOk();
		void onCancel();
};
////////////////////////////////////////////////////////////////////////////
//���ʽ�������������ú��ı����ԣ�
class settingExpress_dlg: public QDialog
{
	Q_OBJECT
public:
	settingExpress_dlg(QString txt,QString exp,QWidget *parent = 0);
	~settingExpress_dlg();	
	void setDs(CSsp_Dataset* dbSet);

	QString getExp();
	void setExp(QString col);

	QString getFun();
	void setFun(QString fun);

	QString getTxt();
	void setTxt(QString txt);

	void setFontSize(QString size);
	QString getFontSize();

	void setFontFamily(QString family);
	QString getFontFamily();

	void setColor(QColor color);
	QColor getColor();

	void setFontType(int type);//0 normal 1 bold 2 italic 3 bolditalic
	int getFontType();

	void setTxtOp(int op);//0 center 1 left 2 right
	int getTxtOp();

	void setFontDelLine(bool delLine);
	bool getFontDelLine();

	void setFontUndLine(bool undLine);
	bool getFontUndLine();

	int getIsRect();
	void setIsRect(int isRect);
	int getRectpx();
	void setRectpx(int px);
	QColor getRectColor();
	void setRectColor(QColor color);

	void initDlg();
private:
	QTabWidget *tabWidget;
	QLabel *funLabel;
	QTableWidget *funList;
	QLabel *colLabel;
	QLabel *curColLabel;
	QLabel *curFunLabel;
	QTableWidget *colList;
	int txtop;
	QPushButton *Ok;
	QPushButton *cancel;
	QWidget *tab1;
	CSsp_Dataset* dbSet;
	QString col;//����
	QString fun;//����
	QString txt;
	QString fontSize;
	QColor color;//������ɫ
	QFont font;//����
	int isRect;//�Ƿ���ʾ����
	int rectPx;//�����߿�
	QColor rectColor;//������ɫ
	SDatabaseOper *m_pOper;
	txtsetting_widget *tab2;//�ı�����
private:
	
private slots:
	void onOk();
	void onCancel();
};
////////////////////////////////////////////////////////////////////////////
//����Ի���
class settingGroup_dlg: public QDialog
{
	Q_OBJECT
public:
	settingGroup_dlg(int dsNo,QWidget *parent = 0);
	~settingGroup_dlg();
	QString getGrp();
	void setGrp(QString grp);
	void setDs(int dsNo);

	QString getTxt();
	void setTxt(QString txt);

	void setFontSize(QString size);
	QString getFontSize();

	void setFontFamily(QString family);
	QString getFontFamily();

	void setColor(QColor color);
	QColor getColor();

	void setFontType(int type);//0 normal 1 bold 2 italic 3 bolditalic
	int getFontType();

	void setTxtOp(int op);//0 center 1 left 2 right
	int getTxtOp();

	void setFontDelLine(bool delLine);
	bool getFontDelLine();

	void setFontUndLine(bool undLine);
	bool getFontUndLine();

	int getIsRect();
	void setIsRect(int isRect);
	int getRectpx();
	void setRectpx(int px);
	QColor getRectColor();
	void setRectColor(QColor color);

	void initDlg();
private:
	QTabWidget *tabWidget;
	QWidget *tab1;
	QLabel *curColLabel;
	QLabel *colLabel;
	QTableWidget *colList;
	QPushButton *delGrp;
	QPushButton *Ok;
	QPushButton *cancel;
	int dsNo;
	QString col;
	SDatabaseOper *m_pOper;
	SPtrList<CSsp_DwColumn> *m_fieldlist;//�����ֶ���Ϣ
	CSsp_DatawindowMgr *m_dataWindow;
	txtsetting_widget *tab2;//�ı�����

	QString txt;
	QString fontSize;
	int txtop;
	QColor color;//������ɫ
	QFont font;//����
	int isRect;//�Ƿ���ʾ����
	int rectPx;//�����߿�
	QColor rectColor;//������ɫ
private slots:
	void onDelGrp();
	void onOk();
	void onCancel();
signals:
	void deleteGrp();
};
//////////////////////////////////////////////////////
//ҳ�����ã���ߣ�
class pagesetting_dlg: public QDialog
{
	Q_OBJECT
public:
	pagesetting_dlg(QWidget *parent = 0);
	~pagesetting_dlg();
	void getWH(int &width,int &height);
	void setWH(int width,int height);
	void initDlg();
private:
	int width;
	int height;
	QLabel *widthLabel;
	QLineEdit *widthLine;
	QLabel *heightLabel;
	QLineEdit *heightLine;
	QPushButton *Ok;
	QPushButton *cancel;

private slots:
	void onOk();
	void onCancel();
};
//////////////////////////////////////////////////////
//�ı����ԣ�������ɫ�߿����Եȣ�
class txtsetting_widget: public QWidget
{
	Q_OBJECT
public:
	txtsetting_widget(QWidget *parent = 0);
	~txtsetting_widget();
	QString getTxt();
	QFont getFont();
	QColor getColor();
	int getTxtOp();
	int getIsRect();
	int getRectpx();
	QColor getRectColor();

	void setTxt(QString txt);
	void setFont(QFont font);
	void setColor(QColor color);
	void setTxtOp(int txtOp);
	void setIsRect(int isRect);
	void setRectpx(int px);
	void setRectColor(QColor color);

	void setTwoTxt(bool isTwo);
private:
	bool twoTxt;

	QString txt;
	QFont font;
	QColor color;
	QColor rectColor;
	int px;

	QLabel *txtLabel;
	QLineEdit *txtLine;

	QLabel *txtLabel2;
	QLineEdit *txtLine2;

	QLabel *fontExampleLabel;
	QPushButton *fontButton;
	QPushButton *colorButton;

	QComboBox *txtOpCbo;
	QLabel *txtOpLabel;

	QCheckBox *isRect;
	QLabel *pxLabel;
	QLineEdit *pxLine;
	QPushButton *pxcolorButton;
private:
	void initDlg();
private slots:
	void onfontButton();
	void oncolorButton();
	void onisRectCheck();
	void onpxLine(QString txt);
	void onpxcolorButton();
};
//////////////////////////////////////////////////////////////////
//�ı����ô���
class txtsetting_dialog: public QDialog
{
	Q_OBJECT
public:
	txtsetting_dialog(QWidget *parent = 0);
	~txtsetting_dialog();
	QString getTxt();
	void setTxt(QString txt);

	void setFontSize(QString size);
	QString getFontSize();

	void setFontFamily(QString family);
	QString getFontFamily();

	void setColor(QColor color);
	QColor getColor();

	void setFontType(int type);//0 normal 1 bold 2 italic 3 bolditalic
	int getFontType();

	void setTxtOp(int op);//0 center 1 left 2 right
	int getTxtOp();

	void setFontDelLine(bool delLine);
	bool getFontDelLine();

	void setFontUndLine(bool undLine);
	bool getFontUndLine();

	int getIsRect();
	void setIsRect(int isRect);
	int getRectpx();
	void setRectpx(int px);
	QColor getRectColor();
	void setRectColor(QColor color);
private:
	txtsetting_widget *wig;
	QPushButton *Ok;
	QPushButton *cancel;

	QString txt;
	QString fontSize;
	int txtop;
	QColor color;//������ɫ
	QFont font;//����
	int isRect;//�Ƿ���ʾ����
	int rectPx;//�����߿�
	QColor rectColor;//������ɫ

private:
	void initDlg();
private slots:
	void onOk();
	void onCancel();
};
//////////////////////////////////////////////////////////////////
//�����ô���
class linesetting_dialog: public QDialog
{
	Q_OBJECT
public:
	linesetting_dialog(QWidget *parent = 0);
	~linesetting_dialog();
	void setColor(QColor color);
	QColor getColor();

	void setType(int type);
	int getType();

	int getpx();
	void setpx(int px);
private:

	QPushButton *Ok;
	QPushButton *cancel;
	QPushButton *colorButton;
	QLabel *pxLabel;
	QLineEdit *pxLine;
	QLabel *typeLabel;
	QComboBox *typeCbo;

	QColor color;//������ɫ
	QColor tmpColor;
	int type;//����
	int px;//�߿�

private:
	void initDlg();
private slots:
	void onOk();
	void onCancel();
	void onColorButton();
};
//////////////////////////////////////////////////////////////////
//ͼ�����ô���
class chartsetting_dialog: public QDialog
{
	Q_OBJECT
public:
	chartsetting_dialog(QWidget *parent = 0);
	~chartsetting_dialog();
	void setTxt(QString txt);
	QString getTxt();

	void setType(int type);
	int getType();

	int getDs();
	void setDs(int ds);

	void setExtAtu(QString extAtu);
	QString getExtAtu();
private:

	QPushButton *Ok;
	QPushButton *cancel;

	QTableWidget *tableWidget;
	QLabel *dsLabel;
	QLabel *curDsLabel;

	QLabel *txtLabel;
	QLineEdit *txtLine;

	QLabel *typeLabel;
	QComboBox *typeCbo;

	QLabel *extatuLabel;
	QTextEdit *extatuEdit;
	int chartDs;
	int chartType;
	QString txt;
	QString extAtu;
	SDatabaseOper *m_pOper;

private:
	void initDlg();
private slots:
	void onOk();
	void onCancel();
};
#endif // VIEW_REPORTSETTING_H
