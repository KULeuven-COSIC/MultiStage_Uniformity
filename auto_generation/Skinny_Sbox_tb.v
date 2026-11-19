`timescale 1ns / 1ps

module Skinny_Sbox_tb;

	// Inputs
	reg clk;
	reg [3:0] in1;
	reg [3:0] in2;

	// Outputs
	wire [3:0] out1;
	wire [3:0] out2;

	wire [3:0] S_in;
	wire [3:0] S_out;

	reg t, x, y, w;

	integer i;

	assign S_in = in1 ^ in2;
	assign S_out = out1 ^ out2;

	reg  a, b, c, d;

	always @(*) begin
		a = S_in[0];
		b = S_in[1];
		c = S_in[2];
		d = S_in[3];

		x = b ^ a & b ^ c ^ a & c ^ a & b & c ^ d ^ a & d ^ b & d ^ b & c & d ;
		y = a ^ a & b ^ b & c ^ d ^ b & d ^ c & d ^ b & c & d ;
		t = 1'b1 ^ b ^ c ^ b & c ^ d ;
		w = 1'b1 ^ a ^ c ^ d ^ c & d ;
	end


	// Instantiate the Unit Under Test (UUT)
	Skinny_SBOX uut (
		.clk(clk),
		.in1(in1),
		.in2(in2),
		.out1(out1),
		.out2(out2)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		in1 = 0;
		in2 = 0;

		// Wait 100 ns for global reset to finish
		for (i=0; i<256; i=i+1) begin
			in1 = i[3:0];
		in2 = i[7:4];

			#70;
			if(S_out == {w,t,y,x}) begin
				$write("------------------PASS---------------\n");
			end
			else begin
				$write("------------------FAIL---------------\n");
				$write("%x\n%x\n%x\n", S_in, S_out, {w,t,y,x});
				$stop;
			end
		end
		$stop;
		// Add stimulus here

	end
	always #5 clk = ~clk;
endmodule