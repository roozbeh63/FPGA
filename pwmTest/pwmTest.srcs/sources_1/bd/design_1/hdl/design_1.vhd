--Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2016.2 (lin64) Build 1577090 Thu Jun  2 16:32:35 MDT 2016
--Date        : Tue Sep  6 14:41:58 2016
--Host        : roozbeh-ThinkPad-T520 running 64-bit Linux Mint 18 Sarah
--Command     : generate_target design_1.bd
--Design      : design_1
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity design_1 is
  port (
    PWM : out STD_LOGIC;
    button_l : in STD_LOGIC;
    clk : in STD_LOGIC;
    switches : in STD_LOGIC_VECTOR ( 7 downto 0 )
  );
  attribute CORE_GENERATION_INFO : string;
  attribute CORE_GENERATION_INFO of design_1 : entity is "design_1,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=design_1,x_ipVersion=1.00.a,x_ipLanguage=VHDL,numBlks=1,numReposBlks=1,numNonXlnxBlks=1,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=0,numPkgbdBlks=0,bdsource=USER,da_board_cnt=1,synth_mode=Global}";
  attribute HW_HANDOFF : string;
  attribute HW_HANDOFF of design_1 : entity is "design_1.hwdef";
end design_1;

architecture STRUCTURE of design_1 is
  component design_1_AXI_PWM_0_0 is
  port (
    clk : in STD_LOGIC;
    s_axis_tdata : in STD_LOGIC_VECTOR ( 7 downto 0 );
    s_axis_tvalid : in STD_LOGIC;
    s_axis_tready : out STD_LOGIC;
    PWM : out STD_LOGIC
  );
  end component design_1_AXI_PWM_0_0;
  signal AXI_PWM_0_PWM : STD_LOGIC;
  signal button_l_1 : STD_LOGIC;
  signal clk_1 : STD_LOGIC;
  signal switches_1 : STD_LOGIC_VECTOR ( 7 downto 0 );
  signal NLW_AXI_PWM_0_s_axis_tready_UNCONNECTED : STD_LOGIC;
begin
  PWM <= AXI_PWM_0_PWM;
  button_l_1 <= button_l;
  clk_1 <= clk;
  switches_1(7 downto 0) <= switches(7 downto 0);
AXI_PWM_0: component design_1_AXI_PWM_0_0
     port map (
      PWM => AXI_PWM_0_PWM,
      clk => clk_1,
      s_axis_tdata(7 downto 0) => switches_1(7 downto 0),
      s_axis_tready => NLW_AXI_PWM_0_s_axis_tready_UNCONNECTED,
      s_axis_tvalid => button_l_1
    );
end STRUCTURE;
