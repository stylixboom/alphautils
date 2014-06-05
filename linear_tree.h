/*
 * linear_tree.h
 *
 *  Created on: February 12, 2014
 *      Author: Siriwat Kasamwattanarote
 */
#pragma once
#include <cstdlib>
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
#include <cmath>        // Math
#include <limits>       // limit (for max math limit and etc)
#include <unordered_map>
#include "alphautils.h"

using namespace std;

namespace alphautils
{

/*
This class is a simple design of tree structure.
All nodes were stored in the flat vector structure with the lookup link;
Also, the cost to parent is provided.
Note:
-Node label is size_t type
-Node with parent ROOT_ZERO is root node
-Cost to parent is only float type
-off_node is to specify the node and its children are off
*/

class linear_tree
{
    // Struct
    typedef struct _tree_node{ size_t parent_id; size_t node_label; float cost_to_root; bool off_node; } tree_node;  // simple tree node with parent link and cost to root

    // Variable
    size_t node_count;                                      // total node count
    vector<tree_node> tree_space;                           // main tree

    // Function
    void release_mem();

public:
    static const size_t ROOT_ZERO = -1;                     // Root's parent

	// Variable
	unordered_map<size_t, float> leaf_list;                 // leaf_list, node_id with cost to root

    linear_tree(void);
    ~linear_tree(void);
    size_t add_root(size_t node_label);
    size_t add_node(size_t parent_id, size_t node_label);
    size_t add_node(size_t parent_id, size_t node_label, float cost_to_parent);
    void off_node(size_t node_id);
    bool is_off_node(size_t node_id);
    size_t get_node_label(size_t node_id);
    float get_cost_at(size_t node_id);
    vector<size_t> get_path_label(size_t end_node_id);                                // get path_to_root for specified node
    bool is_path_contains_label(size_t end_node_id, size_t chk_node_label);     // check if path contains specified chk_node_label in the path to root
};

};
//;)
