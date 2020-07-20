/**
 *
 * �� �� �� : mlistwidget.h
 * �������� : 2015-7-30 15:06
 * ��    �� : �ұ���
 * �޸����� : 2015-7-30 15:06
 * ��ǰ�汾 : 1.0
 * �������� : ��дQListWidget
 * �޸ļ�¼ : 
 *            $Log: $
 *
 * Ver  Date        Author  Comments
 * ---  ----------  ------  -------------------------------------------
 * 001	2015-7-30 	�ұ�������дQListWidget
 *
 **/

#ifndef MLISTWIDGET_H
#define MLISTWIDGET_H

#include "SObject.h"
#include <QListWidget>

class mListWidget : public QListWidget
{
	Q_OBJECT

public:
	mListWidget(QWidget *parent=0);
	virtual ~mListWidget();
signals:
	void choosed(QListWidgetItem*item);
public slots:
	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ��������ۺ�������choosed�ź�
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-7-30 15:08
	// ����˵��:  @
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void OnItemClicked(QListWidgetItem*item);

	//////////////////////////////////////////////////////////////////////////
	// ��    ��:  ѡ�����仯�ۺ�������choosed�ź�
	// ��    ��:  �ұ���
	// ����ʱ��:  2015-7-30 15:09
	// ����˵��:  @
	// �� �� ֵ:  
	//////////////////////////////////////////////////////////////////////////
	void OnCurrentItemChanged(QListWidgetItem * current, QListWidgetItem * previous);
private:
	
};

#endif // MLISTWIDGET_H
