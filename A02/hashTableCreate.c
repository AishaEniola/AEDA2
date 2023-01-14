//
// AED, November 2022 (Tomás Oliveira e Silva)
//
// Second practical assignement (speed run)
//
// Place your student numbers and names here
//   N.Mec. 110124  Name: Edivaldo Bonfim
//
// Do as much as you can
//   1) MANDATORY: complete the hash table code
//      *) hash_table_create
//      *) hash_table_grow
//      *) hash_table_free
//      *) find_word
//      +) add code to get some statistical data about the hash table
//   2) HIGHLY RECOMMENDED: build the graph (including union-find data) -- use the similar_words function...
//      *) find_representative
//      *) add_edge
//   3) RECOMMENDED: implement breadth-first search in the graph
//      *) breadh_first_search
//   4) RECOMMENDED: list all words belonginh to a connected component
//      *) breadh_first_search
//      *) list_connected_component
//   5) RECOMMENDED: find the shortest path between to words
//      *) breadh_first_search
//      *) path_finder
//      *) test the smallest path from bem to mal
//         [ 0] bem
//         [ 1] tem
//         [ 2] teu
//         [ 3] meu
//         [ 4] mau
//         [ 5] mal
//      *) find other interesting word ladders
//   6) OPTIONAL: compute the diameter of a connected component and list the longest word chain
//      *) breadh_first_search
//      *) connected_component_diameter
//   7) OPTIONAL: print some statistics about the graph
//      *) graph_info
//   8) OPTIONAL: test for memory leaks
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//
// static configuration
//

#define _max_word_size_ 32
int CAPACITY = 500;

//
// data structures (SUGGESTION --- you may do it in a different way)
//

typedef struct adjacency_node_s adjacency_node_t;
typedef struct hash_table_node_s hash_table_node_t;
typedef struct hash_table_s hash_table_t;

struct adjacency_node_s
{
  adjacency_node_t *next;    // link to the next adjacency list node
  hash_table_node_t *vertex; // the other vertex
};

struct hash_table_node_s
{
  // the hash table data
  char word[_max_word_size_]; // the word
  hash_table_node_t *next;    // next hash table linked list node
  // the vertex data
  adjacency_node_t *head;      // head of the linked list of adjancency edges
  int visited;                 // visited status (while not in use, keep it at 0)
  hash_table_node_t *previous; // breadth-first search parent
  // the union find data
  hash_table_node_t *representative; // the representative of the connected component this vertex belongs to
  int number_of_vertices;            // number of vertices of the conected component (only correct for the representative of each connected component)
  int number_of_edges;               // number of edges of the conected component (only correct for the representative of each connected component)
};

struct hash_table_s
{
  unsigned int hash_table_size;   // the size of the hash table array
  unsigned int number_of_entries; // the number of entries in the hash table
  unsigned int number_of_edges;   // number of edges (for information purposes only)
  hash_table_node_t **heads;      // the heads of the linked lists
};

//
// allocation and deallocation of linked list nodes (done)
//

static adjacency_node_t *allocate_adjacency_node(void)
{
  adjacency_node_t *node;

  node = (adjacency_node_t *)malloc(sizeof(adjacency_node_t));
  if (node == NULL)
  {
    fprintf(stderr, "allocate_adjacency_node: out of memory\n");
    exit(1);
  }
  return node;
}

static void free_adjacency_node(adjacency_node_t *node)
{
  free(node);
}

static hash_table_node_t *allocate_hash_table_node(void)
{
  hash_table_node_t *node;

  node = (hash_table_node_t *)malloc(sizeof(hash_table_node_t));
  if (node == NULL)
  {
    fprintf(stderr, "allocate_hash_table_node: out of memory\n");
    exit(1);
  }
  return node;
}

static void free_hash_table_node(hash_table_node_t *node)
{
  free(node);
}

//
// hash table stuff (mostly to be done)
//

unsigned int crc32(const char *str)
{
  static unsigned int table[256];
  unsigned int crc;

  if (table[1] == 0u) // do we need to initialize the table[] array?
  {
    unsigned int i, j;

    for (i = 0u; i < 256u; i++)
      for (table[i] = i, j = 0u; j < 8u; j++)
        if (table[i] & 1u)
          table[i] = (table[i] >> 1) ^ 0xAED00022u; // "magic" constant
        else
          table[i] >>= 1;
  }
  crc = 0xAED02022u; // initial value (chosen arbitrarily)
  while (*str != '\0')
    crc = (crc >> 8) ^ table[crc & 0xFFu] ^ ((unsigned int)*str++ << 24);
  return crc;
}

// hash function
unsigned long hash_function(char *str)
{
  unsigned long i = 0;
  for (int j = 0; str[j]; j++)
    i += str[j];
  return i % CAPACITY;
}

static hash_table_t *hash_table_create(void)
{
  hash_table_t *hash_table;
  unsigned int i;

  hash_table = (hash_table_t *)malloc(sizeof(hash_table_t));
  if (hash_table == NULL)
  {
    fprintf(stderr, "create_hash_table: out of memory\n");
    exit(1);
  }
  //
  // complete this
  hash_table->hash_table_size = 500;
  hash_table->number_of_entries = 0;
  hash_table->heads = (hash_table_node_t **)calloc(hash_table->hash_table_size, sizeof(hash_table_node_t *));

  for (i = 0; i < hash_table->hash_table_size; i++)
  {
    hash_table->heads[i] = NULL;
  }
  //
  return hash_table;
}

static void hash_table_grow(hash_table_t *hash_table)
{
  //
  // complete this

  hash_table_t *new_hash_table; // new hashtable

  int size_grow = hash_table->hash_table_size * 2; // size * 2
  // new_hash_table->hash_table_size = size_grow;
  // new_hash_table->number_of_entries = 0;
  hash_table_node_t **new_heads = malloc(size_grow * sizeof(hash_table_node_t *));
  // new_hash_table->heads = (malloc(new_hash_table->hash_table_size));

  if (new_heads == NULL)
  {
    fprintf(stderr, "hash_table_grow: out of memory\n");
    exit(1);
  }

  unsigned int i = 0;
  for (i = 0; i < hash_table->hash_table_size; i++)
  {
    hash_table_node_t *node = hash_table->heads[i];
    while (node != NULL)
    {
      hash_table_node_t *next_node = node->next;
      int new_idx = crc32(node->word) % size_grow;
      node->next = new_heads[new_idx];
      new_heads[new_idx] = node;
      node = next_node;
    }
  }

  free(hash_table->heads);
  hash_table->heads = new_heads;
  hash_table->hash_table_size = size_grow;
  //
}

static void hash_table_free(hash_table_t *hash_table)
{
  //
  // complete this
  for (unsigned int i = 0; i < hash_table->hash_table_size; i++)
  {

    hash_table_node_t *node = hash_table->heads[i];
    while (node != NULL)
    {
      hash_table_node_t *next_node = node->next;
      free_hash_table_node(node);
      node = next_node;
    }
  }

  //
  free(hash_table->heads);
  free(hash_table);
}

// problem resolution

static hash_table_node_t *find_word(hash_table_t *hash_table, const char *word, int insert_if_not_found)
{
  hash_table_node_t *node;
  unsigned int i;

  // comppute the index
  i = crc32(word) % hash_table->hash_table_size; // hash_function
  //
  // complete this

  // create a pointer to a new hash table item
  node = hash_table->heads[i];
  while (node != NULL)
  {
    if (strcmp(word, node->word) == 0)
    {
      return node;
    }
    node = node->next;
  }

  if (insert_if_not_found == 0)
  {
    return NULL;
  }

  node = allocate_hash_table_node();
  strcpy(node->word, word);

  node->next = hash_table->heads[i];
  hash_table->heads[i] = node;

  // grow_hash_table if table is full
  if (hash_table->number_of_entries == hash_table->hash_table_size - 5)
  {
    hash_table_grow(hash_table);
  }

  hash_table->number_of_entries++;
  //
  return node;
}

//
// add edges to the word ladder graph (mostly do be done)
//

static hash_table_node_t *find_representative(hash_table_node_t *node)
{
  hash_table_node_t *representative, *next_node;

  //
  // complete this
  for (representative = node; representative != node->representative; representative = representative->next)
    ;

  // path compression
  for (representative = node; representative != representative->representative; representative = representative->representative)
  {
    representative->representative = node->representative;
    node->representative = representative;
  }

  //
  return representative;
}

static void add_edge(hash_table_t *hash_table, hash_table_node_t *from, const char *word)
{
  hash_table_node_t *to, *from_representative, *to_representative;
  adjacency_node_t *link;

  to = find_word(hash_table, word, 0);
  //
  // complete this
  if (to == NULL)
    return;

  // cirar um novo link para ligação de from para to
  adjacency_node_t *newLink_from = allocate_adjacency_node();
  newLink_from->next = from->head;
  newLink_from->vertex = to;
  from->head = newLink_from;

  // criar link de ligação de to para from
  adjacency_node_t *newLink_to = allocate_adjacency_node();
  newLink_to->vertex = from;
  newLink_to->next = to->head;
  to->head = newLink_to;

  if (from_representative == to_representative)
  {
    return;
  }
  if (from_representative->number_of_vertices < to_representative->number_of_vertices)
  {

    // tenho que fazer o representante do compontente menor(number_of_vertices) tomar o valor do representante maior e atualizar  o respectivo(number_of_vertices)

    from_representative->number_of_edges += to_representative->number_of_edges + 1;
    from_representative->number_of_vertices += to_representative->number_of_vertices;
    from_representative->representative = to_representative;
  }
  else
  {
    to_representative->number_of_edges += from_representative->number_of_edges + 1;
    to_representative->number_of_vertices += from_representative->number_of_vertices;
    to_representative->representative = from_representative;
  }

  //
}

//
// generates a list of similar words and calls the function add_edge for each one (done)
//
// man utf8 for details on the uft8 encoding
//

static void break_utf8_string(const char *word, int *individual_characters)
{
  int byte0, byte1;

  while (*word != '\0')
  {
    byte0 = (int)(*(word++)) & 0xFF;
    if (byte0 < 0x80)
      *(individual_characters++) = byte0; // plain ASCII character
    else
    {
      byte1 = (int)(*(word++)) & 0xFF;
      if ((byte0 & 0b11100000) != 0b11000000 || (byte1 & 0b11000000) != 0b10000000)
      {
        fprintf(stderr, "break_utf8_string: unexpected UFT-8 character\n");
        exit(1);
      }
      *(individual_characters++) = ((byte0 & 0b00011111) << 6) | (byte1 & 0b00111111); // utf8 -> unicode
    }
  }
  *individual_characters = 0; // mark the end!
}

static void make_utf8_string(const int *individual_characters, char word[_max_word_size_])
{
  int code;

  while (*individual_characters != 0)
  {
    code = *(individual_characters++);
    if (code < 0x80)
      *(word++) = (char)code;
    else if (code < (1 << 11))
    { // unicode -> utf8
      *(word++) = 0b11000000 | (code >> 6);
      *(word++) = 0b10000000 | (code & 0b00111111);
    }
    else
    {
      fprintf(stderr, "make_utf8_string: unexpected UFT-8 character\n");
      exit(1);
    }
  }
  *word = '\0'; // mark the end
}

static void similar_words(hash_table_t *hash_table, hash_table_node_t *from)
{
  static const int valid_characters[] =
      {                                                                                          // unicode!
       0x2D,                                                                                     // -
       0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D,             // A B C D E F G H I J K L M
       0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,             // N O P Q R S T U V W X Y Z
       0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D,             // a b c d e f g h i j k l m
       0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A,             // n o p q r s t u v w x y z
       0xC1, 0xC2, 0xC9, 0xCD, 0xD3, 0xDA,                                                       // Á Â É Í Ó Ú
       0xE0, 0xE1, 0xE2, 0xE3, 0xE7, 0xE8, 0xE9, 0xEA, 0xED, 0xEE, 0xF3, 0xF4, 0xF5, 0xFA, 0xFC, // à á â ã ç è é ê í î ó ô õ ú ü
       0};
  int i, j, k, individual_characters[_max_word_size_];
  char new_word[2 * _max_word_size_];

  break_utf8_string(from->word, individual_characters);
  for (i = 0; individual_characters[i] != 0; i++)
  {
    k = individual_characters[i];
    for (j = 0; valid_characters[j] != 0; j++)
    {
      individual_characters[i] = valid_characters[j];
      make_utf8_string(individual_characters, new_word);
      // avoid duplicate cases
      if (strcmp(new_word, from->word) > 0)
        add_edge(hash_table, from, new_word);
    }
    individual_characters[i] = k;
  }
}

//
// breadth-first search (to be done)
//
// returns the number of vertices visited; if the last one is goal, following the previous links gives the shortest path between goal and origin
//
// criar uma queue
// estrutura da queue
/*struct Queue
{
  struct hash_table_node_t *front;
  struct hash_table_node_t *rear;
};
// function to create an empty queue
struct Queue *createQueue()
{
  struct Queue *q = (struct Queue *)malloc(sizeof(struct Queue));
  q->front = q->rear = NULL;
  return q;
}
// function to add an item to the queue
void enqueue(struct Queue *q, hash_table_node_t *v)
{
  struct QNode *temp = newQNode(v);

  if (q->rear == NULL)
  {
    q->front = q->rear = temp;
    return;
  }

  q->rear->next = temp;
  q->rear = temp;
}
// function to remove an item from the queue
struct QNode *dequeue(struct Queue *q)
{
  if (q->front == NULL)
    return NULL;

  struct QNode *temp = q->front;
  q->front = q->front->next;

  if (q->front == NULL)
    q->rear = NULL;

  return temp;
}*/

static int breadh_first_search(int maximum_number_of_vertices, hash_table_node_t **list_of_vertices, hash_table_node_t *origin, hash_table_node_t *goal)
{
  //
  // complete this
  // Initialize an array to store the queue of vertices
  hash_table_node_t *queue[maximum_number_of_vertices];
  // Initialize the front and rear indices of the queue
  int front = 0, rear = 0;

  // Initialize all visited flags of all vertices to false
  for (int i = 0; i < maximum_number_of_vertices; i++)
  {
    list_of_vertices[i]->visited = 0;
  }

  // Mark the origin vertex as visited and enqueue it
  origin->visited = 1;
  queue[rear++] = origin;

  // variable to store the current vertex
  hash_table_node_t *current_vertex;

  // variable to store the adjacent vertex
  adjacency_node_t *adjacent_vertex;

  // loop until the queue is empty
  // while (front != rear)
  while (front <= rear)
  {
    // dequeue a vertex from the front of the queue
    current_vertex = queue[front++];

    // check if the current vertex is the goal vertex
    if (current_vertex == goal)
    {
      return 1;
    }
    // for each vertex adjacent to the current vertex
    adjacent_vertex = current_vertex->head;
    while (adjacent_vertex != NULL)
    {
      // if the adjacent vertex has not been visited
      if (adjacent_vertex->vertex->visited == 0)
      {
        // mark the adjacent vertex as visited
        adjacent_vertex->vertex->visited = 1;
        adjacent_vertex->vertex->previous = current_vertex;
        // enqueue the adjacent vertex
        queue[rear++] = adjacent_vertex->vertex;
      }
      adjacent_vertex = adjacent_vertex->next;
    }
  }

  //
  return -1;
}

//
// list all vertices belonging to a connected component (complete this)
//

static void list_connected_component(hash_table_t *hash_table, const char *word)
{
  //
  // complete this
  // Find the node corresponding to the given word
  hash_table_node_t *node = find_word(hash_table, word, 0);
  if (node == NULL)
  {
    printf("The word %s does not exist in the hash table\n", word);
    return;
  }

  // Find the representative node of the connected component
  hash_table_node_t *representative = find_representative(node);

  // Iterate through all the vertices in the connected component
  hash_table_node_t *current = representative;
  while (current != NULL)
  {
    // Print the word of the current vertex
    printf("%s\n", current->word);
    current = current->next;
  }
  //
}

//
// compute the diameter of a connected component (optional)
//

static int largest_diameter;
static hash_table_node_t **largest_diameter_example;

static int connected_component_diameter(hash_table_node_t *node)
{
  int diameter;

  //
  // complete this
  diameter = 0;
  hash_table_node_t *first, *last;
  first = last = node;

  // find the first and last vertex of the component
  while (first->previous != NULL)
  {
    first = first->previous;
  }

  while (last->next != NULL)
  {
    last = last->next;
  }

  // do bfs from the first vertex and find the farthest vertex
  breadh_first_search(node->number_of_vertices, &first, first, last);
  hash_table_node_t *farthest = last;

  // do bfs from the farthest vertex and find the farthest vertex again
  breadh_first_search(node->number_of_vertices, &farthest, farthest, last);
  hash_table_node_t *second_farthest = last;

  // calculate the distance between the farthest and second farthest vertex
  while (second_farthest->previous != NULL)
  {
    diameter++;
    second_farthest = second_farthest->previous;
  }

  //
  return diameter;
}

//
// find the shortest path from a given word to another given word (to be done)
//

static void path_finder(hash_table_t *hash_table, const char *from_word, const char *to_word)
{
  //
  // complete this
  hash_table_node_t *origin = find_word(hash_table, from_word, 0);
  hash_table_node_t *goal = find_word(hash_table, to_word, 0);
  if (origin == NULL || goal == NULL)
  {
    printf("One of the words is not in the hash table\n");
    return;
  }

  if (!breadh_first_search(hash_table->number_of_entries, hash_table->heads, origin, goal))
  {
    printf("No path found between the words\n");
    return;
  }

  // Print the path
  printf("The shortest path from %s to %s is:\n", from_word, to_word);
  hash_table_node_t *current = goal;
  while (current != origin)
  {
    printf("%s ", current->word);
    current = current->previous;
  }
  printf("%s\n", origin->word);

  //
}

//
// some graph information (optional)
//

static void graph_info(hash_table_t *hash_table)
{
  //
  // complete this
  int number_of_components = 0;
  int max_component_size = 0;
  int total_component_size = 0;
  int total_number_of_edges = 0;
  hash_table_node_t *current;
  for (unsigned int i = 0; i < hash_table->hash_table_size; i++)
  {
    current = hash_table->heads[i];
    while (current != NULL)
    {
      if (current->representative == current)
      {
        number_of_components++;
        max_component_size = max(max_component_size, current->number_of_vertices);
        total_component_size += current->number_of_vertices;
        total_number_of_edges += current->number_of_edges;
      }
      current = current->next;
    }
  }
  printf("Number of connected components: %d\n", number_of_components);
  printf("Max component size: %d\n", max_component_size);
  printf("Average component size: %f\n", (float)total_component_size / number_of_components);
  printf("Number of edges in the graph: %d\n", total_number_of_edges);
  //
}

//
// main program
//

int main(int argc, char **argv)
{
  char word[100], from[100], to[100];
  hash_table_t *hash_table;
  hash_table_node_t *node;
  unsigned int i;
  int command;
  FILE *fp;

  // initialize hash table
  hash_table = hash_table_create();
  // read words
  fp = fopen((argc < 2) ? "wordlist-big-latest.txt" : argv[1], "rb");
  if (fp == NULL)
  {
    fprintf(stderr, "main: unable to open the words file\n");
    exit(1);
  }
  while (fscanf(fp, "%99s", word) == 1)
    (void)find_word(hash_table, word, 1);
  fclose(fp);
  // find all similar words
  for (i = 0u; i < hash_table->hash_table_size; i++)
    for (node = hash_table->heads[i]; node != NULL; node = node->next)
      similar_words(hash_table, node);
  graph_info(hash_table);
  // ask what to do
  for (;;)
  {
    fprintf(stderr, "Your wish is my command:\n");
    fprintf(stderr, "  1 WORD       (list the connected component WORD belongs to)\n");
    fprintf(stderr, "  2 FROM TO    (list the shortest path from FROM to TO)\n");
    fprintf(stderr, "  3            (terminate)\n");
    fprintf(stderr, "> ");
    if (scanf("%99s", word) != 1)
      break;
    command = atoi(word);
    if (command == 1)
    {
      if (scanf("%99s", word) != 1)
        break;
      list_connected_component(hash_table, word);
    }
    else if (command == 2)
    {
      if (scanf("%99s", from) != 1)
        break;
      if (scanf("%99s", to) != 1)
        break;
      path_finder(hash_table, from, to);
    }
    else if (command == 3)
      break;
  }
  // clean up
  hash_table_free(hash_table);
  return 0;
}
