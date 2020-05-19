/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Sean Murphy
 * Date: 03/01/20
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, HashLink* next)
{
    HashLink* link = malloc(sizeof(HashLink));
    link->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(link->key, key);
    link->value = value;
    link->next = next;
    return link;
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(HashLink* link)
{
    free(link->key);
    free(link);
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(HashMap* map, int capacity)
{
    map->capacity = capacity;
    map->size = 0;
    map->table = malloc(sizeof(HashLink*) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(HashMap* map)
{
    assert(map != 0);
    //temp hashlinks
    HashLink* currLink;
    HashLink* nextLink;
    //loop through the map capacity
    for (int i = 0; i < map->capacity; i++) {
        currLink = map->table[i];
        while (currLink != 0) {
            nextLink = currLink->next;
            hashLinkDelete(currLink);
            currLink = nextLink;
        }
    }
    //Free table and set the map size to 0
    free(map->table);
    map->size = 0;

    // FIXME: implement
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    HashMap* map = malloc(sizeof(HashMap));
    hashMapInit(map, capacity);
    return map;
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(HashMap* map)
{
    hashMapCleanUp(map);
    free(map);
}

/**
 * Returns a pointer to the value of the link with the given key  and skip traversing as well. Returns NULL
 * if no link with that key is in the table.
 * 
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 *
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(HashMap* map, const char* key)
{
    //assert that map and key are not equal to zero or null
    assert(map != NULL);
    assert(key != NULL);
    //set index to Hash_Function (input to number that is in cap of map)
    int Index;
    Index = HASH_FUNCTION(key) % hashMapCapacity(map);
    HashLink* CurrentLink = map->table[Index];
    // FIXME: implement
    //loop while our currentlink is not null and return the value if not null
    while (CurrentLink != NULL) {
        if (strcmp(CurrentLink->key, key) == 0) {
            return &(CurrentLink->value);
        }
        CurrentLink = CurrentLink->next;
    }
    return NULL;
}

/**
 * Resizes the hash table to have a number of buckets equal to the given
 * capacity (double of the old capacity). After allocating the new table,
 * all of the links need to rehashed into it because the capacity has changed.
 *
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 *
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(HashMap* map, int capacity)
{
    assert(map != NULL);
    assert(capacity != NULL);
    //Create a new table and a new hash location
    struct HashMap* createTable = hashMapNew(capacity);
    struct HashLink* location;
    
    //do a treversal of links and add the new vaues to the new table!
    for (int i = 0; i < map->capacity; i++)
    {
        location = map->table[i];
        while (location != 0)
        {
            hashMapPut(createTable, location->key, location->value);
            location = location->next;
        }
    }
    //free up the map
    hashMapCleanUp(map);
    //set map table to created table
    map->table = createTable->table;
    //set map cap to created cap
    map->capacity = createTable->capacity;
    //set map size to created size
    map->size = createTable->size;
    createTable->table = 0;
    // Free the createdTable free the memory...
    free(createTable);

}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket.
 *
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(HashMap* map, const char* key, int value)
{
    //make sure that key and map are not equal to zero/null
    assert(key != 0); 
    assert(map != 0);
   // get index value from hashfunction that is withing range of map cap
    int indexValue = HASH_FUNCTION(key) % map->capacity;
    int capValue = map->capacity;
    //
    if (indexValue < 0) {
        indexValue += capValue;
    }

    //if contains then call hashmap get
    if (hashMapContainsKey(map, key)) {
        int *Value = hashMapGet(map, key);
        (*Value) += value;
    }
    else {
        HashLink *linkV = hashLinkNew(key, value, NULL);
            if (map->table[indexValue] == NULL) {
                map->table[indexValue] = linkV;
            }
            else if (map->table[indexValue] !=0) {
                HashLink* LinkX = map->table[indexValue];
                while (LinkX->next) {
                    LinkX = LinkX->next;
                }
                LinkX->next = linkV;
            }
            map->size++;
        }
    if (hashMapTableLoad(map) >= (float) MAX_TABLE_LOAD) {
        resizeTable(map, 2 * map->capacity);
    }
  
}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(HashMap* map, const char* key)
{
    int hashIndex = HASH_FUNCTION(key) % map->capacity;

    if (hashIndex < 0) {
        hashIndex = hashIndex + hashMapSize(map);
    }
    // current link set to the beginning of the list that we hash to and the prev position
    struct HashLink *LinkV = map->table[hashIndex];
    struct HashLink *prevValue = 0;

    // Check to see if the link value is being searched for 
    if (strcmp(LinkV->key, key) == 0) {
        //check for value and if found at first entry point set beginning to the next entry value
        if (prevValue == 0) {
            map->table[hashIndex] = LinkV->next;
        }
        // Check for middle value if true then prevValue->next = LinkV->next
        else {
            prevValue->next = LinkV->next;
        }
    }
    hashLinkDelete(LinkV);
    // decrement.
    map->size--;
    return;
}


/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 *
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(HashMap* map, const char* key)
{
    // FIXME: implement
    // make sure that map and key are not zero or null
    assert(map != NULL);
    assert(key != NULL);
    //grab a index value that is within map->cap using hash_function
    int Index = HASH_FUNCTION(key) % map->capacity;
    struct HashLink* currentValue = map->table[Index];

    while (currentValue != NULL) {
        if (strcmp((currentValue->key), key) == 0){
            return 1; //return 1 if the given key is in table
        }
        currentValue = currentValue->next;
    }
    return 0; // return 0 otherwise
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(HashMap* map)
{
    //return map size
    assert(map != NULL);
    return map->size;
    // FIXME: implement
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(HashMap* map)
{
    //assert map is not zero and return the cap!
    // FIXME: implement
    assert(map != NULL);
    return map->capacity;
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(HashMap* map)
{
    
    assert(map != 0);
    int isEmptyBucket = 0;
    //count our empty buckets! for size cap
    for (int i = 0; i < map->capacity; i++) {
        if (map->table[i] == NULL) {
            isEmptyBucket++;
        }
    }
    // FIXME: implement
    //return the emptybuckets!
    return isEmptyBucket;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(HashMap* map)
{
    //assert map is not null then get map size(number of links) and cap (number of buckets) then return the divided links/buckets value
    assert(map != 0);
    float links = (float) hashMapSize(map);
    float buckets = (float)hashMapCapacity(map);
    float store = links / buckets;
    // FIXME: implement
    return store;
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(HashMap* map)
{
  
    for (int i = 0; i < map->capacity; i++) {
        HashLink* LinkValue = map->table[i];
          
        //print bucket number!
         printf("\nBucket %i:", i);

        //print the key and the value
        while (LinkValue != 0) {
        printf("(%s,%d)", LinkValue->key, LinkValue->value);
        LinkValue = LinkValue->next;
        }
    }

}
