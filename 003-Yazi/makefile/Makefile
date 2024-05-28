# It is recommended to install "Windows Terminal" to run make inside the terminal
# If you using Windows to run g++'s -ldl option, ONLY cygwin can work properly, ...
# because C:\cygwin64\lib contains "libdl.a" but MinGW not

# Here are the steps to setup cygwin default g++ compiler:
# On Windows, search for "Environment Variables".
# On "Environment Variables" window's "System variables" sub-window, select "Path" then click "Edit".
# On Edit environment variable, click "New", then paste the path, i.e. C:\cygwin64\bin
# keep clicking "Move Up" until the compiler prior any other g++ compiler




# gcc

src_to_binary:
	gcc -c main.cpp -o main.o

binary_to_executable:
	gcc main.o -o main -lstdc++

src_to_executable:
	gcc main.cpp -o main -lstdc++




# g++

src_to_preprocess:
	g++ -E main.cpp -o main.i


preprocess_to_assembly:
	g++ -S main.i -o main.s


assembly_to_binary:
	g++ -c main.s -o main.o


binary_to_executable:			# linking
	g++ main.o -o main


src_to_binary:					# combine
	g++ -c main.cpp -o main.o


src_to_executable:
	g++ main.cpp -o main


# compile more than one source file
srcs_to_executable:				# main.cpp has to include add.h
	g++ -c main.cpp -o main.o
	g++ -c add.cpp -o add.o
	g++ add.o main.o -o main


# add root directory to cpp search paths - i.e. #include <add.h>
srcs_to_executable:				# -I[dir], I means include, [dir] means search path, . means current directory
	g++ -I[dir] ...
#i.e.
	g++ -I. add.cpp main.cpp -o main


# add multiple directories to cpp search paths
srcs_to_executable:
	g++ -I[dir1] -I[dir2] -I[dir3] ...
#i.e.
	g++ -I. -I./include add.cpp main.cpp -o main


srcs_to_binaries_to_executable:		# linking doesn't need -I
	g++ -I. -c add.cpp -o add.o
	g++ -I. -c sub.cpp -o sub.o
	g++ -I. -c main.cpp -o main.o
	g++ add.o sub.o main.o -o main




# Makefile basic

check_make_installed:
	make --version


# create target to use Makefile to compile executable from source code
[target]:							# "target" is an arbitrary name
	g++ -I. add.cpp sub.cpp main.cpp -o main


# to run Makefile with first target ONLY
run:
	make


# to run Makefile with one or more specific target(s)
run:
	make [target1] [target2] [target3] ...


# define a target depends on another target, i.e. main
target: main
main:
	g++ -I. add.cpp sub.cpp main.cpp -o main


# define a target depends on multiple targets
target: add_o sub_o main_o main		# main must be the last if main need add.o, sub.o & main.o to compile
add_o:								# once add_o produce add.o, main could use add.o to compile
	g++ -I. -c add.cpp -o add.o
sub_o:
	g++ -I. -c sub.cpp -o sub.o
main_o:
	g++ -I. -c main.cpp -o main.o
main:
	g++ add.o sub.o main.o -o main


# .o can be used as target name but .cpp can't
target: add.o sub.o main.o main
add.o:
	g++ -I. -c add.cpp -o add.o
sub.o:
	g++ -I. -c sub.cpp -o sub.o
main.o:
	g++ -I. -c main.cpp -o main.o
main:
	g++ add.o sub.o main.o -o main


# automatic variables - make targets, input files and output files as placeholder
target: add.o sub.o main.o main		# [target].o replace %.o, [target].cpp replace %.cpp
%.o: %.cpp							# [target].cpp replace $<, [target].o replace $@
	g++ -I. -c $< -o $@
main:								# $<, $@ are called "automatic variables"
	g++ add.o sub.o main.o -o main


# variable - replace multiple targets with a variable
OBJS := add.o sub.o main.o
target: $(OBJS) main
%.o: %.cpp
	g++ -I. -c $< -o $@
main:
	g++ $(OBJS) -o main


# = basic values assignment					i.e. OBJS = add.o main.o
# := override previous assigned values		i.e. OBJS := add.o main.o
# ?= assign values if not assigned yet		i.e. OBJS ?= add.o main.o
# += append values							i.e. OBJS += add.o main.o


# using make to run shell scripts - i.e. remove specified files using make
clear:								# remove all .o files
	rm -rf *.o main
# or								# remove all files contain in variable OBJS
	rm -rf $(OBJS) main
									# note that main.exe can be replace with main


# assign shell commands to variable
SRCS := $(shell find ./* -type f | grep "\.cpp")
# "find ./* -type f" - 	means find all files (find ./*), also include all files inside its sub-directories,
# 						but not include directories (-type f), f stand for file only
# "grep "\.cpp" - 		means filter out all files that are not .cpp extensions,
# "\.cpp" -				backslash "\" is used as an escape character to treat the period "." as a literal character,
# 						not as a wildcard in regular expressions

# print variable
$(warning SRCS is $(SRCS))			# SRCS is ./add.cpp ./main.cpp ./sub.cpp


# patsubst - filter all .cpp files from SRCS variable, then convert all .cpp files to .o files
$(patsubst [pattern], [replacement], [text])
# patsubst -	pat stand for pattern, sub stand for substitute, st stand for string
# patsubst -	Finds whitespace-separated words in text that match pattern and replaces them with replacement,
# 				Here pattern may contain a ‘%’ which acts as a wildcard, matching any number of any characters within a word,
# 				If replacement also contains a ‘%’, the ‘%’ is replaced by the text that matched the ‘%’ in pattern. i.e.:
$(patsubst %.c, %.o, add.c sub.c main.c)		# it will return "add.o sub.o main.o"
# Words that do not match the pattern are kept without change in the output.


# convert all .cpp files (included .cpp inside sub-directories) to .o files
SRCS := $(shell find ./* -type f | grep "\.cpp")			# assign shell scripts to variable
$(warning SRCS is $(SRCS))									# SRCS is ./add.cpp ./main.cpp ./sub.cpp
OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))	# filter all .cpp files from SRCS variable, then convert all .cpp files to .o files
$(warning OBJS is $(OBJS))									# OBJS is ./add.o ./main.o ./sub.o
target: $(OBJS) main
%.o: %.cpp
	g++ -c $< -o $@
main:
	g++ $(OBJS) -o main
clear:
	rm -rf $(OBJS) main
# filter files is one of the most important task in learning Makefile series


# assign compile options to variable
SRCS := $(shell find ./* -type f | grep "\.cpp")
$(warning SRCS is $(SRCS))
OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is $(OBJS))

CFLAGS := -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11

target: $(OBJS) main
%.o: %.cpp
	g++ -I. $(CFLAGS) -c $< -o $@	# g++ -I. -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11 -c add.cpp -o add.o
main:
	g++ $(CFLAGS) $(OBJS) -o main	# g++ -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11  ./add.o  ./main.o  ./sub.o -o main
clear:
	rm -rf $(OBJS) main

# -g - 			is used to include debugging information in the compiled executable.
#				This information is essential for debugging the program with tools like gdb (GNU Debugger) or other debugging tools.
#				When you compile your C++ code with the -g option, the compiler includes additional metadata in the executable binary that
#				provides information about the source code, such as line numbers, variable names and function names.
#				Including debugging information with -g does increase the size of the executable,
#				so it's typically not used in production builds where executable size is a concern.

# -O2 - 		O stand for Optimization option that instructs the compiler to apply a moderate level of optimization to the generated machine code.
#				2 stand for Optimization level 2.
#				For example, Functions marked with the inline keyword or small functions may be inserted directly into the calling code
#				instead of being called as a separate function. This reduces the overhead of function calls.
#				Analyzes loops to make them run faster.
#				Removing code that will never be executed, which could be the result of conditional checks or unreachable code blocks.
#				-O2 strikes a balance between generating fast code and keeping compilation times reasonable.
#				It's a good choice for most production-level code where you want a significant performance improvement without overly impacting compilation time.

# -O0 -			stand for Optimization Level 0. the compiler doesn't perform any optimization. The generated code closely reflects the structure of your source code.
#				Debugging is easier with -O0 because the generated code corresponds more directly to the source code,
#				making it simpler to trace back to specific lines of code during debugging sessions.
#				However, the resulting executable may be slower and consume more memory compared to optimized versions.

# -O0 vs O2 - 	-O0 prioritizes easy debugging and a closer correspondence between the source and generated code,
#				while -O2 focuses on generating faster and more efficient code at the expense of potentially making debugging more challenging.
#				The choice between -O0 and -O2 depends on your priorities,
#				such as whether you're optimizing for performance or prioritizing ease of debugging during development.

# -Wall -		means to enable all warnings during compilation. These warnings could include things like using variables without initialization,
# 				implicit conversions that might lead to loss of data, potential misuse of functions, and other similar issues.

# -Werror -		any warning generated by the compiler will cause the compilation process to fail, treating the warning as if it were an error.

# -Wno-unused -	is used to suppress compiler warnings related to unused entities in your code, such as variables, functions, or parameters.
#				By default, the compiler will issue warnings if it detects any declared entity (like a variable or a function) that is not used anywhere in your code.
#				These warnings can be helpful for identifying potential mistakes or unnecessary code that could be removed to improve clarity and maintainability.

# -ldl -		is used to link against the dynamic linking library, libdl. This library provides functions that
#				enable a program to load and use dynamic shared objects (DSOs) during runtime.

# -std=c++11 -	tells the compiler to use the C++11 standard for compiling your code. Your code has to comform the rules and syntax defined in the C++11 standard.
#				By default, g++ may conform the eariler C++98/03 version if the standard is not specified.

# C++11 -		Lambda expressions for inline anonymous functions. "auto" keyword for automatic type inference. Range-based for loops for iterating over containers.
#				Move semantics for efficient resource management. Smart pointers (std::unique_ptr, std::shared_ptr, std::weak_ptr) for automatic memory management.
#				"constexpr" keyword for compile-time evaluation of expressions. "nullptr" keyword for null pointer constant.
#				Improved support for multithreading with the <thread> and <mutex> libraries.


# assign searching paths to variable
SRCS := $(shell find ./* -type f | grep "\.cpp")
$(warning SRCS is $(SRCS))
OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is $(OBJS))
CFLAGS := -g -O2 -Wall -Werror -Wno-unused -std=c++11
INCLUDE := -I.											# here
target: $(OBJS) main
%.o: %.cpp
	g++ $(INCLUDE) $(CFLAGS) -c $< -o $@				# here
main:
	g++ $(CFLAGS) $(OBJS) -o main
clear:													# not use in target
	rm -rf $(OBJS) main


# compile a project contains multiple main() functions in multiple files, aka multiple main entry points in a project
SRCS := $(shell find ./* -type f | grep "\.cpp" | grep -v "./main\.cpp" | grep -v "./test\.cpp")
$(warning SRCS is $(SRCS))											# last 2 grep above filter out main.cpp & test.cpp

OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is $(OBJS))

CFLAGS := -g -O2 -Wall -Werror -Wno-unused -std=c++11

INCLUDE := -I.

MAIN_SRC := main.cpp
MAIN_OBJ := $(MAIN_SRC:%.cpp=%.o)				# replace main.cpp with main.o
MAIN_EXE := main

TEST_SRC := test.cpp
TEST_OBJ := $(TEST_SRC:%.cpp=%.o)				# replace test.cpp with test.o
TEST_EXE := test

target: $(MAIN_EXE) $(TEST_EXE)

$(MAIN_EXE): $(OBJS) $(MAIN_OBJ)
	g++ -o $@ $^ $(INCLUDE) $(CFLAGS)

$(TEST_EXE): $(OBJS) $(TEST_OBJ)
	g++ -o $@ $^ $(INCLUDE) $(CFLAGS)

%.o: %.cpp										# not use in target
	g++ $(INCLUDE) $(CFLAGS) -c $< -o $@

clear:
	rm -rf $(OBJS) $(MAIN_OBJ) $(TEST_OBJ) $(MAIN_EXE) $(TEST_EXE)


# compile a project contains single main entry template
CC := g++				# specify a compiler

SRCS := $(shell find ./* -type f | grep "\.cpp" | grep -v "./main\.cpp")
$(warning SRCS is $(SRCS))

OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is $(OBJS))

CFLAGS := -g -O2 -Wall -Werror -Wno-unused -std=c++11

INCLUDE := -I.

MAIN_SRC := main.cpp
MAIN_OBJ := $(MAIN_SRC:%.cpp=%.o)
MAIN_EXE := main

target: $(MAIN_EXE)

$(MAIN_EXE): $(OBJS) $(MAIN_OBJ)
	$(CC) -o $@ $^ $(INCLUDE) $(CFLAGS)

%.o: %.cpp
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

clear:
	rm -rf $(OBJS) $(MAIN_OBJ) $(MAIN_EXE)


# if headers are inside a folder, i.e. include
CC := g++

SRCS := $(shell find ./* -type f | grep "\.cpp" | grep -v "./main\.cpp")
$(warning SRCS is $(SRCS))

OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is $(OBJS))

CFLAGS := -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11

INCLUDE := -I. -I./include

MAIN_SRC := main.cpp
MAIN_OBJ := $(MAIN_SRC:%.cpp=%.o)
MAIN_EXE := main

target: $(MAIN_EXE)

$(MAIN_EXE): $(OBJS) $(MAIN_OBJ)
	$(CC) -o $@ $^ $(INCLUDE) $(CFLAGS)

%.o: %.cpp
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

clear:
	rm -rf $(OBJS) $(MAIN_OBJ) $(MAIN_EXE)


# create binary for static library
CC := g++

SRCS := $(shell find ./* -type f | grep "\.cpp" | grep -v "./main\.cpp")
$(warning SRCS is $(SRCS))

OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is $(OBJS))

CFLAGS := -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11

INCLUDE := -I. -I./include

MAIN_SRC := main.cpp
MAIN_OBJ := $(MAIN_SRC:%.cpp=%.o)
MAIN_EXE := main

target: $(MAIN_EXE)

$(MAIN_EXE): $(OBJS) $(MAIN_OBJ)
	$(CC) -o $@ $^ $(INCLUDE) $(CFLAGS)

MUL_SRC := ./lib/mul.cpp						# source for library
MUL_OBJ := $(MUL_SRC:%.cpp=%.o)					# create .o file at first, than use .o to create .a
MUL_LIB := ./lib/libmul.a

static: $(MUL_LIB)

$(MUL_LIB): $(MUL_OBJ)
	ar rs $@ $^									# ar is the command to create .a file
												# mul.o create libmul.a file
%.o: %.cpp
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

clear:
	rm -rf $(OBJS) $(MAIN_OBJ) $(MAIN_EXE)
# then "make static"


# linking static libraries
CC := g++

SRCS := $(shell find ./* -type f | grep "\.cpp" | grep -v "./main\.cpp")
$(warning SRCS is $(SRCS))

OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is $(OBJS))

CFLAGS := -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11

LDFLAGS := -L./lib -lmul								# link static library

INCLUDE := -I. -I./include

MAIN_SRC := main.cpp
MAIN_OBJ := $(MAIN_SRC:%.cpp=%.o)
MAIN_EXE := main

target: $(MAIN_EXE)

$(MAIN_EXE): $(OBJS) $(MAIN_OBJ)
	$(CC) -o $@ $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS)		# add link command to compile executable command

MUL_SRC := ./lib/mul.cpp
MUL_OBJ := $(MUL_SRC:%.cpp=%.o)
MUL_LIB := ./lib/libmul.a

static: $(MUL_LIB)

$(MUL_LIB): $(MUL_OBJ)
	ar rs $@ $^

%.o: %.cpp
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

clear:
	rm -rf $(OBJS) $(MAIN_OBJ) $(MAIN_EXE)
# run "make static" at first to create .a file, then run "make" to create .o files


# linking dynamic libraries
