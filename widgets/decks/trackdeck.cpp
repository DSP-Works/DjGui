#include "trackdeck.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QUrl>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

TrackDeck::TrackDeck(QWidget *parent) :
    DeckTemplate(parent)
{
    setAcceptDrops(true);
    setMinimumHeight(150);

    auto layout = new QVBoxLayout;

    {
        auto hboxLayout = new QHBoxLayout;

        hboxLayout->addWidget(new QLabel(tr("TrackDeck")));

        hboxLayout->addStretch(1);

        {
            auto button = new QToolButton;
            button->setText(tr("▶"));
            hboxLayout->addWidget(button);
        }
        {
            auto button = new QToolButton;
            button->setText(tr("CUE"));
            connect(button, &QAbstractButton::pressed, [](){qDebug() << "pressed";});
            connect(button, &QAbstractButton::released, [](){qDebug() << "released";});
            hboxLayout->addWidget(button);
        }

        layout->addLayout(hboxLayout);
    }

    layout->addStretch(1);

    auto widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
}

AudioSource &TrackDeck::deckAudioSource()
{

}

void TrackDeck::dragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasFormat("text/uri-list"))
        return;

    auto data = event->mimeData()->data("text/uri-list");

    QTextStream textStream(data, QIODevice::ReadOnly | QIODevice::Text);
    if (textStream.atEnd())
        return;

    const QUrl url(textStream.readLine());
    if (!url.isLocalFile())
        return;

    const QFileInfo fileInfo(url.toLocalFile());
    if (!fileInfo.exists())
        return;

    if (!fileInfo.isFile())
        return;

    event->acceptProposedAction();
}

void TrackDeck::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event)
}

void TrackDeck::dropEvent(QDropEvent *event)
{
    if (!event->mimeData()->hasFormat("text/uri-list"))
    {
        qWarning() << "wrong type";
        return;
    }

    auto data = event->mimeData()->data("text/uri-list");

    QTextStream textStream(data, QIODevice::ReadOnly | QIODevice::Text);
    if (textStream.atEnd())
    {
        qWarning() << "no lines";
        return;
    }

    const QUrl url(textStream.readLine());
    if (!url.isLocalFile())
    {
        qWarning() << "isnt local file";
        return;
    }

    const QFileInfo fileInfo(url.toLocalFile());
    if (!fileInfo.exists())
    {
        qWarning() << "doesnt exist";
        return;
    }

    if (!fileInfo.isFile())
    {
        qWarning() << "isnt file";
        return;
    }
}
