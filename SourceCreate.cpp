#include "SourceCreate.h"

SourceCreate::SourceCreate() {

}

SourceCreate::~SourceCreate() {

}

void SourceCreate::CreateBitStream(int *bit_stream, int len) {
	//long int seed = time(NULL);
	for(int i = 0; i < len; ++i)
		bit_stream[i] = (Uniform() < 0.5 ? 0 : 1);
		// bit_stream[i] = 1;
}

void SourceCreate::CreateIntStream(int *int_stream, int len, int max_int) {
	for(int i = 0; i < len; ++i)
		int_stream[i] = Uniform() * (max_int + 1);
}
