#include <sstd/sstd.hpp>
#include <gtest/gtest.h>

#define SSTD_CHashT_DEBUG
#include "./test_CHashT.hpp"
//#define SSTD_IpCHashT_DEBUG
//#include "./test_IpCHashT.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------

// === usage of Google C++ Testing Framework ===
//
// TEST(test_case_name, test_name) {
//     ... test body ...
// }
//
// Ex:
//   namespace SpaceName1{
//       int add1(int x, int y){ return x + y; }
//   };
//   TEST(SpaceName1, add1){ ASSERT_EQ(SpaceName1::add1(1, 1), 2); }

int main(int argc, char** argv){
	printf("\n");
	printf("+---------------------------------------------------+\n");
	printf("|                                                   |\n");
	printf("|     Welcome to Sub Standard Library (SSTD) !      |\n");
	printf("|                                                   |\n");
	printf("|     > This is an Implementation Plan for          |\n");
	printf("|     > In-placeChainedHashTable (IpCHashT)         |\n");
	printf("|     >   and   ChainedHashTable (  CHashT).        |\n");
	printf("|                                                   |\n");
	printf("+---------------------------------------------------+\n");
	printf("\n");
	printf("■ measureTime_start---------------\n\n"); time_m timem; sstd::measureTime_start(timem);
	::testing::InitGoogleTest(&argc, argv);
	
	auto ret = RUN_ALL_TESTS();
	
	printf("\n■ measureTime_stop----------------\n"); sstd::measureTime_stop_print(timem);
	return ret;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

