#include <string>
#include <sstream>
#include <vector>
#include <iostream>
enum QueryType{Get, Put, Delete};
class Query{
    QueryType m_t; 
    std::string m_key; 
    std::string m_value;

public:
    Query(QueryType t, std::string key, std::string value = std::string()) : m_t(t), m_key(key), m_value(value){};

    std::string getKey(){   
        return m_key;
    }

    std::string getValue(){
        return m_value;
    }

    QueryType getQueryType(){
        return m_t;
    }

};

namespace Parser{
    std::string serialize(Query &q){
        auto queryt = q.getQueryType();
        //QueryType get;
        std::string put_s("Put");
        std::string get_s("Get"); 
        std::string del_s("Del");
        std::string querytype = std::string();
        if(queryt == 0){
            querytype.append(get_s);
        }else if(queryt == 1){
            querytype.append(put_s);
        }else{
            querytype.append(del_s);
        }

        std::string serialization = querytype + "+" + q.getKey() + "+" + q.getValue();
        std::cout<<"serialization = "<<serialization<<std::endl;
        int length = serialization.length();
        std::string lengths = std::to_string(length);
        return (lengths +  "+" + serialization);

    }

    Query deserialize(std::string s){
        std::stringstream ss(s);
        std::string segment;
        std::vector<std::string> seglist;

        while(std::getline(ss, segment, '+'))
        {
           seglist.push_back(segment);
        }
        QueryType qt;
        std::string key = seglist[2];
        std::string value;
        if(seglist.size() > 3){
            value = seglist[3];
        }else{
            value = std::string();
        }
        std::string querys = seglist[1];

        std::cout<<"querys = "<<querys<<std::endl;
        if(strcmp(querys.c_str(),std::string("Get").c_str()) == 0){
            qt = QueryType::Get;
        }else if(strcmp(querys.c_str(),std::string("Put").c_str()) == 0){
            qt = QueryType::Put;
        }else{
            qt = QueryType::Delete;
        }

        Query q(qt, key, value);
        return q;
    }

    std::string cutOffLength(std::string s){
        int pos = s.find("+");
        s = s.substr(pos+1,s.size()-1);
        return s;
    }
};