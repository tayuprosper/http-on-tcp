#include "http.h"

// void printContent(std::ifstream &s)
// {
//     std::string line;
//     // while(std::getline(&s,line)){
//     //     std::cout << line << std::endl;
//     // }
// }


class TcpServer
{
private:
    int server_fd;
    int port;
    int max_con;
    
    //type to hold the information about the incoming request
    struct Conn {
        std::string method;                         // The type of http method gotten
        std::string path;                           // The path to the file requested of '/' for the home
        std::string version;                        // The HTTP version used e.g ( 1.1 )
        std::string required_file_type;             // The type of the file requested
    };

    // Structure for the address information of server
    struct sockaddr_in server_addr;


    // Create a socket with the address of the server
    void createSocket()
    {
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1)
            throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
        std::cout << "Socket created successfully." << std::endl;
    }

    // Bind server to the socket 
    void bindServer()
    {
        if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        {
            throw std::runtime_error("Failed to bind socket: " + std::string(strerror(errno)));
        }
        std::cout << "Socket bound to port " << port << " successfully." << std::endl;
    }

    // Listen for a connection on the created socket ( HTTP Connection )
    void listenServer()
    {
        if (listen(server_fd, 10) == -1)
        {
            throw std::runtime_error("Failed to Listen: " + std::string(strerror(errno)));
        }

        std::cout << "Server is listening on port " << port << std::endl;
    }
    

    // Accept any incoming connection the socket.
    void acceptConnetion()
    {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        while(1){
            int client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
            if (client_fd == -1)
            {
                throw std::runtime_error("Failed to accept connection: " + std::string(strerror(errno)));
            }

            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
            int client_port = ntohs(client_address.sin_port);
            std::cout << "Accepted connection from " << client_ip << ":" << client_port << std::endl;

             handleClient(client_fd);
        }
    }

    void handleClient(int client_fd){
        std::string path;
        std::map<std::string, std::string> data = receiveData(client_fd, path);
        if (path == "/" || path.length() == 1)
        {
            sendData(client_fd, "index.html");
        }else{
        ssize_t pos = path.find('/');
        if (pos != std::string::npos){
            path.erase(path.find_first_of('/'), 1);
        }
        std::cout << "sending " << path << std::endl;
        sendData(client_fd, path);
        }
    }

    void handleResourceNotFound(int client_fd){
        std::string header = "";
        status s_status;
        content_type s_content_type;
        std::ifstream file("notfound.html", std::ios::binary);
        if (!file.is_open()){
        
        
        header += s_status.HTTP_404_NOT_FOUND;
        header.append("Connection: close\r\n");
        send(client_fd,header.c_str(), header.length(),0);
        close(client_fd);
        }else{
            file.seekg(0,std::ios::end);
            size_t f_size = file.tellg();
            file.seekg(0, std::ios::beg);
            header.append(s_status.HTTP_200_OK);
            std::stringstream config;
            config << s_content_type.CONTENT_HTML
                    << "Content-Length: " << f_size << "\r\n"
                    << "Connection: close\r\n\r\n";

            header.append(config.str());
            send(client_fd, header.c_str(), header.length(),0);
            char buffer[1024];
            size_t bytesSent = 0;
            while(file){
                file.read(buffer, sizeof(buffer));
                std::streamsize n = file.gcount();
                size_t totalSent = 0;
                while(totalSent < n){
                    size_t sent = send(client_fd, buffer + totalSent,n - totalSent, 0);
                    if (sent <= 0) break;
                    totalSent += sent;
                }
                bytesSent += totalSent;
            }

            close(client_fd);

        }
    }

    void sendData(int fd, std::string filename)
    {
        std::cout << "sending " << filename << " to client" << std::endl;
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()){
            handleResourceNotFound(fd);
        }

        file.seekg(0, std::ios::end);
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::stringstream header;
        header << "HTTP/1.1 200 OK\r\n"
               << "Content-Type: text/html; charset=UTF-8\r\n"
               << "Content-Length: " << fileSize << "\r\n"
               << "Connection: close\r\n\r\n";
        std::string formatedHeader(header.str());
        send(fd, formatedHeader.c_str(), formatedHeader.length(), 0);
        
        char buffer[1024];
        size_t bytesSent = 0;
        while(file){
        file.read(buffer, sizeof(buffer));
        std::streamsize n =file.gcount();
        ssize_t totalSent = 0;
        while(totalSent < n){
           size_t sent = send(fd, buffer + totalSent,n - totalSent, 0);
           if (sent <= 0) break;
            totalSent += sent;
        }
           bytesSent += totalSent;
        }
        std::cout << "sent " << bytesSent << " bytes" << std::endl;
    }

   std::map<std::string, std::string> receiveData(int sock, std::string &path)
{
    std::map<std::string, std::string> result;
    std::string buffer;
    char temp[512];
    bool firstLine = true;

    while (true) {
        ssize_t bytes = recv(sock, temp, sizeof(temp), 0);
        if (bytes <= 0) break;

        buffer.append(temp, bytes);

        // Process all complete lines in buffer
        size_t pos;
        while ((pos = buffer.find("\r\n")) != std::string::npos) {
            std::string line = buffer.substr(0, pos);
            buffer.erase(0, pos + 2); // remove line including \r\n


            if (firstLine) {
                std::istringstream iss(line);
                std::string method, url, version;
                iss >> method >> url >> version;
                path = url;
                firstLine = false;
                continue;
            }

            if (line.empty()) {
                // End of headers
                return result;
            }

            size_t colon = line.find(':');
            if (colon != std::string::npos) {
                std::string key = line.substr(0, colon);
                std::string value = line.substr(colon + 1);
                key.erase(0, key.find_first_not_of(" "));
                value.erase(0, value.find_first_not_of(" "));
                result[key] = value;
            }
        }
    }

    return result;
}



public:
    TcpServer(int _port) : port(_port), server_fd(-1)
    {
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = INADDR_ANY;
    }

    ~TcpServer()
    {
        if (server_fd != -1)
        {
            close(server_fd);
        }
    }

    void start()
    {
        createSocket();
        bindServer();
        listenServer();
        acceptConnetion();
        
        // std::string message = "Hello!, Thank you for connecting to the server...";
        
        
           
    }
};


/*
    /TODO
    * handle path without spacific file request
    * For now only file access is handled e.g http://domain.exm/ or http://doamin.exm/index.html or http://domain.exm/../../file.ext
    * Create handler for no spacific file, just pure path e.g http://domain.exm/login or http://doamin.exm/auth/.../...
*/