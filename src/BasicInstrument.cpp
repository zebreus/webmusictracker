#include <BasicInstrument.hpp>

BasicInstrument::BasicInstrument(SampleLoader* sampleManager){
    this->sampleManager = sampleManager;

    //Check for the float extension
    if(alIsExtensionPresent("AL_EXT_float32") == AL_TRUE){
        cout << "AL_EXT_FLOAT32 extension is available" << endl;
    }else{
        cout << "AL_EXT_FLOAT32 extension is not available" << endl;
    }
    
    //Init device
    audioDevice = alcOpenDevice(NULL);
    if (!audioDevice){
        cout << "error opening audio device" << endl;
    }else{
        cout << "opened audio device" << endl;
    }

    //Create context
    audioContext = alcCreateContext(audioDevice, NULL);
    if (!alcMakeContextCurrent(audioContext)){
        cout << "error creating context" << endl;
    }else{
        cout << "created context" << endl;
    }
    //~ EM_ASM(
        //~ FS.mkdir('/samples');
        //~ FS.mount(IDBFS, {}, '/samples');
        //~ FS.syncfs(true, function (err) {
            //~ console.log(err);
        //~ });
    //~ );
}

void BasicInstrument::drawWindow(){
    ImGui::Begin("Basic Instrument");
    ImGui::Text("The basic instrument plays a single sample");
    
    if (ImGui::Button("Instrument: "))
        ImGui::OpenPopup("instrument_select_popup");
    ImGui::SameLine();
    ImGui::TextUnformatted(selectedSample == nullptr ? "<<None>>" : selectedSample->stem().c_str());
    if (ImGui::BeginPopup("instrument_select_popup"))
    {
        ImGui::Text("Select your sample");
        ImGui::Separator();
        for(const filesystem::path& sampleFile: sampleManager->getAvailableSamples()){
            if (ImGui::Selectable(sampleFile.stem().c_str())) {
                if(selectedSample != nullptr){
                    delete selectedSample;
                }
                selectedSample = new filesystem::path(sampleFile);
                sampleData = sampleManager->loadAvailableSample(*selectedSample);
                pitchSample(0.0);
            }
            ImGui::NextColumn();
        }
        ImGui::EndPopup();
    }
    
    //~ bool disabled = (selectedSample == nullptr);
    bool disabled = false;
    if (disabled){
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
    if (ImGui::Button("Play sample")){
        playSample();
    }
    if (disabled){
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();
    }
    
    static float pitchValue;
    ImGui::SliderFloat("Pitch slider",  &pitchValue, -2.0f, 2.0f);
    ImGui::SameLine();
    if(ImGui::Button("Apply pitch")){
        pitchSample(pitchValue);
    }
    
    ImGui::End();
}

void BasicInstrument::stretchSample(float stretch){
    cout << "Pitching sample." << endl;
    
    int oldSamples = sampleData.size();
    int newSamples = oldSamples*stretch;
    float factor = (float)oldSamples/(float)newSamples;
    
    vector<float> stretchedSample;
    stretchedSample.reserve(newSamples);
    
    for(int i = 0; i<newSamples; i++){
        stretchedSample.emplace_back(sampleData[i*factor]);
    }
    
    pitchedSampleData = stretchedSample;
}

void BasicInstrument::pitchSample(float pitch){
    float stretch = 1.0f/pow(2.0f, pitch);
    stretchSample(stretch);
}

void BasicInstrument::playSample(){
    cout << "Playing sample." << endl;
    
    //Create source
    ALuint source;
    alGenSources((ALuint)1, &source);
    
    //Create buffer
    ALuint buf;
    alGenBuffers(1, &buf);
    
    if(selectedSample != nullptr){
        alBufferData(buf, 0x10010, pitchedSampleData.data(), pitchedSampleData.size()*sizeof(float), 96000);
    }else{
        /* Fill buffer with Sine-Wave */
        float freq = 440.f;
        int seconds = 4;
        unsigned sample_rate = 22050;
        size_t buf_size = seconds * sample_rate;
        float *samples;
        samples = new float[buf_size];
        for(int i=0; i<buf_size; ++i) {
            samples[i] = sin( (2.f*float(M_PI)*freq)/sample_rate * i );
        }
        alBufferData(buf, 0x10010, samples, buf_size, sample_rate);
    }
    
    //Setup source
    alSourcei(source, AL_BUFFER, buf);
    alSourcePlay(source);
    
    //cleanup
    //TODO cleanup after sound has finished playing
    //alDeleteSources(1, &source);
    //alDeleteBuffers(1, &buf);
}
