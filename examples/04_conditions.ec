# ==============================================
# EC 範例 04: 條件判斷
# Example 04: Conditionals
# ==============================================

OUT "=== Conditional Statements Demo ==="
OUT ""

# 簡單 IF-ELSE
OUT "--- Simple IF-ELSE ---"
EC age 20

IF age >= 18
    OUT "You are an adult."
ELSE
    OUT "You are a minor."
ENDIF

OUT ""

# IF-ELIF-ELSE 多重條件
OUT "--- Grade Evaluation ---"
EC score 85

IF score >= 90
    OUT "Score: " + score + " -> Grade: A (Excellent!)"
ELIF score >= 80
    OUT "Score: " + score + " -> Grade: B (Good!)"
ELIF score >= 70
    OUT "Score: " + score + " -> Grade: C (Pass)"
ELIF score >= 60
    OUT "Score: " + score + " -> Grade: D (Need Improvement)"
ELSE
    OUT "Score: " + score + " -> Grade: F (Fail)"
ENDIF

OUT ""

# 數值比較
OUT "--- Number Comparison ---"
EC a 15
EC b 20

OUT "a = " + a + ", b = " + b

IF a == b
    OUT "a equals b"
ENDIF

IF a != b
    OUT "a is not equal to b"
ENDIF

IF a < b
    OUT "a is less than b"
ENDIF

IF a <= b
    OUT "a is less than or equal to b"
ENDIF

OUT ""

# 巢狀條件
OUT "--- Nested Conditions ---"
EC x 50

IF x > 0
    IF x > 100
        OUT "x is greater than 100"
    ELSE
        OUT "x is between 1 and 100"
    ENDIF
ELSE
    OUT "x is zero or negative"
ENDIF

END
