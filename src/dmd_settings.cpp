#include "dmd_settings.h"
#include <QFileDialog>

/****************************
 *
 * DM_Settings_SearchPath
 *
 ***************************/

class QListView_DMS: public QListView
{
public:
	QListView_DMS(QWidget *parent) : QListView(parent) {}
	
	virtual QSize sizeHint () const
	{
		return QSize(QListView::sizeHint().width(), 40);
	}
};


DMD_Settings::DMD_Settings(const DM_Settings &settings, QWidget *parent) :
	QDialog(parent), settings_(settings)
{
	createWidgets();

	setWindowTitle(tr("Settings"));
}

void DMD_Settings::createWidgets()
{
	// WIDGETS

	// search path
	QLabel *lb_searchpaths = new QLabel(tr("&Search paths:"), this);
	searchpaths_ = new QListView_DMS(this);

	DM_Settings_SearchPath_List_Model *searchpathsmodel = new DM_Settings_SearchPath_List_Model(this);
	searchpathsmodel->setList(&settings_.searchPaths());
	searchpaths_->setModel(searchpathsmodel);

	lb_searchpaths->setBuddy(searchpaths_);

	// import path
	QLabel *lb_importpaths = new QLabel(tr("&Import paths:"), this);
	importpaths_ = new QListView_DMS(this);

	DM_Settings_SearchPath_List_Model *importpathsmodel = new DM_Settings_SearchPath_List_Model(this);
	importpathsmodel->setList(&settings_.importPaths());
	importpaths_->setModel(importpathsmodel);

	lb_importpaths->setBuddy(searchpaths_);


	QLabel *lb_importfilters = new QLabel(tr("&Import filters (';' separated):"), this);
	importfilters_ = new QLineEdit(this);
	importfilters_->setText(settings_.importFilters());
	lb_importfilters->setBuddy(importfilters_);

	// output path
	QLabel *lb_outputpaths = new QLabel(tr("Ou&tput paths:"), this);
	outputpaths_ = new QListView_DMS(this);

	DM_Settings_SearchPath_List_Model *outputpathsmodel = new DM_Settings_SearchPath_List_Model(this);
	outputpathsmodel->setList(&settings_.outputPaths());
	outputpaths_->setModel(outputpathsmodel);

	lb_outputpaths->setBuddy(outputpaths_);


	bt_ok_ = new QPushButton(tr("&Ok"), this);
	bt_ok_->setDefault(true);
	bt_cancel_ = new QPushButton(tr("&Cancel"), this);

	bt_path_add_ = new QPushButton(tr("&Add"), this);
	bt_path_del_ = new QPushButton(tr("&Remove"), this);

	bt_import_add_ = new QPushButton(tr("&Add"), this);
	bt_import_del_ = new QPushButton(tr("&Remove"), this);

	bt_output_add_ = new QPushButton(tr("&Add"), this);
	bt_output_del_ = new QPushButton(tr("&Remove"), this);

	// SIGNALS
	connect(bt_ok_, SIGNAL(clicked()), this, SLOT(bt_ok_clicked()));
    connect(bt_cancel_, SIGNAL(clicked()), this, SLOT(reject()));

	connect(bt_path_add_, SIGNAL(clicked()), this, SLOT(bt_path_add_clicked()));
	connect(bt_path_del_, SIGNAL(clicked()), this, SLOT(bt_path_del_clicked()));
	connect(bt_import_add_, SIGNAL(clicked()), this, SLOT(bt_import_add_clicked()));
	connect(bt_import_del_, SIGNAL(clicked()), this, SLOT(bt_import_del_clicked()));
	connect(bt_output_add_, SIGNAL(clicked()), this, SLOT(bt_output_add_clicked()));
	connect(bt_output_del_, SIGNAL(clicked()), this, SLOT(bt_output_del_clicked()));

	// LAYOUTS
	QVBoxLayout *lay_main = new QVBoxLayout;

	// search path
	QHBoxLayout *lay_searchpath = new QHBoxLayout;

	QVBoxLayout *lay_searchpathbody = new QVBoxLayout;
	lay_searchpathbody->addWidget(lb_searchpaths);
	lay_searchpathbody->addWidget(searchpaths_);

	QVBoxLayout *lay_searchpathbuttons = new QVBoxLayout;
	lay_searchpathbuttons->addStretch();
	lay_searchpathbuttons->addWidget(bt_path_add_);
	lay_searchpathbuttons->addWidget(bt_path_del_);
	lay_searchpathbuttons->addStretch();

	lay_searchpath->addLayout(lay_searchpathbody);
	lay_searchpath->addLayout(lay_searchpathbuttons);

	// import path
	QHBoxLayout *lay_importpath = new QHBoxLayout;

	QVBoxLayout *lay_importpathbody = new QVBoxLayout;
	lay_importpathbody->addWidget(lb_importpaths);
	lay_importpathbody->addWidget(importpaths_);
	lay_importpathbody->addWidget(lb_importfilters);
	lay_importpathbody->addWidget(importfilters_);

	QVBoxLayout *lay_importpathbuttons = new QVBoxLayout;
	lay_importpathbuttons->addStretch();
	lay_importpathbuttons->addWidget(bt_import_add_);
	lay_importpathbuttons->addWidget(bt_import_del_);
	lay_importpathbuttons->addStretch();

	lay_importpath->addLayout(lay_importpathbody);
	lay_importpath->addLayout(lay_importpathbuttons);

	// output path
	QHBoxLayout *lay_outputpath = new QHBoxLayout;

	QVBoxLayout *lay_outputpathbody = new QVBoxLayout;
	lay_outputpathbody->addWidget(lb_outputpaths);
	lay_outputpathbody->addWidget(outputpaths_);

	QVBoxLayout *lay_outputpathbuttons = new QVBoxLayout;
	lay_outputpathbuttons->addStretch();
	lay_outputpathbuttons->addWidget(bt_output_add_);
	lay_outputpathbuttons->addWidget(bt_output_del_);
	lay_outputpathbuttons->addStretch();

	lay_outputpath->addLayout(lay_outputpathbody);
	lay_outputpath->addLayout(lay_outputpathbuttons);

	// buttons
	QHBoxLayout *lay_buttons = new QHBoxLayout;
	lay_buttons->addStretch();
	lay_buttons->addWidget(bt_ok_);
	lay_buttons->addWidget(bt_cancel_);

	// main
	lay_main->addLayout(lay_searchpath);
	lay_main->addLayout(lay_importpath);
	lay_main->addLayout(lay_outputpath);
	lay_main->addLayout(lay_buttons);

	setLayout(lay_main);
}

void DMD_Settings::bt_ok_clicked()
{
	settings_.setImportFilters(importfilters_->text());
	accept();
}

void DMD_Settings::bt_path_add_clicked()
{
	QString newDir = QFileDialog::getExistingDirectory(this, tr("Add path"));
	if (!newDir.isEmpty()) settings_.searchPaths().add(QDir::toNativeSeparators(newDir));
}

void DMD_Settings::bt_path_del_clicked()
{
	settings_.searchPaths().remove(searchpaths_->currentIndex().row());
}

void DMD_Settings::bt_import_add_clicked()
{
	QString newDir = QFileDialog::getExistingDirectory(this, tr("Add import path"));
	if (!newDir.isEmpty()) settings_.importPaths().add(QDir::toNativeSeparators(newDir));
}

void DMD_Settings::bt_import_del_clicked()
{
	settings_.importPaths().remove(importpaths_->currentIndex().row());
}

void DMD_Settings::bt_output_add_clicked()
{
	QString newDir = QFileDialog::getExistingDirectory(this, tr("Add output path"));
	if (!newDir.isEmpty()) settings_.outputPaths().add(QDir::toNativeSeparators(newDir));
}

void DMD_Settings::bt_output_del_clicked()
{
	settings_.outputPaths().remove(outputpaths_->currentIndex().row());
}
