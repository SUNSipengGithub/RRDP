#ifndef _Save_Result_h
#define _Save_Result_h

#include <fstream>      
#include "Global_Var.h"

/**
 * @brief Saves the shortest distance results to a specified output file.
 * 
 * @param output_path The path to the output file where shortest distances will be saved.
 */
void save_shortest_dis(string output_path);

/**
 * @brief Saves the shortest path predecessor information to a specified output file.
 * 
 * @param output_path The path to the output file where predecessor data will be saved.
 */
void save_shortest_pre(string output_path);

#endif // !_Save_Result_h
