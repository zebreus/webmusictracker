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
    SampleLoader * sampleManager = nullptr;
    
    ALCdevice *audioDevice;
    ALCcontext *audioContext;
    
    void playSample();
public:
    BasicInstrument(SampleLoader* sampleManager);
    
    void drawWindow();
    
};

#endif
