module AES_SBOX(
	input clk,
	input  [7:0] in1,
    input  [7:0] in2,
    output [7:0] out1,
    output [7:0] out2
    );
    
    integer j, r;

	wire [255:0] CF_stage0_Out [7:0];
	reg  [255:0] CF_stage0_Reg [7:0];
	
	reg  [127:0] CF_stage1_Reg [7:0];
	reg  [63:0] CF_stage2_Reg [7:0];
	reg  [31:0] CF_stage3_Reg [7:0];
	reg  [15:0] CF_stage4_Reg [7:0];
	reg  [7:0] CF_stage5_Reg [7:0];
	reg  [3:0] CF_stage6_Reg [7:0];
	reg  [1:0] CF_stage7_Reg [7:0];
	reg  [7:0] CF_stage8_Reg;
	
	reg [7:0] in1_reg;
	reg [7:0] in1_reg2;
	reg [7:0] in1_reg3;
	reg [7:0] in1_reg4;
	reg [7:0] in1_reg5;
	reg [7:0] in1_reg6;
	reg [7:0] in1_reg7;
	reg [7:0] in1_reg8;
	
	reg [1:0] a;
	reg [1:0] b;
	reg [1:0] c;
	reg [1:0] d;
	reg [1:0] e;
	reg [1:0] f;
	reg [1:0] g;
	reg [1:0] h;
	
	
	always @(*) begin
		a = {in2[0],in1[0]};
		b = {in2[1],in1[1]};
		c = {in2[2],in1[2]};
		d = {in2[3],in1[3]};
		e = {in2[4],in1[4]};
		f = {in2[5],in1[5]};
		g = {in2[6],in1[6]};
		h = {in2[7],in1[7]};
	end

	genvar i, k;
   generate
		//NonLinear Layer
	for (k=0; k < 8; k=k+1) begin
      for (i=0; i < 256; i=i+1) 
      begin: Inst
         AES_NF_CF_stage0 #(.CoordinateFunction(k), .ComponentFunction(i)) CF_State0_Inst (
				.a(a), 
				.b(b), 
				.c(c), 
				.d(d), 
				.e(e),
				.f(f),
				.g(g),
				.h(h),
				.q(CF_stage0_Out[k][i])
			);
      end
	end
		

   endgenerate
	
	always @(posedge clk) begin

		for(r = 0; r < 8; r=r+1) begin
			CF_stage0_Reg[r] <= CF_stage0_Out[r];
			for(j = 0; j < 128; j=j+1) begin
				CF_stage1_Reg[r][j] <= CF_stage0_Reg[r][2*j] ^ CF_stage0_Reg[r][2*j+1];
			end
			for(j = 0; j < 64; j=j+1) begin
				CF_stage2_Reg[r][j] <= CF_stage1_Reg[r][2*j] ^ CF_stage1_Reg[r][2*j+1];
			end
			for(j = 0; j < 32; j=j+1) begin
				CF_stage3_Reg[r][j] <= CF_stage2_Reg[r][2*j] ^ CF_stage2_Reg[r][2*j+1];
			end
			for(j = 0; j < 16; j=j+1) begin
				CF_stage4_Reg[r][j] <= CF_stage3_Reg[r][2*j] ^ CF_stage3_Reg[r][2*j+1];
			end
			for(j = 0; j < 8; j=j+1) begin
				CF_stage5_Reg[r][j] <= CF_stage4_Reg[r][2*j] ^ CF_stage4_Reg[r][2*j+1];
			end
			for(j = 0; j < 4; j=j+1) begin
				CF_stage6_Reg[r][j] <= CF_stage5_Reg[r][2*j] ^ CF_stage5_Reg[r][2*j+1];
			end
			for(j = 0; j < 2; j=j+1) begin
				CF_stage7_Reg[r][j] <= CF_stage6_Reg[r][2*j] ^ CF_stage6_Reg[r][2*j+1];
			end
			//CF_stage4_Reg[r] <= CF_stage3_Reg[r][0] ^ CF_stage3_Reg[r][1];
		end
		
		in1_reg <= in1;

		in1_reg2 <= in1_reg;
		
		in1_reg3 <= in1_reg2;
		
		in1_reg4 <= in1_reg3;
		
		in1_reg5 <= in1_reg4;
		
		in1_reg6 <= in1_reg5;
		
		in1_reg7 <= in1_reg6;
		
		in1_reg8 <= in1_reg7;

	end
	
	always @(*) begin
        for(r = 0; r < 8; r=r+1) begin
           CF_stage8_Reg[r] = CF_stage7_Reg[r][0] ^ CF_stage7_Reg[r][1];
        end
	end
	
	assign out1 = CF_stage8_Reg[7:0];
	assign out2 = {in1_reg8[1], in1_reg8[2], in1_reg8[3], in1_reg8[4], in1_reg8[5], in1_reg8[6], in1_reg8[7], in1_reg8[0]};

endmodule
