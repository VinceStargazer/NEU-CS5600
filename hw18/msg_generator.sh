#!/bin/bash

# Check if the 'main' program exists
if [ ! -x "./main" ]; then
    echo "Error: 'main' program not found or not executable"
    exit 1
fi

# Loop 1000 times
for ((i = 0; i < 1000; i++)); do
    # Run the main program and pass multiple lines of input
    ./main <<EOF
c
Jason
Tom
Copy that
1
q
EOF

done
