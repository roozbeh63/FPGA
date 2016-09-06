# 
# Synthesis run script generated by Vivado
# 

set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000
create_project -in_memory -part xc7z020clg484-1

set_param project.singleFileAddWarning.threshold 0
set_param project.compositeFile.enableAutoGeneration 0
set_param synth.vivado.isSynthRun true
set_property webtalk.parent_dir /home/roozbeh/vivado/ip_repo/edit_myip2_v1_0.cache/wt [current_project]
set_property parent.project_path /home/roozbeh/vivado/ip_repo/edit_myip2_v1_0.xpr [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property target_language VHDL [current_project]
set_property board_part em.avnet.com:zed:part0:1.3 [current_project]
set_property ip_repo_paths {
  /home/roozbeh/vivado/ip_repo/myip2_1.0
  /home/roozbeh/vivado/ip_repo/myip_1.0
} [current_project]
read_vhdl -library xil_defaultlib {
  /home/roozbeh/vivado/ip_repo/myip2_1.0/hdl/myip2_v1_0_S00_AXI.vhd
  /home/roozbeh/vivado/ip_repo/myip2_1.0/hdl/myip2_v1_0.vhd
}
foreach dcp [get_files -quiet -all *.dcp] {
  set_property used_in_implementation false $dcp
}

synth_design -top myip2_v1_0 -part xc7z020clg484-1


write_checkpoint -force -noxdef myip2_v1_0.dcp

catch { report_utilization -file myip2_v1_0_utilization_synth.rpt -pb myip2_v1_0_utilization_synth.pb }
