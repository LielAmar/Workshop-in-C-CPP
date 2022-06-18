#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "markov_chain.h"

#define MAX_TWEET_LENGTH 20
#define BUFFER_SIZE 1000
#define DELIMITERS " \n\t\r"

#define ARGS_4(x) (x == 4)
#define ARGS_5(x) (x == 5)
#define ACCEPTED_ARG_COUNT(x) (ARGS_4 (x) || ARGS_5 (x))

#define FILE_ERROR "Error: Unable to open file %s\n"
#define USAGE_MESSAGE \
  "Usage: Please use ./tweets_generator <seed> <number of tweets> <text " \
  "corpus path> [words to read]\n"

// ===== Declarations =====
static bool parse_integer (int *target, char *raw);
static int run_tweets_generator (unsigned int seed, unsigned int tweets_number,
                                 char *text_corpus_path, int words_to_read);
static int fill_database (FILE *fp, int words_to_read,
                          MarkovChain *markov_chain);
static int insert_word_to_database (MarkovChain *markov_chain, char *word,
                                    Node **previous_node, Node **current_node);

static void print_func (void *item);
static int compare_func (void *item1, void *item2);
static void free_data (void *item);
static void *copy_func (void *item);
static bool is_last (void *item);

// ===== Implementations =====
int main (int argc, char *argv[])
{
  if (!ACCEPTED_ARG_COUNT (argc))
  {
    printf (USAGE_MESSAGE);
    return EXIT_FAILURE;
  }

  int seed, tweets_number, words_to_read = -1;
  char *text_corpus_path;

  parse_integer (&seed, argv[1]);
  parse_integer (&tweets_number, argv[2]);
  text_corpus_path = argv[3];

  if (ARGS_5 (argc))
  {
    parse_integer (&words_to_read, argv[4]);
  }

  return run_tweets_generator (seed, tweets_number, text_corpus_path,
                               words_to_read);
}

static bool parse_integer (int *target, char *raw)
{
  int result = sscanf (raw, "%d", target);
  if (result != 1)
  {
    return false;
  }

  return true;
}

static int run_tweets_generator (unsigned int seed, unsigned int tweets_number,
                                 char *text_corpus_path, int words_to_read)
{
  srand (seed);
  FILE *fp = fopen (text_corpus_path, "r");

  if (fp == NULL)
  {
    printf (FILE_ERROR, text_corpus_path);
    return EXIT_FAILURE;
  }

  LinkedList linked_list = { first : NULL, last : NULL, size : 0 };
  MarkovChain markov_chain = {
    database : &linked_list,
    print_func : &print_func,
    comp_func : &compare_func,
    free_data : &free_data,
    copy_func : &copy_func,
    is_last : &is_last
  };
  MarkovChain *markov_chain_ptr = &markov_chain;

  int result = fill_database (fp, words_to_read, markov_chain_ptr);
  fclose (fp);

  if (result == EXIT_SUCCESS)
  {
    for (unsigned int i = 0; i < tweets_number; i++)
    {
      printf ("Tweet %d:", i + 1);
      generate_random_sequence (markov_chain_ptr, NULL, MAX_TWEET_LENGTH);
    }
  }

  free_markov_chain (&markov_chain_ptr);
  return (result == EXIT_SUCCESS) ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int fill_database (FILE *fp, int words_to_read,
                          MarkovChain *markov_chain)
{
  char buffer[BUFFER_SIZE];
  Node *previous_node = NULL, *current_node = NULL;

  while (fgets (buffer, BUFFER_SIZE, fp))
  {
    char *word = strtok (buffer, DELIMITERS);

    while (word != NULL)
    {
      int result = insert_word_to_database (markov_chain, word, &previous_node,
                                            &current_node);

      if (result == EXIT_FAILURE)
      {
        printf (ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
      }

      word = strtok (NULL, DELIMITERS);
      words_to_read--;

      if (words_to_read == 0)
      {
        return EXIT_SUCCESS;
      }
    }
  }

  return EXIT_SUCCESS;
}

static int insert_word_to_database (MarkovChain *markov_chain, char *word,
                                    Node **previous_node, Node **current_node)
{
  // char *word_copy = copy_func (word);
  // char *word_copy = calloc (1, strlen (word) + 1);

  // If memory allocation failed we want to free everything and return
  // if (word_copy == NULL)
  // {
  // return EXIT_FAILURE;
  // }

  *current_node = add_to_database (markov_chain, word);

  if (*previous_node != NULL)
  {
    add_node_to_counter_list ((*previous_node)->data, (*current_node)->data,
                              markov_chain);
  }

  *previous_node = *current_node;
  return EXIT_SUCCESS;
}

// ===== Node Functions =====
static void print_func (void *item)
{
  char *str = (char *) item;
  printf (" %s", str);
}

static int compare_func (void *item1, void *item2)
{
  char *str1 = (char *) item1;
  char *str2 = (char *) item2;
  return strcmp (str1, str2);
}

static void free_data (void *item)
{
  char *str = (char *) item;
  free (str);
}

static void *copy_func (void *item)
{
  char *str = (char *) item;

  char *word_copy = malloc (sizeof (char) * (strlen (str) + 1));

  // If memory allocation failed we want to free everything and return
  if (word_copy == NULL)
  {
    return NULL;
  }

  memcpy (word_copy, str, sizeof (char) * (strlen (str) + 1));
  return word_copy;
}

static bool is_last (void *item)
{
  char *str = (char *) item;

  return str[strlen (str) - 1] == '.';
}