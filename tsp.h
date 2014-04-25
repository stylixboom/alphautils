/*
 * tsp.h
 *
 *  Created on: February 14, 2014
 *      Author: Siriwat Kasamwattanarote
 */
#pragma once
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>
#include <deque>
#include <vector>
#include <iomanip>      // setw
#include <sys/time.h>   // time
#include <sys/stat.h>   // file-directory existing
#include <sys/types.h>  // file-directory
#include <dirent.h>     // file-directory
#include <math.h>       // Math
#include <limits>       // limit (for max math limit and etc)
#include <tr1/unordered_map>
#include "alphautils.h"
#include "linear_tree.h"

using namespace std;
using namespace tr1;

namespace alphautils
{

/*
This class is a travelling salesman problem
It was designed to find the shortest path through all nodes.
Note:
symmat_dist is only float type
*/

class tsp
{
    // Struct

    // Variable
    linear_tree tsp_space;                                  // main tree
    size_t* node_labels;
    size_t _total_node;
    float* _symmat_dist;
    unordered_map<size_t, vector<size_t> > extended_list;   // an extended_list of leaf (node_label) with node_id

    // Function
    void release_mem();

public:
    // Method
    const static int TSP_BB = 1;        // Branch and bound
    const static int TSP_NN = 2;        // Greedy NN
    const static int TSP_NN_FAST = 3;   // NN without using linear_tree (linear_tree support multiple extensions per node, but not necessary for NN)
    const static int TSP_LINKNJOY = 4;  // Link-and-Join with pre-sorted symmat

    // Variable
    vector<size_t> best_path;

    tsp(size_t total_node, float symmat_dist[]);
    tsp(const vector<size_t>& labels, float symmat_dist[]);
    ~tsp(void);
    void set_node_label(size_t total_node);
    void set_node_label(const vector<size_t>& labels);
    void set_distance_matrix(float symmat_dist[]);
    size_t find_best_leaf_node();
    bool verify_good_candidate(size_t node_id);
    vector<size_t> search_path(int method);                                   // return label list best_path
    float get_average_inter_distance();
    float get_otsu_inter_distance();
    float get_cost_between(size_t label1, size_t label2);
};

};
//;)
