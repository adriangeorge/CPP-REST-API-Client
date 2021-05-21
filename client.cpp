// #include "ClientClass.hpp"

// // ./client server_ip server_port
// int main(int argc, char* argv[]) {

//     // Get Server IP 
    
//     // Get Server PORT


//     // Start while loop 

//         // Wait for input from keyboard
//         // If else construction determines function call


// }

#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

        
    // Ex 1.1: GET dummy from main server
    sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("34.118.48.238", "/api/v1/dummy", NULL, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    close_connection(sockfd);

    // Ex 1.2: POST dummy and print response from main server
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
    // Ex 2: Login into main server
    strcpy(body_data[0], "username=student");
    strcpy(body_data[1], "&password=student");
    
    sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request("34.118.48.238", "/api/v1/auth/login", "application/x-www-form-urlencoded", body_data, 2, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    close_connection(sockfd);
    // Ex 3: GET weather key from main server
    char* token = "connect.sid=s%3A2oddJ1xYfJWbf8vebyknzv5kv3re37am.GnmTwmpEot7%2FYJkhtwrnSqwHg14vXmo57ZIOj2F8kKg";
    sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("34.118.48.238", "/api/v1/weather/key", NULL, &token, 1);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    close_connection(sockfd);
    // {"key":"b912dd495585fbf756dc6d8f415a7649"}
    // Ex 4: GET weather data from OpenWeather API
    sockfd = open_connection("82.196.7.246", 80, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("82.196.7.246", "/data/2.5/weather?lat=44&lon=26&appid=b912dd495585fbf756dc6d8f415a7649", NULL, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    close_connection(sockfd);
    // Ex 5: POST weather data for verification to main server
    strcpy(body_data[0], basic_extract_json_response(response));
    // printf("[TEST] %s", basic_extract_json_response(response));
    sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request("34.118.48.238", "/api/v1/weather/44/26", "application/json", body_data, 1, &token, 1);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    close_connection(sockfd);
    // Ex 6: Logout from main server
    sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("34.118.48.238", "/api/v1/auth/logout", NULL, NULL, 0);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    close_connection(sockfd);
    // BONUS: make the main server return "Already logged in!"
    strcpy(body_data[0], "username=student");
    strcpy(body_data[1], "&password=student");
    
    sockfd = open_connection("34.118.48.238", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_post_request("34.118.48.238", "/api/v1/auth/login", "application/x-www-form-urlencoded", body_data, 2, &token, 1);
    puts(message);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    close_connection(sockfd);
    // free the allocated data at the end!
    for (int i = 0; i < 3; i++)
    {
        free(body_data[i]);
    }
    free(body_data);
    return 0;
}