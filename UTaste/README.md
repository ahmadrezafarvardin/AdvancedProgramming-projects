
# UTaste: Restaurant Reservation System

## Project Overview
UTaste is a full-stack web application for restaurant reservations, featuring a C++ backend with a web interface and support for managing restaurants, districts, and user reservations.

## Prerequisites

### Dependencies
- **Backend**:
  - C++ compiler (supporting C++11 or later)
  - [httplib](https://github.com/yhirose/cpp-httplib) (Header-only HTTP/HTTPS library)
  - Make

### Installation

1. Install httplib:
   - This project uses the header-only version of httplib
   - No separate installation required if `httplib.h` is already in your project

2. Compile the project:
```bash
make
```

## Project Structure
```
.
├── frontend/             # Web interface
│   ├── css/              # Stylesheets
│   ├── html/             # HTML pages
│   └── js/               # JavaScript files
├── src/                  # Backend source code
│   ├── communication/    # Server and communication logic
│   ├── dataLoader/       # Data loading mechanisms
│   ├── exceptions/       # Custom exception handling
│   ├── systemCore/       # Core system logic
│   └── main.cpp          # Application entry point
├── tests/                # Test cases and CSV data
│   ├── csv/              # CSV data files
│   └── test cases/       # Input and output test files
└── Makefile              # Build configuration
```

## Features
- User registration and authentication
- Restaurant details browsing
- Table reservation
- Budget management
- Discount system

## Running the Application
1. Compile and run the project:
```bash
make run
```
2. Open `http://localhost:5000/` in a web browser

## Data Sources
The application reads data from CSV files located in `tests/csv/`:
- `districts.csv`: District information
- `restaurants.csv`: Restaurant details
- `discounts.csv`: Available discounts

## Technologies
- **Backend**: C++
- **Frontend**: HTML, CSS, JavaScript
- **HTTP Library**: cpp-httplib
- **Build System**: Make

## Contributing
1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## Contact
ahmadreza.farvardin@gmail.com
