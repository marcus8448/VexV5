@echo off
pros v5 stop
pros v5 status
echo ===== Building reset run =====
pros make reset

echo ===== Building replay run =====
pros make replay

echo ===== Building left side auto winpoint run =====
pros make left_side_winpoint
echo ===== Building right side auto winpoint =====
pros make right_side_winpoint

echo ===== Building middle left goal run =====
pros make middle_left_goal
echo ===== Building middle right goal run =====
pros make middle_right_goal

echo ===== Building recording run =====
pros make record
