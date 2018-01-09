#include <cstdio>
#include <cmath>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <complex>
#include <vector>
#include <queue>
#include <map>
#include <stack>
#include <set>
using namespace std;

#ifndef _ITCG_COMMON_FUNCTION_

	#define _ITCG_COMMON_FUNCTION_

	const double PI = acos(-1.0);
	const double EPS = 1e-8;

	void SetSeed(int flag);
	void PrintState(FILE *fp);
	double Uniform(double a, double b, long int &seed);
	double Uniform();
	double Gauss(double mean, double segma, long int &seed);
	double Normal(double mean, double segma);
	void Normal(double *arr, int len);
	int HammingDistance(int *arr1, int *arr2, int len);
	int SGN(const double x);
	FILE* OpenFile(const char *file_path, const char *mode);
	double CntSNR(double sigal_power, double noise_power);
	void Debug(char *var_name, int val);
	void Debug(char *var_name, double val);
	void Debug(char *var_name, char *str);

#endif
