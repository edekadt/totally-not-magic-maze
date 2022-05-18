#include "Chat.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);

    //Serializar los campos type, nick y message en el buffer _data
    char* tmp = _data;
    memcpy(tmp, &type, sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    nick[nick.size()+1]='\0';
    memcpy(tmp, nick.c_str(), sizeof(char) * 8);
    tmp += sizeof(char) * 8;
    message[message.size()+1]='\0';
    memcpy(tmp, message.c_str(), sizeof(char) * 80);
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    //Reconstruir la clase usando el buffer _data
    char* tmp = _data;

    memcpy(&type, tmp,sizeof(uint8_t));
    tmp += sizeof(uint8_t);
    nick.resize(8);
    memcpy(&nick[0], tmp, sizeof(char) * 8);
    tmp += sizeof(char) * 8;
    message.resize(80);
    //message = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
    memcpy(&message[0], tmp, sizeof(char) * 80);

    std::cout<<message<<std::endl;

    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
    while (true)
    {
        
        Socket* client = 0;
        ChatMessage msg;
        int isMessage = socket.recv(msg, client);

        if (isMessage != -1)
        {
            switch(msg.type)
            {
                case ChatMessage::LOGIN:
                    clients.push_back(std::unique_ptr<Socket>(client));
                    std::cout << msg.nick.c_str() << " logged in" << std::endl;
                break;
                case ChatMessage::LOGOUT:
                {
                    auto it = clients.begin();
                        
                    while (it != clients.end() && (**it !=  *client))
                    {
                        ++it;
                    }
                    
                    if (it == clients.end())
                    {
                        std::cout << "Jugador desconectado" << std::endl;
                    }
                    else
                    {
                        std::cout << "Jugador desconectandose: " << msg.nick << std::endl;
                        clients.erase(it);                 
                        Socket *delSock = (*it).release(); 
                        delete delSock;                    
                    }
                }
                break;
                case ChatMessage::MESSAGE:
                    for (auto it = clients.begin(); it != clients.end(); it++)
                        {
                            if (**it !=  *client)
                            {
                                socket.send(msg, **it);
                            }
                        }

                    std::cout << msg.nick.c_str() << " sent a message" << std::endl;
                break;
            }

            std::cout << "Conected: " << clients.size() << std::endl;
        }   
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/*
ChatClient::ChatClient(const char * s, const char * p, const char * n):socket(s, p),
        nick(n)
        {socket.printsd(); 
        std::cout << "socket memory address: " << &socket << std::endl;};
*/

void ChatClient::login()
{
    std::cout<<"Logging in\n";
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void ChatClient::logout()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGOUT;

    socket.send(em, socket);
}

void ChatClient::input_thread()
{
    while (true)
    {
        std::string msg; 
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
        std::getline(std::cin, msg, '\n');
        
        if(msg == "LOGOUT") 
        {
            logout();
            break;
        }
        else
        {
            ChatMessage em (nick, msg);
            em.type = ChatMessage::MESSAGE;
            socket.send(em, socket);
        }
    }
}

void ChatClient::net_thread()
{
    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        ChatMessage em;

        int isMessage = socket.recv(em);

        if (isMessage != -1 && em.nick == nick)
            std::cout<<"same nick\n";

        if(isMessage != -1 && em.nick != nick)
            std::cout << "<" << em.nick << ">: " << printMessage(em.message) << std::endl;
    }
}

std::string ChatClient::printMessage(std::string message)
{
    std::string msg = "";
    int c = 0;
    while (message[c] != '\0')
        msg += message[0];
    std::cout << "Message length: " << c;
    return msg;
}

