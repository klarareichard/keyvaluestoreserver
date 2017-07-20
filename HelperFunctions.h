#include <string>
#include <stdlib.h>
#pragma once
//static char* sc;
namespace Helper{
    //static char sc;

    char* string_to_charptr(const std::string &s){
        //string tmp = "cat";
        static char sc[1024];
        strncpy(sc, s.c_str(), sizeof(sc));
        sc[sizeof(sc) - 1] = 0;
        return sc;
    }

    std::string bool_to_string(bool b_in){
        std::string tr("4+true");
        std::string fa("5+false");
        std::string b_out;
        b_out = b_in ? tr : fa;
        for(int i = 0; i < 10; i++){
            std::cout<<"b_out = "<<b_out[i]<<std::endl;
        }
        return b_out;
    }

}