#include "functions.h"


int firstResponse(char *msg, char* p)
{
  char id[MAX_STR_SIZE] = "";

  //put in order names of directorys & file in array
  char **seprPath = NULL;
  int size = allocMemory(msg, &seprPath);
  if(separatePath(msg, seprPath, size) == -1) // init path
  {
    freeMemo(seprPath, size);
    return -1;
  }

  //checks command start with a dot.
  if(*msg != '.')
  {
    freeMemo(seprPath, size);
    return -1;
  }

  //gets id
  int count = 0;
  for(int i=0 ; i<strlen(msg) ; i++)
  {
    if(msg[i] == ' ')
    {
      count++;
      continue;
    }

    if(count > 0)
      append(id, msg[i]);
  }

  // checks identifier is legal
  if(isIdentifier(id) == -1)
  {
    freeMemo(seprPath, size); ///////////////////////NEWWW
    return -1;
  }

  //make directorys + file
  append(p, '/');
  for(int i=0 ; i<size ; i++)
  {
    strcat(p, seprPath[i]);

    //make a dir
    if(i!=size-1)
    {
      append(p, '_'); //underline for mark folder.

      struct stat st = {0};
      if(stat(p, &st) == -1)  //make dir (if not exists)
        mkdir(p, 0700);

      append(p, '/');
    }
    else //make a file
    {
      FILE *fp = fopen(p, "w+");
      if(fp == NULL)
      {
        freeMemo(seprPath, size);
        return -1;
      }
      fputs(id, fp); //puts identifier in path
      fclose(fp);
    }
  }//end for

  freeMemo(seprPath, size);
  return 0;
}

char *secondResponse(char * msg, char *p)
{
  //allocating memory for return variable
  char *id = (char *)malloc(sizeof(char) * MAX_STR_SIZE);

  //make real path
  char* realPath = mkRealPath(msg, p);

  //open file for readen
  FILE *fp = fopen(realPath, "r");
  if(fp == NULL) // if path does not exists
  {
    free(realPath);
    free(id);
    return "-1";
  }

  //read id from file
  fgets(id, MAX_STR_SIZE, fp);

  //close file
  fclose(fp);

  //free alloc
  free(realPath);

  return id;
}

void thirdResponse(char *msg, char *p)
{
  char *txt = secondResponse(msg, p);  //the secondResponse return text from file
  int num;
  char strNum[MAX_STR_SIZE] = "";

  //checks if id composed of digits
  for(int i=0 ; i<strlen(txt)-1 ; i++)
  {
    if(!isdigit(txt[i]))
      return;
  }

  num = atoi(txt); //convert char* to int
  num++; //update num by 1
  sprintf(strNum, "%d", num); //convert int to char*

  char* realPath = mkRealPath(msg, p);   //make real path

  //write the num to file
  FILE *fp = fopen(realPath, "w+");
  fputs(strNum, fp); //puts identifier in path

  fclose(fp);
  free(realPath);
  free(txt);
}

int fourthResponse(char *msg, char *p)
{
  //alloc memo for all paths
  char *allPaths = (char *)malloc(sizeof(char) * (BUFSIZE+1));

  int sum = 0; //init sum
  char *realPath = "";

  //get real begining path (risha)
  append(msg, '.');

  realPath = (char *)mkRealPath(msg, p);

  //get all paths
  findpaths(&allPaths, realPath);

  //read any file in path, if file contain number then append to sum variable.
  char path[MAX_STR_SIZE] = "";
  for(int i=2 ; i < strlen(allPaths) ; i++)
  {
    if(allPaths[i] == '\n')
    {
      sum += readInt(path);
      strcpy(path, "");
      continue;
    }
    else
      append(path, allPaths[i]);
  }//endfor

  //free memory
  free(realPath);
  free(allPaths);

  return sum;
}

char *fifthResponse(char *msg, char *p)
{
  //find id from path
  char *id = secondResponse(msg, p);

  //check id isnt "-1" (secondResponse failed return )
  if(strcmp(id, "-1") == 0)
    return "-1";

  //find amount of folders to the name of file.
  int c=0;
  for(int i=1 ; i<strlen(msg) ; i++)
    if(msg[i] == '.')
      c++;

  //ignore newline at the end of the string
  for(int i=0 ; i<strlen(id) ; i++)
    if(id[i] == '\n')
    {
      id[i] = '\0';
      break;
    }

  //find new path
  char newPath[MAX_STR_SIZE] = "";
  strcat(newPath, msg);
  append(newPath, '.');
  strcat(newPath, id);

  free(id);

  //find id from new path.
  char *newid = secondResponse(newPath, p);

  //check id isnt "-1" (secondResponse failed return )
  if(strcmp(newid, "-1") == 0)
    return "-1";

  //ignore newline at the end of the string
  for(int i=0 ; i<strlen(newid) ; i++)
    if(newid[i] == '\n')
    {
      newid[i] = '\0';
      break;
    }

  return newid;
}
