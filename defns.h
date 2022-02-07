// File defns.h

#define	NSYMBOLS	128
#define	E_LEN	24

struct symbol{
	char symbol; // ASCII symbol
	int freq; // frequency of ASCII symbol (need 4 byte int)
	struct symbol *parent; // pointer to parent node
	struct symbol *left; // pointer to left child
	struct symbol *right; // pointer to right child
	char encoding[ E_LEN ]; // encoding of symbol
};

struct tree{
	int index; // index of symbol in Symbols array
	char symbol; // ASCII symbol
	int freq; // frequency of ASCII symbol (need 4 byte int)
	struct symbol *root; // pointer to a root node in the tree
};
	
