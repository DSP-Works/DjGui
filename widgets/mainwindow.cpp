#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QAudioOutput>

#include "audiomixer.h"
#include "audiovolumecontrol.h"
#include "audiolimiter.h"
#include "streamdevice.h"
#include "decks/trackdeck.h"
#include "decks/remixdeck.h"
#include "decks/synthesizerdeck.h"
#include "browserwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_mixer(std::make_unique<AudioMixer>()),
    m_volumeController(std::make_unique<AudioVolumeControl>(m_mixer.get())),
    m_limiter(std::make_unique<AudioLimiter>(m_volumeController.get())),
    m_device(std::make_unique<StreamDevice>(m_limiter.get()))
{
    auto mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);

    {
        auto hboxLayout = new QHBoxLayout;

        {
            auto label = new QLabel(tr("UltraDJ"));
            hboxLayout->addWidget(label);
        }

        hboxLayout->addStretch(1);

        {
            auto slider = new QSlider(Qt::Horizontal);

            auto label = new QLabel(tr("Master"));
            label->setBuddy(slider);
            hboxLayout->addWidget(label);

            slider->setRange(0, 100);
            slider->setValue(100);
            connect(slider, &QSlider::valueChanged, this, [this](int value){ m_volumeController->setVolume(float(value) / 100.f); });
            hboxLayout->addWidget(slider);
        }

        hboxLayout->addStretch(1);

        {
            m_combobox = new QComboBox;
            refreshAudioDevices();
            hboxLayout->addWidget(m_combobox);

            m_button = new QPushButton;
            connect(m_button, &QAbstractButton::pressed, this, &MainWindow::play);
            play();
            hboxLayout->addWidget(m_button);
        }

        mainLayout->addLayout(hboxLayout);
    }

    {
        auto gridLayout = new QGridLayout;

        {
            auto trackDeck = new TrackDeck;
            m_mixer->addInput(*trackDeck);
            gridLayout->addWidget(trackDeck, 0, 0);
        }

        {
            auto trackDeck = new TrackDeck;
            m_mixer->addInput(*trackDeck);
            gridLayout->addWidget(trackDeck, 0, 1);
        }

        {
            auto remixDeck = new RemixDeck;
            m_mixer->addInput(*remixDeck);
            gridLayout->addWidget(remixDeck, 1, 0);
        }

        {
            auto synthesizerDeck = new SynthesizerDeck;
            m_mixer->addInput(*synthesizerDeck);
            gridLayout->addWidget(synthesizerDeck, 1, 1);
        }

        mainLayout->addLayout(gridLayout);
    }

    mainLayout->addWidget(new BrowserWidget);

    {
        auto centralWidget = new QWidget;
        centralWidget->setLayout(mainLayout);
        setCentralWidget(centralWidget);
    }
}

MainWindow::~MainWindow() = default;

void MainWindow::refreshAudioDevices()
{
    m_combobox->clear();
    m_audioDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    for (const auto &device : m_audioDevices)
        m_combobox->addItem(device.deviceName());
    if (const auto index = m_audioDevices.indexOf(QAudioDeviceInfo::defaultOutputDevice()); index >= 0)
        m_combobox->setCurrentIndex(index);
}

void MainWindow::play()
{
    if (m_output)
    {
        m_output = nullptr;
        m_button->setText("Start");
    }
    else
    {
        m_output = std::make_unique<QAudioOutput>(m_audioDevices.at(m_combobox->currentIndex()), AudioSource::format());
        m_output->start(m_device.get());
        m_output->setBufferSize(8192);
        m_button->setText("Stop");
    }
}
