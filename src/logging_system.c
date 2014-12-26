#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>
#include "logging_system.h"
#include "types.h"
#include "hash.h"
#include "sort.h"

int hashtabsize, settings, temp_set, collisions = 0, entries = 0, numentrfile;

rec **hashtab, **htsorted;
rec deleted;  /* For every entry in the hash table that is being removed,
               * its pointer is going to point to deleted */

int main(int argc, char *argv[]) {
	enum {InsFile = '1', DelFile = '2', NumColl = '3', LoadFactor = '4',
		  DetailCard = '5', Search = '6', BBuyers = '7', Settings = '8', Exit = '9'};   // Choices of the menu
	int select;
	deleted.ptr = NULL;

	intro();
	initload();
	menu();
	while ( (select = getch()) != Exit ) {
		switch ( select ) {
			case InsFile:
				putchar(select);
				selection1();
				break;
			case DelFile:
				putchar(select);
				selection2();
				break;
			case NumColl:
				putchar(select);
				selection3();
				break;
			case LoadFactor:
				putchar(select);
				selection4();
				break;
			case DetailCard:
				putchar(select);
				selection5();
				break;
			case Search:
				putchar(select);
				selection6();
				break;
			case BBuyers:
				putchar(select);
				selection7();
				break;
			case Settings:
				putchar(select);
				setsettings();
				break;
			default:
				continue;
		}
		menu();
	}
		
	return 0;
}

void intro(void) {
	// intro: a welcome sign
	printf("\n    ___________________________________\n");
	printf("   /***********************************\\\n");
	printf("   |**************  CSL Lab Project  **|\n");
	printf("   |**  _____  ************************|\n");
	printf("   |** |_ |__| ****  Logging System  **|\n");
	printf("   |** | |___| ************************|\n");
	printf("   |** |___|_| ************************|\n");
	printf("   |**         ************************|\n");
	printf("   |***********************************|\n");
	printf("   |***  DE5429G77678945675476139R  ***|\n");
	printf("   |***********************************|\n");
	printf("   |***************  VALID 07/23  *****|\n");
	printf("   |***********************************|\n");
	printf("   \\__*__*__*__*__*__*__*__*__*__*__*__/\n\n");
}

void initload(void) {
	// initload: takes the name of the file containing the purchases and creates the hash table
	FILE *fp;
	char filename[30];

	initset();   // Set the option for the probe sequences
	do {
		puts("\n\n\nType in the name of the file containing the records of purchases");
		puts("================================================================");
		puts("[Note: Type the name without its file extension]");
		puts("================================================");
		gatherfilename(filename);   // Take the name of the file with the purchases
		fp = fopen(filename, "r");   // Error checking
		if ( fp != NULL ) break;
		printf("\nThe file %s was not accepted.\nType in the name again.\n", filename);
	} while ( fp == NULL );
	fclose(fp);
	insertprocedure(filename);   // Initiate the procedure for getting the information
}

void initset(void) {
	// initset: takes the choice for the probe sequences
	int c;

	puts("\n\nChoose your preferable technique for computing the Probe Sequences");
	puts("==================================================================");
	puts("[Press 1 or 2 for the choices]");
	puts("==============================");
	puts("1. Linear Probing");
	puts("2. Double Hashing");
	printf("\nSelect: ");
	c = getch();
	while ( c != '1' && c != '2' ) c = getch();
	printf("%c\n", c);
	switch ( c ) {   // Set the value for the technique of computation of the probe sequences
	case '1':
		temp_set = 0;
		break;
	case '2':
		temp_set = 1;
	}
}

void gatherfilename(char *filename) {
	// gatherfilename: gets a filename from the input
	int c, i = 0;

	printf(" Type here: ");
	while ( (c = getchar()) != '\n' || !i ) {
		if ( c == '\n' && !i ) printf(" Type here: ");   // Don't accept #zero characters
        if ( isspace(c) )   // Ignore any spaces or tabs
			continue;
		/* There is no need for any further error checking. Mistakes in the filenames
		 * will be recognized by a later use of fopen as failures to open the files. */
		filename[i++] = c;   // Write down the character
	}   // The filename has been taken
	filename[i++] = '.';   // Complete the string with the extension
	filename[i++] = 't';
	filename[i++] = 'x';
	filename[i++] = 't';
	filename[i] = '\0';   // And null-terminate it
}

void menu(void) {
	// menu: displays the main menu of the program
	puts("\n\n Menu:   [Press 1 through 9 for the choices]");
	puts("  1. Insert a new file of Purchases");
	puts("  2. Delete cards in the hash table");
	puts("  3. Display the number of collisions");
	puts("  4. Display the load factor in the hash table");
	puts("  5. Display the details of a card ID");
	puts("  6. Search for data about a card or a set of cards");
	puts("  7. Display the cards with the highest usage");
	puts("  8. Settings");
	puts("  9. Exit\n");
	printf("Select: ");
}

void selection1(void) {
	// selection1: takes a new file with purchases, deletes the current hash table and creates a new one
	FILE *fp;
	int cert, key;
	char filename[30], id[26];

	puts("\n\n\nInserting a new file is going to delete every record in the hash table.");
	puts("=======================================================================");
	printf("Are you sure you want to proceed? (Y/N): ");
	do {   // Error checking
		cert = toupper(getch());
	} while ( cert != 'Y' && cert != 'N' );
	putchar(cert);
	if ( cert == 'N' ) {
		printf("\n\n[Press any key to continue] ");
		getch();   // Wait
		putchar('\n');
		return; }   // Return to the main menu
	puts("\n\n\nType in the name of the file containing the records of purchases");
	puts("================================================================");
	puts("[Note: Type the name without its file extension]");
	puts("================================================");
	gatherfilename(filename);   // Take the filename
	fp = fopen(filename, "r");   // Error checking
	if ( fp == NULL ) {
		printf("\nThe file %s was not accepted.\n", filename);
		printf("\n[Press any key to continue] ");
		getch();   // Wait
		putchar('\n');
		return; }
	fclose(fp);
	// The filename is valid. Now, delete everything in memory...
	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
	printf("\nDeleting... ");
	fp = fopen("id_record.txt", "r");
	while ( !feof(fp) ) {
		fgets(id,26,fp);
		key = hashsearch(id, NULL);
		if ( key == -1 ) continue;   // If this card has already been deleted, then move on
		deleteprocedure(key);
	}
	fclose(fp);
	free(hashtab);   // free the hash table
	free(htsorted);   // free the sorted table
	collisions = 0;   // Restore the counters
	entries = 0;
	puts("Done!");
	// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
	insertprocedure(filename);   // ... and initiate the inserting procedure 
}

void insertprocedure(char *filename) {
	// insertprocedure: builds the entire hash table
	FILE *fp, *fp2;
	char *line, *data, s1[300], s2[26];
	int i, j, key, day_, year_, entcode, final;
	double expenses;
	year *listptr;
	day *dtemp;
	comserv *ptemp;
	
	// Build the hash table = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = 
	line = s1, data = s2;   // It wouldn't take this definition: char (*line)[200], (*data)[26];
	fp = fopen(filename, "r");   // Open the "filename" for reading
	if ( fp == NULL ) {   // Error checking
		printf("\nError: %s was not opened.\n", filename);
		return;
	} else
		printf("\n%s was accepted.\nRunning... ", filename);
	fp2 = fopen("id_record.txt", "w");   // Keep the IDs as a backup, in case the table needs to be deleted
	settings = temp_set;   // Set settings
	fgets(line, 200, fp);   // Take the first line (Number of different cards IDs)
	numentrfile = atoi(line);
	nextprime( 2 * numentrfile );   // Define the size of the hash table
	hashtab = (rec **) malloc( hashtabsize * sizeof(rec *) ); // Allocate space for it
	htsorted = (rec **) malloc( numentrfile * sizeof(rec *) ); // Allocate space for the sorted table
	for ( i = 0; i < hashtabsize; i++ ) {
		hashtab[i] = NULL;   // Initialize the hash table
	}
	// Start reading lines (purchases) = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
	while ( !feof(fp) ) {   // While you haven't met the end-of-file, act as follows
		fgets(line, 200, fp);   // Take a line from the file
		i = 0;
		final = 1;   // It has to do with the final product in the line (Explained later)
		while ( isspace(*(line + i)) ) i++;   // Ignore any white spaces, and in the first non-
		                                      // white space character that will be received, move on
		for ( j = 0; *(line + i) != ';'; i++, j++ )   // Collect ID
			*(data + j) = *(line + i);   // Copy the ID to data
		*(data + j) = '\0';   // null-terminate data
		key = hashinsert(data);   // Insert the ID and get the key to the table
		fputs(data, fp2);   // Write down the ID to the backup file
		i++;   // Ignore the ';' = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		for ( j = 0; *(line + i) != ';'; i++, j++ )   // Collect the day of the year
			*(data + j) = *(line + i);   // Copy the day to data
		*(data + j) = '\0';   // null-terminate data
		day_ = atoi(data);   // Hold the day
		i++;   // Ignore the ';' = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		for ( j = 0; *(line + i) != ';'; i++, j++ )   // Collect the year
			*(data + j) = *(line + i);   // Copy the year to data
		*(data + j) = '\0';   // null-terminate data
		year_ = atoi(data);   // Hold the year
		i++;   // Ignore the ';' = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		for ( j = 0; *(line + i) != ';'; i++, j++ )   // Collect the enterprise code
			*(data + j) = *(line + i);   // Copy the enterprise code to data
		*(data + j) = '\0';   // null-terminate data
		entcode = atoi(data);   // Hold the enterprise code
		i++;   // Ignore the ';' = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		for ( j = 0; *(line + i) != ';'; i++, j++ )   // Collect the expenses
			*(data + j) = *(line + i);   // Copy the expenses to data
		*(data + j) = '\0';   // null-terminate data
		expenses = atof(data);   // Hold the expenses
		i++;   // Ignore the ';' = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		listptr = hashtab[key]->ptr;
		if ( listptr == NULL ) {   // If there is no list... start one
			listptr = (year *) malloc(sizeof(year));   // Build a year
			listptr->year = year_;   //Set the year
			listptr->next = NULL;
			listptr->ptr = (day *) malloc(sizeof(day));   // Build a day
			listptr->ptr->day = day_;   // Set the day
			listptr->ptr->enterprise = entcode;   // Set the enterprise code
			listptr->ptr->expenses = expenses;   // Set the expenses
			listptr->ptr->ptr = NULL;   // Initialize pointer to comserv
			listptr->ptr->next = NULL;
			hashtab[key]->ptr = listptr;   // Make listptr's pointed struct the head of the list of years
		} else {   // Otherwise, a list is going to be there, and then...
			do {
				if ( listptr->year == year_ ) {   // ... check the year values... (if positive)
					dtemp = (day *) malloc(sizeof(day));   // Build a day
					dtemp->day = day_;   // Set the day
					dtemp->enterprise = entcode;   // Set the enterprise code
					dtemp->expenses = expenses;   // Set the expenses
					dtemp->ptr = NULL;   // Initialize pointer to comserv
					dtemp->next = listptr->ptr;   // Make dtemp to point to the head of the list of days
					listptr->ptr = dtemp;   // Make dtemp's pointed struct the new head of the list of days
					break;
				} else
					listptr = listptr->next;   // ... and if they don't match, check the next element of the list
			} while ( listptr != NULL );
			if ( listptr == NULL ) {   // At the end, if the year wasn't in the list, add it
				listptr = (year *) malloc(sizeof(year));   // Build a year
				listptr->year = year_;   // Set the year
				listptr->next = hashtab[key]->ptr;   // Make listptr to point to the head of the list of years
				listptr->ptr = (day *) malloc(sizeof(day));   // Build a day
				listptr->ptr->day = day_;   // Set the day
				listptr->ptr->enterprise = entcode;   // Set the entreprise code
				listptr->ptr->expenses = expenses;   // Set the expenses
				listptr->ptr->ptr = NULL;   // Initialize pointer to comserv
				listptr->ptr->next = NULL;
				hashtab[key]->ptr = listptr;   // Make listptr's pointed struct the new head of the list of years
			}
		}
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// By now we have created everything but the list of products and services. The pointer for that list is
		// listptr->ptr->ptr, and it's going to be used right away = = = = = = = = = = = = = = = = = = = = = = = =
		for ( j = 0; (*(line + i) != '\0' && *(line + i) != '\n') ||
			         (final-- && (*(line + i) = ';') && (*(line + i + 1) = '\n')); i++, j++ ) {
			/* When last product apears, after the program reads its name, the first expresion is
			 * going to fail, but the second one will hold the loop for one more iteration.
			 * (final-- && *(line + i) = ';' && *(line + i + 1) = '\n'): Here, the first command decrements 
			 * the variable final and on the second iteration will fail the whole expression, the second one 
			 * makes sure that control will pass the if statement inside the loop, and the third one will
			 * terminate the loop on the second iteration */
			if ( *(line + i) == ';' ) {   // When you've got the product ID
				*(data + j) = '\0';   // null-terminate data
				ptemp = (comserv *) malloc(sizeof(comserv));  // Allocate space for a comserv
				strcpy(ptemp->id,data);   // Copy data to id
				ptemp->next = listptr->ptr->ptr;   // Make ptemp to point to the head of the list
				listptr->ptr->ptr = ptemp;   // Make ptemp's pointed struct the new head of the list
				j = -1;   // Prepare for the new product or service to come
				continue;
			}
			*(data + j) = *(line + i);   // Copy the product/service to data
		}
	}
	fclose(fp);
	fclose(fp2);
	puts("Done!");
	printf("\n[Press any key to continue] ");
	getch();   // Wait
	putchar('\n');
}

void selection2(void) {
	// selection2: gets the name of the file containing the cards for deletion, and then deletes them from the hashtab
	FILE *fp;
	int key, count = 0, colls = 0, numdels;
	char filename[30], id[28];
	
	if ( !select2() ) return;   // Return to the main menu
	puts("\n\nType in the name of the file containing the IDs of the cards to delete.");
	puts("=======================================================================");
	puts("[Note: Type the name without its file extension]");
	puts("================================================");
	gatherfilename(filename);   // Take the filename
	fp = fopen(filename, "r");   // Error checking
	if ( fp == NULL ) {
		printf("\nThe file %s was not accepted.\n", filename);
		printf("\n[Press any key to continue] ");
		getch();   // Wait
		putchar('\n');
		return;
	}
	printf("\n%s was accepted.\n\n", filename);
	fgets(id,28,fp);   // Ignore the first line for now
	while ( !feof(fp) ) {   // While you haven't met the end-of-file, act as follows
		fgets(id,28,fp);   // Take an ID from the file
		id[25] = '\0';   // Replace '\n'
		key = hashsearch(id, &colls);   // Search for the ID
		if ( key == -1 ) {
			printf("     Error: %s does not exist.\n", id);
			continue; }
		deleteprocedure(key);   // Delete all the structs related
		entries--;   // Decrement total entries
		collisions -= colls;  // Subtract the number of collisions concerned with the id
		colls = 0;   // Restore the counter
		count++;
		printf("INC");
	}
	rewind(fp);
	fgets(id,28,fp);   // Read the first line again
	fclose(fp);
	numdels = atoi(id);   // Hold the number of requests for deletions
	printf("\nThere have been %d successful deletions out of %d requests.\n", count, numdels);
	printf("\n[Press any key to continue] ");
	getch();   // Wait
	putchar('\n');
}

int select2(void) {
	// select2: gets an ID from the user, and deletes every struct in the hashtab related with this ID
	// returns: 0 for returning t the main menu, or 1 for the second selection of the secondary menu
	int c, key, colls = 0;
	char id[26];

	printf("\n\n         [Press 1 or 2 for the choices]\n");
	printf("  2.1. Delete one specific card (Insert the ID from the keyboard)\n");
	printf("  2.2. Delete a set of cards (Insert the IDs from a file)\n");
	printf("  2.3. Return to the main menu\n\n");
	printf("Select: ");

	while( 1 ) {
		c = getch();
		if ( c != '1' && c != '2' && c != '3' ) continue;
		putchar(c);
		if ( c == '3' ) {
			putchar('\n');
			return 0; }   // Return to the main menu
		else if ( c == '2' ) return 1;
		else break;
	}
	puts("\n\n[Note: Press Escape to abort]");
	puts("Insert the card ID to delete");
	puts("============================");
	if ( collectid(id) != 27 ) {   // Get the card ID, and if Escape hasn't been pressed, continue
		key = hashsearch(id, &colls);   // Search for the ID
		if ( key == -1 ) {   // Error checking
			printf("\n     Error: %s does not exist.\n", id);
			printf("\n[Press any key to continue] ");
			getch();   // Wait
			putchar('\n');
			return 0;   // Return to the main menu
		}
		deleteprocedure(key);   // Delete the entry
		entries--;   // Decrement total entries
		collisions -= colls;  // Subtract the number of collisions concerned with the id
		puts("\nThere has been 1 successful deletion.");
	}
	printf("\n[Press any key to continue] ");
	getch();   // Wait
	putchar('\n');
	return 0;   // Return to the main menu
}

void deleteprocedure(int key) {
	// deleteprocedure: deletes every struct related with the entry of the hashtab, being in the position key
	year *yptr, *p;
	day *dptr, *q;
	comserv *pptr, *r;

	for ( yptr = hashtab[key]->ptr; yptr != NULL; yptr = p ) {
		// free the list of years
		for ( dptr = yptr->ptr; dptr != NULL; dptr = q ) {
			// free the list of days of the current year
			for ( pptr = dptr->ptr; pptr != NULL; pptr = r ) {
				// free the list of products and services of the current day of the current year
				r = pptr->next;
				free(pptr);
			}
			q = dptr->next;
			free(dptr);
		}
		p = yptr->next;
		free(yptr);
	}
	free(hashtab[key]);   // free the current rec of the hash table
	hashtab[key] = &deleted;   // Signify the cell as one that there's been a deletion on
}

void selection3(void) {
	// selection3: displays the number of collisions in the hashtab, and the number of entries
	printf("\n\n\nThe number of collisions in the hash table");
	printf("\n\tis %d out of %d different entries.\n", collisions, entries);
	printf("\n[Press any key to continue] ");
	getch();   // Wait
	putchar('\n');
}

void selection4(void) {
	// selection4: displays the load factor of the hashtab
	printf("\n\n\nThe load factor is %.3f.\n", (double) entries / hashtabsize);
	printf("\n[Press any key to continue] ");
	getch();   // Wait
	putchar('\n');
}

void selection5(void) {
	// selection5: handles the selection 5 of the main menu
	char id[26];

	puts("\n\n\n[Note: Press Escape to abort]");
	puts("Insert the ID and the details will appear!");
	puts("==========================================");
	if ( collectid(id) != 27 )   // Get the card ID, and if Escape hasn't been pressed, continue
		displaydetails(id);   // The card ID has been received. Now display the details
	printf("\n[Press any key to continue] ");
	getch();   // Wait
	putchar('\n');
}

int collectid(char *id) {
	// collectid: writes, the ID entered, in the variable id
	// returns: 'escape' in a cancellation, or 1 on success
	int c, i = 0;

	printf(" Type here: ");
	while ( c = getch() ) {
		if ( c == 27 ) {
			puts("   X");
			return c; }   // If Escape was pressed, return
		if ( isspace(c) ) continue;   // Ignore any white spaces
		if ( c == '\b' ) {   // If backspace is pressed, cancel and start over
			printf("   X\nType again: ");
			i = 0;
			continue; }
		putchar(c);
		if ( i < 24 ) {
			id[i++] = c;
		} else {   // When the id has been almost entered
			id[i++] = c;
			id[i] = '\0';
			break;
		}
	}
	puts("  OK!");
	return 1;
}

void displaydetails(char *id) {
	// displaydetails: displays all the information in memory about the card with the ID id
	int i, key, month, day_, usage = 0, t_usage;
	double exptotal = 0, t_exp;
	char *end;
	static char *month_name[] = { "Illegal month", "January", "February", "March", "April", "May",
		"June", "July", "August", "September", "October", "November", "December" };
	year *yptr;
	day *dptr;
	comserv *pptr;

	key = hashsearch(id, NULL);
	if ( key == -1 )
		printf("   Failure: %s does not exist.\n", id);
	else {
		printf("\n\n               ID: %s\n", hashtab[key]->id);
		puts("============================================");
		yptr = hashtab[key]->ptr;
		while ( yptr != NULL ) {
			dptr = yptr->ptr;
			t_usage = 0;
			t_exp = 0;
			printf("             Year: %d\n", yptr->year);
			puts("=======================");
			while ( dptr != NULL ) {
				pptr = dptr->ptr;
				month_day(yptr->year, dptr->day, &month, &day_);
				if ( day_ == 1 ) end = "st";
				else if ( day_ == 2 ) end = "nd";
				else if ( day_ == 3 ) end = "rd";
				else end = "th";
				printf("             Date: %s %d%s\n", month_name[month], day_, end);
				printf("       Enterprise: %d\n", dptr->enterprise);
				printf("         Expenses: $%.2f\n", dptr->expenses);
				printf("Products/Services:");
				i = 0;
				while ( pptr != NULL ) {
					switch ( i ) {
						case 0:
							i++;
							break;
						default:
							if ( i++ % 10 == 0 ) printf("\n\t\t  ");
					}
					printf(" %s", pptr->id);
					pptr = pptr->next;
				}
				puts("\n");
				t_usage++;
				t_exp += dptr->expenses;
				dptr = dptr->next;
			}
			printf(">>>> In %d, the expenses total was $%.2f\n", yptr->year, t_exp);
			printf(">>>> and the card was used %d times\n", t_usage);
			puts("");
			usage += t_usage;
			exptotal += t_exp;
			yptr = yptr->next;
		}
		puts("====================================");
		printf("The total money spent are: $%.2f\n", exptotal);
		printf("The card has been used %d times\n", usage);
	}
}

void month_day(int year, int yearday, int *pmonth, int *pday) {
	// month_day: takes the day of the year and returns the month and the day of the month
	int i, leap;
	static char daytab[2][13] = { {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},   // Non-leap years
								  {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31} };   // Leap years

	leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
	for (i = 1; yearday > daytab[leap][i]; i++)
		yearday -= daytab[leap][i];
	*pmonth = i;
	*pday = yearday;
}

void selection6(void) {
	// selection6: takes the choice of the menu about what data the user wants to be displayed
	// and also gives the choice to either enter at the moment the card IDs or insert them from a file
	int select1, select2;
	char filename[30];

	select1 = select6_1();   // Take the first choice
	if ( select1 == '5' ) {
		putchar('\n');
		return; }   // Return to the main menu
	select2 = select6_2(select1);   // Take the second choice
	if ( select2 == '3' ) {
		putchar('\n');
		return; }   // Return to the main menu
	if ( select2 == '1' ) {   // If insertion from the keyboard was selected
		if ( !collectids6() ) {   // Collect the card IDs...
			printf("\n[Press any key to continue] ");
			getch();   // Wait
			putchar('\n');
			return; }   // If no IDs were entered, return to the main menu
		display6("temp_ids.txt", select1);   // ... and display the data about them
	} else {   // If insertion from a file was selected
		puts("[Note: Type the name without its file extension]");
		puts("================================================");
		gatherfilename(filename);   // Take the name of the file...
		display6(filename, select1);   // ... and display the data about the card IDs in the file
	}
}

int select6_1(void){
	// select6_1: displays the primary menu of the selection 6 of the main menu
	int c;
	
	printf("\n\n         [Press 1 through 5 for the choices]\n");
	printf("  6.1. Display the set of products and services purchased\n");
	printf("  6.2. Display the expenses total\n");
	printf("  6.3. Display the complete movement\n");
	printf("  6.4. Display the most desirable product or service\n");
	printf("  6.5. Return to the main menu\n\n");
	printf("Select: ");

	while( 1 ) {
		switch( c = getch() ) {   // Error Checking
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
				break;
			default:
				continue;
		}
		putchar(c);
		return c;
	}
}

int select6_2(int select1) {
	// select6_2: displays the secondary menu of the selection 6 of the main menu
	int c;

	printf("\n\n         [Press 1 or 2 for the choices]\n");
	printf("  6.%c.1. Insert the card IDs from the keyboard\n", select1);
	printf("  6.%c.2. Insert the card IDs from a file\n", select1);
	printf("  6.%c.3. Return to the main menu\n\n", select1);
	printf("Select: ");

	while( 1 ) {
		c = getch();
		if ( c != '1' && c != '2' && c != '3' ) continue;   // Error Checking
		putchar(c);
		if ( c == '2' ) puts("\n");
		return c;
	}
}

int collectids6(void) {
	// collectids6: collects card ids from the input and stores them in a file
	int c, i = 0, count = 0;
	char id[26];
	FILE *fp;
	
	fp = fopen("temp_ids.txt", "w");   // Open a file for writing
	fwrite("Ignore this line\n", sizeof(char), 17, fp);   // Write a line to comply with the pattern
	puts("\n\n[Note: Press Enter when you are done]");
	printf("=====================================");
	printf("\n Type here: ");
	while ( (c = getch()) != '\r' ) {
		if ( c == ' ' || c == '\t' ) continue;   // Ignore any spaces or tabs
		if ( c == '\b' ) {   // If backspace is pressed, cancel and start over
			printf("   X\nType again: ");
			i = 0;
			continue; }
		putchar(c);
		if ( i < 24 ) {   // Note the characters
			id[i++] = c;
		} else {   // When the id has been almost entered
			id[i++] = c;
			id[i] = '\n';
			i = 0;   // Rewind
			count++;   // Increment the number of cards to be deleted
			fwrite(id, sizeof(char), 26, fp);   // Write down the id
			printf("  OK!\n      Next: ");
			continue;
		}
	}
	if ( i != 0 ) printf("  Failed!\n");
	else printf(" -\n");
	printf("\nYou entered %d IDs.\n", count);
	if ( !count ) {
		puts("Aborting...");
		return 0; }
	fclose(fp);
	return count;
}

void display6(char *filename, int select) {
	// display6: takes the choice of the user and displays the corresponding data
	FILE *fp;
	int i, j, key, month, day_, usage = 0, t_usage;
	int pns[2][1000], max, posi, posj, allmax = 0, allposi = 0, allposj = 0;
	double exptotal = 0, t_exp;
	char id[28], *end, *maxid;
	static char *month_name[] = { "Illegal month", "January", "February", "March", "April", "May",
		"June", "July", "August", "September", "October", "November", "December" };
	year *yptr;
	day *dptr;
	comserv *pptr;

	fp = fopen(filename, "r");
	if ( fp == NULL ) {
		printf("\nError: %s was not accepted.\n", filename);
		printf("\n[Press any key to continue] ");
		getch();   // Wait
		putchar('\n');
		return; }
	fgets(id, 28, fp);   // Ignore the first line
	switch ( select ) {   // Entitle the procedure
		case '1':
			puts("\n\nCardholders and their Possessions");
			puts("=================================");
			break;
		case '2':
			puts("\n\nCardholders and their Expenses");
			puts("==============================");
			break;
		case '3':
			puts("\n\nCardholders and their Transactions");
			puts("==================================");
			break;
		case '4':
			puts("\n\nCardholders and their most Desirable Product/Service");
			puts("====================================================");
	}
	fgets(id, 28, fp);   // Take an ID from the file
	id[25] = '\0';   // Replace '\n'
	while ( !feof(fp) ) {   // While you haven't met the end-of-file, act as follows
		key = hashsearch(id, NULL);   // Search for the ID
		if ( key == -1 ) {   // If the id is not in the table
			printf("\n            Error: %s does not exist.\n\n", id);
			fgets(id, 28, fp);   // Take the next ID from the file
			id[25] = '\0';   // Replace '\n'
			continue; }
		switch ( select ) {
			case '1':
				printf("\n               ID: %s\n", hashtab[key]->id);
				puts("============================================");
				printf("Products/Services:");
				yptr = hashtab[key]->ptr;
				i = 0;
				while ( yptr != NULL ) {
					dptr = yptr->ptr;
					while ( dptr != NULL ) {
						pptr = dptr->ptr;
						while ( pptr != NULL ) {
							switch ( i ) {
								case 0:
									i++;
									break;
								default:
									if ( i++ % 10 == 0 ) printf("\n\t\t  ");
							}
							printf(" %s", pptr->id);
							pptr = pptr->next;
						}
						dptr = dptr->next;
					}
					yptr = yptr->next;
				}
				puts("\n");
				break;
			case '2':
				printf("\n               ID: %s\n", hashtab[key]->id);
				puts("============================================");
				yptr = hashtab[key]->ptr;
				t_usage = 0;
				t_exp = 0;
				while ( yptr != NULL ) {
					dptr = yptr->ptr;
					while ( dptr != NULL ) {
						t_usage++;
						t_exp += dptr->expenses;
						dptr = dptr->next;
					}
					yptr = yptr->next;
				}
				exptotal += t_exp;
				printf("   Expenses Total: $%.2f\n", t_exp);
				printf("       Times Used: %d\n\n", t_usage);
				break;
			case '3':
				printf("\n               ID: %s\n", hashtab[key]->id);
				puts("============================================");
				yptr = hashtab[key]->ptr;
				usage = 0;
				exptotal = 0;
				while ( yptr != NULL ) {
					dptr = yptr->ptr;
					t_usage = 0;
					t_exp = 0;
					printf("             Year: %d\n", yptr->year);
					puts("=======================");
					while ( dptr != NULL ) {
						pptr = dptr->ptr;
						month_day(yptr->year, dptr->day, &month, &day_);
						if ( day_ == 1 ) end = "st";
						else if ( day_ == 2 ) end = "nd";
						else if ( day_ == 3 ) end = "rd";
						else end = "th";
						printf("             Date: %s %d%s\n", month_name[month], day_, end);
						printf("       Enterprise: %d\n", dptr->enterprise);
						printf("         Expenses: $%.2f\n", dptr->expenses);
						printf("Products/Services:");
						i = 0;
						while ( pptr != NULL ) {
							switch ( i ) {
								case 0:
									i++;
									break;
								default:
									if ( i++ % 10 == 0 ) printf("\n\t\t  ");
							}
							printf(" %s", pptr->id);
							pptr = pptr->next;
						}
						puts("\n");
						t_usage++;
						t_exp += dptr->expenses;
						dptr = dptr->next;
					}
					printf(">>>> In %d, the expenses total was $%.2f\n", yptr->year, t_exp);
					printf(">>>> and the card was used %d times\n", t_usage);
					puts("");
					usage += t_usage;
					exptotal += t_exp;
					yptr = yptr->next;
				}
				puts("====================================");
				printf("The total money spent are: $%.2f\n", exptotal);
				printf("The card has been used %d times.\n\n\n", usage);
				break;
			case '4':
				printf("\n               ID: %s\n", hashtab[key]->id);
				puts("============================================");
				for ( i = 0; i < 2; i++ ) for ( j = 0; j < 1000; j++ ) pns[i][j] = 0;
				yptr = hashtab[key]->ptr;
				while ( yptr != NULL ) {
					dptr = yptr->ptr;
					while ( dptr != NULL ) {
						pptr = dptr->ptr;
						while ( pptr != NULL ) {
							switch ( pptr->id[0] ) {
								case 'P':
									pns[0][atoi(pptr->id + 1)]++;
									break;
								case 'S':
									pns[1][atoi(pptr->id + 1)]++;
							}
							pptr = pptr->next;
						}
						dptr = dptr->next;
					}
					yptr = yptr->next;
				}
				max = pns[0][0];
				// In case several products or services have the same number of purchases, then the p. or s. that
				// is going to be considered as the most desirable is the one with the smallest consecutive number
				for ( i = 0; i < 2; i++ )
					for ( j = 0; j < 1000; j++ )
						if ( pns[i][j] > max ) {
							max = pns[i][j];
							posi = i;
							posj = j;
						}
				printf("Product/ServiceID: %c%s%d\n",posi?'S':'P', posj < 100 ? (posj < 10 ? "00": "0"): "",posj);
				printf("        Purchased: %d times\n\n", max);
				if ( max > allmax ) {
					allmax = max;
					allposi = posi;
					allposj = posj;
					maxid = hashtab[key]->id;
				}
		}
		fgets(id, 28, fp);   // Take the next ID from the file
		id[25] = '\0';   // Replace '\n'
	}
	fclose(fp);
	switch ( select ) {
		case '2':
			puts("\n===================================================");
			printf("All the cardholders together have spent: $%.2f\n\n", exptotal);
			break;
		case '4':
			puts("\n_____________________________________________________________");
			puts("=============================================================");
			puts("The most desirable product/service among all the cardholders:");
			puts("============================================");
			printf("               ID: %s\n", maxid);
			printf("Product/ServiceID: %c%s%d\n",allposi?'S':'P',(allposj<100)?(allposj<10)?"00":"0":"",allposj);
			printf("        Purchased: %d times\n\n", allmax);
	}
	printf("\n[Press any key to continue] ");
	getch();   // Wait
	putchar('\n');
}

void selection7(void) {
	// selection7: gets the choice of the user and displays the data accordingly
	int select, year = 0;
	char syear[5];

	select = select7();   // Get the choice of the user
	if ( select == '4' ) return;   // Return to the main menu
	if ( select == '3' ) {
		puts("\n======================");
		printf("Specify the year: ");
		scanf("%s", syear);   // Get the desired year to be displayed
		year = atoi(syear);
		puts("======================");
	}
	display7(select, year);   // Display the data
}

int select7(void) {
	// select7: takes the choice of the menu from the user
	int c;

	printf("\n\n         [Press 1 through 4 for the choices]\n");
	printf("  7.1. Display those with the most products/services purchased\n");
	printf("  7.2. Display those with the biggest expenses in general\n");
	printf("  7.3. Display those with the biggest expenses in a year\n");
	printf("  7.4. Return to the main menu\n\n");
	printf("Select: ");
	while(1) {
		switch( c = getch() ) {   // Error Checking
			case '1':
			case '2':
			case '3':
			case '4':
				putchar(c);
				putchar('\n');
				break;
			default:
				continue;
		}
		return c;
	}
}

void display7(int select, int year_) {
	// display7: displays data of the best buyers
	int num, i, j;
	double expenses;
	char snum[10];
	year *yptr;
	day *dptr;
	comserv *pptr;

	puts("\n\nInsert the number of cards");
	puts("==========================");
	printf("        you would like to be displayed (from 1 to %d)\n", numentrfile);
	puts("=========================================================");
	printf(" Type here [Press Enter when you are done]: ");
	scanf("%d", &num);   // Take the number of the cards to be displayed
	while ( num < 1 || num > numentrfile ) {   // Range checking
		printf(" Out of range. Please try again: ");
		scanf("%s", snum);
		num = atoi(snum);
	}

	switch ( select ) {   // Entitle the procedure
		case '1':
			puts("\n\nBest Buyers regarding their Possessions");
			printf("=======================================");
			break;
		case '2':
			puts("\n\nBest Buyers regarding their Expenses");
			printf("====================================");
			break;
		case '3':
			printf("\n\nBest Buyers regarding their Expenses in Year %d\n", year_);
			printf("=================================================");
			break;
	}
	switch ( select ) {
		case '1':
			heapSort(htsorted, numentrfile, cmpproducts);   // Sort the IDs according to their products/services
			for ( i = numentrfile - 1; i > numentrfile - 1 - num; i-- ) {
				printf("\n\n               ID: %s\n", htsorted[i]->id);
				puts("============================================");
				printf("            Count: %d\n",countproducts(htsorted[i]));
				printf("Products/Services:");
				yptr = htsorted[i]->ptr;
				j = 0;
				while ( yptr != NULL ) {
					dptr = yptr->ptr;
					while ( dptr != NULL ) {
						pptr = dptr->ptr;
						while ( pptr != NULL ) {
							switch ( j ) {
								case 0:
									j++;
									break;
								default:
									if ( j++ % 10 == 0 ) printf("\n\t\t  ");
							}
							printf(" %s", pptr->id);
							pptr = pptr->next;
						}
						dptr = dptr->next;
					}
					yptr = yptr->next;
				}
			}
			puts("\n");
			break;
		case '2':
			heapSort(htsorted, numentrfile, cmpexpenses);   // Sort the IDs according to their expenses
			for ( i = numentrfile - 1; i > numentrfile - 1 - num; i-- ) {
				printf("\n\n               ID: %s\n", htsorted[i]->id);
				puts("============================================");
				yptr = htsorted[i]->ptr;
				expenses = 0;
				while ( yptr != NULL ) {
					dptr = yptr->ptr;
					while ( dptr != NULL ) {
						expenses += dptr->expenses;
						dptr = dptr->next;
					}
					yptr = yptr->next;
				}
				printf("   Expenses Total: $%.2f", expenses);
			}
			puts("\n");
			break;
		case '3':
			heapSortinayear(htsorted, numentrfile, year_);   // Sort the IDs according to their expenses in the year specified
			for ( i = numentrfile - 1; i > numentrfile - 1 - num; i-- ) {
								printf("\n\n               ID: %s\n", htsorted[i]->id);
				puts("============================================");
				yptr = htsorted[i]->ptr;
				expenses = 0;
				while ( yptr != NULL ) {
					if ( yptr->year != year_ ) {
						yptr = yptr->next;
						continue; }
					dptr = yptr->ptr;
					while ( dptr != NULL ) {
						expenses += dptr->expenses;
						dptr = dptr->next;
					}
					break;
				}
				printf("   Expenses Total: $%.2f", expenses);
			}
			puts("\n");
	}
	printf("\n[Press any key to continue] ");
	getch();   // Wait
	putchar('\n');
	return;
}

void setsettings(void) {
	// setsettings: displays the current computation technique for the probe sequences
	// and gives the choice to change it
	int c;

	switch ( settings ) {
	case 0:
		puts("\n\n\nThe current technique for computing the Probe Sequences is \"Linear Probing\"");
		break;
	case 1:
		puts("\n\n\nThe current technique for computing the Probe Sequences is \"Double Hashing\"");
	}
	
	puts("===========================================================================");
	printf("[Press Enter to change the setting, or Space to return to the main menu] ");
	while ( 1 ) {
		c= getch();
		if ( c == '\r' ) break;
		if ( c == ' ' ) return;
	}
	putchar('\n');

	puts("\n\nChoose your preferable technique for computing the Probe Sequences");
	puts("==================================================================");
	puts("[Note: Changes are not going to affect the current table]");
	puts("=========================================================");
	puts("[Press 1 or 2 for the choices]");
	puts("==============================");
	puts("1. Linear Probing");
	puts("2. Double Hashing");
	printf("\nSelect: ");
	c = getch();
	while ( c != '1' && c != '2' ) c = getch();
	printf("%c\n", c);
	switch ( c ) {
	case '1':
		temp_set = 0;
		break;
	case '2':
		temp_set = 1;
	}
}
