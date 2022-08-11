#pragma once
#include <intrin.h>

void do_print_hex(__m512i data, bool nl = false);
void do_print_hex(__m256i data, bool nl = false);
void do_print_hex(__m128i data, bool nl = false);
void do_print_hex(int data[], int N, bool nl = false);
void do_print_dec(__m512i data, bool nl = false);
void do_print_dec(__m256i data, bool nl = false);
void do_print_dec(__m128i data, bool nl = false);
void do_print_dec(int data[], int N, bool nl = false);
