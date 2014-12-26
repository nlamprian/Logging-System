#include <stdio.h>
#include "sort.h"

void heapSort(rec **htsorted, int array_size, int (*comp)(rec *, rec *)) {
	// heapSort: implementation of the heap sorting
	int i;
	rec *temp;
 
	for ( i = array_size / 2; i >= 0; i-- )
		siftDown(htsorted, i, array_size - 1, comp);
 
	for ( i = array_size - 1; i >= 1; i-- ) {
		temp = htsorted[0];
		htsorted[0] = htsorted[i];
		htsorted[i] = temp;
		siftDown(htsorted, 0, i-1, comp);
	}
}

void siftDown(rec **htsorted, int root, int bottom, int (*comp)(rec *, rec *)) {
	// siftDown: used by heapSort
	int done, maxChild;
	rec *temp;
 
	done = 0;
	while ( root * 2 <= bottom && !done ) {
		if ( root * 2 == bottom )
			maxChild = root * 2;
		else if ( comp(htsorted[root * 2], htsorted[root * 2 + 1]) > 0 )
			maxChild = root * 2;
		else
			maxChild = root * 2 + 1;
 
		if ( comp(htsorted[root], htsorted[maxChild]) < 0 ) {
			temp = htsorted[root];
			htsorted[root] = htsorted[maxChild];
			htsorted[maxChild] = temp;
			root = maxChild;
		} else
			done = 1;
	}
}

void heapSortinayear(rec **htsorted, int array_size, int year) {
	// heapSortinayear: implementation of the heap sorting modified to carry argument 'year'
	int i;
	rec *temp;
 
	for ( i = array_size / 2; i >= 0; i-- )
		siftDowninayear(htsorted, i, array_size - 1, year);
 
	for ( i = array_size - 1; i >= 1; i-- ) {
		temp = htsorted[0];
		htsorted[0] = htsorted[i];
		htsorted[i] = temp;
		siftDowninayear(htsorted, 0, i-1, year);
	}
}

void siftDowninayear(rec **htsorted, int root, int bottom, int year) {
	// siftDowninayear: used by heapSortinayear
	int done, maxChild;
	rec *temp;
 
	done = 0;
	while ( root * 2 <= bottom && !done ) {
		if ( root * 2 == bottom )
			maxChild = root * 2;
		else if ( cmpexpensesinayear(htsorted[root * 2], htsorted[root * 2 + 1], year) > 0 )
			maxChild = root * 2;
		else
			maxChild = root * 2 + 1;
 
		if ( cmpexpensesinayear(htsorted[root], htsorted[maxChild], year) < 0 ) {
			temp = htsorted[root];
			htsorted[root] = htsorted[maxChild];
			htsorted[maxChild] = temp;
			root = maxChild;
		} else
			done = 1;
	}
}

int cmpproducts(rec *rec1, rec *rec2) {
	// cmpproducts: compares the amount of products purchased by two cardholders
	// returns: +1, 0, -1 for rec1>rec2, rec1==rec2, rec1<rec2, respectively
	int count1, count2;

	count1 = countproducts(rec1);
	count2 = countproducts(rec2);
	return count1 <= count2 ? ( count1 < count2 ? -1 : 0 ) : 1;
}

int countproducts(rec *strct) {
	// countproducts: counts the products/services purchased with the card with the ID strct->id
	year *yptr;
	day *dptr;
	comserv *pptr;
	int count = 0;

	yptr = strct->ptr;
	while ( yptr != NULL ) {
		dptr = yptr->ptr;
		while ( dptr != NULL ) {
			pptr = dptr->ptr;
			while ( pptr != NULL ) {
				count++;
				pptr = pptr->next;
			}
			dptr = dptr->next;
		}
		yptr = yptr->next;
	}
	return count;
}

int cmpexpenses(rec *rec1, rec *rec2) {
	// cmpexpenses: compares the expenses of two cardholders
	// returns: +1, 0, -1 for rec1>rec2, rec1==rec2, rec1<rec2, respectively
	double count1, count2;

	count1 = countexpenses(rec1);
	count2 = countexpenses(rec2);
	return count1 <= count2 ? ( count1 < count2 ? -1 : 0 ) : 1;
}

double countexpenses(rec *strct) {
	// countexpenses: counts the expenses made with the card with the ID strct->id
	year *yptr;
	day *dptr;
	double exp = 0;

	yptr = strct->ptr;
	while ( yptr != NULL ) {
		dptr = yptr->ptr;
		while ( dptr != NULL ) {
			exp += dptr->expenses;
			dptr = dptr->next;
		}
		yptr = yptr->next;
	}
	return exp;
}

int cmpexpensesinayear(rec *rec1, rec *rec2, int year_) {
	// cmpexpensesinayear: modified cmpexpenses to carry the year
	double count1, count2;

	count1 = countexpensesinayear(rec1, year_);
	count2 = countexpensesinayear(rec2, year_);
	return count1 <= count2 ? ( count1 < count2 ? -1 : 0 ) : 1;
}

double countexpensesinayear(rec *strct, int year_) {
	// countexpensesinayear: modified countexpenses to count only the expenses in the specified year
	year *yptr;
	day *dptr;
	double exp = 0;

	yptr = strct->ptr;
	while ( yptr != NULL ) {
		if ( yptr->year != year_ ) {
			yptr = yptr->next;
			continue; }
		dptr = yptr->ptr;
		while ( dptr != NULL ) {
			exp += dptr->expenses;
			dptr = dptr->next;
		}
		break;
	}
	return exp;
}
