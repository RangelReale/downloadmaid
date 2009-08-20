#ifndef DMPROCESS_H
#define DMPROCESS_H

#include <QtCore>
#include "dmsettings.h"
#include <QAbstractTableModel>

// forward declarations
class DM_Process_Path;


/****************************
 *
 * DM_Process_Path
 *
 ***************************/
class DM_Process_Path
{
public:
	static const int MATCH_MINIMUM_SCORE			= 40;
	static const int MATCH_OUTPUTFOLDER_SCORE		= 5000;

	DM_Process_Path(const QString &rootpath, const QString &path) : 
		rootpath_(rootpath), path_(path), files_(), size_(0) { load(); }

	const QString &rootpath() { return rootpath_; }
	const QString &path() { return path_; }
	QString fullPath();
	QString displayPath();
	const QStringList &files() { return files_; }
	qlonglong size() { return size_; }

	double calculateScore(const QString &match);
private:
	void load();

	QString rootpath_, path_;
	QStringList files_;
	qlonglong size_;
};

/****************************
 *
 * DM_Process
 *
 ***************************/
class DM_Process
{
public:
	typedef QVector< QSharedPointer<DM_Process_Path> > list_t;

	DM_Process(DM_Settings *settings) : settings_(settings), list_() { load(); }

	DM_Process_Path &operator[] (unsigned i) { return *list_[i]; }
	int size() { return list_.size(); }

	DM_Settings &settings() { return *settings_; }
protected:
	void load();
	void loadPath(const QString &path);
private:
	DM_Settings *settings_;
	list_t list_;
};

/****************************
 *
 * DM_Process_Model
 *
 ***************************/
class DM_Process_Model : public QAbstractTableModel
{
public:
    DM_Process_Model(QObject *parent = 0);

	void setProcess(DM_Process *process);
	virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
	virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
private:
	static QString bytes_format(qlonglong bytes);
	
	DM_Process *process_;
};

/****************************
 *
 * DM_Process_Match_Item
 *
 ***************************/
class DM_Process_Match_Item
{
public:
	DM_Process_Match_Item(DM_Process_Path *path, double score) :
	  path_(path), score_(score) { }

	DM_Process_Path *path() { return path_; }
	double score() { return score_; }
private:
	DM_Process_Path *path_;
	double score_;
};


/****************************
 *
 * DM_Process_Match
 *
 ***************************/
class DM_Process_Match
{
public:
	typedef QVector< QSharedPointer<DM_Process_Match_Item> > list_t;

	DM_Process_Match(DM_Process *process, const QString &filePath, const QString &fileName) : 
		process_(process), filePath_(filePath), fileName_(fileName), list_() { execute(); }

	DM_Process *process() { return process_; }

	int size() { return list_.size(); }
	DM_Process_Match_Item &operator[] (unsigned i) { return *list_[i]; }

	const QString &filePath() { return filePath_; }
	const QString &fileName() { return fileName_; }

	DM_Process_Match_Item *selected() { return selected_; }
	void setSelected(DM_Process_Match_Item *item) { selected_=item; }
protected:
	void execute();
	void autoSelect();
private:
	DM_Process *process_;
	DM_Process_Match_Item *selected_;
	QString filePath_, fileName_;
	list_t list_;
};

/****************************
 *
 * DM_Process_Match_List
 *
 ***************************/
class DM_Process_Match_List
{
public:
	typedef QVector< QSharedPointer<DM_Process_Match> > list_t;

	DM_Process_Match_List(DM_Process *process) : 
		process_(process), list_() { }

	DM_Process *process() { return process_; }

	int size() { return list_.size(); }
	DM_Process_Match &operator[] (unsigned i) { return *list_[i]; }

	void add(const QString &filepath, const QString &filename);
	void add(const QString &fullFilename);
	void addPath(const QString &path, const QString &filters = "");
	void addPath(const QString &path, const QStringList &filters);
	void import();

	bool execute();
private:
	DM_Process *process_;
	list_t list_;
};


/****************************
 *
 * DM_Process_Match_List_Model
 *
 ***************************/
class DM_Process_Match_List_Model : public QAbstractTableModel
{
public:
    DM_Process_Match_List_Model(QObject *parent = 0);

	void setProcessMatchList(DM_Process_Match_List *list);
	virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
	virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
private:
	DM_Process_Match_List *list_;
};

#endif // DMPROCESS_H
