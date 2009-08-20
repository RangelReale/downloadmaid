#ifndef DMSETTINGS_H
#define DMSETTINGS_H

#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QAbstractListModel>

// forward declarations
class DM_Settings;


/****************************
 *
 * DM_Settings_SearchPath
 *
 ***************************/
class DM_Settings_SearchPath : public QObject
{
	Q_OBJECT

public:
	DM_Settings_SearchPath(const QString &path) : QObject(), path_(path) {}

	const QString &path() { return path_; }
	void setPath(const QString &path) { path_=path; emit changed(); }
signals:
	void changed();
private:
	QString path_;
};

/****************************
 *
 * DM_Settings_SearchPath_List
 *
 ***************************/
class DM_Settings_SearchPath_List : public QObject
{
	Q_OBJECT

public:
	typedef QVector< QSharedPointer<DM_Settings_SearchPath> > listdef_t;
	//typedef int listdef_t;

	DM_Settings_SearchPath_List() : QObject(), list_() { }
	DM_Settings_SearchPath_List(const DM_Settings_SearchPath_List &other) { list_=other.list_; }
    DM_Settings_SearchPath_List & operator = (const DM_Settings_SearchPath_List & other)
    {
        if (this != &other) // protect against invalid self-assignment
        {
			list_=other.list_;
        }
        // by convention, always return *this
        return *this;
    }
	DM_Settings_SearchPath &operator[] (unsigned i) { return *list_[i]; }

	void add(const QString &path);
	void remove(unsigned i) { list_.remove(i); emit changed(); }
	bool exists(const QString &path);
	void clear() { list_.clear(); emit changed(); }
	inline int size() { return list_.size(); }

	//listdef_t &list() { return list_; }
signals:
	void changed();
private:
	friend class DM_Settings;

	listdef_t list_;
};

/****************************
 *
 * DM_Settings_SearchPath_List_Model
 *
 ***************************/
class DM_Settings_SearchPath_List_Model : public QAbstractListModel
{
	Q_OBJECT

public:
    DM_Settings_SearchPath_List_Model(QObject *parent = 0);

	void setList(DM_Settings_SearchPath_List *list);
	virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;
private slots:
	void listChanged();
private:
    DM_Settings_SearchPath_List *list_;
};

/****************************
 *
 * DM_Settings
 *
 ***************************/
class DM_Settings
{
public:
	DM_Settings();

	void load();
	void save();

	DM_Settings_SearchPath_List &searchPaths() { return searchpaths_; }
	DM_Settings_SearchPath_List &importPaths() { return importpaths_; }
	DM_Settings_SearchPath_List &outputPaths() { return outputpaths_; }
	const QString &importFilters() const { return importfilters_; }
	void setImportFilters(const QString &filters) { importfilters_=filters; }
private:
	DM_Settings_SearchPath_List searchpaths_, importpaths_, outputpaths_;
	QString importfilters_;
};

#endif // DMSETTINGS_H
