#include <stdio.h>
#include <arm_neon.h> 
void soma_neon(int32_t *a, int32_t *b, int32_t *c, int n) {
    int i;
    
    for (i = 0; i < n; i += 4) {
       
        int32x4_t va = vld1q_s32(&a[i]);
        int32x4_t vb = vld1q_s32(&b[i]);

        
        int32x4_t vresult = vaddq_s32(va, vb);

        
        vst1q_s32(&c[i], vresult);
    }
}

int main() {
    int n = 8;
    int32_t a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int32_t b[] = {10, 20, 30, 40, 50, 60, 70, 80};
    int32_t c[8];

    soma_neon(a, b, c, n);

    printf("Resultado: ");
    for(int i=0; i<n; i++) printf("%d ", c[i]);
    printf("\n");

    return 0;
}