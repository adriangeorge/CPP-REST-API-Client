#ifndef __CLIENT__
#define __CLIENT__

#include <string>
#include <stdlib.h>
#include "json.hpp"
#include "laboratoryFuncs.cpp"

#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>

using namespace std;

#define BUFLEN 4096
#define LINELEN 1000

class ClientClass
{
private:
    string loginToken;
    string libraryTkn;
    
    string buffer;

    string returnMessage;

    int server_socket;

    char *compute_post_request(char *host, char *url, char* content_type, char **body_data, int body_data_fields_count, char **cookies, int cookies_count);
    char *compute_get_request(char *host, char *url, char *query_params, char **cookies, int cookies_count);
    char *compute_delete_request(char *host, char *url, char *query_params, char **cookies, int cookies_count);
public:

    // Establish connection to server
    int connectClient(char* ip, char* port);
    // Functions to interact with the server
    // register
    int registCli(string username, string password);
    // login
    int authCli(string username, string password);
    // enter_library
    int accessLib();
    // get_books
    int bookList();
    // get_book
    int bookInfo(int id);
    // add_book
    int addBook(string title, string author, string genre, string publisher, int page_count);
    // delete_book
    int delBook(int id);
    // logout
    int logoutCli();
    
    ClientClass();
    ~ClientClass();
};


#endif