#ifndef LOGGING_SYSTEM_H
#define LOGGING_SYSTEM_H

void intro(void);
void initload(void);
void initset(void);
void gatherfilename(char *filename);
void menu(void);
void selection1(void);
void insertprocedure(char *s);
void selection2(void);
int select2(void);
void deleteprocedure(int key);
void selection3(void);
void selection4(void);
void selection5(void);
int collectid(char *id);
void displaydetails(char *id);
void month_day(int year, int yearday, int *pmonth, int *pday);
void selection6(void);
int select6_1(void);
int select6_2(int select);
int collectids6(void);
void display6(char *filename, int select1);
void selection7(void);
int select7(void);
void display7(int select, int syear);
void setsettings(void);

#endif  // LOGGING_SYSTEM_H
