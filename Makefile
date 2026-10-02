CXX       := mpicxx
TARGET    := main
BUILD_DIR := build

CPP_DIRS  := . cpp_files
INC_DIRS  := . hpp_files
SOURCES   := $(foreach dir,$(CPP_DIRS),$(wildcard $(dir)/*.cpp))
OBJECTS   := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
DEPENDS   := $(OBJECTS:.o=.d)

CPPFLAGS  := $(addprefix -I,$(INC_DIRS))
CPPFLAGS  += -DUSE_MPI -I$(mkLisInc)
CXXFLAGS  ?= -std=c++17 -Wall -Wextra -pedantic
LDFLAGS   += -L$(mkLisLib)
LDLIBS    += -llis

.PHONY: all lis-build build-with-lis clean run debug release

all: lis-build

lis-build:
	bash -lc 'source /u/sw/etc/bash.bashrc && module load gcc-glibc && module load lis && $(MAKE) build-with-lis'

build-with-lis: $(TARGET)

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