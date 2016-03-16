#include "FLOAT.h"

typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

FLOAT F_mul_F(FLOAT a, FLOAT b) {
    return ((uint64_t)a * (uint64_t)b) >> 16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
    // return (((uint64_t)a << 32) / (uint64_t)b) >> 16;

    int sign = 1;
    if (a < 0) {
        sign *= -1;
        a = -a;
    }
    if (b < 0) {
        sign *= -1;
        b = -b;
    }
    int res = a / b;
    a = a % b;
    int i;
    for (i = 0; i < 16; i++) {
        a <<= 1;
        res <<= 1;
        if (a >= b) {
            a -= b;
            res++;
        }
    }
    return res * sign;
}

FLOAT f2F(float a) {
    uint32_t ui = *(uint32_t*)&a;
    uint32_t sign = ui >> 31;
    uint32_t exp = (ui >> 23) & 0xff;
    int res = ui & 0x7fffff;
    if (exp != 0)
        res += 1 << 23;
    exp -= 150;
    if (exp < -16)
        res >>= -16 - exp;
    if (exp > -16)
        res <<= exp + 16;
    return sign == 0 ? res : -res;
}

// float F2f(FLOAT a) {
//     float f = (float)(a);
//     return f / (1 << 16);
// }

FLOAT Fabs(FLOAT a) {
    uint32_t sign = (a >> 31) & 0x1;
    if (sign) {
        a = -a;
    }
    return a;
    // return a & (1 << 31);
}

FLOAT sqrt(FLOAT x) {
    // printf("\nsqrt(%f)\n", F2f(x));
    FLOAT dt, t = int2F(2);

    do {
        // printf("x = %f, t = %f, F_div_F(x, t) = %f\n", F2f(x), F2f(t), F2f(F_div_F(x, t)));
        dt = F_div_int((F_div_F(x, t) - t), 2);
        // printf("dt = %d, %f\n", dt, F2f(dt));
        t += dt;
        // printf("t = %d, %f\n", t, F2f(t));
    } while(Fabs(dt) > f2F(1e-4));

    return t;
}


/* we only compute x^0.333 */
FLOAT pow(FLOAT x, FLOAT y) {
    FLOAT t2, dt, t = int2F(2);

    do {
        t2 = F_mul_F(t, t);
        // assert(F2f(t) * F2f(t) == F2f(t2));
        // printf("x = %f, t2 = %f, F_div_F(x, t2) = %f\n", F2f(x), F2f(t2), F2f(F_div_F(x, t2)));
        dt = (F_div_F(x, t2) - t) / 3;
        t += dt;
    } while(Fabs(dt) > f2F(1e-4));

    return t;
}


/*
   int main() {
   uint32_t a = f2F(0.5);
   uint32_t b = f2F(0.5);
   uint32_t c = f2F(-0.5);
// printf("f2F(0.5)  = 0x%x\n", a);
// printf("F2f(a)  = %f\n", F2f(a));
// printf("f2F(-0.5) = 0x%x\n", c);
// printf("F2f(c)  = %f\n", F2f(c));
// printf("Fabs(a) = 0x%x\n", Fabs(a));
// printf("Fabs(c) = 0x%x\n", Fabs(c));

FLOAT Fmul = F_mul_F(b, b);
// printf("F_mul_F(b, b) = 0x%x, %f\n", Fmul, F2f(Fmul));
FLOAT Fmuli = F_mul_int(a, 4);
// printf("F_mul_int(a, 4) = 0x%x, %f\n", Fmuli, F2f(Fmuli));
// printf("- F_mul_F(F_mul_int(a, 4), c) = 0x%x\n",
//         - F_mul_F(F_mul_int(a, 4), c));
FLOAT dt = F_mul_F(b, b) - F_mul_F(F_mul_int(a, 4), c);
// 0x14000 => 1.25
// printf("dt = 0x%x, %f\n", dt, F2f(dt));


// F_div_F, F_div_int


// printf("sqrt(16) = 0x%x, %f\n", sqrt(int2F(16)), F2f(sqrt(int2F(16))));
FLOAT sqrt_dt = sqrt(dt);
// printf("sqrt_dt = 0x%x, %f\n", sqrt_dt, F2f(sqrt_dt));

FLOAT x1 = F_div_F(-b + sqrt_dt, F_mul_int(a, 2));
printf("x1 = 0x%x, %f\n", x1, F2f(x1));
FLOAT x2 = F_div_F(-b - sqrt_dt, F_mul_int(a, 2));
printf("x2 = 0x%x, %f\n", x2, F2f(x2));

// FLOAT x1_ans = f2F(0.618);
// printf("x1_ans = 0x%x\n", x1_ans);
// FLOAT x2_ans = f2F(-1.618);
// printf("x2_ans = 0x%x\n", x2_ans);

// printf("Fabs(x1_ans - x1) = 0x%x\n", Fabs(x1_ans - x1));
// printf("Fabs(x2_ans - x2) = 0x%x\n", Fabs(x2_ans - x2));

return 0;
}
*/
