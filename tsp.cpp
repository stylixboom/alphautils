/*
 * tsp.cpp
 *
 *  Created on: February 14, 2014
 *      Author: Siriwat Kasamwattanarote
 */

#include "tsp.h"

using namespace std;
using namespace tr1;

namespace alphautils
{

tsp::tsp(size_t total_node, float symmat_dist[])
{
    set_node_label(total_node);
    set_distance_matrix(symmat_dist);
}

tsp::tsp(const vector<size_t>& labels, float symmat_dist[])
{
    set_node_label(labels);
    set_distance_matrix(symmat_dist);
}

tsp::~tsp(void)
{
    release_mem();
}

void tsp::set_node_label(size_t total_node)
{
    _total_node = total_node;
    node_labels = new size_t[_total_node];
    for (size_t node_id = 0; node_id < _total_node; node_id++)
        node_labels[node_id] = node_id;
}

void tsp::set_node_label(const vector<size_t>& labels)
{
    _total_node = labels.size();
    node_labels = new size_t[_total_node];
    for (size_t node_id = 0; node_id < _total_node; node_id++)
        node_labels[node_id] = labels[node_id];
}

void tsp::set_distance_matrix(float symmat_dist[])
{
    _symmat_dist = symmat_dist;
}

// Return node_id in tsp_space for the best node in the leaf
size_t tsp::find_best_leaf_node()
{
    // Finding best node in term of lowest cost_to_root
    size_t best_node = 0;           // node_id
    float best_dist = 1000000;  // cost_to_root
    unordered_map<size_t, float>::const_iterator leaf_list_it;
    for (leaf_list_it = tsp_space.leaf_list.begin(); leaf_list_it != tsp_space.leaf_list.end(); leaf_list_it++)
    {
        // Find shortest cost_to_root
        if (best_dist > leaf_list_it->second)
        {
            best_node = leaf_list_it->first;   // keep shortest node
            best_dist = leaf_list_it->second;  // keep shortest dist
        }
    }

    return best_node;
}

//To verify node_id is good or not
// true, it is good
// false, it is not good
bool tsp::verify_good_candidate(size_t node_id)
{
    size_t candidate_label = tsp_space.get_node_label(node_id);
    float candidate_cost = tsp_space.get_cost_at(node_id);
    if (extended_list.find(candidate_label) != extended_list.end())  // found node_label exists in extended_list
    {
        // check better cost_to_root for each leaf in the same label of extended_list
        size_t extended_node_size = extended_list[candidate_label].size();
        for (size_t extended_idx = 0; extended_idx < extended_node_size; extended_idx++)
        {
            size_t extended_node_id = extended_list[candidate_label][extended_idx];
            float extended_cost = tsp_space.get_cost_at(extended_node_id);
            if (candidate_cost > extended_cost)
            {
                // Found better cost, then remove this candidate from leaf_node, then return false
                tsp_space.off_node(node_id);
                return false;
            }
        }
    }
    // return true, this node is valid to be extended
    return true;
}

vector<size_t> tsp::search_path(int method)
{
    // GLobal search variable
    float best_leaf_cost = 1000000;

    // TSP_BB:Branch and Bound (NP-Hard)
    if (method == TSP_BB)
    {
        // Start extending
        // Add first node in tsp_space, not add extended_list yet
        size_t best_parent_id = tsp_space.add_root(node_labels[0]);
        //cout << "Add root: 0,0" << endl;

        while (tsp_space.leaf_list.size() > 0)
        {
            // Extending child from best leaf
            best_parent_id = find_best_leaf_node();

            // Verify best node, then off node if possible
            while (!verify_good_candidate(best_parent_id))
                best_parent_id = find_best_leaf_node();

            size_t best_parent_label = tsp_space.get_node_label(best_parent_id);

            //size_t leaf_size = tsp_space.leaf_list.size();
            //cout << "Leaf size: " << leaf_size << endl;
            //cout << "Extending.." << endl;
            //cout << "Best parent node: " << best_parent_label << "," << best_parent_id << endl;

            // New children
            vector<size_t> children_lists;

            // Path mask
            vector<size_t> prev_path = tsp_space.get_path_label(best_parent_id);
            bool* path_exits = new bool[_total_node];
            for (size_t node_id = 0; node_id < _total_node; node_id++)
                path_exits[node_id] = false;
            for (size_t prev_path_idx = 0; prev_path_idx < prev_path.size(); prev_path_idx++)
                path_exits[prev_path_idx] = true;

            // Touching (not touch root, id = 0)
            for (size_t label_id = 1; label_id < _total_node; label_id++)
            {
                size_t child_id;
                size_t child_label = node_labels[label_id];

                // Skip if this gen child_label exists in the selected path_to_root
                if (path_exits[child_label] || best_parent_label == child_label)
                    continue;

                // Add child to best_parent
                float cost_to_parent = get_symmat_at(_symmat_dist, child_label, best_parent_label);
                child_id = tsp_space.add_node(best_parent_id, child_label, cost_to_parent);

                // Add child to new generated children_list
                children_lists.push_back(child_id);
            }

            // Check expandable children for this node
            if (children_lists.size() == 0)
            {

                // if not expandable, this is the final leaf node
                // Track if it is best leaf node
                float best_parent_cost = tsp_space.get_cost_at(best_parent_id);
                if (best_leaf_cost >  best_parent_cost)
                    best_leaf_cost = best_parent_cost;
                tsp_space.off_node(best_parent_id);
                continue;
            }

            // Add parent to extended_list
            extended_list[best_parent_label].push_back(best_parent_id);

            //cout << "Expanded children: "; cout.flush();

            // Debug, child expandible
            /*float best_child_cost_to_root = 1000000;
            for (size_t child_idx = 0; child_idx < children_lists.size(); child_idx++)
            {
                size_t child_id = children_lists[child_idx];
                float child_cost = tsp_space.get_cost_at(child_id);
                size_t child_label = tsp_space.get_node_label(child_id);

                cout << child_label << "," << child_id << " "; cout.flush();
            }
            cout << endl;*/

            delete[] path_exits;
        }

        // Save best path
        if (best_path.size() > 0)
            best_path.clear();
        best_path = tsp_space.get_path_label(best_parent_id);
    }
    // TSP_NN:Nearest Neightbor (Greedy)
    else if (method == TSP_NN)
    {
        // Start extending
        // Add first node in tsp_space, not add extended_list yet
        size_t best_parent_id = tsp_space.add_root(node_labels[0]);
        //cout << "Add root: 0,0" << endl;

        bool expandable_node;
        do
        {
            expandable_node = false;
            // Extending child from best leaf
            size_t best_parent_label = tsp_space.get_node_label(best_parent_id);
            float best_parent_cost = tsp_space.get_cost_at(best_parent_id);

            //size_t leaf_size = tsp_space.leaf_list.size();
            //cout << "Leaf size: " << leaf_size << endl;
            //cout << "Extending.." << endl;
            //cout << "Best parent node: " << best_parent_label << "," << best_parent_id << endl;


            // Path mask
            vector<size_t> prev_path = tsp_space.get_path_label(best_parent_id);
            bool* path_exits = new bool[_total_node];
            for (size_t node_id = 0; node_id < _total_node; node_id++)
                path_exits[node_id] = false;
            for (size_t prev_path_idx = 0; prev_path_idx < prev_path.size(); prev_path_idx++)
                path_exits[prev_path[prev_path_idx]] = true;

            // Touching (not touch root, id = 0)
            size_t best_child_label = 0;
            float best_child_cost_to_root = 1000000;
            float best_child_cost = 0;
            for (size_t label_id = 1; label_id < _total_node; label_id++)
            {
                size_t child_label = node_labels[label_id];

                // Skip if this gen child_label exists in the selected path_to_root
                if (path_exits[child_label] || best_parent_label == child_label)
                    continue;

                // Add child to best_parent
                float cost_to_parent = get_symmat_at(_symmat_dist, child_label, best_parent_label);
                float cost_to_root = best_parent_cost + cost_to_parent;

                // Find best child to go next
                if (best_child_cost_to_root > cost_to_root)
                {
                    best_child_label = child_label;
                    best_child_cost_to_root = cost_to_root;
                    best_child_cost = cost_to_parent;
                }

                expandable_node = true;
            }

            // Add best child
            best_parent_id = tsp_space.add_node(best_parent_id, best_child_label, best_child_cost);

            prev_path.clear();
            delete[] path_exits;
        }
        while (expandable_node);

        // Save best path
        if (best_path.size() > 0)
            best_path.clear();
        best_path = tsp_space.get_path_label(best_parent_id);
    }
    // TSP_NN_FAST:Nearest Neighbor without using linear_tree
    else if (method == TSP_NN_FAST)
    {
        // Path mask
        bool* path_exits = new bool[_total_node];
        for (size_t node_id = 0; node_id < _total_node; node_id++)
            path_exits[node_id] = false;

        // Start extending
        size_t parent_label = node_labels[0];
        best_path.push_back(parent_label);
        path_exits[parent_label] = true;

        bool expandable_node;
        do
        {
            expandable_node = false;

            // Touching (not touch root, id = 0)
            size_t best_child_label = 0;
            float best_child_cost = 1000000;
            for (size_t child_id = 1; child_id < _total_node; child_id++)
            {
                size_t child_label = node_labels[child_id];

                // Skip if this gen child_label exists in the selected path_to_root
                if (path_exits[child_label] || parent_label == child_label)
                    continue;

                // Add child to best_parent
                float cost_to_parent = get_symmat_at(_symmat_dist, child_label, parent_label);

                // Find best child to go next
                if (best_child_cost > cost_to_parent)
                {
                    best_child_label = child_label;
                    best_child_cost = cost_to_parent;
                }

                expandable_node = true;
            }

            // Add best child
            if (expandable_node)
            {
                best_path.push_back(best_child_label);
                path_exits[best_child_label] = true;
            }

            // Save last child
            parent_label = best_child_label;
        }
        while (expandable_node);

        // Release memory
        delete[] path_exits;
    }
    // TSP_LINKNJOY:Fastest TSP with pre-sorted symmat-dist, link-and-join
    else
    {
        // Variable
        size_t* path1 = new size_t[_total_node];                                                                        // Two-way, raw best path
        size_t* path2 = new size_t[_total_node];                                                                        //
        int* linked = new int[_total_node];                                                                             // linked node counter
        size_t max_link = _total_node;
        size_t* link_id = new size_t[_total_node];                                                                      // link_id for each linked node
        vector< pair<size_t, size_t> > link;                                                                            // end nodes for each link
        vector<bool> disable_link;                                                                                      // flag of disabled link
        vector<size_t> sorted_symmat_idx = get_sorted_indices(_symmat_dist, get_symmat_size(_total_node), SORT_ASC);    // sorted_idx for symmat_dist
        size_t symmat_size = get_symmat_size(_total_node);

        /*
        cout << "symmat sorted test ";
        for (size_t idx = 0; idx < sorted_symmat_idx.size(); idx++)
            cout << _symmat_dist[sorted_symmat_idx[idx]] << " ";
        cout.flush();
        cout << endl;
        */

        // Initialize
        // path1, path2, ok_link, link_id, and linked
        for (size_t node_id = 0; node_id < _total_node; node_id++)
        {
            path1[node_id] = -1;
            path2[node_id] = -1;
            linked[node_id] = 0;
            link_id[node_id] = -1;
        }

        // Linking
        for (size_t symmat_id = 0; symmat_id < symmat_size; symmat_id++)
        {
            if (max_link == 2)                                                  /// Skip the rest if only two nodes left
            {
                //cout << "Stop at symmat_id: " << symmat_id << endl;
                break;
            }

            size_t first, second;
            xy_idx rc = half2rc_idx(sorted_symmat_idx[symmat_id]);

            // Reorder first-second pair of this link
            if (rc.x < rc.y) {
                first = rc.x; second = rc.y;
            } else {
                first = rc.y; second = rc.x;
            }

            int first_linked = linked[first];
            int second_linked = linked[second];
            size_t first_link_id = link_id[first];
            size_t second_link_id = link_id[second];

            if (first_linked == 2 || second_linked == 2)                        // Skip if this node has fully linked
                continue;

            if (first_linked == 0 && second_linked == 0)                        /// Never exist -> create new link
            {
                link_id[first] = link.size();                                   // Update link_id for first node
                link_id[second] = link.size();                                  // Update link_id for second node
                link.push_back(pair<size_t, size_t>(first, second));            // Add link
                disable_link.push_back(false);                                  // Disable link flag is false for new link
                path1[first] = second;                                          // Set path1, for first node
                path1[second] = first;                                          // Set path1, for second node
            }
            else if (first_linked == 1 && second_linked == 1)                   /// Both nodes can join two links together
            {
                if (first_link_id == second_link_id)                            /// Reject if this link is on the same line_id (loopback)
                    continue;

                size_t join_first, join_second;
                                                                                // Save only the rest node to be joined
                if (link[first_link_id].first != first)                         // Keep the rest node from first link
                    join_first = link[first_link_id].first;
                else
                    join_first = link[first_link_id].second;

                if (link[second_link_id].first != second)                       // Keep the rest node from second link
                    join_second = link[second_link_id].first;
                else
                    join_second = link[second_link_id].second;

                if (first_link_id < second_link_id)                             // Joining link to lower order, then disable higher order
                {
                    link_id[join_first] = first_link_id;                        // Update link_id for new joined node to first_link_id
                    link_id[join_second] = first_link_id;

                    link[first_link_id].first = join_first;                     // Replace first link with updated-join-link
                    link[first_link_id].second = join_second;

                    disable_link[second_link_id] = true;                        // Disable second link

                    if (link[first_link_id].first > link[first_link_id].second) // Reorder link from low label to high lebel
                    {
                        size_t tmp = link[first_link_id].first;
                        link[first_link_id].first = link[first_link_id].second;
                        link[first_link_id].second = tmp;
                    }
                }
                else
                {
                    link_id[join_first] = second_link_id;                       // Update link_id for new joined node to first_link_id
                    link_id[join_second] = second_link_id;

                    link[second_link_id].first = join_first;                    // Replace second link with updated-join-link
                    link[second_link_id].second = join_second;

                    disable_link[first_link_id] = true;                         // Disable first link

                    if (link[second_link_id].first > link[second_link_id].second) // Reorder link from low label to high lebel
                    {
                        size_t tmp = link[second_link_id].first;
                        link[second_link_id].first = link[second_link_id].second;
                        link[second_link_id].second = tmp;
                    }
                }

                path2[first] = second;                                          // Set path2, for first node
                path2[second] = first;                                          // Set path2, for second node
                max_link -= 2; // Tracking count of the rest end node
            }
            else if (first_linked == 1)                                         /// First node can be linked
            {
                link_id[second] = first_link_id;                                // Update link_id

                if (link[first_link_id].first == first)                         // Replace second node to link
                    link[first_link_id].first = second;
                else
                    link[first_link_id].second = second;

                if (link[first_link_id].first > link[first_link_id].second)     // Reorder link from low label to high lebel
                {
                    size_t tmp = link[first_link_id].first;
                    link[first_link_id].first = link[first_link_id].second;
                    link[first_link_id].second = tmp;
                }

                path2[first] = second;                                          // Set path2 for inner node
                path1[second] = first;                                          // Set path1 for new node
                max_link--; // Tracking count of the rest end node
            }
            else if (second_linked == 1)                                        /// Second node can be linked
            {
                link_id[first] = second_link_id;                                // Update link_id

                if (link[second_link_id].first == second)                       // Replace second node to link
                    link[second_link_id].first = first;
                else
                    link[second_link_id].second = first;

                if (link[second_link_id].first > link[second_link_id].second)   // Reorder link from low label to high lebel
                {
                    size_t tmp = link[second_link_id].first;
                    link[second_link_id].first = link[second_link_id].second;
                    link[second_link_id].second = tmp;
                }

                path2[second] = first;                                          // Set path2 for inner node
                path1[first] = second;                                          // Set path1 for new node
                max_link--; // Tracking count of the rest end node
            }
            linked[first]++;                                                    // Accumulate counter on first link
            linked[second]++;                                                   // Accumulate counter on second link
        }

        /*
        cout << "Linked: ";
        for (size_t linked_idx = 0; linked_idx < _total_node; linked_idx++)
            cout << linked[linked_idx] << " "; cout.flush();
        cout << endl;
        cout << "Path1: ";
        for (size_t path_idx = 0; path_idx < _total_node; path_idx++)
            cout << path1[path_idx] << " "; cout.flush();
        cout << endl;
        cout << "Path2: ";
        for (size_t path_idx = 0; path_idx < _total_node; path_idx++)
            cout << path2[path_idx] << " "; cout.flush();
        cout << endl;
        */

        // Generate best_path from raw path
        // The best path is on the first link
        size_t parent_label = link[0].first;
        size_t child_label = path1[parent_label];

        /*
        int* path_chk = new int[_total_node];
        for (size_t idx = 0; idx < _total_node; idx++)
            path_chk[idx] = 0;

        cout << "Found path " << parent_label << ":" << end_label << endl;
        cout << parent_label << "\t-> " << child_label <<  "\t" << path_chk[parent_label]++ << "\t-> " << path_chk[child_label]++ << endl;
        */

        // Add parent
        best_path.push_back(parent_label);
        best_path.push_back(child_label);


        while (best_path.size() < _total_node)
        {
            if (path1[child_label] != parent_label)
            {
                parent_label = child_label;
                child_label = path1[child_label];
            }
            else
            {
                parent_label = child_label;
                child_label = path2[child_label];
            }

            //cout << parent_label << "\t-> " << child_label <<  "\t" << path_chk[parent_label]++ << "\t-> " << path_chk[child_label]++ << endl;

            best_path.push_back(child_label);
        }

        // Release memory
        //delete[] path_chk;
        delete[] path1;
        delete[] path2;
        delete[] link_id;
        delete[] linked;
        link.clear();
        disable_link.clear();
        sorted_symmat_idx.clear();
    }

    // Debug
    /*
    cout << "Get path: [" << best_path.size() << "] ";
    for (size_t path_id = 0; path_id < best_path.size(); path_id++)
    {
        cout << best_path[path_id] << " "; cout.flush();
    }
    cout << endl;
    */
    cout << "Total distance: ";
    float total_dist = 0;
    for (size_t path_id = 0; path_id < best_path.size() - 1; path_id++)
    {
        total_dist += get_symmat_at(_symmat_dist, best_path[path_id], best_path[path_id + 1]);
    }
    cout << total_dist << endl;



    return best_path;   // best_path
}

float tsp::get_average_inter_distance()
{
    float avg_dist = 0;

    for (size_t node_idx = 0; node_idx < best_path.size() - 1; node_idx++)
        avg_dist += get_symmat_at(_symmat_dist, best_path[node_idx + 1], best_path[node_idx]);

    return avg_dist / (best_path.size() - 1);
}

float tsp::get_otsu_inter_distance()
{
    // Fill in distance
    // edge amount is equal to n - 1
    size_t data_size = best_path.size() - 1;
    float* data = new float[data_size];
    for (size_t node_idx = 0; node_idx < best_path.size() - 1; node_idx++)
        data[node_idx] = get_symmat_at(_symmat_dist, best_path[node_idx + 1], best_path[node_idx]);

    // Save threshold
    float ret_threshold = otsu_threshold_from_data(data, data_size, 4);

    // Release memory
    delete[] data;

    return ret_threshold;
}

float tsp::get_cost_between(size_t label1, size_t label2)
{
    return get_symmat_at(_symmat_dist, label1, label2);
}

void tsp::release_mem()
{
    delete[] node_labels;
}

}
//:)
