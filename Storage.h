#include <iostream>
#include <fstream>
#include <unordered_map>
#include <list>

#define max_size 4

class Storage{

    std::unordered_map<std::string,std::pair<std::string,int>> data_base;
    int access_count;
    bool full;
    bool exists_keys_on_disk;
public:
    Storage():access_count(0), full(false){};

    bool putKeyValuePair(std::string key, std::string value){

        if(!full){
            bool not_stored = true;
            if(data_base.find(key) == data_base.end()){
                auto pair = std::make_pair(value,access_count);
                data_base.insert({key, pair});
            }else{
                not_stored = false;
            }
            if(data_base.size() == max_size){
                full = true;
            }
            return not_stored;
        }else{
            bool not_stored = true;
            std::string found_value;
            if((data_base.find(key) == data_base.end()) && !(scan_swapfile(key, found_value))){
                std::string lru_key = findLruKey();
                write_to_swapfile(lru_key, data_base[lru_key].first);
                data_base.erase(lru_key);
                data_base.insert({key,std::make_pair(value,access_count)});
                exists_keys_on_disk = true;
            }
        }
    }

    bool deleteKey(std::string key){
        bool deleted = true; 
        if(data_base.find(key) != data_base.end()){
            data_base.erase(key);
            full = false;
        }else if(exists_keys_on_disk){
            std::string value;
            bool found = scan_swapfile(key,value); 
            if(!found){
                deleted = false;
                std::cout<<"not found on Disk"<<std::endl;
            }else{
                bool empty = erase_from_swapfile(key, value);  
                std::cout<<"empty = "<<empty<<std::endl;
                if(empty){
                    exists_keys_on_disk = false;
                }
            }
        }else{
            deleted = false;
        }
        return deleted;
    }

    std::pair<bool,std::string> getValue(std::string key){
        access_count++;
        bool stored = true; 
        auto pos_it = data_base.find(key);
        if( pos_it != data_base.end()){
            data_base[key].second = access_count;
            return std::make_pair(true, (pos_it->second).first);
        }else if(exists_keys_on_disk){
            std::string value;
            bool found = scan_swapfile(key, value);
            if(full){
                std::string lru_key = findLruKey();
                write_to_swapfile(lru_key, data_base[lru_key].first);
                exists_keys_on_disk = true;
                data_base.erase(lru_key);
            }
            data_base.insert({key,std::make_pair(value,access_count)});
            bool empty = erase_from_swapfile(key,value);
            if(empty){
                exists_keys_on_disk = false;
            }

            return std::make_pair(found,value);

        }else{
            return std::make_pair(false, (pos_it->second).first);
        }

    }

    int getLRUValue(std::string key){
        return data_base[key].second;
    }
    int getAccessCount(){
        return access_count;
    }
    bool getExistsKeysOnDisk(){
        return exists_keys_on_disk;
    }
    bool getFull(){
        return full;
    }

    std::string findLruKey(){
        int curr_min = access_count;
        std::string min_key;
        for(auto it(data_base.begin()); it != data_base.end(); ++it){
            if((it->second).second < curr_min ){
                curr_min = (it->second).second;
                min_key = (it->first);
            }
        }
        return min_key;
    }

    bool scan_swapfile(const std::string &key, std::string &value){
        std::ifstream swapfile; 
        std::cout<<"hello"<<std::endl;
        swapfile.open("swap.txt",std::ios::in);
        if(!swapfile.is_open()){
            std::cout<<"swapfile was not opened"<<std::endl;
        }
        std::string a, b;
        while (swapfile >> a >> b)
        {
            std::cout<<" a = "<<a<<std::endl;
            std::cout<<"b = "<<b<<std::endl;
            if(a.compare(key) == 0){
                value = b; 
                return true;
            }
        }
        swapfile.close();
        return false;
    }

    void write_to_swapfile(const std::string &key, const std::string &value){
        std::ofstream swapfile; 
        swapfile.open("swap.txt", std::ios::out|std::ios::app);
        swapfile<<std::endl;
        std::string new_line = key+" "+value;
        swapfile <<new_line;
        swapfile<<std::endl;
        swapfile.close();
    }
    bool erase_from_swapfile(const std::string &key, const std::string &value){
        bool empty = false;
        std::ifstream swapfile; 
        std::list<std::pair<std::string, std::string>> file_list;
        swapfile.open("swap.txt", std::ios::in); 
        if(!swapfile.is_open()){
            std::cout<<"swapfile was not opened"<<std::endl; 
        }else{
            std::string a, b; 
            while(swapfile >> a >> b)
            {
                std::cout<<"a = "<<a<<std::endl; 
                std::cout<<"b = "<<b<<std::endl;
                file_list.push_back(std::make_pair(a,b));

            }
        }
        for(auto it(file_list.begin()); it != file_list.end();){
            if(key.compare(it->first) == 0){
                it = file_list.erase(it);
            }else{
                ++it;
            }
        }
        if(file_list.empty()){
            empty = true;
        }

        for(auto it(file_list.begin()); it != file_list.end(); ++it){
            std::cout<<" a = "<<it->first<<std::endl;
            std::cout<<"b = "<<it->second<<std::endl;
        }



        std::ofstream new_file;
        new_file.open("swap.txt", std::ios::out);
        for(auto it(file_list.begin()); it != file_list.end(); ++it){
            std::string new_line = it->first+" "+ it->second;
            new_file << new_line; 
            new_file <<std::endl;

            //write_to_swapfile(it->first, it->second);
        }
        new_file.close();
        return empty;
        
    }
};