#include <BasicSequencer.hpp>

BasicSequencer::BasicSequencer(BasicInstrument* instrument){
    this->instrument = instrument;
    for(int i = 0;i<8;i++){
        sequence.push_back(SequenceEntry());
    }
    sequence[0].tone = 0;
    sequence[0].loudness = 1;
    sequence[1].tone = 0.2;
    sequence[1].loudness = 1;
    sequence[2].tone = 0.4;
    sequence[2].loudness = 1;
    sequence[3].tone = 0.6;
    sequence[3].loudness = 1;
    sequence[4].tone = 0.2;
    sequence[4].loudness = 1;
    sequence[6].tone = 0.6;
    sequence[6].loudness = 1;
    
}

void BasicSequencer::drawWindow(){
    update();
    ImGui::Begin("Basic Sequencer");
    ImGui::Text("The basic sequencer can play sequences");
    ImGui::Checkbox("Play sequence", &playing);
    ImGui::Text("Count: %d", count);
    ImGui::Text("Position: %d", position);
    ImGui::End();
}

void BasicSequencer::update(){
    if(playing){
        if(count == 0){
            SequenceEntry entry = sequence[position];
            if(entry.loudness > 0){
                instrument->pitchSample(entry.tone);
                instrument->playSample();
            }
            position++;
            if(position == sequence.size()){
                position = 0;
            }
        }
        count++;
        if(count == 16){
            count = 0;
        }
    }
    
}
