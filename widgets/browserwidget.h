#pragma once

#include <QWidget>

#include <memory>

class QSqlDatabase;
class QLineEdit;

class DatabaseModel;

class BrowserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BrowserWidget(QWidget *parent = nullptr);
    virtual ~BrowserWidget() override;

private:
    const std::unique_ptr<QSqlDatabase> m_db;
    const std::unique_ptr<DatabaseModel> m_model;

    QLineEdit *m_lineEdit;
};
