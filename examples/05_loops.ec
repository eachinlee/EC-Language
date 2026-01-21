# ==============================================
# EC 範例 05: 迴圈
# Example 05: Loops
# ==============================================

OUT "=== Loop Statements Demo ==="
OUT ""

# 基本計數迴圈
OUT "--- Counting Loop (1 to 5) ---"
EC i 1
LOOP i <= 5
    OUT "Count: " + i
    ADD i 1
ENDLOOP

OUT ""

# 倒數迴圈
OUT "--- Countdown Loop (5 to 1) ---"
EC count 5
LOOP count > 0
    OUT "Countdown: " + count
    SUB count 1
ENDLOOP
OUT "Liftoff!"

OUT ""

# 無限迴圈 + BREAK
OUT "--- Infinite Loop with BREAK ---"
EC n 0
LOOP
    ADD n 1
    OUT "n = " + n
    IF n >= 3
        OUT "Breaking out of loop!"
        BREAK
    ENDIF
ENDLOOP

OUT ""

# CONTINUE 跳過
OUT "--- CONTINUE Example (skip even numbers) ---"
EC j 0
LOOP j < 10
    ADD j 1
    
    # 計算是否為偶數 (j % 2 == 0)
    EC temp j
    MOD temp 2
    IF temp == 0
        CONTINUE
    ENDIF
    
    OUT "Odd number: " + j
ENDLOOP

OUT ""

# 巢狀迴圈
OUT "--- Nested Loops (3x3 grid) ---"
EC row 1
LOOP row <= 3
    EC col 1
    LOOP col <= 3
        OUT "(" + row + ", " + col + ")"
        ADD col 1
    ENDLOOP
    ADD row 1
ENDLOOP

END
