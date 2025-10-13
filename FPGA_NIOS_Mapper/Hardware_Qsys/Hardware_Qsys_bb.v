
module Hardware_Qsys (
	clk_clk,
	hex3_hex0_export,
	hex5_hex4_export,
	ledr_export,
	pushbutton_export,
	reset_reset_n,
	switch_export);	

	input		clk_clk;
	output	[31:0]	hex3_hex0_export;
	output	[15:0]	hex5_hex4_export;
	output	[9:0]	ledr_export;
	input	[1:0]	pushbutton_export;
	input		reset_reset_n;
	input	[9:0]	switch_export;
endmodule
