library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity i2c_driver_e is
    Port (
        -- Pins physiques I2C
        SDA    : inout std_logic;
        SCL    : inout std_logic;

        -- Horloge et reset (optionnel selon ton design)
        clk    : in std_logic;
        rst_n  : in std_logic;

        -- Signaux venant du module I2C
        sda_oe : in std_logic;
        scl_oe : in std_logic;
        sda_in : out std_logic;
        scl_in : out std_logic
    );
end i2c_driver_e;

architecture i2c_driver_a of i2c_driver_e is
begin

    -- ==============================
    -- Gestion bidirectionnelle SDA
    -- ==============================
    SDA <= '0' when sda_oe = '1' else 'Z';  -- open-drain
    sda_in <= SDA;                          -- lecture du bus

    -- ==============================
    -- Gestion bidirectionnelle SCL
    -- ==============================
    SCL <= '0' when scl_oe = '1' else 'Z';
    scl_in <= SCL;

end i2c_driver_a;
