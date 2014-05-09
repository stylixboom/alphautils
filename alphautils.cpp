/*
 * alphautils.cpp
 *
 *  Created on: October 15, 2012
 *      Author: Siriwat Kasamwattanarote
 */

#include "alphautils.h"

using namespace std;

namespace alphautils
{
    // Version
    string version()
    {
        return alphautils_AutoVersion::alphautils_FULLVERSION_STRING;
    }

    // Mathermatic
    unsigned long long gcd(unsigned long long x, unsigned long long y)
    {
        while (y != 0)
        {
            unsigned long long t = x % y;
            x = y;
            y = t;
        }
        return x;
    }

    unsigned long long nCr(unsigned long long n, unsigned long long r) // Overflow prevented combination
    // ref http://stackoverflow.com/questions/1838368/calculating-the-amount-of-combinations
    {
        unsigned long long ret = 1;
        for (unsigned long long d = 1; d <= r; ++d, --n)
        {
            unsigned long long g = gcd(ret, d);
            ret /= g;
            unsigned long long t = n / (d / g);
            if (ret > std::numeric_limits<unsigned long long>::max() / t)
            {
               cout << "Overflow occur in nCr! n:" << n << " r:" << r << endl;
               exit(0);
            }
            ret *= t;
        }
        return ret;
    }

    int sequence(int a1, int d, int n)
    // ref http://www.doesystem.com/abe0324453cc7626f2ae1c83481a9ce8/%E0%B8%AA%E0%B8%B9%E0%B8%95%E0%B8%A3-%E0%B8%A5%E0%B8%B3%E0%B8%94%E0%B8%B1%E0%B8%9A%E0%B9%81%E0%B8%A5%E0%B8%B0%E0%B8%AD%E0%B8%99%E0%B8%B8%E0%B8%81%E0%B8%A3%E0%B8%A1-%E0%B8%82%E0%B8%AD%E0%B8%87%E0%B9%80%E0%B8%A5%E0%B8%82%E0%B8%84%E0%B8%93%E0%B8%B4%E0%B8%95%E0%B8%81%E0%B8%B1%E0%B8%9A%E0%B9%80%E0%B8%A5%E0%B8%82%E0%B8%B2%E0%B8%84%E0%B8%93%E0%B8%B4%E0%B8%95-%E0%B9%81%E0%B8%A5%E0%B8%B0%E0%B8%95%E0%B8%B1%E0%B8%A7%E0%B8%AD%E0%B8%A2%E0%B9%88%E0%B8%B2%E0%B8%87%E0%B8%9B%E0%B8%A3%E0%B8%B0%E0%B8%81%E0%B8%AD%E0%B8%9A.htm
    {
        return a1 + (n - 1) * d;
    }

    void calc_stat3m(const float data[], size_t data_size, stat3m& data_stat)
    {
        if (data_size != 0)
        {
            float sum = 0.0f, min = 1000000.0f, max = 0.0f;
            for(size_t idx = 0; idx < data_size; idx++)
            {
                float curr_data = data[idx];
                if (min > curr_data)
                    min = curr_data;
                if (max < curr_data)
                    max = curr_data;
                sum += curr_data;
            }
            data_stat.min = min;                // min
            data_stat.max = max;                // max
            data_stat.mean = sum / data_size;   // mean
        }
    }

    float calc_mean(const float data[], size_t data_size)
    {
        if (data_size == 0)
            return 0.0f;
        float sum = 0;
        for (size_t idx = 0; idx < data_size; idx++)
           sum += data[idx];
        return sum / data_size;
    }

    // Standard Variation original version
    float calc_sd(float data[], size_t data_size)
    {
        if (data_size == 0)
            return 0.0f;
        float sum = 0;
        for (size_t idx = 0; idx < data_size; idx++)
           sum += data[idx];
        float mean = sum / data_size;
        float sq_diff_sum = 0;
        for (size_t idx = 0; idx < data_size; idx++)
        {
           float diff = data[idx] - mean;
           sq_diff_sum += diff * diff;
        }
        float variance = sq_diff_sum / data_size;
        return sqrt(variance);
    }

    // Fast but Inaccurate SD version
    // Ref http://www.strchr.com/standard_deviation_in_one_pass
    float calc_sd_fast(float data[], size_t data_size)
    {
        if (data_size == 0)
            return 0.0f;
        float sum = 0;
        float sq_sum = 0;
        for (size_t idx = 0; idx < data_size; idx++) {
           sum += data[idx];
           sq_sum += data[idx] * data[idx];
        }
        float mean = sum / data_size;
        float variance = sq_sum / data_size - mean * mean;
        return sqrt(variance);
    }

    // Standard Variation original version with precalculate mean
    float calc_sd_premean(const float data[], size_t data_size, float mean)
    {
        if (data_size == 0)
            return 0.0f;
        float sq_diff_sum = 0;
        for (size_t idx = 0; idx < data_size; idx++)
        {
           float diff = data[idx] - mean;
           sq_diff_sum += diff * diff;
        }
        float variance = sq_diff_sum / data_size;
        return sqrt(variance);
    }

    // Slope of linear regression
    float calc_slope(const float x_data[], const float y_data[], size_t data_size)
    {
        // Linear regression excel fomular: http://office.microsoft.com/en-001/excel-help/slope-HP005209264.aspx
        float x_bar = calc_mean(x_data, data_size);
        float y_bar = calc_mean(y_data, data_size);

        // slope = sum((x-x_bar)*(y-y_bar)) / sum((x-x_bar)^2)

        float sum1 = 0.0f, sum2 = 0.0f;
        float slope = 0.0f;

        for (size_t data_id = 0; data_id < data_size; data_id++)
        {
            float x_diff = x_data[data_id] - x_bar;
            float y_diff = y_data[data_id] - y_bar;
            sum1 += (x_diff * y_diff);
            sum2 += (x_diff * x_diff);
        }

        slope = sum1 / sum2;

        return slope;
    }

    // Timing
    double TimeElapse(timespec start)
    {
        // Ref http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
        // Get current time
        timespec curr;
        clock_gettime(CLOCK_MONOTONIC, &curr);

        timespec elapse;
        if ((curr.tv_nsec - start.tv_nsec) < 0)
        {
            elapse.tv_sec = curr.tv_sec - start.tv_sec - 1;
            elapse.tv_nsec = 1000000000 + curr.tv_nsec - start.tv_nsec;
        }
        else
        {
            elapse.tv_sec = curr.tv_sec - start.tv_sec;
            elapse.tv_nsec = curr.tv_nsec - start.tv_nsec;
        }

        // Return time in second precision
        return double(elapse.tv_sec) + elapse.tv_nsec / 1000000000.0;
    }

    timespec CurrentPreciseTime()
    {
        timespec curr;

        clock_gettime(CLOCK_MONOTONIC, &curr);

        return curr;
    }

    // String manipulation
    string toString(int val)
    {
        stringstream output;
        output << val;
        return output.str();
    }

    string toString(float val)
    {
        stringstream output;
        output << val;
        return output.str();
    }

    string toString(double val)
    {
        stringstream output;
        output << val;
        return output.str();
    }

    string toString(size_t val)
    {
        stringstream output;
        output << val;
        return output.str();
    }

    string ZeroPadNumber(int num, int length)
    {
        ostringstream ss;
        ss << setw( length ) << setfill( '0' ) << num;
        return ss.str();
    }

    string ZeroPadString(const string& txt, int length)
    {
        ostringstream ss;
        ss << setw( length ) << setfill( '0' ) << txt;
        return ss.str();
    }

    string CharPadString(const string& txt, const char& pad, int length)
    {
        ostringstream ss;
        ss << setw( length ) << setfill( pad ) << txt;
        return ss.str();
    }

    string str_replace_first(const string& str, const string& from, const string& to)
    {
        string ret = str;
        size_t start_pos = ret.find(from);
        if (start_pos == string::npos)
            return ret;
        ret.replace(start_pos, from.length(), to);
        return ret;
    }

    string str_replace_last(const string& str, const string& from, const string& to)
    {
        string ret = str;
        size_t start_pos = ret.find_last_of(from);
        if (start_pos == string::npos)
            return ret;
        ret.replace(start_pos, from.length(), to);
        return ret;
    }

    string str_replace_all(const string& str, const string& from, const string& to)
    {
        string ret = str;
        size_t start_pos = ret.find(from);
        while (start_pos != string::npos)
        {
            ret.replace(start_pos, from.length(), to);
            start_pos = ret.find(from);
        }
        return ret;
    }

    template<class T>
    void string_splitter(const string& s, const char* d, T& ret)
    {
        T output;

        bitset<255> delims;
        while( *d )
        {
            unsigned char code = *d++;
            delims[code] = true;
        }
        typedef string::const_iterator iter;
        iter beg;
        bool in_token = false;
        for( string::const_iterator it = s.begin(), end = s.end(); it != end; ++it )
        {
            if( delims[*it] )
            {
                if( in_token )
                {
                    output.push_back(typename T::value_type(beg, it));
                    in_token = false;
                }
            }
            else if( !in_token )
            {
                beg = it;
                in_token = true;
            }
        }
        if( in_token )
        output.push_back(typename T::value_type(beg, s.end()));
        output.swap(ret);

        output.clear();
    }

    string trim_string_until(const string& str, const char& character)
    {
        // cf_id:
        int startPos = 0, count = 0;
        for(size_t pos = 0; pos < str.length(); pos++)
        {
            if(str[pos] == character)
            {
                startPos = ((int)pos) + 1;
                count = str.length() - startPos;
                break;
            }
        }

        return str.substr(startPos, count);
    }

    bool str_contains(const string& text, const string& str)
    {
        return text.find(str) != string::npos;
    }

    // String, file/directory
    void ls2null(const string& path)
    {
        exec("ls -f " + path + COUT2NULL);
    }

    vector<string> directory_splitter(const string& path)
    {
        const char* delimsDir = "/";// Dir

        vector<string> ret;
        string_splitter(path, delimsDir, ret);

        return ret;
    }

    string get_directory(const string& path)
    {
        int endPos = 0;
        for(size_t pos = path.length() - 1; pos >= 0 ; pos--)
        {
            if(path[pos] == '/')
            {
                endPos = (int)pos;
                break;
            }
        }

        return path.substr(0, endPos);
    }

    string get_filename(const string& path)
    {
        int startPos = 0, count = 0;
        for(size_t pos = path.length() - 1; pos >= 0 ; pos--)
        {
            if(path[pos] == '/')
            {
                startPos = ((int)pos) + 1;
                count = path.length() - startPos;
                break;
            }
        }

        return path.substr(startPos, count);
    }

    bool is_path_exist(const string& path)
    {
        struct stat pathStat;
        return !stat(path.c_str(), &pathStat); // stat == 0 if exist, -1 otherwise
    }

    bool contain_path(const string& text)
    {
        if (text[0] == '.' || text[0] == '/')
            return true;
        else
            return false;
    }

    void make_dir_available(const string& path, const string& permission)
    {
        stringstream cmd;
        cmd << "mkdir -m " << permission << " -p " << path;
        exec(cmd.str());
    }

    bool lockfile(const string& path)
    {
        if (!is_path_exist(path + ".lock"))
        {
            ofstream lock_file ((path + ".lock").c_str(), ios::binary);
            if (lock_file.is_open())
                lock_file.close(); // zero-byte file
            return true; // can lock
        }
        return false; // cannot lock, file exist
    }

    bool unlockfile(const string& path)
    {
        if (is_path_exist(path + ".lock"))
        {
            remove((path + ".lock").c_str());
            return true; // can unlock
        }
        return false; // cannot unlock, file not exist
    }

    bool islock(const string& path)
    {
        return is_path_exist(path + ".lock");
    }

    // IO
	bool text_write(const string& out, const string& text, bool append)
	{
        ofstream OutFile;
        if (append) // Append
            OutFile.open(out.c_str(), ios::app);
        else        // Create new, overwrite
            OutFile.open(out.c_str());
        if (OutFile.is_open())
        {
            OutFile << text;
            OutFile.flush();
            // Close file
            OutFile.close();

            return true;
        }
        return false;
	}

	bool text_read(const string& in, string& read_text)
	{
        stringstream output;
        ifstream InFile (in.c_str());
        if (InFile)
        {
            string read_line;
            while (!InFile.eof())
            {
                getline(InFile, read_line);
                output << read_line;
            }

            // Close file
            InFile.close();

            read_text = output.str();

            return true;
        }
        return false;
	}

	vector<string> text_readline2vector(const string& in)
	{
        vector<string> output;
        ifstream InFile (in.c_str());
        if (InFile)
        {
            string read_line;
            while (!InFile.eof())
            {
                getline(InFile, read_line);
                if (read_line != "")
                    output.push_back(read_line);
            }

            // Close file
            InFile.close();
        }

        return output;
	}

	bool bin_write_vector_SIZET(const string& out, vector<size_t>& data)
	{
        size_t data_size;
        ofstream OutFile (out.c_str(), ios::binary);
        if (OutFile.is_open())
        {
            // Write data_size
            data_size = data.size();
            OutFile.write(reinterpret_cast<char*>(&data_size), sizeof(data_size));

            // Write data
            vector<size_t>::iterator data_it;
            for (data_it = data.begin(); data_it != data.end(); data_it++)
                OutFile.write(reinterpret_cast<char*>(&(*data_it)), sizeof(*data_it));

            // Close file
            OutFile.close();

            return true;
        }
        return false;
	}

	bool bin_read_vector_SIZET(const string& in, vector<size_t>& data)
	{
        size_t data_size;
        ifstream InFile (in.c_str(), ios::binary);
        if (InFile)
        {
            // Read data_size
            InFile.read((char*)(&data_size), sizeof(data_size));

            // Read data
            for (size_t data_idx = 0; data_idx < data_size; data_idx++)
            {
                size_t read_data;
                InFile.read((char*)(&read_data), sizeof(read_data));
                data.push_back(read_data);
            }

            // Close file
            InFile.close();

            return true;
        }
        return false;
	}

	bool bin_write_array_SIZET(const string& out, size_t *data, size_t row, size_t col)
	{
        ofstream OutFile (out.c_str(), ios::binary);
        if (OutFile.is_open())
        {
            // Write row
            OutFile.write(reinterpret_cast<char*>(&row), sizeof(row));

            // Write col
            OutFile.write(reinterpret_cast<char*>(&col), sizeof(col));

            // Write data
            OutFile.write(reinterpret_cast<char*>(data), row * col * sizeof(data[0]));

            // Close file
            OutFile.close();

            return true;
        }
        return false;
	}

	bool bin_read_array_SIZET(const string& in, size_t *&data, size_t &row, size_t &col)
	{
        ifstream InFile (in.c_str(), ios::binary);
        if (InFile)
        {
            // Read row
            InFile.read((char*)(&row), sizeof(row));

            // Read col
            InFile.read((char*)(&col), sizeof(col));

            // Read data
            InFile.read((char*)(data), row * col * sizeof(data[0]));

            // Close file
            InFile.close();

            return true;
        }
        return false;
	}

    // Execution
    string exec(string cmd)
    {
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) return "ERROR";
        char buffer[128];
        string result = "";
        while(!feof(pipe)) {
            if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
        pclose(pipe);
        return result;
    }

    string execp(string cmd)
    {
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) return "ERROR";
        char buffer[8];
        string result = "";
        while(!feof(pipe)) {
            if(fgets(buffer, 8, pipe) != NULL)
            {
                result += buffer;
                cout << buffer; cout.flush();
            }
        }
        pclose(pipe);
        return result;
    }

    // Percentage
    void percentout(int curr, int full, int sampling = 1)
    {
        if (curr % sampling == 0)
        {
            cout << setw(6) << setfill(' ') << setprecision(2) << fixed << 100.0 * curr / full << " %\b\b\b\b\b\b\b\b";
            cout.flush();
        }
    }

    // 2D
    void normalise2dpts(const float inPtsx[], const float inPtsy[], float normPtsx[], float normPtsy[], size_t amountPts)
    {
        float sumX = 0.0f, sumY = 0.0f;
        float cenX = 0.0f, cenY = 0.0f;
        float meanDist = 0.0f, sumDist = 0.0f;

        // Centroid
        size_t inPtsCount = amountPts;
        for(size_t index = 0; index < inPtsCount; index++)
        {
            float pX = inPtsx[index];
            float pY = inPtsy[index];
            sumX += pX;
            sumY += pY;
        }
        cenX = sumX / inPtsCount;
        cenY = sumY / inPtsCount;

        // Mean Distance
        float* diffX = new float[inPtsCount];
        float* diffY = new float[inPtsCount];
        for(size_t index = 0; index < inPtsCount; index++)
        {
            float diffXtmp = inPtsx[index] - cenX;
            float diffYtmp = inPtsy[index] - cenY;
            sumDist += sqrt(diffXtmp * diffXtmp + diffYtmp * diffYtmp);

            // Keep
            diffX[index] = diffXtmp;
            diffY[index] = diffYtmp;
        }
        meanDist = sumDist / inPtsCount;

        // Scale
        float scale = 1.414213562373 / meanDist;

        // Mean dist and Scale and Norm
        normPtsx = new float[inPtsCount];
        normPtsy = new float[inPtsCount];
        for(size_t index = 0; index < inPtsCount; index++)
        {
            // Norm
            normPtsx[index] = diffX[index] * scale;
            normPtsy[index] = diffY[index] * scale;
        }

        // Release memory
        delete[] diffX;
        delete[] diffY;
    }

    bool pnpoly(int nvert, const float vertx[], const float verty[], float testx, float testy)
    {
        // Ref
        // http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
        // http://stackoverflow.com/questions/217578/point-in-polygon-aka-hit-test
        // Jordan curve theorem
        bool c = false;
        int i, j = 0;
        for (i = 0, j = nvert-1; i < nvert; j = i++)
        {
            if (((verty[i] > testy) != (verty[j] > testy)) && (testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
                c = !c;
        }

        /*if (!c) // outside print
        {
            cout << "box" << endl;
            for (int idx=0; idx < nvert; idx++)
                cout << vertx[idx] << "," << verty[idx] << endl;
            cout << testx << "," << testy << " is " << c << endl;
        }*/

        return c;
    }

    // Matrix
    void create_symmat(float *&symmat, size_t width)
    {
        symmat = new float[get_symmat_size(width)];
    }

    void set_symmat_at(float symmat[], size_t row, size_t col, float value)
    {
        symmat[rc2half_idx(row, col)] = value;
    }

    float get_symmat_at(const float symmat[], size_t row, size_t col)
    {
        // Debug mode, no diagonal
        /*if (col == row)
        {
            cout << "Error: get symmat invalid at diagonal, [" << row << "," << col << "]" << endl;
            exit(-1);
        }*/

        // Redirect to only half matrix
        if (col > row)
        {                           // 0 - - -
            size_t tmp = col;       // a 0 - -
            col = row;              // b c 0 -
            row = tmp;              // d e f 0
        }

        return symmat[rc2half_idx(row, col)];
    }

    size_t get_symmat_size(size_t width)
    {
        size_t n = width;
        return n * (n - 1) / 2;
    }

    size_t rc2half_idx(size_t row, size_t col)
    {
        // row * (row - 1) / 2 is the diagonal index
        return row * (row - 1) / 2 + col;
    }

	xy_idx half2rc_idx(size_t idx)
	{
        xy_idx ret_rc;

        ret_rc.y = ceil((-1 + sqrt(8 * (idx+1) - 3)) / 2);  // Row
        ret_rc.x = idx - ret_rc.y * (ret_rc.y - 1) / 2;     // Col

        return ret_rc;
	}

	// Algorithm
    vector<size_t> get_sorted_indices(const vector<float> &data, int mode)
    {
        return get_sorted_indices(&data[0], data.size(), mode);
    }

    vector<size_t> get_sorted_indices(const float data[], size_t data_size, int mode)
    {
        // Create index with value
        vector< pair<size_t, float> > idx_data;
        for (size_t idx = 0; idx < data_size; idx++)
            idx_data.push_back( pair<size_t, float>(idx, data[idx]) );

        // sort indexes based on comparing values in data
        if (mode == SORT_DSC)
            sort(idx_data.begin(), idx_data.end(), compare_pair_second<>());
        else
            sort(idx_data.begin(), idx_data.end(), compare_pair_second<less>());

        // Keep idx
        vector<size_t> sorted_idx;
        for (size_t idx = 0; idx < data_size; idx++)
            sorted_idx.push_back(idx_data[idx].first);

        // Release memory
        idx_data.clear();

        return sorted_idx;
    }

	void data_to_range_histogram(const float data[], size_t data_size, HIST_CONF hist_config, size_t *&histogram, float *&hist_label, size_t& hist_size)
	{
        // Calculate hist_size
        float hist_low, hist_high, step_size = 1;

        if (hist_config.mode == HIST_RANGEMODE_AUTO_SD)
        {
            // Calc SD
            stat3m data_stat = {0.0f, 0.0f, 0.0f};
            calc_stat3m(data, data_size, data_stat);
            float sd = calc_sd_premean(data, data_size, data_stat.mean);

            // Find min max
            hist_low = data_stat.mean;
            while (hist_low > data_stat.min)    // find hist lower_bound
                hist_low -= step_size;
            hist_high = data_stat.mean;
            while (hist_high < data_stat.max)   // find hist upper_bound
                hist_high += step_size;

            // Calc step
            step_size = sd / hist_config.sd_step_ratio;
        }
        else
        {
            // Use predefined parameters
            hist_low = hist_config.hist_min;
            hist_high = hist_config.hist_max;
            step_size = hist_config.step_size;
        }
        // Calculate hist size
        hist_size = ceil((hist_high - hist_low) / step_size) + 1;

        cout << "hist_low: " << hist_low << endl;
        cout << "hist_high: " << hist_high << endl;
        cout << "step_size: " << step_size << endl;
        cout << "hist_size: " << hist_size << endl;

        // Calculate hist label
        hist_label = new float[hist_size];
        for (size_t hist_id = 0; hist_id < hist_size; hist_id++)
            hist_label[hist_id] = hist_low + (hist_id * step_size);

        // Histogram generation
        histogram = new size_t[hist_size];
        for (size_t hist_id = 0; hist_id < hist_size; hist_id++)
            histogram[hist_id] = 0; // Initialize histogram
        for (size_t hist_id = 1; hist_id < hist_size; hist_id++)   // skip first lower_bound, it's always zero, not necessary to be count
        {
            size_t data_pass_count = 0;
            for (size_t data_idx = 0; data_idx < data_size; data_idx++)
            {
                if (data[data_idx] <= hist_label[hist_id] && data[data_idx] > hist_label[hist_id - 1])
                    data_pass_count++;  // count all this bin and previous bin
            }
            // Frequency of current bin is whis bin without previous bin
            histogram[hist_id] = data_pass_count;
        }
	}

	float otsu_threshold_from_data(const float data[], size_t data_size, float sd_step_ratio)
	{
        // Convert data to histogram
        float ret_threshold;
        size_t* histogram;
        float* hist_label;
        size_t hist_size;
        HIST_CONF hist_config = {HIST_RANGEMODE_AUTO_SD, sd_step_ratio, 0, 0, 0};
        data_to_range_histogram(data, data_size, hist_config, histogram, hist_label, hist_size);

        /*
        cout << "Histogram size: " << hist_size << endl;
        cout << "Histogram: ";
        for (size_t hist_idx = 0; hist_idx < hist_size; hist_idx++)
            cout << histogram[hist_idx] << " ";
        cout << endl;
        cout << "Histogram label: ";
        for (size_t hist_idx = 0; hist_idx < hist_size; hist_idx++)
            cout << hist_label[hist_idx] << " ";
        cout << endl;
        */

        // Calculate Otsu threshold
        size_t threshold = otsu_threshold_from_hist(histogram, hist_size, data_size);

        // Save threshold
        ret_threshold = hist_label[threshold];

        // Release memory
        delete[] histogram;
        delete[] hist_label;

        return ret_threshold;
	}

	// Finding threshold for bi-modal histogram
	size_t otsu_threshold_from_hist(const size_t histogram[], size_t hist_size, size_t data_size)
	{
        // Ref otsu http://www.dandiggins.co.uk/arlib-9.html
        // Ref2 another code http://cis.k.hosei.ac.jp/~wakahara/otsu_th.c
        float prob[hist_size], omega[hist_size]; /* prob of graylevels */
        float myu[hist_size];   /* mean value for separation */
        float max_sigma, sigma[hist_size]; /* inter-class variance */
        size_t threshold; /* threshold for binarization */


        /* calculation of probability density */
        for (size_t idx = 0; idx < hist_size; idx++)
            prob[idx] = histogram[idx] / float(data_size);

        /* omega & myu generation */
        omega[0] = prob[0];
        myu[0] = 0.0f;       /* 0.0 times prob[0] equals zero */
        for (size_t idx = 1; idx < hist_size; idx++)
        {
            omega[idx] = omega[idx - 1] + prob[idx];
            myu[idx] = myu[idx - 1] + idx * prob[idx];
        }

        /* sigma maximization
        sigma stands for inter-class variance
        and determines optimal threshold value */
        threshold = 0;
        max_sigma = 0.0f;
        for (size_t idx = 0; idx < hist_size - 1; idx++)
        {
            if (omega[idx] != 0.0f && omega[idx] != 1.0f)
            {
                float va = myu[hist_size - 1] * omega[idx] - myu[idx];
                sigma[idx] = va * va / (omega[idx] * (1.0f - omega[idx]));
            }
            else
                sigma[idx] = 0.0f;
            if (max_sigma < sigma[idx])
            {
                max_sigma = sigma[idx];
                threshold = idx;
            }
        }

        return threshold;
    }
};
//;)
