	component NIOSII_QSYS is
		port (
			clk_clk                              : in  std_logic                     := 'X';             -- clk
			reset_reset_n                        : in  std_logic                     := 'X';             -- reset_n
			switch_external_connection_export    : in  std_logic_vector(9 downto 0)  := (others => 'X'); -- export
			pb_external_connection_export        : in  std_logic_vector(1 downto 0)  := (others => 'X'); -- export
			ledr_external_connection_export      : out std_logic_vector(9 downto 0);                     -- export
			i2c_i2c_serial_sda_in                : in  std_logic                     := 'X';             -- sda_in
			i2c_i2c_serial_scl_in                : in  std_logic                     := 'X';             -- scl_in
			i2c_i2c_serial_sda_oe                : out std_logic;                                        -- sda_oe
			i2c_i2c_serial_scl_oe                : out std_logic;                                        -- scl_oe
			uart_external_connection_rxd         : in  std_logic                     := 'X';             -- rxd
			uart_external_connection_txd         : out std_logic;                                        -- txd
			hex3_hex0_external_connection_export : out std_logic_vector(31 downto 0)                     -- export
		);
	end component NIOSII_QSYS;

	u0 : component NIOSII_QSYS
		port map (
			clk_clk                              => CONNECTED_TO_clk_clk,                              --                           clk.clk
			reset_reset_n                        => CONNECTED_TO_reset_reset_n,                        --                         reset.reset_n
			switch_external_connection_export    => CONNECTED_TO_switch_external_connection_export,    --    switch_external_connection.export
			pb_external_connection_export        => CONNECTED_TO_pb_external_connection_export,        --        pb_external_connection.export
			ledr_external_connection_export      => CONNECTED_TO_ledr_external_connection_export,      --      ledr_external_connection.export
			i2c_i2c_serial_sda_in                => CONNECTED_TO_i2c_i2c_serial_sda_in,                --                i2c_i2c_serial.sda_in
			i2c_i2c_serial_scl_in                => CONNECTED_TO_i2c_i2c_serial_scl_in,                --                              .scl_in
			i2c_i2c_serial_sda_oe                => CONNECTED_TO_i2c_i2c_serial_sda_oe,                --                              .sda_oe
			i2c_i2c_serial_scl_oe                => CONNECTED_TO_i2c_i2c_serial_scl_oe,                --                              .scl_oe
			uart_external_connection_rxd         => CONNECTED_TO_uart_external_connection_rxd,         --      uart_external_connection.rxd
			uart_external_connection_txd         => CONNECTED_TO_uart_external_connection_txd,         --                              .txd
			hex3_hex0_external_connection_export => CONNECTED_TO_hex3_hex0_external_connection_export  -- hex3_hex0_external_connection.export
		);

