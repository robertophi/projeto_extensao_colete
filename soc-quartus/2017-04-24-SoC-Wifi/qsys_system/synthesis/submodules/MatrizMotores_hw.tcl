# TCL File Generated by Component Editor 13.0sp1
# Wed Jan 25 14:18:09 BRT 2017
# DO NOT MODIFY


# 
# MatrizMotores "MatrizMotores" v1.0
#  2017.01.25.14:18:09
# 
# 

# 
# request TCL package from ACDS 13.1
# 
package require -exact qsys 13.1


# 
# module MatrizMotores
# 
set_module_property DESCRIPTION ""
set_module_property NAME MatrizMotores
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME MatrizMotores
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property ANALYZE_HDL AUTO
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL matriz_interface
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
add_fileset_file fsm_commands.vhd VHDL PATH fsm_commands.vhd
add_fileset_file matriz.vhd VHDL PATH matriz.vhd
add_fileset_file matriz_interface.vhd VHDL PATH matriz_interface.vhd TOP_LEVEL_FILE
add_fileset_file MatrizMotores_hw.tcl OTHER PATH MatrizMotores_hw.tcl
add_fileset_file motor.vhd VHDL PATH motor.vhd
add_fileset_file motor_fsm.vhd VHDL PATH motor_fsm.vhd
add_fileset_file MotorMatrixControl.vhd VHDL PATH MotorMatrixControl.vhd
add_fileset_file reg.vhd VHDL PATH reg.vhd
add_fileset_file register_data_width.vhd VHDL PATH register_data_width.vhd
add_fileset_file temporizador.vhd VHDL PATH temporizador.vhd
add_fileset_file timer_pwm.vhd VHDL PATH timer_pwm.vhd


# 
# parameters
# 
add_parameter rows POSITIVE 4
set_parameter_property rows DEFAULT_VALUE 4
set_parameter_property rows DISPLAY_NAME rows
set_parameter_property rows TYPE POSITIVE
set_parameter_property rows UNITS None
set_parameter_property rows ALLOWED_RANGES 1:2147483647
set_parameter_property rows HDL_PARAMETER true
add_parameter cols POSITIVE 8 ""
set_parameter_property cols DEFAULT_VALUE 8
set_parameter_property cols DISPLAY_NAME cols
set_parameter_property cols TYPE POSITIVE
set_parameter_property cols UNITS None
set_parameter_property cols ALLOWED_RANGES 1:2147483647
set_parameter_property cols DESCRIPTION ""
set_parameter_property cols HDL_PARAMETER true
add_parameter time_constant INTEGER 1000000 ""
set_parameter_property time_constant DEFAULT_VALUE 1000000
set_parameter_property time_constant DISPLAY_NAME time_constant
set_parameter_property time_constant TYPE INTEGER
set_parameter_property time_constant UNITS None
set_parameter_property time_constant ALLOWED_RANGES -2147483648:2147483647
set_parameter_property time_constant DESCRIPTION ""
set_parameter_property time_constant HDL_PARAMETER true
add_parameter divisor NATURAL 1 ""
set_parameter_property divisor DEFAULT_VALUE 1
set_parameter_property divisor DISPLAY_NAME divisor
set_parameter_property divisor TYPE NATURAL
set_parameter_property divisor UNITS None
set_parameter_property divisor ALLOWED_RANGES 0:2147483647
set_parameter_property divisor DESCRIPTION ""
set_parameter_property divisor HDL_PARAMETER true


# 
# display items
# 


# 
# connection point reset
# 
add_interface reset reset end
set_interface_property reset associatedClock clock_sink
set_interface_property reset synchronousEdges DEASSERT
set_interface_property reset ENABLED true
set_interface_property reset EXPORT_OF ""
set_interface_property reset PORT_NAME_MAP ""
set_interface_property reset SVD_ADDRESS_GROUP ""

add_interface_port reset rsi_rsink_resetn reset_n Input 1


# 
# connection point aslave
# 
add_interface aslave avalon end
set_interface_property aslave addressUnits WORDS
set_interface_property aslave associatedClock clock_sink
set_interface_property aslave associatedReset reset
set_interface_property aslave bitsPerSymbol 8
set_interface_property aslave burstOnBurstBoundariesOnly false
set_interface_property aslave burstcountUnits WORDS
set_interface_property aslave explicitAddressSpan 0
set_interface_property aslave holdTime 0
set_interface_property aslave linewrapBursts false
set_interface_property aslave maximumPendingReadTransactions 0
set_interface_property aslave readLatency 0
set_interface_property aslave readWaitTime 1
set_interface_property aslave setupTime 0
set_interface_property aslave timingUnits Cycles
set_interface_property aslave writeWaitTime 0
set_interface_property aslave ENABLED true
set_interface_property aslave EXPORT_OF ""
set_interface_property aslave PORT_NAME_MAP ""
set_interface_property aslave SVD_ADDRESS_GROUP ""

add_interface_port aslave avs_aslave_read read Input 1
add_interface_port aslave avs_aslave_write write Input 1
add_interface_port aslave avs_aslave_address address Input 1
add_interface_port aslave avs_aslave_writedata writedata Input 32
add_interface_port aslave avs_aslave_readdata readdata Output 32
set_interface_assignment aslave embeddedsw.configuration.isFlash 0
set_interface_assignment aslave embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment aslave embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment aslave embeddedsw.configuration.isPrintableDevice 0


# 
# connection point pwms
# 
add_interface pwms conduit end
set_interface_property pwms associatedClock clock_sink
set_interface_property pwms associatedReset reset
set_interface_property pwms ENABLED true
set_interface_property pwms EXPORT_OF ""
set_interface_property pwms PORT_NAME_MAP ""
set_interface_property pwms SVD_ADDRESS_GROUP ""

add_interface_port pwms coe_pwms_export export Output rows*cols


# 
# connection point clock_sink
# 
add_interface clock_sink clock end
set_interface_property clock_sink clockRate 0
set_interface_property clock_sink ENABLED true
set_interface_property clock_sink EXPORT_OF ""
set_interface_property clock_sink PORT_NAME_MAP ""
set_interface_property clock_sink SVD_ADDRESS_GROUP ""

add_interface_port clock_sink csi_csink_clock clk Input 1

