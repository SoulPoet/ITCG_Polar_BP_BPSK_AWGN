#include "CommonFunc.hpp"

#ifndef _ITCG_POLAR_CODEC_
	#define _ITCG_POLAR_CODEC_

	const double LLR_INF = 1e20;

	class BitArr {
	public:
		int index;
		double metric;

		friend bool operator < (const BitArr &x, const BitArr &y){
			return x.metric > y.metric;
		}
	};

	class CCodec{
	public:

		double **m_LLR;
		double **m_Info_eti;
		int **m_Layer_info;
		bool *m_isFrozen;
		int m_codeword_len;
		int m_message_blk_len;
		int m_Layer_num;
		int m_max_iter_num;
		double m_design_SNR;

		double degen_fai(double y);
		double generate_fai(double x);

		inline void LLR_Clip(double &d);
		inline void Gaussian_Approximation();
		CCodec();
		~CCodec();
		void Init(const char *config_file_path);
		void Hadamard_Encoder(int *in, int *out, int blk);
		inline void Parallel_BP_Update_LLR();
		inline void Parallel_BP_Update_InfoMap();
		inline void Parallel_BP_Proc();
		void Parallel_BP_Decoder_4_BPSK_AWGN(double *in, int *out, double segma);
		void Clean();
		int CalcBlkNum(int bit_num);
		inline double Tian(const double a, const double b);
	};

#endif
