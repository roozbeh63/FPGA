vlib work
vlib activehdl

vlib activehdl/xil_defaultlib

vmap xil_defaultlib activehdl/xil_defaultlib

vcom -work xil_defaultlib -93 \
"../../../bd/design_1/ipshared/user.org/axi_pwm_v1_0/sources_1/new/pwmGenerator.vhd" \
"../../../bd/design_1/ipshared/user.org/axi_pwm_v1_0/sources_1/new/AXI_PWM.vhd" \
"../../../bd/design_1/ip/design_1_AXI_PWM_0_0/sim/design_1_AXI_PWM_0_0.vhd" \
"../../../bd/design_1/hdl/design_1.vhd" \

