#pragma once
#include <intrin.h>
//#define USE_AVX_512
#ifndef USE_AVX_512

#ifndef _mm256_cmplt_epi32_mask
#define _mm256_cmplt_epi32_mask(a,b) (__mmask8)_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpgt_epi32(b, a)))
#endif
#ifndef _mm_cmplt_epi8_mask
#define _mm_cmplt_epi8_mask(a,b) (__mmask16)_mm_movemask_epi8(_mm_cmpgt_epi8(b, a))
#endif

#ifndef _mm256_cmpgt_epi32_mask
#define _mm256_cmpgt_epi32_mask(a,b) (__mmask8)_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpgt_epi32(a, b)))
#endif
#ifndef _mm256_cmpge_epi32_mask
#define _mm256_cmpge_epi32_mask(a,b) (__mmask8)_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_or_si256(_mm256_cmpgt_epi32(a, b),_mm256_cmpeq_epi32(a,b))))
#endif
#ifndef _mm256_cmpeq_epi32_mask
#define _mm256_cmpeq_epi32_mask(a,b) (__mmask8)_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpeq_epi32(a, b)))
#endif
#ifndef _mm256_cmple_epi32_mask
#define _mm256_cmple_epi32_mask(a,b) (__mmask8)((__mmask8)_mm256_cmplt_epi32_mask(a,b)| (__mmask8)_mm256_cmpeq_epi32_mask(a,b))
#endif

#ifndef _mm256_cmpneq_epi32_mask
#define _mm256_cmpneq_epi32_mask(a,b) ((__mmask8)~_mm256_cmpeq_epi32_mask(a,b))
#endif

#ifndef _mm256_cmpeq_epi8_mask
#define _mm256_cmpeq_epi8_mask(a,b) (__mmask32)_mm256_movemask_epi8(_mm256_cmpeq_epi8(a, b))
#endif
#ifndef _mm256_cmpneq_epi8_mask
#define _mm256_cmpneq_epi8_mask(a,b) (__mmask32)~(_mm256_cmpeq_epi8_mask(a,b))
#endif

#ifndef _mm_cmpeq_epi8_mask
#define _mm_cmpeq_epi8_mask(a,b) (__mmask32)_mm_movemask_epi8 (_mm_cmpeq_epi8(a, b))
#endif
#ifndef _mm_cmpneq_epi8_mask
#define _mm_cmpneq_epi8_mask(a,b) (__mmask32)~(_mm_cmpeq_epi8_mask(a,b))
#endif

#ifndef _mm256_cmpeq_epi16_mask
#define _mm256_cmpeq_epi16_mask(a,b) (__mmask16)_mm256_movemask_ps(_mm256_castsi256_ps(_mm256_cmpeq_epi16(b, a)))
#endif
#ifndef _mm256_cmpneq_epi16_mask
#define _mm256_cmpneq_epi16_mask(a,b) (__mmask16)~(_mm256_cmpeq_epi16_mask(a,b))
#endif
#ifndef _mm256_mask_blend_epi32
#define _mm256_mask_blend_epi32(mask, a,b) _mm256_castps_si256(_mm256_blendv_ps(_mm256_castsi256_ps(a),_mm256_castsi256_ps(b), _mm256_castsi256_ps(_mm256i_convert_mask8_to_epi32(mask))))
#endif

#ifndef _mm256_mask_i32scatter_epi32
#define _mm256_mask_i32scatter_epi32(addr,mask,index,value,size) \
	_mm256_mask_i32scatter_epi32_avx2(addr,mask,index,value,size)
#endif
#ifndef _mm256_mask_add_epi32
#define _mm256_mask_add_epi32(src,cond,a,b) _mm256_mask_blend_epi32(cond, src, _mm256_add_epi32(a,b))
#endif

#ifndef _mm256_mask_sub_epi32
#define _mm256_mask_sub_epi32(src,cond,a,b) _mm256_mask_blend_epi32(cond, src, _mm256_sub_epi32(a,b))
#endif

#endif

__m256i expand_bits_to_bytes(unsigned int x);
__m256i _mm256i_convert_mask8_to_epi32(__mmask8 m);
void _mm256_mask_i32scatter_epi32_avx2(void* base_addr, __mmask8 k, __m256i vindex, __m256i a, const int scale);

void Merge256(int data[], int n);
void Merge256(unsigned int data[], int n);
