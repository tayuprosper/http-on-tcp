#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>
#include <sstream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <vector>
#include <map>
#include <fstream>

#pragma once
struct status  {
    const std::string HTTP_404_NOT_FOUND = "HTTP/1.1 404 NOT_FOUND\r\n";
    const std::string HTTP_200_OK = "HTTP/1.1 200 OK\r\n";
    const std::string HTTP_401_BAD_REQUEST = "HTTP/1.1 401 BAD_REQUEST\r\n";
};

struct content_type  {
    // set up for human readable text Types
    const std::string CONTENT_HTML = "Content-Type: text/html; charset=UTF-8r\n";       // For html response;
    const std::string CONTENT_PLAIN_TEXT = "Content-Type: text/plain\r\n";              // For plain text response;
    const std::string CONTENT_CSS = "Content-Type: text/css\r\n";                       // For CSS response;
    const std::string CONTENT_JS = "Content-Type: text/javascript\r\n";                 // For JavaScript Content response;
    const std::string CONTENT_XML = "Content-Type: text/xml\r\n";                       // For xml content response;
    const std::string CONTENT_MARKDOWN = "Content-Type: text/markdown\r\n";             // For markdown (.md files) response;

    // Set up for Images Types
    const std::string CONTENT_JPEG = "Content-Type:  image/jpeg";                       // For jpeg images response;
    const std::string CONTENT_PNG = "Content-Type:  image/png";                         // For png images response;
    const std::string CONTENT_GIF = "Content-Type:  image/gif";                         // For gif reponse;
    const std::string CONTENT_SVG = "Content-Type:  image/svg+xml";                     // For svg response

    // Set up for Audio/Video Types
    const std::string CONTENT_MPEG = "Content-Type: audio/mpeg";                            // For mp3 audio files
    const std::string CONTENT_OGG = "Content-Type: audio/";                            // For mp3 audio files

    // TODO 
    //More content types suported to be added soon.
};
