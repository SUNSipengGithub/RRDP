#ifndef _Global_Var_h
#define _Global_Var_h

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Define a large value for infinity
#define INF 1000000

// Source vertex information
extern int g_src_vtx;         // Source vertex ID
extern int g_src_vtx_row;     // Row position of the source vertex
extern int g_src_vtx_col;     // Column position of the source vertex

// Global parameters related to the graph
extern int g_dim;             // Dimension of the graph (e.g., a graph is g_dim * g_dim square)
extern int g_n_vtx;           // Total number of vertices in the graph
extern int** g_arcs;          // Distance matrix representing the weights of edges between vertices
extern vector<vector<int>> g_neighbor; // Adjacency list for graph neighbors
extern string PROBLEM_PATH;   // Path to the problem file

// Arrays for storing results related to shortest paths
extern int* g_parent;         // Array to store the parent of each vertex in the shortest path tree
extern int* g_shortest_dis;   // Array to store the shortest distance from the source vertex to each vertex

// Variable to determine how many times to repeat the process
extern int g_repeat_time;     // Number of repetitions for each algorithm

/**
 * @brief Reads parameters from a configuration file.
 * 
 * @param config_path The path to the configuration file.
 */
void get_parameter(string config_path);

/**
 * @brief Allocates memory for global variables.
 */
void allocate_memory();

/**
 * @brief Deallocates memory for global variables.
 */
void destroy_memory();

#endif // !_Global_Var_h
