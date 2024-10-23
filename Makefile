CC = g++
CFLAGS = -Wall -Werror -Wextra -std=c++17 -lstdc++
# TEST_FLAGS = -lgtest_main -lgtest -pthread
TEST_EXE = s21_test

OS := $(shell uname -s)
ifeq ($(OS), Linux)
# OPEN = xdg-open
	TEST_FLAGS = -lgtest_main -lgtest -pthread
# LEAK_COMMAND = valgrind --tool=memcheck --trace-children=yes --track-origins=yes --leak-check=full --show-leak-kinds=all
else # ifeq ($(OS), Darwin)
# OPEN_REPORT = open
	TEST_FLAGS = $(shell pkg-config --cflags --libs gtest) -lgtest_main -pthread
# LEAK_COMMAND = leaks -atExit --
endif

all: test

test: clean compile_tests
	./$(TEST_EXE)

compile_tests:
	@$(CC) $(CFLAGS) tests/*.cc $(TEST_FLAGS) -o $(TEST_EXE)

leaks: clean compile_tests
	leaks --atExit -- ./$(TEST_EXE)

valgrind: clean compile_tests
	valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes -v -q ./$(TEST_EXE)

check_style:
	@echo "┏=========================================┓"
	@echo "┃  Checking your code for Google Style    ┃"
	@echo "┗=========================================┛"
	@cp linters/.clang-format .
	@find . \( -name '*.h' -o -name '*.cc' \) -print0 | xargs -0 clang-format -n -style=Google --verbose
	@rm -f .clang-format

format_style:
	@echo "┏=========================================┓"
	@echo "┃  Formatting your code for Google Style  ┃"
	@echo "┗=========================================┛"
	@find . \( -name '*.h' -o -name '*.cc' \) -print0 | xargs -0 clang-format -i -style=Google --verbose
	@rm -f .clang-format

clean:
	@echo "Deleting unnecessary files..."
	@rm -rf *.o $(TEST_EXE) *.out
