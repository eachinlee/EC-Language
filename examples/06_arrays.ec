# ==============================================
# EC 範例 06: 陣列
# Example 06: Arrays
# ==============================================

OUT "=== Array Operations Demo ==="
OUT ""

# 宣告陣列
OUT "--- Creating Array ---"
ARR numbers 5
OUT "Created array 'numbers' with size 5"

OUT ""

# 設定陣列元素
OUT "--- Setting Array Elements ---"
SET numbers[0] 10
SET numbers[1] 20
SET numbers[2] 30
SET numbers[3] 40
SET numbers[4] 50

OUT "numbers[0] = 10"
OUT "numbers[1] = 20"
OUT "numbers[2] = 30"
OUT "numbers[3] = 40"
OUT "numbers[4] = 50"

OUT ""

# 讀取陣列元素
OUT "--- Reading Array Elements ---"
EC val0 numbers[0]
EC val2 numbers[2]
EC val4 numbers[4]

OUT "Read numbers[0]: " + val0
OUT "Read numbers[2]: " + val2
OUT "Read numbers[4]: " + val4

OUT ""

# 使用迴圈遍歷陣列
OUT "--- Iterating Array with Loop ---"
EC i 0
LOOP i < 5
    OUT "numbers[" + i + "] = " + numbers[i]
    ADD i 1
ENDLOOP

OUT ""

# 計算陣列總和
OUT "--- Calculate Array Sum ---"
EC sum 0
EC idx 0
LOOP idx < 5
    ADD sum numbers[idx]
    ADD idx 1
ENDLOOP
OUT "Sum of all elements: " + sum

OUT ""

# 尋找最大值
OUT "--- Find Maximum Value ---"
EC max numbers[0]
EC k 1
LOOP k < 5
    IF numbers[k] > max
        SET max numbers[k]
    ENDIF
    ADD k 1
ENDLOOP
OUT "Maximum value: " + max

OUT ""

# 使用表達式作為索引
OUT "--- Dynamic Index ---"
EC index 2
EC dynamic_val numbers[index]
OUT "numbers[" + index + "] = " + dynamic_val

EC calculated_index 1 + 2
EC another_val numbers[calculated_index]
OUT "numbers[1+2] = numbers[3] = " + another_val

END
