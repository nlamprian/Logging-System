#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "types.h"

extern int hashtabsize, settings, collisions, entries;
extern rec **hashtab, **htsorted, deleted;

int hashinsert(char *id) {
	// hashinsert: finds a position in the hashtab and places a pointer to a struct rec that it creates
	// reutrns: the position in the hashtab, or -1 on failure
	int i = 0, j;

	while ( i < hashtabsize ) {   // While the cells haven't all been checked
		if ( hashtab[j = hash(id,i)] == NULL || hashtab[j] == &deleted ) {
			// If the cell is not occupied... allocate the some space
			hashtab[j] = (rec *) malloc(sizeof(rec));
			if ( hashtab[j] == NULL ) {   // Error checking
				printf("\nMemory was not allocated. Insertion failed!\n\n");
				return -1; }
			// Every element in the htsorted table is going to point to the exact structs hashtab does
			htsorted[entries] = hashtab[j];
			entries++;
			strcpy(hashtab[j]->id,id);   // Copy the ID
			hashtab[j]->ptr = NULL;   // Initialize the pointer
			return j;   // Return position on the hash table
		}else if ( strcmp(hashtab[j]->id,id) == 0 ) { // If the ids match
			return j;   // Return position on the hash table
		} else {
			collisions++;
			i++; }
	}
	printf("Error: The table is full!");
	return -1;
}

int hashsearch(char *id, int *colls) {
	// hashsearch: searches for an ID in the hashtab
	// reutrns: the position of the ID, or -1 on failure
	int i = 0, j;

	while ( i < hashtabsize ) {   // While the cells haven't all been checked
		if ( hashtab[j = hash(id,i)] == NULL )
			return -1;   // The card with this id does not exist
		// If there hasn't been a deletion at the cell, there will be a card there. Compare the two ids
		else if ( hashtab[j] != &deleted && strcmp(hashtab[j]->id, id) == 0 )
			return j;   // Return position on the hash table
		if ( colls != NULL ) *colls++;   
		/* In case you are searching for an ID to delete, note the collisions that will happen,
		 * so they can be removed from the total amount of collisions in the hash table later */
		i++;
	}
	return -1;
}

unsigned hash(char *id, int i) {
	// hashing: implements the fragmentation
	// returns: a position in the hash table
	if ( settings )
		return ( djb2hash(id) + i * sdbmhash(id) ) % hashtabsize;   // In case of double hashing
	else
		return ( djb2hash(id) + i ) % hashtabsize;   // In case of linear probing
}

unsigned djb2hash(char *str) {
	// djb2hash: the first hash function
	// returns: a position in the hash table
	unsigned long long hash = 5381;
	int c;

	while ( c = *str++ )
		hash = ( (hash << 5) + hash ) + c;
	return hash % hashtabsize;
}

unsigned sdbmhash(char *str) {
	// sdbmhash: the second hash function
	// returns: a position in the hash table
	unsigned long long hash = 0;
	int c;
	
	while ( c = *str++ )
		hash = c + (hash << 6) + (hash << 16) - hash;
	return (hash % (hashtabsize - 1)) + 1;
}

void nextprime(int p) {
	// nextprime: takes a number and computes the immediate next greater prime number
    int d;

	for ( ; ; p++ ) {
		for ( d = 2; d < p; d++ )
			if ( p % d == 0 ) break;
		if ( d == p ) break;
	}
	hashtabsize = p;
}
