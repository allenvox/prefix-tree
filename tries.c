#include "tries.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

trie *trie_create() {
    trie *node = malloc(sizeof(*node));
    if(node == NULL) {
        return NULL;
    }
    node->ch = '\0';
    node->value = NULL;
    node->sibling = NULL;
    node->child = NULL;
    return node;
}

trie *trie_insert(trie *root, char *key, char *value) {
    trie *node, *parent = NULL, *list = root;
    for (; *key != '\0'; key++) {
    /* Lookup sibling node */
        for (node = list; node != NULL; node = node->sibling) {
            if (node->ch == *key) {
                break;
            }
        }
        if (node == NULL) { /* Node not found. Add new node */
            node = trie_create();
            node->ch = *key;
            node->sibling = list;
            if (parent != NULL) {
                parent->child = node;
            } else {
                root = node;
            }
            list = NULL;
        } else { /* Node found. Move to next level */
            list = node->child;
        }
        parent = node;
    }
    /* Update value in leaf */
    if (node->value != NULL) {
        free(node->value);
    }
    node->value = strdup(value);
    return root;
}

trie *trie_lookup(trie *root, char *key) {
    trie *node, *list;
    for (list = root; *key != '\0'; key++) {
        for (node = list; node != NULL; node = node->sibling) {
            if (node->ch == *key) {
                break;
            }
        }
        if (node != NULL) {
            list = node->child;
        } else {
            printf("Not found :(\n");
            return NULL;
        }
    }
    return node;
}

trie *trie_delete(trie *root, char *key) {
    int found;
	return trie_delete_dfs(root, NULL, key, &found);
}

trie *trie_delete_dfs(trie *root, trie *parent, char *key, int *found)
{
	trie *node, *prev = NULL;
	*found = (*key == '\0' && root == NULL) ? 1 : 0;
	if (root == NULL || *key == '\0') {
		return root;
	}
	for (node = root; node != NULL; node = node->sibling) {
		if (node->ch == *key) {
			break;
		}
		prev = node;
	}
	if (node == NULL) {
		return root;
	}
	trie_delete_dfs(node->child, node, key + 1, found);
	if (*found > 0 && node->child == NULL) {
		if (prev != NULL) {
			prev->sibling = node->sibling;
		} else {
			if (parent != NULL) {
				parent->child = node->sibling;
			} else {
				root = node->sibling;
			}
		}
		free(node->value);
		free(node);
	}
	return root;
}

void trie_print(trie *root, int level) {
	for (trie *node = root; node != NULL; node = node->sibling) {
		for (int i = 0; i < level; i++) {
			printf(" ");
		}
		if (node->value != NULL) {
			printf("%c ($) (value = %s)\n", node->ch, node->value);
		} else {
			printf("%c \n", node->ch);
		}
		if (node->child != NULL) {
			trie_print(node->child, level + 1);
		}
	}
}