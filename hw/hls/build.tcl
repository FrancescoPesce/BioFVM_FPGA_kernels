# Skip first 3 args which are -f, scriptname, --
set script_args [lrange $argv 3 end]

if {[llength $script_args] != 4} {
    puts "Usage: vitis_hls -f build.tcl -- <KERNEL> <PROJ_ROOT> <PART_NAME> <TARGET>"
    puts "You passed [llength $script_args] arguments."
    exit 1
}

set KERNEL    [lindex $script_args 0]
set PROJ_ROOT [lindex $script_args 1]
set PART_NAME [lindex $script_args 2]
set TARGET    [lindex $script_args 3]

puts "DEBUG: PROJECT ROOT = '$PROJ_ROOT'"
puts "DEBUG: KERNEL = '$KERNEL'"
puts "DEBUG: PART_NAME = '$PART_NAME'"
puts "DEBUG: TARGET = '$TARGET'"
flush stdout

open_project $PROJ_ROOT
set_top $KERNEL
add_files "$PROJ_ROOT/${KERNEL}.cpp"

if {$TARGET eq "csim" || $TARGET eq "cosim" || $TARGET eq "cosim_waveform"} {
    add_files -tb "$PROJ_ROOT/${KERNEL}_tb.cpp"
}

open_solution solution1
set_part $PART_NAME
create_clock -period 4 -name default

if {$TARGET eq "csim"} {
    csim_design
} elseif {$TARGET eq "csynth"} {
    csynth_design
} elseif {$TARGET eq "cosim"} {
    csynth_design
    cosim_design 
} elseif {$TARGET eq "cosim_waveform"} {
    csynth_design
    cosim_design -tool xsim -wave_debug -trace_level all
} else {
    puts "Invalid target: ${TARGET}! Options: csim, csynth, cosim, cosim_waveform."
}

exit