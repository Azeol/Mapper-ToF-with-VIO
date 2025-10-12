
module mapperNios (
	clk_clk,
	hex3_hex0_export,
	i2c_serial_sda_in,
	i2c_serial_scl_in,
	i2c_serial_sda_oe,
	i2c_serial_scl_oe,
	ledr_export,
	pushbutton_export,
	reset_reset_n,
	switch_export,
	uart_rxd,
	uart_txd,
	hex5_hex4_export);	

	input		clk_clk;
	output	[31:0]	hex3_hex0_export;
	input		i2c_serial_sda_in;
	input		i2c_serial_scl_in;
	output		i2c_serial_sda_oe;
	output		i2c_serial_scl_oe;
	output	[9:0]	ledr_export;
	input	[1:0]	pushbutton_export;
	input		reset_reset_n;
	input	[9:0]	switch_export;
	input		uart_rxd;
	output		uart_txd;
	output	[15:0]	hex5_hex4_export;
endmodule
