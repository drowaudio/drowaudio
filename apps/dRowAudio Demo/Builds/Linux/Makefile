# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Debug
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DDEBUG=1 -D_DEBUG=1 -DDROWAUDIO_UNIT_TESTS=1 -DJUCER_LINUX_MAKE_7346DA2A=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../../../module -I../../../../module/dRowAudio/drowaudio/audio/* -I../../../../module/dRowAudio/drowaudio/audio/fft/* -I../../../../module/dRowAudio/drowaudio/audio/filters/* -I../../../../module/dRowAudio/drowaudio/gui/* -I../../../../module/dRowAudio/drowaudio/gui/audiothumbnail/* -I../../../../module/dRowAudio/drowaudio/gui/filebrowser/* -I../../../../module/dRowAudio/drowaudio/maths/* -I../../../../module/dRowAudio/drowaudio/native/* -I../../../../module/dRowAudio/drowaudio/network/* -I../../../../module/dRowAudio/drowaudio/parameters/* -I../../../../module/dRowAudio/drowaudio/streams/* -I../../../../module/dRowAudio/drowaudio/utility/* -I../../../../../JUCE/modules
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 -Wall
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -L/usr/X11R6/lib/ $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt -lcurl

  TARGET := dRowAudio\ Demo
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  JUCE_BINDIR := build
  JUCE_LIBDIR := build
  JUCE_OBJDIR := build/intermediate/Release
  JUCE_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  JUCE_CPPFLAGS := $(DEPFLAGS) -DLINUX=1 -DNDEBUG=1 -DDROWAUDIO_UNIT_TESTS=1 -DJUCER_LINUX_MAKE_7346DA2A=1 -DJUCE_APP_VERSION=1.0.0 -DJUCE_APP_VERSION_HEX=0x10000 $(shell pkg-config --cflags alsa freetype2 libcurl x11 xext xinerama) -pthread -I../../JuceLibraryCode -I../../../../module -I../../../../module/dRowAudio/drowaudio/audio/* -I../../../../module/dRowAudio/drowaudio/audio/fft/* -I../../../../module/dRowAudio/drowaudio/audio/filters/* -I../../../../module/dRowAudio/drowaudio/gui/* -I../../../../module/dRowAudio/drowaudio/gui/audiothumbnail/* -I../../../../module/dRowAudio/drowaudio/gui/filebrowser/* -I../../../../module/dRowAudio/drowaudio/maths/* -I../../../../module/dRowAudio/drowaudio/native/* -I../../../../module/dRowAudio/drowaudio/network/* -I../../../../module/dRowAudio/drowaudio/parameters/* -I../../../../module/dRowAudio/drowaudio/streams/* -I../../../../module/dRowAudio/drowaudio/utility/* -I../../../../../JUCE/modules
  JUCE_CFLAGS += $(CFLAGS) $(JUCE_CPPFLAGS) $(TARGET_ARCH) -Os -Wall
  JUCE_CXXFLAGS += $(CXXFLAGS) $(JUCE_CFLAGS) -std=c++11
  JUCE_LDFLAGS += $(LDFLAGS) $(TARGET_ARCH) -L$(JUCE_BINDIR) -L$(JUCE_LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ $(shell pkg-config --libs alsa freetype2 libcurl x11 xext xinerama) -lGL -ldl -lpthread -lrt -lcurl

  TARGET := dRowAudio\ Demo
  BLDCMD = $(CXX) -o $(JUCE_OUTDIR)/$(TARGET) $(OBJECTS) $(JUCE_LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(JUCE_OUTDIR)/$(TARGET) $(JUCE_OBJDIR)
endif

OBJECTS := \
  $(JUCE_OBJDIR)/FFTDemo_183d0bd.o \
  $(JUCE_OBJDIR)/PitchDetectorComponent_ecaa29bb.o \
  $(JUCE_OBJDIR)/ConnectionComponent_50a9adbf.o \
  $(JUCE_OBJDIR)/LocalDirectoryListBox_9a5523eb.o \
  $(JUCE_OBJDIR)/NetworkDemo_34cd0d31.o \
  $(JUCE_OBJDIR)/RemoteDirectoryListBox_1e458a90.o \
  $(JUCE_OBJDIR)/AudioPlaybackDemo_f7ebabe9.o \
  $(JUCE_OBJDIR)/BufferTransformAudioSource_866e3b7a.o \
  $(JUCE_OBJDIR)/DistortionComponent_ccf4c935.o \
  $(JUCE_OBJDIR)/DistortionDemo_6a738135.o \
  $(JUCE_OBJDIR)/LoopComponent_29b26e8e.o \
  $(JUCE_OBJDIR)/DemoLookAndFeel_74df4d7c.o \
  $(JUCE_OBJDIR)/Main_90ebc5c2.o \
  $(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o \
  $(JUCE_OBJDIR)/MainWindow_499ac812.o \
  $(JUCE_OBJDIR)/TrackInfoComponent_5f5fdbed.o \
  $(JUCE_OBJDIR)/TransportComponent_2551bb5d.o \
  $(JUCE_OBJDIR)/BinaryData_ce4232d4.o \
  $(JUCE_OBJDIR)/dRowAudio_9254bca1.o \
  $(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o \
  $(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o \
  $(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o \
  $(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o \
  $(JUCE_OBJDIR)/juce_audio_utils_c7eb679f.o \
  $(JUCE_OBJDIR)/juce_box2d_95719d14.o \
  $(JUCE_OBJDIR)/juce_core_75b14332.o \
  $(JUCE_OBJDIR)/juce_cryptography_6de2ebff.o \
  $(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o \
  $(JUCE_OBJDIR)/juce_events_d2be882c.o \
  $(JUCE_OBJDIR)/juce_graphics_9c18891e.o \
  $(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o \
  $(JUCE_OBJDIR)/juce_opengl_cd70b4c2.o \

.PHONY: clean

$(JUCE_OUTDIR)/$(TARGET): check-pkg-config $(OBJECTS) $(RESOURCES)
	@echo Linking dRowAudio Demo
	-@mkdir -p $(JUCE_BINDIR)
	-@mkdir -p $(JUCE_LIBDIR)
	-@mkdir -p $(JUCE_OUTDIR)
	@$(BLDCMD)

check-pkg-config:
	@command -v pkg-config >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@pkg-config --print-errors alsa freetype2 libcurl x11 xext xinerama

clean:
	@echo Cleaning dRowAudio Demo
	@$(CLEANCMD)

strip:
	@echo Stripping dRowAudio Demo
	-@$(STRIP) --strip-unneeded $(JUCE_OUTDIR)/$(TARGET)

$(JUCE_OBJDIR)/FFTDemo_183d0bd.o: ../../Source/fft/FFTDemo.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling FFTDemo.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/PitchDetectorComponent_ecaa29bb.o: ../../Source/fft/PitchDetectorComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling PitchDetectorComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/ConnectionComponent_50a9adbf.o: ../../Source/network/ConnectionComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling ConnectionComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LocalDirectoryListBox_9a5523eb.o: ../../Source/network/LocalDirectoryListBox.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LocalDirectoryListBox.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/NetworkDemo_34cd0d31.o: ../../Source/network/NetworkDemo.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling NetworkDemo.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/RemoteDirectoryListBox_1e458a90.o: ../../Source/network/RemoteDirectoryListBox.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling RemoteDirectoryListBox.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/AudioPlaybackDemo_f7ebabe9.o: ../../Source/playback/AudioPlaybackDemo.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling AudioPlaybackDemo.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BufferTransformAudioSource_866e3b7a.o: ../../Source/playback/BufferTransformAudioSource.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BufferTransformAudioSource.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DistortionComponent_ccf4c935.o: ../../Source/playback/DistortionComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DistortionComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DistortionDemo_6a738135.o: ../../Source/playback/DistortionDemo.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DistortionDemo.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/LoopComponent_29b26e8e.o: ../../Source/playback/LoopComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling LoopComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/DemoLookAndFeel_74df4d7c.o: ../../Source/DemoLookAndFeel.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling DemoLookAndFeel.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/MainWindow_499ac812.o: ../../Source/MainWindow.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling MainWindow.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TrackInfoComponent_5f5fdbed.o: ../../Source/TrackInfoComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TrackInfoComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/TransportComponent_2551bb5d.o: ../../Source/TransportComponent.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling TransportComponent.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/dRowAudio_9254bca1.o: ../../JuceLibraryCode/dRowAudio.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling dRowAudio.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_basics_6b797ca1.o: ../../JuceLibraryCode/juce_audio_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_devices_a742c38b.o: ../../JuceLibraryCode/juce_audio_devices.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_formats_5a29c68a.o: ../../JuceLibraryCode/juce_audio_formats.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_processors_dea3173d.o: ../../JuceLibraryCode/juce_audio_processors.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_audio_utils_c7eb679f.o: ../../JuceLibraryCode/juce_audio_utils.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_box2d_95719d14.o: ../../JuceLibraryCode/juce_box2d.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_box2d.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_cryptography_6de2ebff.o: ../../JuceLibraryCode/juce_cryptography.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_cryptography.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

$(JUCE_OBJDIR)/juce_opengl_cd70b4c2.o: ../../JuceLibraryCode/juce_opengl.cpp
	-@mkdir -p $(JUCE_OBJDIR)
	@echo "Compiling juce_opengl.cpp"
	@$(CXX) $(JUCE_CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
