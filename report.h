/*
 * qb.h
 *
 *  Created on: August 5, 2014
 *              Siriwat Kasamwattanarote
 */
#pragma once

namespace alphautils
{
    // Anything shared with alphautils namespace here
    typedef struct _key_object{ unordered_map<string, string> key;  } key_object;

class report
{
    // Private variables and functions here
    string report_path;
    string report_name;

    vector<string> report_description;
    set<string> key_table;
    vector<string> key_order;
    map<string, string> data_group_table;
    unordered_map<string, key_object> data;
    string last_data_name;
public:
    report();
    ~report(void);

    void init(const string& path, const string& name);

    void add_description(const string& value);
    bool new_data(const string& data_name, const string& group_name = "");
    bool add_group_to_data(const string& data_name, const string& group_name);
    bool add_key(const string& key, const string& value);                               // add key at last data
    bool add_key_at(const string& key, const string& value, const string& data_name);   // add key at specific data name

    bool save_report();

    void reset();
};

};
//;)
