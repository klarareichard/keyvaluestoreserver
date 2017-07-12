class Storage{

    std::unordered_map<std::string,std::string> data_base;

public:
    bool putKeyValuePair(std::string key, std::string value){
        int not_stored = true;
        if(data_base.find(key) == data_base.end()){
            data_base.insert({key, value});
        }else{
            not_stored = false;
        }
        return not_stored;
    }

    bool deleteKey(std::string key){
        int deleted = true; 
        if(data_base.find(key) != data_base.end()){
            data_base.erase(key);
        }else{
            deleted = false;
        }
        return deleted;
    }

    std::pair<bool,std::string> getValue(std::string key){
        int stored = true; 
        auto pos_it = data_base.find(key);
        if( pos_it != data_base.end()){
            return std::make_pair(true, pos_it->second);
        }else{
            return std::make_pair(false, pos_it->second);
        }

    }
};