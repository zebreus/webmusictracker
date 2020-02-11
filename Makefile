#
# Makefile to use with emscripten
# See https://emscripten.org/docs/getting_started/downloads.html
# for installation instructions.
#
# This Makefile assumes you have loaded emscripten's environment.
# (On Windows, you may need to execute emsdk_env.bat or encmdprompt.bat ahead)
#
# Running `make` will produce three files:
#  - example_emscripten.html
#  - example_emscripten.js
#  - example_emscripten.wasm
#
# All three are needed to run the demo.

IMGUI = imgui/
IMGUI_EXAMPLES = $(IMGUI)/examples/
RESOURCE = res/
SOURCE = src/
OUTPUT = out/

CC = emcc
CXX = em++
EMRUN = emrun
BROWSER = chromium
EXE = $(OUTPUT)/example_emscripten.html
SOURCES = $(SOURCE)/main.cpp
SOURCES += $(SOURCE)/SampleLoader.cpp
SOURCES += $(SOURCE)/BasicInstrument.cpp
SOURCES += $(SOURCE)/BasicSequencer.cpp
SOURCES += $(IMGUI_EXAMPLES)/imgui_impl_sdl.cpp $(IMGUI_EXAMPLES)/imgui_impl_opengl3.cpp
SOURCES += $(IMGUI)/imgui.cpp $(IMGUI)/imgui_demo.cpp $(IMGUI)/imgui_draw.cpp $(IMGUI)/imgui_widgets.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

##---------------------------------------------------------------------
## EMSCRIPTEN OPTIONS
##---------------------------------------------------------------------

EMS += -s USE_SDL=2 -s WASM=1
EMS += -s ALLOW_MEMORY_GROWTH=1
EMS += -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=0
EMS += -s ASSERTIONS=1

# Uncomment next line for size optimizations
#EMS += -Os --closure 1

# Uncomment next line to fix possible rendering bugs with Emscripten version older then 1.39.0 (https://github.com/ocornut/imgui/issues/2877)
#EMS += -s BINARYEN_TRAP_MODE=clamp
#EMS += -s SAFE_HEAP=1    ## Adds overhead

# Emscripten allows preloading a file or folder to be accessible at runtime.
# The Makefile for this example project suggests embedding the misc/fonts/ folder into our application, it will then be accessible as "/fonts"
# See documentation for more details: https://emscripten.org/docs/porting/files/packaging_files.html
# (Default value is 0. Set to 1 to enable file-system and include the misc/fonts/ folder as part of the build.)
#~ USE_FILE_SYSTEM ?= 1
#~ ifeq ($(USE_FILE_SYSTEM), 0)
#~ EMS += -s NO_FILESYSTEM=1 -s FILESYSTEM=0 -DIMGUI_DISABLE_FILE_FUNCTIONS
#~ endif
#~ ifeq ($(USE_FILE_SYSTEM), 1)
#~ LDFLAGS += --no-heap-copy --preload-file $(IMGUI)/misc/fonts@/fonts -lidbfs.js
#~ endif

LDFLAGS += --no-heap-copy -DIMGUI_DISABLE_FILE_FUNCTIONS -lidbfs.js

##---------------------------------------------------------------------
## FINAL BUILD FLAGS
##---------------------------------------------------------------------

CPPFLAGS += -I$(IMGUI_EXAMPLES) -I$(IMGUI) -I$(SOURCE)
#CPPFLAGS += -g
CPPFLAGS += -Wall -Wformat -Os -std=c++17
CPPFLAGS += $(EMS)
LIBS += $(EMS)
LDFLAGS += --shell-file $(RESOURCE)/shell_minimal.html -lopenal

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:$(SOURCE)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_EXAMPLES)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_EXAMPLES)/libs/gl3w/GL/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(EXE)

$(EXE): $(OBJS)
	mkdir -p $(OUTPUT)
	$(CXX) -o $@ $^ $(LIBS) $(LDFLAGS)

clean:
	rm -rf $(EXE) $(OBJS) $(OUTPUT) *.js *.wasm *.wasm.pre

run:
	$(EMRUN) --browser $(BROWSER) $(EXE)
