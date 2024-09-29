#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <ctime>

using namespace std;

/**
 * @brief Prints the usage instructions for the program.
 */
void print_help() {
    cout << "Usage: ./GENERATE [Options]" << endl;
    cout << "Options:" << endl;
    cout << "\t-dim: The dimension of the graph (default: 50)" << endl;
    cout << "\t-instances: The number of instances to be tested (default: 1000)" << endl;
    cout << "\t-mode: The mode of instance path (default: 0). 0 for general instances and 1 for special instances" << endl;
    cout << "\t-InsDir: Base directory for instance files (default: ./General(or Special)_Instances/[dim])" << endl;
    cout << "\t-h: Display this help and exit" << endl;
    exit(EXIT_FAILURE);
}

/**
 * @brief Parses command line arguments to set the parameters for the program.
 * 
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @param dim Reference to the dimension variable.
 * @param n_instances Reference to the number of instances variable.
 * @param mode Reference to the mode variable.
 * @param insDir Reference to the instance directory variable.
 */
void parse_arguments(int argc, char* argv[], int& dim, int& n_instances, int& mode, string& insDir) {
    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) { // Check for help flag first
            print_help();
        }

        if (strcmp(argv[i], "-dim") == 0) {
            if (i + 1 < argc) {
                dim = atoi(argv[++i]);
            } else {
                cerr << "Error: -dim requires a value." << endl;
                print_help();
            }
        } else if (strcmp(argv[i], "-instances") == 0) {
            if (i + 1 < argc) {
                n_instances = atoi(argv[++i]);
            } else {
                cerr << "Error: -instances requires a value." << endl;
                print_help();
            }
        } else if (strcmp(argv[i], "-mode") == 0) {
            if (i + 1 < argc) {
                mode = atoi(argv[++i]);
            } else {
                cerr << "Error: -mode requires a value." << endl;
                print_help();
            }
        } else if (strcmp(argv[i], "-InsDir") == 0) {
            if (i + 1 < argc) {
                insDir = string(argv[++i]);
            } else {
                cerr << "Error: -InsDir requires a value." << endl;
                print_help();
            }
        } else {
            cerr << "Error: Unknown argument '" << argv[i] << "'." << endl;
            print_help();
        }
    }

    // Update instance directory based on mode if not set
    if (insDir.length() == 0) {
        if (mode == 0) insDir = "./General_Instances/" + to_string(dim);
        else insDir = "./Special_Instances/" + to_string(dim);
    }
}

/**
 * @brief This program generates instances of a 2D Multi-Network based on the specified 
 * dimensions, number of instances, and generation mode.
 */
int main(int argc, char* argv[]) {
    int dim = 50;  // Graph dimension
    int n_instances = 1000;  // Number of instances
    int mode = 0;  // Instance mode (0 for general, otherwise for uniform edge weight)
    string insDir = "";  // Instance directory
    parse_arguments(argc, argv, dim, n_instances, mode, insDir);

    srand((unsigned)time(0)); // Seed the random number generator with current time

    // Initialize basic variables
    int n_vertices = dim * dim; // Total number of vertices in the graph
    int** arcs = new int* [n_vertices]; // Allocate memory for the adjacency matrix
    for (int i = 0; i < n_vertices; i++)
        arcs[i] = new int[n_vertices];

    // Begin generating instances
    for (int ite = 0; ite < n_instances; ++ite) {
        string ofile_name = insDir + "/instance" + to_string(ite + 1) + ".txt"; // Create the output file name
        ofstream ofile(ofile_name.c_str()); // Open the output file for writing

        int herizon_edge_len = rand() % 10000 + 1; // Generate a random edge length for special mode
        for (int i = 0; i < n_vertices; i++)     // Initialize the arcs (adjacency matrix) to zero
            memset(arcs[i], 0, sizeof(arcs[i]));

        ofile << dim << endl << endl; // Write the dimension to the file
        for (int i = 0; i < n_vertices; i++) {
            ofile << i << " "; // Output the current vertex index
            
            // Upper neighbor
            if (i >= dim) { 
                ofile << i - dim << " " << arcs[i][i - dim] << " ";
            }
            // Below neighbor
            if (i + dim < n_vertices) {
                arcs[i][i + dim] = arcs[i + dim][i] = rand() % 10000 + 1; // Assign random weight
                ofile << i + dim << " " << arcs[i][i + dim] << " ";
            }
            // Left neighbor
            if (i % dim) {
                ofile << i - 1 << " " << arcs[i][i - 1] << " ";
            }
            // Right neighbor
            if (i % dim + 1 != dim) {
                // Assign edge weights based on mode
                if (!mode) 
                    arcs[i][i + 1] = arcs[i + 1][i] = rand() % 10000 + 1; // Random weight for general mode
                else 
                    arcs[i][i + 1] = arcs[i + 1][i] = herizon_edge_len; // Uniform weight for special mode
                
                ofile << i + 1 << " " << arcs[i][i + 1] << " ";
            }

            ofile << endl;
        }
        ofile.close(); // Close the output file
    }

    // Free allocated memory
    for (int i = 0; i < n_vertices; i++)    
        delete[] arcs[i];
    delete[] arcs;
}
