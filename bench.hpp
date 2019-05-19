#include "./CHashT.hpp"
#include "./IpCHashT.hpp"
#include <time.h>
#include <random>
#include <unordered_map>
#include <sparsehash/dense_hash_map> // sparsehash-master

//-----------------------------------------------------------------------------------------------------------------------------------------------
// add

template<typename T_hashTable>
void bench_add(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_ms){
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	
	uint64 interval = 1;
	
	for(;;){
		time_m timem;
		sstd::measureTime_start(timem);
		
		// add
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			hashT[r] = r;
		}
		
		vecX_num <<= hashT.size();
		
		double nsec = sstd::measureTime_stop_ns(timem);
		vecY_quely_per_ms <<= ((double)interval * 1000.0) / (nsec);
		
		uint8 order = (uint8)std::log10((double)hashT.size());
		interval = (uint64)std::pow(10, order);
		if(interval!=1){interval/=10;}
		if(hashT.size()+interval>limitSize){ break; }
	}
}
void bench_plot_add(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d; // quely_per_ms
	{     std::unordered_map<uint64,uint64> hashT(initSize);                            bench_add(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize);                            bench_add(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize);                            bench_add(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); bench_add(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Additional speed [query/ms]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d}; // quely_per_ms
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// add (elapsed time)

template<typename T_hashTable>
void bench_add_et(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_s){
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator

	{
		// warm running
		
		// add
		for(uint i=0; i<100; i++){
			uint64 r = rand();
			hashT[r] = r;
		}
		
		// erase all
//		for(auto itr=hashT.begin(); itr!=hashT.end(); ++itr){
//			hashT.erase(itr);
//		}
	}
	
	uint64 splitNum  = 100;
	uint64 interval  = limitSize/splitNum;
	
	double totalTime_sec = 0.0;
	
//	vecY_s   <<= 0.0;
//	vecX_num <<= 0;
	for(uint sn=0; sn<splitNum; sn++){
		time_m timem; sstd::measureTime_start(timem);
		
		// add
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			hashT[r] = r;
		}
		
		double lapTime_sec = sstd::measureTime_stop_ms(timem) / 1000.0;
		totalTime_sec += lapTime_sec;
		vecY_s   <<= totalTime_sec;
		vecX_num <<= hashT.size();
	}
	vecY_s -= (double)vecY_s[0];
}
void bench_plot_add_et(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d; // sec
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); // this meen that 'NULL' and '1' will not be able to insert as a key-value.
		                                                     bench_add_et(hashT, limitSize, vecX_d, vecY_d); }
	{     std::unordered_map<uint64,uint64> hashT(initSize);                            bench_add_et(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize);                            bench_add_et(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize);                            bench_add_et(hashT, limitSize, vecX_i, vecY_i); }
//	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); bench_add_et(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot_et";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Elapsed time [sec]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d}; // sec
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// find()

template<typename T_hashTable>
void bench_find(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_ms){
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	
	uint64 interval = 1;
	std::vector<uint64> vecKeyVal(limitSize); vecKeyVal.clear();
	
	std::mt19937 engine(seed_gen()); // for std::shuffle()
	
	for(;;){
		// add
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			hashT[r] = r;
			vecKeyVal <<= r;
		}
		std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
		
		vecX_num <<= hashT.size();
		
		// find (all elements are found)
		time_m timem; sstd::measureTime_start(timem);
		for(uint i=0; i<interval; i++){
			uint64 keyVal = vecKeyVal[i];
			if(hashT[keyVal] != keyVal){ sstd::pdbg("ERROR: key val is not same."); exit(0); }
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		vecY_quely_per_ms <<= ((double)interval * 1000.0) / (nsec);
		
		uint8 order = (uint8)std::log10((double)hashT.size());
		interval = (uint64)std::pow(10, order);
		if(interval!=1){interval/=10;}
		if(hashT.size()+interval>limitSize){ break; }
	}
}
void bench_plot_find(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d; // quely_per_ms
	{     std::unordered_map<uint64,uint64> hashT(initSize);                            bench_find(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize);                            bench_find(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize);                            bench_find(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); bench_find(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Find speed [query/ms]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d}; // quely_per_ms
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// find: all lookup is failed

template<typename T_hashTable>
void bench_find_failedAll(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_ms){
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	
	uint64 interval = 1;
	std::vector<uint64> vecKeyVal(limitSize); vecKeyVal.clear();
	
	std::mt19937 engine(seed_gen()); // for std::shuffle()
	
	uint64 numFound=0, numNotFound=0;
	for(;;){
		// add
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			hashT[r] = r;
			vecKeyVal <<= r;
		}
		std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
		
		vecX_num <<= hashT.size();
		
		// find (all elements are not found)
		time_m timem; sstd::measureTime_start(timem);
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			auto itr = hashT.find(r);
			if(itr!=hashT.end()){ numFound++; }else{ numNotFound++; }
//			uint64 keyVal = vecKeyVal[i];
//			if(hashT[keyVal] != keyVal){ sstd::pdbg("ERROR: key val is not same."); exit(0); }
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		vecY_quely_per_ms <<= ((double)interval * 1000.0) / (nsec);
		
		uint8 order = (uint8)std::log10((double)hashT.size());
		interval = (uint64)std::pow(10, order);
		if(interval!=1){interval/=10;}
		if(hashT.size()+interval>limitSize){ break; }
	}
	printf("%lu / %lu = %lf\n", numFound, numFound+numNotFound, (double)numFound/(double)(numFound+numNotFound));
}
void bench_plot_find_failedAll(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d; // quely_per_ms
	{     std::unordered_map<uint64,uint64> hashT(initSize);                            bench_find_failedAll(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize);                            bench_find_failedAll(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize);                            bench_find_failedAll(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); bench_find_failedAll(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Find speed [query/ms]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d}; // quely_per_ms
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// find-erase-add cycle

template<typename T_hashTable>
void bench_find_erase_add(T_hashTable& hashT, const uint64 initSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_ms){
	
	std::random_device seed_gen;
	
	std::vector<uint64> vecKeyVal(initSize); vecKeyVal.clear();
	
	// add elements to the num of limitSize
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	while(hashT.size()<initSize){
		uint64 r = rand();
		hashT[r] = r;
		vecKeyVal <<= r;
	}
	
	// shuffle "vecKeyVal"
	std::mt19937 engine(seed_gen()); // for std::shuffle()
	std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
	
	uint8 order = (uint8)std::log10((double)hashT.size());
	uint64 interval = (uint64)std::pow(10, order); if(interval!=1){interval/=10;}
	
	double change_per_cycle = 0.05; // [%]
	double nonChangedRate   = 1.00; // [%]
	double endRate          = 0.0001; // [%]
	uint64 changeNum_per_cycle = (double)hashT.size() * change_per_cycle; // [count]
	while(nonChangedRate > endRate){
		
		time_m timem; sstd::measureTime_start(timem);
		// find (with all elements found)
		for(uint i=0; i<interval; i++){
			uint64 keyVal = vecKeyVal[i];
			if(hashT[keyVal] != keyVal){ sstd::pdbg("ERROR: key val is not same."); exit(0); }
		}
		// erase change_per_cycle [%]
		for(uint i=0; i<changeNum_per_cycle; i++){
			hashT.erase( vecKeyVal[vecKeyVal.size()-1] );
			vecKeyVal.pop_back();
		}
		// add change_per_cycle [%]
		for(uint i=0; i<changeNum_per_cycle; i++){
			uint64 r = rand();
			hashT[r] = r;
			vecKeyVal <<= r;
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		
		vecY_quely_per_ms <<= ((double)interval * 1000.0) / (nsec);
		vecX_num          <<= nonChangedRate;
		nonChangedRate *= (1.00 - change_per_cycle);
		
		// shuffle vecKeyVal
		std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
	}
}
void bench_plot_find_erase_add(const char* savePath, const uint64 initSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d; // quely_per_ms
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_find_erase_add(hashT, initSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_find_erase_add(hashT, initSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_find_erase_add(hashT, initSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); // this meen that 'NULL' and '1' will not be able to insert as a key-value.
		                                                     bench_find_erase_add(hashT, initSize, vecX_d, vecY_d); }
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot_wErase";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Non-changed rate [\%]";
	const char* ylabel   = "A speed of one find-erase-add cycle [query/ms]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d}; // quely_per_ms
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// erase

template<typename T_hashTable>
void bench_erase(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_s){
	
	std::vector<uint64> vecKeyVal(limitSize); vecKeyVal.clear();
	
	// add elements to the num of limitSize
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	while(hashT.size()<limitSize){
		uint64 r = rand();
		hashT[r] = r;
		vecKeyVal <<= r;
	}
	
	// shuffle "vecKeyVal"
	std::mt19937 engine(seed_gen()); // for std::shuffle()
	std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
	
	uint64 splitNum  = 100;
	uint64 interval  = limitSize/splitNum;
	
	time_m timem;
	sstd::measureTime_start(timem);
	
	uint64 eraseElemNum=0; // erasion element number
	for(uint sn=0; sn<splitNum; sn++){
		// erase
		for(uint i=0; i<interval; i++){
			hashT.erase( vecKeyVal[eraseElemNum] );
			eraseElemNum++;
		}
		
		vecX_num <<= hashT.size();
		
		double sec = sstd::measureTime_stop_ms(timem) / 1000.0;
		vecY_s <<= sec;
	}
}
void bench_plot_erase(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d; // quely_per_ms
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_erase(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_erase(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_erase(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); // this meen that 'NULL' and '1' will not be able to insert as a key-value.
		                                                     bench_erase(hashT, limitSize, vecX_d, vecY_d); }
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot_et_invAxisX";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]\n(This axis is inverted.)";
	const char* ylabel   = "Elapsed time [sec]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d}; // quely_per_ms
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

void bench_maxLoadFactor(){
	
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

void RUN_ALL_BENCHS(){
//	const uint64 limitSize = 500000000; // buy more memory
//	const uint64 limitSize = 200000000; // limit of memory
//	const uint64 limitSize = 50000000; // limit of current graph
	const uint64 limitSize = 5000000;  // 50 sec
	const uint64 initSize_wRehash  = 0;
	const uint64 initSize_preAlloc = limitSize;
	/*
	// Warm running, because of the first bench usually returns bad result.
	bench_plot_add("./bench_warmRunning.png", initSize_preAlloc, limitSize); // pre-allocate
	
	// add: Additional speed [query/sec]
	bench_plot_add("./bench_add_wRehash.png",  initSize_wRehash,  limitSize); // with rehash
	bench_plot_add("./bench_add_preAlloc.png", initSize_preAlloc, limitSize); // pre-allocate
	// wRehash の方が，preAlloc よりも適切なテーブルサイズが選択されているため，最高性能がよい．
	// limitSize 付近の性能はほぼ同一である．
	
	// add: elapsed time [sec]
	bench_plot_add_et("./bench_add_et_wRehash.png",  initSize_wRehash,  limitSize); // with rehash
	bench_plot_add_et("./bench_add_et_preAlloc.png", initSize_preAlloc, limitSize); // pre-allocate
	
	// find: find speed [quely/sec]
	bench_plot_find("./bench_find_wRehash.png",  initSize_wRehash,  limitSize); // with rehash
	bench_plot_find("./bench_find_preAlloc.png", initSize_preAlloc, limitSize); // pre-allocate
	
	// find: all lookup is failed
	bench_plot_find_failedAll("./bench_find_wRehash_failedAll.png",  initSize_wRehash,  limitSize);
	bench_plot_find_failedAll("./bench_find_preAlloc_failedAll.png", initSize_preAlloc, limitSize);
	
	// erase
	bench_plot_erase("./bench_erase_preAlloc.png", initSize_preAlloc, limitSize); // pre-allocate
	
	// find -> erase -> add (reHash occurd)
	bench_plot_find_erase_add("./bench_find_erase_add_pow10_4.png", 10000  ); // find with erasion
	bench_plot_find_erase_add("./bench_find_erase_add_pow10_5.png", 100000 ); // find with erasion
	bench_plot_find_erase_add("./bench_find_erase_add_pow10_6.png", 1000000); // find with erasion
	//*/
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

/*
ハッシュテーブルの特性について，よい点と悪い点を表にまとめて一覧にするとよい．
//最終的には X, O, OO の基準を明確にすること．

                         | add - momentary time | add - momentary time | add - elapsed time | add - elapsed time | find            | find            | erase
                         | (with-rehash)        | (pre-allocate)       | (with-rehash)      | (pre-allocate)     | (with-rehash)   | (pre-allocate)  |
                         | detail: fig.X-X      | detail: fig.X-X      | detail: fig.X-X    | detail: fig.X-X    | detail: fig.X-X | detail: fig.X-X | detail: fig.X-X
-------------------------+----------------------+----------------------+--------------------+--------------------+-----------------+-----------------+-----------------
            unorderd_map |          X           |          X           |        X           |         X          |       X         |       X         |
                  CHashT |          O           |          O           |        X           |         O          |       O         |       O         |
                   dense |          OO          |          O           |        O           |         O          |       OO        |       O         |
InPlaceCHashT (proposed) |          -           |          -           |        -           |         -          |       -         |       -         |

*/

// std::random_device seed_gen;
// std::mt19937 engine(seed_gen());

// TASKs
// - bench of find (exist element)
// - bench of find (empty element)
// - bench of find (random) (shuffle vec)
// - bench of erase
// - compare with "google dense hash"


// https://tessil.github.io/2016/08/29/benchmark-hopscotch-map.html

// unzip -n spa---
// ./ocnfigure
// make


/*
--------------------------------
 Execution time:  1432. 970 sec
--------------------------------
-> 23.882833333 mins
*/

