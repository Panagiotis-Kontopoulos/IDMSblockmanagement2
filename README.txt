Compilation command
  * If file type in main.c is 0
    gcc -o askisi main.c HP_functions.c BF.o

  * If file type in main.c is 1
    gcc -o askisi main.c HT_functions.c BF.o

    The attrType, attrLength, depth(maximum depth 7) variables 
    change inside main before compilation

    For the number of records in block
    uncomment one of the below lines in main.c
    -> for (j=0;j<6;j++)
    -> while (!feof(records))

In function CS_GetAllEntries
    If parameter fieldName show to int
    { "id" , "salary" , "daysOff" , "prevYears" }
    then parameter value in main will be given parameter &value,
    that her value before the call of the function.
    If parameter fieldName shows to chars
    { "name" , "surname" , "status" , "dateOfBirth" , "section" }
    then parameter value in main will be given value: ".....",
    where ..... represent any character.
