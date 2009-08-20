#ifndef DOWNLOADMAID_H
#define DOWNLOADMAID_H

#include <QtCore>
#include <QtGui>
#include "dmsettings.h"
#include "dmprocess.h"

class DownloadMaid : public QMainWindow
{
	Q_OBJECT

public:
	DownloadMaid(QWidget *parent = 0, Qt::WFlags flags = 0);
	~DownloadMaid();
private slots:
	void settings_clicked();
	void process_clicked();
	void refresh_clicked();
private:
	void createActions();
	void createMenus();
	void createWidgets();

	void clearProcess();
	void loadProcess();

	DM_Process *process_;
	DM_Process_Model *process_model_;
	DM_Settings settings_;

	QAction *act_settings_;
	QAction *act_process_;
	QAction *act_refresh_;
	QAction *act_exit_;

	QMenu *menu_file_;
	QMenu *menu_edit_;
	QMenu *menu_help_;

	QTreeView *list_;
};

#endif // DOWNLOADMAID_H
