----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/02/2016 10:25:44 AM
-- Design Name: 
-- Module Name: axis_tiny_fifo - Behavioral
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

entity axis_tiny_fifo is
    generic(
        GC_WIDTH : integer :=  8; 
    GC_DEPTH : integer := 32);
    
    Port ( 
    aclk          : in std_logic;
     -- axi4 stream slave (data input)
     s_axis_tdata  : in  std_logic_vector(GC_WIDTH-1 downto 0);
     s_axis_tvalid : in  std_logic;
     s_axis_tready : out std_logic;
     -- axi4 stream master (data output)
     m_axis_tdata  : out std_logic_vector(GC_WIDTH-1 downto 0);
     m_axis_tvalid : out std_logic;
     m_axis_tready : in  std_logic;
     
     -- status signals
     index         : out signed(5 downto 0));
end axis_tiny_fifo;

architecture Behavioral of axis_tiny_fifo is

begin
  tiny_fifo_i : entity work.FIFO
  generic map (
    GC_WIDTH => GC_WIDTH,
    GC_DEPTH => GC_DEPTH)
  port map (
    clk            => aclk,
    fifo_in_data   => s_axis_tdata, 
    fifo_in_valid  => s_axis_tvalid, 
    fifo_in_ready  => s_axis_tready,
    fifo_out_data  => m_axis_tdata, 
    fifo_out_valid => m_axis_tvalid,
    fifo_out_ready => m_axis_tready,
    fifo_index     => index);

end Behavioral;
