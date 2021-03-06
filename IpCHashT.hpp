#pragma once
#include "./typeDef.h"
#include <memory>
#include <assert.h> // for debug
#include <sstd/sstd.hpp> // for debug

//-----------------------------------------------------------------------------------------------------------------------------------------------
// compile options

#define use_insert_soft // select soft or hard
//#define use_prime_table

//-----------------------------------------------------------------------------------------------------------------------------------------------

#define elem_m    sstd_IpCHashT::element       <T_key,T_val,T_shift> // a macro of table element structure
#define elem_KV_m sstd_IpCHashT::element_KeyVal<T_key,T_val>         // a macro of table element structure
#define itr_m     sstd_IpCHashT::iterator      <T_key,T_val,T_shift> // a macro of iterator

//-----------------------------------------------------------------------------------------------------------------------------------------------

namespace sstd{
	namespace IpCHashT_opt{
		class maxLF50 { private: public: maxLF50 (){}; ~maxLF50 (){}; };
		class maxLF100{ private: public: maxLF100(){}; ~maxLF100(){}; };
		
		class   successfulMajor{ private: public:   successfulMajor(){};   ~successfulMajor(){}; };
		class unsuccessfulMajor{ private: public: unsuccessfulMajor(){}; ~unsuccessfulMajor(){}; };
	}
	
	// set default options
	template <class T_key,
			  class T_val,
			  class T_hash     = std::hash<T_key>,
			  class T_key_eq   = std::equal_to<T_key>,
			  typename T_shift = uint8,                              // or uint16
			  typename T_maxLF = sstd::IpCHashT_opt::maxLF50,        // or sstd::IpCHashT_opt::maxLF100
			  typename T_major = sstd::IpCHashT_opt::successfulMajor // or sstd::IpCHashT_opt::unsuccessfulMajor
			  >
	class IpCHashT; // in-place chained hash table
	
	// set alias for IpCHashT option
	template<class T_key, class T_val, class T_hash=std::hash<T_key>, class T_key_eq=std::equal_to<T_key>>
	using IpCHashT_u8hS = IpCHashT<T_key, T_val, T_hash, T_key_eq, uint8, sstd::IpCHashT_opt::maxLF50, sstd::IpCHashT_opt::successfulMajor>; // uint8, half (maxLoadfactor50), Successful lookup major option
	
	template<class T_key, class T_val, class T_hash=std::hash<T_key>, class T_key_eq=std::equal_to<T_key>>
	using IpCHashT_u8fS = IpCHashT<T_key, T_val, T_hash, T_key_eq, uint8, sstd::IpCHashT_opt::maxLF100, sstd::IpCHashT_opt::successfulMajor>; // uint8, full (maxLoadfactor100), Successful lookup major option
	
	template<class T_key, class T_val, class T_hash=std::hash<T_key>, class T_key_eq=std::equal_to<T_key>>
	using IpCHashT_u16hS = IpCHashT<T_key, T_val, T_hash, T_key_eq, uint16, sstd::IpCHashT_opt::maxLF50, sstd::IpCHashT_opt::successfulMajor>; // uint16, half (maxLoadfactor50), Successful lookup major option
	
	template<class T_key, class T_val, class T_hash=std::hash<T_key>, class T_key_eq=std::equal_to<T_key>>
	using IpCHashT_u16fS = IpCHashT<T_key, T_val, T_hash, T_key_eq, uint16, sstd::IpCHashT_opt::maxLF100, sstd::IpCHashT_opt::successfulMajor>; // uint16, full (maxLoadfactor100), Successful lookup major option
	
	template<class T_key, class T_val, class T_hash=std::hash<T_key>, class T_key_eq=std::equal_to<T_key>>
	using IpCHashT_u8hU = IpCHashT<T_key, T_val, T_hash, T_key_eq, uint8, sstd::IpCHashT_opt::maxLF50, sstd::IpCHashT_opt::unsuccessfulMajor>; // uint8, half (maxLoadfactor50), Unsuccessful lookup major option
	
	template<class T_key, class T_val, class T_hash=std::hash<T_key>, class T_key_eq=std::equal_to<T_key>>
	using IpCHashT_u8fU = IpCHashT<T_key, T_val, T_hash, T_key_eq, uint8, sstd::IpCHashT_opt::maxLF100, sstd::IpCHashT_opt::unsuccessfulMajor>; // uint8, full (maxLoadfactor100), Unsuccessful lookup major option
	
	template<class T_key, class T_val, class T_hash=std::hash<T_key>, class T_key_eq=std::equal_to<T_key>>
	using IpCHashT_u16hU = IpCHashT<T_key, T_val, T_hash, T_key_eq, uint16, sstd::IpCHashT_opt::maxLF50, sstd::IpCHashT_opt::unsuccessfulMajor>; // uint16, half (maxLoadfactor50), Unsuccessful lookup major option
	
	template<class T_key, class T_val, class T_hash=std::hash<T_key>, class T_key_eq=std::equal_to<T_key>>
	using IpCHashT_u16fU = IpCHashT<T_key, T_val, T_hash, T_key_eq, uint16, sstd::IpCHashT_opt::maxLF100, sstd::IpCHashT_opt::unsuccessfulMajor>; // uint16, full (maxLoadfactor100), Unsuccessful lookup major option
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

namespace sstd_IpCHashT{
	template <class T_key, class T_val, typename T_shift> struct element;
	template <class T_key, class T_val>                   struct element_KeyVal;
	template <class T_key, class T_val, typename T_shift> struct iterator;
	
	bool isSuccessfulMajor(sstd::IpCHashT_opt::successfulMajor   dummy);
	bool isSuccessfulMajor(sstd::IpCHashT_opt::unsuccessfulMajor dummy);
	
	                         inline bool isUint8 (const   uint8 dummy){ return true;  }
	template <class T_dummy> inline bool isUint8 (const T_dummy dummy){ return false; }
	                         inline bool isUint16(const  uint16 dummy){ return true;  }
	template <class T_dummy> inline bool isUint16(const T_dummy dummy){ return false; }
	                         inline bool isMaxLF50 (const sstd::IpCHashT_opt::maxLF50  rhs){ return true;  }
	template <class T_dummy> inline bool isMaxLF50 (const                     T_dummy  rhs){ return false; }
	                         inline bool isMaxLF100(const sstd::IpCHashT_opt::maxLF100 rhs){ return true;  }
	template <class T_dummy> inline bool isMaxLF100(const                      T_dummy rhs){ return false; }
	
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
	
	T_key key; // key
	T_val val; // value
};

//-----------------------------------------------------------------------------------------------------------------------------------------------

// in order to reduce the calling time of function, macro expansion will be used.
#define isOverMaxLF_m(elems, elems_maxLF)		\
	(elems>elems_maxLF)
#define isEmpty_m(ELEM)							\
	(ELEM.prev==maxShift)
#define isHead_m(ELEM)							\
	(ELEM.prev==(T_shift)0)
#define isTail_m(ELEM)							\
	(ELEM.next==(T_shift)0)
#define seek2emptyIndex_m(pT, idx, idx_last)							\
	for(;;){															\
		if( idx>=ttSize || idx>=idx_last+seekLimit ){ return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, itr_needRehash_m), false); } \
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
		tIdx = hVal & tSize_m1;												\
		if(use_tIdx_dbg){ idx=tIdx_dbg; } /* over write tIdx for debug */
	#else
	#define key2tableIdx_m(tIdx, key)										\
		uint64 hVal = (uint64)(*pHashFn)(key); /* generate hashed value  */	\
		tIdx = hVal & tSize_m1;
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
	
	inline const T_key  & first    () const { return pT[idx].key;  }
	inline       T_key  & first_RW () const { return pT[idx].key;  }
	inline const T_val  & second   () const { return pT[idx].val;  }
	inline       T_val  & second_RW()       { return pT[idx].val;  }
	inline const uint64 & index    () const { return idx;          }
	inline const T_shift& prev     () const { return pT[idx].prev; } // for debug
	inline const T_shift& next     () const { return pT[idx].next; } // for debug
	
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
		std::cout << " idx: " <<    idx       << std::endl;
		std::cout << " key: " << pT[idx].key  << std::endl;
		std::cout << " val: " << pT[idx].val  << std::endl;
		std::cout << "prev: " << pT[idx].prev << std::endl;
		std::cout << "next: " << pT[idx].next << std::endl;
	}
	#endif
};

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
class sstd::IpCHashT{
private:
	void IpCHashT_constructor(uint64 tableSize);
	
	#ifdef use_prime_table
	uint8  tSizeL_idx;      // table size list index
	#else
	uint64 tSize_m1;        // tSize minus 1.
	#endif
	uint64 tSize;           // table size. This is a hash value division size.
	uint64 pSize;           // padding size of the table
	uint64 ttSize;          // total table size. This is a seek limit size. ( ttSize == tSize + pSize ).
	uint64 elems;           // number of elements on the table
	uint64 elems_maxLF;     // number of elements when tha table is maxLF.
	T_hash* pHashFn;        // pointer to the hash function
	struct elem_m* pT;      // pointer to the table
	
	// constant values
	T_shift maxShift;
	T_shift seekLimit;
	
	void move_hashT2vecKV(std::vector<elem_KV_m>& vecKV, sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>& hashT);
	void failSafe_of_rehashing(sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>& hashT_new);
	
public:
	typedef std::pair<const T_key, T_val> value_type; // for STL compatibility.
	
	IpCHashT();
	IpCHashT(const uint64 tableSize); // nearest size under power of 2 will be allocate.
	#ifdef use_prime_table
	IpCHashT(const uint8 tableSizeL_idx, const uint64 tableSize); // allocate same size of tableSize. for rehashing. (select size from prime table, never form the others).
	#else
	IpCHashT(const uint64 tableSize_minus1, const uint64 tableSize); // allocate same size of tableSize. for rehashing. (select size of power 2, never form the others).
	#endif
	~IpCHashT();
	
	#ifdef use_prime_table
	inline uint8& _tSizeL_idx(){ return tSizeL_idx; }
	#else
	inline uint64& _tSize_m1(){ return tSize_m1; }
	#endif
	inline uint64& _tSize(){ return tSize; }
	inline uint64& _pSize(){ return pSize; }
	inline uint64& _ttSize(){ return ttSize; }
	inline uint64& _elems (){ return elems; }
	inline uint64& _elems_maxLF(){ return elems_maxLF; }
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
	inline const uint64         size(){ return elems; }
	inline const uint64    tableSize(){ return tSize; }
	inline const uint64 bucket_count(){ return tSize; }
	inline const double load_factor() const { return (double)elems/(double)ttSize; }
	
	void rehash();
	
	struct itr_m find(const T_key& key_in, uint64 idx);
	struct itr_m find(const T_key& key_in);
	
	// insert() will force to overwrite while the key-value pair exists.
	std::pair<struct itr_m, bool> _insertBase_soft  (T_key&& key_in, T_val&& val_in, uint64 idx); // tail insertion. (Tail insertion without probing between head and tail elements. (Right of tail element will be linear probed.)) This function is used when rehashing or initializing the table.
//	std::pair<struct itr_m, bool> _insertBase_medium(T_key&& key_in, T_val&& val_in, uint64 idx); // half lineat probing insertion. (Linear probing for only elements evacuation due to the element insertion.)
	std::pair<struct itr_m, bool> _insertBase_hard  (T_key&& key_in, T_val&& val_in, uint64 idx); // full lineat probing insertion. (Linear probing for all of elements insertion including elements evacuation.)
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
	
	std::pair<struct itr_m, bool> insert(const value_type& v); // for STL (1). Ref: https://cpprefjp.github.io/reference/unordered_map/unordered_map/insert.html
	void erase(const T_key& key_in, uint64 idx);
	void erase(const T_key& key_in);
// erase_recursive(); // recursive erasion.
	
#ifdef SSTD_IpCHashT_DEBUG
	bool use_tIdx_dbg    = false; uint64 tIdx_dbg;
	bool use_pSize_dbg   = false; uint64 pSize_dbg;
	bool use_testFOR_dbg = false;
#endif
	
	// ---
	
	T_val& operator[](const T_key&  rhs);
//	T_val& operator[](      T_key&& rhs);
};

//-----------------------------------------------------------------------------------------------------------------------------------------------

#define get_tSizeL_idx(idx)												\
	idx=0;																\
	for(; idx<64; idx++){												\
		if(sstd_IpCHashT::tSizeL[idx]>=tableSize){ break; }				\
	}
#define get_tSize(tSize)						\
	tSize=2;									\
	while(tSize<tableSize){ tSize*=2; }

inline double get_maxLF(const sstd::IpCHashT_opt::maxLF50 & rhs){ return 0.50; }
inline double get_maxLF(const sstd::IpCHashT_opt::maxLF100& rhs){ return 1.00; }

#define constructorBase_init_pSize_m()									\
	/* pSize = (1/a) * tSize + b                      */				\
	/*                                                */				\
	/*         pSize                                  */				\
	/*           |                                    */				\
	/*           |                                    */				\
	/*      254  -          -----------------------   */				\
	/*           |        *                           */				\
	/*           |     *   |                          */				\
	/*           |  *                                 */				\
	/* b (bias)  -         |                          */				\
	/*           |                                    */				\
	/*           +---------|-----------------> tSize  */				\
	/*                   254*a                        */				\
	/*                                                */				\
	/* Fig. pSize vs tSize                            */				\
																		\
	const double a = 18; /* hyper parametor for T_shift==uint8. */		\
	const uint64 b = 35; /* hyper parametor for T_shift==uint8. */		\
	pSize=(uint64)((double)tSize/a + b);								\
	if(pSize>254){ pSize=254ull; } /* when using T_shift=uint8, 0xFF-1==254 is the max-shift. */

#define constructorBase_init_m()										\
	ttSize      = tSize + pSize; /* while "#define use_prime_table" is disabled, ttSize must be satisfied ttSize>=tSize+1. Because (hashVal & tSize) will [0, tSize], not [0, tSize). (when using prime table, hashVal % tSize be satisfied [0, tSize).) */ \
	pT          = new struct elem_m[ttSize];							\
	pHashFn     = new T_hash();											\
	elems       = 0ull;													\
	elems_maxLF = ttSize * get_maxLF(T_maxLF());						\
	maxShift    = (T_shift)0;											\
	maxShift    = ~maxShift; /* 'maxShift' will be filled with '1'. */	\
	seekLimit   = maxShift - 1;

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
inline void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::IpCHashT_constructor(uint64 tableSize){
	if      ( sstd_IpCHashT::isUint8 (T_shift()) && sstd_IpCHashT::isMaxLF50 (T_maxLF()) ){ tableSize = tableSize*2;
	}else if( sstd_IpCHashT::isUint8 (T_shift()) && sstd_IpCHashT::isMaxLF100(T_maxLF()) ){ tableSize = (uint64)(((double)tableSize)*1.429); // with uint8 and maxLT100 option, the load factor will reach a peak at about 0.70. So, 1/0.70 = 1.429.
	}else if( sstd_IpCHashT::isUint16(T_shift()) && sstd_IpCHashT::isMaxLF50 (T_maxLF()) ){ tableSize = tableSize*2;
	}else if( sstd_IpCHashT::isUint16(T_shift()) && sstd_IpCHashT::isMaxLF100(T_maxLF()) ){ tableSize = (uint64)(((double)tableSize)*1.026); // with uint16 and maxLT100 option, the load factor will reach a peak at about 0.975. So, 1/0.975 = 1.026.
	}
	
	#ifdef use_prime_table
	get_tSizeL_idx(tSizeL_idx); tSize = sstd_IpCHashT::tSizeL[tSizeL_idx];
	#else
	get_tSize(tSize); tSize_m1 = tSize - 1;
	#endif
	
	constructorBase_init_pSize_m();
	#ifdef SSTD_IpCHashT_DEBUG
	if(use_pSize_dbg){ pSize=(uint64)pSize_dbg; } // over write pSize for debug
	#endif
	constructorBase_init_m();
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major> inline sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::IpCHashT(                      ){ IpCHashT_constructor(   512   ); }
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major> inline sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::IpCHashT(const uint64 tableSize){ IpCHashT_constructor(tableSize); }

//---

#ifdef use_prime_table
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
inline sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::IpCHashT(const uint8 tableSizeL_idx, const uint64 tableSize){ // allocate same size of tableSize. for rehashing. (select size from prime table, never form the others).
	tSizeL_idx = tableSizeL_idx;
	tSize      = sstd_IpCHashT::tSizeL[tSizeL_idx];
	constructorBase_init_pSize_m();
	
	#ifdef SSTD_IpCHashT_DEBUG
	if(use_pSize_dbg){ pSize=(uint64)pSize_dbg; } // over write pSize for debug
	#endif
	constructorBase_init_m();
}
#else
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
inline sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::IpCHashT(const uint64 tableSize_minus1, const uint64 tableSize){ // allocate same size of tableSize. for rehashing. (select size of power 2, never form the others).
	tSize_m1   = tableSize_minus1;
	tSize      = tableSize;
	
	constructorBase_init_pSize_m();
	#ifdef SSTD_IpCHashT_DEBUG
	if(use_pSize_dbg){ pSize=(uint64)pSize_dbg; } // over write pSize for debug
	#endif
	constructorBase_init_m();
}
#endif

//---

#undef get_tSize
#undef get_tSizeL_idx
#undef constructorBase_init_m

//---

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
inline sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::~IpCHashT(){
	delete[] pT;
	delete pHashFn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
void swap_hashT(sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>& lhs, sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>& rhs){
	// "using std::swap;" is defined, in order to preferentially call overloaded function of swap<T>() for type T. (Ref: https://cpprefjp.github.io/reference/utility/swap.html)
	// In here, scope of using is limited by "{}", this means that scope of using is same as a usual value.
	using std::swap;
	
	#ifdef use_prime_table
	swap(lhs._tSizeL_idx(),  rhs._tSizeL_idx() );
	#else
	swap(lhs._tSize_m1(),    rhs._tSize_m1()   );
	#endif
	
	swap(lhs._tSize(),       rhs._tSize()      );
	swap(lhs._pSize(),       rhs._pSize()      );
	swap(lhs._ttSize(),      rhs._ttSize()     );
	swap(lhs._pHashFn(),     rhs._pHashFn()    );
	swap(lhs._pT(),          rhs._pT()         );
	swap(lhs._elems(),       rhs._elems()      );
	swap(lhs._elems_maxLF(), rhs._elems_maxLF());
}

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::move_hashT2vecKV(std::vector<elem_KV_m>& vecKV, sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>& hashT){
	for(auto itr=hashT.begin(); itr!=hashT.end(); ++itr){
		vecKV.push_back( elem_KV_m(std::move(itr.first_RW()), std::move(itr.second_RW())) );
	}
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::failSafe_of_rehashing(sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>& hashT){
	// "using std::swap;" is defined, in order to preferentially call overloaded function of swap<T>() for type T. (Ref: https://cpprefjp.github.io/reference/utility/swap.html)
	// In here, scope of using is limited by "{}", this means that scope of using is same as a usual value.
	using std::swap;
	
	std::vector<elem_KV_m> vecKV(hashT.size()); vecKV.clear();
	
 CONTINUE_sstd_failSafe_of_rehashing:
	move_hashT2vecKV(vecKV, hashT);
	{
		#ifdef use_prime_table
		sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major> hashT_new(hashT._tSizeL_idx()+1, sstd_IpCHashT::tSizeL[hashT._tSizeL_idx()+1]); // twice size of tSize will be allocated.
		#else
		sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major> hashT_new(hashT.tableSize()*2-1, hashT.tableSize()*2); // twice size of tSize will be allocated.
		#endif
		swap_hashT(hashT, hashT_new);
	}
	
	while(vecKV.size()!=0){
		#ifdef use_prime_table
		uint64 idx; key2tableIdx_wDivisor_m(idx, vecKV[vecKV.size()-1].key, hashT.tableSize());
		#else
		uint64 idx; key2tableIdx_wDivisor_m(idx, vecKV[vecKV.size()-1].key, hashT._tSize_m1());
		#endif
		
		#if defined(use_insert_soft) && (!defined(SSTD_IpCHashT_DEBUG))
		std::pair<struct itr_m, bool> itr_TF = hashT._insertBase_soft(std::move(vecKV[vecKV.size()-1].key), std::move(vecKV[vecKV.size()-1].val), idx);
		#else
		std::pair<struct itr_m, bool> itr_TF = hashT._insertBase_hard(std::move(vecKV[vecKV.size()-1].key), std::move(vecKV[vecKV.size()-1].val), idx); // when rehashing, there is no meaning to use _insertBase_hard() without stress test.
		#endif
		
		if(itr_TF.first.index()==itr_needRehash_m){ goto CONTINUE_sstd_failSafe_of_rehashing; }
		#ifdef SSTD_IpCHashT_DEBUG
		if(use_testFOR_dbg){ // testing failSafe_of_rehashing()
			hashT._elems()++;
			vecKV.pop_back(); // erase the tail element
			use_testFOR_dbg=false;
			goto CONTINUE_sstd_failSafe_of_rehashing;
		}
		#endif
		
		hashT._elems()++;
		vecKV.pop_back(); // erase the tail element
	}
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
inline void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::rehash(){
	// "using std::swap;" is defined, in order to preferentially call overloaded function of swap<T>() for type T. (Ref: https://cpprefjp.github.io/reference/utility/swap.html)
	// In here, scope of using is limited by "{}", this means that scope of using is same as a usual value.
	using std::swap;
	
	#ifdef use_prime_table
	sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major> hashT_new(tSizeL_idx+1, sstd_IpCHashT::tSizeL[tSizeL_idx+1]); // twice size of tSize will be allocated.
	#else
	sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major> hashT_new(tSize*2-1, tSize*2); // twice size of tSize will be allocated.
	#endif
	
	for(auto itr=this->begin(); itr!=this->end(); ){
		
		#ifdef use_prime_table
		uint64 idx; key2tableIdx_wDivisor_m(idx, itr.first(), hashT_new.tableSize());
		#else
		uint64 idx; key2tableIdx_wDivisor_m(idx, itr.first(), hashT_new._tSize_m1());
		#endif
		
		#if defined(use_insert_soft) && (!defined(SSTD_IpCHashT_DEBUG))
		std::pair<struct itr_m, bool> itr_TF = hashT_new._insertBase_soft(std::move(itr.first_RW()), std::move(itr.second_RW()), idx);
		#else
		std::pair<struct itr_m, bool> itr_TF = hashT_new._insertBase_hard(std::move(itr.first_RW()), std::move(itr.second_RW()), idx); // when rehashing, there is no meaning to use _insertBase_hard() without stress test.
		#endif
		
		if(itr_TF.first.index()==itr_needRehash_m){ failSafe_of_rehashing(hashT_new); continue; } // more rehashing is required while rehashing.
		#ifdef SSTD_IpCHashT_DEBUG
		if(use_testFOR_dbg){ // testing failSafe_of_rehashing()
			hashT_new._elems()++;
			++itr;
			failSafe_of_rehashing(hashT_new);
			continue;
		}
		#endif
		
		hashT_new._elems()++;
		++itr;
	}
	swap_hashT(*this, hashT_new);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

// What i really want to impliment is [A].
// And what i really impliment is [B]. for considering the balance between successful lookup and unsuccessful lookup.
// Some T_key types like to have a high calculation cost of "T_key_eq()(X, Y)", need to use [A].
// While the calculation cost of "T_key_eq()(X, Y)" is low, benefiting unsuccessful is more effective than successful.
// For example, if the load factor is 50%, 2 of 1 unsuccessful lookup pointting empty element.

inline bool sstd_IpCHashT::isSuccessfulMajor(sstd::IpCHashT_opt::successfulMajor   dummy){ return true;  }
inline bool sstd_IpCHashT::isSuccessfulMajor(sstd::IpCHashT_opt::unsuccessfulMajor dummy){ return false; }

#define findBase_m()													\
	if(sstd_IpCHashT::isSuccessfulMajor(T_major())){					\
		/* [A] for Successful Major Option */							\
		if(! isHead_m(pT[idx]) ){ return itr_m(maxShift, ttSize, pT, itr_end_m); } /* key is not found. */ \
		for(;;){														\
			if( T_key_eq()(pT[idx].key, key_in) ){ return itr_m(maxShift, ttSize, pT,       idx); } /* key is found. */ \
			if(   pT[idx].next == (T_shift)0    ){ return itr_m(maxShift, ttSize, pT, itr_end_m); } /* key is not found. */ \
			idx += pT[idx].next;										\
		}																\
	}else{																\
		/* [B] for Unsuccessful Major Option */							\
		for(;;){														\
			if( T_key_eq()(pT[idx].key, key_in) ){						\
				if( isEmpty_m(pT[idx]) ){ return itr_m(maxShift, ttSize, pT, itr_end_m); } /* key is not found. */ \
				return itr_m(maxShift, ttSize, pT, idx); /* key is found. */ \
			}															\
			if( pT[idx].next==(T_shift)0 ){ return itr_m(maxShift, ttSize, pT, itr_end_m); } /* key is not found. */ \
			idx += pT[idx].next;										\
		}																\
	}

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
inline struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::find(const T_key& key_in, uint64 idx){
	findBase_m();
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
inline struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::find(const T_key& key_in){
	uint64 idx; key2tableIdx_m(idx, key_in); // get table index
	findBase_m();
}
#undef findBase_m

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
std::pair<struct itr_m, bool> sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::_insertBase_soft(T_key&& key_in, T_val&& val_in, uint64 idx){
	
	// "using std::swap;" is defined, in order to preferentially call overloaded function of swap<T>() for type T. (Ref: https://cpprefjp.github.io/reference/utility/swap.html)
	// in here, scope of "using" is limited by "{}", this means that scope of "using" is same as a usual value.
	using std::swap;
	
	if( isEmpty_m(pT[idx]) ){
		// 'pT[idx]' is empty
		
		// --- case01 ---
		assert(idx<ttSize);
		pT[idx].key  = std::move(key_in);
		pT[idx].val  = std::move(val_in);
		pT[idx].prev = (T_shift)0;
		pT[idx].next = (T_shift)0;
		return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx), true);
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
//				pT[idx].key  = std::move(key_in);
//				pT[idx].val  = std::move(val_in);
				pT[idx].prev = (T_shift)0;
				pT[idx].next = (T_shift)0;
				return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx), false);
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
		return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx), true);
	}else if( isTail_m(pT[idx]) ){
		// For insertion case04, case05 and case06.
		
		// --- case04 --- ==>> pass for soft
		// --- case05 --- ==>> pass for soft
		
		// seek to an empty element until tSize.
		uint64 idx_f = idx;
		idx = idx_f + 1;
		uint64 prevIdx_f = idx_f - pT[idx_f].prev;
		seek2emptyIndex_m(pT, idx, prevIdx_f); // when false -> returned intaernally -> rehash will be occurd.
		{
			// --- case06 ---
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
			
			return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx_f), true);
		}
	}else{
		// For insertion case07, case08, case09, case10 and case11.
		uint64 idx_f = idx; // first index
		uint64 prev2next_f = (uint64)pT[idx_f].prev + (uint64)pT[idx_f].next;
		if(prev2next_f>seekLimit){ return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, itr_needRehash_m), false); }
		
		// --- case07 --- ==>> pass for soft
		// --- case08 --- ==>> pass for soft
		// --- case09 --- ==>> pass for soft
		// --- case10 --- ==>> pass for soft
		
		seek2tail(pT, idx);
		
		// seek to an empty element until tSize.
		uint64 prevIdx_e = idx; // prev index from empty
		seek2emptyIndex_m(pT, idx, prevIdx_e); // when false -> returned intaernally -> rehash will be occurd.
		{
			// --- case11 ---
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
			return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx_f), true);
		}
	}
}
#define insert_init_m()									\
	if(isOverMaxLF_m(elems, elems_maxLF)){ rehash(); }
#define insert_soft_cc_m(key_in, val_in)								\
																		\
	T_key key = key_in; /* copy -> COPY_OR_NOP <- when impliment as a define */	\
	T_val val = val_in; /* copy */										\
																		\
 CONTINUE_sstd_IpCHashT_insert_soft_cc_m:								\
	/* there is not the key-value pair on the table. */					\
	std::pair<struct itr_m, bool> itrI_TF = this->_insertBase_soft(std::move(key), std::move(val), idx); \
	if(itrI_TF.first._needRehash()){									\
		rehash();														\
		key2tableIdx_m(idx, key_in); /* get table index */				\
		goto CONTINUE_sstd_IpCHashT_insert_soft_cc_m;					\
	}																	\
	elems++;
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert_soft(const T_key& key_in, const T_val& val_in){ // copy key and value.
	insert_init_m();
	uint64 idx; key2tableIdx_m(idx, key_in);
	insert_soft_cc_m(key_in, val_in);
	return itrI_TF.first;
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert_soft(const T_key& key_in, const T_val&  val_in, uint64 idx){
	insert_init_m();
	insert_soft_cc_m(key_in, val_in);
	return itrI_TF.first;
} // copy key and value.

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
std::pair<struct itr_m, bool> sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::_insertBase_hard(T_key&& key_in, T_val&& val_in, uint64 idx){
	
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
		return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx), true);
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
				return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx), true);
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
		return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx), true);
	}else if( isTail_m(pT[idx]) ){
		// For insertion case04, case05 and case06.
		
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
					
					return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx_f), true);
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
					// --- case05 ---
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
					
					return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx_f), true);
				}
			}
		}
		
		// seek to an empty element until tSize.
		idx = idx_f + 1;
		uint64 prevIdx_f = idx_f - pT[idx_f].prev;
		seek2emptyIndex_m(pT, idx, prevIdx_f); // when false -> returned intaernally -> rehash will be occurd.
		{
			// --- case06 ---
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
			
			return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx_f), true);
		}
	}else{
		// For insertion case07, case08, case09, case10 and case11.
		
		uint64 idx_f = idx; // first index
		{
			uint64 prevIdx_e = idx - pT[idx].prev; // empty prevIdx
			for(;;){
				idx--;
				if( idx<=prevIdx_e ){ break; }
				if( isEmpty_m(pT[idx]) ){
					// --- case07 ---
					uint64 nextIdx_f = idx_f + pT[idx_f].next;
					T_shift prevShift_e = idx - prevIdx_e;
					uint64  nextShift_e = idx_f + (uint64)pT[idx_f].next - idx;
					if(nextShift_e>seekLimit){ return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, itr_needRehash_m), false); }
					
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
				
					return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx_f), true);
				}
			}
		}
		
		uint64 prev2next_f = (uint64)pT[idx_f].prev + (uint64)pT[idx_f].next;
		if(prev2next_f>seekLimit){ return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, itr_needRehash_m), false); }
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
					// --- case08 ---
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
					
					return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx_f), true);
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
				if(prevIdx_f2idx_e>seekLimit){ return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, itr_needRehash_m), false); }
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
				return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx_f), true);
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
				// --- case10 ---
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
				return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx_f), true);
			}
		}
		
		// seek to an empty element until tSize.
		seek2emptyIndex_m(pT, idx, nextIdx_e); // when false -> returned intaernally -> rehash will be occurd.
		
		{
			// --- case11 ---
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
			return std::pair<struct itr_m, bool>(itr_m(maxShift, ttSize, pT, idx_f), true);
		}
	}
}

//---

#define insert_hard_cc_m(key_in, val_in)								\
																		\
	struct itr_m itrF = this->find(key_in, idx);						\
	if(itrF!=this->end()){ itrF.second_RW()=val_in; return itrF; }		\
																		\
	T_key key = key_in; /* copy -> COPY_OR_NOP <- when impliment as a define */	\
	T_val val = val_in; /* copy */										\
 CONTINUE_sstd_IpCHashT_hard_cc_m:										\
	/* there is not the key-value pair on the table. */					\
	std::pair<struct itr_m, bool> itrI_TF = this->_insertBase_hard(std::move(key), std::move(val), idx); \
	if(itrI_TF.first._needRehash()){									\
		rehash();														\
		key2tableIdx_m(idx, key_in); /* get table index */				\
		goto CONTINUE_sstd_IpCHashT_hard_cc_m;							\
	}																	\
	elems++;
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert_hard(const T_key& key_in, const T_val& val_in){ // copy key and value.
	insert_init_m();
	uint64 idx; key2tableIdx_m(idx, key_in); // get table index
	insert_hard_cc_m(key_in, val_in);
	return itrI_TF.first;
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert_hard(const T_key& key_in, const T_val&  val_in, uint64 idx){
	insert_init_m();
	insert_hard_cc_m(key_in, val_in);
	return itrI_TF.first;
} // copy key and value.
//template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major> void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert(      T_key&& key_in, const T_val&  val_in){} // swap key.           (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.add(std::move(key),           val );".)
//template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major> void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert(const T_key&  key_in,       T_val&& val_in){} // swap value.         (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.add(          key , std::move(val));".)
//template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major> void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert(      T_key&& key_in,       T_val&& val_in){} // swap key and value. (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.add(std::move(key), std::move(val));".)

//-----------------------------------------------------------------------------------------------------------------------------------------------

#ifdef use_insert_soft

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert(const T_key& key_in, const T_val& val_in){ // copy key and value.
	insert_init_m();
	uint64 idx; key2tableIdx_m(idx, key_in);
	insert_soft_cc_m(key_in, val_in);
	return itrI_TF.first;
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert(const T_key& key_in, const T_val&  val_in, uint64 idx){ // copy key and value.
	insert_init_m();
	insert_soft_cc_m(key_in, val_in);
	return itrI_TF.first;
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
std::pair<struct itr_m, bool> sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert(const value_type& v){ // for STL (1). Ref: https://cpprefjp.github.io/reference/unordered_map/unordered_map/insert.html
	insert_init_m();
	uint64 idx; key2tableIdx_m(idx, v.first);
	insert_soft_cc_m(v.first, v.second);
	return itrI_TF;
}

#else

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert(const T_key& key_in, const T_val& val_in){ // copy key and value.
	insert_init_m();
	uint64 idx; key2tableIdx_m(idx, key_in);
	insert_hard_cc_m(key_in, val_in);
	return itrI_TF.first;
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
struct itr_m sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert(const T_key& key_in, const T_val&  val_in, uint64 idx){ // copy key and value.
	insert_init_m();
	insert_hard_cc_m(key_in, val_in);
	return itrI_TF.first;
}
template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
std::pair<struct itr_m, bool> sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::insert(const value_type& v){ // for STL (1). Ref: https://cpprefjp.github.io/reference/unordered_map/unordered_map/insert.html
	insert_init_m();
	uint64 idx; key2tableIdx_m(idx, v.first);
	insert_hard_cc_m(v.first, v.second);
	return itrI_TF;
}

#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
inline T_val& sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::operator[](const T_key& key_in){
	insert_init_m();
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

//template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
//void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::erase(const T_key& key_in, uint64 idx);

template <class T_key, class T_val, class T_hash, class T_key_eq, typename T_shift, typename T_maxLF, typename T_major>
void sstd::IpCHashT<T_key, T_val, T_hash, T_key_eq, T_shift, T_maxLF, T_major>::erase(const T_key& key_in){
	
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

#undef insert_hard_cc_m
#undef insert_soft_cc_m
#undef insert_init_m

#undef key2tableIdx_m
#undef seek2tail
#undef seek2emptyIndex_m
#undef isTail_m
#undef isHead_m
#undef isEmpty_m
#undef isOverMaxLF_m

#undef itr_needRehash_m
#undef itr_end_m

#undef elem_KV_m
#undef elem_m
#undef itr_m

//-----------------------------------------------------------------------------------------------------------------------------------------------

#ifdef use_insert_soft
	#undef use_insert_soft
#endif

#ifdef use_prime_table
	#undef use_prime_table
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------
