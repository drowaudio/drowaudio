# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "DROWAUDIO_UNIT_TESTS" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=0.6.0" -D "JUCE_APP_VERSION_HEX=0x600" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../../../modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt -lcurl
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "DROWAUDIO_UNIT_TESTS" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=0.6.0" -D "JUCE_APP_VERSION_HEX=0x600" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../../../modules
  TARGET := dRowAudio\ Demo
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "DROWAUDIO_UNIT_TESTS" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=0.6.0" -D "JUCE_APP_VERSION_HEX=0x600" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../../../modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -Os
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt -lcurl
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "DROWAUDIO_UNIT_TESTS" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=0.6.0" -D "JUCE_APP_VERSION_HEX=0x600" -I /usr/include -I /usr/include/freetype2 -I ../../JuceLibraryCode -I ../../../../../../modules
  TARGET := dRowAudio\ Demo
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/Main_90ebc5c2.o \
  $(OBJDIR)/DemoLookAndFeel_74df4d7c.o \
  $(OBJDIR)/MainWindow_499ac812.o \
  $(OBJDIR)/MainComponent_a6ffb4a5.o \
  $(OBJDIR)/TrackInfoComponent_5f5fdbed.o \
  $(OBJDIR)/TransportComponent_2551bb5d.o \
  $(OBJDIR)/AudioPlaybackDemo_f7ebabe9.o \
  $(OBJDIR)/LoopComponent_29b26e8e.o \
  $(OBJDIR)/BufferTransformAudioSource_866e3b7a.o \
  $(OBJDIR)/DistortionDemo_6a738135.o \
  $(OBJDIR)/DistortionComponent_ccf4c935.o \
  $(OBJDIR)/NetworkDemo_34cd0d31.o \
  $(OBJDIR)/ConnectionComponent_50a9adbf.o \
  $(OBJDIR)/LocalDirectoryListBox_9a5523eb.o \
  $(OBJDIR)/RemoteDirectoryListBox_1e458a90.o \
  $(OBJDIR)/FFTDemo_183d0bd.o \
  $(OBJDIR)/PitchDetectorComponent_ecaa29bb.o \
  $(OBJDIR)/BinaryData_ce4232d4.o \
  $(OBJDIR)/dRowAudio_77fd40e6.o \
  $(OBJDIR)/juce_audio_basics_de3ff517.o \
  $(OBJDIR)/juce_audio_devices_d3c26015.o \
  $(OBJDIR)/juce_audio_formats_243a9b5.o \
  $(OBJDIR)/juce_audio_processors_895c7cf.o \
  $(OBJDIR)/juce_audio_utils_16a84a55.o \
  $(OBJDIR)/juce_core_236b8bf9.o \
  $(OBJDIR)/juce_data_structures_f6404ef5.o \
  $(OBJDIR)/juce_events_47f553ed.o \
  $(OBJDIR)/juce_graphics_b3c474d1.o \
  $(OBJDIR)/juce_gui_basics_9d6f0bcd.o \
  $(OBJDIR)/juce_gui_extra_f7cbcc95.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking dRowAudio Demo
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning dRowAudio Demo
	@$(CLEANCMD)

strip:
	@echo Stripping dRowAudio Demo
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DemoLookAndFeel_74df4d7c.o: ../../Source/DemoLookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DemoLookAndFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainWindow_499ac812.o: ../../Source/MainWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainComponent_a6ffb4a5.o: ../../Source/MainComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TrackInfoComponent_5f5fdbed.o: ../../Source/TrackInfoComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TrackInfoComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TransportComponent_2551bb5d.o: ../../Source/TransportComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TransportComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AudioPlaybackDemo_f7ebabe9.o: ../../Source/playback/AudioPlaybackDemo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AudioPlaybackDemo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LoopComponent_29b26e8e.o: ../../Source/playback/LoopComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LoopComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BufferTransformAudioSource_866e3b7a.o: ../../Source/playback/BufferTransformAudioSource.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BufferTransformAudioSource.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DistortionDemo_6a738135.o: ../../Source/playback/DistortionDemo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DistortionDemo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/DistortionComponent_ccf4c935.o: ../../Source/playback/DistortionComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling DistortionComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/NetworkDemo_34cd0d31.o: ../../Source/network/NetworkDemo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling NetworkDemo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ConnectionComponent_50a9adbf.o: ../../Source/network/ConnectionComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ConnectionComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LocalDirectoryListBox_9a5523eb.o: ../../Source/network/LocalDirectoryListBox.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LocalDirectoryListBox.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RemoteDirectoryListBox_1e458a90.o: ../../Source/network/RemoteDirectoryListBox.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RemoteDirectoryListBox.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FFTDemo_183d0bd.o: ../../Source/fft/FFTDemo.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FFTDemo.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PitchDetectorComponent_ecaa29bb.o: ../../Source/fft/PitchDetectorComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PitchDetectorComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BinaryData_ce4232d4.o: ../../JuceLibraryCode/BinaryData.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BinaryData.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dRowAudio_77fd40e6.o: ../../../../dRowAudio/dRowAudio.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling dRowAudio.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_de3ff517.o: ../../../../../juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_d3c26015.o: ../../../../../juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_243a9b5.o: ../../../../../juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_895c7cf.o: ../../../../../juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_16a84a55.o: ../../../../../juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_236b8bf9.o: ../../../../../juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_f6404ef5.o: ../../../../../juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_47f553ed.o: ../../../../../juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_b3c474d1.o: ../../../../../juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_9d6f0bcd.o: ../../../../../juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_f7cbcc95.o: ../../../../../juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
