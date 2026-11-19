#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include "conio.h"
#include "stdint.h"
#include <string.h>
#include <stdbool.h>

const uint16_t S[256] = {
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, 
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, 
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, 
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, 
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, 
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, 
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, 
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, 
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, 
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, 
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, 
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, 
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, 
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, 
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, 
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
	}; // AES S-box 

uint16_t* get_ANF(const uint16_t* Sbox, uint16_t n);
void prt_Sbox(const uint16_t* Sbox, uint16_t n);
void prt_ANF_x(uint16_t* ANF, uint16_t n);
uint16_t** init_NF_CF(uint16_t n);
void prt_NF_CF_ANF(uint16_t** NF_CF, uint8_t n);
void prt_NF_CF_mtrx(uint16_t** NF_CF, uint8_t n);
uint16_t** greedy_NF_CF(uint16_t** NF_CF, uint16_t* ANF, uint8_t n);
void prf_NF_CF_vlog(uint16_t** NF, uint8_t n);
void prf_component_functions(uint16_t** NF, FILE* fptr, uint8_t n);

uint16_t* get_ANF(const uint16_t* Sbox, uint16_t n) {		// converts LUT to ANF; storage: n-bits 1<<n times
	uint16_t i, j, k;
	uint16_t *A;
	A = (uint16_t*) malloc((1 << n) * sizeof(uint16_t));
	for (i = 0; i < (1 << n); i++)
		A[i] = Sbox[i];
	for (i = 0; i < n; i++)
		for (j = 0; j < (1 << (n - i - 1)); j++) // reducing cycle: 8, 4, 2, 1
			for (k = 0; k < (1 << i); k++) // increasing cycle: 1, 2, 4, 8
				A[j*(1 << (i + 1)) + (1 << i) + k] ^= A[j*(1 << (i + 1)) + k];
	return A;
}

void prt_Sbox(const uint16_t* Sbox, uint16_t n) {
	uint16_t i;
	for (i = 0; i < (1<<n); i++) {
		printf("%x ", S[i]);
	}
	printf("\n\n");
}

void prt_ANF_x(uint16_t* ANF, uint16_t n) {
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

uint16_t** init_NF_CF(uint16_t n) {
	// We add here extra single-term shares for further compression + fill the rest with zeros
	uint16_t i, j, k;
	uint16_t** NF_CF;
    uint16_t part_sum_1;
	NF_CF = (uint16_t**) malloc((1 << n) * sizeof(uint16_t*));
	for (i = 0; i < (1 << n); i++)
		NF_CF[i] = (uint16_t*) malloc((1 << n) * sizeof(uint16_t));
    size_t size = sizeof(NF_CF);
    printf("The size is %d bytes \n", size);
    printf("%d\n\n", (1 << n));
	for (i = 0; i < (1 << n); i++) { // go through component functions
		NF_CF[i][0] = 0;
		for (j = 1; j < (1 << n); j++) { // go through monomials
			NF_CF[i][j] = 0;
            part_sum_1 = ((j & 128) >> 7) + ((j & 64) >> 5) + ((j & 32) >> 3) + ((j & 16) >> 1) + \
                        ((j & 8) << 1) + ((j & 4 ) << 3) + ((j & 2) << 5) + ((j & 1) << 7);
			for(k = 0; k < n; k++) {	// go through coordinate functions 
				if ( (j == 1 | j == 2 | j == 4 | j == 8 | j == 16 | j == 32 | j == 64 | j == 128) & \
					(!(i % ((( ( part_sum_1 >> k ) + \
						(( part_sum_1 & ( ((1<<n)-1) >> (n-k)) ) << (n-k))
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

void prt_NF_CF_ANF(uint16_t** NF_CF, uint8_t n) {
	// We want to make it in a text form now
	char inp_bit[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
	uint16_t i, j, k, coord_f;
    uint16_t part_sum_1;

	for (coord_f = 0; coord_f < n; coord_f++) { // print out all the coordinate functions 
		printf("Coordinate Function %d:\n", coord_f);
        for (i=0; i < (1<<n); i++) {
			// we go trough the component function
			switch(coord_f) {
  				case 0:
  					printf("\nf%d(%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d) = ", i, inp_bit[0], (i&128)>>7, inp_bit[1], \
						(i&64)>>6, inp_bit[2], (i&32)>>5, inp_bit[3], (i&16)>>4, \
                        inp_bit[4], (i&8)>>3, inp_bit[5], \
						(i&4)>>2, inp_bit[6], (i&2)>>1, inp_bit[7], i&1);
  					break;
  				case 1:
  					printf("\nf%d(%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d) = ", i, inp_bit[7], (i&128)>>7, inp_bit[0], \
						(i&64)>>6, inp_bit[1], (i&32)>>5, inp_bit[2], (i&16)>>4, \
                        inp_bit[3], (i&8)>>3, inp_bit[4], \
						(i&4)>>2, inp_bit[5], (i&2)>>1, inp_bit[6], i&1);
  					break;
  				case 2:
  					printf("\nf%d(%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d) = ", i, inp_bit[6], (i&128)>>7, inp_bit[7], \
						(i&64)>>6, inp_bit[0], (i&32)>>5, inp_bit[1], (i&16)>>4, \
                        inp_bit[2], (i&8)>>3, inp_bit[3], \
						(i&4)>>2, inp_bit[4], (i&2)>>1, inp_bit[5], i&1);
					break;
				case 3:
					printf("\nf%d(%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d) = ", i, inp_bit[5], (i&128)>>7, inp_bit[6], \
						(i&64)>>6, inp_bit[7], (i&32)>>5, inp_bit[0], (i&16)>>4, \
                        inp_bit[1], (i&8)>>3, inp_bit[2], \
						(i&4)>>2, inp_bit[3], (i&2)>>1, inp_bit[4], i&1);
					break;
                case 4:
					printf("\nf%d(%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d) = ", i, inp_bit[4], (i&128)>>7, inp_bit[5], \
						(i&64)>>6, inp_bit[6], (i&32)>>5, inp_bit[7], (i&16)>>4, \
                        inp_bit[0], (i&8)>>3, inp_bit[1], \
						(i&4)>>2, inp_bit[2], (i&2)>>1, inp_bit[3], i&1);
					break;
                case 5:
					printf("\nf%d(%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d) = ", i, inp_bit[3], (i&128)>>7, inp_bit[4], \
						(i&64)>>6, inp_bit[5], (i&32)>>5, inp_bit[6], (i&16)>>4, \
                        inp_bit[7], (i&8)>>3, inp_bit[0], \
						(i&4)>>2, inp_bit[1], (i&2)>>1, inp_bit[2], i&1);
					break;
                case 6:
					printf("\nf%d(%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d) = ", i, inp_bit[2], (i&128)>>7, inp_bit[3], \
						(i&64)>>6, inp_bit[4], (i&32)>>5, inp_bit[5], (i&16)>>4, \
                        inp_bit[6], (i&8)>>3, inp_bit[7], \
						(i&4)>>2, inp_bit[0], (i&2)>>1, inp_bit[1], i&1);
					break;
                case 7:
					printf("\nf%d(%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d,%c%d) = ", i, inp_bit[1], (i&128)>>7, inp_bit[2], \
						(i&64)>>6, inp_bit[3], (i&32)>>5, inp_bit[4], (i&16)>>4, \
                        inp_bit[5], (i&8)>>3, inp_bit[6], \
						(i&4)>>2, inp_bit[7], (i&2)>>1, inp_bit[0], i&1);
					break;
				default:
					break;
			}
			if ((i == 0) & (((NF_CF[0][0]&(1<<coord_f)) >> coord_f) == 1))
				printf("1 + ");
			for (j = 1; j < (1<<n); j++) {
				// we go through the terms that a component function contains
				if (( (NF_CF[i][j] & (1<<coord_f)) >> coord_f) == 1) {
                    part_sum_1 = ((i & 128) >> 7) + ((i & 64) >> 5) + ((i & 32) >> 3) + ((i & 16) >> 1) + \
                        ((i & 8) << 1) + ((i & 4 ) << 3) + ((i & 2) << 5) + ((i & 1) << 7);
					for (k=0; k < n; k++) {
                        // if the term exists, start printing it out
						if (j&(1<<k))
							printf("%c%d ", inp_bit[k], ( ( (1 << k) &  (\
								( part_sum_1  >>  coord_f ) \
								+ ( ( part_sum_1 & (((1<<coord_f)-1)) ) << (n-coord_f)) ) \
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

void prt_NF_CF_mtrx(uint16_t** NF_CF, uint8_t n) {
	// cols = monomials (dcba <= inctremental)
	// rows = component functions (0 <= 15 incremental)
	uint16_t i, j, k;
	for (i = 0; i < (1 << n); i++) {
		for (j = 0; j < (1 << n); j++) {
            printf("%x%x%x%x%x%x%x%x ", (NF_CF[i][j]&128)>>7, (NF_CF[i][j]&64)>>6, (NF_CF[i][j]&32)>>5, (NF_CF[i][j]&16)>>4,
                (NF_CF[i][j]&8)>>3, (NF_CF[i][j]&4)>>2, (NF_CF[i][j]&2)>>1, (NF_CF[i][j]&1)>>0);
		}
		printf("\n");
	}
}

uint16_t** greedy_NF_CF(uint16_t** NF_CF, uint16_t* ANF, uint8_t n) {
	uint16_t i, j, k;
    uint16_t part_sum_1;
	NF_CF[0][0] = ANF[0] & ((1 << n) - 1);	// check, if affine, add 1 
	for (i = 1; i < (1 << n); i++) {	// go through columns - monomials
		for (k = 0; k < n; k++) { 		// go throug different coordinate functions
			if (ANF[i] & (1 << k)) {	 
				NF_CF[0][i] ^= 1 << k;
				for (j = i; j > 0; j--) {	// go through rows - component functions
					if ((i|j) == i) {
                        part_sum_1 = ((j & 128) >> 7) + ((j & 64) >> 5) + ((j & 32) >> 3) + ((j & 16) >> 1) + \
                        ((j & 8) << 1) + ((j & 4 ) << 3) + ((j & 2) << 5) + ((j & 1) << 7);
						NF_CF[( part_sum_1 >> k) + \
							( ( part_sum_1 & (((1<<n)-1)>>(n-k)) ) << (n-k))] \
							[i] ^= 1 << k; // make the flip abcd => dcba iteration
					}
				}
			}
		}
	}
	return NF_CF;
}

void prf_component_functions(uint16_t** NF, FILE* fptr, uint8_t n) {		// creates a Verilog code for NF_CF part of an Sbox
    char inp_bit[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
	uint16_t i, j, k, comp_f;
	bool amp_check = 0, xor_check = 0, empty_line = 1;
    uint16_t part_sum_1;

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
                    part_sum_1 = ((i & 128) >> 7) + ((i & 64) >> 5) + ((i & 32) >> 3) + ((i & 16) >> 1) + \
                        ((i & 8) << 1) + ((i & 4 ) << 3) + ((i & 2) << 5) + ((i & 1) << 7);
					if (xor_check) {
						fprintf(fptr, " ^");
					}
					else
						xor_check = 1; 
					for (k=0; k < n; k++) {									// we go through the bits of a monomial when one is present
						if ( ( (j&(1<<k)) >> k) & amp_check)
							fprintf(fptr, " & %c[%d]", inp_bit[k], ( ( (1 << k) &  (\
								( part_sum_1  >>  comp_f ) \
								+ ( ( part_sum_1 & (((1<<comp_f)-1)) ) << (n-comp_f)) ) \
								) >> k ) \
								);
						else if ((j&(1<<k)) >> k)  {
							fprintf(fptr, " %c[%d]", inp_bit[k], ( ( (1 << k) &  (\
								( part_sum_1  >>  comp_f ) \
								+ ( ( part_sum_1 & (((1<<comp_f)-1)) ) << (n-comp_f)) ) \
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

void prf_NF_CF_vlog(uint16_t** NF_CF, uint8_t n) {
	// Put everything to the file
	FILE *fptr;
	// Open a file in writing mode
	fptr = fopen("./auto_generation/AES_NF_CF_stage0.v", "w");
	// Write some text to the file
	fprintf(fptr, "module NF_CF_stage0 #(parameter CoordinateFunction = 0, ComponentFunction = 0) (\n"
    	"\tinput [1:0] a,\n"
		"\tinput [1:0] b,\n"
    	"\tinput [1:0] c,\n"
    	"\tinput [1:0] d,\n"
        "\tinput [1:0] e,\n"
        "\tinput [1:0] f,\n"
        "\tinput [1:0] g,\n"
        "\tinput [1:0] h,\n"
    	"\toutput q\n" 
		" );\n" 
		"\n\n\n"
		"\tgenerate\n");
	// call print functions to file
	prf_component_functions(NF_CF, fptr, n);
	fprintf(fptr, "\tendgenerate\n\n"
		"endmodule");	
	// Close the file
	fclose(fptr);
}

int main() {
    uint16_t** NF_CF;
    uint16_t* arr_anf;
    uint16_t i;
    uint16_t n = 8;

	arr_anf = get_ANF(S, n);

	prt_Sbox(S, n);
	prt_ANF_x(arr_anf, n);

    NF_CF = init_NF_CF(n);
    
    NF_CF = greedy_NF_CF(NF_CF, arr_anf, n);

    prf_NF_CF_vlog(NF_CF, n);

    for (i = 0; i < (1 << n); i++) {
        free(NF_CF[i]);
	}
    free(NF_CF);
    free(arr_anf);

    return 0;
}