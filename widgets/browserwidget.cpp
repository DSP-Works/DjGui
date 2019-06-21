#include "browserwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTreeView>

BrowserWidget::BrowserWidget(QWidget *parent) :
    QWidget(parent)
{
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
    }

    layout->addWidget(new QTreeView, 1);

    setLayout(layout);
}
