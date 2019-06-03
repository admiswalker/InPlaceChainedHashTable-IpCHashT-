#include <random>
#include "./CHashT.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------
// definition of the user hash function

namespace usrDef_in_CHashT{ class hashFn; }
class usrDef_in_CHashT::hashFn{
private:
public:
	hashFn(){}
	~hashFn(){}
	size_t operator()(const uint64& key){ return key; }
};
TEST(sstd_CHashT, use_usr_defined_hash_func){
	sstd::CHashT<uint64, uint64, usrDef_in_CHashT::hashFn> hashT(10);
	auto
	itr = hashT.insert(1, 10); ASSERT_TRUE( itr!=hashT.end() );
	itr = hashT.find(1); ASSERT_TRUE( itr!=hashT.end() ); ASSERT_TRUE( itr.first()==1 ); ASSERT_TRUE( itr.second()==10 );
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

TEST(sstd_CHashT, insert){
	sstd::CHashT<uint64, uint64> hashT(100);
	
	hashT.use_tIdx_dbg = true; // enable debug option
	hashT.tIdx_dbg     = 0;    // force to set key-val on the table index "tIdx_dbg".
	
	// case 1. insertion of key (there is no conflict)
	auto
	itr = hashT.insert( 1, 10); ASSERT_TRUE( hashT.pT_dbg()[0].key==1 ); ASSERT_TRUE( hashT.pT_dbg()[0].val==10 );
	
	// case 2. insertion of key solving the conflict of tIdx. (table index (tIdx) is a modulation of hash value).
	itr = hashT.insert( 2, 20); ASSERT_TRUE( hashT.pT_dbg()[0].pNext->key==2        ); ASSERT_TRUE( hashT.pT_dbg()[0].pNext->val==20        );
	itr = hashT.insert( 3, 30); ASSERT_TRUE( hashT.pT_dbg()[0].pNext->pNext->key==3 ); ASSERT_TRUE( hashT.pT_dbg()[0].pNext->pNext->val==30 );
	
	// case 3. confliction of key value
	//   type 1. on the table.
	itr = hashT.insert( 1, 99); ASSERT_TRUE( itr.key()==1 ); ASSERT_TRUE( itr.val()==10 );
	//   type 2. on the singly linked list.
	itr = hashT.insert( 2, 99); ASSERT_TRUE( itr.key()==2 ); ASSERT_TRUE( itr.val()==20 );
	itr = hashT.insert( 3, 99); ASSERT_TRUE( itr.key()==3 ); ASSERT_TRUE( itr.val()==30 );
}
TEST(sstd_CHashT, insert_f){
	sstd::CHashT<uint64, uint64> hashT(100);
	
	hashT.insert( 1, 10);
	auto
	itr = hashT.find(1); ASSERT_TRUE( itr.key()==1 ); ASSERT_TRUE( itr.val()==10 );
	
	itr = hashT.find(1); ASSERT_TRUE( itr!=hashT.end() );
	if(itr!=hashT.end()){ itr.val()=20; } // over write
	itr = hashT.find(1); ASSERT_TRUE( itr.key()==1 ); ASSERT_TRUE( itr.val()==20 );
	
	// insert by itr
	itr = hashT.find(4); ASSERT_TRUE(!(itr!=hashT.end()));
	hashT.insert(itr, 4); itr.val() = 99;
	{ auto itr = hashT.find(4); ASSERT_TRUE(itr.val()==99); }
}
TEST(sstd_CHashT, find){
	// case 1. when key is on the table
	{
		sstd::CHashT<uint64, uint64> hashT(1024);
		//        key,  val
		hashT.insert(123, 1230);
		
		auto itr = hashT.find(123); ASSERT_TRUE( itr!=hashT.end() ); ASSERT_TRUE( itr.val()==1230 );
	}
	
	// case 2. when key is on a singly linked list
	{
		sstd::CHashT<uint64, uint64> hashT(1024);
		
		hashT.use_tIdx_dbg = true;
		hashT.tIdx_dbg     = 0;
		//        key, val
		hashT.insert(1, 10);
		hashT.insert(2, 20);
		hashT.insert(3, 30);
		hashT.insert(4, 40);
		hashT.insert(5, 50);
		
		auto
		itr = hashT.find(1); ASSERT_TRUE( itr!=hashT.end() ); ASSERT_TRUE( itr.val()==10 );
		itr = hashT.find(2); ASSERT_TRUE( itr!=hashT.end() ); ASSERT_TRUE( itr.val()==20 );
		itr = hashT.find(3); ASSERT_TRUE( itr!=hashT.end() ); ASSERT_TRUE( itr.val()==30 );
		itr = hashT.find(4); ASSERT_TRUE( itr!=hashT.end() ); ASSERT_TRUE( itr.val()==40 );
		itr = hashT.find(5); ASSERT_TRUE( itr!=hashT.end() ); ASSERT_TRUE( itr.val()==50 );
		itr = hashT.find(6); ASSERT_TRUE(!(itr!=hashT.end()));
	}
}
TEST(sstd_CHashT, sstd_CHashT_operator){
	sstd::CHashT<uint64, uint64> hashT(100);
	auto itr = sstd_CHashT::iterator<uint64, uint64>();
	
	itr = hashT.insert( 1, 10); ASSERT_TRUE( itr!=hashT.end() );
	
	// operator !=(const class itr_m& rhs). "inline bool operator!=(const class itr_m& rhs){ return this->TF != false; }"
	itr = hashT.find(1); ASSERT_TRUE( itr!=hashT.end() );
}
TEST(sstd_CHashT, iterator){
	sstd::CHashT<uint64, uint64> hashT(30);
	hashT.use_tIdx_dbg = true;
	
	hashT.tIdx_dbg = 0;
	hashT.insert(1, 10);
	
	hashT.tIdx_dbg = 5;
	hashT.insert(2, 20);
	hashT.insert(3, 30);
	hashT.insert(4, 40);
	
	hashT.tIdx_dbg = 7;
	hashT.insert(5, 50);
	hashT.tIdx_dbg = 8;
	hashT.insert(6, 60);
	
	hashT.tIdx_dbg = 20;
	hashT.insert(7, 70);
	
	printf("■ print internal value of iterator\n");
	auto itr=hashT.begin();
	itr.print_dbg();
	printf("\n");
	itr._pElem()->print_dbg();
	printf("\n");
	
	printf("■ print const-key and non-const-value of element\n");
	uint i=0;
	for(auto itr=hashT.begin(); itr!=hashT.end(); ++itr){
		itr.print_dbg();
		sstd::printn(itr.key());
		sstd::printn(itr.val());
		printf("\n");
		i++;
	}
	ASSERT_TRUE( i==7 );
	
	//---
	
	// erase all
	for(auto itr=hashT.begin(); itr!=hashT.end(); ++itr){
		hashT.erase(itr);
	}
	ASSERT_TRUE( hashT.size()==0ull );
	
}
TEST(sstd_CHashT, erase){
	
	// case 1. erase an element on the table without singly linked list.
	{
		sstd::CHashT<uint64, uint64> hashT(100);
		
		auto itr = hashT.insert( 1, 10);
		ASSERT_TRUE( hashT.erase(1)==true );
		itr = hashT.find(1); ASSERT_TRUE( !(itr!=hashT.end()) );
	}
	
	// case 2. erase an element on the table with singly linked list.
	{
		sstd::CHashT<uint64, uint64> hashT(100);
		
		hashT.use_tIdx_dbg = true; // enable debug option
		hashT.tIdx_dbg     = 0;    // force to set key-val on the table index "tIdx_dbg".
		
		auto
		itr = hashT.insert( 1, 10);
		itr = hashT.insert( 2, 20);
		itr = hashT.insert( 3, 30);
		itr = hashT.insert( 4, 40);
		
		ASSERT_TRUE( hashT.erase(1)==true );
		itr = hashT.find(1); ASSERT_TRUE( !(itr!=hashT.end()) );
		
		ASSERT_TRUE( hashT.pT_dbg()[0].isUsed==true );
		ASSERT_TRUE( hashT.pT_dbg()[0].key==2       );
		ASSERT_TRUE( hashT.pT_dbg()[0].val==20      );
		
		ASSERT_TRUE( hashT.pT_dbg()[0].pNext->isUsed==true );
		ASSERT_TRUE( hashT.pT_dbg()[0].pNext->key==3       );
		ASSERT_TRUE( hashT.pT_dbg()[0].pNext->val==30      );
		ASSERT_TRUE( hashT.pT_dbg()[0].pNext->pNext!=0ull );
		
		ASSERT_TRUE( hashT.pT_dbg()[0].pNext->pNext->isUsed==true );
		ASSERT_TRUE( hashT.pT_dbg()[0].pNext->pNext->key==4       );
		ASSERT_TRUE( hashT.pT_dbg()[0].pNext->pNext->val==40      );
		ASSERT_TRUE( hashT.pT_dbg()[0].pNext->pNext->pNext==0ull );
	}
	
	// case 3. erase element on the singly linked list
	{
		sstd::CHashT<uint64, uint64> hashT(100);
		
		hashT.use_tIdx_dbg = true; // enable debug option
		hashT.tIdx_dbg     = 0;    // force to set key-val on the table index "tIdx_dbg".
		
		auto
		itr = hashT.insert( 1, 10);
		itr = hashT.insert( 2, 20);
		itr = hashT.insert( 3, 30);
		itr = hashT.insert( 4, 40);
		
		ASSERT_TRUE( hashT.erase(3)==true );
		itr = hashT.find(1); ASSERT_TRUE( itr!=hashT.end() );
		itr = hashT.find(2); ASSERT_TRUE( itr!=hashT.end() );
		itr = hashT.find(3); ASSERT_TRUE(!( itr!=hashT.end() ));
		itr = hashT.find(4); ASSERT_TRUE( itr!=hashT.end() );
		
		ASSERT_TRUE( hashT.erase(4)==true );
		itr = hashT.find(1); ASSERT_TRUE( itr!=hashT.end() );
		itr = hashT.find(2); ASSERT_TRUE( itr!=hashT.end() );
		itr = hashT.find(3); ASSERT_TRUE(!( itr!=hashT.end() ));
		itr = hashT.find(4); ASSERT_TRUE(!( itr!=hashT.end() ));
	}
}
TEST(sstd_CHashT, erase_byItr){
	// find key-val pair, get value and erase the pair
	
	sstd::CHashT<uint64, uint64> hashT(100);
	
	auto
	itr = hashT.insert( 1, 10);  ASSERT_TRUE( itr!=hashT.end() );
	itr = hashT.find(1);         ASSERT_TRUE( itr!=hashT.end() );
	bool ret = hashT.erase(itr); ASSERT_TRUE( ret==true );
	itr = hashT.find(1);         ASSERT_TRUE(!( itr!=hashT.end() ));
}
TEST(sstd_CHashT, rehash){
	uint64 seed=12345ull;     // using a constant value in order to provide reproducibility.
	std::mt19937_64 mt(seed); // pseudo random number generator
	
	uint64 limitSize = 1031; // 7th value of 'tSizeL' (table size list).
	
	sstd::CHashT<uint64, uint64> hashT(0);
	
	{
		for(uint i=0; i<limitSize; i++){
			uint64 r = mt();
			hashT.insert(r, r);
		}
		ASSERT_TRUE( hashT.size() ==1031 );
//		ASSERT_TRUE( hashT.tableSize()==1031 ); // this test is for prime table.
	}
	{
		uint64 r = mt();
		hashT.insert(r, r); // rehash will be occurd
		ASSERT_TRUE( hashT.size() ==1032 );
//		ASSERT_TRUE( hashT.tableSize()==2053 ); // this test is for prime table.
	}
}
TEST(sstd_CHashT, stressTest){
	// this is a stress test of chained hash table
	
	uint64 seed=12345ull; // using a constant value in order to provide reproducibility.
	
	sstd::CHashT<uint64, uint64> hashT;
	uint64 limitSize = 543210;
//	uint64 limitSize = 42949672; // 42949672 elements == 1 G Byte or more
	// when bool is 1 bytes, one elements of "struct sstd_CHashT::element" is 25 bytes (== 1 + key8 + val8 + p8).
	// thus, 1 G Bytes table would have 42949672.96 (== 1*1024*1024*1024 Bytes / 25) elements in rough estimate.
	// in attention, we needs to consider elements on sligle linked list and new table when rehashing.
	
	// add
	{
		std::mt19937_64 mt(seed); // pseudo random number generator
		
		for(uint64 i=0; i<limitSize; i++){
			uint64 r = mt();
			hashT.insert(r, r);
		}
		ASSERT_TRUE( hashT.size()==limitSize );
	}
	
	// find
	{
		std::mt19937_64 mt(seed); // pseudo random number generator
		
		for(uint64 i=0; i<limitSize; i++){
			uint64 r = mt();
			auto itr = hashT.find(r);
			ASSERT_TRUE( itr!=hashT.end() );
			ASSERT_TRUE( itr.key()==r );
			ASSERT_TRUE( itr.val()==r );
		}
	}
	
	// erase
	{
		std::mt19937_64 mt(seed); // pseudo random number generator
		
		for(uint64 i=0; i<limitSize; i++){
			uint64 r = mt();
			ASSERT_TRUE( hashT.erase(r)==true );
		}
		ASSERT_TRUE( hashT.size()==0 );
	}
}
TEST(sstd_CHashT, OPE_bracket){
	// []
	
	{
		sstd::CHashT<uint64, uint64> hashT;
		hashT[1] = 10;         // insert()
		
		auto itr = hashT.find(1);
		ASSERT_TRUE( itr!=hashT.end() );
		uint64 ret = hashT[1]; // find()
		ASSERT_TRUE( ret==10 );
	}
	{
		sstd::CHashT<uint64, uint64> hashT;
		uint64 ret = hashT[1]; // insert() T_key, init emply T_val and return empty T_val.
		ret++; // avoiding "warning: unused variable ‘ret’"
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

