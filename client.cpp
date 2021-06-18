#include "ClientClass.hpp"
#include <iostream>

#define CMDLEN 30
// ./client server_ip server_port
using namespace std;

// Help command to remind user about available commands
void help()
{
    cout << "Unrecognized or Invalid Command, valid commands:\n \
    \tregister <username> <password>\n \
    \tlogin <username> <password>\n \
    \tget_books\n  \
    \tget_book <id>\n \
    \tadd_book <title> <author> <genre> <publisher> <page_count>\n \
    \tdelete_book <id>\n \
    \tlogout\n \
    \texit\n";
}
int main(int argc, char *argv[])
{
    setvbuf(stdin, NULL, _IONBF, 0);
    // Command argc validation
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s ip port\n", argv[0]);
        exit(0);
    }

    // Check for valid port input
    if (strlen(argv[2]) > 5)
    {
        fprintf(stderr, "%s is not a valid port\n", argv[2]);
        exit(0);
    }

    // setting up client with the server's information
    ClientClass client;
    client.ip = argv[1];
    client.port = argv[2];

    // Start while loop
    while (true)
    {
        char commandName[CMDLEN];
        cin >> commandName;
        if (strcmp("register", commandName) == 0)
        {
            string username;
            string password;

            cout << "Username = ";
            cin >> username;

            cout << "Password = ";
            cin >> password;

            client.registCli(username, password);
            cout << "Registered " << username << endl;
        }
        else if (strcmp("login", commandName) == 0)
        {
            string username;
            string password;

            cout << "Username = ";
            cin >> username;

            cout << "Password = ";
            cin >> password;

            int res = client.authCli(username, password);
            cout << "Login ";
            if (res == 1)
                cout << "Successful!" << endl;
            else
                cout << "Failed!" << endl;
        }
        else if (strcmp("enter_library", commandName) == 0)
        {
            if(client.loginToken.length() == 0) {
                cout << "You are not logged into your account!\n";
                continue;
            }
            int res = client.accessLib();
            cout << "Library Access ";
            if (res == 1)
                cout << "Granted!" << endl;
            else
                cout << "Denied!" << endl;
        }
        else if (strcmp("get_book", commandName) == 0)
        {
            if(client.loginToken.length() == 0) {
                cout << "You are not logged into your account!\n";
                continue;
            }
            if(client.libraryTkn.length() == 0) {
                cout << "You are not logged into the library!\n";
                continue;
            }
            
            int id;
            cout << "ID = ";
            cin >> id;

            cout << "Searching for book " << id << " in library" << endl;
            
            client.bookInfo(id);
        }
        else if (strcmp("get_books", commandName) == 0)
        {
            if(client.libraryTkn.length() == 0) {
                cout << "You are not logged into the library!\n";
                continue;
            }
            cout << "Getting all books in library" << endl;
            client.bookList();
        }
        else if (strcmp("add_book", commandName) == 0)
        {
            if(client.libraryTkn.length() == 0) {
                cout << "You are not logged into the library!\n";
                continue;
            }

            // Book fields
            string title;
            string author;
            string genre;
            string publisher;
            int page_count;

            // Read multi word string from stdin
            cout << "Title = ";
            cin >> title;
            cin.getline(client.buffer, BUFSIZ);
            stringstream strStrm1(client.buffer);
            string word;
            while (strStrm1 >> word)
            {
                title += " " + word;
            }

            cout << "Author = ";
            cin >> author;
            cin.getline(client.buffer, BUFSIZ);
            stringstream strStrm2(client.buffer);
            while (strStrm2 >> word)
            {
                author += " " + word;
            }
            cout << "Genre = ";
            cin >> genre;
            cout << "Publisher = ";
            cin >> publisher;
            cout << "Page Count = ";
            cin >> page_count;

            client.addBook(title, author, genre, publisher, page_count);
            cout << "Added " << title << " to the library!" << endl;
        }
        else if (strcmp("delete_book", commandName) == 0)
        {
            if(client.libraryTkn.length() == 0) {
                cout << "You are not logged into the library!\n";
                continue;
            }
            
            int id;
            cout << "ID = ";
            cin >> id;

            if(client.delBook(id))
                cout << "No book found for ID " << id << endl;
            else 
                cout << "Deleted book with id: " << id << endl;
        }
        else if (strcmp("logout", commandName) == 0)
        {
            client.logoutCli();
            cout << "Logged out" << endl;
        }
        else if (strcmp("exit", commandName) == 0)
        {
            cout << "Exiting" << endl;
            break;
        }
        else
        {
            help();
        }
    }
}