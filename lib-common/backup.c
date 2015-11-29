#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
    return ((a >> 16) * (b >> 16)) << 16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
    return (a / b) << 16;
}

FLOAT f2F(float a) {
    return (a * (1 << 16));
}

FLOAT Fabs(FLOAT a) {
    return a & (1 << 31);
}

/*
FLOAT sqrt(FLOAT x) {
    FLOAT dt, t = int2F(2);

    do {
        dt = F_div_int((F_div_F(x, t) - t), 2);
        t += dt;
    } while(Fabs(dt) > f2F(1e-4));

    return t;
}
*/



/* we only compute x^0.333 */
/*
FLOAT pow(FLOAT x, FLOAT y) {
    FLOAT t2, dt, t = int2F(2);

    do {
        t2 = F_mul_F(t, t);
        dt = (F_div_F(x, t2) - t) / 3;
        t += dt;
    } while(Fabs(dt) > f2F(1e-4));

    return t;
}
*/
