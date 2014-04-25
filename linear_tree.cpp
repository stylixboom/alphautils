/*
 * linear_tree.cpp
 *
 *  Created on: February 12, 2014
 *      Author: Siriwat Kasamwattanarote
 */

#include "linear_tree.h"

using namespace std;
using namespace tr1;

namespace alphautils
{

linear_tree::linear_tree(void)
{
    node_count = 0;
}

linear_tree::~linear_tree(void)
{
    release_mem();
}

size_t linear_tree::add_root(size_t node_label)
{
    // Add root node with 0 cost to root and its root is ROOT_ZERO
    tree_space.push_back((tree_node){ROOT_ZERO, node_label, 0, false});
    // Add leaf list with initial cost is 0
    leaf_list[node_count++] = 0;

    return node_count - 1;
}

size_t linear_tree::add_node(size_t parent_id, size_t node_label)
{
    // Default cost=1 if no cost_to_parent specified
    return add_node(parent_id, node_label, 1.0f);
}

size_t linear_tree::add_node(size_t parent_id, size_t node_label, float cost_to_parent)
{
    // Add new node
    /*
    -parent_id is given
    -node_count is to keep track lookup index of current_id in the tree_space
    -cost_to_root is accumulated with parent_cost_to_root
    -off node is false for new node
    */
    float cost_to_root = tree_space[parent_id].cost_to_root + cost_to_parent;
    tree_space.push_back((tree_node){parent_id, node_label, cost_to_root, false});
    // Add leaf list with cost
    leaf_list[node_count++] = cost_to_root;
    // Remove parent from leaf_list
    unordered_map<size_t, float>::iterator parent_node = leaf_list.find(parent_id);
    if (parent_node != leaf_list.end())
        leaf_list.erase(parent_node);

    return node_count - 1;
}

void linear_tree::off_node(size_t node_id)
{
    tree_space[node_id].off_node = true; // Set off_node flag to true
    // Remove node from leaf_list if it is off
    unordered_map<size_t, float>::iterator a_node = leaf_list.find(node_id);
    if (a_node != leaf_list.end())
        leaf_list.erase(a_node);
}

bool linear_tree::is_off_node(size_t node_id)
{
    if (tree_space[node_id].off_node)               // Check current node is off node or not
        return true;

    size_t parent_id = tree_space[node_id].parent_id;

    // Traverse to root to check off_node
    while (parent_id != ROOT_ZERO)
    {
        if (tree_space[parent_id].off_node)           // If path to root is off_node at some node, this node_id is also an off_node
            return true;
        parent_id = tree_space[parent_id].parent_id;
    }

    return false;
}

size_t linear_tree::get_node_label(size_t node_id)
{
    return tree_space[node_id].node_label;
}

float linear_tree::get_cost_at(size_t node_id)
{
    return tree_space[node_id].cost_to_root; // cost to root
}

vector<size_t> linear_tree::get_path_label(size_t end_node_id)
{
    vector<size_t> ret_path;
    size_t parent_id = tree_space[end_node_id].parent_id;

    // Traverse to root
    while (parent_id != ROOT_ZERO)
    {
        // construct path_label to root
        ret_path.push_back(tree_space[parent_id].node_label);
        parent_id = tree_space[parent_id].parent_id;
    }

    return ret_path;
}

bool linear_tree::is_path_contains_label(size_t end_node_id, size_t chk_node_label)
{
    if (tree_space[end_node_id].node_label == chk_node_label)   // check current node if it is the same label
        return true;

    size_t parent_id = tree_space[end_node_id].parent_id;

    // Traverse to root to check off_node
    while (parent_id != ROOT_ZERO)
    {
        if (tree_space[parent_id].node_label == chk_node_label)
            return true;
        parent_id = tree_space[parent_id].parent_id;
    }

    // not contain the same label in the path to root
    return false;
}

void linear_tree::release_mem()
{
    leaf_list.clear();
    tree_space.clear();
}

}
//:)
