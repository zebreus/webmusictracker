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
    if (ImGui::Button("Get available samples")){
        getAvailableSamples();
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
            for(file of files ){
                
                var reader = new FileReader();
                reader.onload = function() {
                            var filename = '/samples/' + file.name;
                            console.log(filename);
                            console.log(reader.result);
                            FS.writeFile(filename, reader.result);
                            FS.syncfs(false, function (err) {
                                // onerror
                            });
                            //TODO find a way to remove input after last file
                            //input.parentNode.removeChild(input);
                };
                reader.readAsBinaryString(file);
            }
        }
        //TODO find a way to remove input, when not used
        //input.parentNode.removeChild(input);
    );
}

vector<string> SampleLoader::getAvailableSamples(){
    std::string path = "/samples";
    vector<string> ret;
    for (const auto & entry : filesystem::directory_iterator(path)){
        ret.push_back(entry.path());
    }
    return ret;
}
