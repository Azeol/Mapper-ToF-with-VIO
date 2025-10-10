
module NIOSII_QSYS (
	clk_clk,
	reset_reset_n,
	switch_external_connection_export,
	pb_external_connection_export,
	ledr_external_connection_export,
	i2c_i2c_serial_sda_in,
	i2c_i2c_serial_scl_in,
	i2c_i2c_serial_sda_oe,
	i2c_i2c_serial_scl_oe,
	uart_external_connection_rxd,
	uart_external_connection_txd,
	hex3_hex0_external_connection_export);	

	input		clk_clk;
	input		reset_reset_n;
	input	[9:0]	switch_external_connection_export;
	input	[1:0]	pb_external_connection_export;
	output	[9:0]	ledr_external_connection_export;
	input		i2c_i2c_serial_sda_in;
	input		i2c_i2c_serial_scl_in;
	output		i2c_i2c_serial_sda_oe;
	output		i2c_i2c_serial_scl_oe;
	input		uart_external_connection_rxd;
	output		uart_external_connection_txd;
	output	[31:0]	hex3_hex0_external_connection_export;
endmodule
