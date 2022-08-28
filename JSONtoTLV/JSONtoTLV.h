#ifndef _ASSIGNMENT_HEADER_
#define _ASSIGNMENT_HEADER_

#include <json-c/json.h>

int input_pointer;

int hash_pointer;

#define CAPACITY 1000 // Size of the Hash Table


typedef struct Ht_item Ht_item;

// Define the Hash Table Item here
struct Ht_item {
    char* key;
    char* value;
    char* type;

};



typedef struct HashTable HashTable;

// Define the Hash Table here
struct HashTable {
    // Contains an array of pointers
    // to items
    Ht_item** items;
    int size;
    int count;
};

void json_parse(json_object * jobj,HashTable * dict1, HashTable * dict2); /*Forward Declaration*/

char* double_to_string(double doub,char * mystr);

char * int_to_string(int type,char* mystr);

unsigned long hash_function(char* str);

Ht_item* create_item(char* key, char* value,enum json_type type);

HashTable* create_table(int size);

void free_item(Ht_item* item);

void free_item(Ht_item* item); 

void ht_insert(HashTable* table, char* key, char* value,enum json_type type);


void print_table(HashTable* table);

#endif 
        
