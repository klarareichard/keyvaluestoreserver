#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unordered_map>
#include <unistd.h>
#include <stdlib.h>
#include <utility>
#include "HelperFunctions.h"

#pragma once

#define m_backlog 5
#define m_buff_length 30 

namespace Connection{


    //int connected;
    void getAdress( addrinfo* &res, socklen_t &addr_size, const char* port = "3044"){

        addrinfo hints;

        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
        getaddrinfo(NULL, port, &hints, &res);
        std::cout<<"m_res->ai_socktype "<<res->ai_socktype<<std::endl;
        std::cout<<"m_res->ai_protocol " <<res->ai_protocol<<std::endl;

        /*sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        bind(sockfd, res->ai_addr, res->ai_addrlen);
        listen(sockfd, m_backlog);*/

        // now accept an incoming connection:

        addr_size = sizeof(sockaddr_storage);

        std::cout<<"m_res->ai_socktype "<<res->ai_socktype<<std::endl;
        std::cout<<"m_res->ai_protocol " <<res->ai_protocol<<std::endl;
        //int new_socket = socket(m_res->ai_family, m_res->ai_socktype, m_res->ai_protocol);
        //m_listening_socket = &new_socket;
        //m_curr_socket = socket(m_res->ai_family, m_res->ai_socktype, m_res->ai_protocol); 
    }

    void makeServerConnection(int &m_listening_socket, addrinfo *res){
        bool binded = bind(m_listening_socket, res->ai_addr, res->ai_addrlen);
        std::cout<<"binded = "<<binded<<std::endl;
        int connected = listen(m_listening_socket, m_backlog); 
        std::cout<<"errno ="<<strerror(errno)<<std::endl;
        std::cout<<"connected = "<<connected<<std::endl;
        if(connected == -1){
            std::cout<<"Server Connection failed"<<std::endl;
        }else{
            std::cout<<"Server Connection established and Listening"<<std::endl;
        }

    }

    void makeClientConnection(int &m_connection_socket, addrinfo *res){
        int connected = connect(m_connection_socket, res->ai_addr, res->ai_addrlen);
        //connected = connect(*m_listening_socket, m_res->ai_addr, m_res->ai_addrlen);

        if(connected == -1){
            std::cout<<"Client Connection failed"<<std::endl;
        }else{
            std::cout<<"Client Connection established"<<std::endl;
        }
    }

    /*bool acceptConnection(int &m_listening_socket){

        sockaddr_storage their_addr;
        int accept_socket; 
        accept_socket = accept(*m_listening_socket, (struct sockaddr *)&their_addr, &m_addr_size);
        m_connection_socket = &accept_socket;
        if(accept_socket != -1){
            return true;
        }else{
            return false;
        }

    }*/

    std::pair<bool,char*> recvData(int &m_connection_socket){
        int max_size = m_buff_length*4 +4;

        char buff[m_buff_length *4 +4];
        //recv(m_connection_socket, buff, max_size, 0);
        
        //bool accepted = acceptQuery();

        std::cout<<"accepted"<<std::endl;
        int pos = 0;
        //Find length of message which is encoded in the beginning of each message
        int message_length = 0;
        bool received_length = false;
        int num_recv_chars = 0;
        while(!received_length){
            if(num_recv_chars < 0){
                std::cout<<"Error or Connection was closed on you"<<std::endl;
                return std::make_pair(false, buff);
            }else{
                num_recv_chars += recv(m_connection_socket, buff+num_recv_chars, max_size, 0);
                //buff.append(tmp_buff);
                std::string received(buff); 
                std::cout<<"received = "<<received<<std::endl;
                pos = received.find("+");
                std::cout<<"pos = "<<pos<<std::endl;
                std::cout<<"string::npos = "<<std::string::npos<<std::endl;
                if(pos != std::string::npos){
                    std::cout<<"in of"<<std::endl;
                    std::string length_of_message_s = received.substr(0,pos+1);
                    received_length = true;
                    char * length_arr = Helper::string_to_charptr(length_of_message_s);
                    message_length = atoi(length_arr);
                    std::cout<<"Message length = "<<message_length<<std::endl;
                }
            }
        }

        std::cout<<"Message length = "<<message_length<<std::endl;
        std::cout<<"num_recv_chars = "<<num_recv_chars<<std::endl;

        //Get the whole message

        while(message_length+pos+1 < num_recv_chars){

            if(num_recv_chars <= 0){
                std::cout<<"Error or Connection was closed on you"<<std::endl;
                return std::make_pair(false, buff);
            }else{
                num_recv_chars += recv(m_connection_socket, buff+num_recv_chars, max_size, 0);
                //buff.append(tmp_buff);
            }

        }

        return std::make_pair(true, buff);

    }

    /*bool sendData(char* data, int data_size){

        send(*m_connection_socket, data, data_size,0);

    }*/

};

