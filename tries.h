#pragma once

typedef struct trie {
    char *value;
    char ch;
    struct trie *sibling;
    struct trie *child;
} trie;

trie *trie_create();
trie *trie_insert(trie *root, char *key, char *value);
trie *trie_lookup(trie *root, char *key);
trie *trie_delete(trie *root, char *key);
trie *trie_delete_dfs(trie *root, trie *parent, char *key, int *found);
void trie_print(trie *root, int level);