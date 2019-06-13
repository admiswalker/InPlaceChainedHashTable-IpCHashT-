#include "./CHashT.hpp"
#include "./IpCHashT.hpp"
#include "./flat_hash_map/flat_hash_map.hpp"
#include <time.h>
#include <random>
#include <unordered_map>
#include <sparsehash/dense_hash_map> // sparsehash-master
#include <sys/time.h>     // getrusage
#include <sys/resource.h> // getrusage
#include <thread> // std::thread

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
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_ms
	{     std::unordered_map<uint64,uint64> hashT(initSize);                                bench_add(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize);                                bench_add(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize);                                bench_add(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull);     bench_add(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize); bench_add(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Additional speed [query/ms]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_ms
	
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
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // sec
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); // this meen that 'NULL' and '1' will not be able to insert as a key-value.
		                                                     bench_add_et(hashT, limitSize, vecX_d, vecY_d); }
	{     std::unordered_map<uint64,uint64> hashT(initSize);                            bench_add_et(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize);                            bench_add_et(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize);                            bench_add_et(hashT, limitSize, vecX_i, vecY_i); }
//	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); bench_add_et(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize); bench_add_et(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot_et";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Elapsed time [sec]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // sec
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// used memory size

/*
VmPeak:	   15676 kB
VmSize:	   15676 kB
VmLck:	       0 kB
VmPin:	       0 kB
VmHWM:	    1688 kB
VmRSS:	    1688 kB
VmData:	     296 kB
VmStk:	     132 kB
VmExe:	     248 kB
VmLib:	    4664 kB
VmPTE:	      48 kB
VmPMD:	      12 kB
VmSwap:	       0 kB
HugetlbPages:	       0 kB

VmPeak:	  254924 kB
VmSize:	  240956 kB
VmLck:	       0 kB
VmPin:	       0 kB
VmHWM:	  242460 kB
VmRSS:	  228608 kB
VmData:	  225576 kB
VmStk:	     132 kB
VmExe:	     248 kB
VmLib:	    4664 kB
VmPTE:	     496 kB
VmPMD:	      12 kB
VmSwap:	       0 kB
HugetlbPages:	       0 kB
//*/
uint64 sstd_statusBase(const char* type){
	FILE *fp;
	char cmd[128];
	sprintf(cmd, "grep %s /proc/%d/status", type, getpid());
	if( (fp=popen(cmd,"r"))==NULL ){ return 0ull; } // failure
	
	uint64 ret=0;
	char fmt[128];
	sprintf(fmt, "%s: %%ull kB", type);
	fscanf(fp, fmt, &ret);
	
	if(pclose(fp)==-1){ return 0ull; } // failure
	return ret;
}
uint64 sstd_status_VmPeak      (){ return sstd_statusBase("VmPeak"      ); }
uint64 sstd_status_VmSize      (){ return sstd_statusBase("VmSize"      ); }
uint64 sstd_status_VmLck       (){ return sstd_statusBase("VmLck"       ); }
uint64 sstd_status_VmPin       (){ return sstd_statusBase("VmPin"       ); }
uint64 sstd_status_VmHWM       (){ return sstd_statusBase("VmHWM"       ); }
uint64 sstd_status_VmRSS       (){ return sstd_statusBase("VmRSS"       ); }
uint64 sstd_status_VmData      (){ return sstd_statusBase("VmData"      ); }
uint64 sstd_status_VmStk       (){ return sstd_statusBase("VmStk"       ); }
uint64 sstd_status_VmExe       (){ return sstd_statusBase("VmExe"       ); }
uint64 sstd_status_VmLib       (){ return sstd_statusBase("VmLib"       ); }
uint64 sstd_status_VmPTE       (){ return sstd_statusBase("VmPTE"       ); }
uint64 sstd_status_VmPMD       (){ return sstd_statusBase("VmPMD"       ); }
uint64 sstd_status_VmSwap      (){ return sstd_statusBase("VmSwap"      ); }
uint64 sstd_status_HugetlbPages(){ return sstd_statusBase("HugetlbPages"); }
// 他の status も取得できるようにして，sstd へ標準導入する．(ただし，#ifndef __WIN32 内に実装．)

uint64 sstd_get_ru_maxrss(){
	struct rusage r;
	if( getrusage(RUSAGE_SELF, &r)!=0 ){ sstd::pdbg("ERROR: getrusage() is failed."); return 0ull; }
	return r.ru_maxrss;
}

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
		// add
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			hashT[r] = r;
		}
		
		double h_MB = sstd_status_VmHWM() / 1024.0;
		double r_MB = sstd_status_VmRSS() / 1024.0;
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
	
	baseSize_MB = sstd_status_VmRSS()/1024.0;
	std::unordered_map<uint64,uint64> hashT_u(initSize);
	bench_usedMemory(hashT_u, limitSize, vecX_u, vecY_u, baseSize_MB);
	std::vector<char> buf_u((sstd_status_VmHWM()-sstd_status_VmRSS())*(1024+100)); // inorder not to count swap memory region
	
	baseSize_MB = sstd_status_VmRSS()/1024.0;
	sstd::CHashT<uint64,uint64> hashT_c(initSize);
	bench_usedMemory(hashT_c, limitSize, vecX_c, vecY_c, baseSize_MB);
	std::vector<char> buf_c((sstd_status_VmHWM()-sstd_status_VmRSS())*(1024+100)); // inorder not to count swap memory region
	
	baseSize_MB = sstd_status_VmRSS()/1024.0;
	sstd::IpCHashT<uint64,uint64> hashT_i(initSize);
	bench_usedMemory(hashT_i, limitSize, vecX_i, vecY_i, baseSize_MB);
	std::vector<char> buf_i((sstd_status_VmHWM()-sstd_status_VmRSS())*(1024+100)); // inorder not to count swap memory region
	
	baseSize_MB = sstd_status_VmRSS()/1024.0;
	google::dense_hash_map<uint64,uint64> hashT_d(initSize); hashT_d.set_empty_key(0ull); // this meen that 'NULL' will not be able to insert as a key-value.
	bench_usedMemory(hashT_d, limitSize, vecX_d, vecY_d, baseSize_MB);
	std::vector<char> buf_d((sstd_status_VmHWM()-sstd_status_VmRSS())*(1024+100)); // inorder not to count swap memory region
	
	baseSize_MB = sstd_status_VmRSS()/1024.0;
	ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT_f(initSize);
	bench_usedMemory(hashT_f, limitSize, vecX_f, vecY_f, baseSize_MB);
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot_memory";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Allocated memory size [MB]";
	
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // memory size [MB]
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // sec
	
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
	
	std::mt19937_64 engine(seed_gen()); // for std::shuffle()
	
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
			if(hashT[keyVal] != keyVal){ sstd::pdbg("ERROR: key val is not same."); exit(-1); }
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
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_ms
	{     std::unordered_map<uint64,uint64> hashT(initSize);                            bench_find(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize);                            bench_find(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize);                            bench_find(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); bench_find(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(0);    bench_find(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Find speed [query/ms]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_ms
	
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
	
	std::mt19937_64 engine(seed_gen()); // for std::shuffle()
	
	uint64 numFound=0ull, numNotFound=0ull;
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
	if(numFound!=0ull){
		printf("ERROR: ");
		printf("%lu / %lu = %lf\n", numFound, numFound+numNotFound, (double)numFound/(double)(numFound+numNotFound));
	}
}
void bench_plot_find_failedAll(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_ms
	{     std::unordered_map<uint64,uint64> hashT(initSize);                            bench_find_failedAll(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize);                            bench_find_failedAll(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize);                            bench_find_failedAll(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); bench_find_failedAll(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize);
		                                                     bench_find_failedAll(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Find speed [query/ms]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_ms
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// find_et

template<typename T_hashTable>
void bench_find_et(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_s){
	std::random_device seed_gen;
	uint64 seed = seed_gen();
	std::mt19937_64 rand_init   (seed); // pseudo random number generator
	std::mt19937_64 rand_measure(seed); // pseudo random number generator
	
	// add
	for(uint i=0; i<limitSize; i++){
		uint64 r = rand_init();
		hashT[r] = r;
	}
	
	uint64 splitNum  = 100;
	uint64 interval  = limitSize/splitNum;
	
	double totalTime_sec = 0.0;
	
	uint64 numFound=0ull, numNotFound=0ull;
	vecY_s   <<= 0.0;
	vecX_num <<= 0;
	for(uint sn=0; sn<splitNum; sn++){
		time_m timem; sstd::measureTime_start(timem);
		
		// find
		for(uint i=0; i<interval; i++){
			uint64 r = rand_measure();
			auto itr = hashT.find(r);
			if(itr!=hashT.end()){ numFound++; }else{ numNotFound++; }
		}
		
		double lapTime_sec = sstd::measureTime_stop_ms(timem) / 1000.0;
		totalTime_sec += lapTime_sec;
		vecY_s   <<= totalTime_sec;
		vecX_num <<= numFound;
	}
	if(numNotFound!=0ull){ sstd::pdbg("ERROR: bench_find_et() was failed.\n"); }
}
void bench_plot_find_et(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // sec
	{     std::unordered_map<uint64,uint64> hashT(initSize);                            bench_find_et(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize);                            bench_find_et(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize);                            bench_find_et(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); bench_find_et(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize); bench_find_et(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot_et_find";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Elapsed time [sec]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // sec
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// find_failedAll_et

template<typename T_hashTable>
void bench_find_failedAll_et(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_s){
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	
	// add
	for(uint i=0; i<limitSize; i++){
		uint64 r = rand();
		hashT[r] = r;
	}
	
	uint64 splitNum  = 100;
	uint64 interval  = limitSize/splitNum;
	
	double totalTime_sec = 0.0;
	
	uint64 numFound=0ull, numNotFound=0ull;
	vecY_s   <<= 0.0;
	vecX_num <<= 0;
	for(uint sn=0; sn<splitNum; sn++){
		time_m timem; sstd::measureTime_start(timem);
		
		// find
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			auto itr = hashT.find(r);
			if(itr!=hashT.end()){ numFound++; }else{ numNotFound++; }
		}
		
		double lapTime_sec = sstd::measureTime_stop_ms(timem) / 1000.0;
		totalTime_sec += lapTime_sec;
		vecY_s   <<= totalTime_sec;
		vecX_num <<= numNotFound;
	}
	if(numFound!=0ull){ sstd::pdbg("ERROR: bench_find_failedAll_et() was failed.\n"); }
}
void bench_plot_find_failedAll_et(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // sec
	{     std::unordered_map<uint64,uint64> hashT(initSize);                            bench_find_failedAll_et(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize);                            bench_find_failedAll_et(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize);                            bench_find_failedAll_et(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); bench_find_failedAll_et(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize); bench_find_failedAll_et(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot_et_find";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]";
	const char* ylabel   = "Elapsed time [sec]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // sec
	
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
	std::mt19937_64 engine(seed_gen()); // for std::shuffle()
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
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_ms
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_find_erase_add(hashT, initSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_find_erase_add(hashT, initSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_find_erase_add(hashT, initSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); // this meen that 'NULL' and '1' will not be able to insert as a key-value.
		                                                     bench_find_erase_add(hashT, initSize, vecX_d, vecY_d); }
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize);
		                                                     bench_find_erase_add(hashT, initSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot_wErase";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Non-changed rate [\%]";
	const char* ylabel   = "A speed of one find-erase-add cycle [query/ms]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_ms
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// find-findFailed-erase-add cycle

template<typename T_hashTable>
void bench_find_findFailedAll_erase_add(T_hashTable& hashT, const uint64 initSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_ms){
	
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
	std::mt19937_64 engine(seed_gen()); // for std::shuffle()
	std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
	
	uint8 order = (uint8)std::log10((double)hashT.size());
	uint64 interval = (uint64)std::pow(10, order); if(interval!=1){interval/=10;}
	
	double change_per_cycle = 0.05; // [%]
	double nonChangedRate   = 1.00; // [%]
	double endRate          = 0.0001; // [%]
	uint64 changeNum_per_cycle = (double)hashT.size() * change_per_cycle; // [count]
	
	// rand for find-failed-all
	std::mt19937_64 randFFA(seed_gen()); // pseudo random number generator
	while(nonChangedRate > endRate){
		
		time_m timem; sstd::measureTime_start(timem);
		// find (with all elements found)
		for(uint i=0; i<interval; i++){
			uint64 keyVal = vecKeyVal[i];
			if(hashT[keyVal] != keyVal){ sstd::pdbg("ERROR: key val is not same."); exit(0); }
		}
		// find-failed-all (with all elements found)
		for(uint i=0; i<interval; i++){
			uint64 keyVal = randFFA();
			auto itr = hashT.find(keyVal);
			if(itr != hashT.end()){ sstd::pdbg("ERROR: key val is same."); exit(0); } // there is a bug in sstd::CHashT
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
void bench_plot_find_findFailedAll_erase_add(const char* savePath, const uint64 initSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_ms
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_find_findFailedAll_erase_add(hashT, initSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_find_findFailedAll_erase_add(hashT, initSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_find_findFailedAll_erase_add(hashT, initSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); // this meen that 'NULL' and '1' will not be able to insert as a key-value.
		                                                     bench_find_findFailedAll_erase_add(hashT, initSize, vecX_d, vecY_d); }
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize);
		                                                     bench_find_findFailedAll_erase_add(hashT, initSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot_wErase";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Non-changed rate [\%]";
	const char* ylabel   = "A speed of one find-erase-add cycle [query/ms]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_ms
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// erase

template<typename T_hashTable>
void bench_erase(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_ms){
	
	std::vector<uint64> vecKeyVal(limitSize); vecKeyVal.clear();
	
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	
	// add elements to the num of limitSize
	while(hashT.size()<limitSize){
		uint64 r = rand();
		hashT[r] = r;
		vecKeyVal <<= r;
	}
	
	std::mt19937_64 engine(seed_gen()); // for std::shuffle()
	std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
	
	uint64 interval = 1;
	for(;;){
		uint8 order = (uint8)std::log10((double)vecKeyVal.size());
		interval = (uint64)std::pow(10, order);
		if(interval!=1){interval/=10;}
		
		// erase
		time_m timem; sstd::measureTime_start(timem);
		for(uint i=0; i<interval && vecKeyVal.size()!=0; i++){
			uint64 r = vecKeyVal[vecKeyVal.size()-1];
			vecKeyVal.pop_back();
			hashT[r] = r;
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		vecY_quely_per_ms <<= ((double)interval * 1000.0) / (nsec);
		vecX_num          <<= vecKeyVal.size();
		if(vecKeyVal.size()==0ull){ break; }
		
		std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
	}
}
void bench_plot_erase(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_ms
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_erase(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_erase(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_erase(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); // this meen that 'NULL' and '1' will not be able to insert as a key-value.
		                                                     bench_erase(hashT, limitSize, vecX_d, vecY_d); }
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize);
		                                                     bench_erase(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.

	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]\n(This axis is inverted.)";
	const char* ylabel   = "Erasion speed [query/ms]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_ms
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// erase_et

template<typename T_hashTable>
void bench_erase_et(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_s){
	
	std::vector<uint64> vecKeyVal(limitSize); vecKeyVal.clear();
	
	// add elements to the num of limitSize
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	while(hashT.size()<limitSize){
		uint64 r = rand();
		hashT[r] = r;
		vecKeyVal <<= r;
	}
	vecY_s   <<= 0.0;
	vecX_num <<= vecKeyVal.size();
	
	// shuffle "vecKeyVal"
	std::mt19937_64 engine(seed_gen()); // for std::shuffle()
	std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
	
	double sec_sum = 0.0;
	uint64 interval = 1;
	for(;;){
		uint8 order = (uint8)std::log10((double)vecKeyVal.size());
		interval = (uint64)std::pow(10, order);
		if(interval!=1){interval/=10;}
		
		// erase
		time_m timem; sstd::measureTime_start(timem);
		for(uint i=0; i<interval && vecKeyVal.size()!=0; i++){
			uint64 r = vecKeyVal[vecKeyVal.size()-1];
			vecKeyVal.pop_back();
			hashT[r] = r;
		}
		sec_sum += sstd::measureTime_stop_ms(timem) / 1000.0;
		vecY_s   <<= sec_sum;
		vecX_num <<= vecKeyVal.size();
		if(vecKeyVal.size()==0ull){ break; }
		
		std::shuffle(vecKeyVal.begin(), vecKeyVal.end(), engine);
	}
}
void bench_plot_erase_et(const char* savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i, vecX_d, vecX_f; // num of elements
	std::vector<double> vecY_u, vecY_c, vecY_i, vecY_d, vecY_f; // quely_per_ms
	{     std::unordered_map<uint64,uint64> hashT(initSize); bench_erase_et(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(initSize); bench_erase_et(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(initSize); bench_erase_et(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); // this meen that 'NULL' and '1' will not be able to insert as a key-value.
		                                                     bench_erase_et(hashT, limitSize, vecX_d, vecY_d); }
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(initSize);
		                                                     bench_erase_et(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot_et_invAxisX";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Number of elements on the table [conut]\n(This axis is inverted.)";
	const char* ylabel   = "Elapsed time [sec]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // num of elements
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // quely_per_ms
	
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
	{     std::unordered_map<uint64,uint64> hashT(0);                                bench_maxLoadFactor(hashT, limitSize, vecX_u, vecY_u); }
	{           sstd::CHashT<uint64,uint64> hashT(0);                                bench_maxLoadFactor(hashT, limitSize, vecX_c, vecY_c); }
	{         sstd::IpCHashT<uint64,uint64> hashT(0);                                bench_maxLoadFactor(hashT, limitSize, vecX_i, vecY_i); }
	{ google::dense_hash_map<uint64,uint64> hashT(0); hashT.set_empty_key(0ull);     bench_maxLoadFactor(hashT, limitSize, vecX_d, vecY_d); } // this meen that 'NULL' will not be able to insert as a key-value.
	{ ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> hashT(0); bench_maxLoadFactor(hashT, limitSize, vecX_f, vecY_f); } // this meen that 'NULL' will not be able to insert as a key-value.
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plot_lf";
	const char* funcName = "vvec2graph";
	
	const char* xlabel   = "Table size [count]\n(Maximum load factor of std::IpHashT is limitted by its maximum length of shift_T. Maximum load factor of google::dense_hash_map is artificially limited by 50 %.)";
	const char* ylabel   = "maximum load factor [%]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i, vecX_d, vecX_f}; // table size
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i, vecY_d, vecY_f}; // maximum load factor
	
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const char*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
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
	// bench of used memory size should run first inorder to avoid memory swap by Linux OS.
	bench_plot_usedMemory("./bench_usedMemory_wRehash.png",  initSize_wRehash,  limitSize);
	bench_plot_usedMemory("./bench_usedMemory_preAlloc.png", initSize_preAlloc, limitSize);
	
	// Warm running, because of the first bench usually returns bad result.
	const char* pWarmRun = "./bench_warmRunning.png";
	bench_plot_add(pWarmRun, initSize_preAlloc, limitSize); // pre-allocate
	
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
	//*/
	// erase
	bench_plot_erase("./bench_erase_preAlloc.png", initSize_preAlloc, limitSize); // pre-allocate
	
	bench_plot_erase_et("./bench_erase_et.png",    initSize_preAlloc, limitSize*1); // pre-allocate
//	bench_plot_erase_et("./bench_erase_et_x2.png", initSize_preAlloc, limitSize*2); // pre-allocate
	/*
	// find -> erase -> add (reHash occurd)
	bench_plot_find_erase_add("./bench_find_erase_add_pow10_4.png", 10000  ); // find with erasion
	bench_plot_find_erase_add("./bench_find_erase_add_pow10_5.png", 100000 ); // find with erasion
	bench_plot_find_erase_add("./bench_find_erase_add_pow10_6.png", 1000000); // find with erasion
	
	// max-load factor
	bench_plot_maxLoadFactor("./bench_maxLoadFactor_pow10_5.png", 100000   );
	bench_plot_maxLoadFactor("./bench_maxLoadFactor_pow10_6.png", 1000000  );
//	bench_plot_maxLoadFactor("./bench_maxLoadFactor_pow10_8.png", 100000000);
	
	bench_plot_find_findFailedAll_erase_add("./bench_find_fainFailedAll_erase_add_pow10_6.png", 1000000); // find with erasion
	//*/
//	sstd::rm(pWarmRun);
	/*
	bench_plot_find_et("./bench_find_et.png", initSize_preAlloc, limitSize);
//	bench_plot_find_et("./bench_find_et.png", initSize_preAlloc, 200000000);
	
	bench_plot_find_failedAll_et("./bench_find_failedAll_et.png", initSize_preAlloc, limitSize);
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

