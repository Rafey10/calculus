#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "b64decode.h"
#include "pofix-stack.h"
#include "pofix-calc.h"
#include "infix-calc.h"


int main(int argc, char** argv, char** env) {

	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len = sizeof(client_addr);
	int fd_server, fd_client; 
	char buf[2048];
	int on = 1;
	
	fd_server = socket (AF_INET, SOCK_STREAM, 0);
	
	if (fd_server < 0){
		perror("socket error");
		exit(1);
	}
	
	setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons (8080);
	
	if (bind(fd_server, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
		perror("bind error");
		close(fd_server);
		exit(1);
	}
	
	if (listen(fd_server, 10) == -1){
		perror("listen error");
		close(fd_server);
		exit(1);
	}
	
	while (1){		/*Main Event Loop*/

		fd_client = accept (fd_server, (struct sockaddr *) &client_addr, &sin_len);
		
		if (fd_client == -1){
			perror("accept failed...\n");
			continue;
		}
		
		printf ("Got a Client Connection....\n");
		
		if (!fork()){		/*Forks for every connection established*/

			close(fd_server);
			memset(buf, 0, 2048);
			read(fd_client, buf, 2047);

			if (!strncmp(buf, "GET /calculus?query=", 20)){
				printf ("GET /calculus?query= is indeed detected\n");
			} else {
				char response [] = "HTTP/1.0 404 Not Found\r\n";
				write(fd_client, response, strlen(response));
				close(fd_client);
				printf("closing...\n");
				exit(0);
			}

			char* piece = strtok(buf, "=");
			piece = strtok(NULL, " ");

			/* at this point, "piece" contains the query string */
			printf("query string: %s\n", piece);

			char* dec;
  			size_t dec_len;

			dec_len = b64_sizeof_decode(piece) + 1;
  			dec = malloc (dec_len);

			if (base64_decode(piece, (unsigned char*) dec, dec_len) == 0){
				printf("decode failure...\n");
				char r1 [] = 
							"HTTP/1.1 200 OK\r\n"
							"Content-Type: application/json; charset=UTF-8\r\n\r\n" 
							"{ \"error\": true, \"message\": \"";
				char *m = 	
							"invalid b64";
				char r2[] = 
							"\" }\r\n";

				char response[256];

				strcat(response, r1);
				strcat(response, m);
				strcat(response, r2);

				write (fd_client, response, strlen(response));	
				close(fd_client);
				printf("closing...\n");
				exit (0);
  			}
			
			//at this point, dec contains the decoded query string
			printf ("dec:   '%s'\n", dec);

			/*Short code to parse the query string;
				Every token is whitespace separated */
			size_t i = 0, j = 0;

			char parsed[3*strlen(dec)];

    		for (i = 0, j = 0; i < strlen(dec); i++, j++){
        		if (dec[i] == '*' || dec[i] == '+' || dec[i] == '-' || 
            		dec[i] == '/' || dec[i] == '%' || dec[i] == '(' ||
            		dec[i] == ')'){
						parsed[j++] = ' ';
						parsed[j++] = dec[i];
						parsed[j] = ' '; 
            	} else if (dec[i] == ' '){
                    	parsed[j] = ' ';
            	} else {
                		parsed[j] = dec[i];
            	}
    		}
    
    		printf("parsed: '%s'\n", parsed);
			/*END code to parse query string*/

			int rc;
			char* result;

			rc = infix_eval_expr(parsed, &result);
			
			if (rc != 0){ 		/* if the calculation failed */

				char r1[] = 
						"HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n" 
						"{ \"error\": true, \"message\": \"";
				char* msg = 
						infix_strerror(rc);
				char r2[] = 
						"\" }\r\n";

				char response[256];

				strcat(response, r1);
				strcat(response, msg);
				strcat(response, r2);

				write (fd_client, response, strlen(response));	
				close(fd_client);
				printf("closing...\n");

			} else {		/*If a result was calculated*/

				printf("Result: '%s'\n", result);
				char r1[] = 
						"HTTP/1.1 200 OK\r\n"
						"Content-Type: application/json; charset=UTF-8\r\n\r\n" 
						"{ \"error\": false, \"result\": ";
				char r2[] = 
						" }\r\n";

				char response [256];

				strcat(response, r1);
				strcat(response, result);
				strcat(response, r2);

				write (fd_client, response, strlen(response));	
				close(fd_client);
				printf("closing...\n");
			}
		
			if (result) free (result);
			
			close(fd_client);
			printf("closing...\n");
			exit(0);
		}
		close(fd_client);	
	}
	return 0;
}
