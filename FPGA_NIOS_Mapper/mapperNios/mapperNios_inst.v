	mapperNios u0 (
		.clk_clk           (<connected-to-clk_clk>),           //        clk.clk
		.hex3_hex0_export  (<connected-to-hex3_hex0_export>),  //  hex3_hex0.export
		.i2c_serial_sda_in (<connected-to-i2c_serial_sda_in>), // i2c_serial.sda_in
		.i2c_serial_scl_in (<connected-to-i2c_serial_scl_in>), //           .scl_in
		.i2c_serial_sda_oe (<connected-to-i2c_serial_sda_oe>), //           .sda_oe
		.i2c_serial_scl_oe (<connected-to-i2c_serial_scl_oe>), //           .scl_oe
		.ledr_export       (<connected-to-ledr_export>),       //       ledr.export
		.pushbutton_export (<connected-to-pushbutton_export>), // pushbutton.export
		.reset_reset_n     (<connected-to-reset_reset_n>),     //      reset.reset_n
		.switch_export     (<connected-to-switch_export>),     //     switch.export
		.uart_rxd          (<connected-to-uart_rxd>),          //       uart.rxd
		.uart_txd          (<connected-to-uart_txd>),          //           .txd
		.hex5_hex4_export  (<connected-to-hex5_hex4_export>)   //  hex5_hex4.export
	);

