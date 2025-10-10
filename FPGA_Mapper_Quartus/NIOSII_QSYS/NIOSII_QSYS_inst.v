	NIOSII_QSYS u0 (
		.clk_clk                              (<connected-to-clk_clk>),                              //                           clk.clk
		.reset_reset_n                        (<connected-to-reset_reset_n>),                        //                         reset.reset_n
		.switch_external_connection_export    (<connected-to-switch_external_connection_export>),    //    switch_external_connection.export
		.pb_external_connection_export        (<connected-to-pb_external_connection_export>),        //        pb_external_connection.export
		.ledr_external_connection_export      (<connected-to-ledr_external_connection_export>),      //      ledr_external_connection.export
		.i2c_i2c_serial_sda_in                (<connected-to-i2c_i2c_serial_sda_in>),                //                i2c_i2c_serial.sda_in
		.i2c_i2c_serial_scl_in                (<connected-to-i2c_i2c_serial_scl_in>),                //                              .scl_in
		.i2c_i2c_serial_sda_oe                (<connected-to-i2c_i2c_serial_sda_oe>),                //                              .sda_oe
		.i2c_i2c_serial_scl_oe                (<connected-to-i2c_i2c_serial_scl_oe>),                //                              .scl_oe
		.uart_external_connection_rxd         (<connected-to-uart_external_connection_rxd>),         //      uart_external_connection.rxd
		.uart_external_connection_txd         (<connected-to-uart_external_connection_txd>),         //                              .txd
		.hex3_hex0_external_connection_export (<connected-to-hex3_hex0_external_connection_export>)  // hex3_hex0_external_connection.export
	);

