# ==============================================
# EC 範例 09: 九九乘法表
# Example 09: Multiplication Table
# ==============================================

OUT "=========================================="
OUT "        九九乘法表 / Multiplication Table"
OUT "=========================================="
OUT ""

EC i 1
LOOP i <= 9
    EC j 1
    LOOP j <= 9
        EC result i * j
        OUT i + " x " + j + " = " + result
        ADD j 1
    ENDLOOP
    OUT "---"
    ADD i 1
ENDLOOP

OUT ""
OUT "=========================================="
OUT "               Complete!"
OUT "=========================================="

END
