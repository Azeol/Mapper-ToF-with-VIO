	component Hardware_Qsys is
		port (
			clk_clk           : in  std_logic                     := 'X';             -- clk
			hex3_hex0_export  : out std_logic_vector(31 downto 0);                    -- export
			hex5_hex4_export  : out std_logic_vector(15 downto 0);                    -- export
			ledr_export       : out std_logic_vector(9 downto 0);                     -- export
			pushbutton_export : in  std_logic_vector(1 downto 0)  := (others => 'X'); -- export
			reset_reset_n     : in  std_logic                     := 'X';             -- reset_n
			switch_export     : in  std_logic_vector(9 downto 0)  := (others => 'X')  -- export
		);
	end component Hardware_Qsys;

	u0 : component Hardware_Qsys
		port map (
			clk_clk           => CONNECTED_TO_clk_clk,           --        clk.clk
			hex3_hex0_export  => CONNECTED_TO_hex3_hex0_export,  --  hex3_hex0.export
			hex5_hex4_export  => CONNECTED_TO_hex5_hex4_export,  --  hex5_hex4.export
			ledr_export       => CONNECTED_TO_ledr_export,       --       ledr.export
			pushbutton_export => CONNECTED_TO_pushbutton_export, -- pushbutton.export
			reset_reset_n     => CONNECTED_TO_reset_reset_n,     --      reset.reset_n
			switch_export     => CONNECTED_TO_switch_export      --     switch.export
		);

