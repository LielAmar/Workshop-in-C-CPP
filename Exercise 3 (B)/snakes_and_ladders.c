#include "markov_chain.h"
#include <string.h> // For strlen(), strcmp(), strcpy()

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

#define ACCEPTED_ARG_COUNT(x) (x == 3)

#define USAGE_MESSAGE \
 "Usage: Please use ./snakes_and_ladders <seed> <number of paths>"

/**
* represents the transitions by ladders and snakes in the game
* each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
*/
const int transitions[][2]
   = { { 13, 4 },  { 85, 17 }, { 95, 67 }, { 97, 58 }, { 66, 89 },
       { 87, 31 }, { 57, 83 }, { 91, 25 }, { 28, 50 }, { 35, 11 },
       { 8, 30 },  { 41, 62 }, { 81, 43 }, { 69, 32 }, { 20, 39 },
       { 33, 70 }, { 79, 99 }, { 23, 76 }, { 15, 47 }, { 61, 14 } };

/**
* struct represents a Cell in the game board
*/
typedef struct Cell
{
 int number;    // Cell number 1-100
 int ladder_to; // ladder_to represents the jump of the ladder in case there
                // is one from this square
 int snake_to;  // snake_to represents the jump of the snake in case there is
                // one from this square
 // both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error (char *error_msg, MarkovChain **database)
{
 printf ("%s", error_msg);
 if (database != NULL)
 {
   free_markov_chain (database);
 }
 return EXIT_FAILURE;
}

static int create_board (Cell *cells[BOARD_SIZE])
{
 for (int i = 0; i < BOARD_SIZE; i++)
 {
   cells[i] = malloc (sizeof (Cell));
   if (cells[i] == NULL)
   {
     for (int j = 0; j < i; j++)
     {
       free (cells[j]);
     }
     handle_error (ALLOCATION_ERROR_MASSAGE, NULL);
     return EXIT_FAILURE;
   }
   *(cells[i]) = (Cell){ i + 1, EMPTY, EMPTY };
 }

 for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
 {
   int from = transitions[i][0];
   int to = transitions[i][1];
   if (from < to)
   {
     cells[from - 1]->ladder_to = to;
   }
   else
   {
     cells[from - 1]->snake_to = to;
   }
 }
 return EXIT_SUCCESS;
}

/**
* fills database
* @param markov_chain
* @return EXIT_SUCCESS or EXIT_FAILURE
*/
static int fill_database (MarkovChain *markov_chain)
{
 Cell *cells[BOARD_SIZE];
 if (create_board (cells) == EXIT_FAILURE)
 {
   return EXIT_FAILURE;
 }
 MarkovNode *from_node = NULL, *to_node = NULL;
 size_t index_to;
 for (size_t i = 0; i < BOARD_SIZE; i++)
 {
   add_to_database (markov_chain, cells[i]);
 }

 for (size_t i = 0; i < BOARD_SIZE; i++)
 {
   from_node = get_node_from_database (markov_chain, cells[i])->data;

   if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
   {
     index_to = MAX (cells[i]->snake_to, cells[i]->ladder_to) - 1;
     to_node = get_node_from_database (markov_chain, cells[index_to])->data;
     add_node_to_counter_list (from_node, to_node, markov_chain);
   }
   else
   {
     for (int j = 1; j <= DICE_MAX; j++)
     {
       index_to = ((Cell *) (from_node->data))->number + j - 1;
       if (index_to >= BOARD_SIZE)
       {
         break;
       }
       to_node = get_node_from_database (markov_chain, cells[index_to])->data;
       add_node_to_counter_list (from_node, to_node, markov_chain);
     }
   }
 }
 // free temp arr
 for (size_t i = 0; i < BOARD_SIZE; i++)
 {
   free (cells[i]);
 }
 return EXIT_SUCCESS;
}

static int run_paths_generator (int paths, MarkovChain *markov_chain);
static bool parse_integer (int *target, char *raw);

static void print_func (void *item);
static int compare_func (void *item1, void *item2);
static void free_data (void *item);
static void *copy_func (void *item);
static bool is_last (void *item);

/**
* @param argc num of arguments
* @param argv 1) Seed
*             2) Number of sentences to generate
* @return EXIT_SUCCESS or EXIT_FAILURE
*/
int main (int argc, char *argv[])
{
 if (!ACCEPTED_ARG_COUNT (argc))
 {
   printf (USAGE_MESSAGE);
   return EXIT_FAILURE;
 }

 int seed, paths;
 parse_integer (&seed, argv[1]);
 parse_integer (&paths, argv[2]);

 srand (seed);

 LinkedList linked_list = { .first = NULL, .last = NULL, .size = 0 };
 MarkovChain markov_chain = { .database = &linked_list,
                              .print_func = &print_func,
                              .comp_func = &compare_func,
                              .free_data = &free_data,
                              .copy_func = &copy_func,
                              .is_last = &is_last };
 MarkovChain *markov_chain_ptr = &markov_chain;

 return run_paths_generator (paths, markov_chain_ptr);
}

static int run_paths_generator (int paths, MarkovChain *markov_chain)
{
 fill_database (markov_chain);

 MarkovNode * first = markov_chain->database->first->data;

 for (int i = 0; i < paths; i++)
 {
   printf("Random Walk %d: ", i+1);
   generate_random_sequence (markov_chain, first, MAX_GENERATION_LENGTH);
 }

 free_markov_chain (&markov_chain);
 return EXIT_SUCCESS;
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

// ===== Node Functions =====
static void print_func (void *item)
{
 Cell *object = (Cell *) item;

 if (object->ladder_to == -1 && object->snake_to == -1)
 {
   printf ("[%d]%s", object->number,
           is_last(item) ? "" : " -> ");
 }
 else if (object->ladder_to != -1)
 {
   printf ("[%d]%s%d%s", object->number, "-ladder to ", object->ladder_to,
           is_last(item) ? "" : " -> ");
 }
 else if (object->snake_to != -1)
 {
   printf ("[%d]%s%d%s", object->number, "-snake to ", object->snake_to,
           is_last(item) ? "" : " -> ");
 }
}

static int compare_func (void *item1, void *item2)
{
 Cell *object1 = (Cell *) item1;
 Cell *object2 = (Cell *) item2;

 return (object1->number - object2->number); // TODO: maybe change this
}

static void free_data (void *item)
{
 Cell *object = (Cell *) item;
 free (object);
}

static void *copy_func (void *item)
{
 Cell *object = (Cell *) item;

 Cell *object_copy = malloc (sizeof (Cell));

 // If memory allocation failed we want to free everything and return
 if (object_copy == NULL)
 {
   return NULL;
 }

 object_copy->number = object->number;
 object_copy->ladder_to = object->ladder_to;
 object_copy->snake_to = object->snake_to;

 //  memcpy (object_copy, object, sizeof (Cell));
 return object_copy;
}

static bool is_last (void *item)
{
 Cell *object = (Cell *) item;

 return object->number == BOARD_SIZE;
}