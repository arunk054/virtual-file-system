# Virtual file system
The goal of this project is to build a virtual file system (VFS), which allows basic operations on files / directories like create, delete and search. The VFS is persisted in a file on the local drive which allows it to be easily transferrable. In our implementation, we use several data structures like n-ary tree to store the directory structure, binary search tree to perform efficient file search, hash tables to detect duplicate files and linked lists to maintain empty blocks. The VFS also supports trash functionality to delete and restore files. The major challenges involved persistence and synchronization of different data structures. The source code is written entirely in C and uses GTK libraries to implement a graphical user interface.

Following is a demo of this application in action, please use the full screen view for a clearer picture.

[![View Demo](https://img.youtube.com/vi/IALBRVxv4d4/0.jpg)](https://www.youtube.com/watch?v=IALBRVxv4d4 "View Demo")

====== STEPS  TO COMPILE THE SOURCE FILES

We use a Makefile. Following is a list of make commands along with their descriptions

 'make list' - To know the compile options avaliable

'make all' - To compile all source files

 'make gui'- To build a binary for the gui.  Note: This requires GTK 2.0 or higher 

'make exe'  - To build a command line binary
