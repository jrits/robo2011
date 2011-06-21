#include <stdio.h>
#include <math.h>
#include <float.h> // FLT_MAX 

// /usr/bin/g++ TestMath.cpp
int main() {
    float Inf = 1/0.0;
    float NInf = -1/0.0;
    float Nan = 0.0/0.0;
    printf("%f %f %f\n", Inf, NInf, Nan);
    printf("%d %d %d\n", isfinite(Inf), isfinite(NInf), isfinite(Nan));
    printf("%d %d %d\n", isinf(Inf), isinf(NInf), isnan(Nan));
    float RInf = 1 / Inf;
    float RNInf = 1 / NInf;
    float RNan = 1 / Nan;
    printf("%f %f %f\n", RInf, RNInf, RNan);
    printf("%d %d %d\n", isfinite(RInf), isfinite(RNInf), isfinite(RNan));
    printf("%d %d %d\n", isinf(RInf), isinf(RNInf), isnan(RNan));
    float AInf = Inf + 1;
    float ANInf = NInf + 1;
    float ANan = Nan + 1;
    printf("%f %f %f\n", AInf, ANInf, ANan);
    printf("%d %d %d\n", isfinite(AInf), isfinite(ANInf), isfinite(ANan));
    printf("%d %d %d\n", isinf(AInf), isinf(ANInf), isnan(ANan));
    float MInf = Inf * 2.0;
    float MNInf = NInf * 2.0;
    float MNan = Nan * 2.0;
    printf("%f %f %f\n", MInf, MNInf, MNan);
    printf("%d %d %d\n", isfinite(MInf), isfinite(MNInf), isfinite(MNan));
    printf("%d %d %d\n", isinf(MInf), isinf(MNInf), isnan(MNan));

    float FInf = FLT_MAX * 10;
    float FNInf = -FLT_MAX * 10;
    printf("%f %f %f %f\n", FLT_MAX, FInf, -FLT_MAX, FNInf);
    printf("%d %d\n", isfinite(FInf), isfinite(FNInf));
    printf("%d %d\n", isinf(FInf), isinf(FNInf));
}
// http://www.aerith.net/cpp/typical_bug-j.html
// http://www.qnx.co.jp/developers/docs/6.4.1/neutrino/lib_ref/i/isinf.html
