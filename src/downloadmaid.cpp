#include "downloadmaid.h"

#include "dmd_settings.h"
#include "dmd_match.h"
#include "dmprocess.h"
#include "downloadmaidversion.h"

DownloadMaid::DownloadMaid(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags), settings_(), process_(NULL), process_model_(NULL)
{
	setWindowTitle(QString(tr("DownloadMaid %1")).arg(DOWNLOADMAIDVERSION_STRING));;

	createActions();
	createMenus();
	createWidgets();

	settings_.load();

	loadProcess();


	setCentralWidget(list_);
}

DownloadMaid::~DownloadMaid()
{
	clearProcess();
}

void DownloadMaid::createActions()
{
	act_process_ = new QAction(tr("&Process"), this);
	connect(act_process_, SIGNAL(triggered()), this, SLOT(process_clicked()));

	act_settings_ = new QAction(tr("&Settings"), this);
	connect(act_settings_, SIGNAL(triggered()), this, SLOT(settings_clicked()));

	act_refresh_ = new QAction(tr("&Refresh"), this);
	act_refresh_->setShortcut(tr("F5"));
	connect(act_refresh_, SIGNAL(triggered()), this, SLOT(refresh_clicked()));

	act_exit_ = new QAction(tr("E&xit"), this);
	connect(act_exit_, SIGNAL(triggered()), this, SLOT(close()));
}

void DownloadMaid::createMenus()
{
	menu_file_ = menuBar()->addMenu(tr("&File"));
	menu_file_->addAction(act_process_);
	menu_file_->addAction(act_settings_);
	menu_file_->addSeparator();
	menu_file_->addAction(act_exit_);

	menu_edit_ = menuBar()->addMenu(tr("&Edit"));
	menu_edit_->addAction(act_refresh_);

	menu_help_ = menuBar()->addMenu(tr("&Help"));
}

void DownloadMaid::createWidgets()
{
	list_ = new QTreeView(this);
	list_->setRootIsDecorated(false);
}

void DownloadMaid::settings_clicked()
{
	DMD_Settings d(settings_, this);
	if (d.exec()) {
		settings_=d.settings();
		settings_.save();

		loadProcess();
	}
}

void DownloadMaid::process_clicked()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);

	DM_Process_Match_List plist(process_);
	plist.import();

	QApplication::restoreOverrideCursor();

	DMD_Match d(plist, this);
	if (d.exec()) {
		loadProcess();
	}
}

void DownloadMaid::refresh_clicked()
{
	loadProcess();
}

void DownloadMaid::clearProcess()
{
	if (process_) 
	{ 
		list_->setModel(NULL); 
		delete process_model_;
		process_model_=NULL;
		delete process_; 
		process_=NULL; 
	}

}

void DownloadMaid::loadProcess()
{
	QApplication::setOverrideCursor(Qt::WaitCursor);

	clearProcess();
	process_ = new DM_Process(&settings_);
	process_model_ = new DM_Process_Model();
	process_model_->setProcess(process_);

	list_->setModel(process_model_);
	list_->setColumnWidth(0, 400);

	QApplication::restoreOverrideCursor();
}

