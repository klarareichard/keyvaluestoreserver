#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unordered_map>
#include <zmq.h> 
#include "zhelpers.h"
#include "Query.h"
#include "Storage.h"
#include "HelperFunctions.h"
#include <unistd.h>


class Server{

    Storage data_base;
public:
    Server(){
        Storage data_base = Storage();
    }

    /*~Server(){
        c.~Connection();
    }*/
    std::string handleQuery(Query &q){
        bool success;
        std::pair<bool, std::string> pair;
        std::string value;

        if(q.getQueryType() == 0){
            pair = data_base.getValue(q.getKey());
            success = pair.first; 
            value = pair.second;
            return value;

        }else if(q.getQueryType() == 1){
            //char v_char[m_buff_length*4 +4];
            success = data_base.putKeyValuePair(q.getKey(), q.getValue());
            std::string key = q.getKey();
            std::string value(data_base.getValue(key).second);

            std::cout<<"value of data_base at "<< key<<" = " <<value<<std::endl;
            auto v_string = Helper::bool_to_string(success);
            for(int i = 0; i < 10; ++i){
                std::cout <<"success = "<<v_string[i]<<std::endl;
            }
            return v_string;
        }else{
            //char v_char[m_buff_length*4 +4];
            success = data_base.deleteKey(q.getKey()); 
            if(!success){
                std::cout<<" Key is not in data base, so it cannot be deleted"<<std::endl;
            }
            std::string v_string = Helper::bool_to_string(success);
            return v_string;
        }
    }

    void run(){

            //  Socket to talk to clients
        void *context = zmq_ctx_new ();
        void *responder = zmq_socket (context, ZMQ_REP);
        int rc = zmq_bind (responder, "tcp://*:5555");
        assert (rc == 0);
        char world[6] = "World";

        while (1) {
            
            char* buffer = s_recv (responder);
            Query q = Parser::deserialize(std::string(buffer));
            auto response = handleQuery(q);
            char char_response[response.size()]; 
            strcpy(char_response, response.data());
            zmq_send (responder, char_response, 50, 0);
            free(buffer);
        }
            

            
    }
    




};






























//First Implementation of Server
/*class Server{

    int* m_curr_socket;
    const char* m_port;
    //int m_size;
    int m_buff_length; // write application on client side that parts the string into chunks with length buff_length
    int m_backlog;
    Storage data_base; //How many connections queue will hold
    //struct sockaddr_storage their_addr;
    addrinfo m_hints;
    addrinfo* m_res;
    socklen_t m_addr_size;

    bool received;
    //struct addrinfo hints, *res;
public:
    Server(const char* port = "1024", int buff_length = 30, int backlog = 5): m_port(port), m_buff_length(buff_length), m_backlog(backlog){

        memset(&m_hints, 0, sizeof m_hints);
        m_hints.ai_family = AF_UNSPEC;
        m_hints.ai_socktype = SOCK_STREAM;
        m_hints.ai_flags = AI_PASSIVE;
        getaddrinfo(NULL, m_port, &m_hints, &m_res);
        std::cout<<"m_res->ai_socktype "<<m_res->ai_socktype<<std::endl;
        std::cout<<"m_res->ai_protocol " <<m_res->ai_protocol<<std::endl;

        // now accept an incoming connection:

        m_addr_size = sizeof(sockaddr_storage);

        std::cout<<"m_res->ai_socktype "<<m_res->ai_socktype<<std::endl;
        std::cout<<"m_res->ai_protocol " <<m_res->ai_protocol<<std::endl;
        m_curr_socket = new int; 
        int new_socket = socket(m_res->ai_family, m_res->ai_socktype, m_res->ai_protocol);
        m_curr_socket = &new_socket;
        bind(*m_curr_socket, m_res->ai_addr, m_res->ai_addrlen);

        bool connected = listen(*m_curr_socket, m_backlog); 
        if(connected == -1){
            std::cout<<"Connection failed"<<std::endl;
        }else{
            std::cout<<"Connection established"<<std::endl;
        }
        //m_curr_socket = socket(m_res->ai_family, m_res->ai_socktype, m_res->ai_protocol); 
    }
    ~Server(){
        close(*m_curr_socket);
        delete m_curr_socket;
    }
    void handleQuery(Query &q, int &socket){
        bool success;
        std::pair<bool, std::string> pair;
        std::string value;
        if(q.getQueryType() == 0){
            pair = data_base.getValue(q.getKey());
            success = pair.first; 
            value = pair.second;
            auto v_char = Helper::string_to_charptr(value);
            send(socket, v_char, value.size()-1,0);
            

        }else if(q.getQueryType() == 1){
            success = data_base.putKeyValuePair(q.getKey(), q.getValue()); 
        }else{
            success = data_base.deleteKey(q.getKey()); 
            if(!success){
                std::cout<<" Key is not in data base, so it cannot be deleted"<<std::endl;
            }
        }
    }

    int acceptQuery(){

        bool connected = listen(*m_curr_socket, m_backlog); 
        if(connected == -1){
            std::cout<<"Connection failed"<<std::endl;
        }else{
            std::cout<<"Connection established"<<std::endl;
        }
        char buff[m_buff_length+1];

        sockaddr_storage their_addr;
        int accept_socket; 
        accept_socket = accept(*m_curr_socket, (struct sockaddr *)&their_addr, &m_addr_size);
        std::cout<<"accept_socket = "<<accept_socket<<std::endl;
        if(accept_socket != -1){
            std::cout<<"accepted"<<std::endl;
            int recv_success;
            do{
                recv_success = recv(accept_socket, buff, m_buff_length, 0);
                if(recv_success == -1){
                    std::cout<<"receive failed"<<std::endl;
                }else if(recv_success == 0){
                    std::cout<<" connection was closed on you. So sorry :-O!"<<std::endl;
                }else if( recv_success < m_buff_length){
                    std::cout<<" received only "<< recv_success << "bits, while should have received "<<m_buff_length<<std::endl;
                }else{
                //accept_socket.close();

                //store data in data_base
                    std::string received(buff);
                    Query recv_query = Parser::deserialize(received);
                    handleQuery(recv_query, accept_socket); 
                    close(accept_socket);
                }
            }while(!recv_success);
        }

    }
};*/

int main(){
    Server s;
    s.run();
    return 0;
}