#include "databasemodel.h"

#include <QDirIterator>

#include <QDebug>

#include <optional>

// taglib includes
#include <taglib/fileref.h>
#include <taglib/tstring.h>

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

DatabaseModel::DatabaseModel(QSqlDatabase &db, QObject *parent) :
    QAbstractListModel(parent),
    m_db(db)
{
}

void DatabaseModel::import(const QString &path)
{
    QDirIterator iter(path, QDir::Files);
    while (iter.hasNext())
    {
        const QFileInfo info(iter.next());

        TagLib::FileRef f(info.absoluteFilePath().toUtf8().constData());

        if (f.isNull())
        {
            qWarning() << "file is null";
            continue;
        }

        QString title;
        QString artist;
        QString album;
        QString comment;
        QString genre;
        std::optional<unsigned int> year;
        std::optional<unsigned int> track;
        std::optional<int> lengthInMilliseconds;
        std::optional<int> bitrate;
        std::optional<int> sampleRate;
        std::optional<int> channels;

        {
            const auto tag = f.tag();
            if (!tag || tag->isEmpty())
            {
                qWarning() << "could not read metadata" << info;
                title = info.baseName();
            }
            else
            {
                title = QString::fromStdString(tag->title().to8Bit(true));
                artist = QString::fromStdString(tag->artist().to8Bit(true));
                album = QString::fromStdString(tag->album().to8Bit(true));
                comment = QString::fromStdString(tag->comment().to8Bit(true));
                genre = QString::fromStdString(tag->genre().to8Bit(true));
                year = tag->year();
                track = tag->track();
            }
        }
        {
            const auto audioProperties = f.audioProperties();
            if (!audioProperties)
            {
                qWarning() << "could not read audioProperties" << info;
                continue;
            }

            lengthInMilliseconds = audioProperties->lengthInMilliseconds();
            bitrate = audioProperties->bitrate();
            sampleRate = audioProperties->sampleRate();
            channels = audioProperties->channels();
        }
    }
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
