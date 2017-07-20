#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zmq.hpp>
#include "zhelpers.hpp"
#include <netdb.h>
#include <unordered_map>
#include "Query.h"
#include "Storage.h"
#include "Connection.h"
#include "HelperFunctions.h"
#include <unistd.h>

namespace Client{

    std::pair<bool,std::string> getValue(const std::string &s, int &send_socket){
        char buff[m_buff_length *4 +4];
        QueryType qt = QueryType::Get;
        Query q(qt,s);
        auto query_string = Parser::serialize(q);
        auto query_char = strdup(query_string.c_str());
        for(int i = 0; i < query_string.size(); ++i){
            std::cout<<"char_arr = "<<query_char[i]<<std::endl;
        }
        std::cout<<"query_string = "<<query_string<<std::endl;
        send(send_socket,query_char,query_string.length(), 0);
        free(query_char);
        char* value;

        std::cout<<"before recv"<<std::endl;
        auto pair = Connection::recvData(send_socket, buff);
        std::cout<<"after recv"<<std::endl;
        bool received = pair; 
        value = buff;
        
        std::string value_s(value);
        if(received){
            value_s = Parser::cutOffLength(value_s);
        }else{
            return std::make_pair(false, std::string());
        }
        return std::make_pair(received,value_s);

    }

    void putKeyValuePair(const std::string &key, const std::string &value, int &send_socket){
        char buff[m_buff_length *4 +4];
        //char query_char[m_buff_length*4 +4];

        QueryType qt = QueryType::Put;
        Query q(qt,key, value);
        auto query_string = Parser::serialize(q);
        std::cout<<"query_string = "<<query_string<<std::endl;
        auto query_char = strdup(query_string.c_str());
        for(int i = 0; i < m_buff_length*4 + 4; i ++){
            std::cout<<query_char[i]<<std::endl;
        }
        bool succ = send(send_socket,query_char,query_string.length(), 0);
        std::cout<<"sent = "<<succ<<std::endl;
        auto pair = Connection::recvData(send_socket, buff);
        auto response = std::string(buff);
        std::cout<<"response = "<<response<<std::endl;
        free(query_char);

    }

    void deleteKey(const std::string &key, int &send_socket){
        char buff[m_buff_length *4 +4];
        //char query_char[m_buff_length*4 +4];
        QueryType qt = QueryType::Delete;
        Query q(qt,key);
        auto query_string = Parser::serialize(q);
        auto query_char = strdup(query_string.c_str());
        for(int i = 0; i < m_buff_length*4 + 4; i ++){
            std::cout<<query_char[i]<<std::endl;
        }
        auto succ = send(send_socket,query_char,query_string.length(), 0);
         std::cout<<"sent = "<<succ<<std::endl;

        auto pair = Connection::recvData(send_socket, buff);
        auto response = std::string(buff);
        std::cout<<"response = "<<response<<std::endl;
        free(query_char);
    }

};

int main(){
    addrinfo* res;
    socklen_t addr_size;
    Connection::getAdress(res, addr_size);
    int client = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    Connection::makeClientConnection(client,res);
    //client.c.makeClientConnection();
    std::string s("Hello"); 
    std::string r("World");
    Client::putKeyValuePair(s,r,client);
   // Connection::makeClientConnection(client,res);
    std::string q("Tonsai"); 
    std::string v("Rocks");
    Client::putKeyValuePair(q,v,client);
    //Connection::makeClientConnection(client,res);
    char buff[m_buff_length *4 +4];
    //auto answer = Connection::recvData(client, buff);
    /*if(answer){
        std::cout<<"received answer"<<std::endl;
    }*/
    //auto pair = Client::getValue(s,client); 
    //std::cout<<"value = "<<pair.second<<std::endl;

    Client::deleteKey(s,client);

}