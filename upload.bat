@echo off
echo ===== Building left side auto winpoint run =====
make -B EXTRA_CXXFLAGS="-D LEFT_SIDE_WINPOINT" EXTRA_CXXFLAGS="-D LEFT_SIDE_WINPOINT"
echo ===== Uploading left side auto winpoint run =====
pros upload --slot 1 --after none --target v5 --name "Left side winpoint"
echo ===== Building right side auto winpoint =====
make -B EXTRA_CXXFLAGS="-D RIGHT_SIDE_WINPOINT" EXTRA_CXXFLAGS="-D RIGHT_SIDE_WINPOINT"
echo ===== Uploading right side auto winpoint run =====
pros upload --slot 2 --after none --target v5 --name "Right side winpoint"

echo ===== Building middle left goal run =====
make -B EXTRA_CXXFLAGS="-D MIDDLE_LEFT_GOAL" EXTRA_CXXFLAGS="-D MIDDLE_LEFT_GOAL"
echo ===== Uploading middle left goal run =====
pros upload --slot 3 --after none --target v5 --name "Middle left goal"
echo ===== Building middle right goal run =====
make -B EXTRA_CXXFLAGS="-D MIDDLE_RIGHT_GOAL" EXTRA_CXXFLAGS="-D MIDDLE_RIGHT_GOAL"
echo ===== Uploading middle right goal run =====
pros upload --slot 4 --after none --target v5 --name "Middle right goal"

echo ===== Building reset run =====
make -B EXTRA_CXXFLAGS="-D RESET_POSITIONS" EXTRA_CXXFLAGS="-D RESET_POSITIONS"
echo ===== Uploading reset run =====
pros upload --slot 6 --after none --target v5 --name "Reset"
echo ===== Building basic run =====
make -B
echo ===== Uploading basic run =====
pros upload --slot 5 --after none --target v5 --name "Basic"
