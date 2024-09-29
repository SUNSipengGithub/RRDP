import os
import sys
import random

def print_help():
    """Print help information for command line arguments."""
    print("Usage: python create_configPrun_files.py [Options]")
    print("Options:")
    print("\t-dim: The dimension of the graph (default: 50)")
    print("\t-instances: The number of instances to be tested (default: 1000)") 
    print("\t-repeat: The number of times each algorithm runs on each instance (default: 1000)")
    print("\t-mode: The mode of instance path (default: 0). 0 for general instances and 1 for special instances")
    print("\t-ConfigDir: Directory for config files (default: Config/[dim])")
    print("\t-InsDir: Base directory for instance files (default: ./General(or Special)_Instances/[dim])")
    print("\t-h: Display this help and exit")
    sys.exit(0)

def parse_arguments():
    """Parse command line arguments."""
    # Default values
    dim = 50  # Graph dimension
    n_instances = 1000 # Number of instances
    repeat_time = 1000  # Repeat count
    mode = 0  # Instance mode
    config_dir = f""  # Config directory
    ins_dir = f""  # Instance directory

    # Check for help flag
    if '-h' in sys.argv:
        print_help()  # Show help and exit

    # Traverse command line arguments
    i = 1  # Start from the first argument
    while i < len(sys.argv):
        if sys.argv[i] == '-dim':
            if i + 1 < len(sys.argv): 
                try:
                    dim = int(sys.argv[i + 1])
                except ValueError:
                    print("Error: -dim must be an integer.")  
                    sys.exit(1)
            else:
                print("Error: -dim requires a value.")  
                sys.exit(1)
        elif sys.argv[i] == '-instances':
            if i + 1 < len(sys.argv): 
                try:
                    n_instances = int(sys.argv[i + 1])
                except ValueError:
                    print("Error: -instances must be an integer.")  
                    sys.exit(1)
            else:
                print("Error: -instances requires a value.")  
                sys.exit(1)
        elif sys.argv[i] == '-repeat':
            if i + 1 < len(sys.argv): 
                try:
                    repeat_time = int(sys.argv[i + 1])
                except ValueError:
                    print("Error: -repeat must be an integer.")  
                    sys.exit(1)
            else:
                print("Error: -repeat requires a value.")  
                sys.exit(1)
        elif sys.argv[i] == '-mode':
            if i + 1 < len(sys.argv): 
                try:
                    mode = int(sys.argv[i + 1])  # Parse mode
                except ValueError:
                    print("Error: -mode must be an integer.")  
                    sys.exit(1)
            else:
                print("Error: -mode requires a value.")  
                sys.exit(1)
        elif sys.argv[i] == '-ConfigDir':
            if i + 1 < len(sys.argv):
                config_dir = sys.argv[i + 1]  
            else:
                print("Error: -ConfigDir requires a value.")  
                sys.exit(1)
        elif sys.argv[i] == '-InsDir':
            if i + 1 < len(sys.argv): 
                ins_dir = sys.argv[i + 1]  # Update instance directory
            else:
                print("Error: -InsDir requires a value.")  
                sys.exit(1)
        else:
            print(f"Error: Unknown argument '{sys.argv[i]}'.")  # Unknown argument error
            print_help()  # Show help and exit

        i += 2  # Move to next argument


    # Update config directory if not set
    if config_dir == '':
        config_dir = f"Config/{dim}"

    # Update instance directory based on mode if not set
    if ins_dir == '':
        if mode == 0:
            ins_dir = f"./General_Instances/{dim}"
        else:
            ins_dir = f"./Special_Instances/{dim}"
    return dim, n_instances, repeat_time, mode, config_dir, ins_dir

def create_config_files(dim, n_instances, repeat_time, mode, config_dir, ins_dir):
    """Create configuration files in the specified directory."""
    os.makedirs(config_dir, exist_ok=True)  # Create config directory if it doesn't exist

    # Loop to generate configuration files
    for i in range(1, n_instances + 1):
        file_path = f"{config_dir}/config{i}.txt"  # Path for the config file
        with open(file_path, "w") as f:  # Open file for writing
            source_vertex_row = random.randint(0, dim - 1)  # Random row for source vertex
            source_vertex_column = random.randint(0, dim - 1)  # Random column for source vertex
            
            # Write configuration parameters
            f.write(f"INSTANCE_PATH:   {ins_dir}/instance{i}.txt\n")
            f.write(f"DIMENSION:   {dim}\n")
            f.write(f"SOURCE_VERTEX_ROW:  {source_vertex_row}\n")
            f.write(f"SOURCE_VERTEX_COLUMN:  {source_vertex_column}\n")
            f.write(f"REPEAT_TIME: {repeat_time}\n")

        print(f"Generated {file_path}")

if __name__ == "__main__":
    random.seed()
    dim, n_instances, repeat_time, mode, config_dir, ins_dir = parse_arguments()  # Parse arguments
    create_config_files(dim, n_instances, repeat_time, mode, config_dir, ins_dir)  # Create config files
    print("Config files generation completed.")

    file_path = "run.sh"
    with open(file_path, "a") as f:
        # Loop to generate configuration file paths for each instance
        for i in range(n_instances):
            config_file_path = f"{config_dir}/config{i + 1}.txt"
            f.write(f"./RRDP {config_file_path}\n")
    print("Run file generation completed.")
