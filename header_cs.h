/************************************************************************
Header File	: header_cs.h
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BF.h"

typedef struct {
     int fileDesc;  // anagnoristikos aritmos anigmatos arxeiou apo to epipedo block
     char attrType; // o tipos tu pediou pou einai to klidi gia to sigekrimeno arxeio, 'c' h 'i'
     char *attrName;    // to onoma tu pediu pu einai to klidi gia to sigekrimeno arxeio
     int attrLength;    // to megethos tou pediu pu einai to klidi gia to sigekrimeno arxeio
     int depth; // to oliko vathos tou evretiriou
} CSHT_info;

typedef struct{
   int id;
   char name[15];
   char surname[20];
   char status[2]; // allagh apo 1 se 2 tou plh8ous ton 8eseon oste na diabazetai os stirng kai na yparxei xoros gia ton "\0" xarakthra
   char dateOfBirth[11];
   int salary;
   char section[2]; // allagh apo 1 se 2 tou plh8ous ton 8eseon oste na diabazetai os stirng kai na yparxei xoros gia ton "\0" xarakthra
   int daysOff;
   int prevYears;
} Record;

//////////////////////// RECORDS /////////////////////////////////////////////////////////
typedef struct	{
	int rowid;
        int id;
} idRec;

typedef struct	{
	int rowid;
	char name[15];
} nameRec;

typedef struct	{
	int rowid;
	char surname[20];
} surnameRec;

typedef struct	{
	int rowid;
	char status[2];
} statusRec;

typedef struct	{
	int rowid;
	char dateOfBirth[11];
} dateOfBirthRec;

typedef struct	{
	int rowid;
	int salary;
} salaryRec;

typedef struct	{
	int rowid;
	char section[2];
} sectionRec;

typedef struct	{
	int rowid;
	int daysOff;
} daysOffRec;

typedef struct	{
	int rowid;
	int prevYears;
} prevYearsRec;
//////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
   int filetype; //0 = cs_heap , 1 = cs_hash

} info_block;       // domh anagnorishs tou typou tou arxeiou

typedef struct {
   char id[25];
   char name[25];
   char surname[25];
   char status[25];
   char dateOfBirth[25];
   char salary[25];
   char section[25];
   char daysOff[25];
   char prevYears[25];
} files;

//******************************************************************************

/* Column-Store Heap File Functions */
int CS_CreateFiles( char **, int);
int CS_OpenFile( char *);
int CS_CloseFile( int);
int CS_InsertEntry( int, Record);
void CS_GetAllEntries( int, char*, void*, char**, int);

/* Column-Store Hash Table Functions */
int CSHT_CreateIndex( char *, char, char *, int, int);
CSHT_info* CSHT_OpenIndex( char *);
int CSHT_CloseIndex( CSHT_info *);
int CSHT_InsertEntries( char *, CSHT_info*);
void CSHT_GetAllEntries( CSHT_info*, void *, char **, int);

