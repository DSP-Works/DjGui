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
#include <QMenu>
#include <QAction>

#include "databasemodel.h"

BrowserWidget::BrowserWidget(QWidget *parent) :
    QWidget(parent),
    m_db(std::make_unique<QSqlDatabase>(QSqlDatabase::addDatabase("QSQLITE"))),
    m_model(std::make_unique<DatabaseModel>(*m_db))
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
            m_treeView = new QTreeView;
            m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(m_treeView, &QWidget::customContextMenuRequested, this, &BrowserWidget::customContextMenuRequested);

            m_treeModel = std::make_unique<QFileSystemModel>();
            m_treeModel->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
            m_treeModel->setRootPath("");
            m_treeView->setModel(m_treeModel.get());
            for (int i = 1; i < m_treeModel->columnCount(); ++i)
                m_treeView->hideColumn(i);
            {
                const auto dirs = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
                if (!dirs.empty())
                    m_treeView->setRootIndex(m_treeModel->index(dirs.first()));
            }
            hboxLayout->addWidget(m_treeView, 1);
        }

        m_listView = new QTreeView;
        m_listView->setModel(m_model.get());
        hboxLayout->addWidget(m_listView, 4);

        layout->addLayout(hboxLayout, 1);
    }


    setLayout(layout);
}

BrowserWidget::~BrowserWidget() = default;

void BrowserWidget::customContextMenuRequested(const QPoint &pos)
{
    const auto index = m_treeView->indexAt(pos);
    if (!index.isValid())
        return;

    const auto path = m_treeModel->filePath(index);
    if (path.isEmpty())
        return;

    QMenu menu;
    const auto importAction = menu.addAction(tr("Import folder to collection"));
    const auto selectedAction = menu.exec(m_treeView->viewport()->mapToGlobal(pos));

    if (selectedAction == importAction)
        m_model->import(path);
}
