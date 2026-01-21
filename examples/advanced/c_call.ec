# ==============================================
# EC 進階範例: 呼叫 C 程式
# Advanced Example: C Integration
# ==============================================

OUT "=== C Integration Demo ==="
OUT ""

OUT "This example demonstrates calling C from EC."
OUT "Make sure GCC is installed and in PATH."
OUT ""

# 編譯並執行 C 程式
OUT "--- Compile and Run C Program ---"
CRUN "c_helper.c" c_output
OUT "C program output:"
OUT c_output

OUT ""

# 也可以使用 EXEC 直接呼叫已編譯的程式
OUT "--- Using EXEC for compiled programs ---"
OUT "(After CRUN compiles, you can use EXEC for faster execution)"

OUT ""

# 使用 C 進行高效能計算
OUT "--- High Performance Computing with C ---"
OUT "C is useful for:"
OUT "  - CPU-intensive calculations"
OUT "  - System-level operations"
OUT "  - Memory manipulation"
OUT "  - Performance-critical code"

OUT ""

# 計算範例
OUT "--- Calculation Examples ---"

# 使用 EXEC 呼叫 gcc 編譯執行
EC calc_source "echo '#include <stdio.h>' > temp_calc.c"
EXEC "echo #include ^<stdio.h^> > temp_calc.c && echo int main() { printf(\"Sum 1-100: %d\\n\", 5050); return 0; } >> temp_calc.c && gcc -o temp_calc temp_calc.c && temp_calc" calc_result
OUT calc_result

OUT ""
OUT "=== C Integration Demo Complete ==="
OUT ""
OUT "Note: The c_helper.c file contains reusable C functions."
OUT "You can modify it to add your own calculations."

END
