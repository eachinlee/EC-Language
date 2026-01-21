#!/usr/bin/env python3
"""
EC Language Python Helper
This script provides utility functions for EC programs.
"""

import math
import random
import sys

def add(a, b):
    """Add two numbers"""
    return a + b

def multiply(a, b):
    """Multiply two numbers"""
    return a * b

def factorial(n):
    """Calculate factorial"""
    if n <= 1:
        return 1
    result = 1
    for i in range(2, n + 1):
        result *= i
    return result

def fibonacci(n):
    """Calculate nth Fibonacci number"""
    if n <= 1:
        return n
    a, b = 0, 1
    for _ in range(2, n + 1):
        a, b = b, a + b
    return b

def is_prime(n):
    """Check if number is prime"""
    if n < 2:
        return False
    for i in range(2, int(math.sqrt(n)) + 1):
        if n % i == 0:
            return False
    return True

def generate_primes(limit):
    """Generate primes up to limit"""
    return [n for n in range(2, limit + 1) if is_prime(n)]

def calculate_statistics(numbers):
    """Calculate mean, min, max of a list"""
    if not numbers:
        return 0, 0, 0
    return sum(numbers) / len(numbers), min(numbers), max(numbers)

# Main execution when called directly
if __name__ == "__main__":
    print("EC Python Helper v1.0")
    print("Available functions:")
    print("  - add(a, b)")
    print("  - multiply(a, b)")
    print("  - factorial(n)")
    print("  - fibonacci(n)")
    print("  - is_prime(n)")
    print("  - generate_primes(limit)")
    print("")
    print("Example results:")
    print(f"  factorial(5) = {factorial(5)}")
    print(f"  fibonacci(10) = {fibonacci(10)}")
    print(f"  is_prime(17) = {is_prime(17)}")
    print(f"  Primes up to 20: {generate_primes(20)}")
