#include "CommonFunc.h"
#include "SourceCreate.h"
#include "PolarCodec.h"
#include "BPSK_Modulater.h"
#include "AWGN_Channel.h"

#ifndef _ITCG_Simulator_
	#define _ITCG_Simulator_

	class Simulator {
	public:
	    double m_min_snr;
	    double m_max_snr;
	    double m_snr_step;
	    double m_snr;


		int m_blk_num;
		int m_blk_len;
		int m_bit_num;
		int m_err_blk_cnt;
		int m_err_bit_cnt;
		int m_tot_blk;
		int m_tot_bit;
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
		void DisplayResult();
	};

#endif
