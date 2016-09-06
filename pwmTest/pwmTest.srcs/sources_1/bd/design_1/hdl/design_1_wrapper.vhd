--Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2016.2 (lin64) Build 1577090 Thu Jun  2 16:32:35 MDT 2016
--Date        : Tue Sep  6 14:41:58 2016
--Host        : roozbeh-ThinkPad-T520 running 64-bit Linux Mint 18 Sarah
--Command     : generate_target design_1_wrapper.bd
--Design      : design_1_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity design_1_wrapper is
  port (
    PWM : out STD_LOGIC;
    button_l : in STD_LOGIC;
    clk : in STD_LOGIC;
    switches : in STD_LOGIC_VECTOR ( 7 downto 0 )
  );
end design_1_wrapper;

architecture STRUCTURE of design_1_wrapper is
  component design_1 is
  port (
    PWM : out STD_LOGIC;
    clk : in STD_LOGIC;
    switches : in STD_LOGIC_VECTOR ( 7 downto 0 );
    button_l : in STD_LOGIC
  );
  end component design_1;
begin
design_1_i: component design_1
     port map (
      PWM => PWM,
      button_l => button_l,
      clk => clk,
      switches(7 downto 0) => switches(7 downto 0)
    );
end STRUCTURE;
