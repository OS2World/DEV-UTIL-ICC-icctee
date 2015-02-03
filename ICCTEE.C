#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRINGS 50 

unsigned char * tStringToIgnore[MAX_STRINGS] ;
int    nStringToCheck = 0 ;

int CheckLine(unsigned char * line)
{
  int i;

  if (!nStringToCheck)
    return 1 ;
  
  if (strstr(line,"warning EDC")==NULL)
    return 1 ;

  for (i=0 ; i<nStringToCheck ; i++)
    if (strstr(line,tStringToIgnore[i])!=NULL)
      return 0 ;

  return 1 ;
}

main(int argc,char * argv[])
{
  FILE * fileOut=(FILE *)0;
  int    errFound = 0 ;
  int    i ;

  if (argc<2)
    {
    fprintf(stderr,"Usage:\n\t%s filename [warning number] ...\n\n",argv[0]);
    exit(-2);
    }

  /*
  ** if the first argument is a - don't perform the TEE function
  */

  if (strcmp(argv[1],"-")!=0)
    {
    fileOut=fopen(argv[1],"w");
    if (!fileOut)
      {
      fprintf(stderr,"Can't open <%s>\n",argv[1]);
      exit(-3);
      }
    }

  /*
  ** read all warning to disable
  */

  for (i=2 ; i<argc ; i++)  /* ok ok there is better but quick and dirty */
    {
    if (strcmp(argv[i],"SOM")==0)
      {
      tStringToIgnore[nStringToCheck++]="EDC0811";
      tStringToIgnore[nStringToCheck++]="EDC0805";
      tStringToIgnore[nStringToCheck++]="somInitializeClass";
      }
    else
      tStringToIgnore[nStringToCheck++]=argv[i];
    }

  while (!feof(stdin))
    {
    unsigned char line[1000];
    if (fgets(line,sizeof(line),stdin))
      {
      if (CheckLine(line))
        {
        if (fileOut)
          fputs(line,fileOut);
        fputs(line,stdout);
        }

      /*
      ** check for the string "error"
      */
      if (!errFound && (strstr(line,"error EDC")!=NULL))
        errFound = 1 ;
      }
    }
  if (fileOut)
    fclose(fileOut);
  return(errFound ? -1 : 0);
}
