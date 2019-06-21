#include "audiogenerator.h"

#include <algorithm>
#include <cmath>

constexpr float doublePi() { return std::atan(1) * 4 * 2; }

void AudioGenerator::writeSamples(float *buffer, std::size_t frames)
{
    const float stepSize = doublePi() / sampleRate() * m_frequency;
    int channelcounter { 0 };
    std::generate(buffer, buffer + frames, [&channelcounter,&pos=m_pos,stepSize](){
        const auto sample = std::sin(pos);
        if (++channelcounter == channelCount())
        {
            channelcounter = 0;
            pos += stepSize;
            while (pos > doublePi())
                pos -= doublePi();
        }
        return sample;
    });
}
