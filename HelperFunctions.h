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

    char* bool_to_charptr(bool b_in){
        std::string tr("true");
        std::string fa("false");
        char* b_out = b_in ? string_to_charptr(tr) : string_to_charptr(fa);
        return b_out;
    }

}