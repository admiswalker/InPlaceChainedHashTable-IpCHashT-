#include <time.h>
#include <random>

#include <unordered_map>
#include "./CHashT.hpp"
#include "./IpCHashT.hpp"
#include <sparsehash/dense_hash_map> // sparsehash-master
#include "./flat_hash_map-master/flat_hash_map.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------
// definitions
//*
typedef std::unordered_map<uint64,uint64>                                    uHashT;
typedef sstd::CHashT<uint64,uint64>                                          cHashT;
typedef sstd::IpCHashT_u8hS<uint64,uint64>                                   iHashT_u8h; // uint8, half (maxLoadfactor50), Successful search major option
typedef sstd::IpCHashT_u8fS<uint64,uint64>                                   iHashT_u8f; // uint8, full (maxLoadfactor100), Successful search major option
typedef sstd::IpCHashT_u16fS<uint64,uint64>                                  iHashT_u16; // uint16, full (maxLoadfactor100), Successful search major option
typedef google::dense_hash_map<uint64,uint64>                                dHashT;
typedef ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> fHashT;
//*/
/*
typedef std::unordered_map<uint64,uint64>                                    uHashT;
typedef sstd::CHashT<uint64,uint64>                                          cHashT;
typedef sstd::IpCHashT_u8hU<uint64,uint64>                                   iHashT_u8h; // uint8, half (maxLoadfactor50), Unsuccessful search major option
typedef sstd::IpCHashT_u8fU<uint64,uint64>                                   iHashT_u8f; // uint8, full (maxLoadfactor100), Unsuccessful search major option
typedef sstd::IpCHashT_u16fU<uint64,uint64>                                  iHashT_u16; // uint16, full (maxLoadfactor100), Unsuccessful search major option
typedef google::dense_hash_map<uint64,uint64>                                dHashT;
typedef ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>> fHashT;
//*/
#define RUN_BENCH(vvecX, vvecY, initSize, limitSize, Fnc)				\
	std::vector<double> vecX_u, vecX_c, vecX_i8h, vecX_i8f, vecX_i16, vecX_d, vecX_f; \
	std::vector<double> vecY_u, vecY_c, vecY_i8h, vecY_i8f, vecY_i16, vecY_d, vecY_f; \
	{ uHashT     hashT(initSize); Fnc(hashT, limitSize, vecX_u,   vecY_u  ); } \
	{ cHashT     hashT(initSize); Fnc(hashT, limitSize, vecX_c,   vecY_c  ); } \
	{ iHashT_u8h hashT(initSize); Fnc(hashT, limitSize, vecX_i8h, vecY_i8h); } \
	{ iHashT_u8f hashT(initSize); Fnc(hashT, limitSize, vecX_i8f, vecY_i8f); } \
	{ iHashT_u16 hashT(initSize); Fnc(hashT, limitSize, vecX_i16, vecY_i16); } \
	{ dHashT     hashT(initSize); hashT.set_empty_key(0ull); /* this meen that 'NULL' will not be able to insert as a key-value. */ \
	                              Fnc(hashT, limitSize, vecX_d,   vecY_d  ); } \
	{ fHashT     hashT(initSize); Fnc(hashT, limitSize, vecX_f,   vecY_f  ); } \
	vvecX={vecX_u, vecX_c, vecX_i8h, vecX_i8f, vecX_i16, vecX_d, vecX_f}; \
	vvecY={vecY_u, vecY_c, vecY_i8h, vecY_i8f, vecY_i16, vecY_d, vecY_f};

#define RUN_BENCH_withErase(vvecX, vvecY, initSize, limitSize, Fnc)		\
	std::vector<double> vecX_u, vecX_c, vecX_i8h, vecX_i8f, vecX_i16, vecX_d, vecX_f; \
	std::vector<double> vecY_u, vecY_c, vecY_i8h, vecY_i8f, vecY_i16, vecY_d, vecY_f; \
	{ uHashT       hashT(initSize); Fnc(hashT, limitSize, vecX_u,   vecY_u  ); } \
	{ cHashT       hashT(initSize); Fnc(hashT, limitSize, vecX_c,   vecY_c  ); } \
	{ iHashT_u8h   hashT(initSize); Fnc(hashT, limitSize, vecX_i8h, vecY_i8h); } \
	{ iHashT_u8f   hashT(initSize); Fnc(hashT, limitSize, vecX_i8f, vecY_i8f); } \
	{ iHashT_u16   hashT(initSize); Fnc(hashT, limitSize, vecX_i16, vecY_i16); } \
	{ dHashT       hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); /* this meen that 'NULL' and '1' will not be able to insert as a key-value. */ \
	                                Fnc(hashT, limitSize, vecX_d,   vecY_d  ); } \
	{ fHashT       hashT(initSize); Fnc(hashT, limitSize, vecX_f,   vecY_f  ); } \
	vvecX={vecX_u, vecX_c, vecX_i8h, vecX_i8f, vecX_i16, vecX_d, vecX_f}; \
	vvecY={vecY_u, vecY_c, vecY_i8h, vecY_i8f, vecY_i16, vecY_d, vecY_f};

#define BENCH_to_CSV(savePath, vvecX, vvecY, vvecHeader)				\
	sstd::vvec<     double> vvec     = {vvecX[0]}; for(uint i=0;i<vvecY.size();i++){ vvec <<= {vvecY[i]}; } \
	sstd::vvec<std::string> vvec_str = sstd::double2str(sstd::Tr(vvec)); \
	sstd::vvec<std::string> vvec_csv = vvecHeader << vvec_str;			\
	sstd::vvec2csv(savePath, vvec_csv);
	
	// vvecX[0],           vvecY[0],           vvecY[1],               vvecY[2],              vvecY[3],               vvecY[4],           vvecY[5],           vvecY[6]
	//  [count], uHashT [query/μs], cHashT [query/μs], iHashT_u8h [query/μs], iHashT_u8f [query/μs], iHashT_u16 [query/μs], dHashT [query/μs], fHashT [query/μs]
	//       0 , 
	//       1 , 
	//      ︙ , 
	//      ︙ , 

std::string hashT2typeStr(const uHashT    & hashT){ return std::string("uHashT"    ); }
std::string hashT2typeStr(const cHashT    & hashT){ return std::string("cHashT"    ); }
std::string hashT2typeStr(const iHashT_u8h& hashT){ return std::string("iHashT_u8h"); }
std::string hashT2typeStr(const iHashT_u8f& hashT){ return std::string("iHashT_u8f"); }
std::string hashT2typeStr(const iHashT_u16& hashT){ return std::string("iHashT_u16"); }
std::string hashT2typeStr(const dHashT    & hashT){ return std::string("dHashT"    ); }
std::string hashT2typeStr(const fHashT    & hashT){ return std::string("fHashT"    ); }

//-----------------------------------------------------------------------------------------------------------------------------------------------
// common functions

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
// used memory size

template<typename T_hashTable>
void bench_usedMemory(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_GB, const double baseSize_GB){
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	
	vecY_GB  <<= baseSize_GB; // base size of allocated memory
	vecX_num <<= 0;
	
//	uint64 splitNum  = 100;
	uint64 splitNum  = 200;
//	uint64 splitNum  = 1000;
	uint64 interval  = limitSize/splitNum;
	uint64 tSize_prev = hashT.bucket_count(); // table size
	for(uint sn=0; sn<splitNum; sn++){
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			hashT.insert(std::pair<uint64,uint64>(r,r));
		}
		
		double h_GB = sstd::status_VmHWM() / (1024.0*1024.0);
		double r_GB = sstd::status_VmRSS() / (1024.0*1024.0);
		uint64 tSize = hashT.bucket_count();
		double GB; if(tSize > tSize_prev){ GB=h_GB; tSize_prev=tSize; }else{ GB=r_GB; }
		vecY_GB  <<= GB;
		vecX_num <<= hashT.size();
	}
	vecY_GB -= (double)vecY_GB[0];
}
//---
void vvec2plot_usedMemory(const std::string& savePath, const std::vector<std::string>& saveAs, const sstd::vvec<double>& vvecX, const sstd::vvec<double>& vvecY){
	const char* xlabel = "Number of elements on the table [conut]";
	const char* ylabel = "Allocated memory size [GB]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF50)", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF100)", "sstd::IpCHashT<uint64,uint64> (as uint16 and maxLF100)", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_memory";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}
//---
void bench2csv_usedMomory(const std::string& savePath, const sstd::vvec<std::string>& vvecHeader, const sstd::vec<double>& vecX, const sstd::vec<double>& vecY){
	sstd::vvec<     double> vvec     = {vecX, vecY};
	sstd::vvec<std::string> vvec_str = sstd::double2str(sstd::Tr(vvec));
	sstd::vvec<std::string> vvec_csv = vvecHeader << vvec_str;
	sstd::vvec2csv(savePath, vvec_csv);
}
//---
void call_from__main_bench_usedMemory(int& argc, char**& argv){
	
	uint argIdx=0;
	const std::string fileName = argv[argIdx++];
	const std::string option   = argv[argIdx++];
	const std::string savePath = argv[argIdx++];
	
	const uint64 initSize  = sstd::str2int(argv[argIdx++]);
	const uint64 limitSize = sstd::str2int(argv[argIdx++]);
	
	const std::string xLabel = argv[argIdx++];
	const std::string yLabel = argv[argIdx++];
	
	std::vector<double> vecX; // table size
	std::vector<double> vecY; // memory size [GB]
	double baseSize_GB = sstd::status_VmRSS()/(1024.0*1024.0);
	if      (sstd::strcmp("uHashT",     option)){     uHashT hashT(initSize); bench_usedMemory(hashT, limitSize, vecX, vecY, baseSize_GB);
	}else if(sstd::strcmp("cHashT",     option)){     cHashT hashT(initSize); bench_usedMemory(hashT, limitSize, vecX, vecY, baseSize_GB);
	}else if(sstd::strcmp("iHashT_u8h", option)){ iHashT_u8h hashT(initSize); bench_usedMemory(hashT, limitSize, vecX, vecY, baseSize_GB);
	}else if(sstd::strcmp("iHashT_u8f", option)){ iHashT_u8f hashT(initSize); bench_usedMemory(hashT, limitSize, vecX, vecY, baseSize_GB);
	}else if(sstd::strcmp("iHashT_u16", option)){ iHashT_u16 hashT(initSize); bench_usedMemory(hashT, limitSize, vecX, vecY, baseSize_GB);
	}else if(sstd::strcmp("dHashT",     option)){     dHashT hashT(initSize); hashT.set_empty_key(0ull); /* this meen that 'NULL' will not be able to insert as a key-value. */
	                                                                          bench_usedMemory(hashT, limitSize, vecX, vecY, baseSize_GB);
	}else if(sstd::strcmp("fHashT",     option)){     fHashT hashT(initSize); bench_usedMemory(hashT, limitSize, vecX, vecY, baseSize_GB);
	}
	
	sstd::vvec<std::string> vvecHeader = {{xLabel, yLabel}};
	bench2csv_usedMomory(savePath, vvecHeader, vecX, vecY);
}
//---
void call__main_bench_usedMemory(const std::string& saveDir, const char* num, const char* option, const char* xLabel, const char* yLabel, const uint64 initSize, const uint64 limitSize){
	const std::string savePath = saveDir+'/'+num+'_'+option+".csv";
	int ret = sstd::system(sstd::ssprintf("./exe_bench_uM %s %s %d %d %s %s", option, savePath.c_str(), initSize, limitSize, xLabel, yLabel));
	if(WIFEXITED(ret)<=0){ printf("ERROR:\n"); }
}
void bench2csv_usedMemory_runBench(const std::string& saveDir, const uint64 initSize, const uint64 limitSize){
	call__main_bench_usedMemory(saveDir, "0",     "uHashT", "[count]",     "uHashT [GB]", initSize, limitSize);
	call__main_bench_usedMemory(saveDir, "1",     "cHashT", "[count]",     "cHashT [GB]", initSize, limitSize);
	call__main_bench_usedMemory(saveDir, "2", "iHashT_u8h", "[count]", "iHashT_u8h [GB]", initSize, limitSize);
	call__main_bench_usedMemory(saveDir, "3", "iHashT_u8f", "[count]", "iHashT_u8f [GB]", initSize, limitSize);
	call__main_bench_usedMemory(saveDir, "4", "iHashT_u16", "[count]", "iHashT_u16 [GB]", initSize, limitSize);
	call__main_bench_usedMemory(saveDir, "5",     "dHashT", "[count]",     "dHashT [GB]", initSize, limitSize);
	call__main_bench_usedMemory(saveDir, "6",     "fHashT", "[count]",     "fHashT [GB]", initSize, limitSize);
}
void bench2csv_usedMemory_meargeResult(const std::string& savePath, const std::string& tmpDir){
	std::string csvPath  = tmpDir+ "/*";
	
	std::vector<std::string> vecPath = sstd::glob(csvPath);
	sstd::vvec<double> vvecX(vecPath.size()), vvecY(vecPath.size());
	for(uint i=0; i<vecPath.size(); i++){
		sstd::vvec<std::string> vvecOrig = sstd::csv2vvec(vecPath[i]);
		sstd::vvec<std::string> header   = vvecOrig && sstd::slice_mv(sstd::begin(), 1);
		sstd::vvec<std::string> vvecStr  = vvecOrig && sstd::slice_mv(1, sstd::end());
		sstd::vvec<     double> vvecD    = sstd::Tr(sstd::str2double(vvecStr));
		vvecX[i] <<= vvecD[0];
		vvecY[i] <<= vvecD[1];
	}
	sstd::vvec<std::string> vvecHeader = {{"[count]", "uHashT [GB]", "cHashT [GB]", "iHashT_u8h [GB]", "iHashT_u8f [GB]", "iHashT_u16 [GB]", "dHashT [GB]", "fHashT [GB]"}};
	BENCH_to_CSV(savePath, vvecX, vvecY, vvecHeader);
}
void bench2csv_usedMemory(const std::string& savePath, const std::string& tmpDir, const uint64 initSize, const uint64 limitSize){
	bench2csv_usedMemory_runBench(tmpDir, initSize, limitSize);
	bench2csv_usedMemory_meargeResult(savePath, tmpDir);
}

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
		
		time_m timem; sstd::measureTime_start(timem);
		for(uint i=0; i<interval; i++){
			uint64 r = vecR_toInsert[i];
			hashT.insert(std::pair<uint64,uint64>(r,r)); // insert with "hashT[r] = r;" will slow down dense_hash_map and IpCHashT-successfulMajorOption.
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		
		vecX_num          <<= hashT.size();
		vecY_quely_per_us <<= ((double)interval * 1000.0) / (nsec);
		
		interval = size2interval(hashT.size());
		if(hashT.size()+interval>limitSize){ break; }
	}
}
//---
void vvec2plot_insert(const std::string& savePath, const std::vector<std::string>& saveAs, const sstd::vvec<double>& vvecX, const sstd::vvec<double>& vvecY){
	const char* xlabel = "Number of elements on the table [conut]";
	const char* ylabel = "Insertion speed [query/μs]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF50)", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF100)", "sstd::IpCHashT<uint64,uint64> (as uint16 and maxLF100)", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}
void bench2plot_insert(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_insert);
	
	vvec2plot_insert(savePath, saveAs, vvecX, vvecY);
}
//---
void bench2csv_insert(const std::string& savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_insert);
	
	sstd::vvec<std::string> vvecHeader = {{"[count]", "uHashT [query/μs]", "cHashT [query/μs]", "iHashT_u8h [query/μs]", "iHashT_u8f [query/μs]", "iHashT_u16 [query/μs]", "dHashT [query/μs]", "fHashT [query/μs]"}};
	BENCH_to_CSV(savePath, vvecX, vvecY, vvecHeader);
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
		
		time_m timem; sstd::measureTime_start(timem);
		for(uint i=0; i<interval; i++){
			uint64 r = vecR_toInsert[i];
			hashT.insert(std::pair<uint64,uint64>(r,r)); // insert with "hashT[r] = r;" will slow down dense_hash_map and IpCHashT-successfulMajorOption.
		}
		double ms = sstd::measureTime_stop_ms(timem);
		totalTime_sec += ms / 1000.0;
		vecX_num <<= hashT.size();
		vecY_s   <<= totalTime_sec;
	}
}
//---
void vvec2plot_insert_et(const std::string& savePath, const std::vector<std::string>& saveAs, const sstd::vvec<double>& vvecX, const sstd::vvec<double>& vvecY){
	const char* xlabel = "Number of elements on the table [conut]";
	const char* ylabel = "Elapsed time [sec]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF50)", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF100)", "sstd::IpCHashT<uint64,uint64> (as uint16 and maxLF100)", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_et_insert";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}
void bench2plot_insert_et(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_insert_et);
	
	vvec2plot_insert_et(savePath, saveAs, vvecX, vvecY);
}
//---
void bench2csv_insert_et(const std::string& savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_insert_et);
	
	sstd::vvec<std::string> vvecHeader = {{"[count]", "uHashT [sec]", "cHashT [sec]", "iHashT_u8h [sec]", "iHashT_u8f [sec]", "iHashT_u16 [sec]", "dHashT [sec]", "fHashT [sec]"}};
	BENCH_to_CSV(savePath, vvecX, vvecY, vvecHeader);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// find: successful search

template<typename T_hashTable>
void bench_find(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_us){
	std::random_device seed_gen;
	uint64 seed1 = seed_gen();
	uint64 seed2 = seed_gen();
	std::mt19937_64 rand       (seed1); // pseudo random number generator
	std::mt19937_64 rand_toFind(seed2); // pseudo random number generator
	
	uint64 interval = 1;
	std::vector<uint64> vecR(limitSize); vecR.clear();
	
	uint64 numFound=0ull, numNotFound=0ull;
	for(;;){
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			vecR <<= r;
			hashT.insert(std::pair<uint64,uint64>(r,r));
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
//			uint64 keyVal = vecR_toFind[i];
//			if(hashT[keyVal] != keyVal){ sstd::pdbg("ERROR: key val is not same."); exit(-1); } // using operator[] will slow down google::dense_hash_map and std::unordered_map
			auto itr = hashT.find( vecR_toFind[i] );
			if(itr!=hashT.end()){ numFound++; }else{ numNotFound++; printf("seed1: %lu, seed2: %lu\n", seed1, seed2); }
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		vecY_quely_per_us <<= ((double)interval * 1000.0) / (nsec);
		
		interval = size2interval(hashT.size());
		if(hashT.size()+interval>limitSize){ break; }
	}
	if(numNotFound!=0ull){
		printf("ERROR: in bench_find(): "); // There are some bugs in the software or the keys are actually clashing.
		printf("%lu / %lu = %lf\n", numFound, numFound+numNotFound, (double)numFound/(double)(numFound+numNotFound));
	}
}
//---
void vvec2plot_find(const std::string& savePath, const std::vector<std::string>& saveAs, const sstd::vvec<double>& vvecX, const sstd::vvec<double>& vvecY){
	const char* xlabel = "Number of elements on the table [conut]";
	const char* ylabel = "Successful search speed [query/μs]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF50)", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF100)", "sstd::IpCHashT<uint64,uint64> (as uint16 and maxLF100)", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}
void bench2plot_find(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_find);
	
	vvec2plot_find(savePath, saveAs, vvecX, vvecY);
}
//---
void bench2csv_find(const std::string& savePath, const uint64 initSize, const uint64 limitSize){
	sstd::vvec<double> vvecX, vvecY;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_find);
	
	sstd::vvec<std::string> vvecHeader = {{"[count]", "uHashT [query/μs]", "cHashT [query/μs]", "iHashT_u8h [query/μs]", "iHashT_u8f [query/μs]", "iHashT_u16 [query/μs]", "dHashT [query/μs]", "fHashT [query/μs]"}};
	BENCH_to_CSV(savePath, vvecX, vvecY, vvecHeader);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------
// find: unsuccessful search

template<typename T_hashTable>
void bench_find_failedAll(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_num, std::vector<double>& vecY_quely_per_us){
	std::random_device seed_gen;
	uint64 seed1 = seed_gen();
	uint64 seed2 = seed_gen();
	std::mt19937_64 rand       (seed1); // pseudo random number generator
	std::mt19937_64 rand_toFind(seed2); // pseudo random number generator
	
	uint64 interval = 1;
	std::vector<uint64> vecR(limitSize); vecR.clear();
	
	uint64 numFound=0ull, numNotFound=0ull;
	for(;;){
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			vecR <<= r;
			hashT.insert(std::pair<uint64,uint64>(r,r));
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
//			if(itr!=hashT.end()){ numFound++; printf("seed1: %lu, seed2: %lu\n", seed1, seed2); }else{ numNotFound++; }
			if(!(itr!=hashT.end())){ numNotFound++; }else{ numFound++; printf("T_hashTable:%s, seed1: %lu, seed2: %lu\n", hashT2typeStr(hashT).c_str(), seed1, seed2); }
		}
		double nsec = sstd::measureTime_stop_ns(timem);
		vecY_quely_per_us <<= ((double)interval * 1000.0) / (nsec);
		
		interval = size2interval(hashT.size());
		if(hashT.size()+interval>limitSize){ break; }
	}
	if(numFound!=0ull){
		printf("ERROR: in bench_find_failedAll(): "); // There are some bugs in the software or the keys are actually clashing.
		printf("%lu / %lu = %lf\n", numFound, numFound+numNotFound, (double)numFound/(double)(numFound+numNotFound));
	}
}
//---
void vvec2plot_find_failedAll(const std::string& savePath, const std::vector<std::string>& saveAs, const sstd::vvec<double>& vvecX, const sstd::vvec<double>& vvecY){
	const char* xlabel = "Number of elements on the table [conut]";
	const char* ylabel = "Unsuccessful search speed [query/μs]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF50)", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF100)", "sstd::IpCHashT<uint64,uint64> (as uint16 and maxLF100)", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}
void bench2plot_find_failedAll(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_find_failedAll);
	
	vvec2plot_find_failedAll(savePath, saveAs, vvecX, vvecY);
}
//---
void bench2csv_find_failedAll(const std::string& savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_find_failedAll);
	
	sstd::vvec<std::string> vvecHeader = {{"[count]", "uHashT [query/μs]", "cHashT [query/μs]", "iHashT_u8h [query/μs]", "iHashT_u8f [query/μs]", "iHashT_u16 [query/μs]", "dHashT [query/μs]", "fHashT [query/μs]"}};
	BENCH_to_CSV(savePath, vvecX, vvecY, vvecHeader);
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
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			vecR <<= r;
			hashT.insert(std::pair<uint64,uint64>(r,r));
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
		
		for(uint i=0; i<interval; i++){
			uint64 r = rand();
			vecR[vecR_idx[i]] = r;
			hashT.insert(std::pair<uint64,uint64>(r,r));
		}
		vecY_quely_per_us <<= ((double)interval * 1000.0) / (nsec);
		
		interval = size2interval(hashT.size());
		if(hashT.size()+interval>limitSize){ break; }
	}
}
//---
void vvec2plot_erase(const std::string& savePath, const std::vector<std::string>& saveAs, const sstd::vvec<double>& vvecX, const sstd::vvec<double>& vvecY){
	const char* xlabel = "Number of elements on the table [conut]";
	const char* ylabel = "Erasion speed [query/μs]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF50)", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF100)", "sstd::IpCHashT<uint64,uint64> (as uint16 and maxLF100)", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}
void bench2plot_erase(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH_withErase(vvecX, vvecY, initSize, limitSize, bench_erase);
	
	vvec2plot_erase(savePath, saveAs, vvecX, vvecY);
}
//---
void bench2csv_erase(const std::string& savePath, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH_withErase(vvecX, vvecY, initSize, limitSize, bench_erase);
	
	sstd::vvec<std::string> vvecHeader = {{"[count]", "uHashT [query/μs]", "cHashT [query/μs]", "iHashT_u8h [query/μs]", "iHashT_u8f [query/μs]", "iHashT_u16 [query/μs]", "dHashT [query/μs]", "fHashT [query/μs]"}};
	BENCH_to_CSV(savePath, vvecX, vvecY, vvecHeader);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

template<typename T_hashTable>
void bench_maxLoadFactor(T_hashTable& hashT, const uint64 limitSize, std::vector<double>& vecX_tSize, std::vector<double>& vecY_lf){
	
	std::random_device seed_gen;
	std::mt19937_64 rand(seed_gen()); // pseudo random number generator
	
	double lf_prev=0.0;
	while(hashT.size()<limitSize){
		uint64 r = rand();
		hashT.insert(std::pair<uint64,uint64>(r,r));
		double lf = hashT.load_factor();
		if(lf < lf_prev){
			vecX_tSize <<= (double)hashT.bucket_count()-1.0;
			vecY_lf    <<= lf_prev;
		}
		lf_prev = lf;
	}
}
//---
void vvec2plot_maxLoadFactor(const std::string& savePath, const std::vector<std::string>& saveAs, const sstd::vvec<double>& vvecX, const sstd::vvec<double>& vvecY){
	const char* xlabel = "Table size [count]";
	const char* ylabel = "Maximum load factor [%]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF50)", "sstd::IpCHashT<uint64,uint64> (as uint8 and maxLF100)", "sstd::IpCHashT<uint64,uint64> (as uint16 and maxLF100)", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_lf";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}
void bench2plot_maxLoadFactor(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	const uint64 initSize=0ull;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_maxLoadFactor);
	
	vvec2plot_maxLoadFactor(savePath, saveAs, vvecX, vvecY);
}
//---
void bench2csv_maxLoadFactor(const std::string& savePath, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	const uint64 initSize=0ull;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_maxLoadFactor);
	
	sstd::vvec<std::string> vvecHeader = {{"[count]", "uHashT [%]", "cHashT [%]", "iHashT_u8h [%]", "iHashT_u8f [%]", "iHashT_u16 [%]", "dHashT [%]", "fHashT [%]"}};
	BENCH_to_CSV(savePath, vvecX, vvecY, vvecHeader);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

uint fileNum(const std::string& path_wWildCard){ return sstd::glob(path_wWildCard).size(); }

void RUN_ALL_BENCHS(){
//	const uint64 limitSize = 200000000; // limit of memory (on 32 GB RAM PC)
	const uint64 limitSize = 5000000;
	const uint64 initSize_wRehash  = 0ull;
	const uint64 initSize_preAlloc = limitSize;
	
	const std::string saveDir = "./tmpBench"; sstd::mkdir(saveDir);
	std::vector<std::string> saveAs = {".pdf", ".png"};
	
	//---------------------------------------------------------------------------------------------------------------------------------------------
	//*
	// insert: insertion speed [query/sec]
	bench2plot_insert(saveDir+"/insert", saveAs, initSize_wRehash, limitSize);
	
	// insert: elapsed time [sec]
	bench2plot_insert_et(saveDir+"/insert_et",          saveAs, initSize_wRehash,  limitSize);
	bench2plot_insert_et(saveDir+"/insert_et_preAlloc", saveAs, initSize_preAlloc, limitSize);
	
	// find: search speed [quely/sec]
	bench2plot_find(saveDir+"/find_successful_search", saveAs, initSize_wRehash, limitSize);
	bench2plot_find_failedAll(saveDir+"/find_unsuccessful_search", saveAs, initSize_wRehash, limitSize);
	
	// erase
	bench2plot_erase(saveDir+"/erase", saveAs, initSize_wRehash, limitSize);
	
	// max-load factor
	bench2plot_maxLoadFactor(saveDir+"/maxLoadFactor", saveAs, limitSize);
	//*/
	//---------------------------------------------------------------------------------------------------------------------------------------------
	/*
	std::string udM    = "usedMemory";          sstd::mkdir(saveDir+'/'+udM   ); sstd::mkdir(saveDir+"/tmp_"+udM   );
	std::string udM_pA = "usedMemory_preAlloc"; sstd::mkdir(saveDir+'/'+udM_pA); sstd::mkdir(saveDir+"/tmp_"+udM_pA);
	for(uint i=fileNum(saveDir+'/'+udM+"/*"); i<1; i++){
		std::string savePath = saveDir +'/'+udM +sstd::ssprintf("/%s_%03u", udM.c_str(), i)+".csv";
		bench2csv_usedMemory(savePath,  saveDir+"/tmp_"+udM, initSize_wRehash, limitSize);
	}
	for(uint i=fileNum(saveDir+'/'+udM_pA+"/*"); i<1; i++){
		std::string savePath = saveDir +'/'+udM_pA +sstd::ssprintf("/%s_%03u", udM_pA.c_str(), i)+".csv";
		bench2csv_usedMemory(savePath, saveDir+"/tmp_"+udM_pA, initSize_preAlloc, limitSize);
	}
	//*/
	//---
	
	uint loopNum = 100;
	/*
	std::string fwR = "/find_successful_search";
	sstd::mkdir(saveDir+'/'+fwR);
	for(uint i=fileNum(saveDir+'/'+fwR+"/*"); i<loopNum; i++){
		std::string savePath = saveDir +'/'+fwR +sstd::ssprintf("/%s_%03u", fwR.c_str(), i)+".csv";
		bench2csv_find(savePath, initSize_wRehash, limitSize);
	}
	
	std::string ffa = "/find_unsuccessful_search";
	sstd::mkdir(saveDir+'/'+ffa);
	for(uint i=fileNum(saveDir+'/'+ffa+"/*"); i<loopNum; i++){
		std::string savePath = saveDir +'/'+ffa +sstd::ssprintf("/%s_%03u", ffa.c_str(), i)+".csv";
		bench2csv_find_failedAll(savePath, initSize_wRehash, limitSize);
	}
	
	std::string iwR = "insert";
	sstd::mkdir(saveDir+'/'+iwR);
	for(uint i=fileNum(saveDir+'/'+iwR+"/*"); i<loopNum; i++){
		std::string savePath = saveDir +'/'+iwR +sstd::ssprintf("/%s_%03u", iwR.c_str(), i)+".csv";
		bench2csv_insert(savePath, initSize_wRehash, limitSize);
	}
	
	std::string iEwR = "insert_et";
	sstd::mkdir(saveDir+'/'+iEwR);
	for(uint i=fileNum(saveDir+'/'+iEwR+"/*"); i<loopNum; i++){
		std::string savePath = saveDir +'/'+iEwR +sstd::ssprintf("/%s_%03u", iEwR.c_str(), i)+".csv";
		bench2csv_insert_et(savePath, initSize_wRehash, limitSize);
	}
	
	std::string iE = "insert_et_preAlloc";
	sstd::mkdir(saveDir+'/'+iE);
	for(uint i=fileNum(saveDir+'/'+iE+"/*"); i<loopNum; i++){
		std::string savePath = saveDir +'/'+iE +sstd::ssprintf("/%s_%03u", iE.c_str(), i)+".csv";
		bench2csv_insert_et(savePath, initSize_preAlloc, limitSize);
	}
	
	std::string ewR = "erase";
	sstd::mkdir(saveDir+'/'+ewR);
	for(uint i=fileNum(saveDir+'/'+ewR+"/*"); i<loopNum; i++){
		std::string savePath = saveDir +'/'+ewR +sstd::ssprintf("/%s_%03u", ewR.c_str(), i)+".csv";
		bench2csv_erase(savePath, initSize_wRehash, limitSize);
	}
	
	std::string mLF = "maxLoadFactor";
	sstd::mkdir(saveDir+'/'+mLF);
	for(uint i=fileNum(saveDir+'/'+mLF+"/*"); i<loopNum; i++){
		std::string savePath = saveDir +'/'+mLF +sstd::ssprintf("/%s_%03u", mLF.c_str(), i)+".csv";
		bench2csv_maxLoadFactor(savePath, limitSize);
	}
	//*/
	//---------------------------------------------------------------------------------------------------------------------------------------------
}

//-----------------------------------------------------------------------------------------------------------------------------------------------


