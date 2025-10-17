library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity i2c_driver_e is
	Port (
		-- I2C Physical pins
		SDA    : inout std_logic;
		SCL    : inout std_logic;

		-- RST and CLK
		clk    : in std_logic;
		rst_n  : in std_logic;

		-- Signal coming from Nios2
		sda_oe : in std_logic;
		scl_oe : in std_logic;
		sda_in : out std_logic;
		scl_in : out std_logic
	);
end i2c_driver_e;

architecture i2c_driver_a of i2c_driver_e is
begin
	
	--SDA
   SDA <= '0' when sda_oe = '1' else 'Z';  -- open-drain
   sda_in <= SDA;                          -- bus reading

   -- SCL
   SCL <= '0' when scl_oe = '1' else 'Z';
   scl_in <= SCL;

end i2c_driver_a;
