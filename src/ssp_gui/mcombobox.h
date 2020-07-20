/**
 *
 * �� �� �� : mcombobox.h
 * �������� : 2015-7-30 15:01
 * ��    �� : �ұ���
 * �޸����� : 2015-7-30 15:01
 * ��ǰ�汾 : 1.0
 * �������� : 
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30 	�ұ�������дQCombobox�ؼ�
 *
 **/

#ifndef MCOMBOBOX_H
#define MCOMBOBOX_H
#include "SObject.h"
#include <QComboBox>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
//////////////////////////////////////////////////////////////////////////
// ��    ��:  mCombobox
// ��    ��:  �ұ���
// ����ʱ��:  2015-7-30 15:02
// ��    ��:  ��дQCombobox�ؼ�����Ӳ��ֹ���
//////////////////////////////////////////////////////////////////////////
class mCombobox : public QComboBox
{
	Q_OBJECT

public:
	mCombobox(QWidget *parent=0,int type=0);
	virtual ~mCombobox();

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ������tablewidget�е��к���
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-7-30 15:02
	// ����˵��:  @row ��
	//         :  @column ��
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void setRowAndColumn(QTableWidgetItem *item,int column)//������tablewidget�е��к���
	{
		m_tableItem = item;
		m_column = column;
	}
	void setTreeItemAndColumn(QTreeWidgetItem*item,int column)
	{
		m_treeItem=item;
		m_column=column;
	}
signals:
	void whitch(QTableWidgetItem *,int);
	void whitch(QTreeWidgetItem*,int);
public slots:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ѡ��仯ʱ�����ۺ���(Qtablewidget��)
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-7-30 15:04
	// ����˵��:  @index
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void changeToWidget(int index);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ѡ��仯ʱ�����ۺ�����QTreeWidget�У�
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-7-30 15:04
	// ����˵��:  @index
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void changeToTree(int index);
private:
	int m_row;//��tablewidget�е���
	int m_column;//��tablwidget�е���
	int m_type;//���ͣ�0:��tablewidget�� 1����treewidget��
	QTreeWidgetItem * m_treeItem;
	QTableWidgetItem* m_tableItem;
};

#endif // MCOMBOBOX_H
