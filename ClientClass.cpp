#include "ClientClass.hpp"

char *compute_post_request(string host, string url, 
                        string content_type, string data,
                        vector<string> cookies) {
    string request;

    
    // char *message = calloc(BUFLEN, sizeof(char));
    // char *line = calloc(LINELEN, sizeof(char));
    // char *body_data_buffer = calloc(LINELEN, sizeof(char));

    // // Step 1: write the method name, URL and protocol type
    // sprintf(line, "POST %s HTTP/1.1", url);
    // compute_message(message, line);
    
    // // Step 2: add the host
    // sprintf(line, "Host: %s", host);
    // compute_message(message,line);
    // /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
    //         in order to write Content-Length you must first compute the message size
    // */
    // sprintf(line, "Content-Type: %s", content_type);
    // compute_message(message,line);
    
    // int len = 0;
    // for (int i = 0; i < body_data_fields_count; i++)
    // {
    //     len += strlen(body_data[i]);
    //     strcat(body_data_buffer, body_data[i]);

    // }
    // sprintf(line, "Content-Length: %d", len);
    // compute_message(message,line);
    // // Step 4 (optional): add cookies
    // if (cookies != NULL) {
    //     for (int i = 0; i < cookies_count; i++) {
    //         sprintf(line, "Cookie: %s", cookies[i]);
    //         compute_message(message, line);
    //     } 
    // }
    // // Step 5: add new line at end of header
    // compute_message(message, "");
    // // Step 6: add the actual payload data
    // memset(line, 0, LINELEN);
    // compute_message(message, body_data_buffer);

    // free(line);
    // return message;
}


char *compute_get_request(char *host, char *url, char *query_params,
                          char **cookies, int cookies_count) {
    // nlohmann::json j;

    // char *message = (char*)calloc(BUFLEN, sizeof(char));
    // char *line = (char*)(LINELEN, sizeof(char));

    // // Step 1: write the method name, URL, request params (if any) and protocol type
    // if (query_params != NULL) {
    //     sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    // } else {
    //     sprintf(line, "GET %s HTTP/1.1", url);
    // }

    // compute_message(message, line);

    // // Step 2: add the host
    // sprintf(line, "Host: %s", host);
    // compute_message(message,line);
    // // Step 3 (optional): add headers and/or cookies, according to the protocol format
    // if (cookies != NULL) {
    //     for (int i = 0; i < cookies_count; i++) {
    //         sprintf(line, "Cookie: %s", cookies[i]);
    //         compute_message(message, line);
    //     } 
    // }
    // // Step 4: add final new line
    // compute_message(message, "");
    // return message;                          
}

char *compute_delete_request(char *host, char *url, char *query_params,
                             char **cookies, int cookies_count) {
    nlohmann::json j;
    

}

// Connect
int ClientClass::connectClient(char* ip, char* port) {

    int port_num;
    port_num = atoi(port);

    server_socket = open_connection(ip, port_num, AF_INET, SOCK_STREAM, 0);
}

// register
int ClientClass::registCli(string username, string password)
{
    char** body_data = malloc(3 * sizeof(char*));
    body_data[0] = malloc(LINELEN);
    body_data[1] = malloc(LINELEN);
    body_data[2] = malloc(LINELEN);
    strcpy(body_data[0], "name=test");
    strcpy(body_data[1], "&message=iansiazteca");
    
    sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request("34.118.48.238", "/api/v1/dummy", "application/x-www-form-urlencoded", body_data, 2, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    close_connection(sockfd);

}

// login
int ClientClass::authCli(string username, string password)
{
    nlohmann::json j;
}

// enter_library
int ClientClass::accessLib()
{
    nlohmann::json j;
}

// get_books
int ClientClass::bookList()
{
    nlohmann::json j;
}

// get_book
int ClientClass::bookInfo(int id)
{
    nlohmann::json j;
}

// add_book
int ClientClass::addBook(string title, string author, string genre, string publisher, int page_count)
{
    nlohmann::json j;
}

// delete_book
int ClientClass::delBook(int id)
{
    nlohmann::json j;
}

// logout
int ClientClass::logoutCli()
{
}

ClientClass::ClientClass(/* args */)
{
}

ClientClass::~ClientClass()
{
}
