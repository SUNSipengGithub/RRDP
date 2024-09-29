#include <iostream>
#include <fstream>

#include "Save_Result.h"
using namespace std;

/**
 * @brief Saves the shortest distance results to a specified output file.
 * 
 * @param output_path The path to the output file where shortest distances will be saved.
 */
void save_shortest_dis(string output_path) {
    ofstream ofile(output_path.c_str());	// Open the output file for writing
    
    // Write the number of vertices to the file
    ofile << g_n_vtx << " " << endl;
	// Write the source vertex coordinates (row and column)
    ofile << g_src_vtx / g_dim << " " << g_src_vtx % g_dim << endl << endl;

    // Iterate through each vertex to write its shortest distance
    for (int i = 0; i < g_n_vtx; i++) {
        if (g_shortest_dis[i] < INF) // Check if the distance is valid
            // Write vertex coordinates and its shortest distance
            ofile << i / g_dim << " " << i % g_dim << " \t "
                  << g_shortest_dis[i] << " " << endl;
        else
            // Write vertex coordinates and indicate infinite distance
            ofile << i / g_dim << " " << i % g_dim << " \t "
                  << INF << " " << endl;
    }
    
    ofile.close(); // Close the output file

    cout << "Save the shortest distance: " << output_path << endl;
}

/**
 * @brief Saves the shortest path predecessor information to a specified output file.
 * 
 * @param output_path The path to the output file where predecessor data will be saved.
 */
void save_shortest_pre(string output_path) {
    ofstream ofile(output_path.c_str());	// Open the output file for writing
    
    // Write the number of vertices to the file
    ofile << g_n_vtx << " " << endl;
    // Write the source vertex coordinates (row and column)
    ofile << g_src_vtx / g_dim << " " << g_src_vtx % g_dim << endl << endl;

    // Iterate through each vertex to write its predecessor information
    for (int i = 0; i < g_n_vtx; i++) {
        if (i == g_src_vtx) continue; // Skip the source vertex

        // Write vertex coordinates and its predecessor's coordinates
        ofile << i / g_dim << " " << i % g_dim << " \t "
              << g_parent[i] / g_dim << " " << g_parent[i] % g_dim << " " << endl;
    }

    ofile.close(); // Close the output file

    cout << "Save the predecessor: " << output_path << endl;
}
