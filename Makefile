CXX       ?= g++
TARGET    := main
BUILD_DIR := build

CPP_DIRS  := . cpp_files
INC_DIRS  := . hpp_files
SOURCES   := $(foreach dir,$(CPP_DIRS),$(wildcard $(dir)/*.cpp))
OBJECTS   := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDS   := $(OBJECTS:.o=.d)

CPPFLAGS  := $(addprefix -I,$(INC_DIRS))
CXXFLAGS  ?= -std=c++17 -Wall -Wextra -pedantic
LDFLAGS   ?=
LDLIBS    ?=

.PHONY: all clean run debug release

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS += -g -O0
debug: clean all

release: CXXFLAGS += -O2 -DNDEBUG
release: clean all

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPENDS)