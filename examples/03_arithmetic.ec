# ==============================================
# EC 範例 03: 算術運算
# Example 03: Arithmetic Operations
# ==============================================

OUT "=== Arithmetic Operations Demo ==="
OUT ""

EC x 100
OUT "Initial x = " + x

OUT ""
OUT "--- ADD Operation ---"
ADD x 50
OUT "After ADD x 50: x = " + x

OUT ""
OUT "--- SUB Operation ---"
SUB x 30
OUT "After SUB x 30: x = " + x

OUT ""
OUT "--- MUL Operation ---"
MUL x 2
OUT "After MUL x 2: x = " + x

OUT ""
OUT "--- DIV Operation ---"
DIV x 4
OUT "After DIV x 4: x = " + x

OUT ""
OUT "--- MOD Operation ---"
EC y 17
OUT "y = " + y
MOD y 5
OUT "After MOD y 5: y = " + y

OUT ""
OUT "--- Expression Calculation ---"
EC a 10
EC b 3
EC result (a + b) * 2
OUT "(" + a + " + " + b + ") * 2 = " + result

EC c a * b + 5
OUT a + " * " + b + " + 5 = " + c

END
