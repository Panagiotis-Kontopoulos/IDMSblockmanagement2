/************************************************************************
Header File	: CS_functions.c
*************************************************************************/

#include "header_cs.h"

int CS_CreateFiles(char** fieldname, int n)
{	
	void *block;   // h domh me ta dedomena anagnoriseis arxeiou sto proto block tou arxeiou
	int filedesc , i=0;
    	info_block i_block;
	files f;
	char **pinakas;
// n: strings pu einai apothikevmena ston pinaka "pinakas"
	int cNumber = 20;	// plithos xarakthron pu tha xoraei kathe string tou pinaka "pinakas"
	int counter = 0;

	//f = malloc(sizeof(files));	// desmefsh mnhmhs gia th voithitikh struct "files"

	pinakas = malloc(sizeof(char*)*(n+1));		// desmefsh voithitikou pinaka
	for (i=0; i < n+1; i++)
	{	pinakas[i] = malloc(sizeof(char)*cNumber);
	}


	
// ta diakritika ton arxeion - pedion
	strcpy(f.id, fieldname[0]);
	strcpy(f.name, fieldname[1]);
	strcpy(f.surname, fieldname[2]);
	strcpy(f.status, fieldname[3]);
	strcpy(f.dateOfBirth, fieldname[4]);
	strcpy(f.salary, fieldname[5]);
 	strcpy(f.section, fieldname[6]);
	strcpy(f.daysOff, fieldname[7]);
	strcpy(f.prevYears, fieldname[8]);

	strcpy(pinakas[0], "CSFile_Header_Info");	// onoma genikou arxeiou
// onomata arxeion - pedion
	strcpy(pinakas[1], fieldname[0]);
	strcpy(pinakas[2], fieldname[1]);
	strcpy(pinakas[3], fieldname[2]);
	strcpy(pinakas[4], fieldname[3]);
	strcpy(pinakas[5], fieldname[4]);
	strcpy(pinakas[6], fieldname[5]);
 	strcpy(pinakas[7], fieldname[6]);
	strcpy(pinakas[8], fieldname[7]);
	strcpy(pinakas[9], fieldname[8]);
	
	BF_Init();
	
	for (i=0; i < n+1; i++)		// dimiourgia tou genikou arxeiou kai ton arxeion, ena gia kathe pedio
	{	if (BF_CreateFile(pinakas[i]) < 0)
		{	BF_PrintError("Error creating file");
			return -1;
		}
		if((filedesc = BF_OpenFile(pinakas[i])) < 0)
		{	BF_PrintError("Error openning file");
			return -1;
		}
	    
	    if (BF_AllocateBlock(filedesc) < 0)    // desmefsh tou protou block
		{	 BF_PrintError("Error Allocating Block");
			    return -1;
		}
		if (BF_ReadBlock(filedesc, 0, &block) < 0) 
		{	BF_PrintError("Error reading block");
		    return -1;
		}
		if (i == 0)	// arxikopoihsh tou geniku arxeiou - arxikopoihsh tou protou block aftou
		{	i_block.filetype = 0;
	    	memcpy(block, &i_block, sizeof(info_block)); // katagrafh ton dedomenon anagnorishs arxeio, sto 1o block
			memcpy(block + sizeof(info_block), &f, sizeof(files));  // katagrafh ths struct me ta onomata ton arxeion ton pedion

			if (BF_WriteBlock(filedesc, 0) < 0)    // egrafh tu block sto disko
			{	BF_PrintError("Error writing block");
				return -1;
			}
			if(BF_CloseFile(filedesc) < 0)	// kleisimo tou arxeiou
			{	BF_PrintError("Error closing file");
				return -1;
			}
		}
		else	// i > 0	arxikopoihsh ton arxeion - pedion
		{	memcpy(block, &i_block, sizeof(info_block));	// pliroforia anagnorishs (heap h hash file)
			memcpy(block + sizeof(info_block), pinakas[i], sizeof(pinakas[i]));	// egrafh tou diakritikou tou arxeiou
			if (BF_WriteBlock(filedesc, 0) < 0)    // egrafh tu block sto disko
			{	BF_PrintError("Error writing block");
				return -1;
			}	
	
			if(BF_AllocateBlock(filedesc) < 0)	// desmefsh tou defteru block kathe arxeiou - pediou
			{	BF_PrintError("Error allocating block");
				return -1;
			}
		 	if (BF_ReadBlock(filedesc, 1, &block) < 0) 
			{	BF_PrintError("Error reading block");
			    return -1;
			}
			memcpy(block, &counter, sizeof(int));	// arxikopoihsh tou metrith egrafon se miden
			if (BF_WriteBlock(filedesc, 1) < 0)    // egrafh tu block sto disko
			{	BF_PrintError("Error writing block");
				return -1;
			}
			if(BF_CloseFile(filedesc) < 0)	// kleisimo tou arxeiou
			{	BF_PrintError("Error closing file");
				return -1;
			}
		}
	}
if((filedesc = BF_OpenFile(pinakas[0])) < 0)
{	BF_PrintError("Error openning file");
	return -1;
}

if (BF_ReadBlock(filedesc, 0, &block) < 0) 
{	BF_PrintError("Error reading block");
    return -1;
}
memcpy(&i_block, block, sizeof(info_block));
memcpy(&f, block+sizeof(info_block), sizeof(files));

if(BF_CloseFile(filedesc) < 0)
{	BF_PrintError("Error closing file");
	return -1;
}

//******************************* apodesmefsh ths mnhmhs ***************************		
for (i=0; i < n+1; i++)	
	free(pinakas[i]);
free(pinakas);
//**********************************************************************************
	return 0;
}

/*########################################################################################################################*/

char* PutFileDigit( char *destination, char *source, int i)
{
   char digit[3];
   strcpy(destination,source);
   sprintf(digit,"%d",i);
   strcat(destination," ");
   strcat(destination,digit);
   return destination;
}
/* topo8etei to pshfio toy arxeioy opos epistreafhke apo thn BF_OpenFile sto telos toy destination kai epistrefei to destination */

int CS_OpenFile( char *header_info)
{
   int fileDesc, filetype, i, col_fileDesc, error = 0;
   void *block;
   files buffer_f;
   char *buffer_c, digit, *fileName;
   buffer_c = malloc(25*sizeof(char));
   fileName = malloc(20*sizeof(char));

   fileDesc = BF_OpenFile(header_info);
   if (fileDesc < 0)
     {  BF_PrintError("Error opening file");
        return fileDesc;
     }
   else
     {  if (BF_ReadBlock(fileDesc,0,&block) != 0)
          {  BF_PrintError("Error reading block");
             return -1;
          }
        else
          {  filetype = ((info_block*)block)->filetype;
             if (filetype != 0)
               {  printf("Not a Column-Store Header_Info File. \n");
                  BF_CloseFile(fileDesc);
                  return -1;
               }
          }
        i = 0;
        while (i<9 && error == 0)
           {  memcpy(&buffer_f,block+sizeof(info_block),sizeof(files));
              if (i == 0) /* Anoigei to arxeio gia to id kai to fileDesc toy enhmeronetai sto header_info */
                {  strcpy(fileName,buffer_f.id);
                   col_fileDesc = BF_OpenFile(fileName);
                   if (col_fileDesc < 0)
                     {  BF_PrintError("Error opening file");
                        error = -1;
                        return error;
                     }
                   if (BF_ReadBlock(col_fileDesc,0,&block) != 0)
                     {  BF_PrintError("Error getting block");
                        error = -1;
                        return error;
                     }
                   filetype = ((info_block*)block)->filetype;
                   if (filetype != 0)
                     {  printf("Not a Column-Store File. \n");
                        BF_CloseFile(fileDesc);
                        error = -1;
                        return error;
                     }
                   else
                     {  if (BF_ReadBlock(fileDesc,0,&block) != 0)
                          {  BF_PrintError("Error getting block");
                             error = -1;
                             return error;
                          }
                        strcpy(buffer_f.id,PutFileDigit(buffer_c,buffer_f.id,col_fileDesc));
                     }
                }
              if (i == 1) /* Anoigei to arxeio gia to name kai to fileDesc toy enhmeronetai sto header_info */
                {  strcpy(fileName,buffer_f.name);
                   col_fileDesc = BF_OpenFile(fileName);
                   if (col_fileDesc < 0)
                     {  BF_PrintError("Error opening file");
                        error = -1;
                        return error;
                     }
                   if (BF_ReadBlock(col_fileDesc,0,&block) != 0)
                     {  BF_PrintError("Error getting block");
                        error = -1;
                        return error;
                     }
                   filetype = ((info_block*)block)->filetype;
                   if (filetype != 0)
                     {  printf("Not a Column-Store File. \n");
                        BF_CloseFile(fileDesc);
                        error = -1;
                        return error;
                     }
                   else
                     {  if (BF_ReadBlock(fileDesc,0,&block) != 0)
                          {  BF_PrintError("Error getting block");
                             error = -1;
                             return error;
                          }
                        strcpy(buffer_f.name,PutFileDigit(buffer_c,buffer_f.name,col_fileDesc));
                     }
                }
              if (i == 2) /* Anoigei to arxeio gia to surname kai to fileDesc toy enhmeronetai sto header_info */
                {  strcpy(fileName,buffer_f.surname);
                   col_fileDesc = BF_OpenFile(fileName);
                   if (col_fileDesc < 0)
                     {  BF_PrintError("Error opening file");
                        error = -1;
                        return error;
                     }
                   if (BF_ReadBlock(col_fileDesc,0,&block) != 0)
                     {  BF_PrintError("Error getting block");
                        error = -1;
                        return error;
                     }
                   filetype = ((info_block*)block)->filetype;
                   if (filetype != 0)
                     {  printf("Not a Column-Store File. \n");
                        BF_CloseFile(fileDesc);
                        error = -1;
                        return error;
                     }
                   else
                     {  if (BF_ReadBlock(fileDesc,0,&block) != 0)
                          {  BF_PrintError("Error getting block");
                             error = -1;
                             return error;
                          }
                        strcpy(buffer_f.surname,PutFileDigit(buffer_c,buffer_f.surname,col_fileDesc));
                     }
                }
              if (i == 3) /* Anoigei to arxeio gia to status kai to fileDesc toy enhmeronetai sto header_info */
                {  strcpy(fileName,buffer_f.status);
                   col_fileDesc = BF_OpenFile(fileName);
                   if (col_fileDesc < 0)
                     {  BF_PrintError("Error opening file");
                        error = -1;
                        return error;
                     }
                   if (BF_ReadBlock(col_fileDesc,0,&block) != 0)
                     {  BF_PrintError("Error getting block");
                        error = -1;
                        return error;
                     }
                   filetype = ((info_block*)block)->filetype;
                   if (filetype != 0)
                     {  printf("Not a Column-Store File. \n");
                        BF_CloseFile(fileDesc);
                        error = -1;
                        return error;
                     }
                   else
                     {  if (BF_ReadBlock(fileDesc,0,&block) != 0)
                          {  BF_PrintError("Error getting block");
                             error = -1;
                             return error;
                          }
                        strcpy(buffer_f.status,PutFileDigit(buffer_c,buffer_f.status,col_fileDesc));
                     }
                }
              if (i == 4) /* Anoigei to arxeio gia to dateOffBirth kai to fileDesc toy enhmeronetai sto header_info */
                {  strcpy(fileName,buffer_f.dateOfBirth);
                   col_fileDesc = BF_OpenFile(fileName);
                   if (col_fileDesc < 0)
                     {  BF_PrintError("Error opening file");
                        error = -1;
                        return error;
                     }
                   if (BF_ReadBlock(col_fileDesc,0,&block) != 0)
                     {  BF_PrintError("Error getting block");
                        error = -1;
                        return error;
                     }
                   filetype = ((info_block*)block)->filetype;
                   if (filetype != 0)
                     {  printf("Not a Column-Store File. \n");
                        BF_CloseFile(fileDesc);
                        error = -1;
                        return error;
                     }
                   else
                     {  if (BF_ReadBlock(fileDesc,0,&block) != 0)
                          {  BF_PrintError("Error getting block");
                             error = -1;
                             return error;
                          }
                        strcpy(buffer_f.dateOfBirth,PutFileDigit(buffer_c,buffer_f.dateOfBirth,col_fileDesc));
                     }
                }
              if (i == 5) /* Anoigei to arxeio gia to salary kai to fileDesc toy enhmeronetai sto header_info */
                {  strcpy(fileName,buffer_f.salary);
                   col_fileDesc = BF_OpenFile(fileName);
                   if (col_fileDesc < 0)
                     {  BF_PrintError("Error opening file");
                        error = -1;
                        return error;
                     }
                   if (BF_ReadBlock(col_fileDesc,0,&block) != 0)
                     {  BF_PrintError("Error getting block");
                        error = -1;
                        return error;
                     }
                   filetype = ((info_block*)block)->filetype;
                   if (filetype != 0)
                     {  printf("Not a Column-Store File. \n");
                        BF_CloseFile(fileDesc);
                        error = -1;
                        return error;
                     }
                   else
                     {  if (BF_ReadBlock(fileDesc,0,&block) != 0)
                          {  BF_PrintError("Error getting block");
                             error = -1;
                             return error;
                          }
                        strcpy(buffer_f.salary,PutFileDigit(buffer_c,buffer_f.salary,col_fileDesc));
                     }
                }
              if (i == 6) /* Anoigei to arxeio gia to section kai to fileDesc toy enhmeronetai sto header_info */
                {  strcpy(fileName,buffer_f.section);
                   col_fileDesc = BF_OpenFile(fileName);
                   if (col_fileDesc < 0)
                     {  BF_PrintError("Error opening file");
                        error = -1;
                        return error;
                     }
                   if (BF_ReadBlock(col_fileDesc,0,&block) != 0)
                     {  BF_PrintError("Error getting block");
                        error = -1;
                        return error;
                     }
                   filetype = ((info_block*)block)->filetype;
                   if (filetype != 0)
                     {  printf("Not a Column-Store File. \n");
                        BF_CloseFile(fileDesc);
                        error = -1;
                        return error;
                     }
                   else
                     {  if (BF_ReadBlock(fileDesc,0,&block) != 0)
                          {  BF_PrintError("Error getting block");
                             error = -1;
                             return error;
                          }
                        strcpy(buffer_f.section,PutFileDigit(buffer_c,buffer_f.section,col_fileDesc));
                     }
                }
              if (i == 7) /* Anoigei to arxeio gia to daysOff kai to fileDesc toy enhmeronetai sto header_info */
                {  strcpy(fileName,buffer_f.daysOff);
                   col_fileDesc = BF_OpenFile(fileName);
                   if (col_fileDesc < 0)
                     {  BF_PrintError("Error opening file");
                        error = -1;
                        return error;
                     }
                   if (BF_ReadBlock(col_fileDesc,0,&block) != 0)
                     {  BF_PrintError("Error getting block");
                        error = -1;
                        return error;
                     }
                   filetype = ((info_block*)block)->filetype;
                   if (filetype != 0)
                     {  printf("Not a Column-Store File. \n");
                        BF_CloseFile(fileDesc);
                        error = -1;
                        return error;
                     }
                   else
                     {  if (BF_ReadBlock(fileDesc,0,&block) != 0)
                          {  BF_PrintError("Error getting block");
                             error = -1;
                             return error;
                          }
                        strcpy(buffer_f.daysOff,PutFileDigit(buffer_c,buffer_f.daysOff,col_fileDesc));
                     }
                }
              if (i == 8) /* Anoigei to arxeio gia to prevYears kai to fileDesc toy enhmeronetai sto header_info */
                {  strcpy(fileName,buffer_f.prevYears);
                   col_fileDesc = BF_OpenFile(fileName);
                   if (col_fileDesc < 0)
                     {  BF_PrintError("Error opening file");
                        error = -1;
                        return error;
                     }
                   if (BF_ReadBlock(col_fileDesc,0,&block) != 0)
                     {  BF_PrintError("Error getting block");
                        error = -1;
                        return error;
                     }
                   filetype = ((info_block*)block)->filetype;
                   if (filetype != 0)
                     {  printf("Not a Column-Store File. \n");
                        BF_CloseFile(fileDesc);
                        error = -1;
                        return error;
                     }
                   else
                     {  if (BF_ReadBlock(fileDesc,0,&block) != 0)
                          {  BF_PrintError("Error getting block");
                             error = -1;
                             return error;
                          }
                        strcpy(buffer_f.prevYears,PutFileDigit(buffer_c,buffer_f.prevYears,col_fileDesc));
                     }
                }
              memcpy(block+sizeof(info_block),&buffer_f,sizeof(files));
              if (BF_WriteBlock(fileDesc, 0) < 0)
                {  BF_PrintError("Error writing block");
                        error = -1;
                        return error;
                }
              i++;
/* Gia ka8e ena neo arxeio column poy anoigei, to fileDesc toy pernaei sto antistoixo pedio ths files sto arxeio header_info */
           }
     }
   free(fileName);
   free(buffer_c);
return fileDesc;
}

/*########################################################################################################################*/

int CS_CloseFile(int fileDesc)
{	void* block;
	files *f;  // voithitikh struct "files"
	char **pinakas;
	int sNumber = 10;	// strings pu einai apothikevmena ston pinaka "pinakas"
	int cNumber = 20;	// plithos xarakthron pu tha xoraei kathe string tou pinaka "pinakas"
	//int counter = 0;
	int i;

	f = malloc(sizeof(files));	// desmevsh xoru gia th voithitikh struct "files"

	if(BF_ReadBlock(fileDesc, 0, &block) < 0)
	{	BF_PrintError("Error reading block");
		return -1;
	}

	memcpy(f, block + sizeof(info_block), sizeof(files)); 	// diavasma ths struct pu periexei ta diakritika ton arxeion - pedion

	pinakas = malloc(sizeof(char*)*(sNumber-1));		// desmefsh voithitikou pinaka
	for (i=0; i < sNumber - 1; i++)
	{	pinakas[i] = malloc(sizeof(char)*cNumber);
	}

// ta file descriptions ton arxeion - pedion
	strtok(f->id, " ");
	strcpy(pinakas[0], strtok(NULL, ""));

	strtok(f->name, " ");
	strcpy(pinakas[1], strtok(NULL, ""));

	strtok(f->surname, " ");
	strcpy(pinakas[2], strtok(NULL, ""));

	strtok(f->status, " ");
	strcpy(pinakas[3], strtok(NULL, ""));

	strtok(f->dateOfBirth, " ");
	strcpy(pinakas[4], strtok(NULL, "")); 

	strtok(f->salary, " ");
 	strcpy(pinakas[5], strtok(NULL, "")); 

	strtok(f->section, " ");
	strcpy(pinakas[6], strtok(NULL, "")); 

	strtok(f->daysOff, " ");
	strcpy(pinakas[7], strtok(NULL, "")); 

	strtok(f->prevYears, " ");
	strcpy(pinakas[8], strtok(NULL, ""));

	for (i=0; i < sNumber-1; i++)
	{	if (BF_CloseFile(atoi(pinakas[i])) < 0)
		{   BF_PrintError("Error closing file");
	    	return -1;
		}
	}
	if (BF_CloseFile(fileDesc) < 0)	// klisimo tou genikou arxeiou
	{   BF_PrintError("Error closing file");
  	  	return -1;
	}

//******************************* apodesmefsh ths mnhmhs ***************************		
for (i=0; i < sNumber-1; i++)	
	free(pinakas[i]);

free(pinakas);

free(f);
//**********************************************************************************
	return 0;
}

/*########################################################################################################################*/

int CS_InsertEntry( int fileDesc, Record record)
{
   int recordspace, inserted, i, blocknumber, col_fileDesc;
   idRec idr;
   nameRec namer;
   surnameRec surnamer;
   statusRec statusr;
   dateOfBirthRec dateOfBirthr;
   salaryRec salaryr;
   sectionRec sectionr;
   daysOffRec daysOffr;
   prevYearsRec prevYearsr;
   void *block;
   files buffer_f;
   char *buffer_c;
   buffer_c = malloc(25*sizeof(char));
   /* blocknumber : krata to plh8os ton block opos dinontai apo thn BF_GetBlockCounter
      recordspace : krata poses eggrafes exei to block
      inserted : os logikh metablhth gia na deixnei an eishx8ei to stoixeio sto arxeio h' oxi
   */
   if (BF_ReadBlock(fileDesc, 0, &block) < 0)
     {  BF_PrintError("Error reading block");
        return -1;
     }
    memcpy(&buffer_f,block+sizeof(info_block),sizeof(files));
    i = 0;
    while (i<9)
      {  if (i == 0)
           {  inserted = 0;
              strcpy(buffer_c,buffer_f.id);
              strcpy(buffer_c,strtok(buffer_c," "));
              strcpy(buffer_c,strtok(NULL," "));
              col_fileDesc = atoi(buffer_c); // Apomononetai to fileDesc toy column-store arxeioy kai apo8hkeyetai gia xrhsh toy argotera
              while (inserted == 0)
                   {  blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                      if (blocknumber < 0)
                        {  BF_PrintError("Error getting block");
                           return -1;
                        }
                      if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                        {  BF_PrintError("Error reading block");
                           return -1;
                        }
                      memcpy(&recordspace,block,sizeof(int));
                      if (recordspace < ((BLOCK_SIZE - sizeof(int))/sizeof(idRec)))
                        {  recordspace = recordspace + 1;
                           memcpy(block,&recordspace,sizeof(int));
                           idr.rowid = recordspace + (blocknumber-1)*((BLOCK_SIZE - sizeof(int))/sizeof(idRec));
                           idr.id = record.id;
                           memcpy(block+sizeof(int)+(recordspace-1)*sizeof(idRec),&idr,sizeof(idRec));
                           if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                             {  return -1;
                             }
                           inserted = 1;
/* An den yparxei la8os pernaei sto block to record pou dinetai */
                         }
                       else
                         {  if (BF_AllocateBlock(col_fileDesc) != 0)
                              {  return -1;
                              }
                            blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                            if (blocknumber < 0)
                              {  BF_PrintError("Error getting block");
                                 return -1;
                              }
                            if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                              {  BF_PrintError("Error reading block");
                                 return -1;
                              }
                            memcpy(block,&inserted,sizeof(int));
                            if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                              {  return -1;
                              }
/* An to block einai gemato dhmiourgeitai neo block kai mpainei se ayto to record */
                         }
                   }
           }
         if (i == 1)
           {  inserted = 0;
              strcpy(buffer_c,buffer_f.name);
              strcpy(buffer_c,strtok(buffer_c," "));
              strcpy(buffer_c,strtok(NULL," "));
              col_fileDesc = atoi(buffer_c); // Apomononetai to fileDesc toy column-store arxeioy kai apo8hkeyetai gia xrhsh toy argotera
              while (inserted == 0)
                {  blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                   if (blocknumber < 0)
                     {  BF_PrintError("Error getting block");
                        return -1;
                     }
                   if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                     {  BF_PrintError("Error reading block");
                        return -1;
                     }
                   memcpy(&recordspace,block,sizeof(int));
                   if (recordspace < ((BLOCK_SIZE - sizeof(int))/sizeof(nameRec)))
                     {  recordspace = recordspace + 1;
                        memcpy(block,&recordspace,sizeof(int));
                        namer.rowid = recordspace + (blocknumber-1)*((BLOCK_SIZE - sizeof(int))/sizeof(nameRec));
                        strcpy(namer.name,record.name);
                        memcpy(block+sizeof(int)+(recordspace-1)*sizeof(nameRec),&namer,sizeof(nameRec));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
                        inserted = 1;
/* An den yparxei la8os pernaei sto block to record pou dinetai */
                     }
                   else
                     {  if (BF_AllocateBlock(col_fileDesc) != 0)
                          {  return -1;
                          }
                        blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                        if (blocknumber < 0)
                          {  BF_PrintError("Error getting block");
                             return -1;
                          }
                        if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                          {  BF_PrintError("Error reading block");
                             return -1;
                          }
                        memcpy(block,&inserted,sizeof(int));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
/* An to block einai gemato dhmiourgeitai neo block kai mpainei se ayto to record */
                     }
                }
           }
         if (i == 2)
           {  inserted = 0;
              strcpy(buffer_c,buffer_f.surname);
              strcpy(buffer_c,strtok(buffer_c," "));
              strcpy(buffer_c,strtok(NULL," "));
              col_fileDesc = atoi(buffer_c); // Apomononetai to fileDesc toy column-store arxeioy kai apo8hkeyetai gia xrhsh toy argotera
              while (inserted == 0)
                {  blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                   if (blocknumber < 0)
                     {  BF_PrintError("Error getting block");
                        return -1;
                     }
                   if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                     {  BF_PrintError("Error reading block");
                        return -1;
                     }
                   memcpy(&recordspace,block,sizeof(int));
                   if (recordspace < ((BLOCK_SIZE - sizeof(int))/sizeof(surnameRec)))
                     {  recordspace = recordspace + 1;
                        memcpy(block,&recordspace,sizeof(int));
                        surnamer.rowid = recordspace + (blocknumber-1)*((BLOCK_SIZE - sizeof(int))/sizeof(surnameRec));
                        strcpy(surnamer.surname,record.surname);
                        memcpy(block+sizeof(int)+(recordspace-1)*sizeof(surnameRec),&surnamer,sizeof(surnameRec));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
                        inserted = 1;
/* An den yparxei la8os pernaei sto block to record pou dinetai */
                     }
                   else
                     {  if (BF_AllocateBlock(col_fileDesc) != 0)
                          {  return -1;
                          }
                        blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                        if (blocknumber < 0)
                          {  BF_PrintError("Error getting block");
                             return -1;
                          }
                        if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                          {  BF_PrintError("Error reading block");
                             return -1;
                          }
                        memcpy(block,&inserted,sizeof(int));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
/* An to block einai gemato dhmiourgeitai neo block kai mpainei se ayto to record */
                      }
                }
           }
         if (i == 3)
           {  inserted = 0;
              strcpy(buffer_c,buffer_f.status);
              strcpy(buffer_c,strtok(buffer_c," "));
              strcpy(buffer_c,strtok(NULL," "));
              col_fileDesc = atoi(buffer_c); // Apomononetai to fileDesc toy column-store arxeioy kai apo8hkeyetai gia xrhsh toy argotera
              while (inserted == 0)
                {  blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                   if (blocknumber < 0)
                     {  BF_PrintError("Error getting block");
                        return -1;
                     }
                   if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                     {  BF_PrintError("Error reading block");
                        return -1;
                     }
                   memcpy(&recordspace,block,sizeof(int));
                   if (recordspace < ((BLOCK_SIZE - sizeof(int))/sizeof(statusRec)))
                     {  recordspace = recordspace + 1;
                        memcpy(block,&recordspace,sizeof(int));
                        statusr.rowid = recordspace + (blocknumber-1)*((BLOCK_SIZE - sizeof(int))/sizeof(statusRec));
                        strcpy(statusr.status,record.status);
                        memcpy(block+sizeof(int)+(recordspace-1)*sizeof(statusRec),&statusr,sizeof(statusRec));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
                        inserted = 1;
/* An den yparxei la8os pernaei sto block to record pou dinetai */
                     }
                   else
                     {  if (BF_AllocateBlock(col_fileDesc) != 0)
                          {  return -1;
                          }
                        else
                          {  blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                             if (blocknumber < 0)
                               {  BF_PrintError("Error getting block");
                                  return -1;
                               }
                             if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                               {  BF_PrintError("Error reading block");
                                  return -1;
                               }
                             memcpy(block,&inserted,sizeof(int));
                             if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                               {  return -1;
                               }
/* An to block einai gemato dhmiourgeitai neo block kai mpainei se ayto to record */
                          }
                     }
                }
           }
         if (i == 4)
           {  inserted = 0;
              strcpy(buffer_c,buffer_f.dateOfBirth);
              strcpy(buffer_c,strtok(buffer_c," "));
              strcpy(buffer_c,strtok(NULL," "));
              col_fileDesc = atoi(buffer_c); // Apomononetai to fileDesc toy column-store arxeioy kai apo8hkeyetai gia xrhsh toy argotera
              while (inserted == 0)
                {  blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                   if (blocknumber < 0)
                     {  BF_PrintError("Error getting block");
                        return -1;
                     }
                   if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                     {  BF_PrintError("Error reading block");
                        return -1;
                     }
                   memcpy(&recordspace,block,sizeof(int));
                   if (recordspace < ((BLOCK_SIZE - sizeof(int))/sizeof(dateOfBirthRec)))
                     {  recordspace = recordspace + 1;
                        memcpy(block,&recordspace,sizeof(int));
                        dateOfBirthr.rowid = recordspace + (blocknumber-1)*((BLOCK_SIZE - sizeof(int))/sizeof(dateOfBirthRec));
                        strcpy(dateOfBirthr.dateOfBirth,record.dateOfBirth);
                        memcpy(block+sizeof(int)+(recordspace-1)*sizeof(dateOfBirthRec),&dateOfBirthr,sizeof(dateOfBirthRec));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
                        inserted = 1;
/* An den yparxei la8os pernaei sto block to record pou dinetai */
                     }
                   else
                     {  if (BF_AllocateBlock(col_fileDesc) != 0)
                          {  return -1;
                          }
                        blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                        if (blocknumber < 0)
                          {  BF_PrintError("Error getting block");
                             return -1;
                          }
                        if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                          {  BF_PrintError("Error reading block");
                             return -1;
                          }
                        memcpy(block,&inserted,sizeof(int));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
/* An to block einai gemato dhmiourgeitai neo block kai mpainei se ayto to record */
                     }
                }
           }
         if (i == 5)
           {  inserted = 0;
              strcpy(buffer_c,buffer_f.salary);
              strcpy(buffer_c,strtok(buffer_c," "));
              strcpy(buffer_c,strtok(NULL," "));
              col_fileDesc = atoi(buffer_c); // Apomononetai to fileDesc toy column-store arxeioy kai apo8hkeyetai gia xrhsh toy argotera
              while (inserted == 0)
                {  blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                   if (blocknumber < 0)
                     {  BF_PrintError("Error getting block");
                        return -1;
                     }
                   if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                     {  BF_PrintError("Error reading block");
                        return -1;
                     }
                   memcpy(&recordspace,block,sizeof(int));
                   if (recordspace < ((BLOCK_SIZE - sizeof(int))/sizeof(salaryRec)))
                     {  recordspace = recordspace + 1;
                        memcpy(block,&recordspace,sizeof(int));
                        salaryr.rowid = recordspace + (blocknumber-1)*((BLOCK_SIZE - sizeof(int))/sizeof(salaryRec));
                        salaryr.salary = record.salary;
                        memcpy(block+sizeof(int)+(recordspace-1)*sizeof(salaryRec),&salaryr,sizeof(salaryRec));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
                        inserted = 1;
/* An den yparxei la8os pernaei sto block to record pou dinetai */
                     }
                   else
                     {  if (BF_AllocateBlock(col_fileDesc) != 0)
                          {  return -1;
                          }
                        blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                        if (blocknumber < 0)
                          {  BF_PrintError("Error getting block");
                             return -1;
                          }
                        if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                          {  BF_PrintError("Error reading block");
                             return -1;
                          }
                        memcpy(block,&inserted,sizeof(int));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
/* An to block einai gemato dhmiourgeitai neo block kai mpainei se ayto to record */
                     }
                 }
           }
         if (i == 6)
              {  inserted = 0;
                 strcpy(buffer_c,buffer_f.section);
                 strcpy(buffer_c,strtok(buffer_c," "));
                 strcpy(buffer_c,strtok(NULL," "));
                 col_fileDesc = atoi(buffer_c); // Apomononetai to fileDesc toy column-store arxeioy kai apo8hkeyetai gia xrhsh toy argotera
                 while (inserted == 0)
                   {  blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                      if (blocknumber < 0)
                        {  BF_PrintError("Error getting block");
                           return -1;
                        }
                      if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                        {  BF_PrintError("Error reading block");
                           return -1;
                        }
                      memcpy(&recordspace,block,sizeof(int));
                      if (recordspace < ((BLOCK_SIZE - sizeof(int))/sizeof(sectionRec)))
                        {  recordspace = recordspace + 1;
                           memcpy(block,&recordspace,sizeof(int));
                           sectionr.rowid = recordspace + (blocknumber-1)*((BLOCK_SIZE - sizeof(int))/sizeof(sectionRec));
                           strcpy(sectionr.section,record.section);
                           memcpy(block+sizeof(int)+(recordspace-1)*sizeof(sectionRec),&sectionr,sizeof(sectionRec));
                           if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                             {  return -1;
                             }
                           inserted = 1;
/* An den yparxei la8os pernaei sto block to record pou dinetai */
                        }
                      else
                        {  if (BF_AllocateBlock(col_fileDesc) != 0)
                             {  return -1;
                             }
                           blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                           if (blocknumber < 0)
                             {  BF_PrintError("Error getting block");
                                return -1;
                             }
                           if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                             {  BF_PrintError("Error reading block");
                                return -1;
                             }
                           memcpy(block,&inserted,sizeof(int));
                           if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                             {  return -1;
                             }
/* An to block einai gemato dhmiourgeitai neo block kai mpainei se ayto to record */
                     }
                 }
           }
         if (i == 7)
           {  inserted = 0;
              strcpy(buffer_c,buffer_f.daysOff);
              strcpy(buffer_c,strtok(buffer_c," "));
              strcpy(buffer_c,strtok(NULL," "));
              col_fileDesc = atoi(buffer_c); // Apomononetai to fileDesc toy column-store arxeioy kai apo8hkeyetai gia xrhsh toy argotera
              while (inserted == 0)
                {  blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                   if (blocknumber < 0)
                     {  BF_PrintError("Error getting block");
                        return -1;
                     }
                   if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                     {  BF_PrintError("Error reading block");
                        return -1;
                     }
                   memcpy(&recordspace,block,sizeof(int));
                   if (recordspace < ((BLOCK_SIZE - sizeof(int))/sizeof(daysOffRec)))
                     {  recordspace = recordspace + 1;
                        memcpy(block,&recordspace,sizeof(int));
                        daysOffr.rowid = recordspace + (blocknumber-1)*((BLOCK_SIZE - sizeof(int))/sizeof(daysOffRec));
                        daysOffr.daysOff = record.daysOff;
                        memcpy(block+sizeof(int)+(recordspace-1)*sizeof(daysOffRec),&daysOffr,sizeof(daysOffRec));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
                        inserted = 1;
/* An den yparxei la8os pernaei sto block to record pou dinetai */
                     }
                   else
                     {  if (BF_AllocateBlock(col_fileDesc) != 0)
                          {  return -1;
                          }
                        blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                        if (blocknumber < 0)
                          {  BF_PrintError("Error getting block");
                             return -1;
                          }
                        if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                          {  BF_PrintError("Error reading block");
                             return -1;
                          }
                        memcpy(block,&inserted,sizeof(int));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
/* An to block einai gemato dhmiourgeitai neo block kai mpainei se ayto to record */
                     }
                }
           }
         if (i == 8)
           {  inserted = 0;
              strcpy(buffer_c,buffer_f.prevYears);
              strcpy(buffer_c,strtok(buffer_c," "));
              strcpy(buffer_c,strtok(NULL," "));
              col_fileDesc = atoi(buffer_c); // Apomononetai to fileDesc toy column-store arxeioy kai apo8hkeyetai gia xrhsh toy argotera
              while (inserted == 0)
                {  blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                   if (blocknumber < 0)
                     {  BF_PrintError("Error getting block");
                        return -1;
                     }
                   if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                     {  BF_PrintError("Error reading block");
                        return -1;
                     }
                   memcpy(&recordspace,block,sizeof(int));
                   if (recordspace < ((BLOCK_SIZE - sizeof(int))/sizeof(prevYearsRec)))
                     {  recordspace = recordspace + 1;
                        memcpy(block,&recordspace,sizeof(int));
                        prevYearsr.rowid = recordspace + (blocknumber-1)*((BLOCK_SIZE - sizeof(int))/sizeof(prevYearsRec));
                        prevYearsr.prevYears = record.prevYears;
                        memcpy(block+sizeof(int)+(recordspace-1)*sizeof(prevYearsRec),&prevYearsr,sizeof(prevYearsRec));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
                        inserted = 1;
/* An den yparxei la8os pernaei sto block to record pou dinetai */
                     }
                   else
                     {  if (BF_AllocateBlock(col_fileDesc) != 0)
                          {  return -1;
                          }
                        blocknumber = BF_GetBlockCounter(col_fileDesc)-1;
                        if (blocknumber < 0)
                          {  BF_PrintError("Error getting block");
                             return -1;
                          }
                        if (BF_ReadBlock(col_fileDesc, blocknumber, &block) < 0)
                          {  BF_PrintError("Error reading block");
                             return -1;
                          }
                        memcpy(block,&inserted,sizeof(int));
                        if (BF_WriteBlock(col_fileDesc, blocknumber) < 0)
                          {  return -1;
                          }
/* An to block einai gemato dhmiourgeitai neo block kai mpainei se ayto to record */
                     }
                }
           }
         i++;
      }
/******************************** apodesmefsh ths mnhmhs ****************************/
   free(buffer_c);
/******************************** apodesmefsh ths mnhmhs ****************************/
   return fileDesc;
}

/*########################################################################################################################*/

void CS_GetAllEntries(int fileDesc, char* fieldName, void* value, char** fieldNames, int n)
{	int counter;	/* header_filename: "filename_header_info fileDesc_header_info" */
  	void* block;
	int* v = value; 
  	//void* data;
  	int i=0, j=0, z=0, metriths, pimp = -1;
  	files* f;  // voithitikh struct "files"
	char** pinakas;
	char** records;
//	char* temp;
	int cNumber = 20;	// plithos xarakthron pu tha xoraei kathe string tou pinaka "pinakas"
	int rowid = -1;  

	int k = 0;	// metriths ton blocks pu diavazode kathe fora (afkasanetai kata 1, kathe fora pu kalite h BF_ReadBlock)
	
// open the txt file
  	FILE *fp;

	if ((fp = fopen("./Entries.txt", "w")) == NULL) /* Open Print_Data File */
    {	perror("fopen Entries-file");
    }

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  void getTheRecord(int recordcounter, int x)
  {	if (strcmp(fieldNames[x], "id") == 0)
	{	idRec data;
		memcpy(&data, block + sizeof(int) + recordcounter*sizeof(idRec), sizeof(idRec));
		fprintf(fp, "%d  ", data.id);
	}
	else if (strcmp(fieldNames[x], "name") == 0)
	{	nameRec data;
		memcpy(&data, block + sizeof(int) + recordcounter*sizeof(nameRec), sizeof(nameRec));
		fprintf(fp, "%s  ", data.name);
	}
	else if (strcmp(fieldNames[x], "surname") == 0)
	{	surnameRec data;
		memcpy(&data, block + sizeof(int) + recordcounter*sizeof(surnameRec), sizeof(surnameRec));
		fprintf(fp, "%s  ", data.surname);
	}
	else if (strcmp(fieldNames[x], "status") == 0)
	{	statusRec data;
		memcpy(&data, block + sizeof(int) + recordcounter*sizeof(statusRec), sizeof(statusRec));
		fprintf(fp, "%s  ", data.status);
	}
	else if (strcmp(fieldNames[x], "dateOfBirth") == 0)
	{	dateOfBirthRec data;
		memcpy(&data, block + sizeof(int) + recordcounter*sizeof(dateOfBirthRec), sizeof(dateOfBirthRec));
		fprintf(fp, "%s  ", data.dateOfBirth);
	}
	else if (strcmp(fieldNames[x], "salary") == 0)
	{	salaryRec data;
		memcpy(&data, block + sizeof(int) + recordcounter*sizeof(salaryRec), sizeof(salaryRec));
		fprintf(fp, "%d  ", data.salary);
	}
	else if (strcmp(fieldNames[x], "section") == 0)
	{	sectionRec data;
		memcpy(&data, block + sizeof(int) + recordcounter*sizeof(sectionRec), sizeof(sectionRec));
		fprintf(fp, "%s  ", data.section);
	}
	else if (strcmp(fieldNames[x], "daysOff") == 0)
	{	daysOffRec data;
		memcpy(&data, block + sizeof(int) + recordcounter*sizeof(daysOffRec), sizeof(daysOffRec));
		fprintf(fp, "%d  ", data.daysOff);
	}
	else if (strcmp(fieldNames[x], "prevYears") == 0)
	{	 prevYearsRec data;
		memcpy(&data, block + sizeof(int) + recordcounter*sizeof(prevYearsRec), sizeof(prevYearsRec));
		fprintf(fp, "%d  ", data.prevYears);
	}
  }
////////////////////////////////////////////////////////////////////////////////
  void InFunction()	// epistrefei to plithos ton blocks pu diavastika
  {	int x,y,w;
	int Counter, Metriths;
/*
printf("%d  %d  %d  %d  %d  %d  %d  %d  %d\n", sizeof(idRec), sizeof(nameRec), sizeof(surnameRec), sizeof(statusRec), sizeof(dateOfBirthRec), sizeof(salaryRec), sizeof(sectionRec), sizeof(daysOffRec), sizeof(prevYearsRec));	*/
	for (x=0; x < n; x++)
	{	if (x != pimp)	// ektos tou fieldName
		{	if((Counter = BF_GetBlockCounter(atoi(pinakas[x]))) < 0)	// anigma tou trexon pediou fieldNames
			{	BF_PrintError("Error getting block counter");
				exit(EXIT_FAILURE);
			}

           	for (y=1; y < Counter; y++)   // vgazo ektos to proto block pu einai gia th pliroforia anagnorishs arxeiou
    		{   if (BF_ReadBlock(atoi(pinakas[x]), y, &block) < 0)  // fortosh tu block sth mnhmh
				{   BF_PrintError("Error reading block 1");
					exit(EXIT_FAILURE);
				}
				memcpy(&Metriths, block,sizeof(int));   // anagnosh tou metrhth egrafon

				k++;

				for(w=0; w < Metriths/*h egrafes se kathe block dedomenon*/; w++)
				{  if (strcmp(fieldNames[x], "id") == 0)
					{	idRec data;
						memcpy(&data, block + sizeof(int) + w*sizeof(idRec), sizeof(idRec));
   	                	if (data.rowid == rowid)
 				 	    {	
							sprintf(records[x], "%d", data.id);// apothikefsh ths timhs, os string, ston pinaka
						}
					}
					else if (strcmp(fieldNames[x], "name") == 0)
					{	nameRec data;
						memcpy(&data, block + sizeof(int) + w*sizeof(nameRec), sizeof(nameRec));
						
   	                	if (data.rowid == rowid)
 				 	    {	
							strcpy(records[x], data.name); // apothikefsh ths timhs, os string, ston pinaka
						}
					}
					else if (strcmp(fieldNames[x], "surname") == 0)
					{	surnameRec data;
						memcpy(&data, block + sizeof(int) + w*sizeof(surnameRec), sizeof(surnameRec));
   	                	if (data.rowid == rowid)
 				 	    {	
							strcpy(records[x], data.surname); // apothikefsh ths timhs, os string, ston pinaka
						}
					}
					else if (strcmp(fieldNames[x], "status") == 0)
					{	statusRec data;
						memcpy(&data, block + sizeof(int) + w*sizeof(statusRec), sizeof(statusRec));
						if (data.rowid == rowid)
 				 	    {	
							strcpy(records[x], data.status); // apothikefsh ths timhs, os string, ston pinaka
						}
					}
					else if (strcmp(fieldNames[x], "dateOfBirth") == 0)
					{	dateOfBirthRec data;	
						memcpy(&data, block + sizeof(int) + w*sizeof(dateOfBirthRec), sizeof(dateOfBirthRec));
   	                	if (data.rowid == rowid)
 				 	    {	
							strcpy(records[x], data.dateOfBirth); // apothikefsh ths timhs, os string, ston pinaka
						}
					}
					else if (strcmp(fieldNames[x], "salary") == 0)
					{	salaryRec data;
						memcpy(&data, block + sizeof(int) + w*sizeof(salaryRec), sizeof(salaryRec));
						if (data.rowid == rowid)
 				 	    {	
							sprintf(records[x], "%d", data.salary); // apothikefsh ths timhs, os string, ston pinaka
						}
					}
					else if (strcmp(fieldNames[x], "section") == 0)
					{	sectionRec data;
						memcpy(&data, block + sizeof(int) + w*sizeof(sectionRec), sizeof(sectionRec));
						   	                	
						if (data.rowid == rowid)
 				 	    {	
							strcpy(records[x], data.section); // apothikefsh ths timhs, os string, ston pinaka
						}
					}
					else if (strcmp(fieldNames[x], "daysOff") == 0)
					{	daysOffRec data;
						memcpy(&data, block + sizeof(int) + w*sizeof(daysOffRec), sizeof(daysOffRec));
   	                	if (data.rowid == rowid)
 				 	    {	
							sprintf(records[x], "%d", data.daysOff); // apothikefsh ths timhs, os string, ston pinaka
						}
					}
					else if (strcmp(fieldNames[x], "prevYears") == 0)
					{	prevYearsRec data;
						memcpy(&data, block + sizeof(int) + w*sizeof(prevYearsRec), sizeof(prevYearsRec));
   	                	if (data.rowid == rowid)
 				 	    {	
							sprintf(records[x], "%d", data.prevYears); // apothikefsh ths timhs, os string, ston pinaka
						}
					}
				}
			}
			k += y-1;
		}
		
	}
///////////////////////// apotiposh ton egrafon sto arxeio pros anagnosh(.txt) /////////////////////////////////////////////////////
	for (x=0; x < n; x++)
		fprintf(fp, "%s  ", records[x]);	
	fprintf(fp, "\n");
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  }
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  	
	f = malloc(sizeof(files));

	pinakas = malloc(sizeof(char*)*(n+1));	// desmefsh voithitikou pinaka
	for (i=0; i < n+1; i++)
		pinakas[i] = malloc(sizeof(char)*cNumber);

	records = malloc(sizeof(char*)*n);	// desmefsh voithitikou pinaka gia thn apothikefsh ton egrafon
	for (i=0; i < n; i++)
		records[i] = malloc(sizeof(char)*cNumber);


        sprintf(pinakas[n],"%d",fileDesc);//pinakas[n] = (char*)(((int)'0')+fileDesc);
        strcat(pinakas[n], "");	// termatizo to string sto pinaka[n] me NULL xarakthra

	if (BF_ReadBlock(atoi(pinakas[n]), 0, &block) < 0)	// anagnosh tou protou block tou genikou arxeiou
	{	BF_PrintError("Error reading block 2");
		exit(EXIT_FAILURE);
	}

	k++;

	memcpy(f, block + sizeof(info_block), sizeof(files));	// anagnosh ths struct anagnoristikon arxeion

  if (fieldNames != NULL)
  {	for (i=0; i < n; i++)
	{	if (strcmp(fieldNames[i], "id") == 0)
		{	strcpy(pinakas[i], strtok(f->id, " "));	
			strcpy(pinakas[i],  strtok(NULL, ""));		
		}
		else if (strcmp(fieldNames[i], "name") == 0)
		{	strcpy(pinakas[i], strtok(f->name, " "));
			strcpy(pinakas[i],  strtok(NULL, ""));
		}
		else if (strcmp(fieldNames[i], "surname") == 0)
		{	strcpy(pinakas[i], strtok(f->surname, " "));
			strcpy(pinakas[i],  strtok(NULL, ""));
		}
		else if (strcmp(fieldNames[i], "status") == 0)
		{	strcpy(pinakas[i], strtok(f->status, " "));
			strcpy(pinakas[i],  strtok(NULL, "")); 
		}
		else if (strcmp(fieldNames[i], "dateOfBirth") == 0)
		{	strcpy(pinakas[i], strtok(f->dateOfBirth, " "));
			strcpy(pinakas[i],  strtok(NULL, "")); 
		}
		else if (strcmp(fieldNames[i], "salary") == 0)
		{	strcpy(pinakas[i], strtok(f->salary, " "));
			strcpy(pinakas[i],  strtok(NULL, "")); 
		}
		else if (strcmp(fieldNames[i], "section") == 0)
		{	strcpy(pinakas[i], strtok(f->section, " "));
			strcpy(pinakas[i],  strtok(NULL, "")); 
		}
		else if (strcmp(fieldNames[i], "daysOff") == 0)
		{	strcpy(pinakas[i], strtok(f->daysOff, " "));
			strcpy(pinakas[i],  strtok(NULL, "")); 
		}
		else if (strcmp(fieldNames[i], "prevYears") == 0)
		{	strcpy(pinakas[i], strtok(f->prevYears, " "));
			strcpy(pinakas[i],  strtok(NULL, "")); 
		}
		if (fieldName != NULL && strcmp(fieldNames[i], fieldName) == 0)
		{	pimp = i;
		}		
	}
  }	
 
    if (fieldName == NULL && value == NULL && fieldNames != NULL)    // periptosh NULL
    {   fprintf(fp, "ALL ENTRIES\n");   // oles oi kataxoriseis
		int blockcounter[n]; 
		int recordcounter[n];
		int pimp = 1;
		for (i=0; i < n; i++)
		{	blockcounter[i] = 1;	// ksekina h metrish apo to block 1
			recordcounter[i] = 0;	// h metrish ksekina apo thn egrafh 0
		}
		
		while (pimp == 1)
		{	for (i=0; i < n; i++)     // gia kathe pedio pu thelo na emfanistei...   
			{	int printed = 0;
				while (printed == 0)
				{	if((counter = BF_GetBlockCounter(atoi(pinakas[i]))) < 0)	//...pare to plithos ton blocks
					{	BF_PrintError("Error getting block counter");
						exit(EXIT_FAILURE);
					}
					if (blockcounter[i] < counter)	
					{	if(BF_ReadBlock(atoi(pinakas[i]), blockcounter[i], &block) < 0)	// anikse to block me dikth, blockcounter
						{	BF_PrintError("Error reading block 3");
							exit(EXIT_FAILURE);
						}
	
				        k++;

						memcpy(&metriths, block, sizeof(int));	// diabase to metrith egrafon
						if (recordcounter[i] < metriths)
						{	getTheRecord(recordcounter[i], i);	// pare thn egrafh me arithmo recordcounter
							recordcounter[i]++;
							printed = 1;
						}
						else 
						{	recordcounter[i] = 0;
							blockcounter[i]++;

							k++;
						}
					}
					else	// diaforetika vges apo to while
					{	pimp = 0;
						break;
					}
				}
			}
			fprintf(fp, "\n");	// alagh gramhs gia thn epomenh omada pedion
		}
printf("Plh8os blocks poy diabastikan = %d\n", k);

	}   
    else if (fieldName != NULL && value != NULL && fieldNames != NULL)  // den exo NULL
    {	if (strcmp(fieldName, "id") == 0)
		{  
			fprintf(fp, "ID\n");    // pedio id
		
			if((counter = BF_GetBlockCounter(atoi(pinakas[pimp]))) < 0)	// posa blocks exei to pedio fieldName
			{	BF_PrintError("Error getting block counter");
				exit(EXIT_FAILURE);
			}

           	for (j=1; j < counter; j++)   // vgazo ektos to proto block pu einai gia th pliroforia anagnorishs arxeiou
    		{   if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
				{   BF_PrintError("Error reading block 4");
					exit(EXIT_FAILURE);
				}

				k++;

				memcpy(&metriths, block,sizeof(int));   // anagnosh tou metrhth egrafon
				for(z=0; z < metriths/*h egrafes se kathe block dedomenon*/; z++)
				{   
					if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
					{   BF_PrintError("Error reading block 5");
						exit(EXIT_FAILURE);
					}	
	
					k++;

					idRec data;
					memcpy(&data, block + sizeof(int) + z*sizeof(idRec), sizeof(idRec));
					   	                
					if (data.id == *v)
				    {	rowid = data.rowid;
						sprintf(records[pimp], "%d", data.id); // apothikefsh ths timhs, os string, ston pinaka

						InFunction();	// adlish pliroforias apo ta pedia fieldNames
					}
				}
			}
		}	
		else if (strcmp(fieldName, "name") == 0)
		{	fprintf(fp, "NAME\n");  // pedio name
		
    	    if((counter = BF_GetBlockCounter(atoi(pinakas[pimp]))) < 0)	// posa blocks exei to pedio fieldName
				{	BF_PrintError("Error getting block counter");
					exit(EXIT_FAILURE);
				}

    	       	for (j=1; j < counter; j++)   // vgazo ektos to proto block pu einai gia th pliroforia anagnorishs arxeiou
    			{   if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
					{   BF_PrintError("Error reading block 5");
						exit(EXIT_FAILURE);
					}

					k++;

					memcpy(&metriths, block,sizeof(int));   // anagnosh tou metrhth egrafon
					for(z=0; z < metriths/*h egrafes se kathe block dedomenon*/; z++)
					{   
						if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
						{   BF_PrintError("Error reading block 5");
							exit(EXIT_FAILURE);
						}						

						k++;

						nameRec data;
						memcpy(&data, block + sizeof(int) + z*sizeof(nameRec), sizeof(nameRec));
					
   		                if (strcmp(data.name, value) == 0)
					    {	rowid = data.rowid;
							strcpy(records[pimp], data.name); // apothikefsh ths timhs, os string, ston pinaka
						
							InFunction();	// adlish pliroforias apo ta pedia fieldNames
						}
					}
				}

		}
		else if (strcmp(fieldName, "surname") == 0)
		{   fprintf(fp, "SURNAME\n");   // pedio surname
    	    
    	    if((counter = BF_GetBlockCounter(atoi(pinakas[pimp]))) < 0)	// posa blocks exei to pedio fieldName
				{	BF_PrintError("Error getting block counter");
					exit(EXIT_FAILURE);
				}

    	       	for (j=1; j < counter; j++)   // vgazo ektos to proto block pu einai gia th pliroforia anagnorishs arxeiou
    			{   if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
					{   BF_PrintError("Error reading block 6");
						exit(EXIT_FAILURE);
					}

					k++;

					memcpy(&metriths, block,sizeof(int));   // anagnosh tou metrhth egrafon
					for(z=0; z < metriths/*h egrafes se kathe block dedomenon*/; z++)
					{	
						if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
						{   BF_PrintError("Error reading block 5");
							exit(EXIT_FAILURE);
						}	

						k++;

						surnameRec data;
						memcpy(&data, block + sizeof(int) + z*sizeof(surnameRec), sizeof(surnameRec));
						
   		                if (strcmp(data.surname, value) == 0)
					    {	rowid = data.rowid;
							strcpy(records[pimp], data.surname); // apothikefsh ths timhs, os string, ston pinaka
						
							InFunction();	// adlish pliroforias apo ta pedia fieldNames
						}
					}
				}
		}
		else if (strcmp(fieldName, "status") == 0)
		{   fprintf(fp, "STATUS\n");    // pedio status
    	    
    	    if((counter = BF_GetBlockCounter(atoi(pinakas[pimp]))) < 0)	// posa blocks exei to pedio fieldName
			{	BF_PrintError("Error getting block counter");
				exit(EXIT_FAILURE);
			}
	
           	for (j=1; j < counter; j++)   // vgazo ektos to proto block pu einai gia th pliroforia anagnorishs arxeiou
    		{   if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
				{   BF_PrintError("Error reading block 7");
					exit(EXIT_FAILURE);
				}

				k++;

				memcpy(&metriths, block,sizeof(int));   // anagnosh tou metrhth egrafon
				for(z=0; z < metriths/*h egrafes se kathe block dedomenon*/; z++)
				{   
					if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
					{   BF_PrintError("Error reading block 5");
						exit(EXIT_FAILURE);
					}	

					k++;

					statusRec data;
					memcpy(&data, block + sizeof(int) + z*sizeof(statusRec), sizeof(statusRec));
					
   	                if (strcmp(data.status, value) == 0)
				    {	rowid = data.rowid;
						strcpy(records[pimp], data.status); // apothikefsh ths timhs, os string, ston pinaka

						InFunction();	// adlish pliroforias apo ta pedia fieldNames
					}
				}
			}
		}
		else if (strcmp(fieldName, "dateOfBirth") == 0)
		{   fprintf(fp, "DATE OF BIRTH\n"); // pedio dateOfBirth
       
        if((counter = BF_GetBlockCounter(atoi(pinakas[pimp]))) < 0)	// posa blocks exei to pedio fieldName
			{	BF_PrintError("Error getting block counter");
				exit(EXIT_FAILURE);
			}

           	for (j=1; j < counter; j++)   // vgazo ektos to proto block pu einai gia th pliroforia anagnorishs arxeiou
    		{   if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
				{   BF_PrintError("Error reading block 8");
					exit(EXIT_FAILURE);
				}

				k++;

				memcpy(&metriths, block,sizeof(int));   // anagnosh tou metrhth egrafon
				for(z=0; z < metriths/*h egrafes se kathe block dedomenon*/; z++)
				{   
					if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
					{   BF_PrintError("Error reading block 5");
						exit(EXIT_FAILURE);
					}	

					k++;

					dateOfBirthRec data;
					memcpy(&data, block + sizeof(int) + z*sizeof(dateOfBirthRec), sizeof(dateOfBirthRec));
					
   	                if (strcmp(data.dateOfBirth, value) == 0)
				    {	rowid = data.rowid;
						strcpy(records[pimp], data.dateOfBirth); // apothikefsh ths timhs, os string, ston pinaka
				
						InFunction();	// adlish pliroforias apo ta pedia fieldNames	
					}
				}			
			}
		}
		else if (strcmp(fieldName, "salary") == 0)
		{   fprintf(fp, "SALARY\n");    // pedio salary
    	           
    	    if((counter = BF_GetBlockCounter(atoi(pinakas[pimp]))) < 0)	// posa blocks exei to pedio fieldName
			{	BF_PrintError("Error getting block counter");
				exit(EXIT_FAILURE);
			}

           	for (j=1; j < counter; j++)   // vgazo ektos to proto block pu einai gia th pliroforia anagnorishs arxeiou
    		{   if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
				{   BF_PrintError("Error reading block 9");
					exit(EXIT_FAILURE);
				}

				k++;

				memcpy(&metriths, block,sizeof(int));   // anagnosh tou metrhth egrafon
				for(z=0; z < metriths/*h egrafes se kathe block dedomenon*/; z++)
				{   
					if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
					{   BF_PrintError("Error reading block 5");
						exit(EXIT_FAILURE);
					}

					k++;

					salaryRec data;
					memcpy(&data, block + sizeof(int) + z*sizeof(salaryRec), sizeof(salaryRec));
					
   	                if (data.salary == *v)
				    {	rowid = data.rowid;
						sprintf(records[pimp], "%d", data.salary); // apothikefsh ths timhs, os string, ston pinaka

						InFunction();	// adlish pliroforias apo ta pedia fieldNames
					}
				}
			}
		}
		else if (strcmp(fieldName, "section") == 0)
		{    fprintf(fp, "SECTION\n");   // pedio section
	                
	        if((counter = BF_GetBlockCounter(atoi(pinakas[pimp]))) < 0)	// posa blocks exei to pedio fieldName
			{	BF_PrintError("Error getting block counter");
				exit(EXIT_FAILURE);
			}

           	for (j=1; j < counter; j++)   // vgazo ektos to proto block pu einai gia th pliroforia anagnorishs arxeiou
    		{   if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
				{   BF_PrintError("Error reading block 10");
					exit(EXIT_FAILURE);
				}

				k++;

				memcpy(&metriths, block,sizeof(int));   // anagnosh tou metrhth egrafon
				for(z=0; z < metriths/*h egrafes se kathe block dedomenon*/; z++)
				{   
					if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
					{   BF_PrintError("Error reading block 5");
						exit(EXIT_FAILURE);
					}

					k++;

					sectionRec data;
					memcpy(&data, block + sizeof(int) + z*sizeof(sectionRec), sizeof(sectionRec));
					
   	                if (strcmp(data.section, value) == 0)
				    {	rowid = data.rowid;
						strcpy(records[pimp], data.section); // apothikefsh ths timhs, os string, ston pinaka

						InFunction();	// adlish pliroforias apo ta pedia fieldNames
					}
				}
			}
   	 	}
		else if (strcmp(fieldName, "daysOff") == 0)
		{   fprintf(fp, "DAYS OFF\n");  // pedio daysOff
        	       
       		if((counter = BF_GetBlockCounter(atoi(pinakas[pimp]))) < 0)	// posa blocks exei to pedio fieldName
			{	BF_PrintError("Error getting block counter");
				exit(EXIT_FAILURE);
			}

           	for (j=1; j < counter; j++)   // vgazo ektos to proto block pu einai gia th pliroforia anagnorishs arxeiou
    		{   if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
				{   BF_PrintError("Error reading block 11");
					exit(EXIT_FAILURE);
				}

				k++;

				memcpy(&metriths, block,sizeof(int));   // anagnosh tou metrhth egrafon
				for(z=0; z < metriths/*h egrafes se kathe block dedomenon*/; z++)
				{  
					if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
					{   BF_PrintError("Error reading block 5");
						exit(EXIT_FAILURE);
					}	

					k++;

					daysOffRec data;
					memcpy(&data, block + sizeof(int) + z*sizeof(daysOffRec), sizeof(daysOffRec));
					
   	                if (data.daysOff == *v)
				    {	rowid = data.rowid;
						sprintf(records[pimp], "%d", data.daysOff); // apothikefsh ths timhs, os string, ston pinaka

						InFunction();	// adlish pliroforias apo ta pedia fieldNames
					}
				}
			}	
		}
		else if (strcmp(fieldName, "prevYears") == 0)
		{   fprintf(fp, "PREVIOUS YEARS\n");    // pedio prevYears
               
    	    if((counter = BF_GetBlockCounter(atoi(pinakas[pimp]))) < 0)	// posa blocks exei to pedio fieldName
			{	BF_PrintError("Error getting block counter");
				exit(EXIT_FAILURE);
			}

           	for (j=1; j < counter; j++)   // vgazo ektos to proto block pu einai gia th pliroforia anagnorishs arxeiou
    		{   if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
				{   BF_PrintError("Error reading block 12");
					exit(EXIT_FAILURE);
				}

				k++;

				memcpy(&metriths, block,sizeof(int));   // anagnosh tou metrhth egrafon
				for(z=0; z < metriths/*h egrafes se kathe block dedomenon*/; z++)
				{ 
					if (BF_ReadBlock(atoi(pinakas[pimp]), j, &block) < 0)  // fortosh tou block, tou pediou fieldName, sth mnhmh
					{   BF_PrintError("Error reading block 5");
						exit(EXIT_FAILURE);
					}	

					k++;

					prevYearsRec data;
					memcpy(&data, block + sizeof(int) + z*sizeof(prevYearsRec), sizeof(prevYearsRec));
					
   	                if (data.prevYears == *v)
				    {	rowid = data.rowid;
						sprintf(records[pimp], "%d", data.prevYears); // apothikefsh ths timhs, os string, ston pinaka

						InFunction();	// adlish pliroforias apo ta pedia fieldNames
					}
				}
			}
		}
printf("Plh8os blocks poy diabastikan = %d\n",(j-1)+k);
	}

fclose(fp);

//////////////////////////////////	eleftherosh mnhmhs ///////////////////////////////////////////////////////////////////////////
free(f); 

for (i=0; i < n+1; i++)
	free(pinakas[i]);
free(pinakas);

for (i=0; i < n; i++)
	free(records[i]);
free(records);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

