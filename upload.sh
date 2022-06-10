#!/bin/bash
pros v5 stop
pros v5 status
echo ===== Building reset run =====
pros make reset

echo ===== Building replay run =====
pros make replay

echo ===== Building recording run =====
pros make record
