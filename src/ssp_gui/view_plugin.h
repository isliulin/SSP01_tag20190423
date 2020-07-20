#pragma once

#include "ssp_gui.h"
#include "ssp_plugin.h"
#include <QVector>

namespace Ui
{
	class ViewPluginClass;
}


class QStandardItemModel;
class ViewPlugin : public CBaseView
{
	Q_OBJECT

	enum TreeType
	{
		Group = 1,
		Module,					// ģ��
		ModuleFunPointSupport,	// ģ�鹦��
	};

public:
	ViewPlugin(QWidget *parent = 0);
	virtual ~ViewPlugin();

protected slots:
	void onQuery();
	void onCheckedShowFunPointChanged(int state);

protected:
	Ui::ViewPluginClass *ui;
	QStandardItemModel *m_model;
	QVector<QStandardItem *> m_funPointItemList;
};