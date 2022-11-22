#include "tries.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

trie *trie_create() { // initialize empty node
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
    for (; *key != '\0'; key++) { // loop through all characters
        for (node = list; node != NULL; node = node->sibling) {
            if (node->ch == *key) { // lookup for char through siblings
                break; // if found - ok, leave the loop
            }
        }
        if (node == NULL) { // if node with (*key) letter not found, create one
            node = trie_create();
            node->ch = *key;

            /* unordered insert
            node->sibling = list; // this level node is sibling for our new node
            if (parent != NULL) { // parent exists
                parent->child = node; // node becomes his child
            } else { // parent is NULL, new node is now root
                root = node;
            }*/

            // ordered
            if(parent != NULL) { // parent exists
                if(list == NULL || list->ch > node->ch) { // child is NULL or is larger lexicographically then new node
                    node->sibling = list;
                    parent->child = node;
                } else { // child exists and is smaller then new node
                    while(list->sibling != NULL && (list->ch - node->ch < 0)) {
                        list = list->sibling; // find alphabetic position for node
                    }
                    // insert new node between two other
                    node->sibling = list->sibling;
                    list->sibling = node;
                }
            } else if (root) {
                if (root->ch > node->ch) {
                    node->sibling = root;
                    root = node;
                } else {
                    while (list->sibling && (list->sibling->ch < node->ch)) {
                        list = list->sibling;
                    }

                    node->sibling = list->sibling;
                    list->sibling = node;
                }
            } else {
                // root is NULL, new node is now root
                root = node;
            }
            list = NULL;
        } else { // node found, move to next level
            list = node->child;
        }
        parent = node; // our node becomes the parent/previous (the next level becomes current)
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

void trie_min(trie *root) {
    trie *node = root;
    while(node->child != NULL) {
        printf("%c", node->ch);
        node = node->child;
    }
    printf("%c ($) (value = %s)\n", node->ch, node->value);
}

void trie_max(trie *root) {
    trie *node = root;
    while(node->sibling != NULL) {
        node = node->sibling;
    }
    printf("%c", node->ch);
    if(node->value != NULL) {
        printf(" ($) (value = %s)\n", node->value);
    } else {
        trie_max(node->child);
    }
}

trie *trie_delete(trie *root, char *key) {
    int found; // variable for found/not marker
	return trie_delete_dfs(root, NULL, key, &found);
}

trie *trie_delete_dfs(trie *root, trie *parent, char *key, int *found)
{
	trie *node, *prev = NULL;
	*found = (*key == '\0' && root == NULL) ? 1 : 0; // if root is empty and key is 0, we went to the end of the key -> found
	if (root == NULL || *key == '\0') { // if root is empty or we're at the end of the key - nothing to do
		return root;
	}
	for (node = root; node != NULL; node = node->sibling) {
		if (node->ch == *key) { // lookup through siblings
			break; // if found - ok, leave the loop
		}
		prev = node; // current becomes previous node
	}
	if (node == NULL) {
		return root; // if node node not found - nothing to do
	}
	trie_delete_dfs(node->child, node, key + 1, found); // delete the sequence starting from next char (node)
	if (*found > 0 && node->child == NULL) { // if node is found and his child is empty
		if (prev != NULL) { // if previous node exists - override the sibling of previous to sibling of removable node
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
