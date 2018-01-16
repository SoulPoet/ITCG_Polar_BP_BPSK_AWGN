#include "CommonFunc.h"

#ifndef _ITCG_BPSK_Modulater_
    #define _ITCG_BPSK_Modulater_

    class Modulater {
    public:
        Modulater();
        ~Modulater();
        static void Modulate(int *in, int *out, int len);
        static double CntSignalPower();
    };

#endif
