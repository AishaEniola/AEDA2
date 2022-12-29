#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//cc hashTableCreate.c   -o hashTableCreate

#define _max_word_size_  32     //size of the word in the hashtable

typedef struct adjacency_node_s  adjacency_node_t;
typedef struct hash_table_s      hash_table_t;
typedef struct hash_table_node_s hash_table_node_t;

struct adjacency_node_s
{
  adjacency_node_t *next;            // link to th enext adjacency list node
  hash_table_node_t *vertex;         // the other vertex
};

struct hash_table_node_s
{
  // the hash table data
  char word[_max_word_size_];        // the word
  hash_table_node_t *next;           // next hash table linked list node
  // the vertex data
  adjacency_node_t *head;            // head of the linked list of adjancency edges
  int visited;                       // visited status (while not in use, keep it at 0)
  hash_table_node_t *previous;       // breadth-first search parent
  // the union find data
  hash_table_node_t *representative; // the representative of the connected component this vertex belongs to
  int number_of_vertices;            // number of vertices of the conected component (only correct for the representative of each connected component)
  int number_of_edges;               // number of edges of the conected component (only correct for the representative of each connected component)
};

struct hash_table_s
{
  unsigned int hash_table_size;      // the size of the hash table array-capacity
  unsigned int number_of_entries;    // the number of entries in the hash table-length
  unsigned int number_of_edges;      // number of edges (for information purposes only)
  hash_table_node_t **heads;         // the heads of the linked lists
};

static hash_table_t *hash_table_create(void)
{
  hash_table_t *hash_table;
  unsigned int i;

  hash_table = (hash_table_t *)malloc(sizeof(hash_table_t));
  if(hash_table == NULL)
  {
    fprintf(stderr,"create_hash_table: out of memory\n");
    exit(1);
  }
  //
  // complete this
  //int size;
  hash_table->hash_table_size =_max_word_size_;
  hash_table->number_of_entries=0;
  hash_table->heads = (hash_table_node_t **)calloc(hash_table->hash_table_size, sizeof(hash_table_node_t*));

  //
  hash_table=hash_table_create();
  for (i=0; i < hash_table->hash_table_size; i++){
      hash_table->heads[i]=NULL;
  }
  return hash_table;
}

int main(int argc,char **argv)
{
    hash_table_t *hash_table;
    hash_table = hash_table_create();
    
    return 0;
}
