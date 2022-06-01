@echo off
pros v5 stop
pros v5 status
echo ===== Building reset run =====
make reset

echo ===== Building left side auto winpoint run =====
make left_side_winpoint
echo ===== Building right side auto winpoint =====
make right_side_winpoint

::echo ===== Building middle left goal run =====
::make middle_left_goal
::echo ===== Building middle right goal run =====
::make middle_right_goal

echo ===== Building recording run =====
make record_match
echo ===== Building replay run =====
make replay_match
