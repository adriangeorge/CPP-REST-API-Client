#ifndef __CLIENT__
#define __CLIENT__

#include "json.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <stdio.h>      /* printf, sprintf */
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h> /* exit, atoi, malloc, free */
#include <string.h> /* memcpy, memset */
#include <string>
#include <sys/socket.h> /* socket, connect */
#include <unistd.h>     /* read, write, close */
#include <vector>
#include <sstream>
using namespace std;

#define LINELEN 1000

// Error checking function, copied from the laboratory
#define DIE(assertion, call_description)  \
    do                                    \
    {                                     \
        if (assertion)                    \
        {                                 \
            fprintf(stderr, "(%s, %d): ", \
                    __FILE__, __LINE__);  \
            perror(call_description);     \
            exit(EXIT_FAILURE);           \
        }                                 \
    } while (0)

class ClientClass
{
public:
    string loginToken;
    string libraryTkn;

    char buffer[BUFSIZ];

    string returnMessage;

    string ip;
    string port;
    int server_socket;
    
    vector<string> command;

    string post(string host, string url,
                                string content_type, nlohmann::json data,
                                vector<nlohmann::json> cookies, string auth);

    string get(string host, string url,
                               string query_params,
                               vector<nlohmann::json> cookies, string auth);

    string del(string host, string url,
                                  string query_params,
                                  vector<nlohmann::json> cookies, string auth);

    // Get a new command
    void getCommand();
    // Establish connection to server
    int connectClient(string ip, string port);
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