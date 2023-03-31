# lorraine
The new, multipurpose, optimizing compiler for the Lua++ programming language. Lorraine utilizes LLVM's powerful IR to generate optimized Lua++ bytecode and native code for various architectures. The goal of this project was to provide a safer and more powerful alternative to the existing compiler for the Lua++ programming language.  


## Contributing
If you've got any ideas in mind and feel the need to contribute to the project, refer to the steps below.
1. **Installing important dependencies**: Before you clone the repository, make sure you have the following "packages" installed: [LLVM](https://apt.llvm.org/), [flex](https://github.com/westes/flex), and [bison](https://www.gnu.org/software/bison/). These are all avalible via the APT packange manager (Linux yay!).  
2. **Installing the project**: After you have successfully cloned lorraine onto your machine you can run ``cmake .`` to setup the project. Various files will appear in your home directory, most of which you can ignore as they are CMake specific files.
3. **Running**: You should see a Makefile appear after running the previous command. You can run any commands avalible and all binaries will be placed in the `bin/` directory (To start the compiler run: `./bin/compiler`).
