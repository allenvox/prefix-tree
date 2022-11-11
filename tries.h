#pragma once

typedef struct trie {
    char *value;
    char ch;
    struct trie *sibling; // linked list prototype
    struct trie *child; // only first child - next are in siblings
} trie;

trie *trie_create();
trie *trie_insert(trie *root, char *key, char *value);
trie *trie_lookup(trie *root, char *key);
trie *trie_delete(trie *root, char *key);
trie *trie_delete_dfs(trie *root, trie *parent, char *key, int *found);
void trie_print(trie *root, int level);