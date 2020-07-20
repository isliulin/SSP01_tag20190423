#pragma once
#include <QObject>
#include "SApi.h"
enum  UPDATETYPE
{
	CREATE,
	DROP,
	SELECTALL
};
class Rsignal:public QObject
{
	Q_OBJECT
public:
	Rsignal(QObject *parent = 0);
	~Rsignal(void);
	virtual void sendSiganl(void *mData,bool b_select){}
	virtual void sendSiganl(void *mData,UPDATETYPE type){}
signals:
	void runsql(SString sSql,bool b_select);//����ִ�е����
	void updateTree(SString sTable,UPDATETYPE type);//����treewidget
};

class mRsignal:public Rsignal
{
	Q_OBJECT
public:
	mRsignal() {}
	~mRsignal(){}
	void sendSiganl(void *mData,bool b_select)
	{
		SString sSql = *((SString *)mData);
		emit runsql(sSql,b_select);
	}
	void sendSiganl(void *mData,UPDATETYPE type)
	{
		SString sTable = *((SString *)mData);
		emit updateTree(sTable,type);
	}
};