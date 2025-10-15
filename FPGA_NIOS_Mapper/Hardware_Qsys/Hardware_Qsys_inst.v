	Hardware_Qsys u0 (
		.clk_clk           (<connected-to-clk_clk>),           //        clk.clk
		.hex3_hex0_export  (<connected-to-hex3_hex0_export>),  //  hex3_hex0.export
		.hex5_hex4_export  (<connected-to-hex5_hex4_export>),  //  hex5_hex4.export
		.i2c_serial_sda_in (<connected-to-i2c_serial_sda_in>), // i2c_serial.sda_in
		.i2c_serial_scl_in (<connected-to-i2c_serial_scl_in>), //           .scl_in
		.i2c_serial_sda_oe (<connected-to-i2c_serial_sda_oe>), //           .sda_oe
		.i2c_serial_scl_oe (<connected-to-i2c_serial_scl_oe>), //           .scl_oe
		.ledr_export       (<connected-to-ledr_export>),       //       ledr.export
		.pushbutton_export (<connected-to-pushbutton_export>), // pushbutton.export
		.reset_reset_n     (<connected-to-reset_reset_n>),     //      reset.reset_n
		.switch_export     (<connected-to-switch_export>),     //     switch.export
		.uart_lidar_rxd    (<connected-to-uart_lidar_rxd>),    // uart_lidar.rxd
		.uart_lidar_txd    (<connected-to-uart_lidar_txd>),    //           .txd
		.uart_pc_rxd       (<connected-to-uart_pc_rxd>),       //    uart_pc.rxd
		.uart_pc_txd       (<connected-to-uart_pc_txd>)        //           .txd
	);

