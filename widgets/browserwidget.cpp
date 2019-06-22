#include "browserwidget.h"

#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeView>
#include <QFileSystemModel>
#include <QStandardPaths>

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
        auto hboxLayout = new QHBoxLayout;

        {
            auto treeView = new QTreeView;
            treeView->setDragEnabled(true);

            auto model = new QFileSystemModel;
            model->setRootPath("");
            treeView->setModel(model);
            for (int i = 1; i < model->columnCount(); ++i)
                treeView->hideColumn(i);
            {
                const auto dirs = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
                if (!dirs.empty())
                    treeView->setRootIndex(model->index(dirs.first()));
            }
            hboxLayout->addWidget(treeView, 1);
        }

        {
            auto treeView = new QTreeView;
            treeView->setModel(m_model.get());
            hboxLayout->addWidget(treeView, 4);
        }

        layout->addLayout(hboxLayout, 1);
    }


    setLayout(layout);
}

BrowserWidget::~BrowserWidget() = default;
