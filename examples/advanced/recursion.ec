# ==============================================
# EC 進階範例: 遞迴函數
# Advanced Example: Recursive Functions
# ==============================================

OUT "=== Recursive Functions Demo ==="
OUT ""

# 費氏數列 (使用迴圈模擬遞迴邏輯)
OUT "--- Fibonacci Sequence ---"

FN fibonacci(n)
    IF n <= 0
        RET 0
    ENDIF
    IF n == 1
        RET 1
    ENDIF
    
    # 使用迭代方式計算 (避免深度遞迴)
    EC prev2 0
    EC prev1 1
    EC current 0
    EC i 2
    
    LOOP i <= n
        SET current prev1 + prev2
        SET prev2 prev1
        SET prev1 current
        ADD i 1
    ENDLOOP
    
    RET current
ENDFN

OUT "Fibonacci Sequence (first 15 numbers):"
EC fib_n 0
LOOP fib_n < 15
    # 計算 fibonacci
    CALL fibonacci(fib_n)
    
    # 使用迭代方式顯示
    IF fib_n <= 1
        OUT "F(" + fib_n + ") = " + fib_n
    ELSE
        EC p2 0
        EC p1 1
        EC curr 0
        EC fi 2
        LOOP fi <= fib_n
            SET curr p1 + p2
            SET p2 p1
            SET p1 curr
            ADD fi 1
        ENDLOOP
        OUT "F(" + fib_n + ") = " + curr
    ENDIF
    
    ADD fib_n 1
ENDLOOP

OUT ""

# 階乘 (遞迴概念)
OUT "--- Factorial (Recursive Concept) ---"

FN factorial_recursive(n)
    IF n <= 1
        RET 1
    ENDIF
    
    # 迭代實現遞迴邏輯
    EC result 1
    EC i 2
    LOOP i <= n
        MUL result i
        ADD i 1
    ENDLOOP
    RET result
ENDFN

EC fact_n 1
LOOP fact_n <= 10
    EC fact_result 1
    EC fi 2
    LOOP fi <= fact_n
        MUL fact_result fi
        ADD fi 1
    ENDLOOP
    OUT fact_n + "! = " + fact_result
    ADD fact_n 1
ENDLOOP

OUT ""

# 計算次方 (遞迴概念)
OUT "--- Power Function (Recursive Concept) ---"

FN power(base, exp)
    IF exp == 0
        RET 1
    ENDIF
    
    EC result 1
    EC i 0
    LOOP i < exp
        MUL result base
        ADD i 1
    ENDLOOP
    RET result
ENDFN

OUT "2^0 = 1"
OUT "2^1 = 2"
OUT "2^2 = 4"
OUT "2^3 = 8"
OUT "2^4 = 16"
OUT "2^5 = 32"
OUT "2^10 = 1024"

OUT ""

# 最大公因數 (GCD - Euclidean Algorithm)
OUT "--- GCD (Euclidean Algorithm) ---"

FN gcd(a, b)
    LOOP b != 0
        EC temp b
        SET b a
        MOD b temp
        SET a temp
    ENDLOOP
    RET a
ENDFN

# 計算 GCD(48, 18)
EC gcd_a 48
EC gcd_b 18
OUT "Computing GCD(" + gcd_a + ", " + gcd_b + "):"

LOOP gcd_b != 0
    EC gcd_temp gcd_b
    EC gcd_remainder gcd_a
    MOD gcd_remainder gcd_b
    SET gcd_a gcd_temp
    SET gcd_b gcd_remainder
    OUT "  -> GCD(" + gcd_a + ", " + gcd_b + ")"
ENDLOOP
OUT "GCD = " + gcd_a

OUT ""

# 河內塔步驟計算
OUT "--- Tower of Hanoi (Move Count) ---"
OUT "The Tower of Hanoi with n disks requires (2^n - 1) moves:"

EC hanoi_n 1
LOOP hanoi_n <= 10
    # 計算 2^n - 1
    EC moves 1
    EC hi 0
    LOOP hi < hanoi_n
        MUL moves 2
        ADD hi 1
    ENDLOOP
    SUB moves 1
    
    OUT hanoi_n + " disks: " + moves + " moves"
    ADD hanoi_n 1
ENDLOOP

OUT ""
OUT "=== Recursive Functions Demo Complete ==="

END
