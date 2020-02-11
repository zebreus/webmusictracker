#include <BasicSequencer.hpp>

BasicSequencer::BasicSequencer(BasicInstrument* instrument){
    this->instrument = instrument;
    for(int i = 0;i<sequenceLength;i++){
        sequence.push_back(SequenceEntry());
    }
    sequence[0].tone = TONE_VALUE_0;
    sequence[0].velocity = 1;
    sequence[1].tone = TONE_VALUE_1;
    sequence[1].velocity = 1;
    sequence[2].tone = TONE_VALUE_2;
    sequence[2].velocity = 1;
    sequence[3].tone = TONE_VALUE_3;
    sequence[3].velocity = 1;
    sequence[4].tone = TONE_VALUE_4;
    sequence[4].velocity = 1;
    sequence[5].tone = TONE_VALUE_5;
    sequence[5].velocity = 1;
    sequence[6].tone = TONE_VALUE_6;
    sequence[6].velocity = 1;
    sequence[7].tone = TONE_VALUE_7;
    sequence[7].velocity = 1;
    sequence[8].tone = TONE_VALUE_8;
    sequence[8].velocity = 1;
    sequence[9].tone = TONE_VALUE_9;
    sequence[9].velocity = 1;
    sequence[10].tone = TONE_VALUE_10;
    sequence[10].velocity = 1;
    sequence[11].tone = TONE_VALUE_11;
    sequence[11].velocity = 1;
    
}

void BasicSequencer::drawWindow(){
    update();
    static bool show_sequencer_window = true;
    ImGui::Begin("Sequencer", &show_sequencer_window, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if(!playing){
        if(ImGui::Button("Play")){
            count = 0;
            playing = true;
        }
    }else{
        if(ImGui::Button("Stop")){
            playing = false;
        }
    }
    //ImGui::Text("Count: %d", count);
    ImGui::Text("Pos: %d", position);
    
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Queue: %d", io.InputQueueCharacters.Size);
    drawSequencerColum(16);
    ImGui::End();
}

void BasicSequencer::update(){
    if(playing){
        if(count == 0){
            SequenceEntry entry = sequence[position];
            if(entry.velocity > 0){
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

void BasicSequencer::drawSequencerColum(int editorHeight){
    ImGui::Text("Sequencer");
    
    ImGuiIO& io = ImGui::GetIO();
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(1.0f,1.0f,1.0f,0.0f));
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(1.0f,1.0f,1.0f,0.0f));
    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.0f,0.1f,0.65f,1.0f));
    
    ImGui::Separator();
    
    int center = editorHeight/2-1;
    
    //These values will be changed if an element is clicked
    int nextPosition = position;
    int nextSelectedPropertyId = selectedPropertyId;
    
    for(int i = 0; i<editorHeight; i++){
        bool isCenter = ( i==center );
        
        //Draw entry
        int entryId = i+position-center;
        if(entryId >= 0 && entryId < sequence.size()){
            SequenceEntry& entry = sequence[entryId];
            string entryIdChars = toHexString(entryId);
            string toneChars = entry.toneString();
            string gateChars = entry.gateString();
            string velocityChars = entry.velocityString();
            ImVec4 editColor = ImVec4(0.0f,0.2f,0.7f,1.0f);
            
            //TODO make number clickable
            //TODO selected entry should keep color if hovered
            if(isCenter) pushSelectableColor(ImVec4(0.5f,0.0f,0.0f,1.0f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f,1.0f,0.0f,1.0f));
            ImGui::PushID(i*102-2);
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            if(ImGui::Selectable(entryIdChars.c_str(), isCenter)){
                nextPosition = entryId;
            }
            ImGui::PopItemFlag();
            ImGui::PopID();
            ImGui::PopStyleColor();
            if(isCenter) popSelectableColor();
            
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(4,0));
            ImGui::SameLine();
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f,0.3f,1.0f,1.0f));
            ImGui::PushID(i*102+0);
            if(ImGui::Selectable(toneChars.c_str(), isCenter && selectedPropertyId==0, 0, ImGui::CalcTextSize(toneChars.c_str()))){
                nextPosition = entryId;
                nextSelectedPropertyId = 0;
            }
            ImGui::PopID();
            ImGui::PopStyleColor();
            
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(2,0));
            ImGui::SameLine();
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f,1.0f,0.0f,1.0f));
            ImGui::PushID(i*102+1);
            if(ImGui::Selectable(gateChars.c_str(), isCenter && selectedPropertyId==1, 0, ImGui::CalcTextSize(gateChars.c_str()))){
                nextPosition = entryId;
                nextSelectedPropertyId = 1;
            }
            ImGui::PopID();
            ImGui::PopStyleColor();
            
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(2,0));
            ImGui::SameLine();
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f,1.0f,1.0f,1.0f));
            ImGui::PushID(i*102+2);
            if(ImGui::Selectable(velocityChars.c_str(), isCenter && selectedPropertyId==2, 0, ImGui::CalcTextSize(velocityChars.c_str()))){
                nextPosition = entryId;
                nextSelectedPropertyId = 2;
            }
            ImGui::PopID();
            ImGui::PopStyleColor();
            
            ImGui::SameLine();
            ImGui::Selectable("", false, 0);
        }else{
            ImGui::Dummy(ImGui::CalcTextSize("#"));
        }
        
    }
    
    //Set new values for selected items
    position = nextPosition;
    selectedPropertyId = nextSelectedPropertyId;
    
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

void BasicSequencer::pushSelectableColor(const ImVec4& color){
    ImGui::PushStyleColor(ImGuiCol_Header, color);
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, color);
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, color);
}
void BasicSequencer::popSelectableColor(){
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

string BasicSequencer::SequenceEntry::toneString(){
    int semi = ((int)(tone*12)) % 12;
    int oct = ((int)tone)/1;
    
    string name;
    
    //TODO use an enum
    switch (semi){
        case 0:
            name = TONE_NAME_0;
            break;
        case 1:
            name = TONE_NAME_1;
            break;
        case 2:
            name = TONE_NAME_2;
            break;
        case 3:
            name = TONE_NAME_3;
            break;
        case 4:
            name = TONE_NAME_4;
            break;
        case 5:
            name = TONE_NAME_5;
            break;
        case 6:
            name = TONE_NAME_6;
            break;
        case 7:
            name = TONE_NAME_7;
            break;
        case 8:
            name = TONE_NAME_8;
            break;
        case 9:
            name = TONE_NAME_9;
            break;
        case 10:
            name = TONE_NAME_10;
            break;
        case 11:
            name = TONE_NAME_11;
            break;
        default:
            name = "??";
            break;
    }
    
    name.append(to_string(oct+4).substr(0,1));
    return name;
}

string BasicSequencer::SequenceEntry::gateString(){
    stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hex << gate*255;
    return ss.str().substr(0,2);
}

string BasicSequencer::SequenceEntry::velocityString(){
    stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hex << ((int)(velocity*255));
    return ss.str().substr(0,2);
}

string BasicSequencer::toHexString(int number){
    stringstream ss;
    ss << std::setw(2) << std::setfill('0') << hex << number;
    return ss.str().substr(0,2);
}
