# Recursive Projects Collection

## Project Overview
A collection of recursive programming challenges, demonstrating various problem-solving techniques using recursive algorithms across multiple scenarios.

## Project Structure
```
.
├── Q1-610301221.cpp     # First recursive problem
├── Q2-610301221.cpp     # Second recursive problem
├── Q3-610301221.cpp     # Third recursive problem
├── test/                # Test input and output files
│   ├── 01/              # Test case 1
│   ├── 02/              # Test case 2
│   ├── ...              # More test cases
└── test_script.sh       # Test automation script
```

## Prerequisites
- C++ compiler (supporting C++11 or later)
- Make

## Installation
1. Clone the repository
2. Compile the programs

## Compilation
```bash
# Compile individual programs
g++ Q1-610301221.cpp -o Q1
g++ Q2-610301221.cpp -o Q2
g++ Q3-610301221.cpp -o Q3

# Or use Make
make
```

## Programs Description

## Running the Programs
```bash
# Run individual programs
./Q1 < test/01/01.in
./Q2 < test/02/02.in
./Q3 < test/03/03.in
```

## Testing
A test script is provided to automate testing:
```bash
./test_script.sh
```

## Recursive Programming Techniques Demonstrated
- Base case handling
- Recursive case decomposition
- Problem reduction
- Recursive call management

## Build Configuration
Makefile commands:
- `make`: Compile all programs
- `make clean`: Remove compiled executables
- `make test`: Run test cases

## Troubleshooting
- Ensure C++ compiler is up to date
- Check input file formats
- Verify recursive logic implementation

## Technologies
- **Language**: C++
- **Build System**: Make

## Contributing
1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## Contact
- **Email**: ahmadreza.farvardin@gmail.com

## Learning Resources
- Recursive algorithm design
- Divide and conquer strategies
- Recursive problem-solving techniques

## Limitations and Considerations
- Recursive solutions may have higher memory overhead
- Deep recursion can lead to stack overflow
- Some problems are more efficiently solved iteratively

## Future Improvements
- Add more diverse recursive problems
- Implement iterative alternatives
- Enhance error handling and input validation
