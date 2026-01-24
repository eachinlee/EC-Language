# ==============================================
# EC 進階範例: 錯誤處理演示
# Advanced Example: Error Handling Demo
# ==============================================

OUT "=== Error Handling Demo ==="
OUT "Uncomment lines below to test error detection."
OUT ""

# 1. 靜態語法檢查示範
#    如果取消下面註解，程式會在執行前被擋下
# IF 1 == 1
#     OUT "Missing ENDIF"
# (No ENDIF here)

# 2. 函數定義與錯誤堆疊
FN dangerous_math(a, b)
    OUT "Inside dangerous_math..."
    # 這裡會發生除以零錯誤
    EC result a
    DIV result b
    RET result
ENDFN

FN safe_wrapper(x)
    OUT "Wrapper calling dangerous function..."
    CALL dangerous_math(x, 0)
ENDFN

# 測試 runtime error
OUT "--- Testing Runtime Stack Trace ---"
# 取消下面這行來測試除以零錯誤
# CALL safe_wrapper(100)

# 3. 陣列越界檢查
ARR list 3
SET list[0] 10
SET list[1] 20
SET list[2] 30

OUT "--- Testing Array Bounds ---"
# 取消下面這行來測試越界
# OUT list[5]

# 4. 變數未定義檢查
OUT "--- Testing Undefined Variable ---"
# 取消下面這行
# OUT unknown_variable

OUT "Demo complete. No errors triggered."
END
