module Midori_NF_CF_stage0 #(parameter CoordinateFunction = 0, ComponentFunction = 0) (
	input [1:0] a,
	input [1:0] b,
	input [1:0] c,
	input [1:0] d,
	output q
 );



	generate
		if (CoordinateFunction == 0) begin
			if (ComponentFunction == 0) begin
				assign q =  a[0] ^ c[0] ^ a[0] & c[0] ^ a[0] & b[0] & c[0] ^ d[0] ^ a[0] & d[0] ^ a[0] & b[0] & d[0] ^ b[0] & c[0] & d[0];
			end
			if (ComponentFunction == 1) begin
				assign q =  a[0] & d[1] ^ a[0] & b[0] & d[1] ^ b[0] & c[0] & d[1];
			end
			if (ComponentFunction == 2) begin
				assign q =  a[0] & c[1] ^ a[0] & b[0] & c[1] ^ d[0] ^ b[0] & c[1] & d[0];
			end
			if (ComponentFunction == 3) begin
				assign q =  b[0] & c[1] & d[1];
			end
			if (ComponentFunction == 4) begin
				assign q =  b[1] ^ c[0] ^ a[0] & b[1] & c[0] ^ d[0] ^ a[0] & b[1] & d[0] ^ b[1] & c[0] & d[0];
			end
			if (ComponentFunction == 5) begin
				assign q =  a[0] & b[1] & d[1] ^ b[1] & c[0] & d[1];
			end
			if (ComponentFunction == 6) begin
				assign q =  a[0] & b[1] & c[1] ^ d[0] ^ b[1] & c[1] & d[0];
			end
			if (ComponentFunction == 7) begin
				assign q =  b[1] & c[1] & d[1];
			end
			if (ComponentFunction == 8) begin
				assign q =  b[0] ^ c[0] ^ a[1] & c[0] ^ a[1] & b[0] & c[0] ^ d[0] ^ a[1] & d[0] ^ a[1] & b[0] & d[0];
			end
			if (ComponentFunction == 9) begin
				assign q =  a[1] & d[1] ^ a[1] & b[0] & d[1];
			end
			if (ComponentFunction == 10) begin
				assign q =  a[1] & c[1] ^ a[1] & b[0] & c[1] ^ d[0];
			end
			if (ComponentFunction == 11) begin
				assign q = 1'b0;
			end
			if (ComponentFunction == 12) begin
				assign q =  c[0] ^ a[1] & b[1] & c[0] ^ d[0] ^ a[1] & b[1] & d[0];
			end
			if (ComponentFunction == 13) begin
				assign q =  a[1] & b[1] & d[1];
			end
			if (ComponentFunction == 14) begin
				assign q =  a[1] & b[1] & c[1] ^ d[0];
			end
			if (ComponentFunction == 15) begin
				assign q = 1'b0;
			end
		end

		if (CoordinateFunction == 1) begin
			if (ComponentFunction == 0) begin
				assign q =  b[0] ^ a[0] & c[0] ^ d[0] ^ a[0] & d[0] ^ c[0] & d[0];
			end
			if (ComponentFunction == 1) begin
				assign q =  c[1] ^ a[0] & c[1] ^ c[1] & d[0];
			end
			if (ComponentFunction == 2) begin
				assign q =  c[0];
			end
			if (ComponentFunction == 3) begin
				assign q = 1'b0;
			end
			if (ComponentFunction == 4) begin
				assign q =  a[1] ^ b[0] ^ c[0] ^ a[1] & c[0] ^ a[1] & d[0];
			end
			if (ComponentFunction == 5) begin
				assign q =  a[1] & c[1];
			end
			if (ComponentFunction == 6) begin
				assign q =  c[0];
			end
			if (ComponentFunction == 7) begin
				assign q = 1'b0;
			end
			if (ComponentFunction == 8) begin
				assign q =  a[0] ^ b[0] ^ c[0] ^ a[0] & d[1] ^ c[0] & d[1];
			end
			if (ComponentFunction == 9) begin
				assign q =  c[1] & d[1];
			end
			if (ComponentFunction == 10) begin
				assign q =  c[0];
			end
			if (ComponentFunction == 11) begin
				assign q = 1'b0;
			end
			if (ComponentFunction == 12) begin
				assign q =  b[0] ^ c[0] ^ a[1] & d[1];
			end
			if (ComponentFunction == 13) begin
				assign q = 1'b0;
			end
			if (ComponentFunction == 14) begin
				assign q =  c[0];
			end
			if (ComponentFunction == 15) begin
				assign q = 1'b0;
			end
		end

		if (CoordinateFunction == 2) begin
			if (ComponentFunction == 0) begin
				assign q = 1'b1 ^  b[0] ^ c[0] ^ a[0] & b[0] & c[0] ^ a[0] & d[0] ^ a[0] & b[0] & d[0] ^ b[0] & c[0] & d[0];
			end
			if (ComponentFunction == 1) begin
				assign q =  a[0] & b[1] & c[0] ^ a[0] & b[1] & d[0] ^ b[1] & c[0] & d[0];
			end
			if (ComponentFunction == 2) begin
				assign q =  a[1] ^ b[0] ^ a[1] & b[0] & c[0] ^ a[1] & d[0] ^ a[1] & b[0] & d[0];
			end
			if (ComponentFunction == 3) begin
				assign q =  a[1] & b[1] & c[0] ^ a[1] & b[1] & d[0];
			end
			if (ComponentFunction == 4) begin
				assign q =  a[0] ^ b[0] ^ d[1] ^ a[0] & d[1] ^ a[0] & b[0] & d[1] ^ b[0] & c[0] & d[1];
			end
			if (ComponentFunction == 5) begin
				assign q =  a[0] & b[1] & d[1] ^ b[1] & c[0] & d[1];
			end
			if (ComponentFunction == 6) begin
				assign q =  b[0] ^ a[1] & d[1] ^ a[1] & b[0] & d[1];
			end
			if (ComponentFunction == 7) begin
				assign q =  a[1] & b[1] & d[1];
			end
			if (ComponentFunction == 8) begin
				assign q =  a[0] ^ b[0] ^ a[0] & b[0] & c[1] ^ d[0] ^ b[0] & c[1] & d[0];
			end
			if (ComponentFunction == 9) begin
				assign q =  a[0] & b[1] & c[1] ^ b[1] & c[1] & d[0];
			end
			if (ComponentFunction == 10) begin
				assign q =  b[0] ^ a[1] & b[0] & c[1];
			end
			if (ComponentFunction == 11) begin
				assign q =  a[1] & b[1] & c[1];
			end
			if (ComponentFunction == 12) begin
				assign q =  a[0] ^ b[0] ^ b[0] & c[1] & d[1];
			end
			if (ComponentFunction == 13) begin
				assign q =  b[1] & c[1] & d[1];
			end
			if (ComponentFunction == 14) begin
				assign q =  b[0];
			end
			if (ComponentFunction == 15) begin
				assign q = 1'b0;
			end
		end

		if (CoordinateFunction == 3) begin
			if (ComponentFunction == 0) begin
				assign q = 1'b1 ^  a[0] ^ b[0] ^ a[0] & b[0] ^ c[0] ^ d[0] ^ b[0] & d[0] ^ a[0] & b[0] & d[0] ^ c[0] & d[0] ^ b[0] & c[0] & d[0];
			end
			if (ComponentFunction == 1) begin
				assign q =  a[1] & b[0] ^ a[1] & b[0] & d[0];
			end
			if (ComponentFunction == 2) begin
				assign q =  a[0] ^ b[0] & d[1] ^ a[0] & b[0] & d[1] ^ c[0] & d[1] ^ b[0] & c[0] & d[1];
			end
			if (ComponentFunction == 3) begin
				assign q =  a[1] & b[0] & d[1];
			end
			if (ComponentFunction == 4) begin
				assign q =  a[0] ^ d[0] ^ c[1] & d[0] ^ b[0] & c[1] & d[0];
			end
			if (ComponentFunction == 5) begin
				assign q = 1'b0;
			end
			if (ComponentFunction == 6) begin
				assign q =  a[0] ^ c[1] & d[1] ^ b[0] & c[1] & d[1];
			end
			if (ComponentFunction == 7) begin
				assign q = 1'b0;
			end
			if (ComponentFunction == 8) begin
				assign q =  a[0] ^ a[0] & b[1] ^ c[0] ^ d[0] ^ b[1] & d[0] ^ a[0] & b[1] & d[0] ^ b[1] & c[0] & d[0];
			end
			if (ComponentFunction == 9) begin
				assign q =  a[1] & b[1] ^ a[1] & b[1] & d[0];
			end
			if (ComponentFunction == 10) begin
				assign q =  a[0] ^ b[1] & d[1] ^ a[0] & b[1] & d[1] ^ b[1] & c[0] & d[1];
			end
			if (ComponentFunction == 11) begin
				assign q =  a[1] & b[1] & d[1];
			end
			if (ComponentFunction == 12) begin
				assign q =  a[0] ^ d[0] ^ b[1] & c[1] & d[0];
			end
			if (ComponentFunction == 13) begin
				assign q = 1'b0;
			end
			if (ComponentFunction == 14) begin
				assign q =  a[0] ^ b[1] & c[1] & d[1];
			end
			if (ComponentFunction == 15) begin
				assign q = 1'b0;
			end
		end

	endgenerate

endmodule