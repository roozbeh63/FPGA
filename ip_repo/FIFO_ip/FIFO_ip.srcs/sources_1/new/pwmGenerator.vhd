----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/02/2016 10:43:55 AM
-- Design Name: 
-- Module Name: pwmGenerator - Behavioral
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

entity pwmGenerator is
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
end pwmGenerator;

architecture Behavioral of pwmGenerator is
    signal duty : integer;
    signal fifo_in_enable  : boolean;
    signal count : integer;
begin
    
process (clk)
        
        begin
          if rising_edge(clk) then 
            duty <= to_integer(signed((s_axis_tdata(GC_WIDTH-1 downto 0))));
            
          end if;
        end process;
	-- User logic ends

	process (clk)
	--variable to count the clock pulse
	variable count : integer range 0 to 50000;
	begin
		if (rising_edge(clk)) then
		    --increasing the count for each clock cycle
			count:= count+1;
			--setting output to logic 1 when count reach duty cycle value
			--output stays at logic 1 @ duty_cycle <= count <=50000
			if (count < 25000) then
				PWM <= '1';
		     else
		     PWM <= '0';
			end if;
			
			--setting output to logic 0 when count reach 50000
			--output stays at logic 0 @ 50000,0 <= count <= duty_cycle
			if (count = 50000) then
				PWM <= '1';
				count:= 0;
			end if;	
			
		end if;
		
	end process;


end Behavioral;
