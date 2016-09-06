# Definitional proc to organize widgets for parameters.
proc init_gui { IPINST } {
  ipgui::add_param $IPINST -name "Component_Name"
  #Adding Page
  set Page_0 [ipgui::add_page $IPINST -name "Page 0"]
  ipgui::add_param $IPINST -name "FREQ" -parent ${Page_0}
  ipgui::add_param $IPINST -name "GC_WIDTH" -parent ${Page_0}


}

proc update_PARAM_VALUE.FREQ { PARAM_VALUE.FREQ } {
	# Procedure called to update FREQ when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.FREQ { PARAM_VALUE.FREQ } {
	# Procedure called to validate FREQ
	return true
}

proc update_PARAM_VALUE.GC_WIDTH { PARAM_VALUE.GC_WIDTH } {
	# Procedure called to update GC_WIDTH when any of the dependent parameters in the arguments change
}

proc validate_PARAM_VALUE.GC_WIDTH { PARAM_VALUE.GC_WIDTH } {
	# Procedure called to validate GC_WIDTH
	return true
}


proc update_MODELPARAM_VALUE.GC_WIDTH { MODELPARAM_VALUE.GC_WIDTH PARAM_VALUE.GC_WIDTH } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.GC_WIDTH}] ${MODELPARAM_VALUE.GC_WIDTH}
}

proc update_MODELPARAM_VALUE.FREQ { MODELPARAM_VALUE.FREQ PARAM_VALUE.FREQ } {
	# Procedure called to set VHDL generic/Verilog parameter value(s) based on TCL parameter value
	set_property value [get_property value ${PARAM_VALUE.FREQ}] ${MODELPARAM_VALUE.FREQ}
}

