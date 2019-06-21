#include "databasemodel.h"

namespace {
enum class Columns {
    ID,
    Cover,
    Title,
    Artist,
    Bpm,
    Key,
    Rating,
    Comment
};
}

DatabaseModel::DatabaseModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int DatabaseModel::rowCount(const QModelIndex &parent) const
{
    return 0;
}

int DatabaseModel::columnCount(const QModelIndex &parent) const
{
    return int(Columns::Comment) + 1;
}

QVariant DatabaseModel::data(const QModelIndex &index, int role) const
{
    return {};
}

QVariant DatabaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (orientation)
    {
    case Qt::Horizontal:
        switch (section)
        {
        case int(Columns::ID):      switch (role) { case Qt::DisplayRole: case Qt::EditRole: return tr("#"); }       break;
        case int(Columns::Cover):   switch (role) { case Qt::DisplayRole: case Qt::EditRole: return tr("Cover"); }   break;
        case int(Columns::Title):   switch (role) { case Qt::DisplayRole: case Qt::EditRole: return tr("Title"); }   break;
        case int(Columns::Artist):  switch (role) { case Qt::DisplayRole: case Qt::EditRole: return tr("Artist"); }  break;
        case int(Columns::Bpm):     switch (role) { case Qt::DisplayRole: case Qt::EditRole: return tr("Bpm"); }     break;
        case int(Columns::Key):     switch (role) { case Qt::DisplayRole: case Qt::EditRole: return tr("Key"); }     break;
        case int(Columns::Rating):  switch (role) { case Qt::DisplayRole: case Qt::EditRole: return tr("Rating"); }  break;
        case int(Columns::Comment): switch (role) { case Qt::DisplayRole: case Qt::EditRole: return tr("Comment"); } break;
        }
        break;
    }

    return {};
}
