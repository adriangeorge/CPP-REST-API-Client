#include "ClientClass.hpp"
#include "json.hpp"

#define BUFLEN 8192
#define LINELEN 1000

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

using namespace std;
void error(const char *msg)
{
    perror(msg);
    exit(0);
}


// OPEN CONNECTION taken from laboratory
int open_connection(char *host_ip,
                    int portno,
                    int ip_type,
                    int socket_type,
                    int flag)
{
    struct sockaddr_in serv_addr;
    int sockfd = socket(ip_type, socket_type, flag);
    if (sockfd < 0)
        error("ERROR opening socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip, &serv_addr.sin_addr);

    /* connect the socket */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    return sockfd;
}

// SEND TO SERVER taken from laboratory
void send_to_server(int sockfd, const char *message)
{
    int bytes, sent = 0;
    int total = strlen(message);

    do
    {
        bytes = write(sockfd, message + sent, total - sent);
        if (bytes < 0)
        {
            error("ERROR writing message to socket");
        }

        if (bytes == 0)
        {
            break;
        }

        sent += bytes;
    } while (sent < total);
}

void ClientClass::getCommand()
{
    cin.getline(buffer, BUFSIZ);
    stringstream strStrm(buffer);
    string word;
    command.clear();
    while (strStrm >> word)
    {
        command.push_back(word);
    }
}

// POST request taken from laboratory
string ClientClass::post(string host, string url,
                         string content_type, nlohmann::json data,
                         vector<nlohmann::json> cookies, string auth)
{
    string request;

    // Step 1: write the method name, URL and protocol type
    request += "POST ";
    request += url;
    request += " HTTP/1.1";
    request += "\r\n";

    // Step 2: add the host
    request += "Host: ";
    request += host;
    request += "\r\n";

    if (auth.length() > 0)
    {
        request += "Authorization: ";
        request += "Bearer ";
        request += auth;
        request += "\r\n";
    }

    // Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
    //     in order to write Content-Length you must first compute the message size
    request += "Content-Type: ";
    request += content_type;
    request += "\r\n";

    int cont_len;
    cont_len = 0;
    string content_buffer;
    content_buffer = data.dump();
    cont_len = content_buffer.length();
    request += "Content-Length: ";
    request += to_string(cont_len);
    request += "\r\n";
    // sprintf(line, "Content-Length: %d", len);
    // compute_message(message,line);
    // Step 4 (optional): add cookies
    for (auto cookie : cookies)
    {
        request += "Cookie: ";
        request += cookie;
        request += "\r\n";
    }

    // Step 5: add new line at end of header
    request += "\r\n";
    // Step 6: add the actual payload data
    request += content_buffer;

    return request;
}

// GET request taken from laboratory
string ClientClass::get(string host, string url,
                        string query_params,
                        vector<nlohmann::json> cookies, string auth)
{
    string request;

    // Step 1: write the method name, URL and protocol type
    request += "GET ";
    request += url;

    if (query_params.length() > 0)
        request += "?" + query_params;

    request += " HTTP/1.1";
    request += "\r\n";

    // Step 2: add the host
    request += "Host: ";
    request += host;
    request += "\r\n";

    if (auth.length() > 0)
    {
        request += "Authorization: ";
        request += "Bearer ";
        request += auth;
        request += "\r\n";
    }

    // Step 3: add cookies
    for (auto cookie : cookies)
    {
        request += "Cookie: ";
        request += cookie;
        request += "\r\n";
    }

    // Step 5: add new line at end of header
    request += "\r\n";

    return request;
}

// DEL request taken from laboratory
string ClientClass::del(string host, string url,
                        string query_params,
                        vector<nlohmann::json> cookies, string auth)
{
    string request;

    // Step 1: write the method name, URL and protocol type
    request += "DELETE ";
    request += url;

    if (query_params.length() > 0)
        request += "?" + query_params;

    request += " HTTP/1.1";
    request += "\r\n";

    // Step 2: add the host
    request += "Host: ";
    request += host;
    request += "\r\n";

    if (auth.length() > 0)
    {
        request += "Authorization: ";
        request += "Bearer ";
        request += auth;
        request += "\r\n";
    }

    // Step 3: add cookies
    for (auto cookie : cookies)
    {
        request += "Cookie: ";
        request += cookie;
        request += "\r\n";
    }

    // Step 5: add new line at end of header
    request += "\r\n";

    return request;
}

// Structure used to store a message received from the server
struct recvServer {
    string headers;
    string content;
    int content_len;
};

// Recv from server taken from laboratory
recvServer receive_from_server(int sockfd)
{
    recvServer resp;
    char response[BUFLEN];
    memset(response, 0, BUFLEN);
    int bytes = read(sockfd, response, BUFLEN);
    DIE(bytes < 0, "RECV ERR");

    
    string buffer(response);

    int content_length = 0;
    ulong content_length_start = buffer.find(CONTENT_LENGTH, 0);
    resp.headers = buffer.substr(0, content_length_start);

    if (content_length_start != string::npos) {
        content_length_start += CONTENT_LENGTH_SIZE;

        content_length = strtol(buffer.substr(content_length_start).c_str(),
                                NULL,
                                10);
    }

    resp.content_len = content_length;

    int body_index = buffer.find("\r\n\r\n", 0);
    body_index += strlen("\r\n\r\n");

    resp.content = buffer.substr(body_index, content_length);
    if(resp.content[content_length - 1] != resp.content[0] + 2) {
        resp.content += resp.content[0] + 2;
        resp.content_len++;
    }
    return resp;
}

// Establish a connection to the server
int ClientClass::connectClient(string str_ip, string str_port)
{
    int port_num;
    port_num = atoi(str_port.c_str());

    char *ip = (char *)malloc(14);
    strcpy(ip, str_ip.c_str());

    server_socket = open_connection(ip, port_num, AF_INET, SOCK_STREAM, 0);
    DIE(server_socket < 0, "CONNECT ERROR");
    return 1;
}

// register a new user 
int ClientClass::registCli(string username, string password)
{
    connectClient(ip, port);
    nlohmann::json data;
    data["password"] = password;
    data["username"] = username;

    vector<nlohmann::json> cookies;
    string message;
    
    message = post("34.118.48.238", "/api/v1/tema/auth/register",
                   "application/json", data, cookies, "");

    recvServer response;
    send_to_server(server_socket, message.c_str());
    response = receive_from_server(server_socket);

    // check for error
    if (response.content.find("error") != string::npos)
    {
        return 0;
    }
    close(server_socket);
    return 1;
}

// login using a username and password
int ClientClass::authCli(string username, string password)
{
    connectClient(ip, port);
    nlohmann::json data;
    data["password"] = password;
    data["username"] = username;
    string message;
    vector<nlohmann::json> cookies;
    message = post("34.118.48.238", "/api/v1/tema/auth/login",
                   "application/json", data, cookies, "");

    recvServer response;
    send_to_server(server_socket, message.c_str());
    response = receive_from_server(server_socket);

    // check for error
    if (response.content.find("error") != string::npos)
    {
        return 0;
    }

    // find the login cookie in the response header
    int login_cookie_START = response.headers.find("connect.sid=", 0);
    int login_cookie_END = response.headers.find(";", login_cookie_START);

    // login cookie will be stored in this instance
    loginToken = string(&response.headers[login_cookie_START],
                        &response.headers[login_cookie_END]);

    close(server_socket);
    return 1;
}

// enter_library
int ClientClass::accessLib()
{
    connectClient(ip, port);
    nlohmann::json data;
    string message;
    vector<nlohmann::json> cookies;
    cookies.push_back(loginToken);
    message = get("34.118.48.238", "/api/v1/tema/library/access",
                  "", cookies, libraryTkn);

    recvServer response;
    send_to_server(server_socket, message.c_str());
    response = receive_from_server(server_socket);

    if (response.content.find("error") != string::npos)
    {
        return 0;
    }
    int jwt_START = response.content.find("{\"token\"", 0);
    int jwt_END = response.content.find("}", jwt_START);

    string jwt = string(&response.content[jwt_START],
                        &response.content[jwt_END + 1]);

    nlohmann::json respJSON;
    respJSON = nlohmann::json::parse(jwt);
    libraryTkn = respJSON["token"];

    close(server_socket);

    return 1;
}

// Returns a formatted list of all the id book pairs in the user's library
int ClientClass::bookList()
{
    connectClient(ip, port);
    nlohmann::json data;
    string message;
    vector<nlohmann::json> cookies;
    cookies.push_back(loginToken);
    message = get("34.118.48.238", "/api/v1/tema/library/books",
                  "", cookies, libraryTkn);

    recvServer response;
    send_to_server(server_socket, message.c_str());
    response = receive_from_server(server_socket);
    // Various feedback massages to handle possible cases
    // error when getting lib
    if (response.content.find("error") != string::npos)
    {
        return 0;
    }

    data = nlohmann::json::parse(response.content);
    // no books in library
    if(data.size() == 0) {
        cout << "No books found in your library, use add_book to add some!\n";
    }
    // Print formatted list of books in library
    for(auto book : data) {
        cout << book["id"] << " | " << book["title"] << endl;
    }

    close(server_socket);
    
    return 1;
}

// Get more details about a certain book by providing the ID
int ClientClass::bookInfo(int id)
{
    connectClient(ip, port);
    nlohmann::json data;
    string message;
    vector<nlohmann::json> cookies;
    cookies.push_back(loginToken);
    message = get("34.118.48.238", "/api/v1/tema/library/books/" + to_string(id),
                  "", cookies, libraryTkn);

    recvServer response;
    send_to_server(server_socket, message.c_str());
    response = receive_from_server(server_socket);

    if (response.content.find("error") != string::npos)
    {
        cout << "No book found!" << endl;
        return 0;
    }

    data = nlohmann::json::parse(response.content);
    for(auto book : data) {
        cout << "Title : \t" << book["title"] << endl; 
        cout << "Author : \t" << book["author"] << endl;
        cout << "Publisher : \t" << book["publisher"] << endl; 
        cout << "Genre : \t" << book["genre"] << endl;
        cout << "Page Count : \t" << book["page_count"] << endl;
    }

    close(server_socket);
    return 1;
}

// Add book with all required fields
int ClientClass::addBook(string title, string author, string genre, string publisher, int page_count)
{
    connectClient(ip, port);
    nlohmann::json data;

    // Populate the json object
    data["title"] = title;
    data["author"] = author;
    data["genre"] = genre;
    data["publisher"] = publisher;
    data["page_count"] = page_count;
    string message;
    vector<nlohmann::json> cookies;

    cookies.push_back(loginToken);
    message = post("34.118.48.238", "/api/v1/tema/library/books",
                   "application/json", data, cookies, libraryTkn);

    recvServer response;
    send_to_server(server_socket, message.c_str());
    response = receive_from_server(server_socket);
    close(server_socket);
    return 0;
}

// delete book using the provided ID
int ClientClass::delBook(int id)
{
    connectClient(ip, port);
    nlohmann::json data;
    string message;
    vector<nlohmann::json> cookies;
    cookies.push_back(loginToken);
    message = del("34.118.48.238", "/api/v1/tema/library/books/" + to_string(id),
                  "", cookies, libraryTkn);

    recvServer response;
    send_to_server(server_socket, message.c_str());
    response = receive_from_server(server_socket);

    close(server_socket);
    // Error check
    if(response.content.find("error") == string::npos)
        return 0;
    return 1;
}

// logout
int ClientClass::logoutCli()
{
    connectClient(ip, port);
    nlohmann::json data;

    string message;
    vector<nlohmann::json> cookies;
    cookies.push_back(loginToken);
    message = get("34.118.48.238", "/api/v1/tema/auth/logout",
                  "", cookies, libraryTkn);

    // Reste login token and JWT token
    libraryTkn = "";
    loginToken = "";

    recvServer response;
    send_to_server(server_socket, message.c_str());
    response = receive_from_server(server_socket);

    close(server_socket);
    return 0;
}

ClientClass::ClientClass()
{
}

ClientClass::~ClientClass()
{
}