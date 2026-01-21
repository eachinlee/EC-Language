# ==============================================
# EC 範例 02: 變數操作
# Example 02: Variables
# ==============================================

OUT "--- Variable Declaration ---"

# 宣告數字變數
EC age 25
EC height 175.5
EC score 0

# 宣告字串變數
EC name "Alice"
EC city "Taipei"

OUT "Name: " + name
OUT "Age: " + age
OUT "Height: " + height
OUT "City: " + city

OUT ""
OUT "--- Variable Assignment ---"

# 修改變數值
SET age 26
SET name "Bob"
SET score 95

OUT "Updated name: " + name
OUT "Updated age: " + age
OUT "Score: " + score

OUT ""
OUT "--- Expression Assignment ---"

# 使用表達式賦值
EC a 10
EC b 20
EC sum a + b
EC product a * b

OUT "a = " + a
OUT "b = " + b
OUT "a + b = " + sum
OUT "a * b = " + product

END
