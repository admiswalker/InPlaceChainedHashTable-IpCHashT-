#pragma once
#include "./typeDef.h"
#include <memory>
#include <assert.h> // for debug
#include <sstd/sstd.hpp> // for debug

//-----------------------------------------------------------------------------------------------------------------------------------------------
// compile options

// select soft or hard
#define use_insert_soft
//#define use_insert_hard

//#define use_prime_table

//-----------------------------------------------------------------------------------------------------------------------------------------------

#define elem_m    sstd_IpCHashT::element       <T_key,T_val,T_shift> // a macro of table element structure
#define elem_KV_m sstd_IpCHashT::element_KeyVal<T_key,T_val>         // a macro of table element structure
#define itr_m     sstd_IpCHashT::iterator      <T_key,T_val,T_shift> // a macro of iterator

//-----------------------------------------------------------------------------------------------------------------------------------------------

namespace sstd{
	template <class T_key,
			  class T_val,
			  class T_hash   = std::hash<T_key>,
			  class T_key_eq = std::equal_to<T_key>,
			  typename T_shift = uint8
//			  typename T_shift = uint16
			  >
	class IpCHashT; // chained hash table
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

namespace sstd_IpCHashT{
	template <class T_key, class T_val, typename T_shift> struct element;
	template <class T_key, class T_val>                   struct element_KeyVal;
	template <class T_key, class T_val, typename T_shift> struct iterator;
	
	#ifdef use_prime_table
	const uint64 tSizeL[64] = { // table size list. (Smallest prime list larger than power of 2.)
		2ull,                   // 2^ 1 + 0   = 2ull
		5ull,                   // 2^ 2 + 1   = 5ull
		11ull,                  // 2^ 3 + 3   = 11ull
		17ull,                  // 2^ 4 + 1   = 17ull
		37ull,                  // 2^ 5 + 5   = 37ull
		67ull,                  // 2^ 6 + 3   = 67ull
		131ull,                 // 2^ 7 + 3   = 131ull
		257ull,                 // 2^ 8 + 1   = 257ull
		521ull,                 // 2^ 9 + 9   = 521ull
		1031ull,                // 2^10 + 7   = 1031ull
		2053ull,                // 2^11 + 5   = 2053ull
		4099ull,                // 2^12 + 3   = 4099ull
		8209ull,                // 2^13 + 17  = 8209ull
		16411ull,               // 2^14 + 27  = 16411ull
		32771ull,               // 2^15 + 3   = 32771ull
		65537ull,               // 2^16 + 1   = 65537ull
		131101ull,              // 2^17 + 29  = 131101ull
		262147ull,              // 2^18 + 3   = 262147ull
		524309ull,              // 2^19 + 21  = 524309ull
		1048583ull,             // 2^20 + 7   = 1048583ull
		2097169ull,             // 2^21 + 17  = 2097169ull
		4194319ull,             // 2^22 + 15  = 4194319ull
		8388617ull,             // 2^23 + 9   = 8388617ull
		16777259ull,            // 2^24 + 43  = 16777259ull
		33554467ull,            // 2^25 + 35  = 33554467ull
		67108879ull,            // 2^26 + 15  = 67108879ull
		134217757ull,           // 2^27 + 29  = 134217757ull
		268435459ull,           // 2^28 + 3   = 268435459ull
		536870923ull,           // 2^29 + 11  = 536870923ull
		1073741827ull,          // 2^30 + 85  = 1073741827ull
		2147483659ull,          // 2^31 + 11  = 2147483659ull
		4294967311ull,          // 2^32 + 15  = 4294967311ull
		8589934609ull,          // 2^33 + 17  = 8589934609ull
		17179869209ull,         // 2^34 + 25  = 17179869209ull
		34359738421ull,         // 2^35 + 53  = 34359738421ull
		68719476767ull,         // 2^36 + 31  = 68719476767ull
		137438953481ull,        // 2^37 + 9   = 137438953481ull
		274877906951ull,        // 2^38 + 7   = 274877906951ull
		549755813911ull,        // 2^39 + 23  = 549755813911ull
		1099511627791ull,       // 2^40 + 15  = 1099511627791ull
		2199023255579ull,       // 2^41 + 27  = 2199023255579ull
		4398046511119ull,       // 2^42 + 15  = 4398046511119ull
		8796093022237ull,       // 2^43 + 29  = 8796093022237ull
		17592186044423ull,      // 2^44 + 7   = 17592186044423ull
		35184372088891ull,      // 2^45 + 59  = 35184372088891ull
		70368744177679ull,      // 2^46 + 15  = 70368744177679ull
		140737488355333ull,     // 2^47 + 5   = 140737488355333ull
		281474976710677ull,     // 2^48 + 21  = 281474976710677ull
		562949953421381ull,     // 2^49 + 69  = 562949953421381ull
		1125899906842679ull,    // 2^50 + 55  = 1125899906842679ull
		2251799813685269ull,    // 2^51 + 21  = 2251799813685269ull
		4503599627370517ull,    // 2^52 + 21  = 4503599627370517ull
		9007199254740997ull,    // 2^53 + 5   = 9007199254740997ull
		18014398509482143ull,   // 2^54 + 159 = 18014398509482143ull
		36028797018963971ull,   // 2^55 + 3   = 36028797018963971ull
		72057594037928017ull,   // 2^56 + 81  = 72057594037928017ull
		144115188075855881ull,  // 2^57 + 9   = 144115188075855881ull
		288230376151711813ull,  // 2^58 + 69  = 288230376151711813ull
		576460752303423619ull,  // 2^59 + 131 = 576460752303423619ull
		1152921504606847009ull, // 2^60 + 33  = 1152921504606847009ull
		2305843009213693967ull, // 2^61 + 15  = 2305843009213693967ull
		4611686018427388039ull, // 2^62 + 135 = 4611686018427388039ull
		9223372036854775837ull, // 2^63 + 29  = 9223372036854775837ull
	//	18446744073709551629ull // 2^64 + 13  = 18446744073709551629ull // larger than the range of uint64
		18446744073709551360ull	// 2^64 - 254 = 18446744073709551360ull // not prive (need to find out an appropriate prime)
	};
	#else
	const uint64 tSizeL[64] = { // table size list. (Smallest prime list larger than power of 2.)
		1ull,                    // 2^  1 - 1   = 1ull
		3ull,                    // 2^  2 - 1   = 3ull
		7ull,                    // 2^  3 - 1   = 7ull
		15ull,                   // 2^  4 - 1   = 15ull
		31ull,                   // 2^  5 - 1   = 31ull
		63ull,                   // 2^  6 - 1   = 63ull
		127ull,                  // 2^  7 - 1   = 127ull
		255ull,                  // 2^  8 - 1   = 255ull
		511ull,                  // 2^  9 - 1   = 511ull
		1023ull,                 // 2^ 10 - 1   = 1023ull
		2047ull,                 // 2^ 11 - 1   = 2047ull
		4095ull,                 // 2^ 12 - 1   = 4095ull
		8191ull,                 // 2^ 13 - 1   = 8191ull
		16383ull,                // 2^ 14 - 1   = 16383ull
		32767ull,                // 2^ 15 - 1   = 32767ull
		65535ull,                // 2^ 16 - 1   = 65535ull
		131071ull,               // 2^ 17 - 1   = 131071ull
		262143ull,               // 2^ 18 - 1   = 262143ull
		524287ull,               // 2^ 19 - 1   = 524287ull
		1048575ull,              // 2^ 20 - 1   = 1048575ull
		2097151ull,              // 2^ 21 - 1   = 2097151ull
		4194303ull,              // 2^ 22 - 1   = 4194303ull
		8388607ull,              // 2^ 23 - 1   = 8388607ull
		16777215ull,             // 2^ 24 - 1   = 16777215ull
		33554431ull,             // 2^ 25 - 1   = 33554431ull
		67108863ull,             // 2^ 26 - 1   = 67108863ull
		134217727ull,            // 2^ 27 - 1   = 134217727ull
		268435455ull,            // 2^ 28 - 1   = 268435455ull
		536870911ull,            // 2^ 29 - 1   = 536870911ull
		1073741823ull,           // 2^ 30 - 1   = 1073741823ull
		2147483647ull,           // 2^ 31 - 1   = 2147483647ull
		4294967295ull,           // 2^ 32 - 1   = 4294967295ull
		8589934591ull,           // 2^ 33 - 1   = 8589934591ull
		17179869183ull,          // 2^ 34 - 1   = 17179869183ull
		34359738367ull,          // 2^ 35 - 1   = 34359738367ull
		68719476735ull,          // 2^ 36 - 1   = 68719476735ull
		137438953471ull,         // 2^ 37 - 1   = 137438953471ull
		274877906943ull,         // 2^ 38 - 1   = 274877906943ull
		549755813887ull,         // 2^ 39 - 1   = 549755813887ull
		1099511627775ull,        // 2^ 40 - 1   = 1099511627775ull
		2199023255551ull,        // 2^ 41 - 1   = 2199023255551ull
		4398046511103ull,        // 2^ 42 - 1   = 4398046511103ull
		8796093022207ull,        // 2^ 43 - 1   = 8796093022207ull
		17592186044415ull,       // 2^ 44 - 1   = 17592186044415ull
		35184372088831ull,       // 2^ 45 - 1   = 35184372088831ull
		70368744177663ull,       // 2^ 46 - 1   = 70368744177663ull
		140737488355327ull,      // 2^ 47 - 1   = 140737488355327ull
		281474976710655ull,      // 2^ 48 - 1   = 281474976710655ull
		562949953421311ull,      // 2^ 49 - 1   = 562949953421311ull
		1125899906842623ull,     // 2^ 50 - 1   = 1125899906842623ull
		2251799813685247ull,     // 2^ 51 - 1   = 2251799813685247ull
		4503599627370495ull,     // 2^ 52 - 1   = 4503599627370495ull
		9007199254740991ull,     // 2^ 53 - 1   = 9007199254740991ull
		18014398509481983ull,    // 2^ 54 - 1   = 18014398509481983ull
		36028797018963967ull,    // 2^ 55 - 1   = 36028797018963967ull
		72057594037927935ull,    // 2^ 56 - 1   = 72057594037927935ull
		144115188075855871ull,   // 2^ 57 - 1   = 144115188075855871ull
		288230376151711743ull,   // 2^ 58 - 1   = 288230376151711743ull
		576460752303423487ull,   // 2^ 59 - 1   = 576460752303423487ull
		1152921504606846975ull,  // 2^ 60 - 1   = 1152921504606846975ull
		2305843009213693951ull,  // 2^ 61 - 1   = 2305843009213693951ull
		4611686018427387903ull,  // 2^ 62 - 1   = 4611686018427387903ull
		9223372036854775807ull,  // 2^ 63 - 1   = 9223372036854775807ull
//		18446744073709551615ull  // 2^ 64 - 1   = 18446744073709551615ull
		18446744073709486080ull  // 2^ 64 - 1 - 65535 = 18446744073709551615ull - 65535 = 18446744073709486080ull

//		   18446744073709551615
//		-)                65535
//		-----------------------
//		   18446744073709000000
//		+)               551615
//		-)                65535
//		-----------------------
//		   18446744073709000000
//		+)               486080
//		-----------------------
//		   18446744073709486080
		
	};
	#endif
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, typename T_shift>
struct sstd_IpCHashT::element{ // defined as elem_m
private:
public:
	element(){
		T_shift maxShift=(T_shift)0; maxShift=~maxShift; // 'maxShift' indicates that the array is empty.
		prev = maxShift;
		next = (T_shift)0;
	}
	~element(){}
	
	T_key key;    // key
	T_val val;    // value
	T_shift prev; // A shift number to the previous element. Or, the head of in-place chain while 'prev==0'. Or, empty element while 'prev==t_shift_max' (while "T_shift t_shift_max=(T_shift)0; t_shift_max=~t_shift_max;").
	T_shift next; // A shift number to the next element. Only right shift is allowed. 'next==0' means myself.
};

// definition for rehash
template <class T_key, class T_val>
struct sstd_IpCHashT::element_KeyVal{ // defined as elem_KV_m
private:
public:
	element_KeyVal(){}
	element_KeyVal(T_key&& key_in, T_val&& val_in){
		key = std::move(key_in);
		val = std::move(val_in);
	}
	~element_KeyVal(){}
	
	T_key key;    // key
	T_val val;    // value
};

//-----------------------------------------------------------------------------------------------------------------------------------------------

// in order to reduce the calling time of function, macro expansion will be used.
#define isEmpty_m(ELEM)							\
	(ELEM.prev==maxShift)
#define isHead_m(ELEM)							\
	(ELEM.prev==(T_shift)0)
#define isTail_m(ELEM)							\
	(ELEM.next==(T_shift)0)
#define seek2emptyIndex_m(pT, idx, idx_last)							\
	for(;;){															\
		if( idx>=ttSize || idx>=idx_last+seekLimit ){ return itr_m(maxShift, ttSize, pT, itr_needRehash_m); } \
		if( isEmpty_m(pT[idx]) ){ break; }								\
		idx++;															\
	}
#define seek2tail(pT, idx)												\
	for(;; idx+=pT[idx].next){											\
		if( isTail_m(pT[idx]) ){ break; }								\
	}

#ifdef use_prime_table
	#ifdef SSTD_IpCHashT_DEBUG
	#define key2tableIdx_m(tIdx, key)										\
		uint64 hVal = (uint64)(*pHashFn)(key); /* generate hashed value  */	\
		tIdx = hVal % tSize;												\
		if(use_tIdx_dbg){ idx=tIdx_dbg; } /* over write tIdx for debug */
	#else
	#define key2tableIdx_m(tIdx, key)										\
		uint64 hVal = (uint64)(*pHashFn)(key); /* generate hashed value  */	\
		tIdx = hVal % tSize;
	#endif
#else
	#ifdef SSTD_IpCHashT_DEBUG
	#define key2tableIdx_m(tIdx, key)										\
		uint64 hVal = (uint64)(*pHashFn)(key); /* generate hashed value  */	\
		tIdx = hVal & tSize;												\
		if(use_tIdx_dbg){ idx=tIdx_dbg; } /* over write tIdx for debug */
	#else
	#define key2tableIdx_m(tIdx, key)										\
		uint64 hVal = (uint64)(*pHashFn)(key); /* generate hashed value  */	\
		tIdx = hVal & tSize;
	#endif
#endif

#ifdef use_prime_table
	#define key2tableIdx_wDivisor_m(tIdx, key, div) /* with divisor */		\
		uint64 hVal = (uint64)(*pHashFn)(key); /* generate hashed value  */	\
		tIdx = hVal % div;
#else
	#define key2tableIdx_wDivisor_m(tIdx, key, div) /* with divisor */		\
		uint64 hVal = (uint64)(*pHashFn)(key); /* generate hashed value  */	\
		tIdx = hVal & div;
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------

#define itr_end_m        0xFFFFFFFFFFFFFFFF
#define itr_needRehash_m 0xFFFFFFFFFFFFFFFE

template <class T_key, class T_val, typename T_shift>
struct sstd_IpCHashT::iterator{ // defined as itr_m
private:
	const T_shift maxShift;
	const uint64 ttSize;
	
	struct elem_m* pT; // pointer of element
	uint64 idx; // when idx==0xFFFFFFFFFFFFFFFF: idx is end flag.
	            // when idx==0xFFFFFFFFFFFFFFFE: rehash flag.
	
public:
	inline iterator(){}
	inline iterator(const T_shift maxShift_in, const uint64 totalTableSize, struct elem_m* pT_in, const uint64 idx_in) : maxShift(maxShift_in), ttSize(totalTableSize) { pT=pT_in; idx=idx_in; }
	inline ~iterator(){}
	
	inline const T_key & first    () const { return pT[idx].key; }
	inline       T_key & first_RW () const { return pT[idx].key; }
	inline const T_val & second   () const { return pT[idx].val; }
	inline       T_val & second_RW()       { return pT[idx].val; }
	inline const uint64& index    () const { return idx; }
	
	inline bool _needRehash(){ return idx==itr_needRehash_m; }
	
	// operators
	inline const bool operator!=(const struct itr_m& rhs){ return this->idx != rhs.index(); }
	
	// pre-increment (++itr)
	inline struct itr_m operator++(){
		idx++;
		for(; idx<ttSize; idx++){
			if(! isEmpty_m(pT[idx]) ){ return *this; }
		}
		idx=itr_end_m;
		return *this;
	}
	
	// post-increment (itr++)
//	class itr_m operator++(int){ // int is a dummy arg
//		== not implimented yet. ==
//	}
	
	#ifdef SSTD_IpCHashT_DEBUG
	inline void print_dbg(){
		std::cout << " idx: " <<          idx << std::endl;
		std::cout << " key: " << pT[idx].key  << std::endl;
		std::cout << " val: " << pT[idx].val  << std::endl;
		std::cout << "prev: " << pT[idx].prev << std::endl;
		std::cout << "next: " << pT[idx].next << std::endl;
	}
	#endif
};

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
class sstd::IpCHashT{
private:
	uint8  tSizeL_idx;      // table size list index
	uint64 tSize;           // table size. This is a hash value division size.
	uint64 pSize;           // padding size of the table
	uint64 ttSize;          // total table size. This is a seek limit size. ( ttSize == tSize + pSize ).
	uint64 elems;           // number of elements on the table
	T_hash* pHashFn;        // pointer to the hash function
	struct elem_m* pT;      // pointer to the table
//	double lf;             // load factor
	
	// constant values
	T_shift maxShift;
	T_shift seekLimit;
	
	uint64 get_tSizeL_idx(const uint64& tableSize); // get table size list index
	void IpCHashT_constructor(const uint64& tableSize);
	
	void move_hashT2vecKV(std::vector<elem_KV_m>& vecKV, sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>& hashT);
	void failSafe_of_rehashing(sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>& hashT_new);
	
public:
	IpCHashT();
	IpCHashT(const uint64& tableSize); // allocate twice size of tableSize.
	~IpCHashT();
	
	inline uint8& _tSizeL_idx(){ return tSizeL_idx; }
	inline uint64& _tSize(){ return tSize; }
	inline uint64& _pSize(){ return pSize; }
	inline uint64& _ttSize(){ return ttSize; }
	inline uint64& _elems (){ return elems; }
	inline T_hash*& _pHashFn(){ return pHashFn; }
	inline struct elem_m*& _pT(){ return pT; }
	inline const T_shift& _maxShift() const { return maxShift; }
	inline const T_shift& _seekLimit() const { return seekLimit; }
	
	inline struct itr_m begin() const {
		uint64 idx=0ull;
		for(; idx<ttSize; idx++){
			if(! isEmpty_m(pT[idx]) ){ return itr_m(maxShift, ttSize, pT, idx); }
		}
		return itr_m(maxShift, ttSize, pT, itr_end_m);
	}
	inline struct itr_m end() const { return itr_m(maxShift, ttSize, pT, itr_end_m); }
	inline const uint64&      size(){ return elems; }
	inline const uint64& tableSize(){ return tSize; }
	
	void rehash();
	
	struct itr_m find(const T_key& key_in, uint64 idx);
	struct itr_m find(const T_key& key_in);
	
	// insert() will force to overwrite while the key-value pair exists.
	struct itr_m _insertBase_soft  (T_key&& key_in, T_val&& val_in, uint64 idx); // tail insertion. (Tail insertion without probing between head and tail elements. (Right of tail element will be linear probed.)) This function is used when rehashing or initializing the table.
//	struct itr_m _insertBase_medium(T_key&& key_in, T_val&& val_in, uint64 idx); // half lineat probing insertion. (Linear probing for only elements evacuation due to the element insertion.)
	struct itr_m _insertBase_hard  (T_key&& key_in, T_val&& val_in, uint64 idx); // full lineat probing insertion. (Linear probing for all of elements insertion including elements evacuation.)
	struct itr_m insert     (const T_key&  key_in, const T_val&  val_in); // copy key and value.
	struct itr_m insert_soft(const T_key&  key_in, const T_val&  val_in); // copy key and value.
	struct itr_m insert_hard(const T_key&  key_in, const T_val&  val_in); // copy key and value.
//	struct itr_m insert(      T_key&& key_in, const T_val&  val_in); // swap key.           (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.add(std::move(key),           val );".)
//	struct itr_m insert(const T_key&  key_in,       T_val&& val_in); // swap value.         (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.add(          key , std::move(val));".)
//	struct itr_m insert(      T_key&& key_in,       T_val&& val_in); // swap key and value. (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.add(std::move(key), std::move(val));".)
	struct itr_m insert     (const T_key&  key_in, const T_val&  val_in, uint64 idx); // copy key and value.
	struct itr_m insert_soft(const T_key&  key_in, const T_val&  val_in, uint64 idx); // copy key and value.
	struct itr_m insert_hard(const T_key&  key_in, const T_val&  val_in, uint64 idx); // copy key and value.
//	struct itr_m insert(      T_key&& key_in, const T_val&  val_in, uint64 idx); // swap key.           (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.add(std::move(key),           val );".)
//	struct itr_m insert(const T_key&  key_in,       T_val&& val_in, uint64 idx); // swap value.         (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.add(          key , std::move(val));".)
//	struct itr_m insert(      T_key&& key_in,       T_val&& val_in, uint64 idx); // swap key and value. (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.add(std::move(key), std::move(val));".)
	
	void erase(const T_key& key_in, uint64 idx);
	void erase(const T_key& key_in);
// erase_recursive(); // recursive erasion.
	
	#ifdef SSTD_IpCHashT_DEBUG
	bool   use_tIdx_dbg = false;
	uint64     tIdx_dbg;
	bool   use_pSize_dbg = false;
	uint64     pSize_dbg;
	#endif
	
	// ---
	
	T_val& operator[](const T_key&  rhs);
//	T_val& operator[](      T_key&& rhs);
};

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
inline uint64 sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::get_tSizeL_idx(const uint64& tableSize){
	
	uint idx=0;
	for(; idx<64; idx++){
		if(sstd_IpCHashT::tSizeL[idx]>=tableSize){ break; }
	}
	idx++; // twice size of table will adjust the load factor 50%.
	return idx;
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
inline void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::IpCHashT_constructor(const uint64& tableSize){
	tSizeL_idx = get_tSizeL_idx(tableSize);
	tSize      = sstd_IpCHashT::tSizeL[tSizeL_idx];
	
	if(tSize<256){ pSize=tSize; }else{ pSize=256ull; }
#ifdef SSTD_IpCHashT_DEBUG
	if(use_pSize_dbg){ pSize=(uint64)pSize_dbg; } // over write pSize for debug
#endif
	ttSize     = tSize + pSize;
	
	pT         = new struct elem_m[ttSize];
	pHashFn    = new T_hash();
	elems      = 0ull;
	
	maxShift   = (T_shift)0;
	maxShift   = ~maxShift; // 'maxShift' will be filled with '1'.
	seekLimit  = maxShift - 1;
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift> inline sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::IpCHashT(                       ){ IpCHashT_constructor(   512   ); } // In order to store 512 elements, 1024 table length will be allocated.
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift> inline sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::IpCHashT(const uint64& tableSize){ IpCHashT_constructor(tableSize); }

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
inline sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::~IpCHashT(){
	delete[] pT;
	delete pHashFn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
void swap_hashT(sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>& lhs, sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>& rhs){
	// "using std::swap;" is defined, in order to preferentially call overloaded function of swap<T>() for type T. (Ref: https://cpprefjp.github.io/reference/utility/swap.html)
	// In here, scope of using is limited by "{}", this means that scope of using is same as a usual value.
	using std::swap;
	
	swap(lhs._tSizeL_idx(), rhs._tSizeL_idx());
	swap(lhs._tSize(),      rhs._tSize()     );
	swap(lhs._pSize(),      rhs._pSize()     );
	swap(lhs._ttSize(),     rhs._ttSize()    );
	swap(lhs._pHashFn(),    rhs._pHashFn()   );
	swap(lhs._pT(),         rhs._pT()        );
	swap(lhs._elems(),      rhs._elems()     );
}

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::move_hashT2vecKV(std::vector<elem_KV_m>& vecKV, sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>& hashT){
	for(auto itr=hashT.begin(); itr!=hashT.end(); ++itr){
		vecKV.push_back( elem_KV_m(std::move(itr.first_RW()), std::move(itr.second_RW())) );
	}
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::failSafe_of_rehashing(sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>& hashT){
	// "using std::swap;" is defined, in order to preferentially call overloaded function of swap<T>() for type T. (Ref: https://cpprefjp.github.io/reference/utility/swap.html)
	// In here, scope of using is limited by "{}", this means that scope of using is same as a usual value.
	using std::swap;
	
	std::vector<elem_KV_m> vecKV(hashT.size()); vecKV.clear();
	move_hashT2vecKV(vecKV, hashT);
	
	sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift> hashT_new(hashT.tableSize()); // twice size of tSize will be allocated.
	swap_hashT(hashT, hashT_new);
	
	while(vecKV.size()!=0){
		uint64 idx; key2tableIdx_wDivisor_m(idx, vecKV[vecKV.size()-1].key, hashT.tableSize());
		auto itrRet = hashT._insertBase_soft(std::move(vecKV[vecKV.size()-1].key), std::move(vecKV[vecKV.size()-1].val), idx);
		if(itrRet.index()==itr_needRehash_m){
			move_hashT2vecKV(vecKV, hashT);
			sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift> hashT_new(hashT.tableSize()); // twice size of tSize will be allocated.
			swap_hashT(hashT, hashT_new);
			continue;
		}
		hashT._elems()++;
		vecKV.pop_back(); // erase the tail element
	}
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
inline void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::rehash(){
	// "using std::swap;" is defined, in order to preferentially call overloaded function of swap<T>() for type T. (Ref: https://cpprefjp.github.io/reference/utility/swap.html)
	// In here, scope of using is limited by "{}", this means that scope of using is same as a usual value.
	using std::swap;
	sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift> hashT_new(tSize); // twice size of tSize will be allocated.
	for(auto itr=this->begin(); itr!=this->end(); ){
		uint64 idx; key2tableIdx_wDivisor_m(idx, itr.first(), hashT_new.tableSize());
		auto itrRet = hashT_new._insertBase_soft(std::move(itr.first_RW()), std::move(itr.second_RW()), idx);
		if(itrRet.index()==itr_needRehash_m){
			failSafe_of_rehashing(hashT_new); continue; } // more rehashing is required while rehashing.
		hashT_new._elems()++;
		++itr;
	}
	swap_hashT(*this, hashT_new);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

#define findBase_m()													\
	for(;;){															\
		if( T_key_eq()(pT[idx].key, key_in) ){ return itr_m(maxShift, ttSize, pT,       idx); } /* key is found. */ \
		if(   pT[idx].next == (T_shift)0    ){ return itr_m(maxShift, ttSize, pT, itr_end_m); } /* key is not found. */ \
		idx += pT[idx].next;											\
	}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
inline struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::find(const T_key& key_in, uint64 idx){
	findBase_m();
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
inline struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::find(const T_key& key_in){
	uint64 idx; key2tableIdx_m(idx, key_in); // get table index
	findBase_m();
}
#undef findBase_m

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::_insertBase_soft(T_key&& key_in, T_val&& val_in, uint64 idx){
	
	// "using std::swap;" is defined, in order to preferentially call overloaded function of swap<T>() for type T. (Ref: https://cpprefjp.github.io/reference/utility/swap.html)
	// in here, scope of using is limited by "{}", this means that scope of using is same as a usual value.
	using std::swap;
	
	if( isEmpty_m(pT[idx]) ){
		// 'pT[idx]' is empty
		
		// --- case01 ---
		assert(idx<ttSize);
		pT[idx].key  = std::move(key_in);
		pT[idx].val  = std::move(val_in);
		pT[idx].prev = (T_shift)0;
		pT[idx].next = (T_shift)0;
		return itr_m(maxShift, ttSize, pT, idx);
	}
	// below:: 'pT[idx]' is not empty
	
	//-----------------------------------------------------------------------------------------------------------------------------------------
	
	// The following process solves a conflict with the existing elements.
	if( isHead_m(pT[idx]) ){
		// If the element is head, append the element between the chain(s).
		// For insertion case02 and case03.
		
		// --- case02 --- ==>> pass for soft
		
		// find key and seek idx.
		for(;;){
			if( T_key_eq()(pT[idx].key, key_in) ){
				// key is found.
				assert(idx<ttSize);
				pT[idx].key  = std::move(key_in);
				pT[idx].val  = std::move(val_in);
				pT[idx].prev = (T_shift)0;
				pT[idx].next = (T_shift)0;
				return itr_m(maxShift, ttSize, pT, idx);
			}
			if( pT[idx].next == (T_shift)0 ){ break; } // key is not found. (At this time, idx indicates the tail of the chain.)
			idx += pT[idx].next;
		}
		uint64 prevIdx_e = idx; // prev index from empty
		
		// seek to an empty element until tSize.
		idx++;
		seek2emptyIndex_m(pT, idx, prevIdx_e); // when false -> returned intaernally -> rehash will be occurd.
		
		// --- case03 ---
		assert(idx<ttSize);
		T_shift prevShift_e = idx - prevIdx_e;
		pT[prevIdx_e].next = prevShift_e;
		pT[    idx  ].key  = std::move(key_in);
		pT[    idx  ].val  = std::move(val_in);
		pT[    idx  ].prev = prevShift_e;
//		pT[    idx  ].next = (T_shift)0; // aleady zero
		return itr_m(maxShift, ttSize, pT, idx);
	}else if( isTail_m(pT[idx]) ){
		// For insertion case04, case06 and case11.
		
		// --- case04 --- ==>> pass for soft
		// --- case06 --- ==>> pass for soft
		
		// seek to an empty element until tSize.
		uint64 idx_f = idx;
		idx = idx_f + 1;
		uint64 prevIdx_f = idx_f - pT[idx_f].prev;
		seek2emptyIndex_m(pT, idx, prevIdx_f); // when false -> returned intaernally -> rehash will be occurd.
		{
			// --- case11 ---
			T_shift prevShift_e = idx - prevIdx_f; assert(prevShift_e<=seekLimit);
			assert(prevIdx_f<ttSize);
			assert(    idx  <ttSize);
			assert(    idx_f<ttSize);
			
			pT[prevIdx_f].next = prevShift_e;
			swap(pT[idx].key, pT[idx_f].key); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
			swap(pT[idx].val, pT[idx_f].val); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
			pT[    idx  ].prev = prevShift_e;
			pT[    idx  ].next = (T_shift)0;
			
			pT[idx_f].key = std::move(key_in);
			pT[idx_f].val = std::move(val_in);
			pT[idx_f].prev = (T_shift)0;
//			pT[idx_f].next = (T_shift)0; // aleady zero
			
			return itr_m(maxShift, ttSize, pT, idx_f);
		}
	}else{
		// For insertion case05, case07, case09, case08 and case10.
		uint64 idx_f = idx; // first index
		uint64 prev2next_f = (uint64)pT[idx_f].prev + (uint64)pT[idx_f].next;
		if(prev2next_f>seekLimit){ return itr_m(maxShift, ttSize, pT, itr_needRehash_m); }
		
		// --- case05 --- ==>> pass for soft
		// --- case07 --- ==>> pass for soft
		// --- case09 --- ==>> pass for soft
		// --- case08 --- ==>> pass for soft
		
		seek2tail(pT, idx);
		
		// seek to an empty element until tSize.
		uint64 prevIdx_e = idx; // prev index from empty
		seek2emptyIndex_m(pT, idx, prevIdx_e); // when false -> returned intaernally -> rehash will be occurd.
		{
			// --- case10 ---
			uint64 prevIdx_f = idx_f - pT[idx_f].prev;
			uint64 nextIdx_f = idx_f + pT[idx_f].next;
			assert(prevIdx_f<ttSize);
			assert(    idx_f<ttSize);
			assert(nextIdx_f<ttSize);
			assert(prevIdx_e<ttSize);
			assert(    idx  <ttSize);
			
			pT[prevIdx_f].next = prev2next_f;
			swap(pT[idx].key, pT[idx_f].key); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
			swap(pT[idx].val, pT[idx_f].val); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
			pT[    idx_f].key  = std::move(key_in);
			pT[    idx_f].val  = std::move(val_in);
			pT[    idx_f].prev = (T_shift)0;
			pT[    idx_f].next = (T_shift)0;
			pT[nextIdx_f].prev = prev2next_f;
			
			T_shift prevShift_e = idx - prevIdx_e;
			pT[prevIdx_e].next = prevShift_e;
			pT[    idx  ].prev = prevShift_e;
			pT[    idx  ].next = (T_shift)0;
			return itr_m(maxShift, ttSize, pT, idx_f);
		}
	}
}
#define insert_soft_cc_m()												\
																		\
	T_key key = key_in; /* copy -> COPY_OR_NOP <- when impliment as a define */	\
	T_val val = val_in; /* copy */										\
	for(;;){															\
		/* there is not the key-value pair on the table. */				\
		struct itr_m itrI = this->_insertBase_soft(std::move(key), std::move(val), idx); \
		if(itrI._needRehash()){											\
			rehash();													\
			key2tableIdx_m(idx, key_in); /* get table index */			\
			continue;													\
		}																\
		elems++;														\
		return itrI;													\
	}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::insert_soft(const T_key& key_in, const T_val& val_in){ // copy key and value.
	uint64 idx; key2tableIdx_m(idx, key_in);
	insert_soft_cc_m();
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::insert_soft(const T_key&  key_in, const T_val&  val_in, uint64 idx){
	insert_soft_cc_m();
} // copy key and value.

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::_insertBase_hard(T_key&& key_in, T_val&& val_in, uint64 idx){
	
	// "using std::swap;" is defined, in order to preferentially call overloaded function of swap<T>() for type T. (Ref: https://cpprefjp.github.io/reference/utility/swap.html)
	// in here, scope of using is limited by "{}", this means that scope of using is same as a usual value.
	using std::swap;
	
	if( isEmpty_m(pT[idx]) ){
		// 'pT[idx]' is empty
		
		// --- case01 ---
		assert(idx<ttSize);
		pT[idx].key  = std::move(key_in);
		pT[idx].val  = std::move(val_in);
		pT[idx].prev = (T_shift)0;
		pT[idx].next = (T_shift)0;
		return itr_m(maxShift, ttSize, pT, idx);
	}
	// below:: 'pT[idx]' is not empty
	
	// The following process solves a conflict with the existing elements.
	if( isHead_m(pT[idx]) ){
		// If the element is head, append the element between the chain(s).
		// For insertion case02 and case03.

		uint64 prevIdx_e = idx;                // prev index from empty
		uint64 nextIdx_e = idx + pT[idx].next; // next index from empty
		for(;; idx++){
			// seek to an empty element until nextIdx.
			if(idx>=nextIdx_e){
				prevIdx_e = idx;
				if( isTail_m(pT[idx]) ){ break; }
				nextIdx_e = idx + pT[idx].next;
			}
			if( isEmpty_m(pT[idx]) ){
				// --- case02 ---
				T_shift prevShift_e = idx - prevIdx_e; assert(prevShift_e<=seekLimit);
				T_shift nextShift_e = nextIdx_e - idx; assert(nextShift_e<=seekLimit);
				assert(prevIdx_e<ttSize);
				assert(    idx  <ttSize);
				assert(nextIdx_e<ttSize);
				
				pT[prevIdx_e].next = prevShift_e;
				pT[    idx  ].key  = std::move(key_in);
				pT[    idx  ].val  = std::move(val_in);
				pT[    idx  ].prev = prevShift_e;
				pT[    idx  ].next = nextShift_e;
				pT[nextIdx_e].prev = nextShift_e;
				return itr_m(maxShift, ttSize, pT, idx);
			}
		}
		
		// seek to an empty element until tSize.
		seek2emptyIndex_m(pT, idx, nextIdx_e); // when false -> returned intaernally -> rehash will be occurd.
		
		// --- case03 ---
		assert(idx<ttSize);
		T_shift prevShift_e = idx - prevIdx_e;
		pT[prevIdx_e].next = prevShift_e;
		pT[    idx  ].key  = std::move(key_in);
		pT[    idx  ].val  = std::move(val_in);
		pT[    idx  ].prev = prevShift_e;
//		pT[    idx  ].next = (T_shift)0; // aleady zero
		return itr_m(maxShift, ttSize, pT, idx);
	}else if( isTail_m(pT[idx]) ){
		// For insertion case04, case06 and case11.
		
		uint64 idx_f = idx; // first index
		{
			uint64 prevIdx_e = idx - pT[idx].prev; // empty prevIdx
			for(;;){
				idx--;
				if( idx<=prevIdx_e ){ break; }
				if( isEmpty_m(pT[idx]) ){
					// --- case04 ---
					T_shift prevShift_e = idx - prevIdx_e; assert(prevShift_e<=seekLimit);
					assert(prevIdx_e<ttSize);
					assert(    idx  <ttSize);
					assert(    idx_f<ttSize);
					
					pT[prevIdx_e].next = prevShift_e;
					swap(pT[idx].key, pT[idx_f].key); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
					swap(pT[idx].val, pT[idx_f].val); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
					pT[    idx  ].prev = prevShift_e;
					pT[    idx  ].next = (T_shift)0;
					
					pT[idx_f].key  = std::move(key_in);
					pT[idx_f].val  = std::move(val_in);
					pT[idx_f].prev = (T_shift)0;
					pT[idx_f].next = (T_shift)0;
					
					return itr_m(maxShift, ttSize, pT, idx_f);
				}
			}
		}
		
		if(! isHead_m(pT[idx]) ){
			uint64 prevIdx_e = idx - pT[idx].prev;
			uint64 nextIdx_e = idx;
			for(;;){
				idx--;
				if( idx<=prevIdx_e ){
					nextIdx_e = idx;
					if( isHead_m(pT[idx]) ){ break; }
					prevIdx_e = idx - pT[idx].prev;
					continue;
				}
				if( isEmpty_m(pT[idx]) ){
					// --- case06 ---
					uint64 prevIdx_f = idx_f - pT[idx_f].prev;
					T_shift prevShift_e = idx - prevIdx_e; assert(prevShift_e<=seekLimit);
					T_shift nextShift_e = nextIdx_e - idx; assert(nextShift_e<=seekLimit);
					assert(prevIdx_e<ttSize);
					assert(    idx  <ttSize);
					assert(nextIdx_e<ttSize);
					assert(prevIdx_f<ttSize);
					assert(    idx_f<ttSize);
					
					pT[prevIdx_e].next = prevShift_e;
					swap(pT[idx].key, pT[idx_f].key); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
					swap(pT[idx].val, pT[idx_f].val); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
					pT[    idx  ].prev = prevShift_e;
					pT[    idx  ].next = nextShift_e;
					pT[nextIdx_e].prev = nextShift_e;
					
					pT[prevIdx_f].next = (T_shift)0;
					pT[    idx_f].key = std::move(key_in);
					pT[    idx_f].val = std::move(val_in);
					pT[    idx_f].prev = (T_shift)0;
//					pT[    idx_f].next = (T_shift)0; // aleady zero
					
					return itr_m(maxShift, ttSize, pT, idx_f);
				}
			}
		}
		
		// seek to an empty element until tSize.
		idx = idx_f + 1;
		uint64 prevIdx_f = idx_f - pT[idx_f].prev;
		seek2emptyIndex_m(pT, idx, prevIdx_f); // when false -> returned intaernally -> rehash will be occurd.
		{
			// --- case11 ---
			T_shift prevShift_e = idx - prevIdx_f; assert(prevShift_e<=seekLimit);
			assert(prevIdx_f<ttSize);
			assert(    idx  <ttSize);
			assert(    idx_f<ttSize);
			
			pT[prevIdx_f].next = prevShift_e;
			swap(pT[idx].key, pT[idx_f].key); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
			swap(pT[idx].val, pT[idx_f].val); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
			pT[    idx  ].prev = prevShift_e;
			pT[    idx  ].next = (T_shift)0;
			
			pT[idx_f].key = std::move(key_in);
			pT[idx_f].val = std::move(val_in);
			pT[idx_f].prev = (T_shift)0;
//			pT[idx_f].next = (T_shift)0; // aleady zero
			
			return itr_m(maxShift, ttSize, pT, idx_f);
		}
	}else{
		// For insertion case05, case07, case09, case08 and case10.
		
		uint64 idx_f = idx; // first index
		{
			uint64 prevIdx_e = idx - pT[idx].prev; // empty prevIdx
			for(;;){
				idx--;
				if( idx<=prevIdx_e ){ break; }
				if( isEmpty_m(pT[idx]) ){
					// --- case05 ---
					uint64 nextIdx_f = idx_f + pT[idx_f].next;
					T_shift prevShift_e = idx - prevIdx_e;
					uint64  nextShift_e = idx_f + (uint64)pT[idx_f].next - idx;
					if(nextShift_e>seekLimit){ return itr_m(maxShift, ttSize, pT, itr_needRehash_m); }
					
					assert(prevIdx_e<ttSize);
					assert(    idx  <ttSize);
					assert(nextIdx_f<ttSize);
					assert(    idx_f<ttSize);
					assert(prevShift_e<=seekLimit);
					assert(nextShift_e<=seekLimit);
					
					pT[prevIdx_e].next = prevShift_e;
					swap(pT[idx].key, pT[idx_f].key); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
					swap(pT[idx].val, pT[idx_f].val); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
					pT[    idx  ].prev = prevShift_e;
					pT[    idx  ].next = (T_shift)nextShift_e;
					pT[nextIdx_f].prev = (T_shift)nextShift_e;
				
					pT[    idx_f].key  = std::move(key_in);
					pT[    idx_f].val  = std::move(val_in);
					pT[    idx_f].prev = (T_shift)0;
					pT[    idx_f].next = (T_shift)0;
				
					return itr_m(maxShift, ttSize, pT, idx_f);
				}
			}
		}
		
		uint64 prev2next_f = (uint64)pT[idx_f].prev + (uint64)pT[idx_f].next;
		if(prev2next_f>seekLimit){ return itr_m(maxShift, ttSize, pT, itr_needRehash_m); }
		if(! isHead_m(pT[idx]) ){
			uint64 prevIdx_e = idx - pT[idx].prev;
			uint64 nextIdx_e = idx;
			for(;;){
				idx--;
				if( idx<=prevIdx_e ){
					nextIdx_e = idx;
					if( isHead_m(pT[idx]) ){ break; }
					prevIdx_e = idx - pT[idx].prev;
					continue;
				}
				if( isEmpty_m(pT[idx]) ){
					// --- case07 ---
					uint64 prevIdx_f = idx_f - pT[idx_f].prev;
					uint64 nextIdx_f = idx_f + pT[idx_f].next;
					T_shift prevShift_e = idx - prevIdx_e;
					T_shift nextShift_e = nextIdx_e - idx;
					
					assert(prevIdx_e<ttSize);
					assert(    idx  <ttSize);
					assert(nextIdx_e<ttSize);
					assert(prevIdx_f<ttSize);
					assert(    idx_f<ttSize);
					assert(nextIdx_f<ttSize);
					assert(prevShift_e<=seekLimit);
					assert(nextShift_e<=seekLimit);
					assert(prev2next_f<=seekLimit);
					
					pT[prevIdx_e].next = prevShift_e;
					swap(pT[idx].key, pT[idx_f].key); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
					swap(pT[idx].val, pT[idx_f].val); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
					pT[    idx  ].prev = prevShift_e;
					pT[    idx  ].next = nextShift_e;
					pT[nextIdx_e].prev = nextShift_e;
						
					pT[prevIdx_f].next = (T_shift)prev2next_f;
					pT[    idx_f].key = std::move(key_in);
					pT[    idx_f].val = std::move(val_in);
					pT[    idx_f].prev = (T_shift)0;
					pT[    idx_f].next = (T_shift)0;
					pT[nextIdx_f].prev = (T_shift)prev2next_f;
					
					return itr_m(maxShift, ttSize, pT, idx_f);
				}
			}
		}
		
		uint64 prevIdx_f = idx_f - pT[idx_f].prev;
		uint64 nextIdx_f = idx_f + pT[idx_f].next;
		for(idx=idx_f+1;; idx++){
			if( idx>=nextIdx_f ){ break; }
			if( isEmpty_m(pT[idx]) ){
				// --- case09 ---
				uint64 prevIdx_f2idx_e = idx - prevIdx_f;
				if(prevIdx_f2idx_e>seekLimit){ return itr_m(maxShift, ttSize, pT, itr_needRehash_m); }
				T_shift nextShift_e = nextIdx_f - idx;
				
				assert(prevIdx_f<ttSize);
				assert(    idx_f<ttSize);
				assert(    idx  <ttSize);
				assert(nextIdx_f<ttSize);
				assert(prevIdx_f2idx_e<=seekLimit);
				assert(nextShift_e    <=seekLimit);
				
				pT[prevIdx_f].next = (T_shift)prevIdx_f2idx_e;
				
				swap(pT[idx].key, pT[idx_f].key); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
				swap(pT[idx].val, pT[idx_f].val); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
				pT[idx_f].key  = std::move(key_in);
				pT[idx_f].val  = std::move(val_in);
				pT[idx_f].prev = (T_shift)0;
				pT[idx_f].next = (T_shift)0;
				
				pT[    idx  ].prev = (T_shift)prevIdx_f2idx_e;
				pT[    idx  ].next = nextShift_e;
				pT[nextIdx_f].prev = nextShift_e;
				return itr_m(maxShift, ttSize, pT, idx_f);
			}
		}
		
		// The end of case09 guarantees that "idx==idx_f+1" is not empty.
		uint64 prevIdx_e = idx;
		uint64 nextIdx_e = idx + pT[idx].next;
		for(;; idx++){
			// seek to an empty element until nextIdx.
			if( idx>=nextIdx_e ){
				prevIdx_e = idx;
				if( isTail_m(pT[idx]) ){ break; }
				nextIdx_e = idx + pT[idx].next;
			}
			if( isEmpty_m(pT[idx]) ){
				// --- case08 ---
				assert(prevIdx_f<ttSize);
				assert(    idx_f<ttSize);
				assert(nextIdx_f<ttSize);
				assert(prevIdx_e<ttSize);
				assert(    idx  <ttSize);
				assert(nextIdx_e<ttSize);
				assert(prev2next_f<=seekLimit);
				
				pT[prevIdx_f].next = prev2next_f;
				swap(pT[idx].key, pT[idx_f].key); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
				swap(pT[idx].val, pT[idx_f].val); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
				pT[    idx_f].key  = std::move(key_in);
				pT[    idx_f].val  = std::move(val_in);
				pT[    idx_f].prev = (T_shift)0;
				pT[    idx_f].next = (T_shift)0;
				pT[nextIdx_f].prev = prev2next_f;
				
				T_shift prevShift_e = idx - prevIdx_e;
				T_shift nextShift_e = nextIdx_e - idx;
				assert(prevShift_e<=seekLimit);
				assert(nextShift_e<=seekLimit);
				
				pT[prevIdx_e].next = prevShift_e;
				pT[    idx  ].prev = prevShift_e;
				pT[    idx  ].next = nextShift_e;
				pT[nextIdx_e].prev = nextShift_e;
				return itr_m(maxShift, ttSize, pT, idx_f);
			}
		}
		
		// seek to an empty element until tSize.
		seek2emptyIndex_m(pT, idx, nextIdx_e); // when false -> returned intaernally -> rehash will be occurd.
		
		{
			// --- case10 ---
			assert(prevIdx_f<ttSize);
			assert(    idx_f<ttSize);
			assert(nextIdx_f<ttSize);
			assert(prevIdx_e<ttSize);
			assert(    idx  <ttSize);
			
			pT[prevIdx_f].next = prev2next_f;
			swap(pT[idx].key, pT[idx_f].key); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
			swap(pT[idx].val, pT[idx_f].val); // move pT[idx_f] to pT[idx] and make pT[idx_f] empty.
			pT[    idx_f].key  = std::move(key_in);
			pT[    idx_f].val  = std::move(val_in);
			pT[    idx_f].prev = (T_shift)0;
			pT[    idx_f].next = (T_shift)0;
			pT[nextIdx_f].prev = prev2next_f;
			
			T_shift prevShift_e = idx - prevIdx_e;
			pT[prevIdx_e].next = prevShift_e;
			pT[    idx  ].prev = prevShift_e;
			pT[    idx  ].next = (T_shift)0;
			return itr_m(maxShift, ttSize, pT, idx_f);
		}
	}
}

//---

#define insert_hard_cc_m()												\
	struct itr_m itrF = this->find(key_in, idx);						\
	if(itrF!=this->end()){ itrF.second_RW()=val_in; return itrF; }		\
																		\
	T_key key = key_in; /* copy -> COPY_OR_NOP <- when impliment as a define */	\
	T_val val = val_in; /* copy */										\
	for(;;){															\
		/* there is not the key-value pair on the table. */				\
		struct itr_m itrI = this->_insertBase_hard(std::move(key), std::move(val), idx); \
		if(itrI._needRehash()){											\
			rehash();													\
			key2tableIdx_m(idx, key_in); /* get table index */			\
			continue;													\
		}																\
		elems++;														\
		return itrI;													\
	}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::insert_hard(const T_key& key_in, const T_val& val_in){ // copy key and value.
	uint64 idx; key2tableIdx_m(idx, key_in); // get table index
	insert_hard_cc_m();
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::insert_hard(const T_key&  key_in, const T_val&  val_in, uint64 idx){
	insert_hard_cc_m();
} // copy key and value.
//template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift> void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::insert(      T_key&& key_in, const T_val&  val_in){} // swap key.           (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.add(std::move(key),           val );".)
//template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift> void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::insert(const T_key&  key_in,       T_val&& val_in){} // swap value.         (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.add(          key , std::move(val));".)
//template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift> void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::insert(      T_key&& key_in,       T_val&& val_in){} // swap key and value. (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.add(std::move(key), std::move(val));".)

//-----------------------------------------------------------------------------------------------------------------------------------------------

#ifdef use_insert_soft
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::insert(const T_key& key_in, const T_val& val_in){ // copy key and value.
	uint64 idx; key2tableIdx_m(idx, key_in);
	insert_soft_cc_m();
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::insert(const T_key&  key_in, const T_val&  val_in, uint64 idx){ // copy key and value.
	insert_soft_cc_m();
}
#endif

//---

#ifdef use_insert_hard
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::insert(const T_key& key_in, const T_val& val_in){ // copy key and value.
	uint64 idx; key2tableIdx_m(idx, key_in);
	insert_hard_cc_m();
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::insert(const T_key&  key_in, const T_val&  val_in, uint64 idx){ // copy key and value.
	insert_hard_cc_m();
}
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
inline T_val& sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::operator[](const T_key& key_in){
	uint64 idx; key2tableIdx_m(idx, key_in); // get table index
	auto itrF = find(key_in, idx);
	if(itrF!=this->end()){ return itrF.second_RW(); }
	
	T_val val_in = T_val(); // generation of empty buf for insert()
	auto itrI = insert(key_in, val_in, idx);
//	auto itrI = insert(key_in, std::move(val_in), idx);
	return itrI.second_RW();
}
//T_val& operator[](      T_key&& rhs);

//-----------------------------------------------------------------------------------------------------------------------------------------------

//template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
//void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::erase(const T_key& key_in, uint64 idx);

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift>
void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift>::erase(const T_key& key_in){
	
	// "using std::swap;" is defined, in order to preferentially call overloaded function of swap<T>() for type T. (Ref: https://cpprefjp.github.io/reference/utility/swap.html)
	// in here, scope of using is limited by "{}", this means that scope of using is same as a usual value.
	using std::swap;
	
	auto itr = find(key_in);
	if(! (itr!=this->end()) ){ return; }
	uint64 idx = itr.index();
	
	if( isHead_m(pT[idx]) && isTail_m(pT[idx]) ){
		// --- case01 ---
		T_key keyBuf; // in order to call destructor
		T_val valBuf; // in order to call destructor
		swap(pT[idx].key, keyBuf);
		swap(pT[idx].val, valBuf);
		pT[idx].prev = maxShift;
//		pT[idx].next = (T_shift)0; // already zero
	}else if( isTail_m(pT[idx]) ){
		// --- case02 ---
		uint64 prevIdx = idx - pT[idx].prev;
		pT[prevIdx].next = (T_shift)0;
		
		T_key keyBuf; // in order to call destructor
		T_val valBuf; // in order to call destructor
		swap(pT[idx].key, keyBuf);
		swap(pT[idx].val, valBuf);
		pT[idx].prev = maxShift;
//		pT[idx].next = (T_shift)0; // already zero
	}else if( isHead_m(pT[idx]) ){
		uint64 idx_head = idx;
		seek2tail(pT, idx);
		
		// --- case03 ---
		swap(pT[idx_head].key, pT[idx].key); // move the tail element to the head.
		swap(pT[idx_head].val, pT[idx].val); // move the tail element to the head.
		
		uint64 prevIdx = idx - pT[idx].prev;
		pT[prevIdx].next = (T_shift)0;
		T_key keyBuf; // in order to call destructor
		T_val valBuf; // in order to call destructor
		swap(pT[idx].key, keyBuf); // move the tail element to the head.
		swap(pT[idx].val, valBuf); // move the tail element to the head.
		pT[idx].prev = maxShift;
//		pT[idx].next = (T_shift)0; // already zero
	}else{
		uint64 idx_middle = idx;
		seek2tail(pT, idx);
		
		// --- case04 ---
		swap(pT[idx_middle].key, pT[idx].key); // move the tail element to the head.
		swap(pT[idx_middle].val, pT[idx].val); // move the tail element to the head.
		
		uint64 prevIdx = idx - pT[idx].prev;
		pT[prevIdx].next = (T_shift)0;
		T_key keyBuf; // in order to call destructor
		T_val valBuf; // in order to call destructor
		swap(pT[idx].key, keyBuf); // move the tail element to the head.
		swap(pT[idx].val, valBuf); // move the tail element to the head.
		pT[idx].prev = maxShift;
//		pT[idx].next = (T_shift)0; // already zero
	}
	elems--;
	return;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

#undef key2tableIdx_m
#undef seek2emptyIndex_m
#undef isTail_m
#undef isHead_m
#undef isEmpty_m

#undef itr_needRehash_m
#undef itr_end_m

#undef elem_KV_m
#undef elem_m
#undef itr_m

//-----------------------------------------------------------------------------------------------------------------------------------------------

#ifdef use_insert_soft
	#undef use_insert_soft
#endif

#ifdef use_insert_hard
	#undef use_insert_hard
#endif

#ifdef use_prime_table
	#undef use_prime_table
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------
