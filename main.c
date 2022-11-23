#include <stdio.h>
#include "tries.h"

int main() {
	trie *root = trie_insert(NULL, "arlington", "12");
	root = trie_insert(root, "arkhangelsk", "34");
	root = trie_insert(root, "arkham", "56");
	root = trie_insert(root, "albuquerque", "78");
	root = trie_insert(root, "almaty", "91");
	root = trie_insert(root, "alicante", "101");
	root = trie_insert(root, "zula", "300");
	trie_print(root, 0);

	trie *lookup = trie_lookup(root, "arkham");
	if(lookup != NULL) {
		printf("Lookup 'arkham': Found %s\n", lookup->value);
	}

	printf("Delete 'alicante':\n");
	root = trie_delete(root, "alicante");
	trie_print(root, 0);

	lookup = trie_lookup(root, "alicante");
	if(lookup != NULL) {
		printf("Lookup 'alicante': Found %s\n", lookup->value);
	}

	printf("Minimum key in tree is ");
	trie_min(root);
	printf("Maximum key in tree is ");
	trie_max(root);
    return 0;
}
