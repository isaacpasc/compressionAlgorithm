//
// Created by cse310 on 2/3/22.
//
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>

#include "defns.h"

void insertion(tree*, int);
void merge(tree* arr, int, int, int);
void mergeSort(tree*, int, int);
void buildTree(tree*, int);
void preorder(struct symbol*, std::string*, std::string, int*);

int main(int argc, char** argv) {

    // open input file
    std::string algorithm = argv[2];
    if (algorithm != "insertion" && algorithm != "merge") {
        std::cout << "ERROR, algorithm named: " << algorithm << " does not exist here" << std::endl;
        return -1;
    }
    std::ifstream inFile (argv[1]);
    if (inFile.is_open()) {

        // init array of symbols
        struct symbol symbols[NSYMBOLS];
        for (int i=0; i<NSYMBOLS; i++) {
            symbols[i].symbol = i;
            symbols[i].parent = NULL;
            symbols[i].left = NULL;
            symbols[i].right = NULL;
            symbols[i].freq = 0;
            for (char & j : symbols[i].encoding) {
                j = '\0';
            }
        }
        int a, b;
        int alphaCount = 0;
        int nonAlphaCount = 0;
        while (inFile >> a >> b) {
            symbols[a].freq = b;
            if ((a >= 65 && a <= 90) || (a >= 97 && a <= 122)) {
                alphaCount = alphaCount + 1;
            } else {
                nonAlphaCount = nonAlphaCount + 1;
            }
        }
        inFile.close();

        // init arrays of alpha and nonalpha
        struct tree alpha[alphaCount];
        struct tree nonAlpha[nonAlphaCount];
        int alphaPointer = 0;
        int nonAlphaPointer = 0;
        for (int i=0; i<NSYMBOLS; i++) {
            if (symbols[i].freq > 0) {
                if ((i >= 65 && i <= 90) || (i >= 97 && i <= 122)) {
                    alpha[alphaPointer].index = i;
                    alpha[alphaPointer].symbol = symbols[i].symbol;
                    alpha[alphaPointer].freq = symbols[i].freq;
                    alpha[alphaPointer].root = &symbols[i];
                    alphaPointer = alphaPointer + 1;
                } else {
                    nonAlpha[nonAlphaPointer].index = i;
                    nonAlpha[nonAlphaPointer].symbol = symbols[i].symbol;
                    nonAlpha[nonAlphaPointer].freq = symbols[i].freq;
                    nonAlpha[nonAlphaPointer].root = &symbols[i];
                    nonAlphaPointer = nonAlphaPointer + 1;
                }
            }
        }

        // send array to be sorted
        if (algorithm == "merge") {
            mergeSort(alpha, 0, alphaCount-1);
            mergeSort(nonAlpha, 0, nonAlphaCount-1);
        } else {
            insertion(alpha, alphaCount);
            insertion(nonAlpha, nonAlphaCount);
        }

        // create bst
        buildTree(alpha, alphaCount);
        buildTree(nonAlpha, nonAlphaCount);

        // combine trees
        struct symbol Root{};
        Root.parent=NULL;
        Root.left = alpha[alphaCount-1].root;
        Root.right = nonAlpha[nonAlphaCount-1].root;
        alpha[alphaCount-1].root->parent = &Root;
        nonAlpha[nonAlphaCount-1].root->parent = &Root;
        Root.freq = alpha[alphaCount-1].freq + nonAlpha[nonAlphaCount-1].freq;
        struct symbol* rootPtr = &Root;

        // traverse tree to set code
        std::string code = "";
        std::string* codePtr = &code;
        int react = 0;
        int* reactPtr = &react;
        preorder(rootPtr, codePtr, "root", reactPtr);

        // output result
        std::cout << alphaCount+nonAlphaCount << std::endl;
        for (int i=0; i<NSYMBOLS; i++) {
            if (symbols[i].freq > 0) {
                std::cout << (int)symbols[i].symbol << '\t'
                << symbols[i].symbol << '\t' << symbols[i].encoding << std::endl;
            }
        }
        std::string encodeFinal = "";

        // read from stdin
        char str[256];
        while (fgets(str, 50, stdin)) {
            std::stringstream ls( str );
            for (int i=0; i<ls.str().length(); i++) {
                int tmp = (int)ls.str()[i];
                if (tmp < 0 || tmp > 127) {
                    //std::cout << "ERROR, Character from input file out of bounds" << std::endl;
                } else {
                    encodeFinal = encodeFinal + symbols[tmp].encoding;
                }
            }
        }
        encodeFinal = encodeFinal + symbols[10].encoding;

        // output final encoding
        std::cout << '\n' << encodeFinal;
    } else {
        std::cout << "Input File Unavailable: " << argv[1] << "\nEnding program..." << std::endl;
        return -1;
    }
    return 0;
}

void mergeSort(tree* arr, int l, int r) {

}

void merge(tree* arr, int l, int m, int r) {

}

void insertion(tree* arr, int n) {
    int j;
    tree tmp{};
    for (int i=1; i<n; i++) {
        j=i-1;
        tmp = arr[i];
        while((j >= 0 && arr[j].freq > tmp.freq) || (j >= 0 && arr[j].freq == tmp.freq && arr[j].index > tmp.index)) {
            arr[j+1]=arr[j];
            j=j-1;
        }
        arr[j+1]=tmp;
    }
}

void buildTree(tree* arr, int n) {
    int i = 0;
    int j = 1;
    int insertLocation = 0;
    while(j != n) {
        struct symbol* t = (symbol*)malloc(sizeof(symbol));
        t->parent = NULL;
        t->left = arr[i].root;
        t->right = arr[j].root;
        t->freq = arr[i].freq + arr[j].freq;
        arr[i].root->parent = t;
        arr[j].root->parent = t;

        // find where to insert node t
        for (int k=j; k<n; k++) {
            if (t->freq < arr[k].freq) {
                insertLocation=k-1;
                break;
            } else if (k == n-1) {
                insertLocation=k;
                break;
            }
        }

        // insert t
        for (int k=i; k<insertLocation; k++) {
            arr[k] = arr[k+1];
        }
        arr[insertLocation].freq = t->freq;
        arr[insertLocation].root = t;

        // increase i and j (simulate array element deletion without making copies
        i++;
        j++;
    }
}

void preorder(struct symbol* node, std::string* code, std::string concat, int* react)
{
    if (node == NULL) {
        *react = *react + 1;
        return;
    }

    // concat code unless starting from root
    if (concat != "root") {
        *code = *code + concat;
    }

    // recur on both trees
    preorder(node->left, code, "0", react);
    preorder(node->right, code, "1", react);

    // after reaching 2 null, assign code
    if (*react >= 2) {
        for (int i=0; i<code->length(); i++) {
            node->encoding[i] = (*code)[i];
        }
        *react = 0;
        //std::cout << "symbol: " << node->symbol << " is code: " << node->encoding << std::endl;
    }

    // remove tail end char from code
    if (code->length() >= 1) {
        code->erase(code->length()-1,1);
    }
}