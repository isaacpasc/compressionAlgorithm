//
// Created by cse310 on 2/3/22.
//
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#include "defns.h"

void insertion(tree*, int);
void merge(tree* arr, int, int, int);
void mergeSort(tree*, int, int);
void buildTree(tree*, int);
void decode(struct symbol*,struct symbol*, std::string, int*);

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

        // read from stdin and output decoded text
        int skipToLine = alphaCount+nonAlphaCount+3;
        for (std::string line; std::getline(std::cin, line);) {
            if (skipToLine == 0) {
                int index = 0;
                decode(&Root, &Root, line, &index);
            }
            skipToLine--;
        }
        std::cout << '\n';
    } else {
        std::cout << "Input File Unavailable: " << argv[1] << "\nEnding program..." << std::endl;
        return -1;
    }
    return 0;
}

void mergeSort(tree* arr, int l, int r) {

    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void merge(tree* arr, int l, int m, int r) {

    // create temp arrays, left and right
    int i, j, k;
    int lTmp = m - l + 1;
    int rTmp = r - m;
    struct tree L[lTmp], R[rTmp];

    // copy data to temp arrays
    for (i = 0; i < lTmp; i++)
        L[i] = arr[l + i];
    for (j = 0; j < rTmp; j++)
        R[j] = arr[m + 1 + j];

    //merge temp to original array by frequency then index
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < lTmp && j < rTmp) {
        if (L[i].freq < R[j].freq) {
            arr[k] = L[i];
            i++;
        } else if (L[i].freq > R[j].freq) {
            arr[k] = R[j];
            j++;
        } else if (L[i].freq == R[j].freq && L[i].index > R[j].index) {
            arr[k] = R[j];
            j++;
        } else if (L[i].freq == R[j].freq && L[i].index < R[j].index) {
            arr[k] = L[i];
            i++;
        }
        k++;
    }
    while (i < lTmp) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < rTmp) {
        arr[k] = R[j];
        j++;
        k++;
    }
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

void decode(struct symbol* node,struct symbol* root , std::string str, int* index) {
    if (*index == str.length()) {
        return;
    }
    if (str[*index] == '0') {
        if (node->left == NULL) {
            std::cout << node->symbol;
            decode(root, root, str, index);
        } else {
            *index = *index + 1;
            decode(node->left, root, str, index);
        }
    } else {
        if (node->right == NULL) {
            std::cout << node->symbol;
            decode(root, root, str, index);
        } else {
            *index = *index + 1;
            decode(node->right, root, str, index);
        }
    }
}