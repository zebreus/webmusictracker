#include "SampleLoader.hpp"

SampleLoader::SampleLoader(){
    EM_ASM(
        FS.mkdir('/samples');
        FS.mount(IDBFS, {}, '/samples');
        FS.syncfs(true, function (err) {
            // onerror
        });
    );
}

void SampleLoader::drawWindow(){
    ImGui::Begin("Sample Manager");
    ImGui::Text("The sample manager manages and loads samples");
    if (ImGui::Button("Load samples")){
        loadSamples();
    }
    if (ImGui::Button("Update available samples")){
        loadAvailableSamples();
    }
    
    ImGui::Text("Available samples");
    ImGui::Columns(1, "mycolumns3", false);  // 3-ways, no border
    ImGui::Separator();
    for(const filesystem::path& sampleFile: availableSamples){
        bool selected = sampleFile == *selectedSample;
        if (ImGui::Selectable(sampleFile.stem().c_str(), selected, ImGuiSelectableFlags_SpanAllColumns)) {
            if(selectedSample != nullptr){
                delete selectedSample;
            }
            selectedSample = new filesystem::path(sampleFile);
        }
        ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::Separator();
    
    if (ImGui::Button("Load sample")){
        if(selectedSample != nullptr){
            loadAvailableSample(*selectedSample);
        }
    }
    
    ImGui::End();
}

void SampleLoader::loadSamples(){
    EM_ASM(
        var input = document.createElement("input");
        input.setAttribute("type", "file");
        input.setAttribute("accept", "audio/*");
        input.setAttribute("style", "display:none");
        input.setAttribute("multiple", "true");
        
        document.body.appendChild(input);
        input.click();
        
        input.onchange = function(evt) {
            var files = evt.target.files;
            console.log(files);
            var audioCtx = new (window.AudioContext || window.webkitAudioContext)({
              sampleRate: 96000,
            });
            //TODO better solution to close the audio context
            setTimeout(function(){ audioCtx.close(); }, 30000);
            
            for(file of files ){
                
                var reader = new FileReader();
                reader.onload = function() {
                            console.log(file.name);
                            console.log(reader.result);
                            var filename = '/samples/' + file.name;

                            
                            audioCtx.decodeAudioData(reader.result, function(buffer){
                                    //~ const source = audioCtx.createBufferSource();
                                    //~ source.buffer = buffer;
                                    //~ source.connect(audioCtx.destination);
                                    //~ source.start();
                                    var myAudioBuffer = buffer.getChannelData(0);
                                    var byteAudioBuffer = new Uint8Array(myAudioBuffer.buffer);
                                    console.log(filename);
                                    console.log(myAudioBuffer);
                                    console.log(byteAudioBuffer);
                                    //TODO decode audio
                                    FS.writeFile(filename, byteAudioBuffer);
                                    FS.syncfs(false, function (err) {
                                        // onerror
                                    });
                                }, function(e){
                                    console.log("ERROR decoding audio");
                                }
                            );
                            
                            //TODO find a way to remove input after last file
                            //input.parentNode.removeChild(input);
                };
                reader.readAsArrayBuffer(file);
            }
        }
        //TODO find a way to remove input, when not used
        //input.parentNode.removeChild(input);
    );
}

vector<filesystem::path> SampleLoader::getAvailableSamples(){
    return availableSamples;
}

void SampleLoader::loadAvailableSamples(){
    std::string path = "/samples";
    vector<filesystem::path> ret;
    for (const auto & entry : filesystem::directory_iterator(path)){
        ret.push_back(entry.path());
    }
    availableSamples = ret;
}

vector<float> SampleLoader::loadAvailableSample(filesystem::path samplePath){
    vector<float> sampleData;
    
    ifstream sampleFile( samplePath, ios::in | ios::binary);
    while( sampleFile.is_open() && !sampleFile.eof()){
        float f;
        sampleFile.read((char*)&f, sizeof(f));
        sampleData.push_back(f);
    }
    
    for(int i = 0; i<100;i++){
        cout << sampleData[i] << endl;
    }
    return sampleData;
}
