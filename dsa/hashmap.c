#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 101
#define LOAD_FACTOR 0.85

//tao node 
typedef struct Node{
  char *key;
  int data;
  struct Node *next;
} Node;

//tao hash map co capacity = INITIAL_SIZE va size = current size base on real time
typedef struct Hashmap{
  Node **table;
  int capacity;
  int size;
}Hashmap;

Hashmap *createMap(); //return map
void freeMap(Hashmap *map); //use for free map when no longer use
void insertNode(char *key, int data, Hashmap *map); //use for insert key(char *) and data
unsigned long hash(char *key, int capacity); //hash
Node *createNode(char *key, int data); //init a node
void deleteMode(Hashmap *map, char *key); //delete a node if you like
int lookUpData(Hashmap *map, char *key); //find data by key
char *lookUpKey(Hashmap *map, int data); //find key by data
void printAll(Hashmap *map); //print all map for debug
unsigned long nextSize(int capacity); //for find a better size and replace it
void resize(Hashmap *map);

int main(){

  Hashmap *myMap = createMap();

  insertNode("chanh", 12, myMap);

  printf("%d", lookUpData(myMap, "chanh"));

  //  printAll(myMap);

  freeMap(myMap);
  return EXIT_SUCCESS;
}

Hashmap *createMap(){
  Hashmap *newMap = (Hashmap *) malloc (sizeof(Hashmap));
  if(newMap == NULL){
    perror("malloc");
    return NULL;
  }

  newMap->capacity = INITIAL_SIZE;
  newMap->size = 0;
  newMap->table = calloc(newMap->capacity, sizeof(Node*));

  return newMap;
}

void freeMap(Hashmap *map){
  for(int i = 0; i < map->capacity; i++){
    Node *entry = map->table[i];
    while(entry){
      Node *temp = entry;
      entry = entry->next;
      free(temp->key);
      free(temp);   
    }
  }
  free(map->table);
  free(map);
}

unsigned long hash(char *key, int capacity){
  unsigned long index = 5381;
  int c;
  while((c = *key++)){
    index = ((index << 5) + index) + c;
  }
  return index % capacity;
}

Node *createNode(char *key, int data){
  Node *newNode = (Node*) malloc (sizeof(Node));
  newNode->data = data;
  newNode->next = NULL;
  newNode->key = strdup(key);
  return newNode;
}

void insertNode(char *key, int data, Hashmap *map){

  if((float)map->size/map->capacity >= LOAD_FACTOR){
    resize(map);
  }

  unsigned long index = hash(key, map->capacity);
  Node *entry = map->table[index];
  while(entry){
    if(strcmp(key, entry->key) == 0){
      entry->data = data; //update
      return;
    }
    entry = entry->next;
  }
    Node *newNode = createNode(key, data);
    newNode->next = map->table[index];
    map->table[index] = newNode;
    map->size++;
}

void deleteMode(Hashmap *map, char *key){
  unsigned long index = hash(key, map->capacity);
  Node *entry = map->table[index];
  Node *prev = NULL;
  while(entry){
    if(strcmp(key, entry->key) == 0){
      if(prev){
        entry->next = prev->next;
      } else {
        map->table[index] = prev->next;
      }
      free(entry->key);
      free(entry);
      map->size--;
      return;
    }
    prev = entry;
    entry = entry->next;
  }
}

int lookUpData(Hashmap *map, char *key){
  unsigned long index = hash(key, map->capacity);
  Node *entry = map->table[index];
  while(entry){
    if(strcmp(entry->key, key) == 0){
      return entry->data;
    }
    entry = entry->next;
  }
  return -1;
}

char *lookUpKey(Hashmap *map, int data){
  char *str = (char*) malloc (20 * sizeof(char));
    for(int i = 0; i < map->capacity; i++){
      Node *entry = map->table[i];
      while(entry){
        if(data == entry->data){
          strcpy(str, entry->key);
          return str;
        }
        entry = entry->next;
      }
    }
    strcpy(str, "NULL");
    return str;
}

void printAll(Hashmap *map){
  for(int i = 0; i < map->capacity; i++){
    Node *entry = map->table[i];
    while(entry){
      printf("%d %s\n", entry->data, entry->key);
      entry = entry->next;
    }
  }
}

unsigned long nextSize(int capacity){
  int arr[] = {317, 677, 1277, 2477, 4877, 7919};
  size_t size = sizeof(arr) / sizeof(arr[0]);
  for(size_t i = 0; i < size; i++){
    if(arr[i] > capacity) return arr[i];
  }
  return capacity * 2 + 1;
}

void resize(Hashmap *map){
  int oldCapacity = map->capacity;
  int newCapacity = nextSize(oldCapacity);

  Node **newTable = calloc(newCapacity, sizeof(Node*));

  for(int i = 0; i < oldCapacity; i++){
    Node *entry = map->table[i];
    while(entry){
      Node *curr = entry->next;
      unsigned long index = hash(entry->key, newCapacity);
      entry->next = newTable[index];
      newTable[index] = entry;
      entry = curr;
    }
    free(map->table);
    map->table = newTable;
    map->capacity = newCapacity;
  }
}
