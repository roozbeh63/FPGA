----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/02/2016 09:41:53 AM
-- Design Name: 
-- Module Name: FIFO - Behavioral
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

entity FIFO is
    generic(
        GC_WIDTH :integer := 8;
        GC_DEPTH : integer := 32
    );
    Port ( 
        clk : in std_logic;
            -- FIFO data input
        fifo_in_data   : in  std_logic_vector(GC_WIDTH-1 downto 0);
        fifo_in_valid  : in  std_logic;
        fifo_in_ready  : out std_logic := '0';
        -- FIFO data output
        fifo_out_data  : out std_logic_vector(GC_WIDTH-1 downto 0) := (others => '0');
        fifo_out_valid : out std_logic := '0';
        fifo_out_ready : in  std_logic;
          -- status signals
        fifo_index     : out signed(5 downto 0)
    );
end FIFO;

architecture Behavioral of FIFO is
    type ram_type is array (GC_DEPTH-1 downto 0) of std_logic_vector (GC_WIDTH-1 downto 0);
    signal fifo            : ram_type := (others => (others => '0'));
    signal fifo_index_i    : signed (5 downto 0) := to_signed(-1, 6);
    signal fifo_empty      : boolean;
    signal fifo_full       : boolean;
    signal fifo_in_enable  : boolean;
    signal fifo_out_enable : boolean;
begin
    fifo_full       <= (fifo_index_i = GC_DEPTH-1);  
    fifo_empty      <= (fifo_index_i = -1);
 
    fifo_in_ready   <= '1' when (not  fifo_full) else '0';
    fifo_out_valid  <= '1' when (not fifo_empty) else '0';
    fifo_in_enable  <= (fifo_in_valid  = '1') and (not fifo_full );
    fifo_out_enable <= (fifo_out_ready = '1') and (not fifo_empty);

    fifo_out_data   <= fifo(to_integer(unsigned(fifo_index_i(4 downto 0))));  
    fifo_index      <= fifo_index_i;

  process (clk)
  begin
    if rising_edge(clk) then
      if fifo_in_enable then
        fifo(GC_DEPTH-1 downto 1) <= fifo(GC_DEPTH-2 downto 0);
        fifo(0)                   <= fifo_in_data;
        if not fifo_out_enable then fifo_index_i <= fifo_index_i + 1; end if;
      elsif fifo_out_enable then fifo_index_i <= fifo_index_i - 1;
      end if;
    end if;  
  end process;


end Behavioral;
