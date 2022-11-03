#include <stdio.h>
#include "tries.h"

int main() {
	trie *root = trie_insert(NULL, "arlington", "12");
	root = trie_insert(root, "arkhangelsk", "34");
	root = trie_insert(root, "arkham", "56");
	root = trie_insert(root, "albuquerque", "78");
	root = trie_insert(root, "almaty", "91");
	root = trie_insert(root, "alicante", "101");
	trie_print(root, 0);
	printf("Lookup 'alicante': Found %s\n", trie_lookup(root, "alicante")->value);
	printf("Delete 'alicante':\n");
	root = trie_delete(root, "alicante");
	trie_print(root, 0);
    return 0;
}