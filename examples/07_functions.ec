# ==============================================
# EC 範例 07: 函數
# Example 07: Functions
# ==============================================

OUT "=== Function Demo ==="
OUT ""

# 定義簡單函數
FN sayHello()
    OUT "Hello from function!"
ENDFN

FN greet(name)
    OUT "Hello, " + name + "!"
ENDFN

FN greetWithAge(name, age)
    OUT name + " is " + age + " years old."
ENDFN

# 呼叫函數
OUT "--- Simple Function Calls ---"
CALL sayHello()
CALL greet("Alice")
CALL greet("Bob")
CALL greetWithAge("Charlie", 25)

OUT ""

# 帶返回值的函數
OUT "--- Functions with Return Values ---"

FN add(a, b)
    EC result a + b
    RET result
ENDFN

FN multiply(x, y)
    EC result x * y
    RET result
ENDFN

FN square(n)
    EC result n * n
    RET result
ENDFN

CALL add(10, 20)
OUT "add(10, 20) = 30"

CALL multiply(5, 6)
OUT "multiply(5, 6) = 30"

CALL square(7)
OUT "square(7) = 49"

OUT ""

# 階乘函數
OUT "--- Factorial Function ---"

FN factorial(n)
    IF n <= 1
        RET 1
    ENDIF
    
    EC result 1
    EC i 1
    LOOP i <= n
        MUL result i
        ADD i 1
    ENDLOOP
    RET result
ENDFN

CALL factorial(5)
OUT "factorial(5) = 120"

CALL factorial(10)
OUT "factorial(10) = 3628800"

OUT ""

# 判斷奇偶
OUT "--- Even/Odd Check ---"

FN isEven(num)
    EC temp num
    MOD temp 2
    IF temp == 0
        OUT num + " is even"
    ELSE
        OUT num + " is odd"
    ENDIF
ENDFN

CALL isEven(10)
CALL isEven(7)
CALL isEven(0)

OUT ""

# 計算總和
OUT "--- Sum Function ---"

FN sumRange(start, end)
    EC sum 0
    EC i start
    LOOP i <= end
        ADD sum i
        ADD i 1
    ENDLOOP
    OUT "Sum from " + start + " to " + end + " = " + sum
    RET sum
ENDFN

CALL sumRange(1, 10)
CALL sumRange(1, 100)

END
