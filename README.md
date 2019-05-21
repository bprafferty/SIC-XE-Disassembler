Brian Rafferty -- cssc0475
Youssef Iraqi -- cssc0428
Adam Hancock -- cssc0435
---------------------
CS 530 Systems Programming, Spring 2019
---------------------
Program Assignment #2
---------------------
README.md
---------------------
Manifest:
    README.md
	XEDisassemblerMain.cpp
	transformer.cpp
	SymbolHandler.cpp
	SymbolHandler.h
	sicGenerator.cpp
	lisGenerator.cpp
	labelMatcher.h
	labelMatcher.cpp
	Instructions.cpp
	Instructions.h
	Makefile
----------------------
Compiling xed:
    make
----------------------
Produce .sic and .lis output:
	./xed <filename>
----------------------
Necessary files:
	<filename>.obj and <filename>.sym in same folder
----------------------
List/description of novel/significant design decisions:
We decided to make the program modular, and split the
handling of data between many different functions.
We used multiple files to parse and interpret the .obj and .sym files.
----------------------
List/description of any extra features/algorithms/functionality 
you included which were not required:
We added error handlers if the files failed to open, and
also inserted an error message if the user fails to enter
a file on the command line.
----------------------
List/description of all known deficiencies or bugs:
Nothing to report in terms of bugs.
----------------------
Lessons Learned:
We learned the benefits of classes in C++ and efficiently
kept record of our data using instances of objects. We also
learned the importance of using multiple classes and files
in order to keep programs concise and easy to read.
