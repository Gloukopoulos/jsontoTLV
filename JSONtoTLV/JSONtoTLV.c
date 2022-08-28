#include <stdio.h>
#include <stdlib.h>
#include "assignment.h"
#include <string.h>


/*printing the value corresponding to boolean, double, integer and strings*/
void print_json_value(json_object *jobj,char * key,HashTable * dict1, HashTable * dict2){
  char hp1_string[10];
  char hp2_string[10];
  char hp2_dstring[20];
  enum json_type type;
  printf("type: ");
  type = json_object_get_type(jobj); /*Getting the type of the json object*/
  switch (type) {
    case json_type_boolean: printf("json_type_boolean \n");
                        printf("value: %s \n", json_object_get_boolean(jobj)? "true \n": "false \n");
                        ht_insert(dict2,int_to_string(hash_pointer,hp2_string),json_object_get_boolean(jobj)?"true ":"false",json_type_boolean);
                        break;
    case json_type_double: printf("json_type_double \n");
                        printf("value: %lf \n", json_object_get_double(jobj));
                        ht_insert(dict2,int_to_string(hash_pointer,hp2_string),double_to_string(json_object_get_double(jobj),hp2_dstring),json_type_double);
                        break;
    case json_type_int: printf("json_type_int \n");
                        printf("value: %d \n", json_object_get_int(jobj));
                        ht_insert(dict2,int_to_string(hash_pointer,hp2_string),int_to_string(json_object_get_double(jobj),hp2_dstring),json_type_int);
                        break;
    case json_type_string: printf("json_type_string \n");
                        printf("value: %s \n", json_object_get_string(jobj));
                        ht_insert(dict2,int_to_string(hash_pointer,hp2_string),(char*)json_object_get_string(jobj),json_type_string);
                        break;
  }
  
  ht_insert(dict1,key,int_to_string(hash_pointer,hp1_string),json_type_int);
  hash_pointer++;

}

/*Parsing the json object*/
void json_parse(json_object * jobj,HashTable * dict1, HashTable * dict2) {
  enum json_type type;
  json_object_object_foreach(jobj, key, val) { /*Passing through every array element*/
    //printf("type: ",type);
    printf("key: %s \n",key);
    type = json_object_get_type(val);
    switch (type) {
      case json_type_boolean: 
      case json_type_double: 
      case json_type_int: 
      case json_type_string: print_json_value(val,key,dict1,dict2);
                           break; 
      case json_type_object: printf("json_type_object \n");
                           jobj = json_object_object_get(jobj, key);
                           json_parse(jobj,dict1,dict2); 
                           break;
      
    }
  }
} 
void ht_insert(HashTable* table, char* key, char* value,enum json_type type) {
    // Create the item
    Ht_item* item = create_item(key, value, type);

    // Compute the index
    unsigned long index = hash_function(key);

    Ht_item* current_item = table->items[index];
    
    if (current_item == NULL) {
        // Key does not exist.
        if (table->count == table->size) {
            // Hash Table Full
            printf("Insert Error: Hash Table is full\n");
            // Remove the create item
            free_item(item);
            return;
        }
        
        // Insert directly
        table->items[index] = item; 
        table->count++;
    }

    else {
        // Scenario 1: We only need to update value
        if (strcmp(current_item->key, key) == 0) {
            char type_string[10];
            strcpy(table->items[index]->value, value);
            strcpy(table->items[index]->type, int_to_string(type,type_string));
            return;
        }

        else {
          // Scenario 2: Collision
          // We will handle case this a bit later
          //handle_collision(table, index, item);
          printf("collision !");
          return;
        }
    }
}
unsigned long hash_function(char* str) {
    unsigned long i = 0;
    for (int j=0; str[j]; j++)
        i += str[j];
    return i % CAPACITY;
}

Ht_item* create_item(char* key, char* value,enum json_type type) {
    // Creates a pointer to a new hash table item
    char type_string[10];
    Ht_item* item = (Ht_item*) malloc (sizeof(Ht_item));
    item->key = (char*) malloc (strlen(key) + 1);
    item->value = (char*) malloc (strlen(value) + 1);
    item->type = (char*) malloc (strlen(type_string) + 1);
    strcpy(item->key, key);
    strcpy(item->value, value);
    strcpy(item->type, int_to_string(type,type_string));

    return item;
}

HashTable* create_table(int size) {
    // Creates a new HashTable
    HashTable* table = (HashTable*) malloc (sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**) calloc (table->size, sizeof(Ht_item*));
    for (int i=0; i<table->size; i++)
        table->items[i] = NULL;

    return table;
}
void free_item(Ht_item* item) {
    // Frees an item
    free(item->key);
    free(item->value);
    free(item->type);
    free(item);
}

void free_table(HashTable* table) {
    // Frees the table
    for (int i=0; i<table->size; i++) {
        Ht_item* item = table->items[i];
        if (item != NULL)
            free_item(item);
    }

    free(table->items);
    free(table);
}

void print_table(HashTable* table) {
    printf("\nHash Table\n-------------------\n");
    for (int i=0; i<table->size; i++) {
        if (table->items[i]) {
            printf("Index:%d, Key:%s, Value:%s, Type:%s\n", i, table->items[i]->key, table->items[i]->value,table->items[i]->type);
        }
    }
    printf("-------------------\n\n");
}

char* int_to_string(int type,char * mystr) {
    int len;
    sprintf(mystr, "%d", type); 
    return mystr;
}
char* double_to_string(double doub,char * mystr) {
    int len;
    sprintf(mystr, "%lf", doub); 
    return mystr;
}
int main() {
    FILE * fp;
    char buffer[1024];
    hash_pointer = 0;
    HashTable* dict1 = create_table(CAPACITY);
    HashTable* dict2 = create_table(CAPACITY);
    char line[1024] = {0};
    unsigned int line_count = 0;
    int num = 10;
    //char mystr[10];
    //printf("string = %s",int_to_string(num,mystr));

    fp = fopen("example_1.json","r");
    while(!feof(fp)){
        fgets(buffer, 1024,fp);
        printf("JSON string: %s \n", buffer);

        json_object * jobj = json_tokener_parse(buffer);     
        json_parse(jobj,dict1,dict2);
        
    }

    fclose(fp);
    print_table(dict1);
    print_table(dict2);

    
}	

    // fp = fopen("example_1.json","r");
    // fread(buffer, 1024, 1, fp);
    // fclose(fp);

    // parsed_json = json_tokener_parse(buffer);

   




