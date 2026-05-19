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

.PHONY: help clean xo xo_test hls_csim hls_csynth hls_cosim hls_cosim_waveform

PLATFORM := xilinx_u250_gen3x16_xdma_4_1_202210_1
DEVICE := xcu250-figd2104-2L-e
TARGET := sw_emu
KERNEL := broadcast_x3
	
help:
	$(ECHO) "Makefile Usage:"
	$(ECHO) ""
	$(ECHO) "  make xo"
	$(ECHO) "      Generate the xo kernel file."
	$(ECHO) ""
	$(ECHO) "  make xo_test"
	$(ECHO) "      Run the unit test for xo file."
	$(ECHO) ""
	$(ECHO) "  make hls_csim"
	$(ECHO) "      Run the unit test using Vitis HLS csim."
	$(ECHO) ""
	$(ECHO) "  make hls_csynth"
	$(ECHO) "      Compile the HLS kernel to generate the HDL files."
	$(ECHO) ""
	$(ECHO) "  make hls_cosim"
	$(ECHO) "      Run the unit test using Vitis HLS cosim."
	$(ECHO) ""
	$(ECHO) "  make hls_cosim_waveform"
	$(ECHO) "      Run the unit test using Vitis HLS cosim with interactive waveform."
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Remove all the generated files."

INCLUDE_DIR    := include

KERNEL_SRC     := src/$(KERNEL).cpp
KERNEL_INC     := $(INCLUDE_DIR)/$(KERNEL).hpp
DEFINES        := $(INCLUDE_DIR)/defines.hpp
KERNEL_TB_BASE := $(KERNEL)_tb
KERNEL_TB      := tb/$(KERNEL_TB_BASE).cpp

################## Vitis build and test

XOCCFLAGS := --platform $(PLATFORM) -t $(TARGET) -s -g
INCFLGAS  := -I../../$(INCLUDE_DIR)

KERNEL_XO_BASE := $(KERNEL).xo
KERNEL_XO := build/$(TARGET)/$(KERNEL_XO_BASE)
XO_TEST_EXE := build/$(TARGET)/$(KERNEL_TB_BASE)

# Build the HLS kernel.
# Use --optimize 3 to enable post-route optimizations. This may improve the bitstream but SIGNIFICANTLY increase compilation time
$(KERNEL_XO): $(KERNEL_SRC) $(KERNEL_INC)
	@mkdir -p build/$(TARGET)
	cd build/$(TARGET); \
		v++ $(XOCCFLAGS) --kernel $(KERNEL) -c -o $(KERNEL_XO_BASE) ../../$(KERNEL_SRC) $(INCFLGAS)

xo: $(KERNEL_XO)

# Build a unit test for the HLS kernel.
# As every C++ program, you may add libraries like: `pkg-config --libs opencv` `pkg-config --cflags opencv`
$(XO_TEST_EXE): $(KERNEL_TB) $(KERNEL_SRC) $(KERNEL_INC)
	@mkdir -p build/tb_xo
	cd build/tb_xo; \
		$(CXX) -std=c++14 -I. -I$(XILINX_HLS)/include -O2 \
	    ../../$(KERNEL_TB) ../../$(KERNEL_SRC) -o $(KERNEL_TB_BASE) $(INCFLGAS)

# Run the test for the kernel.
xo_test: $(KERNEL_XO) $(XO_TEST_EXE)
	cd build/tb_xo; \
	    ./$(KERNEL_TB_BASE)

################## HLS build and test

HLS_PROJ_NAME := $(KERNEL)_$(shell date +%Y%m%d_%H%M%S)
VITIS_HLS_DIR := build/vitis_hls
HLS_PROJ_DIR := $(VITIS_HLS_DIR)/$(HLS_PROJ_NAME)

# Prepare project directory for Vitis HLS, as expected by the build.tcl script.
$(HLS_PROJ_DIR): $(KERNEL_SRC) $(KERNEL_INC)
	@mkdir -p $(HLS_PROJ_DIR)
	cp $(KERNEL_SRC) $(HLS_PROJ_DIR)
	cp $(KERNEL_INC) $(HLS_PROJ_DIR)
	cp $(DEFINES) $(HLS_PROJ_DIR)
	cp build.tcl $(VITIS_HLS_DIR);

$(HLS_PROJ_DIR)/tb: $(HLS_PROJ_DIR) $(KERNEL_TB)
	cp $(KERNEL_TB) $(HLS_PROJ_DIR);

# Run the unit test for the kernel using Vitis HLS C simulation.
hls_csim: $(HLS_PROJ_DIR)/tb
	@cd $(VITIS_HLS_DIR); \
	    CMD="vitis_hls -f build.tcl -- $(KERNEL) $(HLS_PROJ_NAME) $(DEVICE) csim"; \
	    echo $$CMD; \
	    $$CMD

# Generate the HDL files for the kernel using Vitis HLS.
hls_csynth: $(HLS_PROJ_DIR)
	@cd $(VITIS_HLS_DIR); \
	    CMD="vitis_hls -f build.tcl -- $(KERNEL) $(HLS_PROJ_NAME) $(DEVICE) csynth"; \
	    echo $$CMD; \
	    $$CMD

# Run the unit test for the kernel using Vitis HLS co-simulation.
hls_cosim: $(HLS_PROJ_DIR)/tb
	@cd $(VITIS_HLS_DIR); \
	    CMD="vitis_hls -f build.tcl -- $(KERNEL) $(HLS_PROJ_NAME) $(DEVICE) cosim"; \
	    echo $$CMD; \
	    $$CMD

# Run the unit test for the kernel using Vitis HLS co-simulation with interactive waveform.
# You must have access to the server's display (e.g., running locally or with remote desktop).
hls_cosim_waveform: $(HLS_PROJ_DIR)/tb
	@cd $(VITIS_HLS_DIR); \
	    CMD="vitis_hls -f build.tcl -- $(KERNEL) $(HLS_PROJ_NAME) $(DEVICE) cosim_waveform"; \
	    echo $$CMD; \
	    $$CMD

################## clean up
clean:
	$(RM) -rf build