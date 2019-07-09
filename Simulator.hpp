#include "CommonFunc.hpp"
#include "SourceCreate.hpp"
#include "PolarCodec.hpp"
#include "BPSK_Modulator.hpp"
#include "AWGN_Channel.hpp"
#include "Error_Counter.hpp"

#ifndef _ITCG_Simulator_
	#define _ITCG_Simulator_

	class Simulator {
	public:
	    double m_min_snr;
	    double m_max_snr;
	    double m_snr_step;
	    double m_snr;

	    int m_bit_num;
		int m_blk_len;
		int m_max_blk_num;
		int m_max_err_blk;
		double m_fer;
		double m_ber;

		int *m_source_signal;
		int *m_encoded_signal;
		int *m_send_signal;
		double *m_receive_signal;
		int *m_restore_codeword;
		int *m_restore_signal;
		FILE *m_out_file;
		CCodec m_enc1;
		Channel m_chn1;
		C_Error_Counter m_cnt1;

		Simulator();
		~Simulator();
		void Init(const char *config_file_path);
		void Start();
		void Clean();
		void DisplaySourceSignal();
		void DisplayEncodedSignal();
		void DisplaySendSignal();
		void DisplayReceiveSignal();
		void DisplayRestoreCodeword();
		void DisplayRestoreSignal();
		void DisplayResult(FILE *out = NULL);
	};

#endif
