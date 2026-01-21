#!/bin/bash
# EC Language Linux/macOS Build Script

echo "==================================="
echo "  EC Language Build Script"
echo "  Linux / macOS"
echo "==================================="
echo ""

# Check for GCC
if ! command -v gcc &> /dev/null; then
    echo "Error: GCC not found!"
    echo "Please install GCC:"
    echo "  Ubuntu/Debian: sudo apt install gcc"
    echo "  CentOS/RHEL:   sudo yum install gcc"
    echo "  macOS:         xcode-select --install"
    exit 1
fi

echo "Building EC interpreter..."
gcc -Wall -O2 -o EC src/ec.c -lm

if [ $? -eq 0 ]; then
    echo ""
    echo "Build successful!"
    echo ""
    chmod +x EC
    echo "Executable: ./EC"
    echo ""
    echo "Testing..."
    ./EC --version
    echo ""
    echo "Run examples:"
    echo "  ./EC examples/01_hello.ec"
    echo "  ./EC examples/09_multiplication.ec"
else
    echo ""
    echo "Build failed!"
    exit 1
fi
