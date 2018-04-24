#include "Simulator.hpp"

Simulator::Simulator() {

}

Simulator::~Simulator() {

}

void Simulator::Init(const char *config_file_path) {
    char encoder_config_file_path[50];
    char channel_config_file_path[50];
    char str[50] = "";
    char output_file_path[50];

    FILE *in_file = OpenFile(config_file_path, "r");
    while(~fscanf(in_file, "%[a-z_]=", str)) {
        if(!strcmp(str, "bit_num"))
            fscanf(in_file, "%d", &m_bit_num);
        else if(!strcmp(str, "encoder_config_file"))
            fscanf(in_file, "%s", encoder_config_file_path);
        else if(!strcmp(str, "channel_config_file"))
            fscanf(in_file, "%s", channel_config_file_path);
        else if(!strcmp(str, "output_file"))
            fscanf(in_file, "%s", output_file_path);
        else if(!strcmp(str, "min_snr"))
            fscanf(in_file, "%lf", &m_min_snr);
        else if(!strcmp(str, "max_snr"))
            fscanf(in_file, "%lf", &m_max_snr);
        else if(!strcmp(str, "snr_step"))
            fscanf(in_file, "%lf", &m_snr_step);
        else if(!strcmp(str, "block_num")) {
            fscanf(in_file, "%d", &m_blk_num);
        }
        str[0] = '\0';
        fscanf(in_file, "%*c");
    }

    m_enc1.Init(encoder_config_file_path);
    // m_chn1.Init(channel_config_file_path);
    m_bit_num = m_enc1.m_message_blk_len;
    m_blk_len = m_enc1.m_codeword_len;
    m_tot_bit = m_bit_num * m_blk_num;
    // m_blk_num = m_enc1.CalcBlkNum(m_bit_num);

    m_source_signal = new int[m_bit_num];
    m_encoded_signal = new int[m_blk_len];
    m_send_signal = new int[m_blk_len];
    m_receive_signal = new double[m_blk_len];
    m_restore_codeword = new int[m_blk_len];
    m_restore_signal = new int[m_bit_num];

    m_out_file = OpenFile(output_file_path, "w");

}

void Simulator::Start() {
    SetSeed(-1);
    for(m_snr = m_min_snr; m_snr <= m_max_snr; m_snr += m_snr_step) {
        m_err_bit_cnt = m_err_blk_cnt = 0;
        m_chn1.SetNoisePower(m_snr, Modulater::CntSignalPower());
        for(int i = 0; i < m_blk_num; ++i) {
            if(i % 100 == 0)
                printf("SNR=%f, the m_err_bit_cnt=%d, the m_err_blk_cnt=%d\n", m_snr, m_err_bit_cnt, m_err_blk_cnt);
            SourceCreate::CreateBitStream(m_source_signal, m_bit_num);
            m_enc1.Hadamard_Encoder(m_source_signal, m_encoded_signal, 1);
            Modulater::Modulate(m_encoded_signal, m_send_signal, m_blk_len);
            m_chn1.transmit(m_send_signal, m_receive_signal, m_blk_len);
            m_enc1.Pallel_BP_Decoder4BPSK_AWGN(m_receive_signal, m_restore_signal, m_chn1.m_segma);

            for(int j = 0; j < m_bit_num; ++j)
                if(m_source_signal[j] != m_restore_signal[j])
                    ++m_err_bit_cnt;
            if(HammingDistance(m_source_signal, m_restore_signal, m_bit_num))
                ++m_err_blk_cnt;
        }

        // m_enc1.Decode4BPSK_AWGN(m_receive_signal, m_restore_codeword, m_restore_signal, m_blk_num);

        // for(int i = 0; i < m_bit_num; ++i)
        //     if(m_source_signal[i] != m_restore_signal[i])
        //         ++m_err_bit_cnt;
        // for(int i = 0; i < m_blk_num; ++i)
        //     if(HammingDistance(m_encoded_signal + (i * m_enc1.codeword_len), m_restore_codeword + (i * m_enc1.codeword_len), m_enc1.codeword_len))
        //         ++m_err_blk_cnt;
        m_ber = (double)m_err_bit_cnt / m_tot_bit;
        m_fer = (double)m_err_blk_cnt / m_blk_num;
        DisplayResult();
    }
}

void Simulator::Clean() {
    delete[] m_source_signal;
    delete[] m_encoded_signal;
    delete[] m_send_signal;
    delete[] m_receive_signal;
    delete[] m_restore_codeword;
    delete[] m_restore_signal;
    m_enc1.Clean();
    m_chn1.Clean();
    fclose(m_out_file);
}

void Simulator::DisplaySourceSignal() {
    fprintf(m_out_file, "The source signal is:\n");
    fprintf(m_out_file, "%d", m_source_signal[0]);
    for(int i = 1; i < m_bit_num; ++i)
        fprintf(m_out_file, " %d", m_source_signal[i]);
    fprintf(m_out_file, "\n\n");
}

void Simulator::DisplayEncodedSignal() {
    int idx;
    fprintf(m_out_file, "The encoded signal is:\n");
    for(int i = 0; i < m_blk_num; ++i) {
        idx = i * m_enc1.m_codeword_len;
        fprintf(m_out_file, "%d", m_encoded_signal[idx]);
        for(int j = 1; j < m_enc1.m_codeword_len; ++j)
            fprintf(m_out_file, " %d", m_encoded_signal[idx + j]);
        fprintf(m_out_file, "\n");
    }
    fprintf(m_out_file, "\n\n");
}

void Simulator::DisplaySendSignal() {
    int idx;
    fprintf(m_out_file, "The sent signal is:\n");
    for(int i = 0; i < m_blk_num; ++i) {
        idx = i * m_enc1.m_codeword_len;
        fprintf(m_out_file, "%d", m_send_signal[idx]);
        for(int j = 1; j < m_enc1.m_codeword_len; ++j)
            fprintf(m_out_file, " %d", m_send_signal[idx + j]);
        fprintf(m_out_file, "\n");
    }
    fprintf(m_out_file, "\n\n");
}

void Simulator::DisplayReceiveSignal() {
    int idx;
    fprintf(m_out_file, "The received signal is:\n");
    for(int i = 0; i < m_blk_num; ++i) {
        idx = i * m_enc1.m_codeword_len;
        fprintf(m_out_file, "%f", m_receive_signal[idx]);
        for(int j = 1; j < m_enc1.m_codeword_len; ++j)
            fprintf(m_out_file, " %f", m_receive_signal[idx + j]);
        fprintf(m_out_file, "\n");
    }
    fprintf(m_out_file, "\n\n");
}

void Simulator::DisplayRestoreCodeword() {
    int idx;
    fprintf(m_out_file, "The restored codeword is:\n");
    for(int i = 0; i < m_blk_num; ++i) {
        idx = i * m_enc1.m_codeword_len;
        fprintf(m_out_file, "%d", m_restore_codeword[idx]);
        for(int j = 1; j < m_enc1.m_codeword_len; ++j)
            fprintf(m_out_file, " %d", m_restore_codeword[idx + j]);
        fprintf(m_out_file, "\n");
    }
    fprintf(m_out_file, "\n\n");
}

void Simulator::DisplayRestoreSignal() {
    fprintf(m_out_file, "The restored signal is:\n");
    fprintf(m_out_file, "%d", m_restore_signal[0]);
    for(int i = 1; i < m_bit_num; ++i)
        fprintf(m_out_file, " %d", m_restore_signal[i]);
    fprintf(m_out_file, "\n\n");
}

void Simulator::DisplayResult() {
    /*
    DisplaySourceSignal();
    DisplayEncodedSignal();
    DisplaySendSignal();
    DisplayReceiveSignal();
    DisplayRestoreCodeword();
    DisplayRestoreSignal();
    */
    fprintf(m_out_file, "SNR=%f, the BER=%f, the FER=%f\n", m_snr, m_ber, m_fer);
}
