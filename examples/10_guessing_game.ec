# ==============================================
# EC 範例 10: 猜數字遊戲
# Example 10: Number Guessing Game
# ==============================================

OUT "=========================================="
OUT "      猜數字遊戲 / Number Guessing Game"
OUT "=========================================="
OUT ""

# 設定目標數字 (在實際遊戲中應該是隨機的)
EC target 42
EC attempts 0
EC max_attempts 7
EC guessed 0

OUT "I'm thinking of a number between 1 and 100."
OUT "You have " + max_attempts + " attempts to guess it."
OUT ""

# 遊戲迴圈
LOOP guessed == 0
    ADD attempts 1
    
    OUT "Attempt " + attempts + " of " + max_attempts
    IN guess "Enter your guess: "
    
    IF guess == target
        SET guessed 1
        OUT ""
        OUT "*** Congratulations! ***"
        OUT "You guessed the number " + target + " in " + attempts + " attempts!"
    ELIF guess < target
        OUT "Too low! Try a higher number."
        OUT ""
    ELSE
        OUT "Too high! Try a lower number."
        OUT ""
    ENDIF
    
    IF attempts >= max_attempts
        IF guessed == 0
            OUT ""
            OUT "*** Game Over ***"
            OUT "You've used all your attempts."
            OUT "The number was: " + target
            BREAK
        ENDIF
    ENDIF
ENDLOOP

OUT ""
OUT "Thanks for playing!"
OUT "=========================================="

END
