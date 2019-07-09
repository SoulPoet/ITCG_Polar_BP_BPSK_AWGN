#include "AWGN_Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

void Channel::Init(const char *config_file_path) {
    FILE *in_file = OpenFile(config_file_path, "r");
    char str[50] = "";
    while(~fscanf(in_file, "%[a-z]=", str)) {
        if(!strcmp(str, "segma"))
            fscanf(in_file, "%lf", &m_segma);
        str[0] = '\0';
        fscanf(in_file, "%*c");
    }
    fclose(in_file);
}

void Channel::transmit(int *in, double *out, int len) {
    //long int seed = time(NULL);
    Normal(out, len);
    for(int i = 0; i < len; ++i)
        out[i] = in[i] + out[i] * m_segma;
}

void Channel::Clean() {

}

double Channel::CntNoisePower() {
    return m_segma * m_segma;
}

void Channel::SetNoisePower(double snr, double signal_power) {
    m_segma = sqrt(signal_power * pow(10.0, -snr / 10.0));
}
