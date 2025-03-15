# Leitner Box Learning System

## Project Overview
A digital implementation of the Leitner flashcard learning system, designed to help users efficiently memorize and review educational content using spaced repetition techniques.

## Prerequisites

### Dependencies
- C++ compiler (supporting C++11 or later)
- Make

### Installation

1. Compile the project:
```bash
make
```

## Project Structure
```
.
├── LeitnerBox/           # Potential output directory
├── Makefile              # Build configuration
├── src/                  # Source code
│   ├── App.cpp           # Application logic
│   ├── App.h             # Application header
│   ├── Box.cpp           # Leitner box implementation
│   ├── Box.h             # Box header
│   ├── constants.h       # Project constants
│   ├── FlashCard.cpp     # Flashcard implementation
│   ├── FlashCard.h       # Flashcard header
│   ├── main.cpp          # Program entry point
│   ├── utils.cpp         # Utility functions
│   └── utils.h           # Utility header
└── test/                 # Test cases and input/output files
```

## Features
- Spaced repetition learning system
- Flashcard management
- Multiple box levels for efficient learning
- Customizable flashcard content

## Running the Application
1. Compile the project:
```bash
make
```

2. Run the executable:
```bash
./LeitnerBox
```

## Testing
The `test/` directory contains various test cases and input/output files for verifying the application's functionality.

## Build Configuration
The Makefile provides several commands:
- `make`: Compile the project
- `make clean`: Remove compiled objects
- `make test`: Run test cases (if configured)

## Leitner System Concept
The Leitner system is a learning technique that uses spaced repetition:
- Flashcards are organized into multiple boxes/levels
- Correctly remembered cards move to higher levels
- Forgotten cards move back to lower levels
- Promotes efficient long-term memory retention

## Technologies
- **Language**: C++
- **Build System**: Make

## Troubleshooting
- Ensure you have a modern C++ compiler
- Check file permissions
- Verify input file formats

## Contributing
1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request


## Contact
- **Email**: ahmadreza.farvardin@gmail.com

## Acknowledgments
- Inspiration from Sebastian Leitner's learning method
- Open-source learning tools community

## Future Improvements
- Persistent storage of flashcards
- User interface
- Advanced statistics and learning analytics
