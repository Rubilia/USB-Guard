#!/bin/bash

echo "Running all USB Encryption Layer tests..."

./test_write
./test_read
./test_error_handling

echo "Tests completed."
