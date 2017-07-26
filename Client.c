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

    std::string getValue(const std::string &s, zmq::socket_t &send_socket, zmq::message_t &request, zmq::message_t &reply){
        QueryType qt = QueryType::Get;
        Query q(qt,s);
        auto query_string = Parser::serialize(q);
        Helper::s_send(send_socket, query_string, request);

        std::string value = Helper::s_recv(send_socket, reply);
        std::cout<<"after recv"<<std::endl;
        
        return value;

    }

    std::string putKeyValuePair(const std::string &key, const std::string &value, zmq::socket_t &send_socket){

        QueryType qt = QueryType::Put;
        Query q(qt,key, value);
        auto query_string = Parser::serialize(q); //Change serialize!!!!!!!!!!!!!!!!!!!
        std::cout<<"query_string = "<<query_string<<std::endl;

        return query_string;
        //std::cout<<"response = "<<response<<std::endl;

    }

    void deleteKey(const std::string &key, zmq::socket_t &send_socket, zmq::message_t &request, zmq::message_t &reply){

        QueryType qt = QueryType::Delete;
        Query q(qt,key);
        std::string query_string = Parser::serialize(q);
        Helper::s_send(send_socket, query_string, request);

        std::string response = Helper::s_recv(send_socket, reply);
        std::cout<<"response = "<<response<<std::endl;
    }

};

int main(){
//  Socket to talk to clients

    void *context = zmq_ctx_new ();

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

    const char* hello = "Hello";
    s_send (requester, hello);
    char *string = s_recv (requester);
    printf ("Received reply [%s]\n", string);
    free (string);
    
    zmq_close (requester);
    zmq_ctx_destroy (context);
    return 0;



    /*std::string r("World");
    auto query_string = Client::putKeyValuePair(s,r,requester);
    std::cout<<"query_string = "<<query_string<<std::endl;
    zmq::message_t request(query_string.size());
    //Helper::s_send(requester,query_string, request);

    bool rc = memcpy(request.data(), query_string.data(), query_string.size());
    std::cout<<" size = "<< request.size()<<std::endl;
    //bool rc = true;
    char *charPtr = static_cast<char*>(request.data());
    for(int i = 0; i < s.size(); ++i){
        std::cout<<" data = "<<charPtr[i]<<std::endl;
    }

    requester.send(request,0);
    std::cout<<"sent "<<std::endl;


    zmq::message_t reply;
    //std::string response = Helper::s_recv(requester, reply);
    requester.recv(&reply,0);
    std::string reply_s = std::string(static_cast<char*>(reply.data()), reply.size());
    std::cout<<"reply_s = "<<reply_s<<std::endl;
    std::string response = reply_s;

    std::cout<<"response = "<<response<<std::endl;
   // Connection::makeClientConnection(client,res);
    std::string q("Tonsai"); 
    std::string v("Rocks");
    query_string = Client::putKeyValuePair(q,v,requester);

    zmq::message_t request1(query_string.size());
    Helper::s_send(requester,query_string, request1);
    zmq::message_t reply1;
    std::string response1 = Helper::s_recv(requester, reply1);
    std::cout<<"response1 = "<<response1<<std::endl;
    //Connection::makeClientConnection(client,res);
    //auto answer = Connection::recvData(client, buff);
    //auto pair = Client::getValue(s,client); 
    //std::cout<<"value = "<<pair.second<<std::endl;

    Client::deleteKey(s,requester, request, reply);
    requester.close();*/
    //return 0;

}