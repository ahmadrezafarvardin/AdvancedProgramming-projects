# Judge Script README

This script is used to compile and run tests for your code, and then compare the results against expected outputs.
Please note that you have to unzip this archive at the same folder as your Makefile.
The judge will only work if it has the "tests" folder and "Makefile" file right next to it.

## Setup

1. **Place `judge.sh` in the same directory** as your `Makefile` (or your `.cpp` file if you don't have a `Makefile`).

2. **Make `judge.sh` executable**:
   - In the terminal, run the following command to make the script executable:

     ```bash
     chmod +x judge.sh
     ```

3. **Prepare your tests**:
   - Create a `tests` folder in the same directory as `judge.sh`.
   - Inside the `tests` folder, create subfolders for each test case. Each subfolder should contain:
     - An input file (e.g., `test1.in`) that provides input to your program.
     - An output file (e.g., `test1.out`) that contains the expected output for your program.
     - Optionally, a `csv` folder containing the `restaurants.csv` and `districts.csv` files for test input.

   Example directory structure:

   ```text
   judge.sh
   Makefile
   tests/
     ├── 01/
     │   ├── 01.in
     │   ├── test1.out
     ├── 02/
     │   ├── 02.in
     │   ├── 02.out
     ├── csv/
     │   ├── restaurants.csv
     │   ├── districts.csv
   ```

## How to Run Tests

1. Open a terminal.

2. **Run the tests** by executing the following command:

   ```bash
   ./judge.sh -t
   ```

3. The script will:
   - Compile your code.
   - Run each test case (using the `.in` file as input).
   - Compare the output against the expected `.out` file.
   - Print whether each test passed or failed, along with details for any failed tests.

## After Testing

- Once the script finishes running, check the output in the terminal.
  - **Passed tests** will be marked as "Accepted".
  - **Failed tests** will show "Wrong answer" along with a diff of the differences if available.

## Cleaning Up

1. **Remove test outputs and diffs** (if needed) by running:

   ```bash
   ./judge.sh -r
   ```

   This will delete any `.result` and `.diff` files generated during the tests.
