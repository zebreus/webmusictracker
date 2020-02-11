#ifndef BASIC_SEQUENCER_HPP
#define BASIC_SEQUENCER_HPP

#include "imgui.h"
#include "imgui_internal.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <emscripten.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include "BasicInstrument.hpp"

using namespace std;

#define TONE_VALUE_0 (1.f/12.f)*0.f
#define TONE_VALUE_1 (1.f/12.f)*1.f
#define TONE_VALUE_2 (1.f/12.f)*2.f
#define TONE_VALUE_3 (1.f/12.f)*3.f
#define TONE_VALUE_4 (1.f/12.f)*4.f
#define TONE_VALUE_5 (1.f/12.f)*5.f
#define TONE_VALUE_6 (1.f/12.f)*6.f
#define TONE_VALUE_7 (1.f/12.f)*7.f
#define TONE_VALUE_8 (1.f/12.f)*8.f
#define TONE_VALUE_9 (1.f/12.f)*9.f
#define TONE_VALUE_10 (1.f/12.f)*10.f
#define TONE_VALUE_11 (1.f/12.f)*11.f

#define TONE_NAME_0 "C-"
#define TONE_NAME_1 "C#"
#define TONE_NAME_2 "D-"
#define TONE_NAME_3 "D#"
#define TONE_NAME_4 "E-"
#define TONE_NAME_5 "F-"
#define TONE_NAME_6 "F#"
#define TONE_NAME_7 "G-"
#define TONE_NAME_8 "G#"
#define TONE_NAME_9 "A-"
#define TONE_NAME_10 "A#"
#define TONE_NAME_11 "B-"

class BasicSequencer{
private:
    struct SequenceEntry{
        float tone;
        float gate;
        float velocity;
        
        string toneString();
        string gateString();
        string velocityString();
    };
    vector<SequenceEntry> sequence;
    BasicInstrument * instrument = nullptr;
    
    int count = 0;
    int position = 0;
    bool playing = false;
    
    int selectedPropertyId = 0;
    
    const int sequenceLength = 16;
    
    void drawSequencerColum(int editorHeight = 8);
    
    void pushSelectableColor(const ImVec4& color);
    void popSelectableColor();
    string toHexString(int number);
    
public:
    BasicSequencer(BasicInstrument* instrument);
    
    void drawWindow();
    
    void update();
};

#endif
