#include "dmsettings.h"
#include <QSettings>
#include <QStringList>

/****************************
 *
 * DM_Settings_SearchPath
 *
 ***************************/

/****************************
 *
 * DM_Settings_SearchPath_List
 *
 ***************************/
void DM_Settings_SearchPath_List::add(const QString &path)
{
	if (!exists(path))
	{
		list_.push_back(QSharedPointer<DM_Settings_SearchPath>(new DM_Settings_SearchPath(path)));
		emit changed();
	}
}

bool DM_Settings_SearchPath_List::exists(const QString &path)
{
	foreach(QSharedPointer<DM_Settings_SearchPath> sp, list_)
		if (sp->path()==path) return true;
	return false;
}

/****************************
 *
 * DM_Settings_SearchPath_List_Model
 *
 ***************************/
DM_Settings_SearchPath_List_Model::DM_Settings_SearchPath_List_Model(QObject *parent) :
	QAbstractListModel(parent), list_(NULL)
{

}


void DM_Settings_SearchPath_List_Model::setList(DM_Settings_SearchPath_List *list) 
{ 
	list_=list; 
	connect(list_, SIGNAL(changed()), this, SLOT(listChanged()));
}

Qt::ItemFlags DM_Settings_SearchPath_List_Model::flags ( const QModelIndex & index ) const
{
	return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

int DM_Settings_SearchPath_List_Model::rowCount(const QModelIndex &parent) const
{
	if (list_)
		return list_->size();
	return 0;
}

QVariant DM_Settings_SearchPath_List_Model::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (role == Qt::DisplayRole)
	{
		return (*list_)[index.row()].path();
	}
	//return QAbstractListModel::data(index, role);
	return QVariant();
}

QVariant DM_Settings_SearchPath_List_Model::headerData(int section, Qt::Orientation orientation,
                    int role) const
{
	return QAbstractListModel::headerData(section, orientation, role);
}

void DM_Settings_SearchPath_List_Model::listChanged()
{
	reset();
}


/****************************
 *
 * DM_Settings
 *
 ***************************/
DM_Settings::DM_Settings() : searchpaths_(), importpaths_(), outputpaths_(), importfilters_()
{

}

void DM_Settings::load()
{
	QSettings s("SIB IT", "DownloadMaid");

	importfilters_=s.value("importfilters").toString();
	searchpaths_.clear();
	importpaths_.clear();
	QStringList l;
	l = s.value("searchpaths").toStringList();
	foreach (QString s, l)
		searchpaths_.add(s);
	l = s.value("importpaths").toStringList();
	foreach (QString s, l)
		importpaths_.add(s);
	l = s.value("outputpaths").toStringList();
	foreach (QString s, l)
		outputpaths_.add(s);
}

void DM_Settings::save()
{
	QSettings s("SIB IT", "DownloadMaid");
	s.setValue("importfilters", importfilters_);
	QStringList l;
	foreach (QSharedPointer<DM_Settings_SearchPath> s, searchpaths_.list_)
		l.push_back(s->path());
	s.setValue("searchpaths", l);
	l.clear();
	foreach (QSharedPointer<DM_Settings_SearchPath> s, importpaths_.list_)
		l.push_back(s->path());
	s.setValue("importpaths", l);
	l.clear();
	foreach (QSharedPointer<DM_Settings_SearchPath> s, outputpaths_.list_)
		l.push_back(s->path());
	s.setValue("outputpaths", l);
}
