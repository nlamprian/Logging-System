#ifndef SORT_H
#define SORT_H

#include "types.h"

void heapSort(rec **htsorted, int array_size, int (*comp)(rec *, rec *));
void siftDown(rec **htsorted, int root, int bottom, int (*comp)(rec *, rec *));
void heapSortinayear(rec **htsorted, int array_size, int year);
void siftDowninayear(rec **htsorted, int root, int bottom, int year);
int cmpproducts(rec *p1, rec *p2);
int countproducts(rec *strct);
int cmpexpenses(rec *p1, rec *p2);
double countexpenses(rec *strct);
int cmpexpensesinayear(rec *p1, rec *p2, int year);
double countexpensesinayear(rec *strct, int year);

#endif  // SORT_H
