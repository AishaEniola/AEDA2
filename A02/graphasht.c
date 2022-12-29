#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define _max_word_size_  32

typedef struct adjacency_node_s  adjacency_node_t;
typedef struct hash_table_node_s hash_table_node_t;
typedef struct hash_table_s      hash_table_t;

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
  hash_table_node_t *previous;       // breadth-first search next_node
  // the union find data
  hash_table_node_t *representative; // the representative of the connected component this vertex belongs to
  int number_of_vertices;            // number of vertices of the conected component (only correct for the representative of each connected component)
  int number_of_edges;               // number of edges of the conected component (only correct for the representative of each connected component)
};

struct hash_table_s
{
  unsigned int hash_table_size;      // the size of the hash table array
  unsigned int number_of_entries;    // the number of entries in the hash table
  unsigned int number_of_edges;      // number of edges (for information purposes only)
  hash_table_node_t **heads;         // the heads of the linked lists
};

/*----------------------------------------------------------------------------------------------------
typedef struct hash_table_node_s {
    struct hash_table_node_s * parent;    //representative
    void * value; 
    int depth;          //number_of_edges
} hash_table_node_t;
  element= node
*/

static hash_table_node_t *find_representative(hash_table_node_t *node)
{
  hash_table_node_t *representative,*next_node; //next_node=original_element
  // complete this
  node = (hash_table_node_t *)malloc(sizeof(hash_table_node_t));
  if(!node) return NULL;
    next_node = node;
    while(node->representative != NULL) //representative=parent
        node=node->representative;
    if(node != next_node->representative && node!=next_node)
        next_node->representative = node; //path compression
    return node;    //return representative;
}

static void add_edge(hash_table_t *hash_table,hash_table_node_t *from,const char *word)
{
  hash_table_node_t *to,*from_representative,*to_representative;
  adjacency_node_t *link;
  //to = find_word(hash_table,word,0);
  //int number_of_edges;=depth
  to = find_word(hash_table,word,0);    //to->root_a
  // complete this
  from_representative=find_representative(from);//from_representative->from_representative
  to_representative=to;     //new root
  if(to != from_representative){
      if(to->number_of_edges > from_representative->number_of_edges){
          from_representative->representative=to;
          to->number_of_edges++;
      }else{
          to->representative = from_representative;
          from_representative->number_of_edges++;
          to_representative = from_representative;
      }
  }
  link=to_representative;
}

static hash_table_node_t *find_word(hash_table_t *hash_table,const char *word,int insert_if_not_found)
{
  hash_table_node_t *node;
  unsigned int i;
  i = crc32(word) % hash_table->hash_table_size;
  //
  // complete this
  node = (hash_table_node_t *)malloc(sizeof(hash_table_node_t));
  hash_table = (hash_table_t *)malloc(sizeof(hash_table_t));
  hash_table = (hash_table_t *)malloc(sizeof(hash_table_t));
  if(hash_table == NULL)
  {
    fprintf(stderr,"create_hash_table: out of memory\n");
    exit(1);
  }
  for(i=0;i<;i++){
    if(node != NULL){
      if(strcmp(node[i].word, word) == 0) return &node[i];
    }
  }
  
  //
  return node;
}

static void hash_table_free(hash_table_t *hash_table)
{
  //
  // complete this
  hash_table=(hash_table_t *)malloc(sizeof(hash_table_t));
  for(int i=0;i<hash_table->hash_table_size; i++){
    hash_table_node_t *node=hash_table->node[i];
    if(node != NULL)
        free_hash_table_node(node);
  }
  //
  free(hash_table->node);
  free(hash_table);
}

