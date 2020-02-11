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
    BasicInstrument * instrument = nullptr;
    
public:
    BasicSequencer(BasicInstrument* instrument);
    
    void drawWindow();
};

#endif
