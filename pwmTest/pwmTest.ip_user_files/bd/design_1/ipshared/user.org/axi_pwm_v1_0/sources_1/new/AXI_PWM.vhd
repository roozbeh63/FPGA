----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/02/2016 11:59:42 AM
-- Design Name: 
-- Module Name: AXI_PWM - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.ALL;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity AXI_PWM is
    generic(
    GC_WIDTH : integer :=  8;  -- FIFO data width
    FREQ : integer := 16
);
    Port ( 
    clk : in std_logic;

     -- axi4 stream slave (data input)
     s_axis_tdata  : in  std_logic_vector(GC_WIDTH-1 downto 0);
     s_axis_tvalid : in  std_logic;
     s_axis_tready : out std_logic;
    
    PWM : out std_logic
    
);
end AXI_PWM;

architecture Behavioral of AXI_PWM is

begin
pwm_i : entity work.pwmGenerator
generic map(
    GC_WIDTH => GC_WIDTH,
    FREQ => FREQ
)
port map(
    clk => clk, 
    s_axis_tdata => s_axis_tdata,
    s_axis_tvalid => s_axis_tvalid, 
    s_axis_tready => s_axis_tready
);

end Behavioral;
