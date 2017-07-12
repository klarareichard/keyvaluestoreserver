#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unordered_map>
#include "Query.h"
#include "Storage.h"
#include "Connection.h"
#include "HelperFunctions.h"
#include <unistd.h>

namespace Client{

    std::pair<bool,std::string> getValue(const std::string &s, int &send_socket){
        QueryType qt = QueryType::Get;
        Query q(qt,s);
        auto query_string = Parser::serialize(q);
        auto query_char = Helper::string_to_charptr(query_string);
        send(send_socket,query_char,query_string.size(), 0);
        char* value;
        auto pair = Connection::recvData(send_socket);
        bool received = pair.first; 
        value = pair.second;
        
        std::string value_s(value);
        if(received){
            value_s = Parser::cutOffLength(value_s);
        }
        return std::make_pair(received,value);

    }

    void putKeyValuePair(const std::string &key, const std::string &value, int &send_socket){

        QueryType qt = QueryType::Put;
        Query q(qt,key, value);
        auto query_string = Parser::serialize(q);
        std::cout<<"query_string = "<<query_string<<std::endl;
        auto query_char = Helper::string_to_charptr(query_string);
        send(send_socket,query_char,query_string.size(), 0);

    }

    void deleteKey(const std::string &key, int &send_socket){
        QueryType qt = QueryType::Delete;
        Query q(qt,key);
        auto query_string = Parser::serialize(q);
        auto query_char = Helper::string_to_charptr(query_string);
        send(send_socket,query_char,query_string.size(), 0);
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
    auto answer = Connection::recvData(client);
    if(answer.first){
        std::cout<<"received answer"<<std::endl;
    }
    auto pair = Client::getValue(s,client); 
    std::cout<<"value = "<<pair.second<<std::endl;

    Client::deleteKey(s,client);

}