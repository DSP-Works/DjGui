#pragma once

#include "audiosource.h"
#include "audiomixer.h"
#include "audiovolumecontrol.h"

class AudioCrossMixer : public AudioSource
{
public:
    explicit AudioCrossMixer();
    explicit AudioCrossMixer(const std::vector<AudioSource*> &leftInputs, const std::vector<AudioSource*> &rightInputs);
    explicit AudioCrossMixer(std::vector<AudioSource*> &&leftInputs, std::vector<AudioSource*> &&rightInputs);

    void addLeftInput(AudioSource &source) { m_leftMixer.addInput(source); }
    void removeLeftInput(AudioSource &source) { m_leftMixer.removeInput(source); }

    const std::vector<AudioSource*> &leftInputs() const { return m_leftMixer.inputs(); }

    void addRightInput(AudioSource &source) { m_rightMixer.addInput(source); }
    void removeRightInput(AudioSource &source) { m_rightMixer.removeInput(source); }

    const std::vector<AudioSource*> &rightInputs() const { return m_rightMixer.inputs(); }

    void writeSamples(float *buffer, std::size_t frames) override;

    float position() const { return m_position; }
    void setPosition(float position);

private:
    float m_position { 0 };
    AudioMixer m_leftMixer;
    AudioVolumeControl m_leftControl;
    AudioMixer m_rightMixer;
    AudioVolumeControl m_rightControl;
    AudioMixer m_masterMixer;
};
