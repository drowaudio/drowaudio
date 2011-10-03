# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(TARGET_ARCH),)
  TARGET_ARCH := -march=native
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I "/usr/include" -I "/usr/include/freetype2" -I "../../JuceLibraryCode"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -fPIC -O0
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -shared -L"/usr/X11R6/lib/" -L"../../../../../../juce_source/juce/modules/bin" -lfreetype -lpthread -lrt -lX11 -lGL -lGLU -lXinerama -lasound 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I "/usr/include" -I "/usr/include/freetype2" -I "../../JuceLibraryCode"
  TARGET := Tremolo
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build
  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I "/usr/include" -I "/usr/include/freetype2" -I "../../JuceLibraryCode"
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -O3
  CXXFLAGS += $(CFLAGS) 
  LDFLAGS += -L$(BINDIR) -L$(LIBDIR) -shared -L"/usr/X11R6/lib/" -L"../../../../../../juce_source/juce/modules/bin" -lfreetype -lpthread -lrt -lX11 -lGL -lGLU -lXinerama -lasound 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -I "/usr/include" -I "/usr/include/freetype2" -I "../../JuceLibraryCode"
  TARGET := Tremolo
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
endif

OBJECTS := \
  $(OBJDIR)/PluginProcessor_a059e380.o \
  $(OBJDIR)/PluginEditor_94d4fb09.o \
  $(OBJDIR)/TremoloBufferView_abba89d2.o \
  $(OBJDIR)/TremoloLookAndFeel_9749c89b.o \
  $(OBJDIR)/dRowAudio_66a49a7e.o \
  $(OBJDIR)/juce_audio_basics_15b0217e.o \
  $(OBJDIR)/juce_audio_devices_efd90ebc.o \
  $(OBJDIR)/juce_audio_formats_1e5a585c.o \
  $(OBJDIR)/juce_audio_processors_3aa29d36.o \
  $(OBJDIR)/juce_audio_utils_b936647c.o \
  $(OBJDIR)/juce_core_b5776660.o \
  $(OBJDIR)/juce_data_structures_b481521c.o \
  $(OBJDIR)/juce_events_e22fa2d4.o \
  $(OBJDIR)/juce_graphics_f8def838.o \
  $(OBJDIR)/juce_gui_basics_13b83b4.o \
  $(OBJDIR)/juce_gui_extra_6053123c.o \
  $(OBJDIR)/juce_RTAS_DigiCode1_cbfcb8a9.o \
  $(OBJDIR)/juce_RTAS_DigiCode2_cc0ad02a.o \
  $(OBJDIR)/juce_RTAS_DigiCode3_cc18e7ab.o \
  $(OBJDIR)/juce_RTAS_Wrapper_367a1fff.o \
  $(OBJDIR)/juce_VST_Wrapper_36ac7cd1.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking Tremolo
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning Tremolo
	-@rm -f $(OUTDIR)/$(TARGET)
	-@rm -rf $(OBJDIR)/*
	-@rm -rf $(OBJDIR)

$(OBJDIR)/PluginProcessor_a059e380.o: ../../Source/PluginProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PluginProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PluginEditor_94d4fb09.o: ../../Source/PluginEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PluginEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TremoloBufferView_abba89d2.o: ../../Source/TremoloBufferView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TremoloBufferView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TremoloLookAndFeel_9749c89b.o: ../../Source/TremoloLookAndFeel.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TremoloLookAndFeel.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/dRowAudio_66a49a7e.o: ../../../../../../juce_source/juce/modules/dRowAudio/dRowAudio.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling dRowAudio.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_15b0217e.o: ../../../../../../juce_source/juce/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_efd90ebc.o: ../../../../../../juce_source/juce/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_1e5a585c.o: ../../../../../../juce_source/juce/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_3aa29d36.o: ../../../../../../juce_source/juce/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_b936647c.o: ../../../../../../juce_source/juce/modules/juce_audio_utils/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_b5776660.o: ../../../../../../juce_source/juce/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_b481521c.o: ../../../../../../juce_source/juce/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_e22fa2d4.o: ../../../../../../juce_source/juce/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_f8def838.o: ../../../../../../juce_source/juce/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_13b83b4.o: ../../../../../../juce_source/juce/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_6053123c.o: ../../../../../../juce_source/juce/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_RTAS_DigiCode1_cbfcb8a9.o: ../../../../../../juce_source/juce/modules/juce_audio_plugin_client/RTAS/juce_RTAS_DigiCode1.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_RTAS_DigiCode1.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_RTAS_DigiCode2_cc0ad02a.o: ../../../../../../juce_source/juce/modules/juce_audio_plugin_client/RTAS/juce_RTAS_DigiCode2.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_RTAS_DigiCode2.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_RTAS_DigiCode3_cc18e7ab.o: ../../../../../../juce_source/juce/modules/juce_audio_plugin_client/RTAS/juce_RTAS_DigiCode3.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_RTAS_DigiCode3.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_RTAS_Wrapper_367a1fff.o: ../../../../../../juce_source/juce/modules/juce_audio_plugin_client/RTAS/juce_RTAS_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_RTAS_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST_Wrapper_36ac7cd1.o: ../../../../../../juce_source/juce/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
