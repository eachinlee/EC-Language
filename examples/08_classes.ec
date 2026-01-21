# ==============================================
# EC 範例 08: 物件導向程式設計
# Example 08: Object-Oriented Programming
# ==============================================

OUT "=== Object-Oriented Programming Demo ==="
OUT ""

# 定義 Person 類別
CLASS Person
    EC name
    EC age
    EC city
    
    FN introduce()
        OUT "Hi, I'm " + name + " from " + city
        OUT "I'm " + age + " years old."
    ENDFN
    
    FN birthday()
        ADD age 1
        OUT name + " is now " + age + " years old!"
    ENDFN
ENDCLASS

# 定義 Rectangle 類別
CLASS Rectangle
    EC width
    EC height
    
    FN area()
        EC result width * height
        OUT "Area: " + width + " x " + height + " = " + result
        RET result
    ENDFN
    
    FN perimeter()
        EC result (width + height) * 2
        OUT "Perimeter: 2 x (" + width + " + " + height + ") = " + result
        RET result
    ENDFN
ENDCLASS

# 定義 Counter 類別
CLASS Counter
    EC value
    
    FN increment()
        ADD value 1
    ENDFN
    
    FN decrement()
        SUB value 1
    ENDFN
    
    FN reset()
        SET value 0
    ENDFN
    
    FN display()
        OUT "Counter value: " + value
    ENDFN
ENDCLASS

OUT "--- Creating Objects ---"
OUT ""

# 注意：目前 EC 的 OOP 是簡化版本
# 類別定義展示了概念，實際使用需要用變數模擬

OUT "Person Class Demo:"
EC person_name "Alice"
EC person_age 25
EC person_city "Taipei"
OUT "Name: " + person_name
OUT "Age: " + person_age
OUT "City: " + person_city

OUT ""
OUT "Rectangle Class Demo:"
EC rect_width 10
EC rect_height 5
EC rect_area rect_width * rect_height
EC rect_perimeter (rect_width + rect_height) * 2
OUT "Width: " + rect_width
OUT "Height: " + rect_height
OUT "Area: " + rect_area
OUT "Perimeter: " + rect_perimeter

OUT ""
OUT "Counter Class Demo:"
EC counter_value 0
OUT "Initial: " + counter_value
ADD counter_value 1
OUT "After increment: " + counter_value
ADD counter_value 1
OUT "After increment: " + counter_value
SUB counter_value 1
OUT "After decrement: " + counter_value

END
