# EC-Language / EC 程式語言 (Elegant Code)
**EC (Elegant Code)** is a next-generation minimalist interpreted programming language. The design goal is to provide complete programming functionality with a core instruction set of only 30 commands.
```
    ███████╗ ██████╗
    ██╔════╝██╔════╝
    █████╗  ██║     
    ██╔══╝  ██║     
    ███████╗╚██████╗
    ╚══════╝ ╚═════╝
    Next-Gen Minimalist Language
```

**版本**: 1.1.0  
**授權**: MIT License  
**平台**: Windows / Linux / macOS

---

## 目錄

1. [簡介](#簡介)
2. [安裝與編譯](#安裝與編譯)
3. [快速開始](#快速開始)
4. [語法規範](#語法規範)
5. [完整指令集](#完整指令集)
6. [進階功能](#進階功能)
7. [範例程式](#範例程式)

---

## 簡介

**EC (Elegant Code)** 是一款次世代極簡主義直譯式程式語言。設計目標是在保持語言功能完整性的前提下，將核心指令集精簡至 30 個以內。

### 設計哲學

- **極簡**: 30 個核心指令涵蓋完整程式開發需求
- **統一**: 使用 `EC` 關鍵字宣告所有變數，動態型別推導
- **彈性**: 支援物件導向 (OOP) 與程序式程式設計
- **互通**: 內建 C 語言與 Python 程式碼執行能力

### 適用場景

- 教學用途：語法簡單，易於理解程式設計概念
- 快速原型：快速驗證演算法邏輯
- 腳本自動化：批次處理與系統管理
- 跨語言整合：作為 C/Python 程式的膠水語言

---

## 安裝與編譯

### 需求

- GCC 編譯器 (Windows: MinGW / Linux: gcc)
- Python 3.x (選用，用於 PYRUN 指令)

### Windows 編譯

```batch
cd src
gcc -o EC.exe ec.c -lm
move EC.exe ..
```

### Linux / macOS 編譯

```bash
cd src
gcc -o EC ec.c -lm
chmod +x EC
mv EC ..
```

### 驗證安裝

```bash
./EC --version
# 輸出: EC Language Interpreter v1.1.0
```

---

## 快速開始

### Hello World

建立檔案 `hello.ec`:

```ec
# 我的第一個 EC 程式
OUT "Hello, World!"
END
```

執行：

```bash
./EC hello.ec
```

### 基本變數操作

```ec
# 宣告數字變數
EC age 25
EC pi 3.14159

# 宣告字串變數
EC name "Alice"

# 修改變數值
SET age 26
SET name "Bob"

# 輸出
OUT "Name: " + name
OUT "Age: " + age

END
```

---

## 語法規範

### 註解

```ec
# 這是單行註解
// 這也是單行註解
EC x 10  # 行尾註解
```

### 變數命名規則

- 以字母或底線開頭
- 可包含字母、數字、底線
- 區分大小寫
- 不可使用保留字

### 資料型別

| 型別 | 說明 | 範例 |
|------|------|------|
| 數字 | 整數或浮點數 | `42`, `3.14`, `-100` |
| 字串 | 雙引號包裹 | `"Hello"`, `"EC"` |
| 陣列 | 固定大小數字陣列 | `ARR data 10` |
| 物件 | 類別實例 | `NEW obj MyClass` |

### 運算子

| 類型 | 運算子 |
|------|--------|
| 算術 | `+` `-` `*` `/` `%` |
| 比較 | `==` `!=` `>` `<` `>=` `<=` |

---

## 完整指令集

EC 語言共有 **30 個指令**，分為 7 大類別：

### 1. 變數操作 (3 個)

#### EC - 宣告變數

```ec
EC name                  # 宣告空變數
EC count 0               # 宣告數字變數
EC message "Hello"       # 宣告字串變數
EC result a + b * 2      # 宣告並計算表達式
```

#### SET - 設定變數值

```ec
EC x 10
SET x 20                 # x 變為 20
SET x x + 5              # x 變為 25
SET name "NewName"       # 設定字串
```

#### ARR - 宣告陣列

```ec
ARR numbers 10           # 宣告大小為 10 的陣列
SET numbers[0] 100       # 設定第一個元素
SET numbers[1] 200       # 設定第二個元素
EC val numbers[0]        # 讀取陣列元素
```

---

### 2. 算術運算 (5 個)

#### ADD - 加法

```ec
EC x 10
ADD x 5                  # x = x + 5 = 15
ADD x y                  # x = x + y
```

#### SUB - 減法

```ec
EC x 10
SUB x 3                  # x = x - 3 = 7
```

#### MUL - 乘法

```ec
EC x 5
MUL x 4                  # x = x * 4 = 20
```

#### DIV - 除法

```ec
EC x 20
DIV x 4                  # x = x / 4 = 5
```

#### MOD - 取餘數

```ec
EC x 17
MOD x 5                  # x = x % 5 = 2
```

---

### 3. 輸入輸出 (2 個)

#### OUT - 輸出

```ec
OUT "Hello, World!"              # 輸出字串
OUT 42                           # 輸出數字
OUT name                         # 輸出變數
OUT "Sum = " + a + b             # 字串連接
OUT "Result: " + (x * 2 + 1)     # 輸出表達式
```

#### IN - 輸入

```ec
IN name "請輸入姓名: "           # 帶提示輸入
IN age                           # 無提示輸入
# 自動判斷輸入為數字或字串
```

---

### 4. 條件判斷 (4 個)

#### IF / ELIF / ELSE / ENDIF

```ec
EC score 85

IF score >= 90
    OUT "等級: A"
ELIF score >= 80
    OUT "等級: B"
ELIF score >= 70
    OUT "等級: C"
ELSE
    OUT "等級: F"
ENDIF
```

支援的條件運算子：

| 運算子 | 說明 |
|--------|------|
| `==` | 等於 |
| `!=` | 不等於 |
| `>` | 大於 |
| `<` | 小於 |
| `>=` | 大於等於 |
| `<=` | 小於等於 |

---

### 5. 迴圈控制 (4 個)

#### LOOP / ENDLOOP

```ec
# 無限迴圈 + BREAK
EC i 0
LOOP
    OUT i
    ADD i 1
    IF i >= 5
        BREAK
    ENDIF
ENDLOOP

# 條件迴圈
EC count 10
LOOP count > 0
    OUT count
    SUB count 1
ENDLOOP
```

#### BREAK - 跳出迴圈

```ec
EC i 0
LOOP
    IF i == 5
        BREAK           # 當 i=5 時跳出迴圈
    ENDIF
    OUT i
    ADD i 1
ENDLOOP
```

#### CONTINUE - 繼續下一輪

```ec
EC i 0
LOOP i < 10
    ADD i 1
    IF i == 5
        CONTINUE        # 跳過 5
    ENDIF
    OUT i
ENDLOOP
```

---

### 6. 函數 (4 個)

#### FN / ENDFN - 定義函數

```ec
FN greet(name)
    OUT "Hello, " + name + "!"
ENDFN

FN add(a, b)
    EC sum a + b
    RET sum
ENDFN
```

#### CALL - 呼叫函數

```ec
CALL greet("World")
CALL add(10, 20)
```

#### RET - 返回值

```ec
FN factorial(n)
    IF n <= 1
        RET 1
    ENDIF
    EC result 1
    EC i 2
    LOOP i <= n
        MUL result i
        ADD i 1
    ENDLOOP
    RET result
ENDFN
```

---

### 7. 物件導向 (4 個)

#### CLASS / ENDCLASS - 定義類別

```ec
CLASS Person
    EC name
    EC age
    
    FN introduce()
        OUT "I am " + name
    ENDFN
ENDCLASS
```

#### NEW - 建立物件

```ec
NEW person Person
SET person.name "Alice"
SET person.age 25
```

#### THIS - 當前物件參考

```ec
CLASS Counter
    EC value 0
    
    FN increment()
        ADD THIS.value 1
    ENDFN
ENDCLASS
```

---

### 8. 外部執行 (3 個)

#### EXEC - 執行系統命令

```ec
EXEC "echo Hello from shell" result
OUT "Shell output: " + result

EXEC "dir" files
OUT files
```

#### PYRUN - 執行 Python

```ec
# 執行 Python 腳本
PYRUN "script.py" output

# 呼叫 Python 函數
PYRUN "math_utils.py" calculate(10, 20) result
OUT "Python result: " + result
```

#### CRUN - 編譯執行 C 程式

```ec
# 編譯並執行 C 原始碼
CRUN "helper.c" result
OUT "C output: " + result
```

---

### 9. 程式控制 (1 個)

#### END - 結束程式

```ec
OUT "Program finished"
END
```

---

## 進階功能

### 遞迴函數

EC 支援遞迴呼叫：

```ec
FN fibonacci(n)
    IF n <= 1
        RET n
    ENDIF
    CALL fibonacci(n - 1)
    EC a return_value
    CALL fibonacci(n - 2)
    EC b return_value
    RET a + b
ENDFN
```

### 巢狀迴圈

```ec
EC i 1
LOOP i <= 9
    EC j 1
    LOOP j <= 9
        EC result i * j
        OUT i + " x " + j + " = " + result
        ADD j 1
    ENDLOOP
    ADD i 1
ENDLOOP
```

### C/Python 互操作

詳見 `examples/advanced/` 目錄中的範例。

---

## 範例程式

### 目錄結構

```
examples/
├── 01_hello.ec           # Hello World
├── 02_variables.ec       # 變數操作
├── 03_arithmetic.ec      # 算術運算
├── 04_conditions.ec      # 條件判斷
├── 05_loops.ec           # 迴圈
├── 06_arrays.ec          # 陣列
├── 07_functions.ec       # 函數
├── 08_classes.ec         # 物件導向
├── 09_multiplication.ec  # 九九乘法表
├── 10_guessing_game.ec   # 猜數字遊戲
└── advanced/
    ├── recursion.ec      # 遞迴範例
    ├── python_call.ec    # Python 呼叫
    ├── python_helper.py  # Python 輔助
    ├── c_call.ec         # C 呼叫
    └── c_helper.c        # C 輔助
```

---

## 錯誤處理

EC 直譯器提供行號錯誤訊息：

```
Error line 15: Unknown command 'PIRNT'
Error line 23: Division by zero
Error line 8: Function 'calc' not found
Error line 12: Class 'Person' not found
```

---

## 開發資訊

- **語言**: C (C99)
- **原始碼**: `src/ec.c`
- **編譯器**: GCC 4.8+
- **相依**: 標準 C 函式庫, math (-lm)

---

## 貢獻指南

歡迎提交 Issue 與 Pull Request。

## 授權

MIT License - 自由使用、修改、分發。
