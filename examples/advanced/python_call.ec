# ==============================================
# EC 進階範例: 呼叫 Python 程式
# Advanced Example: Python Integration
# ==============================================

OUT "=== Python Integration Demo ==="
OUT ""

OUT "This example demonstrates calling Python from EC."
OUT "Make sure Python 3 is installed and in PATH."
OUT ""

# 執行簡單 Python 指令
OUT "--- Execute Python command ---"
EXEC "python -c \"print('Hello from Python!')\"" py_result
OUT "Python says: " + py_result

OUT ""

# 執行 Python 腳本
OUT "--- Run Python Script ---"
PYRUN "python_helper.py" script_output
OUT "Script output: " + script_output

OUT ""

# 呼叫 Python 函數計算
OUT "--- Call Python Functions ---"

# 計算平方根
EXEC "python -c \"import math; print(math.sqrt(2))\"" sqrt_result
OUT "sqrt(2) = " + sqrt_result

# 計算 sin
EXEC "python -c \"import math; print(math.sin(math.pi/4))\"" sin_result
OUT "sin(pi/4) = " + sin_result

# 計算 cos
EXEC "python -c \"import math; print(math.cos(0))\"" cos_result
OUT "cos(0) = " + cos_result

OUT ""

# 使用 Python 處理字串
OUT "--- Python String Processing ---"
EXEC "python -c \"print('hello world'.upper())\"" upper_result
OUT "Upper case: " + upper_result

EXEC "python -c \"print('Hello World'.replace(' ', '_'))\"" replace_result
OUT "Replace spaces: " + replace_result

OUT ""

# 使用 Python 產生隨機數
OUT "--- Python Random Numbers ---"
EXEC "python -c \"import random; print(random.randint(1, 100))\"" rand1
OUT "Random number 1: " + rand1

EXEC "python -c \"import random; print(random.randint(1, 100))\"" rand2
OUT "Random number 2: " + rand2

OUT ""

# 使用 Python 取得系統資訊
OUT "--- Python System Info ---"
EXEC "python -c \"import sys; print(sys.version.split()[0])\"" py_version
OUT "Python version: " + py_version

EXEC "python -c \"import platform; print(platform.system())\"" os_name
OUT "Operating System: " + os_name

OUT ""
OUT "=== Python Integration Demo Complete ==="

END
