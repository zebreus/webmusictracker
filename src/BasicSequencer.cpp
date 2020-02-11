#include <BasicSequencer.hpp>

BasicSequencer::BasicSequencer(BasicInstrument* instrument){
    this->instrument = instrument;
}

void BasicSequencer::drawWindow(){
    ImGui::Begin("Basic Sequencer");
    ImGui::Text("The basic sequencer can play sequences");
    ImGui::End();
}
