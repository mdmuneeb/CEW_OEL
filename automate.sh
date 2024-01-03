#!/bin/bash

#compiling the OEL.c file
gcc -o OEL_output OEL.c -lcurl -ljansson

# Seting the interval in seconds (e.g., 1 hour)
interval=10

# Number of times to run the loop
num_iterations=4

#loop counter
counter=0

# Infinite loop
while [ $counter -lt $num_iterations ]
do
    #precompiled C program to fetch data and extract wind speed
    ./OEL_output

    # Printing a timestamp (optional)
    echo "API call made at $(date)"
    
    # Incrementing the counter
    ((counter++))
    
    # Exit the loop if the desired number of iterations is reached
    [ $counter -eq $num_iterations ] && break

    sleep $interval
done

#compiling the calc_mean.c file
gcc -o mean calc_mean.c
./mean

