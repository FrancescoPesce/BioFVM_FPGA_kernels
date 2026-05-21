# BioFVM kernels

Sources and builds for HLS kernels. HLS kernels can be used for both:
- the Vitis flow (`make xo TARGET=(sw_emu/hw_emu/hw)`) 
- the Vivado flow (`make csynth`)
The Makefile expects that each HLS kernel is defined in a single `.cpp` file under `src` and declared in a single `.hpp` file under `include`, with the same name as the kernel. 

If unit testing is supported for the kernel, a single file named kernel name + `_tb.cpp` can be added under `tb`.
Unit testing can be performed as:
- C-simulation (`make xo_test TARGET=sw_emu`)
- RTL co-simulation (`make hls_cosim`)