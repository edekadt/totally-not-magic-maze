#include <string.h>
#include <cerrno>
#include <cstring>

#include "Serializable.h"
#include "Socket.h"

Socket::Socket(const char * address, const char * port):sd(-1)
{
    struct addrinfo hints;
    struct addrinfo* res;

    memset((void*)&hints, 0, sizeof(hints));

    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int rc = getaddrinfo(address, port, &hints, &res);

    if (rc != 0)
    {
        std::cout << "Error getaddrinfo: " << gai_strerror(rc) << std::endl;
        exit(-1);
    }

    sd = socket(res->ai_family, res->ai_socktype, 0);
    
    if (sd < 0)
    {
        std::cout << "socket: " << strerror(errno) << std::endl;
        exit(-1);
    }

    sa = *res->ai_addr;
    sa_len = res->ai_addrlen;

    freeaddrinfo(res);
}

int Socket::recv(Serializable &obj, Socket * &sock)
{
    struct sockaddr sa;
    socklen_t sa_len = sizeof(struct sockaddr);

    char buffer[MAX_MESSAGE_SIZE];


    ssize_t bytes = ::recvfrom(sd, buffer, MAX_MESSAGE_SIZE, 0, &sa, &sa_len);

    if ( bytes <= 0)
    {
        return -1;
    }
    std::cout << "message received\n";

    if ( sock == 0 )
    {
        std::cout<<"Creando socket\n";
        sock = new Socket(&sa, sa_len);
    }

    obj.from_bin(buffer);

    return 0;
}

int Socket::send(Serializable& obj, const Socket& sock)
{
    std::cout<< "Message sent\n";

    obj.to_bin();

    int objeto = sendto(sd, (void*)obj.data(), obj.size(), 0, (struct sockaddr*)&sock.sa, sock.sa_len);

    if (objeto <= 0)
    {
        std::cout<<"Error enviando mensaje: " << std::strerror(errno) << "\n";
        return -1;
    }

    else
    {
        return 0;
    }
}

bool operator== (const Socket &s1, const Socket &s2)
{
    struct sockaddr_in* sock1 = (struct sockaddr_in*)s1.sa.sa_data;
    struct sockaddr_in* sock2 = (struct sockaddr_in*)s2.sa.sa_data;

    return (sock1->sin_family == sock2->sin_family &&
        sock1->sin_port == sock2->sin_port &&
        sock1->sin_addr.s_addr == sock2->sin_addr.s_addr);
};

bool operator!= (const Socket& s1, const Socket& s2)
{
    return !(s1 == s2);
};

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;

    return os;
};

void Socket::printsd()
{
    std::cout<<"socket: "<<sd<<std::endl;
}