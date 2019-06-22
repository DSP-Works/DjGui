#pragma once

#include <QMainWindow>
#include <QList>
#include <QAudioDeviceInfo>
#include <QThread>

#include <memory>

class QAudioOutput;
class QComboBox;
class QSpinBox;
class QPushButton;

class AudioMixer;
class AudioVolumeControl;
class AudioLimiter;
class StreamDevice;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private slots:
    void refreshAudioDevices();
    void play();

private:
    std::unique_ptr<QAudioOutput> m_output;
    const std::unique_ptr<AudioMixer> m_mixer;
    const std::unique_ptr<StreamDevice> m_device;

    QComboBox *m_combobox;
    QList<QAudioDeviceInfo> m_audioDevices;
    QSpinBox *m_spinBox;
    QPushButton *m_button;

    QThread m_decodingThread;
};
