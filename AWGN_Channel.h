#include "CommonFunc.h"

#ifndef _ITCG_AWGN_CHANNEL_
    #define _ITCG_AWGN_CHANNEL_

    class Channel {
    public:
        double m_segma;

        Channel();
        ~Channel();
        void Init(const char *config_file_path);
        void transmit(int *in, double *out, int len);
        void Clean();
        double CntNoisePower();
        void SetNoisePower(double snr, double signal_power);
    };

#endif
