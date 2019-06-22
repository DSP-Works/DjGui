#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QAudioOutput>

#include "audiocrossmixer.h"
#include "streamdevice.h"
#include "decks/trackdeck.h"
#include "decks/remixdeck.h"
#include "decks/synthesizerdeck.h"
#include "browserwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_mixer(std::make_unique<AudioCrossMixer>()),
    m_device(std::make_unique<StreamDevice>(m_mixer.get()))
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
            connect(slider, &QSlider::valueChanged, this, [&output=m_output](int value){ if (output) output->setVolume(float(value) / 100.f); });
            hboxLayout->addWidget(slider);
        }

        hboxLayout->addStretch(1);

        {
            m_combobox = new QComboBox;
            refreshAudioDevices();
            hboxLayout->addWidget(m_combobox);

            m_spinBox = new QSpinBox;
            m_spinBox->setRange(0, std::numeric_limits<int>::max());
            m_spinBox->setValue(1024);
            hboxLayout->addWidget(m_spinBox);

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
            auto trackDeck = new TrackDeck(m_decodingThread);
            m_mixer->addLeftInput(*trackDeck);
            gridLayout->addWidget(trackDeck, 0, 0);
        }

        {
            auto trackDeck = new TrackDeck(m_decodingThread);
            m_mixer->addRightInput(*trackDeck);
            gridLayout->addWidget(trackDeck, 0, 1);
        }

        {
            auto remixDeck = new RemixDeck;
            m_mixer->addLeftInput(*remixDeck);
            gridLayout->addWidget(remixDeck, 1, 0);
        }

        {
            auto synthesizerDeck = new SynthesizerDeck;
            m_mixer->addRightInput(*synthesizerDeck);
            gridLayout->addWidget(synthesizerDeck, 1, 1);
        }

        mainLayout->addLayout(gridLayout);
    }

    {
        auto hboxLayout = new QHBoxLayout;

        hboxLayout->addStretch();

        auto slider = new QSlider(Qt::Horizontal);
        slider->setFixedWidth(200);
        slider->setRange(-100, 100);
        connect(slider, &QSlider::valueChanged, [&mixer=*m_mixer](int value){ mixer.setPosition(float(value) / 100.f); });
        hboxLayout->addWidget(slider);

        hboxLayout->addStretch();

        mainLayout->addLayout(hboxLayout);
    }

    mainLayout->addWidget(new BrowserWidget, 1);

    {
        auto centralWidget = new QWidget;
        centralWidget->setLayout(mainLayout);
        setCentralWidget(centralWidget);
    }

    m_decodingThread.start();
}

MainWindow::~MainWindow()
{
    m_decodingThread.exit();
    m_decodingThread.wait();
}

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
        m_combobox->setEnabled(true);
        m_spinBox->setEnabled(true);
    }
    else
    {
        const auto bufferSize = m_spinBox->value() * 4 * AudioSource::channelCount();
        m_output = std::make_unique<QAudioOutput>(m_audioDevices.at(m_combobox->currentIndex()), AudioSource::format());
        m_output->setBufferSize(bufferSize);
        m_output->start(m_device.get());
        m_output->setBufferSize(bufferSize);
        m_spinBox->setValue(m_output->bufferSize() / 4 / AudioSource::channelCount());
        m_button->setText("Stop");
        m_combobox->setEnabled(false);
        m_spinBox->setEnabled(false);
    }
}
