#pragma once

#include <QWidget>

#include <memory>

class QSqlDatabase;
class QLineEdit;
class QFileSystemModel;
class QTreeView;

class DatabaseModel;

class BrowserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BrowserWidget(QWidget *parent = nullptr);
    virtual ~BrowserWidget() override;

private slots:
    void customContextMenuRequested(const QPoint &pos);

private:
    const std::unique_ptr<QSqlDatabase> m_db;
    const std::unique_ptr<DatabaseModel> m_model;

    QLineEdit *m_lineEdit;

    std::unique_ptr<QFileSystemModel> m_treeModel;

    QTreeView *m_treeView;
    QTreeView *m_listView;
};
