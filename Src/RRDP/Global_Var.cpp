#include <fstream>
#include <sstream>
#include "Global_Var.h"

int g_src_vtx = 0;         // Source vertex ID
int g_src_vtx_row = 0;     // Row position of the source vertex
int g_src_vtx_col = 0;     // Column position of the source vertex

int g_dim = 50;             // Dimension of the graph (e.g., a graph is g_dim * g_dim square)
int g_n_vtx = 50 * 50;      // Total number of vertices in the graph
int** g_arcs = NULL;        // Distance matrix representing the weights of edges between vertices
vector<vector<int>> g_neighbor; // Adjacency list for graph neighbors
string PROBLEM_PATH = ""; 	// Path to the problem file

int* g_parent = NULL;       // Array to store the parent of each vertex in the shortest path tree
int* g_shortest_dis = NULL; // Array to store the shortest distance from the source vertex to each vertex

int g_repeat_time = 0;      // Number of repetitions for each algorithm

/**
 * @brief Reads parameters from a configuration file.
 * 
 * @param config_path The path to the configuration file.
 */
void get_parameter(string config_path) {
    ifstream ifile(config_path.c_str()); // Open the configuration file
    if (ifile.fail()) { // Check if the file was opened successfully
        cout << __FUNCTION__ << std::endl
             << "Fail to open the file "
             << config_path << endl;
        exit(EXIT_FAILURE);
    }

    string line = ""; // Variable to hold each line read from the file
    string tmp = "";  // Temporary string for parsing
    while (getline(ifile, line)) { // Read each line from the file
        istringstream tmp_line(line); // Create a string stream from "line"
        tmp_line >> tmp; // Read the first token

        // Parse the parameters based on expected keywords
        if (tmp == "DIMENSION:") tmp_line >> g_dim;
        else if (tmp == "REPEAT_TIME:") tmp_line >> g_repeat_time;
        else if (tmp == "INSTANCE_PATH:") tmp_line >> PROBLEM_PATH;
        else if (tmp == "SOURCE_VERTEX_ROW:") tmp_line >> g_src_vtx_row;
        else if (tmp == "SOURCE_VERTEX_COLUMN:") tmp_line >> g_src_vtx_col;
    }
    ifile.close(); // Close the input file

    g_n_vtx = g_dim * g_dim; // Update the total number of vertices
    g_src_vtx = g_src_vtx_row * g_dim + g_src_vtx_col; // Calculate the source vertex ID
}

/**
 * @brief Allocates memory for global variables.
 */
void allocate_memory() {
    g_arcs = new int*[g_n_vtx];
    for (int i = 0; i < g_n_vtx; i++)
        g_arcs[i] = new int[g_n_vtx];
    g_neighbor.resize(g_n_vtx);

    g_parent = new int[g_n_vtx];
    g_shortest_dis = new int[g_n_vtx];
}

/**
 * @brief Deallocates memory for global variables.
 */
void destroy_memory() {
    for (int i = 0; i < g_n_vtx; i++)
        delete[] g_arcs[i]; 
    delete[] g_arcs; 

    delete[] g_parent;
    delete[] g_shortest_dis;
}
