#include <stdio.h>
#include <arm_neon.h>
#define MT 0xFFFF
#define MF 0x0000
/*
#define Neon_uint16MINMAX(a,b)
    do{

    }
    while(0)
*/
/*
Instruções interessantes

    vzip1q_s16(a, b)
        A0 B0 A1 B1 ... A3 B3
    vzip2q_s16(a, b)
        A4 B4... A7 B7
    
    void MIN_MAX(int16x8_t *a, int16x8_t *b){
    uint16x8_t mask = vcgtq_s16(*a, *b); 

    int16x8_t min = vbslq_s16(mask, *b, *a);

    int16x8_t max = vbslq_s16(mask, *a, *b);

    *a = min;
    *b = max;
}

*/
void MIN_MAX(int16x8_t *a, int16x8_t *b){
    int16x8_t val_a = *a;
    int16x8_t val_b = *b;

    // vminq_s16: Compara elemento a elemento e retorna o menor de cada par
    *a = vminq_s16(val_a, val_b); 

    // vmaxq_s16: Compara elemento a elemento e retorna o maior de cada par
    *b = vmaxq_s16(val_a, val_b);
}
// nesse tipo de programa usar unsigned(u) ou inteiro(s)


void print_vector(const char *name, const int16_t vec[], int16_t n) {
  printf("%s = {", name);
  for (int16_t i = 0; i < n; i++) {
    printf("%d, ", vec[i]);
  }
  printf("}\n");
}
void print_vectorOrdem(const char *name, const int16_t vec[], int16_t n){
    printf("%s = {", name);
    for (int16_t i = 0; i < n; i++){
        printf("%d, ", vec[(n/2)*(i&1)+i/2]);
    }
    printf("}\n");
}

void sorting16(int16_t *a){
    int16x8_t va = vld1q_s16(&a[0]);
    int16x8_t vb = vld1q_s16(&a[8]);
    //Deep=1
    //7654|3210
    //FEDC|BA98

    int16x8_t v0a7d1 = vuzp1q_s16(va, vb); 
   
    int16x8_t v8a15d1 = vuzp2q_s16(va, vb);

    MIN_MAX(&v0a7d1, &v8a15d1);

    //Deep=2 tbl teste
    int16x8_t v0a7d2 = vtrn1q_s16(v0a7d1, v8a15d1);
    int16x8_t v8a15d2 = vtrn2q_s16(v0a7d1, v8a15d1);
    v8a15d2 = vrev32q_s16(v8a15d2);
    MIN_MAX(&v0a7d2, &v8a15d2);

    int16x8_t v8a15d3 = vrev32q_s16(v8a15d2);
    int16x8_t v0a7d3 = vtrn1q_s16(v0a7d2, v8a15d3);
    v8a15d3 = vtrn2q_s16(v0a7d2, v8a15d3);

    MIN_MAX(&v0a7d3, &v8a15d3);

    //deep=4 tbl bsl

    /*
    int8x16_t v0a7d4 = vreinterpretq_s8_s16(v0a7d3);
    int8x16_t v8a15d4 = vreinterpretq_s8_s16(v8a15d3);
    int8x16x2_t v0a15d4 = { v0a7d4, v8a15d4 };

    uint8x16_t mask2 = {
        22, 23, 6, 7, 20, 21, 4, 5,       
        30, 31, 14, 15, 28, 29, 12, 13
    };
    uint8x16_t mask1 = {
        0, 1, 16, 17, 2, 3, 18, 19,
        8, 9, 24, 25, 10, 11, 26, 27
    };
    int8x16_t v0a7d4_ = vqtbl2q_s8(v0a15d4, mask1);
    int8x16_t v8a15d4_ = vqtbl2q_s8(v0a15d4, mask2);
    int16x8_t v0a7d4__ = vreinterpretq_s16_s8(v0a7d4_);
    int16x8_t v8a15d4__ = vreinterpretq_s16_s8(v8a15d4_);
    */

    /*
    int16x8_t v0a7d4 = vzip1q_s16(v0a7d3, v8a15d3); 
    int16x8_t v8a15d4 = vzip2q_s16(v0a7d3, v8a15d3);
    int16x8_t v0a7d4_ = vextq_s16(v0a7d4, v0a7d4, 4);
    int16x8_t v0a7d4__ = vextq_s16(v0a7d4_,v8a15d4, 4);
    int16x8_t v8a15d4_ = vextq_s16(v8a15d4, v0a7d4_, 4);
    int16x8_t v8a15d4__ = vextq_s16(v8a15d4_, v8a15d4_, 4);

    v8a15d4__= vrev64q_s16(v8a15d4__);
    */
    
    int16x8_t v0a7d4_= v0a7d3;
     int16x8_t v8a15d4= v8a15d3;
    int16x8_t v8a15d4_= vrev64q_s16(v8a15d3);
    uint16x8_t mascara = {
    MF, MF, MT, MT, MF, MF, MT, MT
    
    };

    int16x8_t v0a7d4__ = vbslq_s16(mascara, v8a15d4_, v0a7d4_); 
    int16x8_t v8a15d4__ = vbslq_s16(mascara, v0a7d4_, v8a15d4_);
    

    MIN_MAX(&v0a7d4__, &v8a15d4__);

    //deep 5
    // po
    int16x8_t v0a7d5 = vuzp1q_s16(v0a7d4__, v8a15d4__); 
   
    int16x8_t v8a15d5 = vuzp2q_s16(v0a7d4__, v8a15d4__);

    uint16x8_t mascara2 = {
    MF, MT, MF, MT, MF, MT, MF, MT
    
    };

    int16x8_t v0a7d5__ = vbslq_s16(mascara2, v8a15d5, v0a7d5); 
    int16x8_t v8a15d5__ = vbslq_s16(mascara2, v0a7d5, v8a15d5);

    MIN_MAX(&v0a7d5__, &v8a15d5__);

    //deep 6
    int16x8_t v0a7d6 = vuzp1q_s16(v0a7d5__, v8a15d5__); 
   
    int16x8_t v8a15d6 = vuzp2q_s16(v0a7d5__, v8a15d5__);

    int16x8_t v0a7d6__ = vbslq_s16(mascara, v8a15d6, v0a7d6); 
    int16x8_t v8a15d6__ = vbslq_s16(mascara, v0a7d6, v8a15d6);

    MIN_MAX(&v0a7d6__, &v8a15d6__);

    //deep 7
    int16x8_t v8a15d7 = vrev64q_s16(v8a15d6__);
    int16x8_t v8a15d7_ = vextq_s16(v8a15d7, v8a15d7, 4);
    
    int16x8_t v0a7d7__ = vbslq_s16(mascara2, v8a15d7_, v0a7d6__); 
    int16x8_t v8a15d7__ = vbslq_s16(mascara2, v0a7d6__, v8a15d7_);
    

    MIN_MAX(&v0a7d7__, &v8a15d7__);
    //deep 8 
    int8x16_t v0a7d8 = vreinterpretq_s8_s16(v0a7d7__);
    int8x16_t v8a15d8 = vreinterpretq_s8_s16(v8a15d7__);
    int8x16x2_t v0a15d8 = { v0a7d8, v8a15d8 };

    uint8x16_t mask2 = {
        4, 5, 12, 13, 10, 11, 2, 3,       
        22, 23, 30, 31, 24, 25, 16, 17
    };
    uint8x16_t mask1 = {
        0, 1, 8, 9, 14, 15, 6, 7,
        18, 19, 26, 27, 28, 29, 20, 21
    };
    int8x16_t v0a7d8_ = vqtbl2q_s8(v0a15d8, mask1);
    int8x16_t v8a15d8_ = vqtbl2q_s8(v0a15d8, mask2);
    int16x8_t v0a7d8__ = vreinterpretq_s16_s8(v0a7d8_);
    int16x8_t v8a15d8__ = vreinterpretq_s16_s8(v8a15d8_);

    MIN_MAX(&v0a7d8__, &v8a15d8__);

    //deep 9
    int16x8_t v0a7d9__ = vuzp1q_s16(v0a7d8__, v8a15d8__); 
   
    int16x8_t v8a15d9__ = vuzp2q_s16(v0a7d8__, v8a15d8__);

    MIN_MAX(&v0a7d9__, &v8a15d9__);

    //deep 10

    int16x8_t v0a7d10 = vuzp1q_s16(v0a7d9__, v8a15d9__); 
   
    int16x8_t v8a15d10 = vuzp2q_s16(v0a7d9__, v8a15d9__);

    MIN_MAX(&v0a7d10, &v8a15d10);
    // ajeitando
    int8x16_t v0a7d10_ = vreinterpretq_s8_s16(v0a7d10);
    int8x16_t v8a15d10_ = vreinterpretq_s8_s16(v8a15d10);
    int8x16x2_t v0a15d10_ = { v0a7d10_, v8a15d10_ };

    uint8x16_t mask4 = {
        16, 17, 24, 25, 20, 21, 28, 29,       
        18, 19, 26, 27, 22, 23, 30, 31
    };
    uint8x16_t mask3 = {
        0, 1, 8, 9, 4, 5, 12, 13,
        2, 3, 10, 11, 6, 7, 14, 15
    };
    int8x16_t v0a7d10__ = vqtbl2q_s8(v0a15d10_, mask3);
    int8x16_t v8a15d10__ = vqtbl2q_s8(v0a15d10_, mask4);
    int16x8_t v0a7d10___ = vreinterpretq_s16_s8(v0a7d10__);
    int16x8_t v8a15d10___ = vreinterpretq_s16_s8(v8a15d10__);
    
    vst1q_s16(&a[0], v0a7d10___);
    vst1q_s16(&a[8], v8a15d10___);



    

    return;
    

    




    
}


int main() {
    int16_t a[16] = {10, 12, 10 ,5 ,4, 3, 0, 15, 14, 7, 6, 9, 8, 13, 2, 1};
    //int16_t a[16] = {16, 15, 14 ,13 ,12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    print_vector("a", a, sizeof(a) / sizeof(a[0]));
    sorting16(a);
    print_vectorOrdem("a", a, sizeof(a) / sizeof(a[0]));
    //print_vector("a", a, sizeof(a) / sizeof(a[0]));

    return 0;
}