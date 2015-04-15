/*
 * qb.cpp
 *
 *  Created on: July 30, 2014
 *              Siriwat Kasamwattanarote
 */
#include <ctime>
#include <bitset>
#include <vector>
#include <deque>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sys/time.h>
#include <sstream>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <algorithm>    // sort
#include <cstdlib>      // exit

// Siriwat's header
#include "../alphautils/alphautils.h"

#include "report.h"

#include "version.h"

using namespace std;
using namespace alphautils;

namespace alphautils
{

report::report()
{
}

report::~report(void)
{
    reset();
}

void report::init(const string& path, const string& name)
{
    report_path = path;
    report_name = name;

    // Reset description
    vector<string>().swap(report_description);
}

void report::add_description(const string& value)
{
    report_description.push_back(value);
}

bool report::new_data(const string& data_name, const string& group_name)
{
    if (data_group_table.find(data_name) == data_group_table.end())
    {
        // Update data_group_table
        data_group_table[data_name] = group_name;
        // Add data
        data[data_name] = key_object();
        last_data_name = data_name;
        return true;
    }
    else
        return false;
}

bool report::add_group_to_data(const string& data_name, const string& group_name)
{
    if (data_group_table.find(data_name) != data_group_table.end())
    {
        // Update data_group_table
        data_group_table[data_name] = group_name;
        return true;
    }
    else
        return false;
}

// add key at last data
bool report::add_key(const string& key, const string& value)
{
    return add_key_at(key, value, last_data_name);
}

// add key at specific data name
bool report::add_key_at(const string& key, const string& value, const string& data_name)
{
    // Update key_table
    if (key_table.find(key) == key_table.end())
    {
        key_table.insert(key);
        key_order.push_back(key);
    }
    // Add key to data
    unordered_map<string, string>& data_key = data[data_name].key;
    if (data_key.find(key) == data_key.end())
    {
        data_key[key] = value;
        return true;
    }
    else
        return false;
}

bool report::save_report()
{
    ofstream OutFile((report_path + "/" + report_name + ".csv").c_str());
    if (OutFile.is_open())
    {
        stringstream report_text;
        /// Write report description
        for (size_t desc_idx = 0; desc_idx < report_description.size(); desc_idx++)
            report_text << report_description[desc_idx] << endl;
        /// Write key header
        report_text << "group_name,data_name";
        for (size_t key_idx = 0; key_idx < key_order.size(); key_idx++)
            report_text << "," << key_order[key_idx];
        report_text << endl;
        /// Traverse data
        for (auto data_group_table_it = data_group_table.begin(); data_group_table_it != data_group_table.end(); data_group_table_it++)
        {
            // group name and data name
            report_text << data_group_table_it->second << "," << data_group_table_it->first;
            // data_key
            unordered_map<string, string>& data_key = data[data_group_table_it->first].key;
            for (size_t key_idx = 0; key_idx < key_order.size(); key_idx++)
            {
                if (data_key.find(key_order[key_idx]) != data_key.end()) // Key exist
                    report_text << "," << data_key[key_order[key_idx]];
                else                                                // Key not exist
                    report_text << ", ";
            }
            report_text << endl;
        }

        // Write file
        OutFile << report_text.str();

        // Close file
        OutFile.close();

        return true;
    }
    return false;
}

void report::reset()
{
    // Release memory
    set<string>().swap(key_table);
    vector<string>().swap(key_order);
    map<string, string>().swap(data_group_table);
    for (auto data_it = data.begin(); data_it != data.end(); data_it++)
        unordered_map<string, string>().swap(data_it->second.key);
    unordered_map<string, key_object>().swap(data);
}

}
//;)

