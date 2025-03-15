# Parking Management System

## Project Overview
A comprehensive parking management application that processes parking-related data from CSV files, allowing interactive management of cars, plots, and expenses.

## Prerequisites

### Dependencies
- C++ compiler (supporting C++11 or later)
- Standard C++ libraries

### Input Files
The application requires three CSV input files:
1. Cars CSV: Contains information about vehicles
2. Plots CSV: Parking plot details
3. Expenses CSV: Parking-related expense information

## Installation

1. Compile the project:
```bash
g++ -std=c++11 A2-610301221.cpp -o parking_management
```

## Usage
```bash
./parking_management <cars_file.csv> <plots_file.csv> <expenses_file.csv>
```

### Example
```bash
./parking_management cars.csv plots.csv expenses.csv
```

## Project Structure
```
.
└── A2-610301221.cpp     # Main source file
```

## Features
- Read and process parking-related data from CSV files
- Interactive command-line interface
- Manage:
  - Vehicle information
  - Parking plots
  - Parking expenses

## Input File Formats

### Cars CSV
Expected columns:
- Car ID
- Car details
- Other relevant information

### Plots CSV
Expected columns:
- Plot ID
- Plot details
- Associated expenses

### Expenses CSV
Expected columns:
- Expense ID
- Expense details
- Cost

## Error Handling
- Checks for minimum required command-line arguments
- Handles empty input lines

## Build Configuration
Compile with C++11 standard:
```bash
g++ -std=c++11 A2-610301221.cpp -o parking_management
```

## Running the Application
1. Compile the program
2. Prepare CSV input files
3. Run with CSV file paths as arguments

## Example Workflow
```bash
# Compile
g++ -std=c++11 A2-610301221.cpp -o parking_management

# Run with sample CSV files
./parking_management cars.csv plots.csv expenses.csv
```

## Technologies
- **Language**: C++
- **Parsing**: CSV file processing
- **Interface**: Command-line

## Potential Improvements
- Add more robust error checking
- Implement persistent storage
- Create more complex querying capabilities

## Troubleshooting
- Ensure CSV files are correctly formatted
- Check file paths
- Verify input data consistency

## Contributing
1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## Limitations
- Relies on specific CSV file formats
- Command-line interface only
- Limited by available system memory

## Contact
- **Email**: ahmadreza.farvardin@gmail.com

## Acknowledgments
- C++ Standard Library
- CSV parsing techniques
