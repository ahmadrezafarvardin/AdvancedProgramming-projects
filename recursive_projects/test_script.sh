#!/bin/bash

# Compile your C++ program (assuming it compiles to Q1-610301221)
g++ Q3-610301221.cpp -o Q3-610301221

# Check if the compilation was successful
if [ $? -ne 0 ]; then
  echo "Compilation failed. Please check your C++ code."
  exit 1
fi

# Loop through directories 01 to 10
for dir in {01..10}; do
  echo "Testing in directory $dir..."

  # Construct input and output file names
  input_file="$dir/${dir}.in"
  output_file="$dir/${dir}.out"

  # Check if the input and output files exist
  if [[ -f "$input_file" && -f "$output_file" ]]; then
    # Run the program with the .in file as input and save the output
    ./Q3-610301221 < "$input_file" > "temp.out"

    # Compare the output with the expected .out file
    if diff "temp.out" "$output_file" > /dev/null; then
      echo "Test case in $dir passed."
    else
      echo "Test case in $dir FAILED."
      echo "Differences:"
      diff "temp.out" "$output_file"
    fi

    # Remove the temporary output file
    rm temp.out
  else
    echo "Input or output file missing in $dir."
  fi
done

echo "All tests completed."