	component mapperNios is
		port (
			clk_clk           : in  std_logic                     := 'X';             -- clk
			hex3_hex0_export  : out std_logic_vector(31 downto 0);                    -- export
			i2c_serial_sda_in : in  std_logic                     := 'X';             -- sda_in
			i2c_serial_scl_in : in  std_logic                     := 'X';             -- scl_in
			i2c_serial_sda_oe : out std_logic;                                        -- sda_oe
			i2c_serial_scl_oe : out std_logic;                                        -- scl_oe
			ledr_export       : out std_logic_vector(9 downto 0);                     -- export
			pushbutton_export : in  std_logic_vector(1 downto 0)  := (others => 'X'); -- export
			reset_reset_n     : in  std_logic                     := 'X';             -- reset_n
			switch_export     : in  std_logic_vector(9 downto 0)  := (others => 'X'); -- export
			uart_rxd          : in  std_logic                     := 'X';             -- rxd
			uart_txd          : out std_logic;                                        -- txd
			hex5_hex4_export  : out std_logic_vector(15 downto 0)                     -- export
		);
	end component mapperNios;

	u0 : component mapperNios
		port map (
			clk_clk           => CONNECTED_TO_clk_clk,           --        clk.clk
			hex3_hex0_export  => CONNECTED_TO_hex3_hex0_export,  --  hex3_hex0.export
			i2c_serial_sda_in => CONNECTED_TO_i2c_serial_sda_in, -- i2c_serial.sda_in
			i2c_serial_scl_in => CONNECTED_TO_i2c_serial_scl_in, --           .scl_in
			i2c_serial_sda_oe => CONNECTED_TO_i2c_serial_sda_oe, --           .sda_oe
			i2c_serial_scl_oe => CONNECTED_TO_i2c_serial_scl_oe, --           .scl_oe
			ledr_export       => CONNECTED_TO_ledr_export,       --       ledr.export
			pushbutton_export => CONNECTED_TO_pushbutton_export, -- pushbutton.export
			reset_reset_n     => CONNECTED_TO_reset_reset_n,     --      reset.reset_n
			switch_export     => CONNECTED_TO_switch_export,     --     switch.export
			uart_rxd          => CONNECTED_TO_uart_rxd,          --       uart.rxd
			uart_txd          => CONNECTED_TO_uart_txd,          --           .txd
			hex5_hex4_export  => CONNECTED_TO_hex5_hex4_export   --  hex5_hex4.export
		);

