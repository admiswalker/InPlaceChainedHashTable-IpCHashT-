#pragma once
#include "./typeDef.h"
#include <sstd/sstd.hpp> // for debug
#include <memory>

//-----------------------------------------------------------------------------------------------------------------------------------------------
// compile options

#define use_prime_table

//-----------------------------------------------------------------------------------------------------------------------------------------------

#define elem_m sstd_CHashT::element<T_key,T_val> // a macro of table element structure
#define itr_m sstd_CHashT::iterator<T_key,T_val> // a macro of iterator

//-----------------------------------------------------------------------------------------------------------------------------------------------

namespace sstd{
	template <class T_key,
			  class T_val,
			  class T_hash   = std::hash<T_key>,
			  class T_key_eq = std::equal_to<T_key>
			  >
	class CHashT; // chained hash table
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

namespace sstd_CHashT{
	template <class T_key, class T_val> struct element; 
	template <class T_key, class T_val> struct iterator;
	
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
		18446744073709551615ull  // 2^ 64 - 1   = 18446744073709551615ull
	};
	#endif
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val>
struct sstd_CHashT::element{
private:
public:
	inline element(){ isUsed=false; pNext=0; }
	inline ~element(){ if(pNext!=0){delete pNext; pNext=0;} } // recursive release of the memory
	
	bool isUsed;          // flag
	T_key key;            // key
	T_val val;            // value
	struct elem_m* pNext; // singly linked list
	
	#ifdef SSTD_CHashT_DEBUG
	inline void print_dbg(){
		std::cout << " pElem: " <<  this                             << std::endl;
		std::cout << "isUsed: " << (this->isUsed ? "true" : "false") << std::endl;
		std::cout << "   key: " <<  this->key                        << std::endl;
		std::cout << "   val: " <<  this->val                        << std::endl;
		std::cout << " pNext: " <<  this->pNext                      << std::endl;
	}
	#endif
};

//-----------------------------------------------------------------------------------------------------------------------------------------------
/*
template <class T_key, class T_val>
struct sstd_CHashT::iterator{
private:
	const struct elem_m* pBegin; // pBegin = &pT[0];
	const struct elem_m* pEnd;   // pEnd = pBegin + tSize * sizeof(struct elem_m);
	
	bool TF; // true or false
	struct elem_m* pHead;
	struct elem_m* pElem;
	struct elem_m* pPrev;
	
public:
	inline struct elem_m*& _pHead(){ return pHead; }
	inline struct elem_m*& _pElem(){ return pElem; }
	inline struct elem_m*& _pPrev(){ return pPrev; }
	
	inline iterator(){ pBegin=0ull; pEnd=0ull; TF=false; pHead=0ull; pElem=0ull; pPrev=0ull; }
	inline iterator(const struct elem_m* pBegin_in, const struct elem_m* pEnd_in, const bool TF_in, struct elem_m* pH, struct elem_m* pE, struct elem_m* pP){ pBegin=pBegin_in; pEnd=pEnd_in; TF=TF_in; pHead=pH; pElem=pE; pPrev=pP; }
	inline ~iterator(){}
	
	inline const T_key& key()           { return pElem->key; }
	inline       T_key& _key_nonConst(){ return pElem->key; } // if a key will be changed, the table sorted by the mod of hash value will be broken.
	inline T_val& val(){ return pElem->val; }
	
	// cast operator
	inline operator bool(){ return TF; }
	
	// operator
	inline const bool operator==(uint64& rhs){ return (uint64)pElem==rhs; }
	
	inline const bool operator!=(const struct itr_m_old& rhs){
		return !(this->TF     == rhs.TF     &&
				 this->pHead  == rhs.pHead  &&
				 this->pElem  == rhs.pElem  &&
				 this->pPrev  == rhs.pPrev  &&
				 
				 this->pBegin == rhs.pBegin &&
				 this->pEnd   == rhs.pEnd     );
	}
	
	// pre-increment (++itr)
	inline struct itr_m_old operator++(){
		if(pElem!=0 && pElem->pNext!=0ull){ // "pElem!=0" is for itr after erasing a element will not access.
			pPrev = pElem;
			pElem = pElem->pNext;
			return *this;
		}
		pHead++;
		for(; pHead<pEnd; pHead++){
			if(pHead->isUsed){
				pElem = pHead;
				pPrev = 0ull;
				return *this;
			}
		}
		pBegin = 0ull;
		pEnd   = 0ull;
		TF     = false;
		pHead  = 0ull;
		pElem  = 0ull;
		pPrev  = 0ull;
		return *this;
	}
	
	// post-increment (itr++)
//	class itr_m_old operator++(int){ // int is a dummy arg
//		== not implimented yet. ==
//	}
	
	#ifdef SSTD_CHashT_DEBUG
	inline void print_dbg(){ // print internal variables for debug
		std::cout << "pBegin: " << pBegin << std::endl;
		std::cout << "  pEnd: " <<   pEnd << std::endl;
		std::cout << "    TF: " <<     TF << std::endl;
		std::cout << " pHead: " <<  pHead << std::endl;
		std::cout << " pElem: " <<  pElem << std::endl;
		std::cout << " pPrev: " <<  pPrev << std::endl;
		return;
	}
	#endif
};
//*/

#define itr_end_m        1
#define itr_needRehash_m 2

template <class T_key, class T_val>
struct sstd_CHashT::iterator{
private:
	struct elem_m* pT; // table pointer
	uint64 idx;        // table index
	struct elem_m* pP; // previous element pointer
	struct elem_m* pE; // current element pointer
	struct elem_m* pN; // next element pointer
	uint8 state;
	
public:
	inline iterator(){ pT=NULL; idx=0ull; pP=NULL; pE=NULL; pN=NULL; state=0; }
	inline iterator(struct elem_m* pT_in, const uint64 tIdx_in, struct elem_m* pP_in, struct elem_m* pE_in, struct elem_m* pN_in, const uint8 state_in){
		pT    = pT_in;
		idx   = tIdx_in;
		pP    = pP_in;
		pE    = pE_in;
		pN    = pN_in;
		state = state_in;
	}
	inline ~iterator(){}
	
	const T_key&  first(){ return pE->key; }
	const T_val& second(){ return pE->val; }
	
	uint64 index(){ return idx; }
	struct elem_m*  _pHead(){ return &pT[idx]; }
	struct elem_m*& _pPrev(){ return pP;       }
	struct elem_m*& _pElem(){ return pE;       }
	struct elem_m*& _pNext(){ return pN;       }
	
	const uint8 state_R(){ return state; }
};

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq>
class sstd::CHashT{
private:
	void CHashT_constructor(const uint64& tableSize);
	
	uint8  tSizeL_idx;     // table size list index
	uint64 tSize;          // table size
	
	T_hash* pHashFn;       // pointer to the hash function
	
	struct elem_m* pT;     // pointer to the table
	struct elem_m* pBegin; // = &pT[0]
	struct elem_m* pEnd;   // = &pT[0] + tSize*sizeof(struct elem_m)
	uint64 elems;          // number of elements on the table
	
//	double lf;             // load factor
	inline uint64 get_tSizeL_idx(const uint64& tableSize); // get table size list index
	
public:
	inline uint8&   _tSizeL_idx(){ return tSizeL_idx; }
	inline uint64&       _tSize(){ return tSize;      }
//	inline T_hash*&    _pHashFn(){ return pHashFn;    }
	inline struct elem_m*& _pT    (){ return pT;     }
	inline struct elem_m*& _pBegin(){ return pBegin; }
	inline struct elem_m*& _pEnd  (){ return pEnd;   }
	inline uint64&         _elems (){ return elems;  }
	
	CHashT();
	CHashT(const uint64& tableSize); // allocate twice size of tableSize.
	~CHashT();
	
	// iterator
	/*
	inline struct itr_m_old begin(){
		uint64 tIdx=0ull;
		while(!pT[tIdx].isUsed){
			tIdx++;
			if(tIdx>=tSize){
				// there is no item.
				return itr_m_old(0ull, 0ull, false, 0ull, 0ull, 0ull); // return .end()
			}
		}
		return itr_m_old(pBegin, pEnd, true, &pT[tIdx], &pT[tIdx], 0ull);
	}
	inline const struct itr_m_old end(){ return itr_m_old(0ull, 0ull, false, 0ull, 0ull, 0ull); }
	//*/
	inline const struct itr_m end(){ return itr_m(pT, 0ull, NULL, NULL, NULL, itr_end_m); }
	inline const bool operator!=(const struct itr_m& rhs){ return this->state != rhs.state_R(); }
	
	T_val& operator[](const T_key&  rhs);
	T_val& operator[](      T_key&& rhs);
	
	// bucket();
	// bucket_count();
	// bucket_size();
	// clear();
	// load_factor();
	// rehash();
	// reserve();
	
	// operator[];
	// operator =();
	// operator==();
	// operator!=();
	
	// ---------------------------
	
	struct itr_m insert(const T_key&  key_in, const T_val&  val_in); // copy key and value.
	struct itr_m insert(      T_key&& key_in, const T_val&  val_in); // swap key.           (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.insert(std::move(key),           val );".)
	struct itr_m insert(const T_key&  key_in,       T_val&& val_in); // swap value.         (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.insert(          key , std::move(val));".)
	struct itr_m insert(      T_key&& key_in,       T_val&& val_in); // swap key and value. (Callable by "sstd::CHashT<T_key, T_val> hashT; hashT.insert(std::move(key), std::move(val));".)
	
	struct itr_m insert(struct itr_m& itr, const T_key&  key_in); // insert from itr value of find() for operator[]
	struct itr_m insert(struct itr_m& itr,       T_key&& key_in); // insert from itr value of find() for operator[]
	
	struct itr_m find  (const T_key& key_in, const uint64 idx);
	struct itr_m find  (const T_key& key_in);
	
	bool         erase (struct itr_m&   itr); // erase from itr value of insert() or find()
	bool         erase (const T_key& key_in); // erase from key value
	
	inline const uint64 size(){ return elems; }
	inline const uint64 tableSize(){ return tSize; }
	inline const uint64 bucket_count(){ return tSize; }
	inline const double load_factor(){ return (double)elems/(double)tSize; }
	
	void rehash();
	
	#ifdef SSTD_CHashT_DEBUG
	bool   use_tIdx_dbg = false;
	uint64     tIdx_dbg;
	inline struct elem_m*& pT_dbg(){ return pT; }
	#endif
};

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq>
inline uint64 sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::get_tSizeL_idx(const uint64& tableSize){
	
	uint idx=0;
	for(; idx<64; idx++){
		if(sstd_CHashT::tSizeL[idx]>=tableSize){ break; }
	}
	idx++; // twice size of table will adjust the load factor 50%.
	return idx;
}
template <class T_key, class T_val, class T_hash, class T_key_eq>
inline void sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::CHashT_constructor(const uint64& tableSize){
	
	tSizeL_idx = get_tSizeL_idx(tableSize);
	tSize = sstd_CHashT::tSizeL[tSizeL_idx];
	
	#ifdef use_prime_table
	pT      = new struct elem_m[tSize];
	#else
	pT      = new struct elem_m[tSize + 1];
	#endif
	
	pBegin = &pT[0];
	
	#ifdef use_prime_table
	pEnd   = &pT[0] + tSize; // never "&pT[0] + tSize*sizeof(struct elem_m)", because " * sizeof(struct elem_m)" will be automatically calculated by compiler.
	#else
	pEnd   = &pT[0] + tSize + 1; // never "&pT[0] + tSize*sizeof(struct elem_m)", because " * sizeof(struct elem_m)" will be automatically calculated by compiler.
	#endif
	
	pHashFn = new T_hash();
	
	elems = 0ull;
}
template <class T_key, class T_val, class T_hash, class T_key_eq> inline sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::CHashT(                       ){ CHashT_constructor(   512   ); } // in order to store 512 elements, 1024 table length will be allocated.
template <class T_key, class T_val, class T_hash, class T_key_eq> inline sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::CHashT(const uint64& tableSize){ CHashT_constructor(tableSize); }

template <class T_key, class T_val, class T_hash, class T_key_eq>
inline sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::~CHashT(){
	delete[] pT;
	delete pHashFn;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq>
inline void sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::rehash(){
	sstd::CHashT<T_key, T_val, T_hash, T_key_eq> hashT_new(tSize); // twice size of tSize will be allocated.
	
	// "using std::swap;" is defined, in order to preferentially call overloaded function of swap<T>() for type T. (Ref: https://cpprefjp.github.io/reference/utility/swap.html)
	// in here, scope of using is limited by "{}", this means that scope of using is same as a usual value.
	using std::swap;
	
	for(auto itr=this->begin(); itr!=this->end(); ++itr){
		hashT_new.insert(std::move(itr._key_nonConst()), std::move(itr.val()));
	}
	this->tSizeL_idx = hashT_new._tSizeL_idx(); // this->_tSizeL_idx() = hashT_new._tSizeL_idx();
	this->tSize      = hashT_new._tSize();
//	this->pHashFn    = hashT_new._pHashFn();
	swap(this->pT, hashT_new._pT());
	this->pBegin = hashT_new._pBegin();
	this->pEnd   = hashT_new._pEnd();
//	this->elems = hashT_new._elems();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

#define insert_preproc_m()												\
																		\
	/* checking the needs of rehash. */									\
	if(elems>=tSize){ rehash(); } /* elems needs to be measured before find(), because of the address that the itr suggests will be changed. */ \
																		\
	auto itr = find(key_in);											\
	if(itr.state_R()!=itr_end_m){ return itr; } /* key is already on the table */

#define insert_m(key_in, val_in, CAST_KEY, CAST_VAL)					\
																		\
	elems++;															\
	if(itr._pElem()!=NULL){												\
		/* inserting on the table */									\
																		\
		itr._pElem()->isUsed = true;									\
		itr._pElem()->key    = (CAST_KEY)(key_in);						\
		itr._pElem()->val    = (CAST_VAL)(val_in);						\
		return itr_m(pT, itr.index(), NULL, itr._pElem(), NULL, 0);		\
	}else{																\
		/* inserting on the singly linked list */						\
		/* find() guarantees that itr->pElem() is the tail of singly linked list */	\
																		\
		itr._pPrev()->pNext = new struct elem_m();						\
		itr._pPrev()->pNext->isUsed = true;								\
		itr._pPrev()->pNext->key    = (CAST_KEY)(key_in);				\
		itr._pPrev()->pNext->val    = (CAST_VAL)(val_in);				\
		return itr_m(pT, itr.index(), itr._pPrev(), itr._pElem(), NULL, 0); \
	}

template <class T_key, class T_val, class T_hash, class T_key_eq> inline struct itr_m sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::insert(const T_key&  key_in, const T_val&  val_in){ insert_preproc_m(); insert_m(key_in, val_in, T_key,     T_val    ); }
template <class T_key, class T_val, class T_hash, class T_key_eq> inline struct itr_m sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::insert(      T_key&& key_in, const T_val&  val_in){ insert_preproc_m(); insert_m(key_in, val_in, std::move, T_val    ); }
template <class T_key, class T_val, class T_hash, class T_key_eq> inline struct itr_m sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::insert(const T_key&  key_in,       T_val&& val_in){ insert_preproc_m(); insert_m(key_in, val_in, T_key,     std::move); }
template <class T_key, class T_val, class T_hash, class T_key_eq> inline struct itr_m sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::insert(      T_key&& key_in,       T_val&& val_in){ insert_preproc_m(); insert_m(key_in, val_in, std::move, std::move); }

//---

#define insert_by_itr_m(key_in, CAST_KEY)								\
																		\
	/* checking the needs of rehash. */									\
	if(elems>=tSize){													\
		rehash();														\
		itr = find(key_in); /* because of the address that the itr suggests was changed by rehash(), we needs to find again. */ \
	}																	\
																		\
	T_val val_in = T_val(); /* generation of empty buf for insert() */

template <class T_key, class T_val, class T_hash, class T_key_eq> inline struct itr_m sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::insert(struct itr_m& itr, const T_key&  key_in){ insert_by_itr_m(key_in, T_key    ); insert_m(key_in, val_in, T_key,     T_val    ); }
template <class T_key, class T_val, class T_hash, class T_key_eq> inline struct itr_m sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::insert(struct itr_m& itr,       T_key&& key_in){ insert_by_itr_m(key_in, std::move); insert_m(key_in, val_in, std::move, std::move); }

//---

#undef insert_by_itr_m
#undef insert_m
#undef insert_preproc_m

//-----------------------------------------------------------------------------------------------------------------------------------------------

// in order to reduce the calling time of function, macro expansion will be used.
#ifdef use_prime_table
	#ifdef SSTD_CHashT_DEBUG
	#define key2tIdx_m(tIdx, key)											\
		uint64 hVal = (uint64)(*pHashFn)(key); /* generate hashed value  */	\
		tIdx = hVal % tSize;											    \
		if(use_tIdx_dbg){ idx=tIdx_dbg; } /* over write idx for debug */
	#else
	#define key2tIdx_m(tIdx, key)											\
		uint64 hVal = (uint64)(*pHashFn)(key); /* generate hashed value  */	\
		tIdx = hVal % tSize;
	#endif
#else
	#ifdef SSTD_CHashT_DEBUG
	#define key2tIdx_m(tIdx, key)											\
		uint64 hVal = (uint64)(*pHashFn)(key); /* generate hashed value  */	\
		tIdx = hVal & tSize;
	#else
	#define key2tIdx_m(tIdx, key)											\
		uint64 hVal = (uint64)(*pHashFn)(key); /* generate hashed value  */	\
		tIdx = hVal & tSize;											    \
		if(use_tIdx_dbg){ idx=tIdx_dbg; } /* over write idx for debug */
	#endif
#endif

#define findBase_m()													\
																		\
	if(!pT[idx].isUsed){ return itr_m(pT, idx, NULL, NULL, NULL, itr_end_m); } /* key is not found. */ \
																		\
	struct elem_m* pP = NULL;     /* previous element pointer */		\
	struct elem_m* pE = &pT[idx]; /* current element pointer */			\
	while(pE!=NULL){													\
		if(T_key_eq()(pE->key, key_in)){ return itr_m(pT, idx, pP, pE, pE->pNext, 0); } /* key is found. */ \
		pP = pE;														\
		pE = pE->pNext;													\
	}																	\
	return itr_m(pT, idx, pP, NULL, NULL, itr_end_m); /* key is not found. */

template <class T_key, class T_val, class T_hash, class T_key_eq>
inline struct itr_m sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::find(const T_key& key_in, const uint64 idx){
	findBase_m();
}
template <class T_key, class T_val, class T_hash, class T_key_eq>
inline struct itr_m sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::find(const T_key& key_in){
	uint64 idx; key2tIdx_m(idx, key_in);
	findBase_m();
}

#undef findBase_m
#undef key2tIdx_m

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq>
inline bool sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::erase(struct itr_m& itr){
	// "using std::swap;" is defined, in order to preferentially call overloaded function of swap<T>() for type T. (Ref: https://cpprefjp.github.io/reference/utility/swap.html)
	// in here, scope of using is limited by "{}", this means that scope of using is same as a usual value.
	using std::swap;
	
	if(itr.state_R()==itr_end_m){ return false; } // key is not found
	
	elems--;
	if(itr._pPrev()==NULL && itr._pNext()==NULL){
		// case 1. erase an element on the table without element on the singly linked list.
		
		T_key keyBuf;// in order to call destructor
		T_val valBuf;// in order to call destructor
		itr._pElem()->isUsed = false;
		swap(itr._pElem()->key, keyBuf);
		swap(itr._pElem()->val, valBuf);
	}else if(itr._pPrev()==NULL && itr._pNext()!=NULL){
		// case 2. erase an element on the table with element(s) on the singly linked list.
		
		// <- on the table ...... -> | <- on the singly linked list ......................... -> |
		//
		//  <1>                           <2>                         <3>
		//  itr._pElem()             +->  itr._pElem()->pNext    +->  itr._pElem()->pNext->pNext
		//  == &pT[idx]              |    == itr._pNext()        |    == itr._pNext()->pNext
		// [ isUsed               ]  |   [ isUsed            ]   |   [ isUsed                   ]
		// [ key           <1-1>  ]  |   [ key        <2-1>  ]   |   ...
		// [ val           <1-2>  ]  |   [ val        <2-2>  ]   |
		// [ pNext         <1-3>  ] -+   [ pNext      <2-3>  ] --+

		// table
		swap(itr._pElem()->key, itr._pNext()->key); // <1-1>, <2-1>
		swap(itr._pElem()->val, itr._pNext()->val); // <1-2>, <2-2>
		struct elem_m* pENN_buf = itr._pNext()->pNext; // <2-3>
		itr._pNext()->pNext = NULL; // <2-3> // deleting "itr._pElem()->pNext" without filling "itr._pElem()->pNext->pNext" with zero will cause the recursive release of the memory.
		delete itr._pNext(); // <2>
		itr._pElem()->pNext = pENN_buf; // <1-3> = &<3>
		
		// itr
		itr._pNext()=itr._pElem(); // for operator++.
		itr._pElem()=NULL;         // for operator++ will not access.
	}else{
		// case 3. erase element on the singly linked list. ("case 3" is a interchangeable process by "case 2", while ignoring the over head.)
		
		itr._pPrev()->pNext = itr._pNext();
		itr._pElem()->pNext = NULL; // deleting "itr._pElem" without filling "itr._pElem->pNext" with zero will cause the recursive release of the memory.
		delete itr._pElem();
		
		//itr._pElem() = itr._pHead();
		// inside itr
		//   [ pBegin ]
		//   [ pEnd   ]
		//   [ TF     ]
		//   [ pHead  ]
		//   [ pElem  ]
		//   [ pPrev  ]
	}
	
	return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

template <class T_key, class T_val, class T_hash, class T_key_eq>
inline bool sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::erase(const T_key& key_in){
	auto itr = find(key_in);
	return this->erase(itr);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

#define insert_OPE_bracket_m(CAST_KEY)									\
	auto itrF = find(key_in);											\
	if(itrF.state_R()!=itr_end_m){ return itrF._pElem()->val; }			\
																		\
	auto itrA = insert(itrF, (CAST_KEY)(key_in));						\
	return itrA._pElem()->val;

template <class T_key, class T_val, class T_hash, class T_key_eq> inline T_val& sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::operator[](const T_key&  key_in){ insert_OPE_bracket_m(T_key    ); }
template <class T_key, class T_val, class T_hash, class T_key_eq> inline T_val& sstd::CHashT<T_key, T_val, T_hash, T_key_eq>::operator[](      T_key&& key_in){ insert_OPE_bracket_m(std::move); }
#undef insert_OPE_bracket_m

//-----------------------------------------------------------------------------------------------------------------------------------------------

#undef itr_end_m
#undef itr_needRehash_m

#undef elem_m
#undef itr_m

#ifdef SSTD_CHashT_DEBUG
#undef SSTD_CHashT_DEBUG
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------

#ifdef use_prime_table
	#undef use_prime_table
#endif

