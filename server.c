
#include "functions.h"


int main(int argc , char *argv[])
{
  if(argc == 2 && strcmp(argv[1], "--help") == 0)
  {
    system("cat ./README");
    exit(0);
  }
  else if(argc != 4)
  {
    printf("ERROR:: invalid amount of parameters!!!\n");
    exit(1);
  }

  int PORT = atoi(argv[2]);
  int opt = TRUE;
  int master_socket , addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd;
  int max_sd;
  struct sockaddr_in address;
  char buffer[1025];  //data buffer of 1K


  //set of socket descriptors
  fd_set readfds;

  //a message
  char *message = "TCP Server - Linux hw2 \r\n";

  //initialise all client_socket[] to 0 so not checked
  for (i = 0; i < max_clients; i++)
  {
    client_socket[i] = 0;
  }

  //create a master socket
  if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  //set master socket to allow multiple connections , this is just a good habit, it will work without this
  if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  //type of socket created
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );

  //bind the socket to localhost port
  if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  printf("Listener on port %d \n", PORT);

  //try to specify maximum of 3 pending connections for the master socket
  if (listen(master_socket, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  //accept the incoming connection
  addrlen = sizeof(address);
  puts("Waiting for connections ...");

  while(TRUE)
  {
    //////////////
    //-------------
    char *p = (char *)malloc(sizeof(char) * MAX_STR_SIZE);
    // char *p = ".";  //save location
    strcpy(p,argv[3]);

    //declare variables
    // char **seprPath = NULL;
    int sum, c;
    char toString[MAX_STR_SIZE] = "";
    char *id = "";

    //-------------
    //////////////

    //clear the socket set
    FD_ZERO(&readfds);

    //add master socket to set
    FD_SET(master_socket, &readfds);
    max_sd = master_socket;

    //add child sockets to set
    for ( i = 0 ; i < max_clients ; i++)
    {
      //socket descriptor
      sd = client_socket[i];

      //if valid socket descriptor then add to read list
      if(sd > 0)
        FD_SET( sd , &readfds);

      //highest file descriptor number, need it for the select function
      if(sd > max_sd)
        max_sd = sd;
    }

    //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
    activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

    if ((activity < 0) && (errno!=EINTR))
    {
      printf("select error");
    }

    //If something happened on the master socket , then its an incoming connection
    if (FD_ISSET(master_socket, &readfds))
    {
      if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
      {
          perror("accept");
          exit(EXIT_FAILURE);
      }

      //inform user of socket number - used in send and receive commands
      printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

      //send new connection greeting message
      if( send(new_socket, message, strlen(message), 0) != strlen(message) )
      {
          perror("send");
      }

      puts("Welcome message sent successfully");

      //add new socket to array of sockets
      for (i = 0; i < max_clients; i++)
      {
          //if position is empty
        if( client_socket[i] == 0 )
        {
          client_socket[i] = new_socket;
          printf("Adding to list of sockets as %d\n" , i);

          break;
        }
      }
    }

    //else its some IO operation on some other socket :)
    for (i = 0; i < max_clients; i++)
    {

      sd = client_socket[i];

      if (FD_ISSET( sd , &readfds))
      {
        //Check if it was for closing , and also read the incoming message
        if ((valread = read( sd , buffer, 1024)) == 0)
        {
          //Somebody disconnected , get his details and print
          getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
          printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

          //Close the socket and mark as 0 in list for reuse
          close( sd );
          client_socket[i] = 0;
        }

        ///////////////////////////////////////////////////////////////////////------
        ///////////////////////////////////////////////////////////////////////------

        // Echo back the message that came in
        else
        {
          //set the string terminating NULL byte on the end of the data read
        buffer[valread-1] = '\0';

        ///////////////
        ///////////////

        //find the number of command
        c = whichCommand(buffer);

        char dss[MAX_STR_SIZE]; //data save system
        strcpy(dss,argv[1]);

        char ptxt[MAX_STR_SIZE] = "";
        strcpy(ptxt, p);
        strcat(ptxt, "/saveSys.txt");
        char inFile = isFoundInFile(buffer, ptxt); //checks path in the text file {return 'T' or 'F'}

        //find id from first command
        char idtxt[MAX_STR_SIZE] = "";
        int f = 0;
        for(int i=0 ; i < strlen(buffer) ; i++)
        {
          if(f == 1)
          {
            append(idtxt, buffer[i]);
            continue;
          }

          if(buffer[i] == ' ')
            f = 1;
        }


        switch (c)
        {
          case 1:   //for first command.
            // send(sd, "In 1st command\n", strlen("In 1st command\n"), 0);
            if(strcmp(dss, "tree") == 0)
            {
              if(firstResponse(buffer, p) == -1)
              send(sd, "ERROR: path or identifier.\n" , 27, 0);
            }
            else if(strcmp(dss, "txt") == 0)
            {
              // if path undeclared in file txt then -> append to txt file
              if(inFile == 'F')
              {
                // appendToFile("", "", ptxt);  //append to txt file
                // removeLinefromFile(buffer, findLine(buffer, ptxt)-1, ptxt);
                appendToFile(buffer, idtxt, ptxt);  //append to txt file
              }

              else
              {
                char *oldid = getId(buffer, ptxt);
                int line = findLine(buffer, ptxt);

                removeLinefromFile(buffer, line, ptxt);
                appendToFile(buffer, idtxt, ptxt);  //append to txt file
                free(oldid);
              }

            }

            break;

          case 2:
            //for second command.
            // send(sd, "In 2nd command\n", strlen("In 2nd command\n"), 0);
            if(strcmp(dss, "tree") == 0)
              id = secondResponse(buffer, p);

            else if(strcmp(dss, "txt") == 0)
              id = getId(buffer, ptxt);

            //checks invalidation
            if(strcmp(id, "-1") == 0)
            {
              send(sd, "ERROR: path.\n" , 13, 0);
              break;
            }
            // for priniting screen client
            if(buffer[strlen(buffer)-1] != '\n')
              append(id, '\n');

            send(sd, id, strlen(id), 0);
            free(id);

            break;

          case 3:
            //for third command.
            // send(sd, "In 3rd command\n", strlen("In 3rd command\n"), 0);
            if(strcmp(dss, "tree") == 0)
              thirdResponse(buffer, p);
            else if(strcmp(dss, "txt") == 0)
            {
              id = getId(buffer, ptxt);
              if(strcmp(id, "-1") == 0)
              {
                send(sd, "ERROR: path.\n" , 13, 0);
                break;
              }
              else
              {
                int flg = 0;
                for(int i=0 ; i < strlen(id) ; i++)
                {
                  if(!isdigit(id[i]))
                  {
                    flg = 1;
                    break;
                  }
                }

                if(!flg)
                {
                  char *oldid = getId(buffer, ptxt);
                  int line = findLine(buffer, ptxt);

                  removeLinefromFile(buffer, line, ptxt);

                  int num = atoi(oldid) + 1;
                  char strNumber[MAX_STR_SIZE] = "";
                  sprintf(strNumber, "%d", num); //convert int to char*
                  appendToFile(buffer, strNumber, ptxt);  //append to txt file


                  free(oldid);
                }
              }
            }

            break;

          case 4:
            // for fourth command
            // send(sd, "In 4th command\n", strlen("In 4th command\n"), 0);
            if(strcmp(dss, "tree") == 0)
            {
              sum = fourthResponse(buffer, p);

              sprintf(toString, "%d", sum); //convert int to char*

              append(toString, '\n'); // for priniting screen client
              send(sd, toString , strlen(toString), 0);
            }
            else if(strcmp(dss, "txt") == 0)
            {
              int sumint = sumIntegers(buffer, ptxt);
              char sumint_str[MAX_STR_SIZE] = "";
              sprintf(sumint_str, "%d\n", sumint); //convert int to char*
              send(sd, sumint_str, strlen(sumint_str), 0);
            }

            break;

          case 5:
            // for fifth command
            // send(sd, "In 5th command\n", strlen("In 5th command\n"), 0);
            if(strcmp(dss, "tree") == 0)
            {
              id = fifthResponse(buffer, p);
              if(strcmp(id, "-1") == 0)
              {
                send(sd, "ERROR: path.\n" , 13, 0);
                break;
              }
            }
            else if(strcmp(dss, "txt") == 0)
            {
              char tmp[MAX_STR_SIZE] = "";
              id = getId(buffer, ptxt);
              if(strcmp(id, "-1") == 0)
              {
                send(sd, "ERROR: path.\n" , 13, 0);
                break;
              }

              strcpy(tmp, buffer);
              append(tmp, '.');
              strcat(tmp, id);

              strcpy(id, ""); //init id
              id = getId(tmp, ptxt);
              if(strcmp(id, "-1") == 0)
              {
                send(sd, "ERROR: path.\n" , 13, 0);
                break;
              }
            }

            append(id, '\n'); // for priniting screen client
            send(sd, id , strlen(id), 0);
            free(id);

            break;

          default:
            send(sd, "ERROR: Invalid command.\n" , 24, 0);
            break;
        }
        free(p);


        ///////////////
        ///////////////



        }//end else
      }
    }
  } //end while

  return 0;
}
