#ifndef DMD_SETTINGS_H
#define DMD_SETTINGS_H

#include <QtGui>
#include "dmsettings.h"

class DMD_Settings : public QDialog
{
	Q_OBJECT

public:
	DMD_Settings(const DM_Settings &settings, QWidget *parent = 0);

	const DM_Settings &settings() { return settings_; }
private slots:
	void bt_ok_clicked();
	void bt_path_add_clicked();
	void bt_path_del_clicked();
	void bt_import_add_clicked();
	void bt_import_del_clicked();
	void bt_output_add_clicked();
	void bt_output_del_clicked();
private:
	void createWidgets();
	
	DM_Settings settings_;
	QListView *searchpaths_, *importpaths_, *outputpaths_;
	QPushButton *bt_ok_, *bt_cancel_;
	QPushButton *bt_path_add_, *bt_path_del_;
	QPushButton *bt_import_add_, *bt_import_del_;
	QPushButton *bt_output_add_, *bt_output_del_;
	QLineEdit *importfilters_;
};

#endif // DMD_SETTINGS_H
