# compressionAlgorithm
Encode or decode any text file with ASCII characters by using either insertion sort or merge sort to build a binary search tree and traverse tree to encode/decode.

Usage:
use 'make' to build executables: preprocess, encode, decode
run preprocess:
./preprocess <filetocompress.txt >preprocessedfile.txt
run encode:
./encode preprocessfile.txt insertion/merge <filetocompress.txt >encodedfile.txt
run decode:
./decode preprocessfile.txt insertion/merge <encodedfile.txt >decodedfile.txt

Summary:
In a fixed-length encoding scheme each symbol is assigned a bit string of the same length. An example is the
standard ASCII code which uses 7 bits for each symbol. One way of getting an encoding scheme that yields
a shorter bit string on average is to assign shorter codewords to more frequent symbols and longer ones to
less frequent symbols. Such a variable-length encoding scheme was used in the telegraph code invented by
Samuel Morse. In this project I have emplemented this variable-length encoding and decoding scheme.

Preprocess:
Preprocess uses no command line arguments and takes the text input from stdin(which can be redirected from a file with '<') and outputs to stdout(which can be redirected to a file with '>'). An array of structs, Symbols, is initiated to its ascii character(based on index number) with length 128(number of standard ASCII characters). Stdin is then read line by line, iterating through each character, adding to the frequency value of each corresponding charcater in the Symbols array. Lastly, for each nonzero frequency, the character index and frequency is printed to stdout.

Encode:
Encode uses 2 command line arguments, one is the preporcessed text file and and the second is either insertion or merge, depending on the sorting algorithm you would prefer. Then it takes the text input from stdin(which can be redirected from a file with '<') and outputs the encoded string to stdout(which can be redirected to a file with '>'). An array of struct Symbols is constructed once more with values from preprocess text file. Each charcter that is also a letter is placed in an array named Alpha and the rest are in nonAlpha. These arrays are both sorted using merge or insertion sort. Now a binary search tree is built from the ground up using alpha array then with nonalpha array. Then a root nod is created with its left child being the alpha bst and right being the nonAlpha bst. Now the tree is traversed using preorder, each left edge is a 0 and right edge is a one. Starting at the top of the bst, each 0/1 is concatenated into a final decoding string for each character(determined by concatenated string when traversal reaches a leaf node). Lastly, the original text is read once more but now each character is replaced by its encoded string of 0s and 1s and output to stdout.

Decode:
Decode uses 2 command line arguments, one is the preporcessed text file and and the second is either insertion or merge, depending on the sorting algorithm you would prefer. Then it takes the enmcoded text input from stdin(which can be redirected from a file with '<') and outputs the decoded string to stdout(which can be redirected to a file with '>'). Decode will act nearly identical to encode all the way up to the point where the bst is created. From there, the encoded string is read one index at a time. Starting at the root node of the bst, if the encoded strings current index contains a 1, then I traverse down the right and a 0, I traverse down the left edge. Repeate until a leaf node is reached, this mean I have my decoded character and it is printed to stdout.