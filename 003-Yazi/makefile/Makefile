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


# file extensions can be used as target name
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


# assign shell commands to variable - show all .cpp files filtered from all files in current directory
SRCS := $(shell find ./* -type f | grep "\.cpp")	# all .cpp files inside sub-directories also included
# "find ./* -type f" - means find all files (find ./*) but not include directories (-type f), f stand for file only
# "grep "\.cpp" - means filter out all files that are not .cpp extensions


# print variable
$(warning SRCS is $(SRCS))			# SRCS is ./add.cpp ./main.cpp ./sub.cpp


# patsubst - filter all .cpp files from SRCS variable, then convert all .cpp files to .o files
SRCS := $(shell find ./* -type f | grep "\.cpp")			# assign shell scripts to variable
$(warning SRCS is $(SRCS))									# SRCS is ./add.cpp ./main.cpp ./sub.cpp
OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
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
CFLAGS := -g -O2 -Wall -Werror -Wno-unused -std=c++11
target: $(OBJS) main
%.o: %.cpp
	g++ -I. $(CFLAGS) -c $< -o $@	# g++ -I. -g -O2 -Wall -Werror -Wno-unused -std=c++11 -c add.cpp -o add.o
main:
	g++ $(CFLAGS) $(OBJS) -o main	# g++ -g -O2 -Wall -Werror -Wno-unused -std=c++11  ./add.o  ./main.o  ./sub.o -o main
clear:
	rm -rf $(OBJS) main
# -O2 means Optimization level 2
# -Wall means all warnings
# -Wno-unused means all unused variables
# -ldl means a linking option
# -std=c++11 means using C++11 standard library


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
