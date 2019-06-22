#pragma once

#include <QAbstractListModel>

#include <memory>

class QSqlDatabase;
class QSqlTableModel;

class DatabaseModel : public QAbstractListModel
{
public:
    explicit DatabaseModel(QSqlDatabase &db, QObject *parent = nullptr);
    ~DatabaseModel() override;

    void import(const QString &path);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;

private:
    QSqlDatabase &m_db;
    const std::unique_ptr<QSqlTableModel> m_tableModel;

};
