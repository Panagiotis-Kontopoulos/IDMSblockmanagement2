/*******************************************************************************
File	: main.c
********************************************************************************/

#include "header_cs.h"

#define filetype   0   /* 0 = cs_heap kai 1 = hash */
#define FILENAME_CS      "CSFile_Header_Info"
#define FILENAME_CSHT   "CSHTFile_Header_Info" // H CSHT den ylopoiithike

int main(int argc, char** argv) 
{
   int CSFile, depth, attrLength, open = 0, insert = 0, i, j;
   CSHT_info *CSHTFile;
   Record record;
   FILE *records;
   char temp_s[20], attrType, attrName[20], *fieldNames[9] = {"CSFile_id","CSFile_name","CSFile_surname","CSFile_status","CSFile_dateOffBirth","CSFile_salary","CSFile_section","CSFile_daysOff","CSFile_prevYears"};
/*************************************************************************************************************************/
   attrType = 'c';
   strcpy(attrName,"name");
   attrLength = sizeof(char);
   depth = 3;  // megisto ba8os einai 7
   /* Create heap file */
   if (CS_CreateFiles(fieldNames, 9) < 0)
     {  fprintf(stderr, "Error creating heap file.\n");
        exit(EXIT_FAILURE);
     }
   /* Open heap file */
   if ((CSFile = CS_OpenFile(FILENAME_CS)) < 0) 
     {  fprintf(stderr, "Error opening heap file.\n");
        exit(EXIT_FAILURE);
     }
   if (filetype)
     {  /* Create hash index based on attrName */
        if ( CSHT_CreateIndex(FILENAME_CSHT, attrType, attrName, attrLength, depth) < 0 )
          {  fprintf(stderr, "Error creating hash index.\n");
 	     exit(EXIT_FAILURE);
          }
        /* Open hash index based on attrName */
        if ((CSHTFile = CSHT_OpenIndex(FILENAME_CSHT)) == NULL)
          {  fprintf(stderr, "Error opening hash index.\n");
             CSHT_CloseIndex(CSHTFile);
             exit(EXIT_FAILURE);
          }
     }

/*************************************************************************************************************************/
   if ((records = fopen("./records", "r")) == NULL) /* Open records File */
      {  perror("fopen Records-file");
         return 1;
      }
   while (!feof(records))
      {  fscanf(records, "%d\t", &i);
         record.id = i;
         fscanf(records, "%s\t", temp_s);
         strcpy(record.name, temp_s);
         fscanf(records, "%s\t", temp_s);
         strcpy(record.surname, temp_s);
         fscanf(records, "%s\t", temp_s);
         strcpy(record.status, temp_s);
         fscanf(records, "%s\t", temp_s);
         strcpy(record.dateOfBirth, temp_s);
         fscanf(records, "%d\t", &i);
         record.salary = i;
         fscanf(records, "%s\t", temp_s);
         strcpy(record.section, temp_s);
         fscanf(records, "%d\t", &i);
         record.daysOff = i;
         fscanf(records, "%d\t", &i);
         record.prevYears = i;
         /* Insert record in heap file */
         if (CS_InsertEntry(CSFile, record) < 0) 
           {  fprintf(stderr, "Error inserting entry in heap file.\n");
              CS_CloseFile(CSFile);
              exit(EXIT_FAILURE);
           }
      }
    if (filetype)
      {
        /* Insert record in hash index based on id */
        if (CSHT_InsertEntry(FILENAME_CSHT, CSHTFile) < 0) 
          {  fprintf(stderr, "Error inserting entry in hash index\n");
             CSHT_CloseIndex(CSHTFile);
             exit(EXIT_FAILURE);
           }
      }

/*************************************************************************************************************************/
   int value, n;
   value = 195;
   n = 9;
   if (filetype)
     {
        /* Get records from hash index */
        CSHT_GetAllEntries(CSHTFile, NULL, (char *[]){"id","name","surname","section"}, n);
        //CSHT_GetAllEntries(CSHTFile, &value, (char *[]){"id","name","surname","section"}, n);
        //CSHT_GetAllEntries(CSHTFile, "Georgios", (char *[]){"id","name","surname","section"}, n);
     }
   else
     {
        /* Get records from heap file */
        CS_GetAllEntries(CSFile, NULL, NULL, (char *[]){"id","name","surname","status","dateOfBirth","salary","section","daysOff","prevYears"}, n);
        //CS_GetAllEntries(CSFile, "id", &value, (char *[]){"id","name","surname","status","dateOfBirth","salary","section","daysOff","prevYears"}, n);
        //CS_GetAllEntries(CSFile, "dateOfBirth", "21-08-1977", (char *[]){"id","name","surname","status","dateOfBirth","salary","section","daysOff","prevYears"}, n);
     }
/*************************************************************************************************************************/
   /* Close Column-Store files */
   if (CS_CloseFile(CSFile) < 0) 
     {  fprintf(stderr, "Error closing heap file.\n");
        exit(EXIT_FAILURE);
     }
   if (filetype)
     {
        /* Close id hash index */
        if (CSHT_CloseIndex(CSHTFile) < 0) 
          {  fprintf(stderr, "Error closing id hash index.\n");
             exit(EXIT_FAILURE);
          }
     }
/*************************************************************************************************************************/

}
