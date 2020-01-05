#include "functions.h"

char isFoundInFile(char *msg, char *p)
{
  //get path from msg (like msg var without id & index 0)
  char path[MAX_STR_SIZE] = "";
  for(int i=1 ; msg[i]!=' ' && i<strlen(msg) ; i++)
    append(path, msg[i]);

  FILE * fp = fopen(p, "a+");
  if(fp == NULL)
  {
    printf("Failed to open file !!\n");
    exit(1);
  }

  char word[MAX_STR_SIZE] = "";
  while(!feof(fp))
  {
    char c = fgetc(fp);
    if(c == EOF) break;
    if(c == '\n')
    {
      strcpy(word, "");
      continue;
    }
    if(c == ' ')
    {
      if(strcmp(word, path)==0)
      {
        fclose(fp);
        return 'T';
      }
      strcpy(word, "");
      continue;
    }
    append(word, c);
  }

  fclose(fp);
  return 'F';
}

void appendToFile(char *msg, char *id, char *p)
{
  //get path from msg (like msg var without id & index 0)
  char path[MAX_STR_SIZE] = "";
  for(int i=1 ; msg[i]!=' ' && i<strlen(msg) ; i++)
    append(path, msg[i]);

  FILE * fp = fopen(p, "a");
  if(fp == NULL)
  {
    printf("Failed to open file !!\n");
    exit(1);
  }

  if(isIdentifier(id) != -1)
    fprintf(fp, "%s %s\n", path, id);

  fclose(fp);
}

void removeLinefromFile(char *msg, int line, char* p)
{
    int lno, ctr = 0;
  char ch;
  FILE *fptr1, *fptr2;
  char fname[MAX_STR_SIZE];
  char str[MAX_STR_SIZE], temp[] = "temp.txt";

  strcpy(fname, p);
  fptr1 = fopen(fname, "r");
  if (!fptr1)
  {
          printf(" File not found or unable to open the input file!!\n");
          return;
  }
  fptr2 = fopen(temp, "w"); // open the temporary file in write mode
  if (!fptr2)
  {
          printf("Unable to open a temporary file to write!!\n");
          fclose(fptr1);
          return;
  }
  lno = line;

  // copy all contents to the temporary file except the specific line
  while (!feof(fptr1))
  {
      strcpy(str, "\0");
      fgets(str, MAX_STR_SIZE, fptr1);
      if (!feof(fptr1))
      {
          ctr++;
          /* skip the line at given line number */
          if (ctr != lno)
          {
              fprintf(fptr2, "%s", str);
          }
      }
  }
  fclose(fptr1);
  fclose(fptr2);
  remove(fname);  		// remove the original file
  rename(temp, fname); 	// rename the temporary file to original name
  /*------ Read the file ----------------*/
  fptr1=fopen(fname,"r");
  ch=fgetc(fptr1);
  while(ch!=EOF)
  {
       ch=fgetc(fptr1);
  }
  fclose(fptr1);
  /*------- End of reading ---------------*/

}

int findLine(char *msg, char *p)
{
  int line = 1;

  //get path from msg (like msg var without id & index 0)
  char path[MAX_STR_SIZE] = "";
  for(int i=1 ; msg[i]!=' ' && i<strlen(msg) ; i++)
    append(path, msg[i]);

  FILE * fp = fopen(p, "a+");
  if(fp == NULL)
  {
    printf("Failed to open file !!\n");
    exit(1);
  }

  char word[MAX_STR_SIZE] = "";
  while(!feof(fp))
  {
    char c = fgetc(fp);
    if(c == EOF) break;
    if(c == '\n')
    {
      line++;
      strcpy(word, "");
      continue;
    }
    if(c == ' ')
    {
      if(strcmp(word, path)==0)
      {
        fclose(fp);
        return line;
      }
      strcpy(word, "");
      continue;
    }
    append(word, c);
  }

  fclose(fp);
  return line;
}

char *getId(char *msg, char *p)
{
  int flag = 0;
  //allocating memory for return variable
  char *id = (char *)malloc(sizeof(char) * MAX_STR_SIZE);
  strcpy(id, "");

  //get path from msg (like msg var without id & index 0)
  char path[MAX_STR_SIZE] = "";
  for(int i=1 ; msg[i]!=' ' && i<strlen(msg) ; i++)
    append(path, msg[i]);

  //open file for readen
  FILE * fp = fopen(p, "a+");
  if(fp == NULL)
  {
    printf("Failed to open file !!\n");
    exit(1);
  }

  char word[MAX_STR_SIZE] = "";
  while(!feof(fp))
  {
    char c = fgetc(fp);
    if(c == EOF) break;
    if(flag == 1 && (c != '\n' && c != EOF))
    {
      append(id, c);
      continue;
    }
    if(flag == 1 && (c=='\n' || c==EOF))
      break;

    if(c == ' ')
    {
      if(strcmp(word, path) == 0) // update flag if found our path.
      {
        flag = 1;
        continue;
      }

      while(!feof(fp)) //jump to the next path
      {
        c = fgetc(fp);
        if(c != '\n' && c != EOF)
          continue;
        else
          break;

      }
      strcpy(word, "");
      continue;
    }
    append(word, c);
  }

  fclose(fp);

  if(flag == 1)
    return id;
  else
  {
    free(id);
    return "-1";
  }
}

int sumIntegers(char *msg, char *p)
{
  char fname[MAX_STR_SIZE] = "";
  int sum = 0;

  //find fname
  for(int i=1 ; i<strlen(msg) ; i++)
  {
    if(msg[i] == '.' || msg[i] == ' ')
      break;
    append(fname, msg[i]);
  }

  //open file for readen
  FILE * fp = fopen(p, "r");
  if(fp == NULL)
  {
    printf("Failed to open file !!\n");
    exit(1);
  }

  char word[MAX_STR_SIZE] = "";
  char id[MAX_STR_SIZE] = "";
  while(!feof(fp))
  {
    char c = fgetc(fp);
    if(c == EOF) break;

    if(c == ' ') // if its just file -->> go to next line
    {
      while(c != EOF && c != '\n')
      {
        c = fgetc(fp);
        if(c == EOF || c == '\n')
          break;
      }
      strcpy(word, "");
      continue;

    }
    else if(c == '.') // find start of path
    {
      if(strcmp(word, fname) == 0)
      {
        while(!feof(fp)) //find id
        {
          c = fgetc(fp);
          if(c != ' ')
            continue;
          else
          {
            c = fgetc(fp);
            while(c != EOF && c != '\n')
            {
              append(id,c);
              c = fgetc(fp);
            }
            break;
          }
        }
        printf("id:>%s<\n", id);

        int fInt=0;
        for(int i=0 ; i<strlen(id) ; i++) //checks id is integer
        {
          if(!isdigit(id[i]))
          {
            fInt = 1;
            break;
          }
        }
        if(!fInt)
          sum += atoi(id); // update sum var
      }

      strcpy(id, "");
      strcpy(word, "");
      continue;
    }

    append(word, c);
  }

  fclose(fp);
  return sum;
}

int countlines(FILE * fin)
{
  int nlines = 0;
  char line[MAX_STR_SIZE];

  while (fgets(line, MAX_STR_SIZE, fin) != NULL) {
    nlines++;
  }

  return nlines;
}
