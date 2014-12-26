#ifndef TYPES_H
#define TYPES_H

typedef struct rec {
	char id[26];
	struct year *ptr;
} rec;

typedef struct year {
	int year;
	struct year *next;
	struct day *ptr;
} year;

typedef struct day {
	int day;
	int enterprise;
	double expenses;
	struct comserv *ptr;
	struct day *next;
} day;

typedef struct comserv {
	char id[5];
	struct comserv *next;
} comserv;

#endif  // TYPES_H
