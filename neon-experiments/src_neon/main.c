#define SIMDE_ENABLE_NATIVE_ALIASES
#include <x86/avx2.h>
#include <stdint.h>
#include <stdio.h>

#define AVX2_uint16_MINMAX(a, b)                         \
    do {                                                 \
        __m256i _a = _mm256_loadu_si256((__m256i *)(a)); \
        __m256i _b = _mm256_loadu_si256((__m256i *)(b)); \
        __m256i _min = _mm256_min_epu16(_a, _b);         \
        __m256i _max = _mm256_max_epu16(_a, _b);         \
        _mm256_storeu_si256((__m256i *)(a), _min);       \
        _mm256_storeu_si256((__m256i *)(b), _max);       \
    }                                                    \
    while (0)
#define AVX2_uint16_MINMAX_regs(a, b)          \
    do {                                       \
        __m256i _min = _mm256_min_epu32(a, b); \
        __m256i _max = _mm256_max_epu32(a, b); \
        a = _min;                              \
        b = _max;                              \
    }                                          \
    while (0)

void print_vector(const char *name, const int32_t vec[], int32_t n) {
  printf("%s = {", name);
  for (int32_t i = 0; i < n; i++) {
    printf("%d, ", vec[i]);
  }
  printf("}\n");
}
void print_vectorOrdem(const char *name, const int32_t vec[], int32_t n){
    printf("%s = {", name);
    for (int32_t i = 0; i < n; i++){
        printf("%d, ", vec[(n/2)*(i&1)+i/2]);
    }
    printf("}\n");
}


void sorting16(int32_t *a){
    __m256i v0a7 = _mm256_loadu_si256((__m256i *)&a[0]);
    __m256i v8a15 = _mm256_loadu_si256((__m256i *)&a[8]);
    //Deep=1
    //7654|3210
    //FEDC|BA98
    __m256i sm_PU_32 = _mm256_set_epi32(
        0x07, 0x05, 0x03, 0x01, 
        0x06, 0x04, 0x02, 0x00  
    );
    __m256i vv0a7 = _mm256_permutevar8x32_epi32(v0a7, sm_PU_32);
    __m256i vv08a15 = _mm256_permutevar8x32_epi32(v8a15, sm_PU_32);
    //7531|6420
    //FDB9|ECA8
    
    __m256i vv0a15lo = _mm256_permute2x128_si256(vv0a7, vv08a15, 0x20 );
    __m256i vv0a15hi = _mm256_permute2x128_si256(vv0a7, vv08a15, 0x31 );
    
    
    
    //ECA8|6420
    //FDB9|7531
    
    
    AVX2_uint16_MINMAX_regs(vv0a15lo, vv0a15hi);
    
    
    
    
    //Deep = 2
    __m256i mascara = _mm256_set_epi32(
        0x06, 0x07, 0x04, 0x05, 
        0x02, 0x03, 0x00, 0x01  
    );
    __m256i vv0a15hi2 = _mm256_permutevar8x32_epi32(vv0a15hi, mascara);
    
    //ECA8|6420
    //DF9B|5713
    AVX2_uint16_MINMAX_regs(vv0a15lo, vv0a15hi2);
    //DC98|5410
    //EFAB|6723
    //blend

    __m256i vv0a15lob = _mm256_blend_epi32(vv0a15lo, vv0a15hi2, 0xAA); // 0b01010101
    
    __m256i vv0a15hi2b = _mm256_blend_epi32(vv0a15lo, vv0a15hi2, 0x55);

    
    //
    //ECA8|6420
    //DF9B|5713
  
    ////DC98|5410
    ////EFAB|6723
    
    //Deep = 3
   
    __m256i vv0a15hi3 = _mm256_permutevar8x32_epi32(vv0a15hi2b, mascara);
    
        
    AVX2_uint16_MINMAX_regs(vv0a15lob, vv0a15hi3);
    
  

    
    __m256i mascara2 = _mm256_set_epi32(
        0x04, 0x05, 0x06, 0x07, 
        0x00, 0x01, 0x02, 0x03  
    );
    __m256i vv0a15hi4 = _mm256_permutevar8x32_epi32(vv0a15hi3, mascara2);
 
    //ECA8|6420
    //9BDF|1357
 
    AVX2_uint16_MINMAX_regs(vv0a15lob, vv0a15hi4);
 
    //9BA8|1320
    //ECDF|6457
 
    //blend
    __m256i vv0a15lobb = _mm256_blend_epi32(vv0a15lob, vv0a15hi4, 0xCC); // 0b11001100
    //ECA8|6420
    //ECDF|6457
    __m256i vv0a15hi4b = _mm256_blend_epi32(vv0a15lob, vv0a15hi4, 0x33); //0B00110011
    //ECA8|6420
    //9BDF|1357
    __m256i vv0a15hi4p = _mm256_permutevar8x32_epi32(vv0a15hi4b, mascara2);
    
    //
   

    //deep = 5
   
    //ECA8|6420
    //FDB9|7531
    //

    __m256i vv0a15hi5 = _mm256_permutevar8x32_epi32(vv0a15hi4p, mascara);
    //ECA8|6420
    //DF9B|5713
    
    __m256i vv0a15lo5 = _mm256_blend_epi32(vv0a15lobb, vv0a15hi5, 0xAA);
    __m256i vv0a15hi5blend = _mm256_blend_epi32(vv0a15lobb, vv0a15hi5, 0x55);
    //DC98|5410
    //EFAB|6723
    __m256i vv0a15hi5p = _mm256_permutevar8x32_epi32(vv0a15hi5blend, mascara);
    //DC98|5410
    //EFBA|7632

    


    
    AVX2_uint16_MINMAX_regs(vv0a15lo5, vv0a15hi5p);
    
    //DC98|5410
    //EFBA|7632

    vv0a15hi5 = _mm256_permutevar8x32_epi32(vv0a15hi5p, mascara);
    //ECA8|6420
    //DF9B|5713

    //DC98|5410
    //FEAB|6723
    __m256i vv0a15lo5blend = _mm256_blend_epi32(vv0a15lo5, vv0a15hi5, 0xAA);//0b
    
    vv0a15hi5blend = _mm256_blend_epi32(vv0a15lo5, vv0a15hi5, 0x55);
    //DC98|5410
    //EFAB|6723
    vv0a15hi5p = _mm256_permutevar8x32_epi32(vv0a15hi5blend, mascara);

     
   
    //ECA8|6420
    //FDB9|7531
    
    //deep = 6
    AVX2_uint16_MINMAX_regs(vv0a15lo5blend, vv0a15hi5p);


    //deep = 7

    //ECA8|6420
    //FDB9|7531

    __m256i mascara3 = _mm256_set_epi32(
        0x00, 0x01, 0x02, 0x03, 
        0x04, 0x05, 0x06, 0x07  
    );
     __m256i vv0a15hi7p = _mm256_permutevar8x32_epi32(vv0a15hi5p, mascara3);
    
    //ECA8|6420
    //1357|9BDF

    AVX2_uint16_MINMAX_regs(vv0a15lo5blend, vv0a15hi7p);
    //1357|6420
    //ECA8|9BDF
   
    //blend
    __m256i vv0a15lo5bb = _mm256_blend_epi32(vv0a15lo5blend, vv0a15hi7p, 0xF0);
    
    __m256i vv0a15hi7b = _mm256_blend_epi32(vv0a15lo5blend, vv0a15hi7p, 0x0F);

    __m256i vv0a15hi7bp = _mm256_permutevar8x32_epi32(vv0a15hi7b, mascara3);
    //ECA8|6420
    //FDB9|7531


    //deep = 8
    //ECA8|6420
    //FDB9|7531
    __m256i vv0a15lo8b = _mm256_blend_epi32(vv0a15lo5bb, vv0a15hi7bp, 0x33);
    
    __m256i vv0a15hi8b = _mm256_blend_epi32(vv0a15lo5bb, vv0a15hi7bp, 0xCC);
    //ECB9|6431
    //FDA8|7520
     __m256i mascara4 = _mm256_set_epi32(
        0x05, 0x04, 0x07, 0x06, 
        0x01, 0x00, 0x03, 0x02  
    );
    __m256i vv0a15hi8p = _mm256_permutevar8x32_epi32(vv0a15hi8b, mascara4);
    //ECB9|6431
    //A8FD|2075
    AVX2_uint16_MINMAX_regs(vv0a15lo8b, vv0a15hi8p);
    //A8B9|2031
    //ECFD|6475
    __m256i vv0a15lo8bb = _mm256_blend_epi32(vv0a15lo8b, vv0a15hi8p, 0xCC);
    __m256i vv0a15hi8bb = _mm256_blend_epi32(vv0a15lo8b, vv0a15hi8p, 0x33);
    //ECB9|6431
    //A8FD|2075
    __m256i vv0a15hi8pp = _mm256_permutevar8x32_epi32(vv0a15hi8bb, mascara4);
    //ECB9|6431
    //FDA8|7520
    __m256i vv0a15lo8bbb = _mm256_blend_epi32(vv0a15lo8bb, vv0a15hi8pp, 0x33);
    __m256i vv0a15hi8bbb = _mm256_blend_epi32(vv0a15lo8bb, vv0a15hi8pp, 0xCC);
    //ECA8|6420
    //FDB9|7531
    
    //deep = 9

    //ECA8|6420
    //FDB9|7531
    //

    __m256i vv0a15hi9 = _mm256_permutevar8x32_epi32(vv0a15hi8bbb, mascara);
    //ECA8|6420
    //DF9B|5713
    
    __m256i vv0a15lo9 = _mm256_blend_epi32(vv0a15lo8bbb, vv0a15hi9, 0xAA);
    __m256i vv0a15hi9b = _mm256_blend_epi32(vv0a15lo8bbb, vv0a15hi9, 0x55);
    //DC98|5410
    //EFAB|6723
    __m256i vv0a15hi9pp = _mm256_permutevar8x32_epi32(vv0a15hi9b, mascara);
    //DC98|5410
    //EFBA|7632

    


    
    AVX2_uint16_MINMAX_regs(vv0a15lo9, vv0a15hi9pp);
    
    //DC98|5410
    //EFBA|7632

    vv0a15hi9 = _mm256_permutevar8x32_epi32(vv0a15hi9pp, mascara);
    //ECA8|6420
    //DF9B|5713

    //DC98|5410
    //FEAB|6723
    __m256i vv0a15lo9b = _mm256_blend_epi32(vv0a15lo9, vv0a15hi9, 0xAA);//0b
    
    vv0a15hi9b = _mm256_blend_epi32(vv0a15lo9, vv0a15hi9, 0x55);
    //DC98|5410
    //EFAB|6723
    __m256i vv0a15hi9p = _mm256_permutevar8x32_epi32(vv0a15hi9b, mascara);

      
   
    //ECA8|6420
    //FDB9|7531
    
    //deep = 10
    AVX2_uint16_MINMAX_regs(vv0a15lo9b, vv0a15hi9p);

    _mm256_storeu_si256((__m256i *)&a[0], vv0a15lo9b);

    _mm256_storeu_si256((__m256i *)&a[8], vv0a15hi9p);
    return;   
    /*
    //Deep = 6
    __m256i vv0a15lo6 = _mm256_blend_epi32(vv0a15lo5b, vv0a15hi5b, 0xCC);
    
    __m256i vv0a15hi6blend = _mm256_blend_epi32(vv0a15lo5b, vv0a15hi5b, 0x33);

    //EB8D|6305
    //C9AF|4127

    __m256i vv0a15hi6 = _mm256_permutevar8x32_epi32(vv0a15hi6blend, mascara2);
    //EB8D|6305
    //FA9C|7214
    AVX2_uint16_MINMAX_regs(vv0a15lo6, vv0a15hi6);

    //blend
    __m256i vv0a15lo6b = _mm256_blend_epi32(vv0a15lo6, vv0a15hi6, 0xAA);
    
    __m256i vv0a15hi6b = _mm256_blend_epi32(vv0a15lo6, vv0a15hi6, 0x55);
    //

    //deep =7
    __m256i mascara3 = _mm256_set_epi32(
        0x01, 0x00, 0x03, 0x02, 
        0x05, 0x04, 0x07, 0x06  
    );
    __m256i vv0a15hi7 = _mm256_permutevar8x32_epi32(vv0a15hi6b, mascara3);
    //EB8D|6305
    //1472|9CFA
    AVX2_uint16_MINMAX_regs(vv0a15lo6b, vv0a15hi7);
    //blend
    __m256i vv0a15lo6bb = _mm256_blend_epi32(vv0a15lo6b, vv0a15hi7, 0x0F);
    
    __m256i vv0a15hi7b = _mm256_blend_epi32(vv0a15lo6b, vv0a15hi7, 0xF0);
    //

    //deep=8
    __m256i mascara4 = _mm256_set_epi32(
        0x00, 0x01, 0x02, 0x03, 
        0x04, 0x05, 0x06, 0x07  
    );
    __m256i vv0a15hi8 = _mm256_permutevar8x32_epi32(vv0a15hi7b, mascara4);
    //EB8D|6305
    //AFC9|2741
    AVX2_uint16_MINMAX_regs(vv0a15lo6bb, vv0a15hi8);
    //blend
    __m256i vv0a15lo6bbb = _mm256_blend_epi32(vv0a15lo6bb, vv0a15hi8, 0x99);
    
    __m256i vv0a15hi8b = _mm256_blend_epi32(vv0a15lo6bb, vv0a15hi8, 0x66);
    //
    //deep = 9
    __m256i mascara5 = _mm256_set_epi32(
        0x05, 0x04, 0x07, 0x06, 
        0x01, 0x00, 0x03, 0x02  
    );
    __m256i vv0a15hi9 = _mm256_permutevar8x32_epi32(vv0a15hi8b, mascara5);
    //EB8D|6305
    //C9AF|4127
    AVX2_uint16_MINMAX_regs(vv0a15lo6bbb, vv0a15hi9);
    //blend
    __m256i vv0a15lo6bbbb = _mm256_blend_epi32(vv0a15lo6bbb, vv0a15hi9, 0x33);
    
    __m256i vv0a15hi9b = _mm256_blend_epi32(vv0a15lo6bbb, vv0a15hi9, 0xCC);
    //

    //deep = 10 ultima
    __m256i mascara6 = _mm256_set_epi32(
        0x04, 0x05, 0x06, 0x07, 
        0x00, 0x01, 0x02, 0x03  
    );
    __m256i vv0a15hi10 = _mm256_permutevar8x32_epi32(vv0a15hi9b, mascara6);

    //EB8D|6305
    //FA9C|7214
    AVX2_uint16_MINMAX_regs(vv0a15lo6bbb, vv0a15hi10);
    
    __m256i vv0a15loO = _mm256_permute2x128_si256(vv0a15lo6bbb, vv0a15hi10, 0x20 );
    __m256i vv0a15hiO = _mm256_permute2x128_si256(vv0a15lo6bbb, vv0a15hi10, 0x31 );
    //7214|6305
    //FA9C|EB8D

    __m256i mascaraC= _mm256_set_epi32(
        0x07, 0x03, 0x00, 0x04, 
        0x02, 0x06, 0x05, 0x01  
    );
   
    v0a7 = _mm256_permutevar8x32_epi32(vv0a15loO, mascaraC);
    v8a15 = _mm256_permutevar8x32_epi32(vv0a15hiO, mascaraC);

   _mm256_storeu_si256((__m256i *)&a[0], v0a7);

   _mm256_storeu_si256((__m256i *)&a[8], v8a15);

    */
}

int main() {
    int32_t a[16] = {42, 15, 8, 99, 23, 56, 4, 71, 12, 33, 88, 60, 19, 2, 45, 77};
    print_vector("a", a, sizeof(a) / sizeof(a[0]));
    sorting16(a);
    print_vectorOrdem("a", a, sizeof(a) / sizeof(a[0]));
    //print_vector("a", a, sizeof(a) / sizeof(a[0]));
    return 0;
}