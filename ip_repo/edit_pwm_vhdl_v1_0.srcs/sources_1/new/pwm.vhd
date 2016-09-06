----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 08/31/2016 02:42:15 PM
-- Design Name: 
-- Module Name: pwm - Behavioral
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity pwm is
    Port ( clk : in STD_LOGIC;
           pwm_out : out STD_LOGIC);
end pwm;

architecture Behavioral of pwm is

begin

	process (clk)
	--variable to count the clock pulse
	variable count : integer range 0 to 50000;
	--variable to change duty cycle of the pulse
	variable duty_cycle : integer range 0 to 50000;
	--variable to determine whether to increse or decrese the dutycycle
	variable flag : integer range 0 to 1;
	begin
		if (rising_edge(clk)) then
		    --increasing the count for each clock cycle
			count:= count+1;
			--setting output to logic 1 when count reach duty cycle value
			--output stays at logic 1 @ duty_cycle <= count <=50000
			if (count = duty_cycle) then
				pwm_out <= '1';
			end if;
			--setting output to logic 0 when count reach 50000
			--output stays at logic 0 @ 50000,0 <= count <= duty_cycle
			if (count = 50000) then
				pwm_out <= '0';
				count:= 0;
				--after each complete pulse the duty cycle varies
				if(flag = 0) then
					duty_cycle:= duty_cycle+50;
				else
					duty_cycle:= duty_cycle-50;
				end if;
				-- flag changes when duty_cycle reaches max and min value
				if(duty_cycle = 50000) then
					flag:= 1;
				elsif(duty_cycle = 0) then
					flag:= 0;
				end if;
				
			end if;	
			
		end if;
		
	end process;
end Behavioral;
