#include "Error_Counter.hpp"

C_Error_Counter::C_Error_Counter() {}

C_Error_Counter::~C_Error_Counter() {}

void C_Error_Counter::Count(int *in1, int *in2, int len) {
	m_err_bit_tmp = HammingDistance(in1, in2, len);
	if(m_err_bit_tmp) {
		++m_err_blk_cnt;
		m_err_bit_cnt += m_err_bit_tmp;
	}
	++m_blk_cnt;
	m_bit_cnt += len;
}

void C_Error_Counter::Reset() {
	m_bit_cnt = 0;
	m_blk_cnt = 0;
	m_err_bit_cnt = 0;
	m_err_blk_cnt = 0;
}