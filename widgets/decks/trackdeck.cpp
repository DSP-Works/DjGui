#include "trackdeck.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QSlider>
#include <QToolButton>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QUrl>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

#include "audioplayer.h"
#include "audiodecoder.h"
#include "audioplayer.h"
#include "widgets/previewwidget.h"

TrackDeck::TrackDeck(QThread &decodingThread, bool rightSide, QWidget *parent) :
    DeckTemplate(rightSide, parent),
    m_decodingThread(decodingThread),
    m_player(std::make_unique<AudioPlayer>(&m_buffer))
{
    setAcceptDrops(true);
    setMinimumHeight(150);

    connect(m_player.get(), &AudioPlayer::currentSampleChanged, this, &TrackDeck::currentSampleChanged);

    auto layout = new QVBoxLayout;
    layout->setMargin(0);

    {
        auto hboxLayout = new QHBoxLayout;

        hboxLayout->addWidget(new QLabel(tr("TrackDeck")));

        hboxLayout->addStretch(1);

        m_progressBar = new QProgressBar;
        m_progressBar->setVisible(false);
        hboxLayout->addWidget(m_progressBar);

        hboxLayout->addStretch(1);

        m_slider = new QSlider(Qt::Horizontal);
        m_slider->setRange(80, 120);
        m_slider->setValue(100);
        m_slider->setVisible(false);
        connect(m_slider, &QSlider::valueChanged, this, [&player=*m_player](int value){ player.setPlaybackSpeed(float(value) / 100.f); });
        hboxLayout->addWidget(m_slider);

        hboxLayout->addStretch(1);

        m_playButton = new QToolButton;
        m_playButton->setEnabled(false);
        m_playButton->setText(tr("▶"));
        connect(m_playButton, &QAbstractButton::pressed, this, [&player=*m_player](){
            player.setPlaying(!player.playing());
        });
        connect(m_player.get(), &AudioPlayer::playingChanged, m_playButton, [this](bool playing){
            m_playButton->setText(playing ? tr("▮▮") : tr("▶"));
        });
        hboxLayout->addWidget(m_playButton);

        m_cueButton = new QToolButton;
        m_cueButton->setEnabled(false);
        m_cueButton->setText(tr("CUE"));
        connect(m_cueButton, &QAbstractButton::pressed, [](){qDebug() << "pressed";});
        connect(m_cueButton, &QAbstractButton::released, [](){qDebug() << "released";});
        hboxLayout->addWidget(m_cueButton);

        layout->addLayout(hboxLayout);
    }

    m_previewWidget = new PreviewWidget(&m_buffer);
    m_previewWidget->setMinimumHeight(50);
    connect(this, &TrackDeck::currentSampleChanged, m_previewWidget, &PreviewWidget::setCurrentSample);
    connect(m_previewWidget, &PreviewWidget::sampleSelected, this, [&player=*m_player](double sample){ player.setPosition(sample); });
    connect(this, &TrackDeck::bufferChanged, m_previewWidget, &PreviewWidget::clearCache);
    layout->addWidget(m_previewWidget);

    layout->addStretch(1);

    auto widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
}

TrackDeck::~TrackDeck() = default;

AudioSource &TrackDeck::deckAudioSource()
{
    return *m_player;
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

    m_progressBar->setValue(0);
    m_progressBar->setVisible(true);

    m_decoder = std::make_unique<AudioDecoder>(AudioSource::format());
    m_decoder->moveToThread(&m_decodingThread);
    connect(m_decoder.get(), &AudioDecoder::progress, this, &TrackDeck::progress);
    connect(m_decoder.get(), &AudioDecoder::decodingFinished, this, &TrackDeck::decodingFinished);
    QMetaObject::invokeMethod(m_decoder.get(), "start", Qt::AutoConnection, Q_ARG(QString, fileInfo.absoluteFilePath()));
}

void TrackDeck::progress(int progress, int total)
{
    m_progressBar->setMaximum(total);
    m_progressBar->setValue(progress);
}

void TrackDeck::decodingFinished()
{
    m_buffer = m_decoder->takeBuffer();

    m_player->setPosition(0);
    m_player->setPlaying(true);

    m_slider->setVisible(true);
    m_slider->setValue(100);

    m_playButton->setEnabled(true);
    m_cueButton->setEnabled(true);

    emit bufferChanged();

    m_decoder = nullptr;

    m_progressBar->setVisible(false);
}
