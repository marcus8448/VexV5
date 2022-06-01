#!/bin/bash
echo ===== Building reset run =====
pros make reset

echo ===== Building left side auto winpoint run =====
pros make left_side_winpoint
echo ===== Building right side auto winpoint =====
pros make right_side_winpoint

#echo ===== Building middle left goal run =====
#pros make middle_left_goal
#echo ===== Building middle right goal run =====
#pros make middle_right_goal

echo ===== Building recording run =====
pros make record_match
echo ===== Building replay run =====
pros make replay_match
