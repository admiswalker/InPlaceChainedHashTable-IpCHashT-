#include <time.h>
#include <random>

#include <unordered_map>
#include "./CHashT.hpp"
#include "./IpCHashT.hpp"
#include <sparsehash/dense_hash_map> // sparsehash-master
#include "./flat_hash_map-master/flat_hash_map.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------
// definitions

typedef std::unordered_map<uint64,uint64>                                            uHashT;
typedef sstd::CHashT<uint64,uint64>                                                  cHashT;
typedef sstd::IpCHashT<uint64,uint64>                                                iHashT_u8;
typedef sstd::IpCHashT<uint64,uint64,std::hash<uint64>,std::equal_to<uint64>,uint16> iHashT_u16;
typedef google::dense_hash_map<uint64,uint64>                                        dHashT;
typedef ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>         fHashT;

#define RUN_BENCH(vvecX, vvecY, initSize, limitSize, Fnc)				\
	std::vector<double> vecX_u, vecX_c, vecX_i8, vecX_i16, vecX_d, vecX_f; \
	std::vector<double> vecY_u, vecY_c, vecY_i8, vecY_i16, vecY_d, vecY_f; \
	{ uHashT     hashT(initSize); Fnc(hashT, limitSize, vecX_u,   vecY_u  ); } \
	{ cHashT     hashT(initSize); Fnc(hashT, limitSize, vecX_c,   vecY_c  ); } \
	{ iHashT_u8  hashT(initSize); Fnc(hashT, limitSize, vecX_i8,  vecY_i8 ); } \
	{ iHashT_u16 hashT(initSize); Fnc(hashT, limitSize, vecX_i16, vecY_i16); } \
	{ dHashT     hashT(initSize); hashT.set_empty_key(0ull); /* this meen that 'NULL' will not be able to insert as a key-value. */ \
	                              Fnc(hashT, limitSize, vecX_d,   vecY_d  ); } \
	{ fHashT     hashT(initSize); Fnc(hashT, limitSize, vecX_f,   vecY_f  ); } \
	vvecX={vecX_u, vecX_c, vecX_i8, vecX_i16, vecX_d, vecX_f};			\
	vvecY={vecY_u, vecY_c, vecY_i8, vecY_i16, vecY_d, vecY_f};

#define RUN_BENCH_withErase(vvecX, vvecY, initSize, limitSize, Fnc)		\
	std::vector<double> vecX_u, vecX_c, vecX_i8, vecX_i16, vecX_d, vecX_f; \
	std::vector<double> vecY_u, vecY_c, vecY_i8, vecY_i16, vecY_d, vecY_f; \
	{ uHashT     hashT(initSize); Fnc(hashT, limitSize, vecX_u,   vecY_u  ); } \
	{ cHashT     hashT(initSize); Fnc(hashT, limitSize, vecX_c,   vecY_c  ); } \
	{ iHashT_u8  hashT(initSize); Fnc(hashT, limitSize, vecX_i8,  vecY_i8 ); } \
	{ iHashT_u16 hashT(initSize); Fnc(hashT, limitSize, vecX_i16, vecY_i16); } \
	{ dHashT     hashT(initSize); hashT.set_empty_key(0ull); hashT.set_deleted_key(1ull); /* this meen that 'NULL' and '1' will not be able to insert as a key-value. */ \
	                              Fnc(hashT, limitSize, vecX_d,   vecY_d  ); } \
	{ fHashT     hashT(initSize); Fnc(hashT, limitSize, vecX_f,   vecY_f  ); } \
	vvecX={vecX_u, vecX_c, vecX_i8, vecX_i16, vecX_d, vecX_f};			\
	vvecY={vecY_u, vecY_c, vecY_i8, vecY_i16, vecY_d, vecY_f};

#define BENCH_to_CSV(savePath, vvecX, vvecY, vvecHeader)				\
	sstd::vvec<     double> vvec     = {vvecX[0], vvecY[0], vvecY[1], vvecY[2], vvecY[3], vvecY[4], vvecY[5]}; \
	sstd::vvec<std::string> vvec_str = sstd::double2str(sstd::Tr(vvec)); \
	sstd::vvec<std::string> vvec_csv = vvecHeader << vvec_str;			\
	sstd::vvec2csv(savePath, vvec_csv);
	
	// vvecX[0],           vvecY[0],           vvecY[1],              vvecY[2],               vvecY[3],           vvecY[4],          vvecY[5]
	//  [count], uHashT [query/μs], cHashT [query/μs], iHashT_u8 [query/μs], iHashT_u16 [query/μs], dHashT [query/μs], fHashT [query/μs]
	//       0 , 
	//       1 , 
	//      ︙ , 
	//      ︙ ,

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
void bench2plot_insert(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_insert);
	
	const char* xlabel = "Number of elements on the table [conut]";
	const char* ylabel = "Insertion speed [query/μs]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64,std::hash<uint64>,std::equal_to<uint64>,uint16>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
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
void bench2plot_insert_et(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_insert_et);
	
	const char* xlabel = "Number of elements on the table [conut]";
	const char* ylabel = "Elapsed time [sec]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64,std::hash<uint64>,std::equal_to<uint64>,uint16>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_et_insert";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
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
void bench2plot_usedMemory(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize, const uint64 limitSize){
	std::vector<double> vecX_u, vecX_c, vecX_i8, vecX_i16, vecX_d, vecX_f; // memory size [MB]
	std::vector<double> vecY_u, vecY_c, vecY_i8, vecY_i16, vecY_d, vecY_f; // sec
	double baseSize_MB = 0.0;
	
	baseSize_MB = sstd::status_VmRSS()/1024.0;
	uHashT hashT_u(initSize);
	bench_usedMemory(hashT_u, limitSize, vecX_u, vecY_u, baseSize_MB);
	std::vector<char> buf_u((sstd::status_VmHWM()-sstd::status_VmRSS())*1024); // inorder not to count swap memory region
	
	baseSize_MB = sstd::status_VmRSS()/1024.0;
	cHashT hashT_c(initSize);
	bench_usedMemory(hashT_c, limitSize, vecX_c, vecY_c, baseSize_MB);
	std::vector<char> buf_c((sstd::status_VmHWM()-sstd::status_VmRSS())*1024); // inorder not to count swap memory region
	
	baseSize_MB = sstd::status_VmRSS()/1024.0;
	iHashT_u8 hashT_i8(initSize);
	bench_usedMemory(hashT_i8, limitSize, vecX_i8, vecY_i8, baseSize_MB);
	std::vector<char> buf_i8((sstd::status_VmHWM()-sstd::status_VmRSS())*1024); // inorder not to count swap memory region
	
	baseSize_MB = sstd::status_VmRSS()/1024.0;
	iHashT_u16 hashT_i16(initSize);
	bench_usedMemory(hashT_i16, limitSize, vecX_i16, vecY_i16, baseSize_MB);
	std::vector<char> buf_i16((sstd::status_VmHWM()-sstd::status_VmRSS())*1024); // inorder not to count swap memory region
	
	baseSize_MB = sstd::status_VmRSS()/1024.0;
	dHashT hashT_d(initSize); hashT_d.set_empty_key(0ull); // this meen that 'NULL' will not be able to insert as a key-value.
	bench_usedMemory(hashT_d, limitSize, vecX_d, vecY_d, baseSize_MB);
	std::vector<char> buf_d((sstd::status_VmHWM()-sstd::status_VmRSS())*1024); // inorder not to count swap memory region
	
	baseSize_MB = sstd::status_VmRSS()/1024.0;
	fHashT hashT_f(initSize);
	bench_usedMemory(hashT_f, limitSize, vecX_f, vecY_f, baseSize_MB);
	
	const char* xlabel = "Number of elements on the table [conut]";
	const char* ylabel = "Allocated memory size [MB]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64,std::hash<uint64>,std::equal_to<uint64>,uint16>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	std::vector<std::vector<double>> vvecX={vecX_u, vecX_c, vecX_i8, vecX_i16, vecX_d, vecX_f}; // memory size [MB]
	std::vector<std::vector<double>> vvecY={vecY_u, vecY_c, vecY_i8, vecY_i16, vecY_d, vecY_f}; // sec
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_memory";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
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
void vvec2plot_find(const std::string& savePath, const std::vector<std::string>& saveAs, const sstd::vvec<double>& vvecX, const sstd::vvec<double>& vvecY){
	const char* xlabel = "Number of elements on the table [conut]";
	const char* ylabel = "Find speed [query/μs]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64,std::hash<uint64>,std::equal_to<uint64>,uint16>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
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
void bench2csv_find(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_find);
	
	std::vector<std::vector<std::string>> vvecHeader = {{"[count]", "uHashT [query/μs]", "cHashT [query/μs]", "iHashT_u8 [query/μs]", "iHashT_u16 [query/μs]", "dHashT [query/μs]", "fHashT [query/μs]"}};
	BENCH_to_CSV(savePath, vvecX, vvecY, vvecHeader);
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
void vvec2plot_find_failedAll(const std::string& savePath, const std::vector<std::string>& saveAs, const sstd::vvec<double>& vvecX, const sstd::vvec<double>& vvecY){
	const char* xlabel = "Number of elements on the table [conut]";
	const char* ylabel = "Find speed [query/μs]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64,std::hash<uint64>,std::equal_to<uint64>,uint16>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
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
void bench2csv_find_failedAll(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_find_failedAll);
	
	const char* xlabel = "Number of elements on the table [conut]";
	const char* ylabel = "Find speed [query/μs]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64,std::hash<uint64>,std::equal_to<uint64>,uint16>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);

	// here is under construction
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
	// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
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
void bench2plot_find_erase_insert(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH_withErase(vvecX, vvecY, initSize, initSize, bench_find_erase_insert);
	
	const char* xlabel = "Non-changed rate [\%]";
	const char* ylabel = "A speed of one find-erase-insert cycle [query/μs]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64,std::hash<uint64>,std::equal_to<uint64>,uint16>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_FEA";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
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
void bench2plot_find_findFailedAll_erase_insert(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH_withErase(vvecX, vvecY, initSize, initSize, bench_find_findFailedAll_erase_insert);
	
	const char* xlabel = "Non-changed rate [\%]";
	const char* ylabel = "A speed of one find-erase-insert cycle [query/μs]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64,std::hash<uint64>,std::equal_to<uint64>,uint16>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_FEA";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
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
void bench2plot_erase(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 initSize, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	RUN_BENCH_withErase(vvecX, vvecY, initSize, limitSize, bench_erase);
	
	const char* xlabel = "Number of elements on the table [conut]\n(This axis is inverted.)";
	const char* ylabel = "Erasion speed [query/μs]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64,std::hash<uint64>,std::equal_to<uint64>,uint16>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
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
void bench2plot_maxLoadFactor(const std::string& savePath, const std::vector<std::string>& saveAs, const uint64 limitSize){
	std::vector<std::vector<double>> vvecX, vvecY;
	const uint64 initSize=0ull;
	RUN_BENCH(vvecX, vvecY, initSize, limitSize, bench_maxLoadFactor);
	
	const char* xlabel = "Table size [count]";
	const char* ylabel = "maximum load factor [%]";
	std::vector<std::string> vecLabel={"std::unordered_map<uint64,uint64>", "sstd::CHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64>", "sstd::IpCHashT<uint64,uint64,std::hash<uint64>,std::equal_to<uint64>,uint16>", "google::dense_hash_map<uint64,uint64>", "ska::flat_hash_map<uint64,uint64,ska::power_of_two_std_hash<uint64>>"};
	
	// plot2fig
	const char* tmpDir   = "./tmpDir";
	const char* fileName = "plots";
	const char* funcName = "vvec2graph_lf";
	sstd::c2py<void> vvec2graph(tmpDir, fileName, funcName, "void, const str, const vec<str>*, const char*, const char*, const vec<str>*, const vvec<double>*, const vvec<double>*");
	vvec2graph(savePath, &saveAs, xlabel, ylabel, &vecLabel, &vvecX, &vvecY);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

void RUN_ALL_BENCHS(){
//	const uint64 limitSize = 200000000; // limit of memory
	const uint64 limitSize = 5000000;
	const uint64 initSize_wRehash  = 0ull;
	const uint64 initSize_preAlloc = limitSize;
	
	const std::string saveDir = "./tmpBench"; sstd::mkdir(saveDir);
	std::vector<std::string> saveAs = {".pdf", ".png"};
	/*
	// bench of used memory size should run first inorder to avoid memory swap by Linux OS.
	bench2plot_usedMemory(saveDir+"/usedMemory_wRehash_log",  saveAs, initSize_wRehash, limitSize);
	bench2plot_usedMemory(saveDir+"/usedMemory_preAlloc_log", saveAs, initSize_preAlloc, limitSize);
	
	// Warm running, because of the first bench usually returns bad result.
	bench2plot_insert(saveDir+"/warmRunning", saveAs, initSize_wRehash, limitSize); // pre-allocate
	
	// insert: insertion speed [query/sec]
	bench2plot_insert(saveDir+"/insert_wRehash", saveAs, initSize_wRehash, limitSize); // with rehash
	
	// insert: elapsed time [sec]
	bench2plot_insert_et(saveDir+"/insert_et_wRehash",  saveAs, initSize_wRehash,  limitSize); // with rehash
	bench2plot_insert_et(saveDir+"/insert_et_preAlloc", saveAs, initSize_preAlloc, limitSize); // pre-allocate
	//*/
	// find: find speed [quely/sec]
//	bench2plot_find(saveDir+"/find_wRehash", saveAs, initSize_wRehash, limitSize); // with rehash
	std::string saveDir_fwR = "find_wRehash";
	sstd::mkdir(saveDir+'/'+saveDir_fwR);
	for(uint i=0; i<100; i++){ // 17 mins
		std::string savePath = saveDir+'/'+saveDir_fwR+"/find_wRehash_"+sstd::ssprintf("%03u", i)+".csv";
		bench2csv_find(savePath, saveAs, initSize_wRehash, limitSize); // with rehash
	}
	/*
	// find: all lookup is failed
	bench2plot_find_failedAll(saveDir+"/find_wRehash_failedAll", saveAs, initSize_wRehash, limitSize);
	
	// erase
	bench2plot_erase(saveDir+"/erase_wRehash", saveAs, initSize_wRehash, limitSize); // pre-allocate
	
	// find -> erase -> insert (reHash occurd)
	bench2plot_find_erase_insert(saveDir+"/find_erase_insert_pow10_4", saveAs, 10000  ); // find with erasion
	bench2plot_find_erase_insert(saveDir+"/find_erase_insert_pow10_5", saveAs, 100000 ); // find with erasion
	bench2plot_find_erase_insert(saveDir+"/find_erase_insert_pow10_6", saveAs, 1000000); // find with erasion
	
	bench2plot_find_findFailedAll_erase_insert(saveDir+"/find_fainFailedAll_erase_insert_pow10_6", saveAs, 1000000); // find with erasion
	
	// max-load factor
	bench2plot_maxLoadFactor(saveDir+"/maxLoadFactor", saveAs, limitSize);
	//*/
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

