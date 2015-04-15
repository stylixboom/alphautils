/*
 * hdf5_io.h
 *
 *  Created on: October 24, 2013
 *      Author: Siriwat Kasamwattanarote
 */
#pragma once

#include "H5Cpp.h"      // HDF5 cpp

#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif

using namespace std;

namespace alphautils
{
    namespace hdf5io
    {
        // Header
        bool HDF_get_2Ddimension(const string& in, const string& dataset_name, size_t &row, size_t &col);

        // IO
        bool HDF_write_2DFLOAT(const string& out, const string& dataset_name, const float data[], const size_t row, const size_t col);
        bool HDF_write_append_2DFLOAT(const string& out, bool file_exits, const string& dataset_name, const float data[], const size_t row, const size_t col);
        bool HDF_update_row_2DFLOAT(const string& out, const string& dataset_name, const float data[], const size_t at_row, const size_t row_size, const size_t col_size);
        bool HDF_read_2DFLOAT(const string& in, const string& dataset_name, float *&data, size_t &row, size_t &col);
        bool HDF_read_row_2DFLOAT(const string& in, const string& dataset_name, float *&data, const size_t from_row, const size_t read_count);
    };
};
//;)
