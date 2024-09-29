#ifndef _Read_Problem_h
#define _Read_Problem_h

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Global_Var.h"

/**
 * @brief Reads problem data from a specified file.
 * 
 * @param file_path The path to the problem file.
 */
void read_problem(string file_path) {
    ifstream ifile(file_path.c_str()); // Open the specified file
    if (ifile.fail()) { // Check if the file was opened successfully
        cout << __FUNCTION__ << endl
             << "Fail to open the file "
             << file_path << endl;
        exit(EXIT_FAILURE);
    }
    
    ifile >> g_dim; // Read the dimension of the graph from the file
    g_n_vtx = g_dim * g_dim; // Calculate the total number of vertices

    string line = "";
    int start_ver = -1, end_ver = -1; // Variables to hold the start and end vertices of edges

    // Read each line until the end of the file
    while (getline(ifile, line)) {
        stringstream tmp_line(line);
        tmp_line >> start_ver; // Read the start vertex

        // Read the ending vertices and their corresponding weights
        while (tmp_line >> end_ver) {
            tmp_line >> g_arcs[start_ver][end_ver];
            g_neighbor[start_ver].emplace_back(end_ver);
        }
    }
    ifile.close(); // Close the input file
}

#endif // !_Read_Problem_h
