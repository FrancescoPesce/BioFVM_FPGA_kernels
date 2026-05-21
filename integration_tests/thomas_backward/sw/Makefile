# MIT License

# Copyright (c) 2023 Paolo Salvatore Galfano, Giuseppe Sorrentino

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

ECHO=@echo

.PHONY: help clean run build

TARGET=sw_emu

all: build

help:
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make all"
	$(ECHO) ""
	$(ECHO) "  make run"
	$(ECHO) "      Run the application."
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Remove all the generated files."
	$(ECHO) ""

################## software build for XRT Native API code
CXXFLAGS := -std=c++17 -Wno-deprecated-declarations
CXXFLAGS += -I$(XILINX_XRT)/include -I$(XILINX_HLS)/include

LDFLAGS := -L$(XILINX_XRT)/lib
LDFLAGS += $(LDFLAGS) -lxrt_coreutil -luuid

# You can use them for including something. For example, opencv
#LIBS = `pkg-config --libs opencv`
#CFLAGS = `pkg-config --cflags opencv`

EXECUTABLE := host_$(TARGET)
OVERLAY	   := overlay_$(TARGET).xclbin

HOST_SRCS := src/*.cpp
HOST_INCLUDES := src/*.hpp

build: build/$(EXECUTABLE)

# Eventually add LIBS and CFLAGS
build/$(EXECUTABLE): $(HOST_SRCS) $(HOST_INCLUDES)
	@mkdir -p build
	$(CXX) -o build/$(EXECUTABLE) $(HOST_SRCS) $(CXXFLAGS) $(LDFLAGS) 

build/xrt.ini:
	@mkdir -p build
	@cp xrt.ini build/xrt.ini

################## run
run: build/$(EXECUTABLE) ../link/build/$(OVERLAY) build/xrt.ini
	export XCL_EMULATION_MODE=$(TARGET); \
		cd build; \
		./$(EXECUTABLE) ../../link/build/$(OVERLAY)

################## clean up
clean:
	$(RM) -r build
