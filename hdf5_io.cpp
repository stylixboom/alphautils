/*
 * hdf5_io.cpp
 *
 *  Created on: October 24, 2013
 *      Author: Siriwat Kasamwattanarote
 */

#include "hdf5_io.h"

using namespace std;

#ifndef H5_NO_NAMESPACE
using namespace H5;
#endif

namespace alphautils
{
    namespace hdf5io
    {
        bool HDF_get_2Ddimension(const string& in, const string& dataset_name, size_t &row, size_t &col)
        {
            H5std_string FILE_NAME( in );
            H5std_string DATASET_NAME( dataset_name );

            try
            {
                Exception::dontPrint();

                H5File file( FILE_NAME, H5F_ACC_RDONLY );
                DataSet dataset = file.openDataSet( DATASET_NAME );

                DataSpace dataspace = dataset.getSpace();

                hsize_t dims_out[2];
                dataspace.getSimpleExtentDims( dims_out, NULL);

                row = dims_out[0];
                col = dims_out[1];

                return true;
            }  // end of try block

            // catch failure caused by the H5File operations
            catch( FileIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSet operations
            catch( DataSetIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSpace operations
            catch( DataSpaceIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSpace operations
            catch( DataTypeIException error )
            {
                error.printError();
                return false;
            }

            return true;  // successfully terminated
        }

        bool HDF_write_2DFLOAT(const string& out, const string& dataset_name, const float data[], const size_t row, const size_t col)
        {                                                                               //*data prev work
            H5std_string FILE_NAME( out );
            H5std_string DATASET_NAME( dataset_name );
            size_t amount = row;
            size_t dim = col;       // dataset dimensions
            int RANK = 2;

            // Try block to detect exceptions raised by any of the calls inside it
            try
            {
                /*
                * Turn off the auto-printing when failure occurs so that we can
                * handle the errors appropriately
                */
                Exception::dontPrint();

                /*
                * Create a new file using H5F_ACC_TRUNC access,
                * default file creation properties, and default file
                * access properties.
                */
                H5File file( FILE_NAME, H5F_ACC_TRUNC );

                /*
                * Define the size of the array and create the data space for fixed
                * size dataset.
                */
                hsize_t dimsf[2];              // dataset dimensions
                dimsf[0] = amount;
                dimsf[1] = dim;
                DataSpace dataspace( RANK, dimsf );

                /*
                * Define datatype for the data in the file.
                * We will store little endian FLOAT numbers.
                */
                FloatType datatype( PredType::NATIVE_FLOAT );
                datatype.setOrder( H5T_ORDER_LE );

                /*
                * Create a new dataset within the file using defined dataspace and
                * datatype and default dataset creation properties.
                */
                DataSet dataset = file.createDataSet( DATASET_NAME, datatype, dataspace );

                /*
                * Write the data to the dataset using default memory space, file
                * space, and transfer properties.
                */
                dataset.write( data, PredType::NATIVE_FLOAT );
            }  // end of try block

            // catch failure caused by the H5File operations
            catch( FileIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSet operations
            catch( DataSetIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSpace operations
            catch( DataSpaceIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSpace operations
            catch( DataTypeIException error )
            {
                error.printError();
                return false;
            }

            return true;  // successfully terminated
        }

        bool HDF_write_append_2DFLOAT(const string& out, bool file_exits, const string& dataset_name, const float data[], const size_t row, const size_t col)
        {
            H5std_string FILE_NAME( out );
            H5std_string DATASET_NAME( dataset_name );
            hsize_t dims[2] = {row, col};              // dataset dimensions at creation
            hsize_t maxdims[2] = {H5F_UNLIMITED, col};
            size_t buffer = 512;
            hsize_t chunk_dims[2] ={buffer, col};
            int RANK = 2;

            // Try block to detect exceptions raised by any of the calls inside it
            try
            {
                /*
                * Turn off the auto-printing when failure occurs so that we can
                * handle the errors appropriately
                */
                Exception::dontPrint();

                H5File *file;
                DataSet *dataset;
                DataSpace *dataspace;

                if (file_exits) /// File exist, open, extend
                {
                    // Create a new file using H5F_ACC_RDWR access
                    file = new H5File( FILE_NAME, H5F_ACC_RDWR );

                    // Open existing dataset, then get all previous properties
                    dataset = new DataSet( file->openDataSet( DATASET_NAME ) );

                    // Get existing space info
                    hsize_t exist_dims[2];
                    dataspace = new DataSpace( dataset->getSpace() );
                    dataspace->getSimpleExtentDims( exist_dims, NULL);
                    // After read dim, clear this dataspace.
                    // and it will be replaced by an extended dataspace
                    delete dataspace;

                    // Resize dims
                    hsize_t new_dims[2] = { dims[0] + exist_dims[0], dims[1] };

                    // Extend dataset
                    dataset->extend( new_dims );

                    // Get existing space including extended
                    dataspace = new DataSpace( dataset->getSpace() );

                    // Select a hyperslab in extended portion of the dataset.
                    hsize_t offset[2] = { exist_dims[0], 0 };
                    dataspace->selectHyperslab( H5S_SELECT_SET, dims, offset );

                    // Define memory space.
                    DataSpace *memspace = new DataSpace( 2, dims, NULL );

                    // Write data to the extended portion of the dataset.
                    dataset->write( data, PredType::NATIVE_FLOAT, *memspace, *dataspace );

                    // Release memory
                    delete memspace;
                }
                else            /// File not exist, create with unlimited dim, write
                {
                    /*
                    * Create a new file using H5F_ACC_TRUNC access,
                    * default file creation properties, and default file
                    * access properties.
                    */
                    file = new H5File( FILE_NAME, H5F_ACC_TRUNC );

                    /*
                    * Define the size of the array and create the data space for fixed
                    * size dataset.
                    */
                    dataspace = new DataSpace( RANK, dims, maxdims );

                    /*
                    * Modify dataset creation property to enable chunking (important for being extend)
                    */
                    DSetCreatPropList *prop = new  DSetCreatPropList;
                    prop->setChunk( 2, chunk_dims );

                    /*
                    * Define datatype for the data in the file.
                    * We will store little endian FLOAT numbers.
                    */
                    FloatType datatype( PredType::NATIVE_FLOAT );
                    datatype.setOrder( H5T_ORDER_LE );

                    /*
                    * Create a new dataset within the file using defined dataspace and
                    * datatype and dataset creation properties.
                    */
                    dataset = new DataSet( file->createDataSet( DATASET_NAME, datatype, *dataspace, *prop ) );

                    /*
                    * Write the data to the dataset using default memory space, file
                    * space, and transfer properties.
                    */
                    dataset->write( data, PredType::NATIVE_FLOAT );

                    // Release memory
                    delete prop;
                }

                // Release memory
                delete dataspace;
                delete dataset;
                delete file;

            }  // end of try block

            // catch failure caused by the H5File operations
            catch( FileIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSet operations
            catch( DataSetIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSpace operations
            catch( DataSpaceIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSpace operations
            catch( DataTypeIException error )
            {
                error.printError();
                return false;
            }

            return true;  // successfully terminated
        }

        bool HDF_read_2DFLOAT(const string& in, const string& dataset_name, float *&data, size_t &row, size_t &col)
        {                                                                       //*&data prev work
            H5std_string FILE_NAME( in );
            H5std_string DATASET_NAME( dataset_name );

            /*
            * Try block to detect exceptions raised by any of the calls inside it
            */
            try
            {
                /*
                * Turn off the auto-printing when failure occurs so that we can
                * handle the errors appropriately
                */
                Exception::dontPrint();

                /*
                * Open the specified file and the specified dataset in the file.
                */
                H5File file( FILE_NAME, H5F_ACC_RDONLY );
                DataSet dataset = file.openDataSet( DATASET_NAME );

                /*
                * Get the class of the datatype that is used by the dataset.
                */
                //H5T_class_t type_class = dataset.getTypeClass();

                /*
                * Get class of datatype and print message if it's an integer.
                */

                //if( type_class == H5T_FLOAT )
                //{
                    //cout << "Data set has FLOAT type" << endl;

                    /*
                    * Get the float datatype
                    */
                    //FloatType intype = dataset.getFloatType();

                    /*
                    * Get order of datatype and print message if it's a little endian.
                    */
                    //H5std_string order_string;
                    //H5T_order_t order = intype.getOrder( order_string );
                    //cout << order_string << endl;

                    /*
                    * Get size of the data element stored in file and print it.
                    */
                    //size_t size = intype.getSize();
                    //cout << "Data size is " << size << endl;
                //}

                /*
                * Get dataspace of the dataset.
                */
                DataSpace dataspace = dataset.getSpace();

                /*
                * Get the number of dimensions in the dataspace.
                */
                //int rank = dataspace.getSimpleExtentNdims();


                /*
                * Get the dimension size of each dimension in the dataspace and
                * display them.
                */
                hsize_t dims_out[2];
                //int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);
                dataspace.getSimpleExtentDims( dims_out, NULL);
                //cout << "rank " << rank <<
                //", amount " << (unsigned long)(dims_out[0]) <<
                //" dimension " << (unsigned long)(dims_out[1]) << endl;

                row = dims_out[0];
                col = dims_out[1];
                size_t sub_row = row;
                size_t sub_col = col; // hyperslab dimensions
                size_t amount = row;
                size_t dim = col;    // output buffer dimensions
                int RANK_OUT = 2;

                /*
                * Output buffer initialization.
                */
                cout << "allocating [" << amount << ", " << dim << "] "; cout.flush();
                //size_t dataset_idx, dim_idx;
                data = new float[amount * dim];
                /*for (dataset_idx = 0; dataset_idx < amount; dataset_idx++)
                {
                    for (dim_idx = 0; dim_idx < dim; dim_idx++)
                    {
                        data[dataset_idx * dim + dim_idx] = 0.0f;
                    }
                }*/
                cout << "allocated "; cout.flush();


                /*
                * Define hyperslab in the dataset; implicitly giving strike and
                * block NULL.
                */
                hsize_t offset[2];	// hyperslab offset in the file
                hsize_t count[2];	// size of the hyperslab in the file
                offset[0] = 0;      // set offset here to crop
                offset[1] = 0;
                count[0]  = sub_row;
                count[1]  = sub_col;
                dataspace.selectHyperslab( H5S_SELECT_SET, count, offset );

                /*
                * Define the memory dataspace.
                */
                hsize_t dimsm[2];              /* memory space dimensions */
                dimsm[0] = amount;
                dimsm[1] = dim;
                DataSpace memspace( RANK_OUT, dimsm );

                /*
                * Define memory hyperslab.
                */
                hsize_t offset_out[2];	// hyperslab offset in memory
                hsize_t count_out[2];	// size of the hyperslab in memory
                offset_out[0] = 0;      // set offset here to place
                offset_out[1] = 0;
                count_out[0]  = sub_row;
                count_out[1]  = sub_col;
                memspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );


                /*
                * Read data from hyperslab in the file into the hyperslab in
                * memory and display the data.
                */
                dataset.read( data, PredType::NATIVE_FLOAT, memspace, dataspace );

                /* Debug
                for (dataset_idx = 0; dataset_idx < amount; dataset_idx++)
                {
                    for (dim_idx = 0; dim_idx < dim; dim_idx++)
                    cout << data[dataset_idx * dim + dim_idx] << " ";
                    cout << endl;
                }
                */

                return true;
            }  // end of try block

            // catch failure caused by the H5File operations
            catch( FileIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSet operations
            catch( DataSetIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSpace operations
            catch( DataSpaceIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSpace operations
            catch( DataTypeIException error )
            {
                error.printError();
                return false;
            }

            return true;  // successfully terminated
        }

        bool HDF_read_row_2DFLOAT(const string& in, const string& dataset_name, float *&data, const size_t from_row, const size_t read_count)
        {                                                                       //*&data prev work
            H5std_string FILE_NAME( in );
            H5std_string DATASET_NAME( dataset_name );

            /*
            * Try block to detect exceptions raised by any of the calls inside it
            */
            try
            {
                /*
                * Turn off the auto-printing when failure occurs so that we can
                * handle the errors appropriately
                */
                Exception::dontPrint();

                /*
                * Open the specified file and the specified dataset in the file.
                */
                H5File file( FILE_NAME, H5F_ACC_RDONLY );
                DataSet dataset = file.openDataSet( DATASET_NAME );

                /*
                * Get dataspace of the dataset.
                */
                DataSpace dataspace = dataset.getSpace();

                /*
                * Get the dimension size of each dimension in the dataspace and
                * display them.
                */
                hsize_t dims_out[2];
                //int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);
                dataspace.getSimpleExtentDims( dims_out, NULL);

                size_t sub_row = read_count;
                size_t sub_col = dims_out[1]; // hyperslab dimensions
                size_t amount = read_count;
                size_t dim = dims_out[1];    // output buffer dimensions
                int RANK_OUT = 2;

                /*
                * Output buffer initialization.
                */
                //cout << "allocating [" << amount << ", " << dim << "] "; cout.flush();
                //size_t dataset_idx, dim_idx;
                data = new float[amount * dim];
                /*for (dataset_idx = 0; dataset_idx < amount; dataset_idx++)
                {
                    for (dim_idx = 0; dim_idx < dim; dim_idx++)
                    {
                        data[dataset_idx * dim + dim_idx] = 0.0f;
                    }
                }*/
                //cout << "allocated "; cout.flush();


                /*
                * Define hyperslab in the dataset; implicitly giving strike and
                * block NULL.
                */
                hsize_t offset[2];	// hyperslab offset in the file
                hsize_t count[2];	// size of the hyperslab in the file
                offset[0] = from_row;   // set offset here to crop
                offset[1] = 0;
                count[0]  = sub_row;
                count[1]  = sub_col;
                dataspace.selectHyperslab( H5S_SELECT_SET, count, offset );

                /*
                * Define the memory dataspace.
                */
                hsize_t dimsm[2];              /* memory space dimensions */
                dimsm[0] = amount;
                dimsm[1] = dim;
                DataSpace memspace( RANK_OUT, dimsm );

                /*
                * Define memory hyperslab.
                */
                hsize_t offset_out[2];	// hyperslab offset in memory
                hsize_t count_out[2];	// size of the hyperslab in memory
                offset_out[0] = 0;      // set offset here to place
                offset_out[1] = 0;
                count_out[0]  = sub_row;
                count_out[1]  = sub_col;
                memspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );


                /*
                * Read data from hyperslab in the file into the hyperslab in
                * memory and display the data.
                */
                dataset.read( data, PredType::NATIVE_FLOAT, memspace, dataspace );

                /* Debug
                for (dataset_idx = 0; dataset_idx < amount; dataset_idx++)
                {
                    for (dim_idx = 0; dim_idx < dim; dim_idx++)
                    cout << data[dataset_idx * dim + dim_idx] << " ";
                    cout << endl;
                }
                */

                return true;
            }  // end of try block

            // catch failure caused by the H5File operations
            catch( FileIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSet operations
            catch( DataSetIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSpace operations
            catch( DataSpaceIException error )
            {
                error.printError();
                return false;
            }

            // catch failure caused by the DataSpace operations
            catch( DataTypeIException error )
            {
                error.printError();
                return false;
            }

            return true;  // successfully terminated
        }
    };
};
//;)
