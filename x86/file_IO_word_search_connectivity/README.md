# File IO and Search (x86 and C++)

An x86 connectivity project focused on interfacing between C++ and x86. All of the functions are written in x86 ASM while C++ loops through the file and calls those functions.

You can check out an analysis of runtimes with different buffer sizes (50 all the way up to 750,000) [here](https://github.com/CollateralJ/cs-playground/blob/main/x86/file_IO_word_search_connectivity/analysis.pdf)

### Usage:

make 

./main -f [FILE_NAME] -w [WORD]

### Example Output:

![Searching the US constitution](https://github.com/CollateralJ/cs-playground/blob/main/x86/file_IO_word_search_connectivity/output.png "22 results")