#include <ctime>
#include <cstring>
#include <iomanip>
#include <sstream>

#include "Global_Var.h"
#include "Read_Problem.h"
#include "Save_Result.h"
#include "Heap_Dijkstra.h"
#include "RRDP.h"

// Global variables to store total computation times
double total_time1 = 0; // Time taken by Dijkstra's algorithm
double total_time2 = 0; // Time taken by RRDP algorithm
clock_t start_time, end_time; // For measuring execution time

/**
 * @brief Calculates the elapsed time between two clock ticks.
 * 
 * @param start_time The start time of the measurement.
 * @param end_time The end time of the measurement.
 * @return The elapsed time in seconds.
 */
double get_time(clock_t start_time, clock_t end_time) {
	return (double)(end_time - start_time) / CLOCKS_PER_SEC; // Convert clock ticks to seconds
}

/**
 * @brief Compares the results of two shortest path calculations.
 * 
 * @return true if the results match, false if they differ.
 */
bool compare() {
	ifstream ifile1("dijkstra_dis.txt"); // Open the Dijkstra's result file
	ifstream ifile2("rrdp_dis.txt"); // Open the our (RRDP's) result file

	string tmp1, tmp2; // Temporary strings for reading lines
	while (getline(ifile1, tmp1) && getline(ifile2, tmp2)) {
		if (tmp1 == tmp2) continue; // Skip if the results match

		// Print mismatched results
		cout << "right: " << tmp1 << endl;
		cout << "wrong: " << tmp2 << endl;
		ifile1.close(); // Close both files
		ifile2.close();
		return false; // Return false if there is a discrepancy
	}

	ifile1.close(); // Close both files
	ifile2.close(); 
	return true; // Return true if all results match
}

/**
 * @brief Records the execution times of the algorithms.
 */
void record_time() {
	ofstream ofile;
	
	// Record the time taken by Dijkstra's algorithm
	ofile.open("dijkstra_time.txt", ios::app);
	ofile << total_time1 << endl;
	ofile.close();
	
	// Record the time taken by RRDP algorithm
	ofile.open("rrdp_time.txt", ios::app);
	ofile << total_time2 << endl;
	ofile.close();
	
	// Record the ratio of the two times
	ofile.open("ratio_time.txt", ios::app);
	ofile << total_time1 / total_time2 << endl;
	ofile.close();
}


int main(int argc, char* argv[]) {
	if (argc < 2) { // Check if config path argument is provided
		cout << "ERROR\nNeed Config Path parameter!!!\n\n" << endl;
		exit(EXIT_FAILURE);
	}
	
    string config_file_path = argv[1]; // Get the configuration file path from command line
    get_parameter(config_file_path); // Load parameters from the config file
	allocate_memory();	// Allocate memory for global variables
 
    read_problem(PROBLEM_PATH);	// Read problem data
	cout << "\nFinished reading the problem file -- " << endl
		 << PROBLEM_PATH << endl << endl;

 	// *******************************************DIJKSTRA************************************************
 	start_time = clock(); // Start timing for Dijkstra's algorithm
	for (int i_repeat = 0; i_repeat < g_repeat_time; i_repeat++) {
		dijkstra_heap(g_shortest_dis, g_parent); // Execute Dijkstra's algorithm
	}
	end_time = clock(); // End timing for Dijkstra's algorithm
	total_time1 = get_time(start_time, end_time); // Calculate total time taken
	cout << "Dijkstra with binary heap calculate source vertex : (" 
		 << g_src_vtx_row << ", " << g_src_vtx_col << ")" << endl
		 << "Repeat Time : " << g_repeat_time <<  endl 
		 << "the total time is: " << total_time1 << " seconds" << endl;
	save_shortest_dis("dijkstra_dis.txt");
	save_shortest_pre("dijkstra_pre.txt");
	cout << endl;
	// ***************************************************************************************************

	// *******************************************Another Method************************************************
	start_time = clock(); // Start timing for RRDP algorithm
	for (int i_repeat = 0; i_repeat < g_repeat_time; i_repeat++) {
		rrdp(g_shortest_dis, g_parent); // Execute RRDP algorithm
	}
	end_time = clock(); // End timing for RRDP algorithm
	total_time2 = get_time(start_time, end_time); // Calculate total time taken
	cout << "New method calculate source vertex : (" 
		 << g_src_vtx_row << ", " << g_src_vtx_col << ")" << endl
		 << "Repeat Time : " << g_repeat_time <<  endl 
		 << "the total time is: " << total_time2 << " seconds" << endl;
	save_shortest_dis("rrdp_dis.txt");
	save_shortest_pre("rrdp_pre.txt");
 	cout << endl;
 	// ***************************************************************************************************

	record_time(); // Print and record execution times
	if (compare()) { // Compare the results from both algorithms
		cout << setw(30) << setfill('*') << "SAME!"; // Print if results are the same
		cout << setw(25) << setfill('*') << "" << endl;
	} else {
		cout << setw(30) << setfill('*') << "DIFFERENT!"; // Print if results differ
		cout << setw(20) << setfill('*') << "" << endl;
    }

	cout << "Dijkstra time:  " << total_time1 << endl;
	cout << "Our method time: " << total_time2 << endl;
	cout << "Time improvement rate: " << total_time1 / total_time2 << endl;
	
	destroy_memory(); // Free allocated memory
	return 1;
}
