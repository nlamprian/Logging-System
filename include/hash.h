#ifndef HASH_H
#define HASH_H

int hashinsert(char *key);
int hashsearch(char *key, int *collisions);
unsigned hash(char *key, int i);
unsigned djb2hash(char *str);
unsigned sdbmhash(char *str);
void nextprime(int p);

#endif  // HASH_H
