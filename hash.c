#include "hash.h"
/**
 *  File Name: hash.c
 *  Author: Rehan Nagoor Mohideen
 *  Student ID: 1100592
 *  Purpose: Contains functions for CIS2520 assignment 5
 */

struct Performance *newPerformance() {
    struct Performance *new_performance = malloc(sizeof(struct Performance));//allocate memory to the structure
    if (new_performance == NULL) { //For ERROR handling
        fprintf(stderr, "ERROR: not enough space for mallocof newPerformance\n");
        exit(0);
    }
    //initialize values to 0
    new_performance->reads = 0;
    new_performance->writes = 0;
    new_performance->mallocs = 0;
    new_performance->frees = 0;

    return new_performance;
}

struct HashTable *createTable( struct Performance *performance, unsigned int capacity,
int (*hash)( void *, int ), int (*compar)(const void *, const void *) ) {
    int i;

    struct HashTable *new_HashTable = malloc(sizeof(struct HashTable));//allocate memory to the structure
    if (new_HashTable == NULL) { //For ERROR handling
        fprintf(stderr, "ERROR: not enough space to malloc a new HashTable\n");
        exit(0);
    }
    //initialize values
    new_HashTable->capacity = capacity;
    new_HashTable->hash = hash;
    new_HashTable->compar = compar;
    new_HashTable->nel = 0;

    new_HashTable->data = malloc(sizeof(void *)*capacity);//allocating space for the capacity number fo pointers
    if (new_HashTable->data == NULL) { //For ERROR handling
        fprintf(stderr, "ERROR: not enough space to malloc a new HashTable->data\n");
        exit(0);
    }
    //setting each pointer in data to null
    for (i = 0; i < capacity; i++) {
        new_HashTable->data[i] = NULL;
    }

    performance->reads += capacity;//update the reads for each pointer set to null
    performance->mallocs++;//update mallocs by 1

    return new_HashTable;
}

void addElement( struct Performance *performance, struct HashTable *table, void *src ) {
    if (table->nel == table->capacity) {//to print an error if the table is full
        fprintf(stderr, "ERROR: hash table is full\n");
        exit(0);
    }
    
    int index = table->hash(src, table->capacity);//calculate the index of the value to be inserted
    while (table->data[index] != NULL) {//to find an empty spot to add the element
        if (index == (table->capacity-1)) {//if the index is at the last element, loop to the first
            index = 0;
        } else {
            index++;
        }
        performance->reads++;
    }
    
    table->data[index] = src;//copy the value to the table
    table->nel++;//increment number of elements
    performance->writes++;
}

int getIdx( struct Performance *performance, struct HashTable *table, void *src ) {
    int index = table->hash(src, table->capacity);//calculate the has of the value to be inserted
    int comparVal; //to store te compare value

    while (table->data[index] == NULL) {//to make sure were not comparing when its null
        if (index == (table->capacity-1)) {//if the index is at the last element, loop to the first
            index = 0;
        } else {
            index++;
        }
    }

    //handle the first case outside of the loop to handle the case where it loops back in the loop
    comparVal = (table->compar(src, table->data[index]));
    performance->reads++;
    if (comparVal == 0) {//if the value is found
        return index;
    }
    
    if (index == (table->capacity-1)) {//if the index is at the last element, loop to the first
        index = 0;
    } else {
        index++;
    }
    
    while (table->data[index] == NULL && (index != table->hash(src, table->capacity))) {//to make sure were not comparing when its null
        if (index == (table->capacity-1)) {//if the index is at the last element, loop to the first
            index = 0;
        } else {
            index++;
        }
    }

    while ((index != table->hash(src, table->capacity))) {//loop throught the rest of the list
        comparVal = (table->compar(src, table->data[index]));
        performance->reads++;
        if (comparVal == 0) {//if the value is found
            return index;
        }
        
        if (index == (table->capacity-1)) {//if the index is at the last element, loop to the first
            index = 0;
        } else {
            index++;
        }
        while (table->data[index] == NULL && (index != table->hash(src, table->capacity))) {//to make sure were not comparing when its null
            if (index == (table->capacity-1)) {//if the index is at the last element, loop to the first
                index = 0;
            } else {
                index++;
            }
        }
    }
    return -1;
}

void freeTable( struct Performance *performance, struct HashTable *table ) {
    free(table->data);
    free(table);
    performance->frees++;
}

void *getElement( struct Performance *performance, struct HashTable *table, void *src ) {
    int index = getIdx(performance, table, src);

    if (index == -1) {//if the value is not in the list
        return NULL;
    } else {//if it is in the list return the pointer to the element at that index
        return table->data[index];
    }
}

void removeElement( struct Performance *performance, struct HashTable *table, void *target ) { 
    int index = getIdx(performance, table, target);

    if (index != -1) {//if the value is in the list
        table->data[index] = NULL;//set it to null
        performance->writes++;
        table->nel--;
    }
}

int hashAccuracy( struct HashTable *table ) {
    int i;
    int difference;
    int sum = 0;
     
    for ( i = 0; i < table->capacity; i++) {
        if (table->data[i] != NULL) {
            if (i >= table->hash(table->data[i], table->capacity)) {
                difference = i - table->hash(table->data[i], table->capacity);
            } else {
                difference = table->capacity - table->hash(table->data[i], table->capacity) + i;
            }
            sum += difference;
        }
    
    }
    return sum;
}

void rehash( struct HashTable *table ) {
    int i, j;
    int rehash = 0;
     
    for ( i = 0; i < table->capacity; i++) {//for every element
        if (table->data[i] != NULL) {//if the element exists

            if (i != table->hash(table->data[i], table->capacity)) {
                j = table->hash(table->data[i], table->capacity);
                while (rehash == 0 && i!=j) {//do while the rehash is complete and it is going to rehash to a better position.
                    if (table->data[j] != NULL) {//if the spot is taken
                        if (j == (table->capacity-1)) {//if the index is at the last element, loop to the first
                            j = 0;
                        } else {
                            j++;
                        }
                    } else {//if the spot is available then rehash
                        table->data[j] = table->data[i];
                        table->data[i] = NULL;
                        rehash = 1;
                    }
                }
                rehash = 0;
            }
        }
    
    }
}