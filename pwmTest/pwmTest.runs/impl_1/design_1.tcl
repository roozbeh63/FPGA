proc start_step { step } {
  set stopFile ".stop.rst"
  if {[file isfile .stop.rst]} {
    puts ""
    puts "*** Halting run - EA reset detected ***"
    puts ""
    puts ""
    return -code error
  }
  set beginFile ".$step.begin.rst"
  set platform "$::tcl_platform(platform)"
  set user "$::tcl_platform(user)"
  set pid [pid]
  set host ""
  if { [string equal $platform unix] } {
    if { [info exist ::env(HOSTNAME)] } {
      set host $::env(HOSTNAME)
    }
  } else {
    if { [info exist ::env(COMPUTERNAME)] } {
      set host $::env(COMPUTERNAME)
    }
  }
  set ch [open $beginFile w]
  puts $ch "<?xml version=\"1.0\"?>"
  puts $ch "<ProcessHandle Version=\"1\" Minor=\"0\">"
  puts $ch "    <Process Command=\".planAhead.\" Owner=\"$user\" Host=\"$host\" Pid=\"$pid\">"
  puts $ch "    </Process>"
  puts $ch "</ProcessHandle>"
  close $ch
}

proc end_step { step } {
  set endFile ".$step.end.rst"
  set ch [open $endFile w]
  close $ch
}

proc step_failed { step } {
  set endFile ".$step.error.rst"
  set ch [open $endFile w]
  close $ch
}

set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000

start_step init_design
set rc [catch {
  create_msg_db init_design.pb
  create_project -in_memory -part xc7z020clg484-1
  set_property board_part em.avnet.com:zed:part0:1.3 [current_project]
  set_property design_mode GateLvl [current_fileset]
  set_param project.singleFileAddWarning.threshold 0
  set_property webtalk.parent_dir /home/roozbeh/git/pwmTest/pwmTest.cache/wt [current_project]
  set_property parent.project_path /home/roozbeh/git/pwmTest/pwmTest.xpr [current_project]
  set_property ip_repo_paths {
  /home/roozbeh/git/pwmTest/pwmTest.cache/ip
  /home/roozbeh/vivado/FIFO_ip
} [current_project]
  set_property ip_output_repo /home/roozbeh/git/pwmTest/pwmTest.cache/ip [current_project]
  add_files -quiet /home/roozbeh/git/pwmTest/pwmTest.runs/synth_1/design_1.dcp
  read_xdc -mode out_of_context -ref design_1 /home/roozbeh/git/pwmTest/pwmTest.srcs/sources_1/bd/design_1/design_1_ooc.xdc
  read_xdc /home/roozbeh/git/pwmTest/pwmTest.srcs/constrs_1/new/test.xdc
  link_design -top design_1 -part xc7z020clg484-1
  write_hwdef -file design_1.hwdef
  close_msg_db -file init_design.pb
} RESULT]
if {$rc} {
  step_failed init_design
  return -code error $RESULT
} else {
  end_step init_design
}

start_step opt_design
set rc [catch {
  create_msg_db opt_design.pb
  opt_design 
  write_checkpoint -force design_1_opt.dcp
  report_drc -file design_1_drc_opted.rpt
  close_msg_db -file opt_design.pb
} RESULT]
if {$rc} {
  step_failed opt_design
  return -code error $RESULT
} else {
  end_step opt_design
}

start_step place_design
set rc [catch {
  create_msg_db place_design.pb
  implement_debug_core 
  place_design 
  write_checkpoint -force design_1_placed.dcp
  report_io -file design_1_io_placed.rpt
  report_utilization -file design_1_utilization_placed.rpt -pb design_1_utilization_placed.pb
  report_control_sets -verbose -file design_1_control_sets_placed.rpt
  close_msg_db -file place_design.pb
} RESULT]
if {$rc} {
  step_failed place_design
  return -code error $RESULT
} else {
  end_step place_design
}

start_step route_design
set rc [catch {
  create_msg_db route_design.pb
  route_design 
  write_checkpoint -force design_1_routed.dcp
  report_drc -file design_1_drc_routed.rpt -pb design_1_drc_routed.pb
  report_timing_summary -warn_on_violation -max_paths 10 -file design_1_timing_summary_routed.rpt -rpx design_1_timing_summary_routed.rpx
  report_power -file design_1_power_routed.rpt -pb design_1_power_summary_routed.pb -rpx design_1_power_routed.rpx
  report_route_status -file design_1_route_status.rpt -pb design_1_route_status.pb
  report_clock_utilization -file design_1_clock_utilization_routed.rpt
  close_msg_db -file route_design.pb
} RESULT]
if {$rc} {
  step_failed route_design
  return -code error $RESULT
} else {
  end_step route_design
}

start_step write_bitstream
set rc [catch {
  create_msg_db write_bitstream.pb
  catch { write_mem_info -force design_1.mmi }
  write_bitstream -force design_1.bit 
  catch { write_sysdef -hwdef design_1.hwdef -bitfile design_1.bit -meminfo design_1.mmi -file design_1.sysdef }
  catch {write_debug_probes -quiet -force debug_nets}
  close_msg_db -file write_bitstream.pb
} RESULT]
if {$rc} {
  step_failed write_bitstream
  return -code error $RESULT
} else {
  end_step write_bitstream
}

