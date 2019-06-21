#pragma once

#include <QAbstractListModel>

class DatabaseModel : public QAbstractListModel
{
public:
    explicit DatabaseModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
};
