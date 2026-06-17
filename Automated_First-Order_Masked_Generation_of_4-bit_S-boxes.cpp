// Algebraic Normal Form (ANF)
// (yn, ... , y1, y0) = SB(xn, ... , x1, x0)
// IMPORTANT: take care of the variables size

// The notation for the letters that are used as input bits and output bits
// input bits (we go max up to 8 at the moment):
// a, b, c, d, e, f, g, h	<= we cut the ending
// output bits:
// s, t, u, v, w, x, y, z	<= we cut the beginning 

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
// #include "conio.h"
#include "stdint.h"
#include <string.h>


#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WIN32
#include <direct.h>
#define mkdir_if_needed(path) _mkdir(path)
#else
#include <unistd.h>
#define mkdir_if_needed(path) mkdir(path, 0777)
#endif


//#include "windows.h" // to check memory used and process speed
//#include "psapi.h"

const uint16_t n = 4;	// Sbox size
// const char s_cipher[7] = "Skinny";
const 
	//uint8_t S[16] = { 0x1, 0xa, 0x4, 0xc, 0x6, 0xf, 0x3, 0x9, 0x2, 0xd, 0xb, 0x7, 0x5, 0x0, 0x8, 0xe }; // Gift
	uint8_t S[16] = { 0xc, 0x6, 0x9, 0x0, 0x1, 0xa, 0x2, 0xb, 0x3, 0x8, 0x5, 0xd, 0x4, 0xe, 0x7, 0xf }; // Skinny
	//uint8_t S[16] = { 0xc, 0xa, 0xd, 0x3, 0xe, 0xb, 0xf, 0x7, 0x8, 0x9, 0x1, 0x5, 0x0, 0x2, 0x4, 0x6 }; // Midori
	//uint8_t S[16] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf }; // Identity
	//uint8_t S[16] = { 0x0, 0x4, 0x8, 0xf, 0x1, 0x5, 0xe, 0x9, 0x2, 0x7, 0xa, 0xc, 0xb, 0xd, 0x6, 0x3 }; // PRIDE
	//uint8_t S[16] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x8, 0x9, 0x6, 0x7, 0xC, 0xD, 0xE, 0xF, 0xB, 0xA }; // Class 301
	//uint8_t S[16] = { 0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0xE, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2 }; // PRESENT
	//uint8_t S[16] = { 0xb, 0x7, 0x3, 0x2, 0xf, 0xd, 0x8, 0x9, 0xa, 0x6, 0x4, 0x0, 0x5, 0xe, 0xc, 0x1 }; // PRINCE_inv
	//uint8_t S[16] = { 0xb, 0xF, 0x3, 0x2, 0xA, 0xC, 0x9, 0x1, 0x6, 0x7, 0x8, 0x0, 0xE, 0x5, 0xD, 0x4 }; // PRINCE
	
/* function statement */
/* 
	prt - print to the console,
	prf - print to a file
*/
int dir_exists(const char *path); // Checks if a directory exists
void prf_component_functions(uint16_t** NF, FILE* fptr, uint8_t n);	// Writes Verilog code for component functions of NF_CF
uint16_t* get_ANF(uint16_t* Sbox, uint8_t n);	// creates ANF array out of an Sbox
void prt_Sbox(uint16_t* Sbox, uint8_t n);	// Print an Sbox as a hex matrix 
void prt_ANF_x(uint16_t* ANF, uint8_t n);	// Print (console) ANF of the processed Sbox 
void prf_Sbox_tb(uint16_t* ANF, uint8_t n, const char* s_cipher);	// Generate a Verilog testbench file
void prf_ANF_abcd(uint16_t* ANF, FILE* fptr, uint8_t n);	// Print ANF to file
void prt_ANF_mtrx(uint16_t* ANF, uint8_t n);	// Print ANF in a matrix form (mostly for debugging)
void prt_NF_CF_mtrx(uint16_t** NF_CF, uint8_t n);	// Print NF_CF as a matrix (mostly for debugging)
uint16_t** init_NF_CF(uint8_t n);	// Initialize a matrix for the shared Sbox. i - component function, j - monomials, k - coordinate functions
									// add compression monomials
uint16_t** greedy_NF_CF(uint16_t** NF_CF, uint16_t* ANF, uint8_t n);	// Apply Algorithm 1 to the given ANF
void prt_NF_CF_ANF(uint16_t** NF_CF, uint8_t n);	// Print ANF of the current NF_CF
void prf_NF_CF_vlog(uint16_t** NF, uint8_t n, const char* s_cipher);	// Creates a Verilog file for NF_CF
//void prf_ANF_x(uint16_t* ANF, uint8_t n);	// Print (file) ANF of the Sbox
void prf_Sbox_top(uint8_t n, const char* s_cipher);	// create a Verilog file with the top logic description (compression and sharings)


/* function definition */
int dir_exists(const char *path) {
    struct stat info;

    if (stat(path, &info) != 0)
        return 0;  // does not exist

    return (info.st_mode & S_IFDIR) != 0;
}


void prf_Sbox_tb(uint16_t* ANF, uint8_t n, const char* s_cipher) {
	FILE* fptr;
	char* file_name;
	const char* dir = "./auto_generation/";
	
	if (!dir_exists(dir)) {
        if (mkdir_if_needed(dir) != 0) {
            perror("Failed to create directory");
            // return 1;
        }
    }

	file_name = (char*) malloc( (29 + ( sizeof(s_cipher) / sizeof(s_cipher[0]) )) * sizeof(char));
	strcpy(file_name, dir);
	strcat(file_name, s_cipher);
	strcat(file_name, "_Sbox_tb.v");
	
	fptr = fopen(file_name, "w");
	fprintf(fptr, "`timescale 1ns / 1ps\n\n"
    	"module %s_Sbox_tb;\n\n"
		"\t// Inputs\n"
		"\treg clk;\n"
		"\treg [%d:0] in1;\n"
		"\treg [%d:0] in2;\n\n" 
		"\t// Outputs\n"
		"\twire [%d:0] out1;\n"
		"\twire [%d:0] out2;\n\n"
		"\twire [%d:0] S_in;\n"
		"\twire [%d:0] S_out;\n\n"
		"\treg t, x, y, w;\n\n\tinteger i;\n\n\tassign S_in = in1 ^ in2;\n\tassign S_out = out1 ^ out2;\n\n"
		"\treg  a, b, c, d;\n\n"
		"\talways @(*) begin\n\t\ta = S_in[0];\n\t\tb = S_in[1];\n\t\tc = S_in[2];\n\t\td = S_in[3];\n\n", 
		s_cipher, (n-1), (n-1), (n-1), (n-1), (n-1), (n-1)
	);
	prf_ANF_abcd(ANF, fptr, n);
	fprintf(fptr, "\tend\n\n\n"
		"\t// Instantiate the Unit Under Test (UUT)\n"
		"\t%s_SBOX uut (\n"
		"\t\t.clk(clk),\n\t\t.in1(in1),\n\t\t.in2(in2),\n\t\t.out1(out1),\n\t\t.out2(out2)\n\t);\n\n"
		"\tinitial begin\n"
		"\t\t// Initialize Inputs\n"
		"\t\tclk = 0;\n\t\tin1 = 0;\n\t\tin2 = 0;\n\n"
		"\t\t// Wait 100 ns for global reset to finish\n"
		"\t\tfor (i=0; i<256; i=i+1) begin\n"
		"\t\t\tin1 = i[3:0];\n\t\tin2 = i[7:4];\n\n"
		"\t\t\t#70;\n"
		"\t\t\tif(S_out == {w,t,y,x}) begin\n"
		"\t\t\t\t$write(\"------------------PASS---------------\\n\");\n\t\t\tend\n\t\t\telse begin\n"
		"\t\t\t\t$write(\"------------------FAIL---------------\\n\");\n"
		"\t\t\t\t$write(\"%%x\\n%%x\\n%%x\\n\", S_in, S_out, {w,t,y,x});\n"
		"\t\t\t\t$stop;\n\t\t\tend\n\t\tend\n\t\t$stop;\n\t\t// Add stimulus here\n\n"
		"\tend\n\talways #5 clk = ~clk;\n"
		"endmodule", s_cipher
	);
	fclose(fptr);
}

void prf_ANF_abcd(uint16_t* ANF, FILE* fptr, uint8_t n) {
	char inp_bit[n] = {'a', 'b', 'c', 'd'};
	char out_bit[n] = {'x', 'y', 't', 'w'};
	uint16_t i, j, k;
	bool amp_check = 0, xor_check = 0;
	for (i = 0; i < n; i++) {
		xor_check = 0;
		fprintf(fptr, "\t\t%c = ", out_bit[i]);
		if ((ANF[0] & (1 << i)))
			fprintf(fptr, "1'b1 + ");
		for (j = 1; j < (1 << n); j++) {		// print polynomial
			amp_check = 0;
			if ((ANF[j] & (1 << i))) {
				if (xor_check) {
					fprintf(fptr, "+ ");
				}
				for (k = 0; k < n; k++) {	// print monomial
					if ( ((j & (1 << k)) >> k) & amp_check )
						fprintf(fptr, "& %c ", inp_bit[k]);
					else if((j & (1 << k))) {
						fprintf(fptr, "%c ", inp_bit[k]);
						amp_check = 1;
					}
				}
				xor_check = 1;
			}
		}
		fprintf(fptr, ";\n");
	}
}

void prf_NF_CF_vlog(uint16_t** NF_CF, uint8_t n, const char* s_cipher) {
	FILE *fptr;
	char* file_name;

	file_name = (char*) malloc( (34 + ( sizeof(s_cipher) / sizeof(s_cipher[0]) )) * sizeof(char));
	strcpy(file_name, "./auto_generation/");
	strcat(file_name, s_cipher);
	strcat(file_name, "_NF_CF_stage0.v");
	fptr = fopen(file_name, "w");

	fprintf(fptr, "module NF_CF_stage0 #(parameter CoordinateFunction = 0, ComponentFunction = 0) (\n"
    	"\tinput [1:0] a,\n"
		"\tinput [1:0] b,\n"
    	"\tinput [1:0] c,\n"
    	"\tinput [1:0] d,\n"
    	"\toutput q\n" 
		" );\n" 
		"\n\n\n"
		"\tgenerate\n");
	
	// call print functions to file
	prf_component_functions(NF_CF, fptr, n);
	fprintf(fptr, "\tendgenerate\n\n"
		"endmodule");	
	
	fclose(fptr);
}

void prt_NF_CF_ANF(uint16_t** NF_CF, uint8_t n) {
	// We want to make it in a text form now
	char inp_bit[n] = {'a', 'b', 'c', 'd'};
	uint16_t i, j, k, comp_f;

	for (comp_f = 0; comp_f < n; comp_f++) { // print out all the coordinate functions 
		for (i=0; i < (1<<n); i++) {
			// we go trough the component function
			switch(comp_f) {
  				case 0:
  					printf("\nf%d(%c%d,%c%d,%c%d,%c%d) = ", i, inp_bit[0], (i&8)>>3, inp_bit[1], \
						(i&4)>>2, inp_bit[2], (i&2)>>1, inp_bit[3], i&1);
  					break;
  				case 1:
  					printf("\nf%d(%c%d,%c%d,%c%d,%c%d) = ", i, inp_bit[3], (i&8)>>3, inp_bit[0], \
						(i&4)>>2, inp_bit[1], (i&2)>>1, inp_bit[2], i&1);
  					break;
  				case 2:
  					printf("\nf%d(%c%d,%c%d,%c%d,%c%d) = ", i, inp_bit[2], (i&8)>>3, inp_bit[3], \
						(i&4)>>2, inp_bit[0], (i&2)>>1, inp_bit[1], i&1);
					break;
				case 3:
					printf("\nf%d(%c%d,%c%d,%c%d,%c%d) = ", i, inp_bit[1], (i&8)>>3, inp_bit[2], \
						(i&4)>>2, inp_bit[3], (i&2)>>1, inp_bit[0], i&1);
					break;
				default:
					break;
			}
			if ((i == 0) & (((NF_CF[0][0]&(1<<comp_f)) >> comp_f) == 1))
				printf("1 + ");
			for (j=1; j < (1<<n); j++) {
				// we go through the terms that a component function contains
				if (((NF_CF[i][j]&(1<<comp_f)) >> comp_f) == 1) {
					for (k=0; k < n; k++) {
						if (j&(1<<k))
							printf("%c%d ", inp_bit[k], ( ( (1 << k) &  (\
								( ( ((i&8)>>3) + ((i&4)>>1) + ((i&2)<<1) + ((i&1)<<3) )  >>  comp_f ) \
								+ ( ( ( ((i&8)>>3) + ((i&4)>>1) + ((i&2)<<1) + ((i&1)<<3) ) & (((1<<comp_f)-1)) ) << (n-comp_f)) ) \
								) >> k ) \
								);
					}
					printf(" + ");
				}
			}
			printf("\n");
		}
		printf("\n\n");
	}
}

uint16_t** greedy_NF_CF(uint16_t** NF_CF, uint16_t* ANF, uint8_t n) {
	// we flip coordinate function iteration of the components and then we rotate the components
	// f0 (a0, b0, c0, d0)  g0 (d0, a0, b0, c0)  h0 (c0, d0, a0, b0)  t0 (b0, c0, d0, a0) <= MSB_out
	// flip component functions itaration => dcba (now) => abcd (for one of the functions, then shift) 
	//if (A[0] & (1<<0))
	uint16_t i, j, k;
	NF_CF[0][0] = ANF[0] & ((1 << n) - 1);	// check, if affine, add 1 
	for (i = 1; i < (1 << n); i++) {	// go through columns - monomials
		for (k = 0; k < n; k++) { 		// go throug different coordinate functions
			if (ANF[i] & (1 << k)) {	
				NF_CF[0][i] ^= 1 << k;
				for (j = i; j > 0; j--) {	// go through rows - component functions
					if ((i|j) == i) {
						NF_CF[( (((j&8)>>3) + ((j&4)>>1) + ((j&2)<<1) + ((j&1)<<3)) >> k) + \
							((( ((j & 8) >> 3) + ((j & 4 ) >> 1) + ((j & 2) << 1) + ((j & 1) << 3) ) & (((1<<n)-1)>>(n-k))) << (n-k))] \
							[i] ^= 1 << k; // make the flip abcd => dcba iteration
					}
				}
			}
		}
	}
	return NF_CF;
}

uint16_t** init_NF_CF(uint8_t n) {
	// We add here extra single-term shares for further compression + fill the rest with zeros (! Important)
	// Complexity O(2^(2n)*n)
	uint16_t i, j, k;
	uint16_t** NF_CF;
	uint16_t part_sum_1;
	NF_CF = (uint16_t**) malloc((1 << n) * sizeof(uint16_t*));
	for (i = 0; i < (1 << n); i++)
		NF_CF[i] = (uint16_t*) malloc((1 << n) * sizeof(uint16_t));
	for (i = 0; i < (1 << n); i++) { // go through component functions
		NF_CF[i][0] = 0;
		for (j = 1; j < (1 << n); j++) { // go through monomials
			NF_CF[i][j] = 0;
			for(k = 0; k < n; k++) {	// go through coordinate functions 
				 // then we flip the monomial value and shift depending on the coordinate function
				part_sum_1 = (((j & 8) >> 3) + ((j & 4 ) >> 1) + ((j & 2) << 1) + ((j & 1) << 3));
				if ( (j == 1 | j == 2 | j == 4 | j == 8) & \
					(!(i % ( (( ( part_sum_1 >> k) + \
						(( part_sum_1 & (((1<<n)-1)>>(n-k)) ) << (n-k))
						)) << 1)) \
				)) // here we add the extra "compression"-components
					NF_CF[i][j] ^= 1 << k;
				else
					NF_CF[i][j] ^= 0;
			}
		}
	}
	return NF_CF;
}

void prt_NF_CF_mtrx(uint16_t** NF_CF, uint8_t n) {
	// Check for massive table
	// cols = monomials (dcba <= incremental)
	// rows = component functions (0 <= 15 incremental)
	uint16_t i, j, k;
	for (i = 0; i < (1 << n); i++) {
		for (j = 0; j < (1 << n); j++) {
			printf("%x%x%x%x ", (NF_CF[i][j]&8)>>3, (NF_CF[i][j]&4)>>2, (NF_CF[i][j]&2)>>1, (NF_CF[i][j]&1)>>0);
		}
		printf("\n");
	}
}

void prt_ANF_mtrx(uint16_t* ANF, uint8_t n) {
	uint16_t i;
	for (i = 0; i < (1<<n); i++) {
		printf("%x ", ANF[i]);
	}
	printf("\n\n");
}

void prt_ANF_x(uint16_t* ANF, uint8_t n) {
	uint16_t i, j, k;
	for (i = 0; i < n; i++) {
		printf("y%d = ", i);
		if ((ANF[0] & (1 << i)))
			printf("1 + ");
		for (j = 1; j < (1 << n); j++)
			if ((ANF[j] & (1 << i))) {
				for (k = 0; k < n; k++)
					if ((j & (1 << k)))
						printf("x%d ", k);
				printf("+ ");
			}
		printf("\n\n");
	}
}

void prt_Sbox(uint16_t* Sbox, uint8_t n) {
	uint16_t i;
	for (i = 0; i < (1<<n); i++) {
		printf("%x ", S[i]);
	}
	printf("\n\n");
}

uint16_t * get_ANF(uint16_t* Sbox, uint8_t n) {		// converts LUT to ANF; storage: n-bits 1<<n times
	uint16_t i, j, k;
	uint16_t *A;
	A = (uint16_t*) malloc(n * sizeof(uint16_t));
	for (i = 0; i < (1 << n); i++)
		A[i] = Sbox[i];
	for (i = 0; i < n; i++)
		for (j = 0; j < (1 << (n - i - 1)); j++) // decreasing cycle: 8, 4, 2, 1
			for (k = 0; k < (1 << i); k++) // increasing cycle: 1, 2, 4, 8
				A[j*(1 << (i + 1)) + (1 << i) + k] ^= A[j*(1 << (i + 1)) + k];
	return A;
}

void prf_component_functions(uint16_t** NF, FILE* fptr, uint8_t n) {		// creates a Verilog code for NF_CF part of an Sbox
    char inp_bit[n] = {'a', 'b', 'c', 'd'};
	uint16_t i, j, k, comp_f; 
	bool amp_check = 0, xor_check = 0, empty_line = 1;
	for (comp_f = 0; comp_f < n; comp_f++) { 								// print out all the coordinate functions 
		fprintf(fptr, "\t\tif (CoordinateFunction == %d) begin\n", comp_f);
		for (i=0; i < (1<<n); i++) {										// we go trough the component function
			empty_line = 1;
			fprintf(fptr, "\t\t\tif (ComponentFunction == %d) begin\n\t\t\t\tassign q = ", i);
			if ((i == 0) & (((NF[0][0]&(1<<comp_f)) >> comp_f) == 1)) {
				fprintf(fptr, "1'b1 ^ ");
				empty_line = 0;
			}
			for (j=1; j < (1<<n); j++) {									// we go through the terms that a component function contains
				if (((NF[i][j]&(1<<comp_f)) >> comp_f) == 1) {
					amp_check = 0;
					empty_line = 0;
					if (xor_check) {
						fprintf(fptr, " ^"); // also a check?
						// xor_check = 0; 
					}
					else
						xor_check = 1; 
					for (k=0; k < n; k++) {									// we go through the bits of a monomial when one is present
						if ( ( (j&(1<<k)) >> k) & amp_check)
							fprintf(fptr, " & %c[%d]", inp_bit[k], ( ( (1 << k) &  (\
								( ( ((i&8)>>3) + ((i&4)>>1) + ((i&2)<<1) + ((i&1)<<3) )  >>  comp_f ) \
								+ ( ( ( ((i&8)>>3) + ((i&4)>>1) + ((i&2)<<1) + ((i&1)<<3) ) & (((1<<comp_f)-1)) ) << (n-comp_f)) ) \
								) >> k ) \
								);
						else if ((j&(1<<k)) >> k)  {
							fprintf(fptr, " %c[%d]", inp_bit[k], ( ( (1 << k) &  (\
								( ( ((i&8)>>3) + ((i&4)>>1) + ((i&2)<<1) + ((i&1)<<3) )  >>  comp_f ) \
								+ ( ( ( ((i&8)>>3) + ((i&4)>>1) + ((i&2)<<1) + ((i&1)<<3) ) & (((1<<comp_f)-1)) ) << (n-comp_f)) ) \
								) >> k ) \
								);
							amp_check = 1;
						}
					}
				}
			}
			xor_check = 0;
			if (empty_line)
				fprintf(fptr, "1'b0");
			fprintf(fptr, ";\n\t\t\tend\n");
		}
		fprintf(fptr, "\t\tend\n\n");
	}
}

void prf_Sbox_top(uint8_t n, const char* s_cipher){
	FILE* fptr;
	char* file_name;

	file_name = (char*) malloc( (26 + ( sizeof(s_cipher) / sizeof(s_cipher[0]) )) * sizeof(char));
	strcpy(file_name, "./auto_generation/");
	strcat(file_name, s_cipher);
	strcat(file_name, "_SBOX.v");
	
	fptr = fopen(file_name, "w");
	fprintf(fptr, "module %s_SBOX(\n"
	"\tinput clk,\n"
	"\tinput  [%d:0] in1,\t\t// 1st share\n"
    "\tinput  [%d:0] in2,\t\t// 2nd share\n"
    "\toutput [%d:0] out1,\n"
    "\toutput [%d:0] out2\n"
    ");\n"
	"\n\tinteger j, r;\n\n"
	"\twire [%d:0] CF_stage0_Out [%d:0]; // array of %d-bit output initial stage and %d coordinate functions\n"
	"\treg  [%d:0] CF_stage0_Reg [%d:0];\n\n"
	"\treg  [%d:0] CF_stage1_Reg [%d:0]; // Stage 1\n"
	"\treg  [%d:0] CF_stage2_Reg [%d:0]; // Stage 2\n"
	"\treg  [%d:0] CF_stage3_Reg [%d:0];\n"
	"\treg  [%d:0] CF_stage4_Reg;\n\n"
	"\treg [%d:0] in1_reg;\n"
	"\treg [%d:0] in1_reg2;\n"
	"\treg [%d:0] in1_reg3;\n"
	"\treg [%d:0] in1_reg4;\n"
	"\treg [%d:0] in1_reg5;\n\n"
	"\treg [1:0] a; // input bits shared\n"
	"\treg [1:0] b;\n"
	"\treg [1:0] c;\n"
	"\treg [1:0] d;\n\n" 
	"\t/*\n\n"
	"\tx = f(a, b, c, d)\n"
	"\ty = g(d, a, b, c)\n"
	"\tz = h(c, d, a, b)\n"
	"\tt = k(b, c, d, a)\n\n"
	"\t*/\n\n"
	"\t// in1 = {d0,c0,b0,a0}\n"  
	"\t// in2 = {d1,c1,b1,a1}\n" 
	"\t// output should be {t,z,y,x}\n\n"
	"\talways @(*) begin\n"
	"\t\ta = {in2[0],in1[0]}; // {a1, a0}\n"
	"\t\tb = {in2[1],in1[1]};\n"
	"\t\tc = {in2[2],in1[2]};\n"
	"\t\td = {in2[3],in1[3]}; // {d1, d0}\n"
	"\tend\n\n"
	"\tgenvar i, k;\n\n\tgenerate\n\t//NonLinear Layer\n"
	"\tfor (k=0; k < %d; k=k+1) begin\n"
	"\t\tfor (i=0; i < %d; i=i+1) begin: Inst\n"
	"\t\t\t%s_NF_CF_stage0 #(.CoordinateFunction(k), .ComponentFunction(i)) CF_State0_Inst (\n"
	"\t\t\t\t.a(a),\n"
	"\t\t\t\t.b(b),\n"
	"\t\t\t\t.c(c),\n"
	"\t\t\t\t.d(d),\n"
	"\t\t\t\t.q(CF_stage0_Out[k][i])\n"
	"\t\t\t);\n\t\t\tend\n\t\tend\n\tendgenerate\n\n"
	"\talways @(posedge clk) begin\n"
	"\t\tfor(r = 0; r < %d; r=r+1) begin\n"
	"\t\t\tCF_stage0_Reg[r] <= CF_stage0_Out[r];\n"
	"\t\t\tfor(j = 0; j < %d; j=j+1) begin\n"
	"\t\t\t\tCF_stage1_Reg[r][j] <= CF_stage0_Reg[r][2*j] ^ CF_stage0_Reg[r][2*j+1];\n\t\t\tend\n"
	"\t\t\tfor(j = 0; j < %d; j=j+1) begin\n"
	"\t\t\t\tCF_stage2_Reg[r][j] <= CF_stage1_Reg[r][2*j] ^ CF_stage1_Reg[r][2*j+1];\n\t\t\tend\n"
	"\t\t\tfor(j = 0; j < %d; j=j+1) begin\n"
	"\t\t\t\tCF_stage3_Reg[r][j] <= CF_stage2_Reg[r][2*j] ^ CF_stage2_Reg[r][2*j+1];\n\t\t\tend\n"
	"\t\t\tCF_stage4_Reg[r] <= CF_stage3_Reg[r][0] ^ CF_stage3_Reg[r][1];\n\t\tend\n\n"
	"\t\tin1_reg <= in1;\n"
	"\t\tin1_reg2 <= in1_reg;\n"
	"\t\tin1_reg3 <= in1_reg2;\n"
	"\t\tin1_reg4 <= in1_reg3;\n"
	"\t\tin1_reg5 <= in1_reg4;\n"
	"\tend\n\n"
	"\tassign out1 = CF_stage4_Reg[%d:0];\n"
	"\tassign out2 = {in1_reg5[1], in1_reg5[2], in1_reg5[3], in1_reg5[0]};\n\n"
	"endmodule", s_cipher, n-1, n-1, n-1, n-1, 
	(1<<n)-1, n-1, (1<<n), n, 
	(1<<n)-1, n-1, (1<<(n-1))-1, n-1, (1<<(n-2))-1, n-1, (1<<(n-3))-1, n-1, 
	n-1, n-1, n-1, n-1, n-1, n-1, 
	n, (1<<n), s_cipher, 
	n, (1<<(n-1)), (1<<(n-2)), (1<<(n-3)), n-1);
	fclose(fptr);
}


/////////////* Main code execution */////////////
int main() { //int argc, char *argv[]
	// argv: 0 - name_of_program; 1 - do_you_want_a_custom_Sbox_flag; 2 - name_of_the_Sbox
	
	uint16_t i, j, k;	// ANF size of 2^(Sbox_inp_length) 1-dim array
	uint16_t* arr_anf;
	uint16_t** NF_CF;

	
	uint16_t Sbox[1<<n];

	for (i = 0; i < (1 << n); i++) { // can be changed for keyboard input 
		Sbox[i] = S[i];
	}

	// First we get an ANF from the chosen Sbox
	arr_anf = get_ANF(Sbox, n);

	prt_Sbox(Sbox, n);
	prt_ANF_x(arr_anf, n);
	
	// Now that we have 4-bit values stored in NF (LSB = LSB of function output)
	// We need to adapt rotations for each element 
	// We will store all NF tables in the same way
	// 
	// a0b0c0d0
	// a0b0c0d1
	// 

	// We initialize an array (with correction terms) for masking with 2^n shares (1st step of MultiStage)
	// then fill by direct sharing (greedy algorithm)
	NF_CF = init_NF_CF(n);
	NF_CF = greedy_NF_CF(NF_CF, arr_anf, n);
	prt_NF_CF_ANF(NF_CF, n);

	// Create a Verilog file out of it
	prf_NF_CF_vlog(NF_CF, n, "Skinny");

	// Here we create two other files: tb.v and top_module.v
	prf_Sbox_tb(arr_anf, n, "Skinny");
	prf_Sbox_top(n, "Skinny");

	//PROCESS_MEMORY_COUNTERS_EX pmc;
	//GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	//SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
	
	//_getch();
	return 0;
}