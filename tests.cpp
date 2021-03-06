#include "gtest/gtest.h"
#include "Query.h"
#include "HelperFunctions.h"
#include "Storage.h"


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(test_serialize, test_example1){
    QueryType qt = QueryType::Put;
    std::string s("Hello"); 
    std::string t("World");
    Query q(qt,s,t);
    auto serial = Parser::serialize(q); 
    std::string res("Put+Hello+World");
    std::cout<<"serial = "<<serial<<std::endl;
    ASSERT_EQ(serial,res);

}
TEST(test_serialize_deserialize, test_Get){
    QueryType qt = QueryType::Get;
    std::string s("Hello"); 
    Query q(qt,s);
    auto serial = Parser::serialize(q); 
    auto res = Parser::deserialize(serial); 
    ASSERT_EQ(res.getKey(),s); 
    ASSERT_EQ(res.getQueryType(), QueryType::Get);
}
TEST(test_deserialize, test_example2){
    std::string serial("Put+Hello+World"); 
    Query q = Parser::deserialize(serial);
    ASSERT_EQ(q.getQueryType(), QueryType::Put); 
    std::cout<<"queryType = "<< q.getQueryType() <<std::endl;
    std::string hello("Hello"); 
    std::string world("World");
    ASSERT_EQ(q.getKey(),hello ); 
    ASSERT_EQ(q.getValue(), world);
}

TEST(test_string_to_char_ptr, test_example1){
    std::string s("Hello"); 
    //char hello_char[20];

    auto hello_char = Helper::string_to_charptr(s);
    //char bool_char[20]; 
    auto bool_char = Helper::bool_to_string(true); 
    std::string tr("4+true");
    ASSERT_EQ(bool_char, tr);
    /*for(int i = 0; i < 5; ++i){;
        std::cout<<"hello_char["<<i<<"] = "<<hello_char[i]<<std::endl;
    }*/
    std::string hello(hello_char);
    ASSERT_EQ(hello,s);
}

TEST(test_Storage, test_write_and_read_from_swapfile){
    Storage s; 
    std::string dein("dein");
    std::string vadda("vadda");
    s.write_to_swapfile(dein,vadda); 
    std::string test_vadda;
    s.scan_swapfile(dein,test_vadda); 
    ASSERT_EQ(vadda,test_vadda);
    s.erase_from_swapfile(dein,vadda);
}
TEST(test_Storage, test_erase_from_swapfile){
    Storage s; 
    std::string tonsai("tonsai");
    std::string rocks("rocks");
    s.erase_from_swapfile(tonsai,rocks); 
}

TEST(SwappingtoDisk, test_before_swapping){
    Storage s; 
    std::string one("1"); 
    std::string two("2"); 
    std::string three("3"); 
    std::string four("4"); 
    std::string five("5");
    std::string six("6");
    s.putKeyValuePair(one,two); 
    s.putKeyValuePair(two, three); 
    s.putKeyValuePair(three, four); 
    s.putKeyValuePair(four, five);
    s.getValue(one); 
    ASSERT_EQ(s.getAccessCount(), 1);
    ASSERT_EQ(s.getLRUValue(one), 1);
    s.getValue(two); 
    ASSERT_EQ(s.getAccessCount(), 2);
    ASSERT_EQ(s.getLRUValue(two), 2);
    s.putKeyValuePair(five,six);
    auto lru_key = s.findLruKey(); 
    ASSERT_EQ(lru_key, three);
    s.deleteKey(four);
    ASSERT_EQ(s.getExistsKeysOnDisk(), false);
    ASSERT_EQ(s.getFull(), true);

}