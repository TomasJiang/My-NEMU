#include "FLOAT.h"

typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

FLOAT F_mul_F(FLOAT a, FLOAT b) {
    return ((a >> 16) * (b >> 16)) << 16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
    return (a / b) << 16;
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

FLOAT Fabs(FLOAT a) {
    uint32_t sign = (a >> 31) & 0x1;
    if (sign) {
        a = -a;
    }
    return a; //& (1 << 31);
}

FLOAT sqrt(FLOAT x) {
    FLOAT dt, t = int2F(2);

    do {
        dt = F_div_int((F_div_F(x, t) - t), 2);
        t += dt;
    } while(Fabs(dt) > f2F(1e-4));

    return t;
}



/* we only compute x^0.333 */
FLOAT pow(FLOAT x, FLOAT y) {
    FLOAT t2, dt, t = int2F(2);

    do {
        t2 = F_mul_F(t, t);
        dt = (F_div_F(x, t2) - t) / 3;
        t += dt;
    } while(Fabs(dt) > f2F(1e-4));

    return t;
}
