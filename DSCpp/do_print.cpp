#include "do_print.h"
#include <stdio.h>
void do_print_hex(__m512i data, bool nl) {
	bool first = true;
	printf("[");
	for (int i = 0; i < sizeof(data.m512i_u32) / sizeof(int); i++) {
		if (!first) {
			printf(", ");
		}
		printf("%08X", data.m512i_i32[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print_hex(__m256i data, bool nl) {
	bool first = true;
	printf("[");
	for (int i = 0; i < sizeof(data.m256i_u32) / sizeof(int); i++) {
		if (!first) {
			printf(", ");
		}
		printf("%08X", data.m256i_u32[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print_hex(__m128i data, bool nl) {
	bool first = true;
	printf("[");
	for (int i = 0; i < sizeof(data.m128i_u32) / sizeof(int); i++) {
		if (!first) {
			printf(", ");
		}
		printf("%08X", data.m128i_u32[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print_hex(int data[], int N, bool nl) {
	bool first = true;
	printf("[");
	for (int i = 0; i < N; i++) {
		if (!first) {
			printf(", ");
		}
		printf("%08X", data[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print_dec(__m512i data, bool nl) {
	bool first = true;
	printf("[");
	for (int i = 0; i < sizeof(data.m512i_u32) / sizeof(int); i++) {
		if (!first) {
			printf(", ");
		}
		printf("%d", data.m512i_i32[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print_dec(__m256i data, bool nl) {
	bool first = true;
	printf("[");
	for (int i = 0; i < sizeof(data.m256i_u32) / sizeof(int); i++) {
		if (!first) {
			printf(", ");
		}
		printf("%d", data.m256i_u32[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print_dec(__m128i data, bool nl) {
	bool first = true;
	printf("[");
	for (int i = 0; i < sizeof(data.m128i_u32) / sizeof(int); i++) {
		if (!first) {
			printf(", ");
		}
		printf("%d", data.m128i_u32[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
void do_print_dec(int data[], int N, bool nl) {
	bool first = true;
	printf("[");
	for (int i = 0; i < N; i++) {
		if (!first) {
			printf(", ");
		}
		printf("%d", data[i]);
		first = false;
	}
	printf("]");
	if (nl)printf("\n");
}
