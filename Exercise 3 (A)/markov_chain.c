#include <string.h>

#include "markov_chain.h"

MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  MarkovNode *node = NULL;
  // Keep on drawing random nodes until we get a node that has continuations
  // and doesn't end with a "."
  do
  {
    int index = get_random_number (markov_chain->database->size);
    node = get_node_at_index (markov_chain->database->first, index)->data;
  } while (ends_with_colon (node->data) || node->possible_continuations == 0);

  return node;
}

MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  int possibilities = get_possible_continuations_factor (state_struct_ptr);
  int index = get_random_number (possibilities);

  int i;
  for (i = 0; i < state_struct_ptr->possible_continuations; i++)
  {
    index -= (state_struct_ptr->counter_list + i)->frequency;

    if (index < 0)
    {
      break;
    }
  }

  return (state_struct_ptr->counter_list + i)->markov_node;
}

void generate_random_sequence (MarkovChain *markov_chain,
                               MarkovNode *first_node, int max_length)
{
  if (first_node == NULL)
  {
    first_node = get_first_random_node (markov_chain);
  }

  int current_length = 1;
  MarkovNode *node = first_node;

  while (current_length < max_length && !ends_with_colon (node->data))
  {
    printf ("%s ", node->data);

    node = get_next_random_node (node);
    current_length++;
  }

  // Prints the last node
  printf ("%s\n", node->data);
}

void free_markov_chain (MarkovChain **ptr_chain)
{
  Node *node = (*ptr_chain)->database->first;
  Node *next;

  while (node != NULL)
  {
    next = node->next;

    // Frees the counter_list that each MarkovNode (probably) have.
    free (node->data->counter_list);
    node->data->counter_list = NULL;

    // Frees the string allocated
    free (node->data->data);
    node->data->data = NULL;

    // Frees the data (MarkovNode object)
    free (node->data);
    node->data = NULL;

    // Frees the actual Node holding all data
    free (node);

    node = next;
  }

  // Frees LinkedList (database) and the MarkovChain object.
  (*ptr_chain)->database = NULL;
  *ptr_chain = NULL;
}

bool add_node_to_counter_list (MarkovNode *first_node, MarkovNode *second_node)
{
  NextNodeCounter *node = get_node_from_counter_list (
      first_node->counter_list, first_node->possible_continuations,
      second_node);

  if (node == NULL)
  {
    first_node->possible_continuations++;

    NextNodeCounter *new_counter_list
        = realloc (first_node->counter_list, first_node->possible_continuations
                                                 * sizeof (NextNodeCounter));

    if (new_counter_list == NULL)
    {
      return false;
    }

    first_node->counter_list = new_counter_list;

    node = (first_node->counter_list
            + (first_node->possible_continuations - 1));
    node->markov_node = second_node;
    node->frequency = 0;
  }

  node->frequency = node->frequency + 1;
  return true;
}

Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr)
{
  Node *node = markov_chain->database->first;

  while (node != NULL)
  {
    if (strcmp (node->data->data, data_ptr) == 0)
    {
      return node;
    }

    node = node->next;
  }

  return NULL;
}

Node *add_to_database (MarkovChain *markov_chain, char *data_ptr)
{
  Node *node = get_node_from_database (markov_chain, data_ptr);

  if (node != NULL)
  {
    free (data_ptr);
    return node;
  }

  MarkovNode *m_node = create_markov_node (data_ptr);
  if (m_node == NULL || add (markov_chain->database, m_node) == 1)
  {
    return NULL;
  }

  return markov_chain->database->last;
}

// ===== UTILS =====
int get_possible_continuations_factor (MarkovNode *node)
{
  int possibilities = 0;

  for (int i = 0; i < node->possible_continuations; i++)
  {
    possibilities += ((node->counter_list) + i)->frequency;
  }

  return possibilities;
}

Node *get_node_at_index (Node *head, int index)
{
  while (index > 0)
  {
    head = head->next;
    index--;
  }

  return head;
}

NextNodeCounter *get_node_from_counter_list (NextNodeCounter *counter_list,
                                             int list_size,
                                             MarkovNode *target_node)
{
  for (int i = 0; i < list_size; i++)
  {
    if ((counter_list + i)->markov_node == target_node)
    {
      return (counter_list + i);
    }
  }

  return NULL;
}

MarkovNode *create_markov_node (char *data_ptr)
{
  MarkovNode *m_node = malloc (sizeof (MarkovNode));
  if (m_node == NULL)
  {
    return NULL;
  }

  m_node->data = data_ptr;
  m_node->possible_continuations = 0;
  m_node->counter_list = NULL;

  return m_node;
}

bool ends_with_colon (char *text)
{
  return text[strlen (text) - 1] == '.';
}

int get_random_number (int max_number)
{
  return rand () % max_number;
}