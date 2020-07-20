/**
 *
 * �� �� �� : mtablewidget.h
 * �������� : 2015-7-30 15:18
 * ��    �� : �ұ���
 * �޸����� : 2015-7-30 15:18
 * ��ǰ�汾 : 1.0
 * �������� : ��дQTableWidget
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30 	�ұ�������дQTableWidget
 *
 **/

#ifndef MTABLEWIDGET_H
#define MTABLEWIDGET_H

#include "SObject.h"
#include <QTableWidget>
#include <vector>
using namespace std;

class mTableWidget : public QTableWidget
{
	Q_OBJECT
public:
	enum CELLTYPE
	{
		NONE,
		TREEWIDGET,
		COMBBOX,
	};
	struct CELLINFO
	{
		CELLTYPE m_type;
		int m_column;
	};
public:
	mTableWidget(QWidget *parent=0);
	virtual ~mTableWidget();
	void setColumCellType(int column,CELLTYPE type);
	virtual void setColumnCount(int columns);
private:
	vector<CELLINFO> m_vCellInfo;
};

#endif // MTABLEWIDGET_H
