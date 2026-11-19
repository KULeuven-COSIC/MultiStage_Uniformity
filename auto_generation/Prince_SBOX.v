module Prince_SBOX(
	input clk,
	input  [3:0] in1,		// 1st share
	input  [3:0] in2,		// 2nd share
	output [3:0] out1,
	output [3:0] out2
);

	integer j, r;

	wire [15:0] CF_stage0_Out [3:0]; // array of 16-bit output initial stage and 4 coordinate functions
	reg  [15:0] CF_stage0_Reg [3:0];

	reg  [7:0] CF_stage1_Reg [3:0]; // Stage 1
	reg  [3:0] CF_stage2_Reg [3:0]; // Stage 2
	reg  [1:0] CF_stage3_Reg [3:0];
	reg  [3:0] CF_stage4_Reg;

	reg [3:0] in1_reg;
	reg [3:0] in1_reg2;
	reg [3:0] in1_reg3;
	reg [3:0] in1_reg4;
	reg [3:0] in1_reg5;

	reg [1:0] a; // input bits shared
	reg [1:0] b;
	reg [1:0] c;
	reg [1:0] d;

	/*

	x = f(a, b, c, d)
	y = g(d, a, b, c)
	z = h(c, d, a, b)
	t = k(b, c, d, a)

	*/

	// in1 = {d0,c0,b0,a0}
	// in2 = {d1,c1,b1,a1}
	// output should be {t,z,y,x}

	always @(*) begin
		a = {in2[0],in1[0]}; // {a1, a0}
		b = {in2[1],in1[1]};
		c = {in2[2],in1[2]};
		d = {in2[3],in1[3]}; // {d1, d0}
	end

	genvar i, k;

	generate
	//NonLinear Layer
	for (k=0; k < 4; k=k+1) begin
		for (i=0; i < 16; i=i+1) begin: Inst
			Prince_NF_CF_stage0 #(.CoordinateFunction(k), .ComponentFunction(i)) CF_State0_Inst (
				.a(a),
				.b(b),
				.c(c),
				.d(d),
				.q(CF_stage0_Out[k][i])
			);
			end
		end
	endgenerate

	always @(posedge clk) begin
		for(r = 0; r < 4; r=r+1) begin
			CF_stage0_Reg[r] <= CF_stage0_Out[r];
			for(j = 0; j < 8; j=j+1) begin
				CF_stage1_Reg[r][j] <= CF_stage0_Reg[r][2*j] ^ CF_stage0_Reg[r][2*j+1];
			end
			for(j = 0; j < 4; j=j+1) begin
				CF_stage2_Reg[r][j] <= CF_stage1_Reg[r][2*j] ^ CF_stage1_Reg[r][2*j+1];
			end
			for(j = 0; j < 2; j=j+1) begin
				CF_stage3_Reg[r][j] <= CF_stage2_Reg[r][2*j] ^ CF_stage2_Reg[r][2*j+1];
			end
			CF_stage4_Reg[r] <= CF_stage3_Reg[r][0] ^ CF_stage3_Reg[r][1];
		end

		in1_reg <= in1;
		in1_reg2 <= in1_reg;
		in1_reg3 <= in1_reg2;
		in1_reg4 <= in1_reg3;
		in1_reg5 <= in1_reg4;
	end

	assign out1 = CF_stage4_Reg[3:0];
	assign out2 = {in1_reg5[1], in1_reg5[2], in1_reg5[3], in1_reg5[0]};

endmodule