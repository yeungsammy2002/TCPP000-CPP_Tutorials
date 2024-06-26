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

# When you write a rule in Makefile, you specify a target, dependencies, and commands to execute.
# In this example, "target" is the target, "add_o", "sub_o", "main_o" and "main" are its dependencies.


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

# $< -	represents the first prerequisite (dependency) of a target. When a rule is executed,
#		$< will be replaced with the name of the first prerequisite of the rule.

# $@ -	represents the target of the rule. When a rule is executed, $@ will be replaced with
#		the name of the target

# For example,
output.txt: input.txt
	cp $< $@
# $< would represent "input.txt" since it is the first prerequisite of "output.txt".
# $@ would represent "output.txt"



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

%.o: %.cpp				# i.e. g++ -I. -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11 -c add.cpp -o add.o
	g++ -I. $(CFLAGS) -c $< -o $@

main:					# g++ -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11  ./add.o  ./main.o  ./sub.o -o main
	g++ $(CFLAGS) $(OBJS) -o main

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
#				enable a program to load and use dynamic shared objects (DSOs) during runtime. (ONLY work using cygwin compiler)

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

CFLAGS := -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11

INCLUDE := -I.											# here

target: $(OBJS) main

%.o: %.cpp												# here
	g++ $(INCLUDE) $(CFLAGS) -c $< -o $@

main:
	g++ $(CFLAGS) $(OBJS) -o main

clear:													# not use in target
	rm -rf $(OBJS) main


# compile a project contains multiple main() functions in multiple files, aka multiple main entry points in a project
SRCS := $(shell find ./* -type f | grep "\.cpp" | grep -v "./main\.cpp" | grep -v "./test\.cpp")
$(warning SRCS is $(SRCS))											# last 2 grep above filter out main.cpp & test.cpp

OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is $(OBJS))

CFLAGS := -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11

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

# $^ represents all of the the dependencies of the current target.
# For example:
program: main.o util.o
	gcc -o program $^
main.o: main.c				# compile main.c into main.o
	gcc -c main.c -o main.o
util.o: util.c				# compile util.c into util.o
	gcc -c util.c -o util.o
# In this example, "program" target depends on "main.o" and "util.o".
# When building "program", "$^" represent all dependencies "main.o util.o", so we don't need to explicity list them.
# There are separate rules to compile "main.c" and "util.c" into object files "main.o" and "util.o" respectively.
# you could write `gcc $^ -o program` instead of `gcc -o program $^`. Both forms achieve the same result.


# grep -v ... -	`-v` in the `grep` command is used to invert the match, 
#				meaning it selects all lines that do not match the specified pattern.

# Let's say you have a file named "example.txt" with the following content:
#	apple
#	banana
#	cherry
#	date

# Now you want to find all lines in "example.txt" that do not contain the world "banana".
# You can use `grep -v` to achieve this:
no_banana:
	grep -v "banana" example.txt

# The output would be:
#	apple
#	cherry
#	data


# compile a project contains single main entry template
CC := g++				# specify a compiler

SRCS := $(shell find ./* -type f | grep "\.cpp" | grep -v "./main\.cpp")
$(warning SRCS is $(SRCS))

OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is $(OBJS))

CFLAGS := -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11

INCLUDE := -I.

MAIN_SRC := main.cpp
MAIN_OBJ := $(MAIN_SRC:%.cpp=%.o)
MAIN_EXE := main

target: $(MAIN_EXE)

$(MAIN_EXE): $(OBJS) $(MAIN_OBJ)				# here
	$(CC) -o $@ $^ $(INCLUDE) $(CFLAGS)

%.o: %.cpp										# here
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

clear:
	rm -rf $(OBJS) $(MAIN_OBJ) $(MAIN_EXE)

 
# if headers are inside a folder, i.e. include, you have to add -I./include to INCLUDE variable
CC := g++

SRCS := $(shell find ./* -type f | grep "\.cpp" | grep -v "./main\.cpp")
$(warning SRCS is $(SRCS))

OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is $(OBJS))

CFLAGS := -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11

INCLUDE := -I. -I./include				# here

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

# `ar` -	is not directly related to g++. `ar` command is used to create, modify, and extract
#			from archives, typically used for bundling object files into a library.
#			It is a separate GNU utility used for creating, modifying, and extracting from archives.
# 			An archive is a collection of files bundled together, often used for libraries.
# 			Archives created by ar typically have the file extension .a.
#			When you compile multiple source files into object files (.o files), 
#			you might want to bundle them together into a static library. 
#			This is where `ar` comes in. g++ can call ar internally to create these libraries 
#			when you specify the appropriate options.
#			In summary, g++ is for compiling C++ code, while ar is for creating and managing archives, 
#			though they can work together in certain scenarios during the build process.

# `rs`:
# `r` -		`r` option inserts the specified files into the archive.
#			If a file with the same name already exists in the archive, it replaces it.
# `s` -		`s` option is used to update the index of the archive. It's particularly
#			useful when adding or replacing files in the archive, 
#			as it ensures that the archive's index (or symbol table) is kept up to date. 
#			This is important because without the index, 
#			tools like the linker might not be able to efficiently use the archive.

# When you use ar `rs`, you're essentially updating the archive with the specified files
# and refreshing its index to reflect any changes. This is commonly used when you're adding
# or updating object files within a library archive, ensuring that the archive remains consistent 
# and usable by other tools in the compilation process.


# linking static libraries
CC := g++

SRCS := $(shell find ./* -type f | grep "\.cpp" | grep -v "./main\.cpp")
$(warning SRCS is $(SRCS))

OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is $(OBJS))

CFLAGS := -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11

LDFLAGS := -L./lib -lmul						# -L./lib specify the static library location is in ./lib

INCLUDE := -I. -I./include						# -lmul linking ./lib/libmul.a static library

MAIN_SRC := main.cpp
MAIN_OBJ := $(MAIN_SRC:%.cpp=%.o)
MAIN_EXE := main

target: $(MAIN_EXE)

$(MAIN_EXE): $(OBJS) $(MAIN_OBJ)						# add link target `$(LDFLAGS)` to compile executable command
	$(CC) -o $@ $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS)

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
CC := g++

SRCS := $(shell find ./* -type f | grep "\.cpp" | grep -v "./main\.cpp")
$(warning SRCS is $(SRCS))

OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is $(OBJS))

CFLAGS := -g -O2 -Wall -Werror -Wno-unused -ldl -std=c++11

LDFLAGS := -L./lib -lmul

INCLUDE := -I. -I./include

MAIN_SRC := main.cpp
MAIN_OBJ := $(MAIN_SRC:%.cpp=%.o)
MAIN_EXE := main

target: $(MAIN_EXE)

$(MAIN_EXE): $(OBJS) $(MAIN_OBJ)
	$(CC) -o $@ $^ $(INCLUDE) $(CFLAGS) $(LDFLAGS)

MUL_SRC := ./lib/mul.cpp
MUL_OBJ := $(MUL_SRC:%.cpp=%.o)
MUL_LIB := ./lib/libmul.a
MUL_DLL := ./lib/libmul.so			# add dynamic library `.so`

static: $(MUL_LIB)
$(MUL_LIB): $(MUL_OBJ)
	ar rs $@ $^

shared: $(MUL_DLL)					# target that make dynamic library
$(MUL_DLL): $(MUL_OBJ)
	$(CC) -o $@ $^ $(INCLUDE) $(CFLAGS) -fPIC -shared

 %.o: %.cpp
	$(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

clear:
	rm -rf $(OBJS) $(MAIN_OBJ) $(MAIN_EXE)

# before running `make`, you have to:
# 1.	Copy dynamic library file to `/usr/local/lib` directory. The program will search 
#		for dynamic library files in this directory when the program start executing, or
# 2.	The program search for the dynamic library files through the environment variable
#		For example, running this command `export LD_LIBRARY_PATH=./lib`

# then run "make shared"

# -fPIC -	the option in g++ stands for "Position Independent Code." 
#			When you compile code with this option, it generates position-independent code, 
#			which means the generated machine code is not dependent on being located at a 
#			specific memory address.

#			Position-independent code is particularly useful for shared libraries 
#			(also known as dynamic libraries or shared objects), 
#			which can be loaded into memory at any address. Here's why:

#			1.	Address Independence: Position-independent code can be loaded into memory 
#				at any address without modification. This makes it suitable for shared libraries 
#				because the operating system loader can place the library in memory wherever it 
#				finds space, without worrying about conflicts with other libraries or the executable.

#			2.	Code Sharing: Position-independent code allows multiple processes to share the 
#				same instance of a library in memory. Since the code doesn't rely on fixed memory 
#				addresses, different processes can use the same shared library code simultaneously 
#				without interfering with each other.

#			3.	Dynamic Linking: Shared libraries are often dynamically linked, meaning they are 
#				loaded into memory when the program starts or when they're explicitly required, 
#				rather than being linked at compile time. Position-independent code facilitates this 
#				dynamic linking process because it doesn't rely on specific memory addresses.

#			So, when you compile code with g++ using the -fPIC option, you're ensuring that the resulting 
#			object files can be used to create shared libraries that are suitable for dynamic linking and 
#			can be loaded into memory at any address by the operating system loader.

# -shared -	The -shared option in g++ is used to create a shared library (also known as a dynamic library 
#			or shared object). When you compile code with this option, the output is a shared library file 
#			rather than an executable.
#			Here's what the -shared option does:

#			1.	Produces a Shared Library: Instead of creating an executable file, g++ generates a shared 
#				library file. Shared libraries contain code and data that can be loaded into memory and 
#				used by multiple programs simultaneously. They are particularly useful for code that needs 
#				to be shared among multiple executables, such as commonly used functions or modules.

#			2.	Dynamic Linking: Shared libraries are dynamically linked at runtime, meaning that the
#				necessary library code is loaded into memory when the program starts or when it's explicitly
#				required, rather than being linked at compile time. This allows multiple programs to share 
#				the same instance of the library in memory, reducing memory usage and facilitating code updates.

#			3.	Symbol Visibility: When creating a shared library, g++ also typically hides internal symbols 
#				by default, meaning that they're not exposed to external programs linking against the library.
#				This helps avoid symbol conflicts and keeps the library interface clean. However, you can 
#				control symbol visibility using additional options like -fvisibility if needed.

#			In summary, when you use the -shared option with g++, you're telling the compiler to generate a 
#			shared library file instead of an executable, allowing you to create reusable code that can be 
#			dynamically linked and shared among multiple programs.
