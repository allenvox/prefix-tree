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
        for (node = list; node != NULL; node = node->sibling) {
            if (node->ch == *key) { // lookup through siblings
                break; // if found - ok, leave the loop
            }
        }
        if (node == NULL) { // if node with (*key) letter not found, create one
            node = trie_create();
            node->ch = *key;
            node->sibling = list; // this level node is sibling for our new node
            if (parent != NULL) { // parent exists
                parent->child = node;
            } else { // parent is NULL -> our new node is root of the entire tree
                root = node;
            }
            list = NULL; // leaf node - no next levels
        } else { // node found, move to next level
            list = node->child;
        }
        parent = node; // parent is now the previous level
    }
    if (node->value != NULL) { // if value already exists - override
        free(node->value);
    }
    node->value = strdup(value); // enter duplicate value to the node
    return root;
}

trie *trie_lookup(trie *root, char *key) {
    char *copy = strdup(key); // duplicate the key for printing 'not found' case
    trie *node;
    for (trie *list = root; *key != '\0'; key++) {
        for (node = list; node != NULL; node = node->sibling) {
            if (node->ch == *key) { // lookup through siblings
                break; // if found - ok, leave the loop
            }
        }
        if (node != NULL) { // if found the current ch - go to the next level
            list = node->child;
        } else { // if not found ch on current level - not found the entire key
            printf("'%s' not found :(\n", copy);
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
	if (root == NULL || *key == '\0') { // if removable node is empty - nothing to do
		return root;
	}
	for (node = root; node != NULL; node = node->sibling) {
		if (node->ch == *key) { // lookup through siblings
			break; // if found - ok, leave the loop
		}
		prev = node; // current becomes previous node
	}
	if (node == NULL) {
		return root; // if node is empty - nothing to do
	}
	trie_delete_dfs(node->child, node, key + 1, found); // delete the next char
	if (*found > 0 && node->child == NULL) { // if node is found and his child is empty
		if (prev != NULL) { // if previous node exists - override the siblings without removable node
			prev->sibling = node->sibling;
		} else { // if previous node doesn't exist
			if (parent != NULL) { // if parent exists - override
				parent->child = node->sibling;
			} else { // if no parent - we're removing root, so put the new root
				root = node->sibling;
			}
		}
		free(node->value); // free the removable node value
		free(node); // deletion of the entire node
	}
	return root;
}

void trie_print(trie *root, int level) {
	for (trie *node = root; node != NULL; node = node->sibling) { // loop on all siblings at this level
		for (int i = 0; i < level; i++) {
			printf(" "); // tab for each level
		}
		if (node->value != NULL) { // end of the key
			printf("%c ($) (value = %s)\n", node->ch, node->value);
		} else {
			printf("%c \n", node->ch); // print char for this level
		}
		if (node->child != NULL) { // go to the next level
			trie_print(node->child, level + 1);
		}
	}
}