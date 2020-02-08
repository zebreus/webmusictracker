#ifndef SAMPLE_LOADER_HPP
#define SAMPLE_LOADER_HPP

#include "imgui.h"
#include <stdio.h>
#include <emscripten.h>
#include <string>
#include <vector>
#include <iostream>
#include <filesystem>

using namespace std;

class SampleLoader{
private:
    void loadSamples();
    
    vector<string> getAvailableSamples();
    
public:
    SampleLoader();
    
    void drawWindow();
    
};

#endif

