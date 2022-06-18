#ifndef _MARKOV_CHAIN_H
#define _MARKOV_CHAIN_H

#include "linked_list.h"
#include <stdbool.h> // for bool
#include <stdio.h>   // For printf(), sscanf()
#include <stdlib.h>  // For exit(), malloc()

#define ALLOCATION_ERROR_MASSAGE \
  "Allocation failure: Failed to allocate" \
  "new memory\n"

typedef struct MarkovNode MarkovNode;
typedef struct NextNodeCounter NextNodeCounter;
typedef struct MarkovChain MarkovChain;

typedef struct MarkovNode
{
  char *data;
  NextNodeCounter *counter_list;
  int possible_continuations;
} MarkovNode;

typedef struct NextNodeCounter
{
  MarkovNode *markov_node;
  int frequency;
} NextNodeCounter;

typedef struct MarkovChain
{
  LinkedList *database;
} MarkovChain;

/**
 * Get one random state from the given markov_chain's database.
 * @param markov_chain
 * @return
 */
MarkovNode *get_first_random_node (MarkovChain *markov_chain);

/**
 * Choose randomly the next state, depend on it's occurrence frequency.
 * @param state_struct_ptr MarkovNode to choose from
 * @return MarkovNode of the chosen state
 */
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr);

/**
 * Receive markov_chain, generate and print random sentence out of it. The
 * sentence most have at least 2 words in it.
 * @param markov_chain
 * @param first_node markov_node to start with,
 *                   if NULL- choose a random markov_node
 * @param  max_length maximum length of chain to generate
 */
void generate_random_sequence (MarkovChain *markov_chain,
                               MarkovNode *first_node, int max_length);

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_markov_chain (MarkovChain **ptr_chain);

/**
 * Add the second markov_node to the counter list of the first markov_node.
 * If already in list, update it's counter value.
 * @param first_node
 * @param second_node
 * @param markov_chain
 * @return success/failure: true if the process was successful, false if in
 * case of allocation error.
 */
bool add_node_to_counter_list (MarkovNode *first_node,
                               MarkovNode *second_node);

/**
 * Check if data_ptr is in database. If so, return the markov_node wrapping it
 * in the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return Pointer to the Node wrapping given state, NULL if state not in
 * database.
 */
Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr);

/**
 * If data_ptr in markov_chain, return it's markov_node. Otherwise, create new
 * markov_node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the state to look for
 * @return markov_node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node *add_to_database (MarkovChain *markov_chain, char *data_ptr);

// ===== Utilities =====
/**
 * Returns the combined number of frequencies of all possible next node
 * @param head node to calculate the possible continuations factor of
 * @return Total (combined) number of frequencies of all items in counter_list
 */
int get_possible_continuations_factor (MarkovNode *node);

/**
 * Returns the linked list's node at index ${index}
 * @param head head of the linked list
 * @param index index of the node to return
 * @return Node at index ${index}
 */
Node *get_node_at_index (Node *head, int index);

/**
 * Returns the node similar to ${target_node} that is in ${counter_list}
 * If node doesn't exist, returns NULL
 * @param counter_list list to search in
 * @param list_size size of the list
 * @param target_node target node to find
 * @return NextNodeCounter object if target_node is in list, otherwise NULL
 */
NextNodeCounter *get_node_from_counter_list (NextNodeCounter *counter_list,
                                             int list_size,
                                             MarkovNode *target_node);

/**
 * Creates a new MarkovNode object with data ${data_ptr} and returns it
 * @param data_ptr data of the MarkovNode
 * @return Created MarkovNode if allocation succeeded, NULL otherwise.
 */
MarkovNode *create_markov_node (char *data_ptr);

/**
 * Checks whether ${text} ends with a colon.
 * @param text text to check
 * @return boolean according to ${text}'s last character
 */
bool ends_with_colon (char *text);

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number maximal number to return (not including)
 * @return Random number
 */
int get_random_number (int max_number);

#endif /* markovChain_h */
