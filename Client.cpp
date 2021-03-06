#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zmq.h>
#include "zhelpers.h"
#include <netdb.h>
#include <unordered_map>
#include "Query.h"
#include "Storage.h"
//#include "HelperFunctions.h"
#include <unistd.h>

namespace Client{

    std::string getValue(const std::string &s){
        QueryType qt = QueryType::Get;
        Query q(qt,s);
        auto query_string = Parser::serialize(q);

        std::cout<<"after recv"<<std::endl;
        
        return query_string;

    }

    std::string putKeyValuePair(const std::string &key, const std::string &value){

        QueryType qt = QueryType::Put;
        Query q(qt,key, value);
        auto query_string = Parser::serialize(q); //Change serialize!!!!!!!!!!!!!!!!!!!
        std::cout<<"query_string = "<<query_string<<std::endl;

        return query_string;
        //std::cout<<"response = "<<response<<std::endl;

    }

    std::string deleteKey(const std::string &key){

        QueryType qt = QueryType::Delete;
        Query q(qt,key);
        std::string query_string = Parser::serialize(q);
        //std::cout<<"response = "<<response<<std::endl;
        return query_string;
    }

    void run_more_clients(){

        #pragma omp parallel for
        for(int client_num = 0; client_num < 9; ++client_num){
            void * context = zmq_ctx_new();
            void *requester = zmq_socket (context, ZMQ_REQ);
            zmq_connect (requester, "tcp://localhost:5555");

            std::string key = std::to_string(client_num);
            std::string value = std::to_string(client_num+1);
            auto query_string = Client::putKeyValuePair(key,value);

            std::cout<<"query_string = "<<query_string<<std::endl;
            char query_char[query_string.size()]; 
            strcpy(query_char, query_string.data());

            s_send (requester, query_char);
            char *string = s_recv (requester);
            printf ("Received reply [%s]\n", string);
            auto rep = std::string(string); 
            std::cout<<"rep = "<<rep<<std::endl;
            free (string);

            auto query_string1 = Client::getValue(key);
            std::cout<<"query_string1 = "<<query_string1<<std::endl;
            char query_char1[query_string1.size()]; 
            strcpy(query_char1, query_string1.data());

            s_send (requester, query_char1);
            char *string1 = s_recv (requester);
            printf ("Received reply [%s]\n", string1);
            auto rep1 = std::string(string1); 
            std::cout<<"rep1 = "<<rep1<<std::endl;
            free (string1);

            zmq_close(requester);
            zmq_ctx_destroy(context);

        
        }

    }

};

int main(){
//  Socket to talk to clients
    void *context = zmq_ctx_new ();

    //  Socket to talk to server
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

    char hello[6] = "Hello";
    char world[6] = "World";
    auto query_string = Client::putKeyValuePair(hello,world);
    std::cout<<"query_string = "<<query_string<<std::endl;
    char query_char[query_string.size()]; 
    strcpy(query_char, query_string.data());

    s_send (requester, query_char);
    char *string = s_recv (requester);
    printf ("Received reply [%s]\n", string);
    free (string);

    auto query_string1 = Client::getValue(hello);
    std::cout<<"query_string1 = "<<query_string1<<std::endl;
    char query_char1[query_string1.size()]; 
    strcpy(query_char1, query_string1.data());

    s_send (requester, query_char1);
    char *string1 = s_recv (requester);
    printf ("Received reply [%s]\n", string1);
    free (string1);
    
    zmq_close (requester);
    zmq_ctx_destroy (context);
    Client::run_more_clients();

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