#include "server.cpp"
#include "http.h"
int main(int argc, char** argv)
{
    int PORT;
    if (argc >= 2){
        std::string port =(argv[1]);
        PORT = std::stoi(port);
        std::cout << "Running server on PORT: :" << PORT << "\n";

    }else{
        PORT = 8000;
        std::cout << "No PORT entered, Running server on default PORT: :" << PORT << "\n";
    }

    
    try
    {
        TcpServer server(PORT);
        server.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}