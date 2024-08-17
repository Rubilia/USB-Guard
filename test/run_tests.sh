#!/bin/bash

LOG_FILE="test_results.log"

echo "Running all USB Encryption Layer tests..." | tee -a $LOG_FILE

./test_write 2>&1 | tee -a $LOG_FILE
./test_read 2>&1 | tee -a $LOG_FILE
./test_error_handling 2>&1 | tee -a $LOG_FILE

echo "Tests completed. Results saved to $LOG_FILE" | tee -a $LOG_FILE
