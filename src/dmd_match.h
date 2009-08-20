#ifndef DMD_MATCH_H
#define DMD_MATCH_H

#include <QtGui>
#include "dmprocess.h"

class DMD_Match : public QDialog
{
	Q_OBJECT

public:
	DMD_Match(const DM_Process_Match_List &list, QWidget *parent = 0);

	const DM_Process_Match_List &list() { return list_; }
private slots:
	void bt_ok_clicked();
	void showContextMenu(const QPoint &position);
	void contextMenuSelected(QAction *action);
private:
	void buildContextMenu(DM_Process_Match *pmatch, const QPoint &position);

	void createWidgets();
	void populateWidgets();
	
	DM_Process_Match_List list_;
	QTreeView *matches_;
	QPushButton *bt_ok_, *bt_cancel_;
	QMenu *matches_menu_;
};


#endif // DMD_MATCH_H
