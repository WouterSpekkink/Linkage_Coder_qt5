# Linkage Coder
This is the code repository for the Linkage Coder program, which is a tool I built to facilitate (part of) the qualitative coding of event data. I wrote the program for personal use, but anyone who's interested in the program or the code is free to use it. I therefore release the code under the GLP 3.0 license. 

# Downloading Linux and Windows builds of the program
I make Linux and Windows builds of the program available through my website. Go to <http://www.wouterspekkink.org> and then to the **Software** page to find builds of the program.

It should be possible to also build the program for Mac OSX, but I do not plan to do so myself (I do not have a Mac machine). Please see instructions below on how to build the program yourself. 

# Building the program from source
The program was built in C++ and Qt5, so if you want to build the program from the source code yourself, you'll need to have the C++ standard libraries and the Qt5 libraries installed on your machine. If you have the proper libraries installed on your machine, the following process will allow you to build the program from source.

Open a terminal and navigate to the folder with the source code. Then use the following commands:

```sh
qmake -project
qmake -makefile
```

One of the files that will be generated is a project file (*.pro). Open the file in a text editor, and add the following line somewhere near the top:

```txt
QT += widgets
```

Then, assuming that you have a suitable compiler (e.g, g++) installed on your machine you can build the program using the make command:

```sh
make -k
make clean 
```

# User manual
The user manual is available in the Manual/ folder. There is a tex version of the manual, as well as a pdf. The manual is also included with the Linux and Windows builds of the program (see above).
