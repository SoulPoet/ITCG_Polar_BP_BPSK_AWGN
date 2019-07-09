#include "CommonFunc.hpp"

#ifndef _ITCG_Error_Counter_
    #define _ITCG_Error_Counter_

    class C_Error_Counter {
    public:
    	int m_blk_cnt;
    	int m_err_blk_cnt;
    	int m_bit_cnt;
    	int m_err_bit_cnt;
    	int m_err_bit_tmp;

        C_Error_Counter();
        ~C_Error_Counter();
        void Count(int *in1, int *in2, int len);
        void Reset();
    };

#endif
