#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>

#define MAXMSG 1024
#define BUFMSG 2048

#define SERVER_INFO "Server: jodevilhttpd/0.1.0 (macOS 10.12.3)\r\n"

//#define MULTIPLE 1.5

void error_prefix(const char *);
int make_socket(u_short *);
void accept_request(int);
void not_found(int);

void error_prefix(const char *err_msg)
{
    perror(err_msg);
    exit(1);
}

int make_socket(u_short *port)
{
    int httpd = 0;
    struct sockaddr_in name;

    httpd = socket(PF_INET, SOCK_STREAM, 0);
    if (httpd == -1)
        error_prefix("socket");
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
        error_prefix("bind");

    if (listen(httpd, 5) < 0)
        error_prefix("listen");

    return(httpd);
}

void accept_request(int client)
{
    char *buffer;
    int nbytes = 0;
    char *command;

    char *head;
    char *tail;
    char *tmp;

    char* response_buf;

    buffer = malloc(sizeof(char) * MAXMSG);
    command = malloc(sizeof(char) * BUFMSG);
    response_buf = malloc(sizeof(char) * BUFMSG);

    perror(".....");
    nbytes = read(client, buffer, MAXMSG);
    if (nbytes < 0)
    {
        /* Read error. */
        error_prefix("read");
        exit (EXIT_FAILURE);
    }
    else if (nbytes == 0)
        /* End-of-file. */
        return;
    else
    {
        /* Data read. */
//        fprintf (stderr, "Server: got message: \n‘%s’", buffer);

//        printf("buffer[0] = %s", buffer);
//        printf("%d", sizeof(buffer));
        perror("wtf!!");
        if (strncmp("GET /exec/", buffer, 10) == 0)
        {
/*            perror("lalala");
            strcpy(buf, "HTTP/1.1 200 OK\r\n");
            send(client, buf, strlen(buf), 0);
            */
            printf("%s", buffer+9);
            head = buffer + 10;
            tail = head;
/*
            if (strncmp("/", head + 1, 1) == 0)
            {
                error_prefix("tired");
            }
*/            
//            error_prefix("cry");

            int i = 0;
            while (strncmp(" ", tail, 1) != 0)
            {
                tail++;
                i++;
            }

            strncpy(command, head, i);

  //          fprintf(stderr, "%s\n", command);
  //          fprintf(stderr, "%d\n", strlen(command));
            
            command[strlen(command)] = '\0';

  //          fprintf(stderr, "%s\n", command);
  //          fprintf(stderr, "%d\n", strlen(command));            

            i = 0;
            int j  = 0;
            char xx = command[3];
  //          fprintf(stderr, "%s\n", &xx);

            for (i = 0; command[i] != '\0'; i++)
            {
                if (command[i] == '%')
                {
                    if (command[i+1] == '2')
                    {
                        if (command[i+2] == '0')
                        {
   //                         fprintf(stderr, "%d\n", i);
                            command[i] = ' ';
                            for (j = i+1; j < strlen(command); j++)
                            {
                                command[j] = command[j+2];
                            }
//                            command[strlen(command)]='\0';
 //                           fprintf(stderr, "command = %s\n", command);
                        }
                    }
                }
            }

 strcpy(response_buf, "HTTP/1.1 200 OK\r\n");
 send(client, response_buf, strlen(response_buf), 0);
 strcpy(response_buf, SERVER_INFO);
 send(client, response_buf, strlen(response_buf), 0);
 sprintf(response_buf, "Transfer-encoding: chunked\r\n");
 send(client, response_buf, strlen(response_buf), 0);
 sprintf(response_buf, "Content-Type: text/html\r\n");
 send(client, response_buf, strlen(response_buf), 0);
 sprintf(response_buf, "Connection: keep-alive\r\n");
 send(client, response_buf, strlen(response_buf), 0); 

 strcpy(response_buf, "\r\n");
 send(client, response_buf, strlen(response_buf), 0);
 
    char* cmd_output;
    char* temp;
    int max_opt_len = 5;

    cmd_output = malloc(sizeof(char) * 5);

    FILE *f = popen(command, "r");
    while (fgets(cmd_output, 5, f) != NULL) {
//        printf("%s", cmd_output);
//        fprintf(stderr, "output = %s\n", cmd_output);

//sprintf(response_buf, "%x", 4);
sprintf(response_buf, "%x", strlen(cmd_output));
strcat(response_buf, "\r\n");
  send(client, response_buf, strlen(response_buf), 0);
//  strcpy(response_buf, "\r\n");
//  send(client, response_buf, strlen(response_buf), 0);  
  sprintf(response_buf, "%s", cmd_output);
//  response_buf[strlen(response_buf)] = '\r';
strcat(response_buf, "\r\n");

  send(client, response_buf, strlen(response_buf), 0);
 // strcpy(response_buf, "\r\n");
 // send(client, response_buf, strlen(response_buf), 0);


    }

    pclose(f);

sprintf(response_buf, "%x", 0);
strcat(response_buf, "\r\n");  
  send(client, response_buf, strlen(response_buf), 0);
  strcpy(response_buf, "\r\n");  
  send(client, response_buf, strlen(response_buf), 0);

 //   strcpy(response_buf, "\r\n");
 //   send(client, response_buf, strlen(response_buf), 0);
/*
 strcpy(response_buf, "HTTP/1.1 200 OK\r\n");
 send(client, response_buf, strlen(response_buf), 0);
 strcpy(response_buf, SERVER_INFO);
 send(client, response_buf, strlen(response_buf), 0);
 sprintf(response_buf, "CTransfer-encoding: chunked\r\n", 0);
 send(client, response_buf, strlen(response_buf), 0);
 sprintf(response_buf, "Content-Type: text/html\r\n");
 send(client, response_buf, strlen(response_buf), 0);

 strcpy(response_buf, "\r\n");
 send(client, response_buf, strlen(response_buf), 0);
*/
//    send(client, NULL, 0, 0);




//            system(command);

//error_prefix("...");
            
        }
        else
        {
            not_found(client);
        }
        return;
    }
    return;
}

void not_found(int client)
{
    char *response_buf = malloc(sizeof(char) * BUFMSG);

 strcpy(response_buf, "HTTP/1.1 404 NOT FOUND\r\n");
 send(client, response_buf, strlen(response_buf), 0);
 strcpy(response_buf, SERVER_INFO);
 send(client, response_buf, strlen(response_buf), 0);
 sprintf(response_buf, "Content-Type: text/html\r\n");
 send(client, response_buf, strlen(response_buf), 0);
 sprintf(response_buf, "Connection: close\r\n");
 send(client, response_buf, strlen(response_buf), 0); 

  strcpy(response_buf, "\r\n");  
  send(client, response_buf, strlen(response_buf), 0);


//exit(1);
}


int main(int argc, char *argv[])
{
    int server_socket = -1;
    u_short port = -1;
    int client_socket = -1;
    struct sockaddr_in client_name;
    int client_name_length = sizeof(client_name);
    pthread_t newthread;

    if (argc != 2)
    {
        printf("Usage: ./normal_web_server <port>\n\n");
        exit(1);
    }

    printf("%s, %s\n", argv[0], argv[1]);
    port = atoi(argv[1]);

    printf("%d\n", port);

    server_socket = make_socket(&port);
    printf("httpd running on port %d\n", port);

    while (1)
    {
        client_socket = accept(server_socket,
                              (struct sockaddr *)&client_name,
                              &client_name_length);

        if (client_socket == -1)
            error_prefix("accept");

        /* accept_request(client_socket); */
        if (pthread_create(&newthread , NULL, accept_request, client_socket) != 0)
            perror("pthread_create");
    }

 close(server_socket);

 return(0);
}
