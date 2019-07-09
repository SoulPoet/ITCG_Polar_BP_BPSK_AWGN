#include "PolarCodec.hpp"

inline double CCodec::degen_fai(double y) {
    if(y >= 0 && y <= 0.68458)
        return pow((0.0218 - log(y)) / 0.4527, 1.0 / 0.86);
        //pow(((log(y)-0.0218)/0.4527),(1.0/0.86));
    return (0.4856 - sqrt(0.4856 * 0.4856 + 4.0 * 0.0564 * log(y))) / (2.0 * 0.0564);
}

inline double CCodec::generate_fai(double x) {
    if(x >= 0 && x <= 0.867861)
        return exp(0.0564 * x * x - 0.4856 * x);
    return exp(-0.4527 * pow(x, 0.86) + 0.0218);
}

inline void CCodec::Gaussian_Approximation() {
    BitArr *Iw_N = new BitArr[m_codeword_len];
    double *v = new double[m_codeword_len];
    double *z = new double[m_codeword_len];
    double tmp = 2.0 * pow(10, 0.1 * m_design_SNR);
    for(int i = 0; i < m_codeword_len; i++) z[i] = tmp;
    for(int j = 1; j < m_Layer_num; ++j) {
        int u = pow(2.0, m_Layer_num - j - 1);
        for(int i = 1; i < m_codeword_len / (u << 1) + 1; ++i) {
            for(int k = 0; k < u; ++k) {
                int t = (i - 1) * (u << 1) + k;
                int s = t + u;
                v[t] = degen_fai(1.0 - (1.0 - generate_fai(z[t])) * (1.0 - generate_fai(z[s])));
                v[s] = z[t] + z[s];
            }
        }
        swap(z, v); //z[] = v[]
    }

    for(int i = 0; i < m_codeword_len; ++i) {
        Iw_N[i].index = i;
        Iw_N[i].metric = z[i];
    }
    delete []v;
    delete []z;
    sort(Iw_N, Iw_N + m_codeword_len);
    memset(m_isFrozen, true, sizeof(bool) * m_codeword_len);
    for(int i = 0; i < m_message_blk_len; ++i)
        m_isFrozen[Iw_N[i].index] = false;
    delete []Iw_N;
}

CCodec::CCodec() {}

CCodec::~CCodec() {}

inline void CCodec::LLR_Clip(double &d) {
    if(d < -LLR_INF) d = -LLR_INF;
    if(d > LLR_INF) d = LLR_INF;
}

void CCodec::Init(const char *config_file_path) {
	char str[50] = "";
    FILE *in_file = OpenFile(config_file_path, "r");
    while(~fscanf(in_file, "%[a-z_]=", str)) {
        if(!strcmp(str, "codeword_len")) {
            fscanf(in_file, "%d", &m_codeword_len);
            m_isFrozen = new bool[m_codeword_len];
            m_Layer_num = log((double)m_codeword_len) / log(2.0) + 1.5;
            m_LLR = new double*[m_Layer_num];
            m_Info_eti = new double*[m_Layer_num];
            m_Layer_info = new int*[m_Layer_num];
            for(int i = 0; i < m_Layer_num; ++i) {
            	m_LLR[i] = new double[m_codeword_len];
                m_Info_eti[i] = new double[m_codeword_len];
                m_Layer_info[i] = new int[m_codeword_len];
            }
        }
        else if(!strcmp(str, "message_blk_len"))
            fscanf(in_file, "%d", &m_message_blk_len);
        else if(!strcmp(str, "frozen_bits")) {
        	int tmp;
            for(int i = 0; i < m_codeword_len; ++i) {
                fscanf(in_file, "%d", &tmp);
                m_isFrozen[i] = tmp - 1;
                if(m_isFrozen[i]) m_Layer_info[0][i] = 0;
            }
        }
        else if(!strcmp(str, "max_iter_num"))
            fscanf(in_file, "%d", &m_max_iter_num);
        else if(!strcmp(str, "design_snr"))
            fscanf(in_file, "%lf", &m_design_SNR);
        str[0] = '\0';
        fscanf(in_file, "%*c");
    }
    fclose(in_file);
    Gaussian_Approximation();
}

void CCodec::Hadamard_Encoder(int *in, int *out, int blk) {
	int in_idx = 0, out_idx = 0;
    while(blk--) {
        for(int i = 0; i < m_codeword_len; ++i)
            m_Layer_info[0][i] = m_isFrozen[i] ? 0 : in[in_idx++];
        for(int i = 1; i < m_Layer_num; ++i) {
            int tmp = 1 << (i - 1);
            for(int j = 0; j < m_codeword_len; ++j) {
                if(j & tmp) m_Layer_info[i][j] = m_Layer_info[i - 1][j];
                else m_Layer_info[i][j] = m_Layer_info[i - 1][j] ^ m_Layer_info[i - 1][j | tmp];
            }
        }
        for(int i = 0; i < m_codeword_len; ++i)
            out[out_idx++] = m_Layer_info[m_Layer_num - 1][i];
    }
}

inline double CCodec::Tian(const double a, const double b) {
    double tmp = tanh(a / 2.0) * tanh(b / 2.0);
    return log((1.0 + tmp) / (1.0 - tmp));
}

inline void CCodec::Parallel_BP_Update_LLR() {
    for(int i = m_Layer_num - 1; i; --i) {
        int tmp = 1 << (i - 1);
        for(int j = 0; j < m_codeword_len; ++j) {
            if(j & tmp) m_LLR[i - 1][j] = m_LLR[i][j] + Tian(m_LLR[i][j ^ tmp], m_Info_eti[i - 1][j ^ tmp]);
            else m_LLR[i - 1][j] = Tian(m_LLR[i][j], m_LLR[i][j | tmp] + m_Info_eti[i - 1][j | tmp]);
            LLR_Clip(m_LLR[i - 1][j]);
        }
    }
}

inline void CCodec::Parallel_BP_Update_InfoMap() {
    for(int i = 1; i < m_Layer_num; ++i) {
        int tmp = 1 << (i - 1);
        for(int j = 0; j < m_codeword_len; ++j)  {
            if(j & tmp) m_Info_eti[i][j] = m_Info_eti[i - 1][j] + Tian(m_Info_eti[i - 1][j ^ tmp], m_LLR[i][j ^ tmp]);
            else m_Info_eti[i][j] = Tian(m_Info_eti[i - 1][j], m_Info_eti[i - 1][j | tmp] + m_LLR[i][j | tmp]);
            LLR_Clip(m_Info_eti[i][j]);
        }
    }
}

inline void CCodec::Parallel_BP_Proc() {
    Parallel_BP_Update_LLR();
    Parallel_BP_Update_InfoMap();
}

void CCodec::Parallel_BP_Decoder_4_BPSK_AWGN(double *in, int *out, double segma) {
    double tmp = 2.0 / (segma * segma);
    for(int i = 0; i < m_codeword_len; ++i)
        if(m_isFrozen[i]) m_Info_eti[0][i] = LLR_INF;
        else m_Info_eti[0][i] = 0.0;
    for(int i = 0; i < m_codeword_len; ++i) {
        m_LLR[m_Layer_num - 1][i] = tmp * in[i];
    }
    for(int i = 1; i < m_Layer_num; ++i) {
        for(int j = 0; j < m_codeword_len; ++j)
            m_Info_eti[i][j] = 0.0;
    }

    int iter = 0;
    for(; iter < m_max_iter_num; ++iter)
        Parallel_BP_Proc();
    // double max_LLR = 0.0;
    // int max_LLR_idx = 0;
    // for(; iter < m_max_iter_num; ++iter) {
    //     Pallel_BP_Proc();
    //     for(int i = 0; i < m_codeword_len; ++i) {
    //         if(!m_isFrozen[i] && !SGN(m_Info_eti[0][i]) && fabs(m_LLR[0][i]) > max_LLR) {
    //             max_LLR = fabs(m_LLR[0][i]);
    //             max_LLR_idx = i;
    //         }
    //     }
    //     m_Info_eti[0][max_LLR_idx] = max_LLR > 0 ? LLR_INF : -LLR_INF;
    // }
    int out_idx = 0;
    for(int i = 0; i < m_codeword_len; ++i) {
        if(!m_isFrozen[i])
            out[out_idx++] = (m_Info_eti[0][i] + m_LLR[0][i] > 0) ? 0 : 1;
    }
}

void CCodec::Clean() {
	delete []m_isFrozen;
	for(int i = 0; i < m_Layer_num; ++i) {
		delete [](m_LLR[i]);
        delete [](m_Info_eti[i]);
		delete [](m_Layer_info[i]);
	}
	delete []m_LLR;
    delete []m_Info_eti;
	delete []m_Layer_info;
}

int CCodec::CalcBlkNum(int bit_num) {
    return bit_num / m_message_blk_len;
}
