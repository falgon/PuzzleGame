FLAGS := -Wall -Wextra -pedantic -std=c++1z

all:clang_target1 gcc_target1
clang_target1: src/dfs_analyze.cpp
	clang++-5.0 $(FLAGS) src/dfs_analyze.cpp -o 0x01_clang.out
gcc_target1: src/dfs_analyze.cpp
	g++-6  $(FLAGS) src/dfs_analyze.cpp -o 0x01_gcc.out
clean:
	$(RM) tests/0x01_clang.out tests/0x01_gcc.out
