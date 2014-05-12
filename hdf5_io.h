/*
 * hdf5_io.h
 *
 *  Created on: October 24, 2013
 *      Author: Siriwat Kasamwattanarote
 */
#pragma once
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>       // setw
#include <iomanip>
#include <sys/time.h>   // time
#include <sys/stat.h>   // file-directory existing
#include <sys/types.h>  // file-directory
#include <dirent.h>     // file-directory
#include <math.h>       // Math
#include <limits>       // limit (for max math limit and etc)
#include "H5Cpp.h"      // HDF5 cpp

using namespace std;

#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif

namespace alphautils
{
    namespace hdf5io
    {
        // Header
        bool HDF_get_2Ddimension(const string& in, const string& dataset_name, size_t &row, size_t &col);

        // IO
        bool HDF_write_2DFLOAT(const string& out, const string& dataset_name, const float data[], const size_t row, const size_t col);
        bool HDF_write_append_2DFLOAT(const string& out, bool file_exits, const string& dataset_name, const float data[], const size_t row, const size_t col);
        bool HDF_read_2DFLOAT(const string& in, const string& dataset_name, float *&data, size_t &row, size_t &col);
        bool HDF_read_row_2DFLOAT(const string& in, const string& dataset_name, float *&data, const size_t from_row, const size_t read_count);
    };
};
//;)
