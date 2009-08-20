#include "dmprocess.h"
#include <QtDebug>
#include "stringdistance.h"
#include <QtAlgorithms>

/****************************
 *
 * DM_Process_Path
 *
 ***************************/

QString DM_Process_Path::fullPath()
{
	QDir dir(rootpath_+QDir::separator()+path_);
	return dir.toNativeSeparators(dir.canonicalPath());
}

QString DM_Process_Path::displayPath()
{
	if (!path_.isEmpty()) 
		return path_;
	return rootpath_;
}

void DM_Process_Path::load()
{
	size_=0;
	QDir dir(fullPath(), "", QDir::Name, QDir::Files);
	foreach(QFileInfo fi, dir.entryInfoList())
	{
		size_+=fi.size();
		files_.push_back(fi.fileName());
	}
}

double DM_Process_Path::calculateScore(const QString &match)
{
	if (path_.isEmpty())
		return DM_Process_Path::MATCH_OUTPUTFOLDER_SCORE;
	
	//LevenshteinDistance d;
	HammingDistance d;

	double res=d(match, path_);
	foreach (QString s, files_)
		res+=d(match, s);

	res=res / (double)(files_.size()+1);
	return res;
}


/****************************
 *
 * DM_Process
 *
 ***************************/
void DM_Process::load()
{
	QDir dir;
	for (int i=0; i<settings_->searchPaths().size(); i++)
		loadPath(settings_->searchPaths()[i].path());

	for (int i=0; i<settings_->searchPaths().size(); i++)
	{
		list_.push_back(QSharedPointer<DM_Process_Path>(new DM_Process_Path(settings_->searchPaths()[i].path(), "")));
	}
	for (int i=0; i<settings_->outputPaths().size(); i++)
	{
		list_.push_back(QSharedPointer<DM_Process_Path>(new DM_Process_Path(settings_->outputPaths()[i].path(), "")));
	}
}

void DM_Process::loadPath(const QString &path)
{
	QDir dir(path, "", QDir::Name, QDir::Dirs);
	foreach(QString sd, dir.entryList())
		if (sd!="." && sd!="..")
			list_.push_back(QSharedPointer<DM_Process_Path>(new DM_Process_Path(dir.toNativeSeparators(dir.canonicalPath()), sd)));
			//qDebug() << dir.toNativeSeparators(dir.canonicalPath() + dir.separator() + sd);
}

/****************************
 *
 * DM_Process_Model
 *
 ***************************/
DM_Process_Model::DM_Process_Model(QObject *parent) :
	QAbstractTableModel(parent), process_(NULL)
{

}

void DM_Process_Model::setProcess(DM_Process *process)
{
	process_=process;
}

Qt::ItemFlags DM_Process_Model::flags ( const QModelIndex & index ) const
{
	return Qt::ItemIsSelectable|Qt::ItemIsEnabled;
}

int DM_Process_Model::columnCount(const QModelIndex &parent) const
{
	return 3;
}

int DM_Process_Model::rowCount(const QModelIndex &parent) const
{
	if (parent!=QModelIndex())
		return 0;
	
	if (process_)
		return process_->size();
	return 0;
}

QVariant DM_Process_Model::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
     if (index.row() >= process_->size())
         return QVariant();

	if (role == Qt::DisplayRole)
	{
		DM_Process_Path *p=&(*process_)[index.row()];
		switch (index.column())
		{
		case 0: return p->path();
		case 1: return bytes_format(p->size());
		case 2: return p->rootpath();
		}
	} else if (role == Qt::TextAlignmentRole) {
		if (index.column()==1) return Qt::AlignRight;
/*
	} else if (role == Qt::SizeHintRole) {
		switch (index.column())
		{
		case 0: return 60;
		case 1: return 10;
		case 2: return 30;
		}
*/
	}
	return QVariant();
}

QVariant DM_Process_Model::headerData(int section, Qt::Orientation orientation,
                    int role) const
{
/*
	if (orientation != Qt::Horizontal)
		return QVariant();
*/
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		switch (section)
		{
		case 0: return "Path";
		case 1: return "Size";
		case 2: return "Root path";
		}
	}
/*
	else if (role == Qt::SizeHintRole)
	{
		QSize size = qvariant_cast<QSize>(QAbstractTableModel::headerData(section, orientation, role));
		//size.setHeight(15);
		size.setWidth(600);
		return size;
		switch (section)
		{
		case 0: return QSize(360, 20);
		case 1: return QSize(10, 20);
		case 2: return QSize(30, 20);
		}
	}
*/
	return QAbstractTableModel::headerData(section, orientation, role);
}

QString DM_Process_Model::bytes_format(qlonglong bytes)
{
    const quint64 kb = 1024;
    const quint64 mb = 1024 * kb;
    const quint64 gb = 1024 * mb;
    const quint64 tb = 1024 * gb;

/*
	if (bytes >= tb)
        return QLocale().toString((double)bytes / tb, 'f', 2) + QString(QLatin1String(" TB"));
    if (bytes >= gb)
        return QLocale().toString((double)bytes / gb, 'f', 2) + QString(QLatin1String(" GB"));
    if (bytes >= mb)
*/
        return QLocale().toString((double)bytes / mb, 'f', 2) + QString(QLatin1String(" MB"));
/*
	if (bytes >= kb)
        return QLocale().toString((double)bytes / kb, 'f', 2) + QString(QLatin1String(" KB"));
    return QLocale().toString(bytes) + QString(QLatin1String(" bytes"));
*/
}

/****************************
 *
 * DM_Process_Match
 *
 ***************************/
void DM_Process_Match::execute()
{
	double score;

	for (int i=0; i<process_->size(); i++)
	{
		score=(*process_)[i].calculateScore(fileName_);

		//qDebug() << fileName_ << "-" << (*process_)[i].path() << " = " << score;
		list_.push_back(QSharedPointer<DM_Process_Match_Item>(new DM_Process_Match_Item(&(*process_)[i], score)));
	}
	autoSelect();
}

bool DM_Process_Match_caseInsensitiveLessThan(const QSharedPointer<DM_Process_Match_Item> &s1, const QSharedPointer<DM_Process_Match_Item> &s2)
{
	return s1->score() < s2->score();
}


void DM_Process_Match::autoSelect()
{
	// sort by score
	qSort(list_.begin(), list_.end(), DM_Process_Match_caseInsensitiveLessThan);

	selected_=NULL;
	foreach(QSharedPointer<DM_Process_Match_Item> i, list_)
		if (i->score() < DM_Process_Path::MATCH_MINIMUM_SCORE && (!selected_ || selected_->score() > i->score()))
			selected_=&(*i);

}

/****************************
 *
 * DM_Process_Match_List
 *
 ***************************/
void DM_Process_Match_List::add(const QString &filepath, const QString &filename)
{
	list_.push_back(QSharedPointer<DM_Process_Match>(new DM_Process_Match(process_, filepath, filename)));
}

void DM_Process_Match_List::add(const QString &fullFilename)
{
	QFileInfo fi(fullFilename);
	add(fi.filePath(), fi.fileName());
}

void DM_Process_Match_List::addPath(const QString &path, const QString &filters)
{
	QStringList l=filters.split(";");
	addPath(path, l);
}

void DM_Process_Match_List::addPath(const QString &path, const QStringList &filters)
{
	QDir d(path);
	foreach(QFileInfo i, d.entryInfoList(filters, QDir::AllDirs|QDir::NoDotAndDotDot|QDir::AllEntries))
	{
		if (i.isDir())
		{
			addPath(i.absoluteFilePath(), filters);
		}
		else
		{
			add(path, i.fileName());
		}
	}
}


void DM_Process_Match_List::import()
{
	QStringList l=process_->settings().importFilters().split(";");

	for (int i=0; i<process_->settings().importPaths().size(); i++)
	{
		addPath(process_->settings().importPaths()[i].path(), l);
	}
}

bool DM_Process_Match_List::execute()
{
	bool ret=true;
	QFileInfo srcfile, dstfile;
	foreach( QSharedPointer<DM_Process_Match> i, list_)
	{
		if (i->selected())
		{
			srcfile.setFile(i->filePath(), i->fileName());
			dstfile.setFile(i->selected()->path()->fullPath(), i->fileName());

			//qDebug() << srcfile.absoluteFilePath() << " -> " << dstfile.absoluteFilePath();
			QDir d;

			if (!d.rename(srcfile.absoluteFilePath(), dstfile.absoluteFilePath()))
			{
				if (QFile::copy(srcfile.absoluteFilePath(), dstfile.absoluteFilePath()))
				{
					d.remove(srcfile.absoluteFilePath());
				}
				else
				{
					qDebug() << "Failed: " << srcfile.absoluteFilePath() << " -> " << dstfile.absoluteFilePath();
					ret=false;
				}
			}
		}
	}
	return ret;
}

/****************************
 *
 * DM_Process_Match_List_Model
 *
 ***************************/
DM_Process_Match_List_Model::DM_Process_Match_List_Model(QObject *parent) :
	QAbstractTableModel(parent), list_(NULL)
{

}

void DM_Process_Match_List_Model::setProcessMatchList(DM_Process_Match_List *list)
{
	list_=list;
}

Qt::ItemFlags DM_Process_Match_List_Model::flags ( const QModelIndex & index ) const
{
	Qt::ItemFlags ret=Qt::ItemIsSelectable|Qt::ItemIsEnabled;
/*
	if (index.column()==2) 
		ret|=Qt::ItemIsEditable;
*/
	return ret;
}

int DM_Process_Match_List_Model::columnCount(const QModelIndex &parent) const
{
	return 4;
}

int DM_Process_Match_List_Model::rowCount(const QModelIndex &parent) const
{
	if (parent!=QModelIndex())
		return 0;
	
	if (list_)
		return list_->size();
	return 0;
}

QVariant DM_Process_Match_List_Model::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
     if (index.row() >= list_->size())
         return QVariant();

	if (role == Qt::DisplayRole)
	{
		DM_Process_Match *p=&(*list_)[index.row()];
		switch (index.column())
		{
		case 0: return p->fileName();
		case 1: return p->selected()?p->selected()->path()->displayPath():QVariant();
		case 2: return p->selected()?p->selected()->path()->rootpath():QVariant();
		case 3: return p->selected()?p->selected()->score():QVariant();
		}
	} else if (role == Qt::TextAlignmentRole) {
		if (index.column()==3) return Qt::AlignRight;
	}
	return QVariant();
}

QVariant DM_Process_Match_List_Model::headerData(int section, Qt::Orientation orientation,
                    int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		switch (section)
		{
		case 0: return "Filename";
		case 1: return "Path";
		case 2: return "Root path";
		case 3: return "Score";
		}
	} else if (role == Qt::SizeHintRole && orientation == Qt::Horizontal) {
		switch (section)
		{
		case 0: return QSize(300, -1);
		case 1: return QSize(300, -1);
		case 2: return QSize(300, -1);
		//case 3: return "Score";
		}
	}
	return QAbstractTableModel::headerData(section, orientation, role);
}

