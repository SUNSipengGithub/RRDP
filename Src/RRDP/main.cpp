#include <ctime>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <fstream>

#include "Global_Var.h"
#include "Read_Problem.h"
#include "Save_Result.h"
#include "Heap_Dijkstra.h"
#include "Delta_Stepping.h"
#include "RRDP.h"

// Global variables to store total computation times
double total_time1 = 0; // Time taken by heap Dijkstra
double total_time2 = 0; // Time taken by Delta-Stepping
double total_time3 = 0; // Time taken by RRDP
clock_t start_time, end_time; // Used for measuring execution time

/**
 * @brief Calculates the elapsed time between two clock ticks.
 *
 * @param start_time The start time of the measurement.
 * @param end_time The end time of the measurement.
 * @return The elapsed time in seconds.
 */
double get_time(clock_t start_time, clock_t end_time) {
    return (double)(end_time - start_time) / CLOCKS_PER_SEC;
}

/**
 * @brief Compares two result files line by line.
 *
 * @param file1 Path to the first file.
 * @param file2 Path to the second file.
 * @return true if the two files are identical, otherwise false.
 */
bool compare_two_files(const string& file1, const string& file2) {
    ifstream ifile1(file1.c_str());
    ifstream ifile2(file2.c_str());

    if (!ifile1.is_open() || !ifile2.is_open()) {
        cout << "Fail to open compare files: " << file1 << " or " << file2 << endl;
        return false;
    }

    string tmp1, tmp2;
    int line_no = 1;

    while (getline(ifile1, tmp1) && getline(ifile2, tmp2)) {
        if (tmp1 != tmp2) {
            cout << "Mismatch found at line " << line_no << endl;
            cout << "File1 (" << file1 << "): " << tmp1 << endl;
            cout << "File2 (" << file2 << "): " << tmp2 << endl;
            ifile1.close();
            ifile2.close();
            return false;
        }
        line_no++;
    }

    // Check whether the two files have the same length
    bool extra1 = (bool)getline(ifile1, tmp1);
    bool extra2 = (bool)getline(ifile2, tmp2);

    if (extra1 || extra2) {
        cout << "File length mismatch between " << file1 << " and " << file2 << endl;
        ifile1.close();
        ifile2.close();
        return false;
    }

    ifile1.close();
    ifile2.close();
    return true;
}

/**
 * @brief Compares Dijkstra results with Delta-Stepping and RRDP results.
 *
 * @return true if both comparisons match, otherwise false.
 */
bool compare_all() {
    bool ok1 = compare_two_files("dijkstra_dis.txt", "delta_stepping_dis.txt");
    bool ok2 = compare_two_files("dijkstra_dis.txt", "rrdp_dis.txt");

    cout << endl;
    cout << "[Compare] Dijkstra vs Delta-Stepping : " << (ok1 ? "SAME" : "DIFFERENT") << endl;
    cout << "[Compare] Dijkstra vs RRDP           : " << (ok2 ? "SAME" : "DIFFERENT") << endl;
    cout << endl;

    return ok1 && ok2;
}

/**
 * @brief Records the execution times of the algorithms.
 */
void record_time() {
    ofstream ofile;

    ofile.open("dijkstra_time.txt", ios::app);
    ofile << total_time1 << endl;
    ofile.close();

    ofile.open("delta_stepping_time.txt", ios::app);
    ofile << total_time2 << endl;
    ofile.close();

    ofile.open("rrdp_time.txt", ios::app);
    ofile << total_time3 << endl;
    ofile.close();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "ERROR\nNeed Config Path parameter!!!\n\n" << endl;
        exit(EXIT_FAILURE);
    }

    string config_file_path = argv[1];
    get_parameter(config_file_path);
    allocate_memory();

    read_problem(PROBLEM_PATH);
    cout << "\nFinished reading the problem file -- " << endl
         << PROBLEM_PATH << endl << endl;

    // ******************************************* HEAP DIJKSTRA *******************************************
    start_time = clock();
    for (int i_repeat = 0; i_repeat < g_repeat_time; i_repeat++) {
        dijkstra_heap(g_shortest_dis, g_parent);
    }
    end_time = clock();
    total_time1 = get_time(start_time, end_time);

    cout << "Dijkstra with binary heap calculate source vertex : ("
         << g_src_vtx_row << ", " << g_src_vtx_col << ")" << endl
         << "Repeat Time : " << g_repeat_time << endl
         << "The total time is: " << total_time1 << " seconds" << endl;

    save_shortest_dis("dijkstra_dis.txt");
    save_shortest_pre("dijkstra_pre.txt");
    cout << endl;
    // ****************************************************************************************************

    // ******************************************* DELTA-STEPPING ******************************************
    start_time = clock();
    for (int i_repeat = 0; i_repeat < g_repeat_time; i_repeat++) {
        delta_stepping(g_shortest_dis, g_parent);
    }
    end_time = clock();
    total_time2 = get_time(start_time, end_time);

    cout << "Delta-Stepping calculate source vertex : ("
         << g_src_vtx_row << ", " << g_src_vtx_col << ")" << endl
         << "Repeat Time : " << g_repeat_time << endl
         << "The total time is: " << total_time2 << " seconds" << endl;

    save_shortest_dis("delta_stepping_dis.txt");
    save_shortest_pre("delta_stepping_pre.txt");
    cout << endl;
    // ****************************************************************************************************

    // ******************************************* RRDP ****************************************************
    start_time = clock();
    for (int i_repeat = 0; i_repeat < g_repeat_time; i_repeat++) {
        rrdp(g_shortest_dis, g_parent);
    }
    end_time = clock();
    total_time3 = get_time(start_time, end_time);

    cout << "RRDP calculate source vertex : ("
         << g_src_vtx_row << ", " << g_src_vtx_col << ")" << endl
         << "Repeat Time : " << g_repeat_time << endl
         << "The total time is: " << total_time3 << " seconds" << endl;

    save_shortest_dis("rrdp_dis.txt");
    save_shortest_pre("rrdp_pre.txt");
    cout << endl;
    // ****************************************************************************************************

    record_time();

    if (compare_all()) {
        cout << setw(30) << setfill('*') << "SAME!" << setw(25) << setfill('*') << "" << endl;
    } else {
        cout << setw(30) << setfill('*') << "DIFFERENT!" << setw(20) << setfill('*') << "" << endl;
    }

    cout << "Heap Dijkstra time:  " << total_time1 << endl;
    cout << "Delta-Stepping time: " << total_time2 << endl;
    cout << "RRDP time:           " << total_time3 << endl;
    cout << "Ratio (Heap Dijkstra / RRDP): " << (total_time1 / total_time3) << endl;
    cout << "Ratio (Delta-Stepping / RRDP): " << (total_time2 / total_time3) << endl;
    destroy_memory();
    return 1;
}