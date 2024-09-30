# Project Overview

This project aims to solve the Single-Source Shortest Path (SSSP) problem in 2D mesh networks using our Row by Row Dynamic Programming (RRDP) algorithm.

## Files and Folders Description

- **Config**:
    - Contains configuration files for running the program.
    - Includes an example configuration file for reference. Details on configuration files can be found [here](#configuration-file-structure).

- **Experiment_Results**:
    - Contains the results obtained from the experiments.

- **Experiment_Results/General_Instances**:
    - Contains the results obtained from the experiments conducted on general instances.

- **Experiment_Results/Special_Instances**:
    - Contains the results obtained from the experiments conducted on special instances.

- **General_Instances**:
    - Contains example special instances with 100 instances for each dimension. For additional instances used in experiments, please contact us at SipengSunny@outlook.com.
    - Details on instance files can be found [here](#instance-file-structure).

- **Special_Instances**:
    - Contains example special instances where the horizontal edges have the same weight, with 100 instances for each dimension. For additional instances used in experiments, please contact us at SipengSunny@outlook.com.
    - Details on instance files can be found [here](#instance-file-structure).

- **Src**:
    - Contains all the code related to the project.

- **Src/Generate_Instance/**: 
    - Contains files and code related to instance generation.

- **Src/RRDP/**:
    - Contains code and files for implementing the RRDP algorithm and comparing it with Dijkstra's algorithm.

- **create_configPrun_files.py**:
    - A Python script used to generate configuration files and add relevant commands to the generated `run.sh` file.

- **Makefile**:
    - Defines the rules and targets for building the project.

## Usage Instructions

### Building the Project
To build the project, run the following command:
```bash
make
```
This will create two executable files: `GENERATE` and `RRDP`.

### File Usage

1. **GENERATE**:
    - To create instance files, run:
    ```bash
    ./GENERATE [options]
    ```
    Options for `GENERATE`:
    ```
    Usage: ./GENERATE [Options]
    Options:
        -dim: Dimension of the graph (default: 50)
        -instances: Number of instances to be tested (default: 1000)
        -mode: Mode of instance path (default: 0). Use 0 for general instances or 1 for special instances.
        -InsDir: Base directory for instance files (default: ./General(or Special)_Instances/[dim])
        -h: Display this help and exit
    ```
    - Each generated instance is in square shape.

2. **RRDP**:
    - To solve the SSSP problem as specified by the configuration file and compare the execution time with Dijkstra's algorithm, run:
    ```bash
    ./RRDP [config_path]
    ```
    - After execution, the following output files will be generated:
        - `dijkstra_dis.txt`: Shortest distance results of Dijkstra's algorithm
        - `dijkstra_pre.txt`: Predecessor results of Dijkstra's algorithm
        - `rrdp_dis.txt`: Shortest distance results of the RRDP algorithm
        - `rrdp_pre.txt`: Predecessor results of the RRDP algorithm
        - `dijkstra_time.txt`: Execution time of Dijkstra's algorithm
        - `rrdp_time.txt`: Execution time of the RRDP algorithm
        - `ratio_time.txt`: Ratio of execution times between the two algorithms

3. **create_configPrun_files.py**:
   - To generate configuration files and create the `run.sh` script with the corresponding commands, run:
     ```bash
     python create_configPrun_files.py [options]
     ```
    Options for `create_configPrun_files.py`:
    ```
    Usage: python create_configPrun_files.py [Options]
    Options:
        -dim: Dimension of the graph (default: 50)
        -instances: Number of instances to be tested (default: 1000)
        -repeat: Number of times each algorithm runs on each instance (default: 1000)
        -mode: Mode of instance path (default: 0). Use 0 for general instances or 1 for special instances.
        -ConfigDir: Directory for config files (default: Config/[dim])
        -InsDir: Base directory for instance files (default: ./General(or Special)_Instances/[dim])
        -h: Display this help and exit
    ```

## Instance File Structure

Each instance file includes:
- The first line specifies the dimension of the graph (e.g., `100` for a 100x100 dimension).
- Each subsequent line represents one vertex with its neighbors and corresponding weights:
    ```
    <vtx> <des_vtx> <weight> <des_vtx> <weight> /cdots
    ```
    - Where:
      - `<vtx>`: The vertex.
      - `<des_vtx>`: The endpoint of one edge whose other endpoint is `<vtx>`.
      - `<weight>`: The weight of the edge.

## Configuration File Structure

The configuration files define parameters for running the algorithm. Each configuration file includes:
- **INSTANCE_PATH**: Specifies the path to the instance file.
- **DIMENSION**: Indicates the dimension of the graph.
- **SOURCE_VERTEX_ROW**: Specifies the row index of the source vertex.
- **SOURCE_VERTEX_COLUMN**: Specifies the column index of the source vertex.
- **REPEAT_TIME**: Indicates how many times the algorithms should be executed on the specified instance.

## How to Conduct the Experiments

**Example**:
To conduct an experiment on special instances with a dimension of 100x100, follow these steps:

1. Build the project:
```bash
make
```

2. Generate instance files:
```bash
./GENERATE -dim 100 -mode 1
```

3. Generate configuration files and create `run.sh`:
```bash
python create_configPrun_files.py -dim 100 -mode 1
```

4. Run the experiments:
```bash
bash run.sh
```

Then the results would be collected in these files - `dijkstra_time.txt`,`rrdp_time.txt` and `ratio_time.txt`.
