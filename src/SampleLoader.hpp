#ifndef SAMPLE_LOADER_HPP
#define SAMPLE_LOADER_HPP

#include "imgui.h"
#include <stdio.h>
#include <emscripten.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std;

class SampleLoader{
private:
    vector<filesystem::path> availableSamples;
    filesystem::path const * selectedSample = nullptr;

    //Load all samples on idbfs into this sample loader
    void loadAvailableSamples();
    
    //Upload a user sample
    void loadSamples();
    
public:
    SampleLoader();
    void drawWindow();
    vector<filesystem::path> getAvailableSamples();
    
    vector<float> loadAvailableSample(filesystem::path samplePath);
};

#endif

