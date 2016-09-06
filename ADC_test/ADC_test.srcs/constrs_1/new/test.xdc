# ZYBO Pin Assignments
############################
# clk assignment           #
############################
#IO_L11P_T1_SRCC_35	
set_property PACKAGE_PIN Y9 [get_ports clk]
set_property IOSTANDARD LVCMOS33 [get_ports clk]
create_clock -add -name sys_clk_pin -period 25000.0 -waveform {0 250000000} [get_ports clk]

############################
# rst assignment           #
############################
set_property PACKAGE_PIN N15 [get_ports reset]
set_property IOSTANDARD LVCMOS33 [get_ports reset]
############################
# On-board Slide Switches  #
############################
set_property PACKAGE_PIN F22 [get_ports switches[0]]
set_property IOSTANDARD LVCMOS33 [get_ports switches[0]]
set_property PACKAGE_PIN G22 [get_ports switches[1]]
set_property IOSTANDARD LVCMOS33 [get_ports switches[1]]
set_property PACKAGE_PIN H22 [get_ports switches[2]]
set_property IOSTANDARD LVCMOS33 [get_ports switches[2]]
set_property PACKAGE_PIN F21 [get_ports switches[3]]
set_property IOSTANDARD LVCMOS33 [get_ports switches[3]]
set_property PACKAGE_PIN H19 [get_ports switches[4]]
set_property IOSTANDARD LVCMOS33 [get_ports switches[4]]
set_property PACKAGE_PIN H18 [get_ports button_r]
set_property IOSTANDARD LVCMOS33 [get_ports button_r]
set_property PACKAGE_PIN H17 [get_ports button_l]
set_property IOSTANDARD LVCMOS33 [get_ports button_l]
############################
# On-board led             #
############################
set_property PACKAGE_PIN T22 [get_ports pwm]
set_property IOSTANDARD LVCMOS33 [get_ports pwm]
set_property PACKAGE_PIN T21 [get_ports leds[1]]
set_property IOSTANDARD LVCMOS33 [get_ports leds[1]]
set_property PACKAGE_PIN U22 [get_ports leds[2]]
set_property IOSTANDARD LVCMOS33 [get_ports leds[2]]
set_property PACKAGE_PIN U21 [get_ports leds[3]]
set_property IOSTANDARD LVCMOS33 [get_ports leds[3]]