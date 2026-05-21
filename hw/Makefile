ECHO=@echo

.PHONY: help clean hls all \
		backward_pe \
        forward_pe \
		forward_to_backward \
        pe_loopback \
        naive_mem2stream \
        naive_stream2mem \
		stream_c \
        stream_constant1 \
        stream_denom

PLATFORM := xilinx_u250_gen3x16_xdma_4_1_202210_1
DEVICE := xcu250-figd2104-2L-e
TARGET := sw_emu

all: hls

help:
	$(ECHO) "Makefile Usage:"
	$(ECHO) "  make -j all"
	$(ECHO) ""
	$(ECHO) "  make clean"
	$(ECHO) "      Remove all the generated files."
	$(ECHO) ""

################## call other makefiles
HLS_BUILD_COMMAND = $(MAKE) -C hls xo \
	PLATFORM=$(PLATFORM) \
	DEVICE=$(DEVICE) \
	TARGET=$(TARGET)

hls: \
	backward_pe \
	forward_pe \
	forward_to_backward \
	pe_loopback \
	naive_mem2stream \
	naive_stream2mem \
	stream_c \
	stream_constant1 \
	stream_denom

backward_pe:
	$(HLS_BUILD_COMMAND) KERNEL=backward_pe

forward_pe:
	$(HLS_BUILD_COMMAND) KERNEL=forward_pe

forward_to_backward:
	$(HLS_BUILD_COMMAND) KERNEL=forward_to_backward

pe_loopback:
	$(HLS_BUILD_COMMAND) KERNEL=pe_loopback

naive_mem2stream:
	$(HLS_BUILD_COMMAND) KERNEL=naive_mem2stream

naive_stream2mem:
	$(HLS_BUILD_COMMAND) KERNEL=naive_stream2mem

stream_c:
	$(HLS_BUILD_COMMAND) KERNEL=stream_c

stream_constant1:
	$(HLS_BUILD_COMMAND) KERNEL=stream_constant1

stream_denom:
	$(HLS_BUILD_COMMAND) KERNEL=stream_denom

################## clean up
clean:
	$(MAKE) -C hls clean