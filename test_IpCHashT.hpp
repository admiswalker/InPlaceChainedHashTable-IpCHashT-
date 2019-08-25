#include <random>
#include "./IpCHashT.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------

void printTable_all(const sstd::IpCHashT<uint64, uint64>& hashT){
	for(auto itr=hashT.begin(); itr!=hashT.end(); ++itr){
		itr.print_dbg();
		printf("\n");
	}
}/*
uint64 countup_tableSize(const sstd::IpCHashT<uint64, uint64>& hashT){
	uint64 i=0ull;
	for(auto itr=hashT.begin(); itr!=hashT.end(); ++itr){ i++; }
	return i;
}
//*/
//-----------------------------------------------------------------------------------------------------------------------------------------------
// definition of the user hash function

namespace usrDef_in_IpCHash{ class hashFn; }
class usrDef_in_IpCHash::hashFn{
private:
public:
	hashFn(){}
	~hashFn(){}
	size_t operator()(const uint64& key){ return key; }
};

TEST(sstd_IpCHashT, use_usr_defined_hash_func){
	sstd::IpCHashT<uint64, uint64, usrDef_in_IpCHash::hashFn> hashT(10);
	
	hashT.insert_hard( 1, 10);
	auto itr = hashT.find(1); ASSERT_TRUE( itr!=hashT.end() ); ASSERT_TRUE( itr.first()==1 ); ASSERT_TRUE( itr.second()==10 );
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// insert_soft

TEST(sstd_IpCHashT, insert_soft_case01){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 1;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case01
	hashT.insert_soft( 1, 10);
	{
		// check: case01
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_soft_case03){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 1;    // force to set key-val on the table index "tIdx_dbg".
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 0;

		hashT._elems() = 2;
	}
	
	// insertion: case03
	hashT.insert_soft( 3, 30);
//	printTable_all(hashT);
	{
		// check: case03
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 2 );
		ASSERT_TRUE( hashT._pT()[2].val ==20 );
		ASSERT_TRUE( hashT._pT()[2].prev== 1 );
		ASSERT_TRUE( hashT._pT()[2].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 3 );
		ASSERT_TRUE( hashT._pT()[3].val ==30 );
		ASSERT_TRUE( hashT._pT()[3].prev== 1 );
		ASSERT_TRUE( hashT._pT()[3].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_soft_case06){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 0;

		hashT._elems() = 2;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 2;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case06
	hashT.insert_soft( 3, 30);
//	printTable_all(hashT);
	
	{
		// check: case06
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 2 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 3 );
		ASSERT_TRUE( hashT._pT()[2].val ==30 );
		ASSERT_TRUE( hashT._pT()[2].prev== 0 );
		ASSERT_TRUE( hashT._pT()[2].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 2 );
		ASSERT_TRUE( hashT._pT()[3].val ==20 );
		ASSERT_TRUE( hashT._pT()[3].prev== 2 );
		ASSERT_TRUE( hashT._pT()[3].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_soft_case11){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 1;
		
		hashT._pT()[3].key = 3;
		hashT._pT()[3].val =30;
		hashT._pT()[3].prev= 1;
		hashT._pT()[3].next= 0;

		hashT._elems() = 3;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 2;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case11
	hashT.insert_soft( 4, 40);
//	printTable_all(hashT);
	{
		// check: case11
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 2 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 4 );
		ASSERT_TRUE( hashT._pT()[2].val ==40 );
		ASSERT_TRUE( hashT._pT()[2].prev== 0 );
		ASSERT_TRUE( hashT._pT()[2].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 3 );
		ASSERT_TRUE( hashT._pT()[3].val ==30 );
		ASSERT_TRUE( hashT._pT()[3].prev== 2 );
		ASSERT_TRUE( hashT._pT()[3].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[4].key == 2 );
		ASSERT_TRUE( hashT._pT()[4].val ==20 );
		ASSERT_TRUE( hashT._pT()[4].prev== 1 );
		ASSERT_TRUE( hashT._pT()[4].next== 0 );
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// insert_hard

TEST(sstd_IpCHashT, insert_hard_case01){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 1;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case01
	hashT.insert_hard( 1, 10);
	{
		// check: case01
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_hard_case02){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 1;    // force to set key-val on the table index "tIdx_dbg".
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 2;
		
		// idx==2 is empty
		
		hashT._pT()[3].key = 3;
		hashT._pT()[3].val =30;
		hashT._pT()[3].prev= 2;
		hashT._pT()[3].next= 0;
		
		hashT._elems() = 2;
	}

	// insertion: case02
	hashT.insert_hard( 2, 20);
//	printTable_all(hashT);
	{
		// check: case02
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 2 );
		ASSERT_TRUE( hashT._pT()[2].val ==20 );
		ASSERT_TRUE( hashT._pT()[2].prev== 1 );
		ASSERT_TRUE( hashT._pT()[2].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 3 );
		ASSERT_TRUE( hashT._pT()[3].val ==30 );
		ASSERT_TRUE( hashT._pT()[3].prev== 1 );
		ASSERT_TRUE( hashT._pT()[3].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_hard_case02_02){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 1;    // force to set key-val on the table index "tIdx_dbg".
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 2;
		
		// idx==3 is empty
		
		hashT._pT()[4].key = 4;
		hashT._pT()[4].val =40;
		hashT._pT()[4].prev= 2;
		hashT._pT()[4].next= 0;

		hashT._elems() = 3;
	}
	
	// insertion: case02
	hashT.insert_hard( 5, 50);
//	printTable_all(hashT);
	{
		// check: case02
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 2 );
		ASSERT_TRUE( hashT._pT()[2].val ==20 );
		ASSERT_TRUE( hashT._pT()[2].prev== 1 );
		ASSERT_TRUE( hashT._pT()[2].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 5 );
		ASSERT_TRUE( hashT._pT()[3].val ==50 );
		ASSERT_TRUE( hashT._pT()[3].prev== 1 );
		ASSERT_TRUE( hashT._pT()[3].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[4].key == 4 );
		ASSERT_TRUE( hashT._pT()[4].val ==40 );
		ASSERT_TRUE( hashT._pT()[4].prev== 1 );
		ASSERT_TRUE( hashT._pT()[4].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_hard_case03){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 1;    // force to set key-val on the table index "tIdx_dbg".
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 0;

		hashT._elems() = 2;
	}
	
	// insertion: case03
	hashT.insert_hard( 3, 30);
//	printTable_all(hashT);
	{
		// check: case03
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 2 );
		ASSERT_TRUE( hashT._pT()[2].val ==20 );
		ASSERT_TRUE( hashT._pT()[2].prev== 1 );
		ASSERT_TRUE( hashT._pT()[2].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 3 );
		ASSERT_TRUE( hashT._pT()[3].val ==30 );
		ASSERT_TRUE( hashT._pT()[3].prev== 1 );
		ASSERT_TRUE( hashT._pT()[3].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_hard_case04){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 2;
	
		// idx==2 is empty
		
		hashT._pT()[3].key = 3;
		hashT._pT()[3].val =30;
		hashT._pT()[3].prev= 2;
		hashT._pT()[3].next= 0;

		hashT._elems() = 2;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 3;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case04
	hashT.insert_hard( 4, 40);
//	printTable_all(hashT);
	{
		// check: case04
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 3 );
		ASSERT_TRUE( hashT._pT()[2].val ==30 );
		ASSERT_TRUE( hashT._pT()[2].prev== 1 );
		ASSERT_TRUE( hashT._pT()[2].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 4 );
		ASSERT_TRUE( hashT._pT()[3].val ==40 );
		ASSERT_TRUE( hashT._pT()[3].prev== 0 );
		ASSERT_TRUE( hashT._pT()[3].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_hard_case05){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 2;
		
		hashT._pT()[3].key = 3;
		hashT._pT()[3].val =30;
		hashT._pT()[3].prev= 2;
		hashT._pT()[3].next= 1;
		
		hashT._pT()[4].key = 4;
		hashT._pT()[4].val =40;
		hashT._pT()[4].prev= 1;
		hashT._pT()[4].next= 0;

		hashT._elems() = 3;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 4;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case05
	hashT.insert_hard( 5, 50);
//	printTable_all(hashT);
	{
		// check: case05
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 4 );
		ASSERT_TRUE( hashT._pT()[2].val ==40 );
		ASSERT_TRUE( hashT._pT()[2].prev== 1 );
		ASSERT_TRUE( hashT._pT()[2].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 3 );
		ASSERT_TRUE( hashT._pT()[3].val ==30 );
		ASSERT_TRUE( hashT._pT()[3].prev== 1 );
		ASSERT_TRUE( hashT._pT()[3].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[4].key == 5 );
		ASSERT_TRUE( hashT._pT()[4].val ==50 );
		ASSERT_TRUE( hashT._pT()[4].prev== 0 );
		ASSERT_TRUE( hashT._pT()[4].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_hard_case06){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 0;

		hashT._elems() = 2;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 2;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case06
	hashT.insert_hard( 3, 30);
//	printTable_all(hashT);
	
	{
		// check: case06
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 2 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 3 );
		ASSERT_TRUE( hashT._pT()[2].val ==30 );
		ASSERT_TRUE( hashT._pT()[2].prev== 0 );
		ASSERT_TRUE( hashT._pT()[2].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 2 );
		ASSERT_TRUE( hashT._pT()[3].val ==20 );
		ASSERT_TRUE( hashT._pT()[3].prev== 2 );
		ASSERT_TRUE( hashT._pT()[3].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_hard_case07){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 2;
		
		// idx==2 is empty
		
		hashT._pT()[3].key = 3;
		hashT._pT()[3].val =30;
		hashT._pT()[3].prev= 2;
		hashT._pT()[3].next= 1;
		
		hashT._pT()[4].key = 4;
		hashT._pT()[4].val =40;
		hashT._pT()[4].prev= 1;
		hashT._pT()[4].next= 0;

		hashT._elems() = 3;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 3;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case07
	hashT.insert_hard( 5, 50);
//	printTable_all(hashT);
	{
		// check: case04
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 3 );
		ASSERT_TRUE( hashT._pT()[2].val ==30 );
		ASSERT_TRUE( hashT._pT()[2].prev== 1 );
		ASSERT_TRUE( hashT._pT()[2].next== 2 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 5 );
		ASSERT_TRUE( hashT._pT()[3].val ==50 );
		ASSERT_TRUE( hashT._pT()[3].prev== 0 );
		ASSERT_TRUE( hashT._pT()[3].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[4].key == 4 );
		ASSERT_TRUE( hashT._pT()[4].val ==40 );
		ASSERT_TRUE( hashT._pT()[4].prev== 2 );
		ASSERT_TRUE( hashT._pT()[4].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_hard_case08){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 2;
		
		// idx==2 is empty
		
		hashT._pT()[3].key = 3;
		hashT._pT()[3].val =30;
		hashT._pT()[3].prev= 2;
		hashT._pT()[3].next= 1;
		
		hashT._pT()[4].key = 4;
		hashT._pT()[4].val =40;
		hashT._pT()[4].prev= 1;
		hashT._pT()[4].next= 1;
		
		hashT._pT()[5].key = 5;
		hashT._pT()[5].val =50;
		hashT._pT()[5].prev= 1;
		hashT._pT()[5].next= 0;

		hashT._elems() = 4;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 4;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case08
	hashT.insert_hard( 6, 60);
//	printTable_all(hashT);
	{
		// check: case08
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 4 );
		ASSERT_TRUE( hashT._pT()[2].val ==40 );
		ASSERT_TRUE( hashT._pT()[2].prev== 1 );
		ASSERT_TRUE( hashT._pT()[2].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 3 );
		ASSERT_TRUE( hashT._pT()[3].val ==30 );
		ASSERT_TRUE( hashT._pT()[3].prev== 1 );
		ASSERT_TRUE( hashT._pT()[3].next== 2 );
		
		ASSERT_TRUE( hashT._pT()[4].key == 6 );
		ASSERT_TRUE( hashT._pT()[4].val ==60 );
		ASSERT_TRUE( hashT._pT()[4].prev== 0 );
		ASSERT_TRUE( hashT._pT()[4].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[5].key == 5 );
		ASSERT_TRUE( hashT._pT()[5].val ==50 );
		ASSERT_TRUE( hashT._pT()[5].prev== 2 );
		ASSERT_TRUE( hashT._pT()[5].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_hard_case09){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 2;
		
		// idx==3 is empty
		
		hashT._pT()[4].key = 4;
		hashT._pT()[4].val =40;
		hashT._pT()[4].prev= 2;
		hashT._pT()[4].next= 0;

		hashT._elems() = 3;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 2;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case09
	hashT.insert_hard( 6, 60);
//	printTable_all(hashT);
	{
		// check: case09
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 2 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 6 );
		ASSERT_TRUE( hashT._pT()[2].val ==60 );
		ASSERT_TRUE( hashT._pT()[2].prev== 0 );
		ASSERT_TRUE( hashT._pT()[2].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 2 );
		ASSERT_TRUE( hashT._pT()[3].val ==20 );
		ASSERT_TRUE( hashT._pT()[3].prev== 2 );
		ASSERT_TRUE( hashT._pT()[3].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[4].key == 4 );
		ASSERT_TRUE( hashT._pT()[4].val ==40 );
		ASSERT_TRUE( hashT._pT()[4].prev== 1 );
		ASSERT_TRUE( hashT._pT()[4].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_hard_case10){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 1;
		
		hashT._pT()[3].key = 3;
		hashT._pT()[3].val =30;
		hashT._pT()[3].prev= 1;
		hashT._pT()[3].next= 2;

		// idx==4 is empty
		
		hashT._pT()[5].key = 5;
		hashT._pT()[5].val =50;
		hashT._pT()[5].prev= 2;
		hashT._pT()[5].next= 0;

		hashT._elems() = 4;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 2;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case10
	hashT.insert_hard( 6, 60);
//	printTable_all(hashT);
	{
		// check: case10
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 2 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 6 );
		ASSERT_TRUE( hashT._pT()[2].val ==60 );
		ASSERT_TRUE( hashT._pT()[2].prev== 0 );
		ASSERT_TRUE( hashT._pT()[2].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 3 );
		ASSERT_TRUE( hashT._pT()[3].val ==30 );
		ASSERT_TRUE( hashT._pT()[3].prev== 2 );
		ASSERT_TRUE( hashT._pT()[3].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[4].key == 2 );
		ASSERT_TRUE( hashT._pT()[4].val ==20 );
		ASSERT_TRUE( hashT._pT()[4].prev== 1 );
		ASSERT_TRUE( hashT._pT()[4].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[5].key == 5 );
		ASSERT_TRUE( hashT._pT()[5].val ==50 );
		ASSERT_TRUE( hashT._pT()[5].prev== 1 );
		ASSERT_TRUE( hashT._pT()[5].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_hard_case10_02){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 1;
		
		hashT._pT()[3].key = 3;
		hashT._pT()[3].val =30;
		hashT._pT()[3].prev= 1;
		hashT._pT()[3].next= 1;
		
		hashT._pT()[4].key = 4;
		hashT._pT()[4].val =40;
		hashT._pT()[4].prev= 1;
		hashT._pT()[4].next= 2;

		// idx==5 is empty
		
		hashT._pT()[6].key = 6;
		hashT._pT()[6].val =60;
		hashT._pT()[6].prev= 2;
		hashT._pT()[6].next= 0;

		hashT._elems() = 5;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 2;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case10
	hashT.insert_hard( 7, 70);
//	printTable_all(hashT);
	{
		// check: case10
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 2 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 7 );
		ASSERT_TRUE( hashT._pT()[2].val ==70 );
		ASSERT_TRUE( hashT._pT()[2].prev== 0 );
		ASSERT_TRUE( hashT._pT()[2].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 3 );
		ASSERT_TRUE( hashT._pT()[3].val ==30 );
		ASSERT_TRUE( hashT._pT()[3].prev== 2 );
		ASSERT_TRUE( hashT._pT()[3].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[4].key == 4 );
		ASSERT_TRUE( hashT._pT()[4].val ==40 );
		ASSERT_TRUE( hashT._pT()[4].prev== 1 );
		ASSERT_TRUE( hashT._pT()[4].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[5].key == 2 );
		ASSERT_TRUE( hashT._pT()[5].val ==20 );
		ASSERT_TRUE( hashT._pT()[5].prev== 1 );
		ASSERT_TRUE( hashT._pT()[5].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[6].key == 6 );
		ASSERT_TRUE( hashT._pT()[6].val ==60 );
		ASSERT_TRUE( hashT._pT()[6].prev== 1 );
		ASSERT_TRUE( hashT._pT()[6].next== 0 );
	}
}
TEST(sstd_IpCHashT, insert_hard_case11){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 1;
		
		hashT._pT()[3].key = 3;
		hashT._pT()[3].val =30;
		hashT._pT()[3].prev= 1;
		hashT._pT()[3].next= 0;

		hashT._elems() = 3;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 2;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case11
	hashT.insert_hard( 4, 40);
//	printTable_all(hashT);
	{
		// check: case11
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 2 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 4 );
		ASSERT_TRUE( hashT._pT()[2].val ==40 );
		ASSERT_TRUE( hashT._pT()[2].prev== 0 );
		ASSERT_TRUE( hashT._pT()[2].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 3 );
		ASSERT_TRUE( hashT._pT()[3].val ==30 );
		ASSERT_TRUE( hashT._pT()[3].prev== 2 );
		ASSERT_TRUE( hashT._pT()[3].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[4].key == 2 );
		ASSERT_TRUE( hashT._pT()[4].val ==20 );
		ASSERT_TRUE( hashT._pT()[4].prev== 1 );
		ASSERT_TRUE( hashT._pT()[4].next== 0 );
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
TEST(sstd_IpCHashT, insert_soft_out_of_case_00){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 0;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 0;
		hashT._pT()[2].next= 0;

		hashT._elems() = 2;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 1;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case03
	hashT.insert_soft( 3, 30);
//	printTable_all(hashT);
	{
		// check: case03
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 2 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 2 );
		ASSERT_TRUE( hashT._pT()[2].val ==20 );
		ASSERT_TRUE( hashT._pT()[2].prev== 0 );
		ASSERT_TRUE( hashT._pT()[2].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 3 );
		ASSERT_TRUE( hashT._pT()[3].val ==30 );
		ASSERT_TRUE( hashT._pT()[3].prev== 2 );
		ASSERT_TRUE( hashT._pT()[3].next== 0 );
	}
}
//---
TEST(sstd_IpCHashT, insert_hard_out_of_case_00){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 0;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 0;
		hashT._pT()[2].next= 0;

		hashT._elems() = 2;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 1;    // force to set key-val on the table index "tIdx_dbg".

	// insertion: case03
	hashT.insert_hard( 3, 30);
//	printTable_all(hashT);
	{
		// check: case03
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 2 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 2 );
		ASSERT_TRUE( hashT._pT()[2].val ==20 );
		ASSERT_TRUE( hashT._pT()[2].prev== 0 );
		ASSERT_TRUE( hashT._pT()[2].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[3].key == 3 );
		ASSERT_TRUE( hashT._pT()[3].val ==30 );
		ASSERT_TRUE( hashT._pT()[3].prev== 2 );
		ASSERT_TRUE( hashT._pT()[3].next== 0 );
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
TEST(sstd_IpCHashT, find){
	sstd::IpCHashT<uint64, uint64> hashT(10);

	hashT.insert_hard(1, 10);
	auto itr = hashT.find(1);
	ASSERT_TRUE( itr.first()  ==  1 );
	ASSERT_TRUE( itr.second() == 10 );
}
TEST(sstd_IpCHashT, find_02){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	hashT.insert_hard(1, 10);
	hashT.insert_hard(1, 20);
	auto itr = hashT.find(1);
	ASSERT_TRUE( itr != hashT.end() );
	ASSERT_TRUE( itr.first()  ==  1 );
	ASSERT_TRUE( itr.second() == 20 );
	
	auto itr2 = hashT.find(2);
	ASSERT_FALSE( itr2 != hashT.end() );
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
TEST(sstd_IpCHashT, erase_case01){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 1;    // force to set key-val on the table index "tIdx_dbg".
	
	hashT.insert_hard(1, 10);
	hashT.erase(1);
//	printTable_all(hashT);
	{
		// check: case01
		ASSERT_TRUE( hashT._pT()[1].prev==hashT._maxShift() );
		ASSERT_TRUE( hashT._pT()[1].next== 0                );
	}
}
TEST(sstd_IpCHashT, erase_case02){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 0;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 1;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case02
	hashT.erase(2);
//	printTable_all(hashT);
	{
		// check: case02
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[2].prev==hashT._maxShift() );
		ASSERT_TRUE( hashT._pT()[2].next== 0                );
	}
}
TEST(sstd_IpCHashT, erase_case03){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 1;
		
		hashT._pT()[3].key = 3;
		hashT._pT()[3].val =30;
		hashT._pT()[3].prev= 1;
		hashT._pT()[3].next= 0;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 1;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case11
	hashT.erase(1);
//	printTable_all(hashT);
	{
		// check: case11
		ASSERT_TRUE( hashT._pT()[1].key == 3 );
		ASSERT_TRUE( hashT._pT()[1].val ==30 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 2 );
		ASSERT_TRUE( hashT._pT()[2].val ==20 );
		ASSERT_TRUE( hashT._pT()[2].prev== 1 );
		ASSERT_TRUE( hashT._pT()[2].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[3].prev==hashT._maxShift() );
		ASSERT_TRUE( hashT._pT()[3].next== 0                );
	}
}
TEST(sstd_IpCHashT, erase_case04){
	sstd::IpCHashT<uint64, uint64> hashT(10);
	
	{
		// init
		hashT._pT()[1].key = 1;
		hashT._pT()[1].val =10;
		hashT._pT()[1].prev= 0;
		hashT._pT()[1].next= 1;
		
		hashT._pT()[2].key = 2;
		hashT._pT()[2].val =20;
		hashT._pT()[2].prev= 1;
		hashT._pT()[2].next= 1;
		
		hashT._pT()[3].key = 3;
		hashT._pT()[3].val =30;
		hashT._pT()[3].prev= 1;
		hashT._pT()[3].next= 0;
	}
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 1;    // force to set key-val on the table index "tIdx_dbg".
	
	// insertion: case11
	hashT.erase(2);
//	printTable_all(hashT);
	{
		// check: case11
		ASSERT_TRUE( hashT._pT()[1].key == 1 );
		ASSERT_TRUE( hashT._pT()[1].val ==10 );
		ASSERT_TRUE( hashT._pT()[1].prev== 0 );
		ASSERT_TRUE( hashT._pT()[1].next== 1 );
		
		ASSERT_TRUE( hashT._pT()[2].key == 3 );
		ASSERT_TRUE( hashT._pT()[2].val ==30 );
		ASSERT_TRUE( hashT._pT()[2].prev== 1 );
		ASSERT_TRUE( hashT._pT()[2].next== 0 );
		
		ASSERT_TRUE( hashT._pT()[3].prev==hashT._maxShift() );
		ASSERT_TRUE( hashT._pT()[3].next== 0                );
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
bool stressTest_oneCycle_soft(const uint64 seed, const uint64 limitSize){
	sstd::IpCHashT<uint64, uint64> hashT(0);
	hashT.use_pSize_dbg = true; // enable debug option
	hashT.pSize_dbg     = 0;    // force to set key-val on the table index "tIdx_dbg".
	
//	uint64 limitSize = 42949672; // 42949672 elements == 1 G Byte or more
	// when bool is 1 bytes, one elements of "struct sstd_CHashT::element" is 25 bytes (== 1 + key8 + val8 + p8).
	// thus, 1 G Bytes table would have 42949672.96 (== 1*1024*1024*1024 Bytes / 25) elements in rough estimate.
	// in attention, we needs to consider elements on sligle linked list and new table when rehashing.
	
	// insert_hard
	{
		std::mt19937_64 mt(seed); // pseudo random number generator
		
		for(uint64 i=0; i<limitSize; i++){
			uint64 r = mt();
			hashT.insert_hard(r, r);
		}
		if(!( hashT.size()==limitSize )){ sstd::pdbg("ERROR: stressTest_oneCycle_soft\n"); sstd::printn( limitSize ); sstd::printn( hashT.size() ); return false; }
	}
//	printTable_all(hashT);
	
	// find
	{
		std::mt19937_64 mt(seed); // pseudo random number generator
		
		for(uint64 i=0; i<limitSize; i++){
			uint64 r = mt();
			auto itr = hashT.find(r);
			if(!( itr!=hashT.end() )){ sstd::pdbg("ERROR: stressTest_oneCycle_soft\n"); sstd::printn(r); return false; }
			if(!( itr.first() ==r  )){ sstd::pdbg("ERROR: stressTest_oneCycle_soft\n"); sstd::printn(r); return false; }
			if(!( itr.second()==r  )){ sstd::pdbg("ERROR: stressTest_oneCycle_soft\n"); sstd::printn(r); return false; }
		}
	}
	
	// erase
	{
		std::mt19937_64 mt(seed); // pseudo random number generator
		
		for(uint64 i=0; i<limitSize; i++){
			uint64 r = mt();
			hashT.erase(r);
		}
		if(!( hashT.size()==0 )){ sstd::pdbg("ERROR: stressTest_oneCycle_soft\n"); return false; }
	}
	return true;
}
TEST(sstd_IpCHashT, multiple_rehashing_padding0_soft){
	// test of multiple rehasing while rehasing (this is a case of 0 passings for initial table size)
	uint64 seed = 4163762552;
	uint64 limitSize = 100;
	bool ret = stressTest_oneCycle_soft(seed, limitSize);
	if(ret==false){ sstd::printn(seed); }
	ASSERT_TRUE( ret );
}
TEST(sstd_IpCHashT, stressTest_soft){
	// this is a stress test of chained hash table
	std::random_device rnd;
	for(uint limitSize=0; limitSize<10000; limitSize+=500){
//		for(uint i=0; i<1000; i++){
		for(uint i=0; i<10; i++){
			uint64 seed = rnd();
			bool ret = stressTest_oneCycle_soft(seed, limitSize);
			if(ret==false){ sstd::printn(seed); }
			ASSERT_TRUE( ret );
		}
	}
}
//---
bool stressTest_oneCycle_hard(const uint64 seed, const uint64 limitSize){
	sstd::IpCHashT<uint64, uint64> hashT(0);
	hashT.use_pSize_dbg = true; // enable debug option
	hashT.pSize_dbg     = 0;    // force to set key-val on the table index "tIdx_dbg".
	
//	uint64 limitSize = 42949672; // 42949672 elements == 1 G Byte or more
	// when bool is 1 bytes, one elements of "struct sstd_CHashT::element" is 25 bytes (== 1 + key8 + val8 + p8).
	// thus, 1 G Bytes table would have 42949672.96 (== 1*1024*1024*1024 Bytes / 25) elements in rough estimate.
	// in attention, we needs to consider elements on sligle linked list and new table when rehashing.
	
	// insert_hard
	{
		std::mt19937_64 mt(seed); // pseudo random number generator
		
		for(uint64 i=0; i<limitSize; i++){
			uint64 r = mt();
			hashT.insert_hard(r, r);
		}
		if(!( hashT.size()==limitSize )){ sstd::pdbg("ERROR: stressTest_oneCycle_hard\n"); sstd::printn( limitSize ); sstd::printn( hashT.size() ); return false; }
	}
//	printTable_all(hashT);
	
	// find
	{
		std::mt19937_64 mt(seed); // pseudo random number generator
		
		for(uint64 i=0; i<limitSize; i++){
			uint64 r = mt();
			auto itr = hashT.find(r);
			if(!( itr!=hashT.end() )){ sstd::pdbg("ERROR: stressTest_oneCycle_hard\n"); sstd::printn(r); return false; }
			if(!( itr.first() ==r  )){ sstd::pdbg("ERROR: stressTest_oneCycle_hard\n"); sstd::printn(r); return false; }
			if(!( itr.second()==r  )){ sstd::pdbg("ERROR: stressTest_oneCycle_hard\n"); sstd::printn(r); return false; }
		}
	}
	
	// erase
	{
		std::mt19937_64 mt(seed); // pseudo random number generator
		
		for(uint64 i=0; i<limitSize; i++){
			uint64 r = mt();
			hashT.erase(r);
		}
		if(!( hashT.size()==0 )){ sstd::pdbg("ERROR: stressTest_oneCycle_hard\n"); return false; }
	}
	return true;
}
TEST(sstd_IpCHashT, multiple_rehasing_padding0_hard){
	// test of multiple rehasing while rehasing (this is a case of 0 passings for initial table size)
	uint64 seed = 4163762552;
	uint64 limitSize = 100;
	bool ret = stressTest_oneCycle_hard(seed, limitSize);
	if(ret==false){ sstd::printn(seed); }
	ASSERT_TRUE( ret );
}
TEST(sstd_IpCHashT, stressTest_hard){
	// this is a stress test of chained hash table
	std::random_device rnd;
	for(uint limitSize=0; limitSize<10000; limitSize+=500){
//		for(uint i=0; i<1000; i++){
		for(uint i=0; i<10; i++){
			uint64 seed = rnd();
			bool ret = stressTest_oneCycle_hard(seed, limitSize);
			if(ret==false){ sstd::printn(seed); }
			ASSERT_TRUE( ret );
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
bool stressTest_oneCycle__failSafe_of_rehashing__hard(const uint64 seed, const uint64 limitSize){
	const uint64 testFSOR_size = 553;
	
	sstd::IpCHashT<uint64, uint64> hashT(0);
	hashT.use_pSize_dbg = true; // enable debug option
	hashT.pSize_dbg     = 0;    // force to set key-val on the table index "tIdx_dbg".
	
	// insert_hard
	{
		std::mt19937_64 mt(seed); // pseudo random number generator
		 
		for(uint64 i=0; i<limitSize; i++){
			uint64 r = mt();
			hashT.insert_hard(r, r);
			
			if(i==testFSOR_size){ hashT.use_testFSOR_dbg = true;  }
		}
		
		if(!( hashT.size()==limitSize )){ sstd::pdbg("ERROR: stressTest_oneCycle_hard\n"); sstd::printn( limitSize ); sstd::printn( hashT.size() ); return false; }
	}
//	printTable_all(hashT);
	
	// find
	{
		std::mt19937_64 mt(seed); // pseudo random number generator
		
		for(uint64 i=0; i<limitSize; i++){
			uint64 r = mt();
			auto itr = hashT.find(r);
			if(!( itr!=hashT.end() )){ sstd::pdbg("ERROR: stressTest_oneCycle_hard\n"); sstd::printn(r); return false; }
			if(!( itr.first() ==r  )){ sstd::pdbg("ERROR: stressTest_oneCycle_hard\n"); sstd::printn(r); return false; }
			if(!( itr.second()==r  )){ sstd::pdbg("ERROR: stressTest_oneCycle_hard\n"); sstd::printn(r); return false; }
		}
	}
	
	// erase
	{
		std::mt19937_64 mt(seed); // pseudo random number generator
		
		for(uint64 i=0; i<limitSize; i++){
			uint64 r = mt();
			hashT.erase(r);
		}
		if(!( hashT.size()==0 )){ sstd::pdbg("ERROR: stressTest_oneCycle_hard\n"); return false; }
	}
	return true;
}
TEST(sstd_IpCHashT, stressTest__failSafe_of_rehashing__hard){
	// this is a stress test of chained hash table
	std::random_device rnd;
	for(uint limitSize=0; limitSize<10000; limitSize+=500){
		for(uint i=0; i<1000; i++){
//		for(uint i=0; i<10; i++){
			uint64 seed = rnd();
			bool ret = stressTest_oneCycle__failSafe_of_rehashing__hard(seed, limitSize);
			if(ret==false){ sstd::printn(seed); }
			ASSERT_TRUE( ret );
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

uint64 size2interval(const uint64 size){
	uint8 order = (uint8)std::log10((double)size);
	uint64 interval = (uint64)std::pow(10, order);
	if      (order<1){ return interval;
	}else if(order<2){ return interval/2;
	}else if(order<3){ return interval/4;
	}else if(order<5){ return interval/10;
	}else            { return interval/10;
	}
}
template<typename T_hashTable>
void test_bench_find(T_hashTable& hashT, const uint64 limitSize, uint64 seed1, uint64 seed2){
//	std::random_device seed_gen;
	std::mt19937_64 rand       (seed1); // pseudo random number generator
	std::mt19937_64 rand_toFind(seed2); // pseudo random number generator
	
	uint64 interval = 1;
	std::vector<uint64> vecR(limitSize); vecR.clear();
	
	uint64 numFound=0ull, numNotFound=0ull;
	uint64 count=0ull; // dor dbg
	for(;;){
		// insert
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			vecR <<= r;
			hashT[r] = r;
		}
		
		std::vector<uint64> vecR_toFind(vecR.size());
		{
			std::uniform_int_distribution<uint64> range(0, vecR.size()-1); // make randome number between [0, vecR.size()-1].
			for(uint i=0; i<interval; i++){
				vecR_toFind[i] = vecR[range(rand_toFind)];
			}
		}
		
		// find (all elements are found)
		for(uint i=0; i<interval; i++){
			auto itr = hashT.find( vecR_toFind[i] );
			count++;
			if(itr!=hashT.end()){
				numFound++;
			}else{
				numNotFound++;
				printf("ERROR: count: %lu\n", count);
			}
		}
		
		interval = size2interval(hashT.size());
		if(hashT.size()+interval>limitSize){ break; }
	}
	if(numNotFound!=0ull){
		printf("ERROR: ");
		printf("%lu / %lu = %lf\n", numFound, numFound+numNotFound, (double)numFound/(double)(numFound+numNotFound));
	}
}
TEST(sstd_IpCHashT, stressTest__for__maxLF50){
	std::random_device seed_gen;
//	uint64 seed1 = seed_gen();
//	uint64 seed2 = seed_gen();
	uint64 seed1 = 1276401658;
	uint64 seed2 = 1055862328;
	printf("seed1: %lu, seed2: %lu\n", seed1, seed2);
	
	const uint64 initSize_wRehash  = 0ull;
//	const uint64 limitSize = 5000000;
	const uint64 limitSize = 100;
	
	sstd::IpCHashT<uint64,uint64,std::hash<uint64>,std::equal_to<uint64>,uint8,  sstd::IpCHashT_opt::maxLF50> hashT(initSize_wRehash);
//	std::vector<double> vecX, vecY;
//	test_bench_find(hashT, limitSize, vecX,  vecY );
	test_bench_find(hashT, limitSize, seed1, seed2);
}
/*
seed1: 2781098060, seed2: 2899935815
ERROR: count: 30
ERROR: count: 822
ERROR: count: 1860
ERROR: count: 274265
ERROR: count: 627963
ERROR: count: 1402998
ERROR: count: 1512209
ERROR: 4999993 / 5000000 = 0.999999
*/
/*
seed1: 1276401658, seed2: 1055862328
ERROR: count: 57
ERROR: count: 63
ERROR: count: 203
ERROR: count: 550
ERROR: count: 41484
ERROR: 4999995 / 5000000 = 0.999999
*/
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// under construction

//-----------------------------------------------------------------------------------------------------------------------------------------------
TEST(sstd_IpCHashT, OPE_bracket){
	// []
	
	{
		sstd::IpCHashT<uint64, uint64> hashT;
		hashT[1] = 10;         // insert()
		
		auto itr = hashT.find(1);
		ASSERT_TRUE( itr!=hashT.end() );
		uint64 ret = hashT[1]; // find()
		ASSERT_TRUE( ret==10 );
	}
	{
		sstd::IpCHashT<uint64, uint64> hashT;
		uint64 ret = hashT[1]; // insert() T_key, init emply T_val and return empty T_val.
		ret++; // avoiding "warning: unused variable ‘ret’"
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------




















