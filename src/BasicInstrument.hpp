#ifndef BASIC_INSTRUMENT_HPP
#define BASIC_INSTRUMENT_HPP

#include "imgui.h"
#include "imgui_internal.h"
#include <stdio.h>
#include <emscripten.h>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <filesystem>
#include "SampleLoader.hpp"
#include <AL/al.h>
#include <AL/alc.h>

using namespace std;


class BasicInstrument{
private:
    filesystem::path const * selectedSample = nullptr;
    vector<float> sampleData;
    vector<float> pitchedSampleData;
    SampleLoader * sampleManager = nullptr;
    
    ALCdevice *audioDevice;
    ALCcontext *audioContext;
    
    
public:
    BasicInstrument(SampleLoader* sampleManager);
    
    void drawWindow();
    void playSample();
    //Pitches the sample up or down.
    // pitch 1 should be +1 octave
    void pitchSample(float pitch);
    //Stretches the sample by the given factor
    void stretchSample(float stretch);
    
};

#endif
