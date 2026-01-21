/*
 * EC Language C Helper
 * Provides utility functions for EC programs
 * Compile: gcc -o c_helper c_helper.c -lm
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Calculate factorial */
long long factorial(int n) {
    if (n <= 1) return 1;
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

/* Calculate nth Fibonacci number */
long long fibonacci(int n) {
    if (n <= 1) return n;
    long long a = 0, b = 1, temp;
    for (int i = 2; i <= n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

/* Check if number is prime */
int is_prime(int n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i <= sqrt(n); i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

/* Calculate GCD using Euclidean algorithm */
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/* Calculate LCM */
int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

/* Sum of array */
long long sum_array(int arr[], int size) {
    long long sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

/* Sum from 1 to n */
long long sum_range(int n) {
    return (long long)n * (n + 1) / 2;
}

/* Calculate power */
long long power(int base, int exp) {
    long long result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

int main(int argc, char* argv[]) {
    printf("EC C Helper v1.0\n");
    printf("================\n\n");
    
    printf("Function Demonstrations:\n\n");
    
    /* Factorial demos */
    printf("Factorials:\n");
    for (int i = 1; i <= 10; i++) {
        printf("  %d! = %lld\n", i, factorial(i));
    }
    printf("\n");
    
    /* Fibonacci demos */
    printf("Fibonacci Sequence (first 15):\n  ");
    for (int i = 0; i < 15; i++) {
        printf("%lld ", fibonacci(i));
    }
    printf("\n\n");
    
    /* Prime check demos */
    printf("Prime numbers up to 50:\n  ");
    for (int i = 2; i <= 50; i++) {
        if (is_prime(i)) {
            printf("%d ", i);
        }
    }
    printf("\n\n");
    
    /* GCD and LCM demos */
    printf("GCD and LCM examples:\n");
    printf("  GCD(48, 18) = %d\n", gcd(48, 18));
    printf("  LCM(48, 18) = %d\n", lcm(48, 18));
    printf("  GCD(100, 35) = %d\n", gcd(100, 35));
    printf("  LCM(100, 35) = %d\n", lcm(100, 35));
    printf("\n");
    
    /* Sum range demos */
    printf("Sum of ranges:\n");
    printf("  Sum(1-100) = %lld\n", sum_range(100));
    printf("  Sum(1-1000) = %lld\n", sum_range(1000));
    printf("\n");
    
    /* Power demos */
    printf("Power calculations:\n");
    printf("  2^10 = %lld\n", power(2, 10));
    printf("  3^5 = %lld\n", power(3, 5));
    printf("  10^6 = %lld\n", power(10, 6));
    
    return 0;
}
