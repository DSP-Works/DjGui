#include "databasemodel.h"

#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDirIterator>
#include <QMimeData>
#include <QUrl>

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
    m_db(db),
    m_tableModel(std::make_unique<QSqlTableModel>(nullptr, db))
{
    QSqlQuery query("CREATE TABLE IF NOT EXISTS `Tracks` ("
                    "`ID` INTEGER PRIMARY KEY NOT NULL, "
                    "`Filename` TEXT NOT NULL UNIQUE, "
                    "`Title` TEXT, "
                    "`Artist` TEXT, "
                    "`Album` TEXT, "
                    "`Comment` TEXT, "
                    "`Genre` TEXT, "
                    "`Year` INT, "
                    "`Track` INT, "
                    "`LengthInMilliseconds` INT, "
                    "`Bitrate` INT, "
                    "`SampleRate` INT, "
                    "`Channels` INT"
                    ");", m_db);
    if (query.lastError().isValid())
    {
        QMessageBox::warning(nullptr, tr("Could not setup database!"), tr("Could not setup database!") + "\n\n" + query.lastError().text());
        return;
    }

    m_tableModel->setTable("Tracks");
    m_tableModel->select();
}

DatabaseModel::~DatabaseModel() = default;

namespace {
struct TrackMetadata {
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
};

std::optional<TrackMetadata> parseTrack(const QString &path)
{
    TagLib::FileRef f(path.toUtf8().constData());

    if (f.isNull())
    {
        qWarning() << "file is null";
        return {};
    }

    TrackMetadata metadata;

    {
        const auto tag = f.tag();
        if (!tag || tag->isEmpty())
        {
            qWarning() << "could not read metadata" << path;
            metadata.title = QFileInfo(path).baseName();
        }
        else
        {
            metadata.title = QString::fromStdString(tag->title().to8Bit(true));
            metadata.artist = QString::fromStdString(tag->artist().to8Bit(true));
            metadata.album = QString::fromStdString(tag->album().to8Bit(true));
            metadata.comment = QString::fromStdString(tag->comment().to8Bit(true));
            metadata.genre = QString::fromStdString(tag->genre().to8Bit(true));
            metadata.year = tag->year();
            metadata.track = tag->track();
        }
    }

    {
        const auto audioProperties = f.audioProperties();
        if (!audioProperties)
            qWarning() << "could not read audioProperties" << path;
        else
        {
            metadata.lengthInMilliseconds = audioProperties->lengthInMilliseconds();
            metadata.bitrate = audioProperties->bitrate();
            metadata.sampleRate = audioProperties->sampleRate();
            metadata.channels = audioProperties->channels();
        }
    }

    return std::move(metadata);
}
}

void DatabaseModel::import(const QString &path)
{
    QSqlQuery query(m_db);

    if (!query.prepare("INSERT INTO `Tracks` (`Filename`, `Title`, `Artist`, `Album`, `Comment`, `Genre`, `Year`, `Track`, `LengthInMilliseconds`, `Bitrate`, `SampleRate`, `Channels`)"
                       "VALUES (:Filename, :Title, :Artist, :Album, :Comment, :Genre, :Year, :Track, :LengthInMilliseconds, :Bitrate, :SampleRate, :Channels);"))
    {
        QMessageBox::warning(0, tr("Could not import tracks!"), tr("Could not import tracks!") + "\n\n" + query.lastError().text());
        return;
    }

    QDirIterator iter(path, QDir::Files);
    while (iter.hasNext())
    {
        const QFileInfo info(iter.next());

        const auto metadata = parseTrack(info.absoluteFilePath());
        if (!metadata.has_value())
            continue;

        query.bindValue(":Filename", info.absoluteFilePath());
        query.bindValue(":Title", metadata->title);
        query.bindValue(":Artist", metadata->artist);
        query.bindValue(":Album", metadata->album);
        query.bindValue(":Comment", metadata->comment);
        query.bindValue(":Genre", metadata->genre);
        query.bindValue(":Year", metadata->year.has_value() ? QVariant(metadata->year.value()) : QVariant());
        query.bindValue(":Track", metadata->track.has_value() ? QVariant(metadata->track.value()) : QVariant());
        query.bindValue(":LengthInMilliseconds", metadata->lengthInMilliseconds.has_value() ? QVariant(metadata->lengthInMilliseconds.value()) : QVariant());
        query.bindValue(":Bitrate", metadata->bitrate.has_value() ? QVariant(metadata->bitrate.value()) : QVariant());
        query.bindValue(":SampleRate", metadata->sampleRate.has_value() ? QVariant(metadata->sampleRate.value()) : QVariant());
        query.bindValue(":Channels", metadata->channels.has_value() ? QVariant(metadata->channels.value()) : QVariant());

        if (!query.exec())
            qWarning() << "error inserting:" << query.lastError().text();
    }
}

int DatabaseModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return -1;
    return m_tableModel->rowCount();
}

int DatabaseModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return -1;
    return int(Columns::Comment) + 1;
}

QVariant DatabaseModel::data(const QModelIndex &index, int role) const
{
    int column;

    switch (index.column())
    {
    case int(Columns::ID):      column = m_tableModel->fieldIndex("ID"); break;
    case int(Columns::Cover):   return {};
    case int(Columns::Title):   column = m_tableModel->fieldIndex("Title"); break;
    case int(Columns::Artist):  column = m_tableModel->fieldIndex("Artist"); break;
    case int(Columns::Bpm):     switch (role) { case Qt::DisplayRole: case Qt::EditRole: return tr("...");    default: return {}; }
    case int(Columns::Key):     switch (role) { case Qt::DisplayRole: case Qt::EditRole: return tr("...");    default: return {}; }
    case int(Columns::Rating):  switch (role) { case Qt::DisplayRole: case Qt::EditRole: return tr("..."); default: return {}; }
    case int(Columns::Comment): column = m_tableModel->fieldIndex("Comment"); break;
    }

    const auto mappedIndex = m_tableModel->index(index.row(), column);
    return m_tableModel->data(mappedIndex, role);
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

Qt::ItemFlags DatabaseModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled;
}

QMimeData *DatabaseModel::mimeData(const QModelIndexList &indexes) const
{
    qDebug() << indexes.count();
    if (indexes.count() != int(Columns::Comment) + 1)
        return {};

    const auto index = indexes.first();
    const auto mappedIndex = m_tableModel->index(index.row(), m_tableModel->fieldIndex("Filename"));
    const auto filename = m_tableModel->data(mappedIndex, Qt::EditRole).toString();

    auto data = new QMimeData;
    data->setData("text/uri-list", (QUrl::fromLocalFile(filename).toString() + "\r\n").toUtf8());
    return data;
}
