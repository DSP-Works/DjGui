#pragma once

#include <QAbstractListModel>

class QSqlDatabase;

class DatabaseModel : public QAbstractListModel
{
public:
    explicit DatabaseModel(QSqlDatabase &db, QObject *parent = nullptr);

    void import(const QString &path);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QSqlDatabase &m_db;
};
