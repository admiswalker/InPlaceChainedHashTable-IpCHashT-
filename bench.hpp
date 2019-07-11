#include <time.h>
#include <random>

#include <unordered_map>
#include "./CHashT.hpp"
#include "./IpCHashT.hpp"
#include <sparsehash/dense_hash_map> // sparsehash-master
#include "./flat_hash_map/flat_hash_map.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------
// common functions
// origin
/*
uint64 size2interval(const uint64 size){
	uint8 order = (uint8)std::log10((double)size);
	uint64 interval = (uint64)std::pow(10, order);
	if(interval!=1){interval/=10;}
	return interval;
}//*/
//*
uint64 size2interval(const uint64 size){
	uint8 order = (uint8)std::log10((double)size);
	uint64 interval = (uint64)std::pow(10, order);
	if      (order<1){ return interval;
	}else if(order<2){ return interval/2;
	}else if(order<3){ return interval/4;
	}else if(order<5){ return interval/10;
	}else            { return interval/10;
	}
}//*/
//-----------------------------------------------------------------------------------------------------------------------------------------------
// insert

template<typename T_hashTable>
void bench_insert(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_us){
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	
	uint64 interval = 1;
	
	for(;;){
		std::vector<uint64> vecR_toInsert(interval);
		for(uint i=0; i<interval; i++){
			vecR_toInsert[i] = rand();
		}
		
		// insert
		time_m timem; sstd::measureTime_start(timem);
		for(uint i=0; i<interval; i++){
			uint64 r = vecR_toInsert[i];
			hashT[r] = r;
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		
		vecX_num          <<= hashT.size();
		vecY_quely_per_us <<= ((double)interval * 1000.0) / (nsec);
		
		interval = size2interval(hashT.size());
		if(hashT.size()+interval>limitSize){ break; }
	}
}
void bench_plot_insert(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_us
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_insert(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_insert(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_insert(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull);
	                                                         bench_insert(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize);
	                                                         bench_insert(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Insertion speed [query/us]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_us
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// insert (elapsed time)

template<typename T_hashTable>
void bench_insert_et(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_s){
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	uint64 splitNum  = 100;
	uint64 interval  = limitSize/splitNum;
	
	double totalTime_sec = 0.0;
	vecX_num <<= hashT.size();
	vecY_s   <<= totalTime_sec;
	for(uint sn=0; sn<splitNum; sn++){
		std::vector<uint64> vecR_toInsert(interval);
		for(uint i=0; i<interval; i++){
			vecR_toInsert[i] = rand();
		}
		
		// insert
		time_m timem; sstd::measureTime_start(timem);
		for(uint i=0; i<interval; i++){
			uint64 r = vecR_toInsert[i];
			hashT[r] = r;
		}
		double ms = sstd::measureTime_stop_ms(timem);
		totalTime_sec += ms / 1000.0;
		vecX_num <<= hashT.size();
		vecY_s   <<= totalTime_sec;
	}
}
void bench_plot_insert_et(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // sec
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_insert_et(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_insert_et(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_insert_et(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); // this meen that 'NULL' will not be able to insert as a key-value.
		                                                     bench_insert_et(hashT, limitSize, vecX_d, vecY_d); }
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize);
	                                                         bench_insert_et(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Elapsed time [sec]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // sec
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_et_insert";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// used memory size

template<typename T_hashTable>
void bench_usedMemory(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_MB, const double baseSize_MB){
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	
	vecY_MB  <<= baseSize_MB; // base size of allocated memory
	vecX_num <<= 0;
	
	uint64 splitNum  = 100;
//	uint64 splitNum  = 1000;
	uint64 interval  = limitSize/splitNum;
	uint64 tSize_prev = hashT.bucket_count(); // table size
	for(uint sn=0; sn<splitNum; sn++){
		// insert
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			hashT[r] = r;
		}
		
		double h_MB = sstd::status_VmHWM() / 1024.0;
		double r_MB = sstd::status_VmRSS() / 1024.0;
		uint64 tSize = hashT.bucket_count();
		double MB; if(tSize > tSize_prev){ MB=h_MB; tSize_prev=tSize; }else{ MB=r_MB; }
		vecY_MB  <<= MB;
		vecX_num <<= hashT.size();
	}
	vecY_MB -= (double)vecY_MB[0];
}
void bench_plot_usedMemory(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // memory size [MB]
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // sec
	double baseSize_MB = 0.0;
	
	baseSize_MB = sstd::status_VmRSS()/1024.0;
	std::unordered_map<uint64,uint64> hashT_u(initSize);
	bench_usedMemory(hashT_u, limitSize, vecX_u, vecY_u, baseSize_MB);
	std::vector<char> buf_u((sstd::status_VmHWM()-sstd::status_VmRSS())*1024); // inorder not to count swap memory region
	
	baseSize_MB = sstd::status_VmRSS()/1024.0;
	sstd::CHashT<uint64,uint64> hashT_c(initSize);
	bench_usedMemory(hashT_c, limitSize, vecX_c, vecY_c, baseSize_MB);
	std::vector<char> buf_c((sstd::status_VmHWM()-sstd::status_VmRSS())*1024); // inorder not to count swap memory region
	
	baseSize_MB = sstd::status_VmRSS()/1024.0;
	sstd::IpCHashT<uint64,uint64> hashT_i(initSize);
	bench_usedMemory(hashT_i, limitSize, vecX_i, vecY_i, baseSize_MB);
	std::vector<char> buf_i((sstd::status_VmHWM()-sstd::status_VmRSS())*1024); // inorder not to count swap memory region
	
	baseSize_MB = sstd::status_VmRSS()/1024.0;
	google::dense_hash_map<uint64,uint64> hashT_d(initSize); hashT_d.set_empty_key(0ull); // this meen that 'NULL' will not be able to insert as a key-value.
	bench_usedMemory(hashT_d, limitSize, vecX_d, vecY_d, baseSize_MB);
	std::vector<char> buf_d((sstd::status_VmHWM()-sstd::status_VmRSS())*1024); // inorder not to count swap memory region
	
	baseSize_MB = sstd::status_VmRSS()/1024.0;
	ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT_f(initSize);
	bench_usedMemory(hashT_f, limitSize, vecX_f, vecY_f, baseSize_MB);
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Allocated memory size [MB]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // memory size [MB]
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // sec
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_memory";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// find

template<typename T_hashTable>
void bench_find(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_us){
	std::random_device seed_gen;
	std::mt19937_64 rand       (seed_gen()); // pseudo random number generator
	std::mt19937_64 rand_toFind(seed_gen()); // pseudo random number generator
	
	uint64 interval = 1;
	std::vector<uint64> vecR(limitSize); vecR.clear();
	
	for(;;){
		// insert
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			vecR <<= r;
			hashT[r] = r;
		}
		
		vecX_num <<= hashT.size();
		
		std::vector<uint64> vecR_toFind(vecR.size());
		{
			std::uniform_int_distribution<uint64> range(0, vecR.size()-1); // make randome number between [0, vecR.size()-1].
			for(uint i=0; i<interval; i++){
				vecR_toFind[i] = vecR[range(rand_toFind)];
			}
		}
		
		// find (all elements are found)
		time_m timem; sstd::measureTime_start(timem);
		for(uint i=0; i<interval; i++){
			uint64 keyVal = vecR[i];
			if(hashT[keyVal] != keyVal){ sstd::pdbg("ERROR: key val is not same."); exit(-1); }
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		vecY_quely_per_us <<= ((double)interval * 1000.0) / (nsec);
		
		interval = size2interval(hashT.size());
		if(hashT.size()+interval>limitSize){ break; }
	}
}
void bench_plot_find(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_us
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_find(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_find(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_find(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull);
	                                                         bench_find(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(0);
	                                                         bench_find(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Find speed [query/us]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_us
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// find: all lookup is failed

template<typename T_hashTable>
void bench_find_failedAll(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_us){
	std::random_device seed_gen;
	std::mt19937_64 rand       (seed_gen()); // pseudo random number generator
	std::mt19937_64 rand_toFind(seed_gen()); // pseudo random number generator
	
	uint64 interval = 1;
	std::vector<uint64> vecR(limitSize); vecR.clear();
	
	uint64 numFound=0ull, numNotFound=0ull;
	for(;;){
		// insert
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			hashT[r] = r;
			vecR <<= r;
		}
		
		vecX_num <<= hashT.size();
		
		std::vector<uint64> vecR_toFind(interval);
		for(uint i=0; i<interval; i++){
			vecR_toFind[i] = rand_toFind();
		}
		
		// find (all elements are not found)
		time_m timem; sstd::measureTime_start(timem);
		for(uint i=0; i<interval; i++){
			auto itr = hashT.find( vecR_toFind[i] );
			if(itr!=hashT.end()){ numFound++; }else{ numNotFound++; }
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		vecY_quely_per_us <<= ((double)interval * 1000.0) / (nsec);
		
		interval = size2interval(hashT.size());
		if(hashT.size()+interval>limitSize){ break; }
	}
	if(numFound!=0ull){
		printf("ERROR: ");
		printf("%lu / %lu = %lf\n", numFound, numFound+numNotFound, (double)numFound/(double)(numFound+numNotFound));
	}
}
void bench_plot_find_failedAll(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_us
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_find_failedAll(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_find_failedAll(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_find_failedAll(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); bench_find_failedAll(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize);
		                                                     bench_find_failedAll(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Find speed [query/us]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_us
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// find-erase-insert cycle

template<typename T_hashTable>
void bench_find_erase_insert(T_hashTable& hashT, const uint64 initSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_us){
	
	std::random_device seed_gen;
	
	std::vector<uint64> vecKeyVal(initSize); vecKeyVal.clear();
	
	// insert elements to the num of limitSize
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	while(hashT.size()<initSize){
		uint64 r = rand();
		hashT[r] = r;
		vecKeyVal <<= r;
	}
	
	// shuffle "vecKeyVal"
	std::mt19937_64 engine(seed_gen()); // for std::shuffle()
	std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
	
	double change_per_cycle = 0.05; // [%]
	double nonChangedRate   = 1.00; // [%]
	double endRate          = 0.0001; // [%]
	uint64 changeNum_per_cycle = (double)hashT.size() * change_per_cycle; // [count]
	while(nonChangedRate > endRate){
		
		time_m timem; sstd::measureTime_start(timem);
		// find (with all elements found)
		for(uint i=0; i<changeNum_per_cycle; i++){
			uint64 keyVal = vecKeyVal[i];
			if(hashT[keyVal] != keyVal){ sstd::pdbg("ERROR: key val is not same."); exit(0); }
		}
		// erase change_per_cycle [%]
		for(uint i=0; i<changeNum_per_cycle; i++){
			uint64 r = vecKeyVal[vecKeyVal.size()-1];
			vecKeyVal.pop_back();
			hashT.erase( r );
		}
		// insert change_per_cycle [%]
		for(uint i=0; i<changeNum_per_cycle; i++){
			uint64 r = rand();
			vecKeyVal <<= r;
			hashT[r] = r;
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		
		vecY_quely_per_us <<= ((double)changeNum_per_cycle * 1000.0) / (nsec);
		vecX_num          <<= nonChangedRate;
		nonChangedRate *= (1.00 - change_per_cycle);
		
		// shuffle vecKeyVal
		std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
	}
}
void bench_plot_find_erase_insert(const char* savePath, const uint64 initSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_us
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_find_erase_insert(hashT, initSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_find_erase_insert(hashT, initSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_find_erase_insert(hashT, initSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); // this meen that 'NULL' and '1' will not be able to insert as a key-value.
		                                                     bench_find_erase_insert(hashT, initSize, vecX_d, vecY_d); }
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize);
		                                                     bench_find_erase_insert(hashT, initSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	const char* xlabel   = "Non-changed rate [\%]";
	const char* ylabel   = "A speed of one find-erase-insert cycle [query/us]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_us
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_FEA";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// find-findFailed-erase-insert cycle

template<typename T_hashTable>
void bench_find_findFailedAll_erase_insert(T_hashTable& hashT, const uint64 initSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_us){
	
	std::random_device seed_gen;
	
	std::vector<uint64> vecKeyVal(initSize); vecKeyVal.clear();
	
	// insert elements to the num of limitSize
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	while(hashT.size()<initSize){
		uint64 r = rand();
		hashT[r] = r;
		vecKeyVal <<= r;
	}
	
	// shuffle "vecKeyVal"
	std::mt19937_64 engine(seed_gen()); // for std::shuffle()
	std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
	
	double change_per_cycle = 0.05; // [%]
	double nonChangedRate   = 1.00; // [%]
	double endRate          = 0.0001; // [%]
	uint64 changeNum_per_cycle = (double)hashT.size() * change_per_cycle; // [count]
	
	// rand for find-failed-all
	std::mt19937_64 randFFA(seed_gen()); // pseudo random number generator
	while(nonChangedRate > endRate){
		
		time_m timem; sstd::measureTime_start(timem);
		// find (with all elements found)
		for(uint i=0; i<changeNum_per_cycle; i++){
			uint64 keyVal = vecKeyVal[i];
			if(hashT[keyVal] != keyVal){ sstd::pdbg("ERROR: key val is not same."); exit(0); }
		}
		// find-failed-all (with all elements found)
		for(uint i=0; i<changeNum_per_cycle; i++){
			uint64 keyVal = randFFA();
			auto itr = hashT.find(keyVal);
			if(itr != hashT.end()){ sstd::pdbg("ERROR: key val is same."); exit(0); } // there is a bug in sstd::CHashT
		}
		// erase change_per_cycle [%]
		for(uint i=0; i<changeNum_per_cycle; i++){
			hashT.erase( vecKeyVal[vecKeyVal.size()-1] );
			vecKeyVal.pop_back();
		}
		// insert change_per_cycle [%]
		for(uint i=0; i<changeNum_per_cycle; i++){
			uint64 r = rand();
			hashT[r] = r;
			vecKeyVal <<= r;
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		
		vecY_quely_per_us <<= ((double)changeNum_per_cycle * 1000.0) / (nsec);
		vecX_num          <<= nonChangedRate;
		nonChangedRate *= (1.00 - change_per_cycle);
		
		// shuffle vecKeyVal
		std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
	}
}
void bench_plot_find_findFailedAll_erase_insert(const char* savePath, const uint64 initSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_us
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_find_findFailedAll_erase_insert(hashT, initSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_find_findFailedAll_erase_insert(hashT, initSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_find_findFailedAll_erase_insert(hashT, initSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); // this meen that 'NULL' and '1' will not be able to insert as a key-value.
		                                                     bench_find_findFailedAll_erase_insert(hashT, initSize, vecX_d, vecY_d); }
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize);
		                                                     bench_find_findFailedAll_erase_insert(hashT, initSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	const char* xlabel   = "Non-changed rate [\%]";
	const char* ylabel   = "A speed of one find-erase-insert cycle [query/us]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_us
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_FEA";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// erase

template<typename T_hashTable>
void bench_erase(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_us){
	std::random_device seed_gen;
	std::mt19937_64 rand       (seed_gen()); // pseudo random number generator
	std::mt19937_64 rand_toFind(seed_gen()); // pseudo random number generator
	
	uint64 interval = 1;
	std::vector<uint64> vecR(limitSize); vecR.clear();
	
	for(;;){
		// insert
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			vecR <<= r;
			hashT[r] = r;
		}
		
		vecX_num <<= hashT.size();
		
		std::vector<uint64> vecR_toErase(vecR.size());
		std::vector<uint64> vecR_idx    (vecR.size());
		{
			std::uniform_int_distribution<uint64> range(0, vecR.size()-1); // make randome number between [0, vecR.size()-1].
			for(uint i=0; i<interval; i++){
				vecR_idx[i]     = range(rand_toFind);
				vecR_toErase[i] = vecR[vecR_idx[i]];
			}
		}
		
		// erase (all elements are found)
		time_m timem; sstd::measureTime_start(timem);
		for(uint i=0; i<interval; i++){
			hashT.erase( vecR_toErase[i] );
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		// insert
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			vecR[vecR_idx[i]] = r;
			hashT[r] = r;
		}
		vecY_quely_per_us <<= ((double)interval * 1000.0) / (nsec);
		
		interval = size2interval(hashT.size());
		if(hashT.size()+interval>limitSize){ break; }
	}
}
void bench_plot_erase(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_us
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_erase(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_erase(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_erase(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); // this meen that 'NULL' and '1' will not be able to insert as a key-value.
		                                                     bench_erase(hashT, limitSize, vecX_d, vecY_d); }
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize);
		                                                     bench_erase(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	const char* xlabel   = "Number of elements on the table [conut]\n(This axis is inverted.)";
	const char* ylabel   = "Erasion speed [query/us]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_us
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

template<typename T_hashTable>
void bench_maxLoadFactor(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_tSize, std::vector<double>& vecY_lf){
	
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator

	double lf_prev=0;
	while(hashT.size()<limitSize){
		uint64 r = rand();
		hashT[r] = r;
		double lf = hashT.load_factor();
		if(lf < lf_prev){
			vecX_tSize <<= hashT.bucket_count()-1;
			vecY_lf    <<= lf_prev;
		}
		lf_prev = lf;
	}
}
void bench_plot_maxLoadFactor(const char* savePath, const uint64 limitSize){
	
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // table size
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // maximum load factor
	{     std::unordered_map<uint64,uint64> hashT(0); bench_maxLoadFactor(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(0); bench_maxLoadFactor(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(0); bench_maxLoadFactor(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(0); hashT.set_empty_key(0ull);
		                                              bench_maxLoadFactor(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(0);
		                                              bench_maxLoadFactor(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	const char* xlabel   = "Table size [count]\n(Maximum load factor of std::IpHashT is limitted by its maximum length of shift_T. Maximum load factor of google::dense_hash_map is artificially limited by 50 %.)";
	const char* ylabel   = "maximum load factor [%]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // table size
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // maximum load factor
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_lf";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

void RUN_ALL_BENCHS(){
//	const uint64 limitSize = 200000000; // limit of memory
	const uint64 limitSize = 5000000;   // 50 sec
	const uint64 initSize_wRehash  = 0ull;
	const uint64 initSize_preAlloc = limitSize;
	//*
	// bench of used memory size should run first inorder to avoid memory swap by Linux OS.
	bench_plot_usedMemory("./bench_usedMemory_wRehash_log.png",  initSize_wRehash,  limitSize);
	bench_plot_usedMemory("./bench_usedMemory_preAlloc_log.png", initSize_preAlloc, limitSize);
	
	// Warm running, because of the first bench usually returns bad result.
	const char* pWarmRun = "./bench_warmRunning.png";
	bench_plot_insert(pWarmRun, initSize_wRehash, limitSize); // pre-allocate
	
	// insert: insertion speed [query/sec]
	bench_plot_insert("./bench_insert_wRehash.png",  initSize_wRehash,  limitSize); // with rehash
	// wRehash の方が，preAlloc よりも適切なテーブルサイズが選択されているため，最高性能がよい．
	// limitSize 付近の性能はほぼ同一である．
	
	// insert: elapsed time [sec]
	bench_plot_insert_et("./bench_insert_et_wRehash.png",  initSize_wRehash,  limitSize); // with rehash
	bench_plot_insert_et("./bench_insert_et_preAlloc.png", initSize_preAlloc, limitSize); // pre-allocate
	
	// find: find speed [quely/sec]
	bench_plot_find("./bench_find_wRehash.png",  initSize_wRehash,  limitSize); // with rehash
	
	// find: all lookup is failed
	bench_plot_find_failedAll("./bench_find_wRehash_failedAll.png",  initSize_wRehash,  limitSize);
	
	// erase
	bench_plot_erase("./bench_erase_wRehash.png", initSize_wRehash, limitSize); // pre-allocate
	
	// find -> erase -> insert (reHash occurd)
	bench_plot_find_erase_insert("./bench_find_erase_insert_pow10_4.png", 10000  ); // find with erasion
	bench_plot_find_erase_insert("./bench_find_erase_insert_pow10_5.png", 100000 ); // find with erasion
	bench_plot_find_erase_insert("./bench_find_erase_insert_pow10_6.png", 1000000); // find with erasion
	
	bench_plot_find_findFailedAll_erase_insert("./bench_find_fainFailedAll_erase_insert_pow10_6.png", 1000000); // find with erasion
	
	// max-load factor
	bench_plot_maxLoadFactor("./bench_maxLoadFactor.png", limitSize);
	//*/
//	sstd::rm(pWarmRun);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

/*
ハッシュテーブルの特性について，よい点と悪い点を表にまとめて一覧にするとよい．
//最終的には X, O, OO の基準を明確にすること．

                         | insert - momentary time | insert - momentary time | insert - elapsed time | insert - elapsed time | find            | find            | erase
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

