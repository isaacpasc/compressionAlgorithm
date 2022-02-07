//
// Created by cse310 on 2/3/22.
//
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>

#include "defns.h"

int main(int argc, char** argv) {


    if (argc == 1) {

        // init array of symbols
        struct symbol symbols[NSYMBOLS];
        for (int i=0; i<NSYMBOLS; i++) {
            symbols[i].symbol = i+'0';
            symbols[i].parent = nullptr;
            symbols[i].left = nullptr;
            symbols[i].right = nullptr;
            symbols[i].freq = 0;
            for (char & j : symbols[i].encoding) {
                j = '\0';
            }
        }

        // read from stdin
        char str[256];
        while (fgets(str, 50, stdin)) {
            std::stringstream ls( str );
            for (int i=0; i<ls.str().length(); i++) {
                int tmp = (int)ls.str()[i];
                if (tmp < 0 || tmp > 127) {
                    //std::cout << "ERROR, Character from input file out of bounds" << std::endl;
                } else {
                    symbols[tmp].freq = symbols[tmp].freq+1;
                }
            }
        }
        if (symbols[10].freq != 1) {
            symbols[10].freq = symbols[10].freq + 1;
        }

        // iterate symbols and output result
        for (int i=0; i<NSYMBOLS; i++) {
            if (symbols[i].freq > 0) {
                std::cout << i << '\t' << symbols[i].freq << std::endl;
            }
        }
    } else {
        std::cout << "Incorrect usage\nEnding program..." << std::endl;
    }
    return 0;
}
