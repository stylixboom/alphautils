/*
 * alphautils.h
 *
 *  Created on: October 15, 2012
 *      Author: Siriwat Kasamwattanarote
 */
#pragma once
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <algorithm>    // sort
#include <functional>   // bind
#include <vector>       // setw
#include <iomanip>
#include <sys/time.h>   // time
#include <sys/stat.h>   // file-directory existing
#include <sys/types.h>  // file-directory
#include <dirent.h>     // file-directory
#include <cmath>        // Math
#include <limits>       // limit (for max math limit and etc)
#include "version.h"

using namespace std;


namespace alphautils
{
    // Version
    string version();

    // Data structure
    typedef struct _xy_idx{ size_t x; size_t y; } xy_idx;
    typedef struct _stat3m{ float min; float max; float mean; } stat3m;

    // Comparision
    const int SORT_ASC = 0;
    const int SORT_DSC = 1;
    template<template <typename> class P = std::greater >
    struct compare_pair_second {
        template<class T1, class T2> bool operator()(const pair<T1, T2>& left, const pair<T1, T2>& right) {
            return P<T2>()(left.second, right.second);
        }
    };

    // Mathermatic
    const double PI = 3.1415926535897932384626433832795;
    unsigned long long gcd(unsigned long long x, unsigned long long y);
    unsigned long long nCr(unsigned long long n, unsigned long long k);
    int sequence(int a1, int d, int n);
    void calc_stat3m(const float data[], size_t data_size, stat3m& data_stat);
    float calc_mean(const float data[], size_t data_size);
    float calc_sd(const float data[], size_t data_size);
    float calc_sd_fast(const float data[], size_t data_size);
    float calc_sd_premean(const float data[], size_t data_size, float mean);
    float calc_slope(const float x_data[], const float y_data[], size_t data_size); // Linear regression slope

    // Terminal Style
    // Ref http://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
    const string redc =     "\x1b[31;1m";
    const string greenc =   "\x1b[32;1m";
    const string yellowc =  "\x1b[33;1m";
    const string bluec =    "\x1b[34;1m";
    const string magentac = "\x1b[35;1m";
    const string cyanc =    "\x1b[36;1m";
    const string endc =     "\x1b[0m";

    // String manipulation
    string toString(int val);   // Better to have overloaded function
    string toString(float val); // Because it's easier to use in this case
    string toString(double val);// no need to tell its type when calling
    string toString(size_t val);// toString<int>(), toString<float>(), ...
    string ZeroPadNumber(int num, int length);
    string ZeroPadString(const string& txt, int length);
    string CharPadString(const string& txt, const char& pad, int length);
	string str_replace_first(const string& str, const string& from, const string& to);
	string str_replace_last(const string& str, const string& from, const string& to);
	string str_replace_all(const string& str, const string& from, const string& to);
	template<class T> void string_splitter(const string& s, const char* d, T& ret);
	string trim_string_until(const string& str, const char& character);
	bool str_contains(const string& text, const string& str);

	// String file/directory
	void ls2null(const string& path);
	vector<string> directory_splitter(const string& path);
	string get_directory(const string& path);
	string get_filename(const string& path);
	bool is_path_exist(const string& path);
	bool contain_path(const string& text);
	void make_dir_available(const string& path, const string& permission = "755");
	bool lockfile(const string& path);
	bool unlockfile(const string& path);
	bool islock(const string& path);

	// IO
	bool text_write(const string& out, const string& text, bool append);
	string text_read(const string& in);
	vector<string> text_readline2vector(const string& in);
	bool bin_write_vector_SIZET(const string& out, vector<size_t>& data, bool append = false); //don't use const T& data, explained here https://www.securecoding.cert.org/confluence/display/seccode/EXP05-C.+Do+not+cast+away+a+const+qualification
	bool bin_read_vector_SIZET(const string& in, vector<size_t>& data);
	bool bin_write_array_SIZET(const string& out, size_t *data, size_t row, size_t col, bool append_row = false);
	bool bin_read_array_SIZET(const string& in, size_t *&data, size_t &row, size_t &col);

	// Execution
	const string COUT2NULL = " > /dev/null 2>&1";
	string exec(string cmd);
    string execp(string cmd);

    // Timing
    string SecondToDayHourMinSec(double second);
    double TimeLeft(double timeuse, size_t curr, size_t start, size_t full);
    double TimeElapse(const timespec& start);
    timespec CurrentPreciseTime();

	// Percentage
	void percentout(size_t curr, size_t full, int sampling = 1);
	void percentout_timeleft(size_t curr, size_t start, size_t full, const timespec& timestart, int sampling = 1);

	// 2D
	void normalise2dpts(const float inPtsx[], const float inPtsy[], float normPtsx[], float normPtsy[], size_t amountPts);
	bool pnpoly(int nvert, const float vertx[], const float verty[], float testx, float testy);
	//bool pnpoly(int nvert, float *vertx, float *verty, float testx, float testy); work!!
	void distance_matrix();

	// Matrix
	// Symmat-Matrix
	void create_symmat(float* &symmat, size_t width);
	void set_symmat_at(float* symmat, size_t row, size_t col, float value);
	float get_symmat_at(const float* symmat, size_t row, size_t col);
	size_t get_symmat_size(size_t width);
	size_t rc2half_idx(size_t row, size_t col);
	xy_idx half2rc_idx(size_t idx);

	// Histogram
	const int HIST_RANGEMODE_AUTO_SD = 0;
	const int HIST_RANGEMODE_MANUAL = 1;
	typedef int HIST_RANGEMODE;
	typedef struct _HIST_CONF{ HIST_RANGEMODE mode; float sd_step_ratio; float hist_min; float hist_max; size_t step_size; } HIST_CONF;
	void data_to_range_histogram(const float data[], size_t data_size, HIST_CONF hist_config, size_t *&histogram, float *&hist_label, size_t& hist_size);

	// Algorithm
	vector<size_t> get_sorted_indices(const vector<float> &data, int mode);
	vector<size_t> get_sorted_indices(const float data[], size_t data_size, int mode);
	float otsu_threshold_from_data(const float data[], size_t data_size, float sd_step_ratio);
	size_t otsu_threshold_from_hist(const size_t histogram[], size_t hist_size, size_t data_size);
};
//;)
