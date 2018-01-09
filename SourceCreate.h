#include "CommonFunc.h"

#ifndef _ITCG_SOURCE_CREATE_
	#define _ITCG_SOURCE_CREATE_

	class SourceCreate {
	public:
		SourceCreate();
		~SourceCreate();
		static void CreateBitStream(int *bit_stream, int len);
		static void CreateIntStream(int *int_stream, int len, int max_int);
	};

#endif
