#ifndef BASIC_SEQUENCER_HPP
#define BASIC_SEQUENCER_HPP

#include "imgui.h"
#include "imgui_internal.h"
#include <stdio.h>
#include <emscripten.h>
#include <string>
#include <vector>
#include <iostream>
#include "BasicInstrument.hpp"

using namespace std;


class BasicSequencer{
private:
    struct SequenceEntry{
        float tone;
        float gate;
        float loudness;
    };
    vector<SequenceEntry> sequence;
    BasicInstrument * instrument = nullptr;
    
    int count = 0;
    int position = 0;
    bool playing = false;
    
public:
    BasicSequencer(BasicInstrument* instrument);
    
    void drawWindow();
    
    void update();
};

#endif
