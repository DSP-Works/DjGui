#include "browserwidget.h"

#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeView>

#include "databasemodel.h"

BrowserWidget::BrowserWidget(QWidget *parent) :
    QWidget(parent),
    m_db(std::make_unique<QSqlDatabase>(QSqlDatabase::addDatabase("QSQLITE"))),
    m_model(std::make_unique<DatabaseModel>())
{
    m_db->setDatabaseName("database.db");
    if (!m_db->open())
        QMessageBox::critical(this, tr("Could not open database!"), tr("Could not open database!") + "\n\n" + m_db->lastError().text());

    auto layout = new QVBoxLayout;

    {
        auto hboxLayout = new QHBoxLayout;

        m_lineEdit = new QLineEdit;
        hboxLayout->addWidget(m_lineEdit);

        hboxLayout->addStretch(1);

        {
            auto button = new QPushButton(tr("Refresh"));
            hboxLayout->addWidget(button);
        }

        layout->addLayout(hboxLayout);
    }

    {
        auto treeView = new QTreeView;
        treeView->setModel(m_model.get());
        layout->addWidget(treeView, 1);
    }

    setLayout(layout);
}

BrowserWidget::~BrowserWidget() = default;
