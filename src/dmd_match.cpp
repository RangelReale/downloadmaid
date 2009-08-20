#include "dmd_match.h"
#include <QMap>

/****************************
 *
 * DMD_Match
 *
 ***************************/
DMD_Match::DMD_Match(const DM_Process_Match_List &list, QWidget *parent) :
	QDialog(parent), list_(list)
{
	createWidgets();
	populateWidgets();

	setWindowTitle(tr("Process"));
}

void DMD_Match::createWidgets()
{
	// widgets
	QLabel *lb_matches = new QLabel(tr("&Matches:"), this);
	matches_ = new QTreeView(this);
	matches_->setRootIsDecorated(false);
	matches_->setMinimumWidth(800);
	matches_->setMinimumHeight(400);
	//matches_->setSelectionBehavior(QAbstractItemView::SelectItems);
	matches_->setContextMenuPolicy(Qt::CustomContextMenu);

	DM_Process_Match_List_Model *model = new DM_Process_Match_List_Model(this);
	model->setProcessMatchList(&list_);
	matches_->setModel(model);
	matches_->setColumnWidth(0, 250);
	matches_->setColumnWidth(1, 250);
	matches_->setColumnWidth(2, 150);

	lb_matches->setBuddy(matches_);

	bt_ok_ = new QPushButton(tr("&Ok"), this);
	bt_ok_->setDefault(true);
	bt_cancel_ = new QPushButton(tr("&Cancel"), this);

	matches_menu_ = new QMenu(this);

	// signals
	connect(bt_ok_, SIGNAL(clicked()), this, SLOT(bt_ok_clicked()));
    connect(bt_cancel_, SIGNAL(clicked()), this, SLOT(reject()));
	connect(matches_, SIGNAL(customContextMenuRequested(const QPoint &)),
           this, SLOT(showContextMenu(const QPoint &)));


	// layouts
	QVBoxLayout *lay_main = new QVBoxLayout;

	QVBoxLayout *lay_matches = new QVBoxLayout;
	lay_matches->addWidget(lb_matches);
	lay_matches->addWidget(matches_);

	QHBoxLayout *lay_buttons = new QHBoxLayout;
	lay_buttons->addStretch();
	lay_buttons->addWidget(bt_ok_);
	lay_buttons->addWidget(bt_cancel_);

	lay_main->addLayout(lay_matches);
	lay_main->addLayout(lay_buttons);

	setLayout(lay_main);
}

void DMD_Match::populateWidgets()
{
	QAction *n;
	QMenu *nm;
	QMap<QString, QMenu*> menumap;
	for (int i=0; i<list_.process()->size(); i++)
	{
		if ((*list_.process())[i].path().isEmpty())
		{
			nm = matches_menu_;
		} 
		else if (!menumap.contains((*list_.process())[i].rootpath()))
		{
			nm = matches_menu_->addMenu((*list_.process())[i].rootpath());
			menumap[(*list_.process())[i].rootpath()]=nm;
		} else {
			nm=menumap[(*list_.process())[i].rootpath()];
		}
		if ((*list_.process())[i].path().isEmpty())
			n = new QAction((*list_.process())[i].rootpath(), this);
		else
			n = new QAction((*list_.process())[i].path(), this);
		nm->addAction(n);
	}
/*
	for (int i=0; i<list_.process()->settings().searchPaths().size(); i++)
	{
		n = new QAction(list_.process()->settings().searchPaths()[i].path(), this);
		matches_menu_->addAction(n);
	}
	for (int i=0; i<list_.process()->settings().outputPaths().size(); i++)
	{
		n = new QAction(list_.process()->settings().outputPaths()[i].path(), this);
		matches_menu_->addAction(n);
	}
*/
}

void DMD_Match::bt_ok_clicked()
{
	QMessageBox m(this);
	m.setText(tr("Confirm the file moving operation?"));
	m.setInformativeText(tr("Files will be moved to the selected directories"));
	m.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
	m.setDefaultButton(QMessageBox::No);
	if (m.exec()==QMessageBox::Yes)
	{
		QApplication::setOverrideCursor(Qt::WaitCursor);

		bool ret=list_.execute();
		
		QApplication::restoreOverrideCursor();

		if (!ret)
			QMessageBox::warning(this, tr("Warning"), tr("Some files could not be moved. Please check the output paths and try again"));

		accept();
	}
}

void DMD_Match::showContextMenu(const QPoint &position)
{
	if (matches_->currentIndex()!=QModelIndex())
		buildContextMenu(&list_[matches_->currentIndex().row()], position);
		//matches_menu_->exec(matches_->mapToGlobal(position));

/*
	QList<QAction *> actions;
    if (view->indexAt(position).isValid()) {
        actions.append(openAction);
    }
    if (actions.count() > 0)
        QMenu::exec(actions, views->mapToGlobal(position));
*/
}

void DMD_Match::buildContextMenu(DM_Process_Match *pmatch, const QPoint &position)
{
	QMenu menu, *top, *bottom, *ofolder, *sel;
	top=menu.addMenu(tr("Best scored"));
	bottom=menu.addMenu(tr("Worst scored"));
	ofolder=menu.addMenu(tr("Output folders"));

	QAction *n;
	for (int i=0; i<pmatch->size(); i++)
	{
		if ((*pmatch)[i].score()==DM_Process_Path::MATCH_OUTPUTFOLDER_SCORE)
			sel=ofolder;
		else if ((*pmatch)[i].score()>=DM_Process_Path::MATCH_MINIMUM_SCORE)
			sel=bottom;
		else
			sel=top;
		
		if (sel!=ofolder)
			n = new QAction(QString("%1 (%2)").arg((*pmatch)[i].path()->displayPath()).arg((*pmatch)[i].score(), 0, 'f', 2), this);
		else
			n = new QAction((*pmatch)[i].path()->displayPath(), this);
		n->setData(i);
		//connect(n, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuSelected(QAction*)));

		sel->addAction(n);
	}
	n = new QAction(tr("- None -"), this);
	n->setData(-1);
	menu.addAction(n);

	connect(&menu, SIGNAL(triggered(QAction*)), this, SLOT(contextMenuSelected(QAction*)));
	menu.exec(matches_->mapToGlobal(position));
}

void DMD_Match::contextMenuSelected(QAction *action)
{
	//qDebug() << action->data();

	if (matches_->currentIndex()!=QModelIndex())
	{
		if (action->data().toInt()>=0)
			list_[matches_->currentIndex().row()].setSelected(&(list_[matches_->currentIndex().row()][action->data().toInt()]));
		else
			list_[matches_->currentIndex().row()].setSelected(NULL);
	}

}
