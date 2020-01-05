#include "functions.h"


void append(char *str, char c)
{
  size_t len = strlen(str);
  str[len] = c;
  str[len+1] = '\0';
}

void appendy(char *str, char c)
{
  size_t len = strlen(str);
  str = realloc(str, (len+1)*sizeof(char));
  str[len] = c;
  str[len+1] = '\0';
}

void printMemo(char **A, int size)
{
  printf("path in arr = [");
  for(int i=0; i<size-1 ; i++)
    printf("%s, ", A[i]);

  printf("%s]\n", A[size-1]);

}

void freeMemo(char **arr, int size)
{
  for(int i=0 ; i < size ; i++)
    free(arr[i]);

  free(arr);
}

bool isPath(char * path)
{
  if(!isalpha(*path) && !isdigit(*path))
    return false;

  for(int i=1 ; i<strlen(path) ; i++)
  {
    if(!isalpha(path[i]) && !isdigit(path[i]))
    {
      if(path[i] == '.' && path[i-1] == '.')
        return false;
      else if(path[strlen(path)-1] == '.')
        return false;
      else if(path[i] == '.' && (isalpha(path[i-1]) || isdigit(path[i-1])))
        continue;
      else
        return false;
    }

  }
  return true;
}

int isIdentifier(char *id)
{
  for(int i=0 ; i<strlen(id) ; i++)
  {
    if(!isalpha(id[i]) && !isdigit(id[i]))
    {
      return -1;
    }
  }

  return 0;

}

int allocMemory(char *msg, char ***seprPath)
{
  int size=1;

  //find size of seprPath (amount of directorys + 1 file)
  for(int i=1 ; i<strlen(msg) ; i++)
    if(msg[i] == '.')
      size++;

  *seprPath = (char **)malloc(sizeof(char *) * size);
  return size;
}

int separatePath(char *msg, char **seprPath, int size)
{
  char path[MAX_STR_SIZE] = "";
  char id[MAX_STR_SIZE] = "";

  //allocate memory for seprPath cells
  for(int i=0 ; i<size ; i++)
    seprPath[i] = (char *)malloc(sizeof(char) * MAX_STR_SIZE);

  //gets path from msg
  for(int i=1 ; i<strlen(msg) ; i++)
  {
    if(msg[i] == ' ' || msg[i] == '\0')
      break;
    append(path, msg[i]);
  }

  //checks valid path
  if(isPath(path) == 0)
    return -1;

  //appending names all dir/file from path to seprPath array
  int j=0;
  append(path, '.');
  for(int i=0 ; i<strlen(path) ; i++)
  {
    if(path[i] == '.')
    {
      strcpy(seprPath[j++], id);
      strcpy(id, "");
      continue;
    }
    append(id, path[i]);
  }

  return size;
}

char *mkRealPath(char *msg, char* p)
{
  char path[MAX_STR_SIZE] = "";
  strcpy(path, p);
  append(path, '/');
  for(int i=1 ; i<strlen(msg) ; i++)
  {
    if(msg[i] == '.')
      strcat(path, "_/");
    else
      append(path, msg[i]);
  }

  //alloc memo for realpaths and save in it all paths
  char *realPaths = (char *)malloc(sizeof(char) * strlen(path)+1);
  strcpy(realPaths, path);

  return realPaths;

}

void findpaths(char **paths, char *p)
{
  int pfd[2], n;
  char str[BUFSIZE + 1];

  if (pipe(pfd) < 0)
    exit(-1);

  n = fork();

  if(n < 0)
    exit(-2);
  else if (n == 0)
  {
    close(pfd[0]);

    dup2(pfd[1], 1);
    close(pfd[1]);

    execlp("find", "find", p, "-type", "f", (char *) 0);
    exit(-1); /* To avoid problem if execlp fails, especially if in a loop. */
  }
  else
  {
    close(pfd[1]);

    while ((n = read(pfd[0], str, BUFSIZE)) > 0)
    {
      str[n] = '\0';
      strcat(*paths, str);
    }

    close(pfd[0]);
    wait(&n); /* To avoid the zombie process. */

  }

}

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int readInt(char *path)
{
  char id[MAX_STR_SIZE] = "";

  //open file for readen.
  FILE *fp = fopen(path, "r");
  if(fp == NULL) // if path does not exists or isn't file
    return 0;

  //read id from file
  fgets(id, MAX_STR_SIZE, fp);

  //if id isn't a number, then return 0.
  for(int i=0 ; i<strlen(id)-1 ; i++)
    if(!isdigit(id[i]))
      return 0;

  //close file
  fclose(fp);

  return atoi(id);
}

int whichCommand(char *msg)
{
  switch (msg[0])
  {
    case '.':
      for(int i=0;i<strlen(msg);i++)
        if(msg[i] == ' ')
          return 1;
      return 2;
    case '+':
      return 3;
    case '*':
      return 4;
    case '-':
      return 5;
    default:
      return -1;
  }
}
