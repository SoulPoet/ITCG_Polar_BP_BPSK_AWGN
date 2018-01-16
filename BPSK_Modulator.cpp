#include "BPSK_Modulater.h"

Modulater::Modulater() {

}

Modulater::~Modulater() {

}

void Modulater::Modulate(int *in, int *out, int len) {
    for(int i = 0; i <len; ++i)
        out[i] = (in[i] ? -1 : 1);
}

double Modulater::CntSignalPower() {
    return 1.0;
}
