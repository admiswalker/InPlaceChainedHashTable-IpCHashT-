#include <sstd/sstd.hpp>

//-----------------------------------------------------------------------------------------------------------------------------------------------

int read_csv(const std::vector<std::string>& vecPath){
	
	sstd::printn(vecPath);
	for(uint i=0; i<vecPath.size(); i++){
		std::vector<std::vector<std::string>> vvec = sstd::csv2vvec(vecPath[i]);
		sstd::printn(vvec);
		printf("\n\n");
	}
	
	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv){
	printf("\n");
	printf("+---------------------------------------------------+\n");
	printf("|                                                   |\n");
	printf("|     Welcome to Sub Standard Library (SSTD) !      |\n");
	printf("|                                                   |\n");
	printf("|     > This is an Implementation Plan for          |\n");
	printf("|     > In-placeChainedHashTable (IpCHashT)         |\n");
	printf("|     >   and   ChainedHashTable (  CHashT).        |\n");
	printf("|                                                   |\n");
	printf("+---------------------------------------------------+\n");
	printf("\n");
	printf("■ measureTime_start---------------\n\n"); time_m timem; sstd::measureTime_start(timem);
	
	std::string dirPath = "./tmpBench/*";
	std::vector<std::string> vecPath = sstd::glob(dirPath);
	read_csv(vecPath);
	
	printf("\n■ measureTime_stop----------------\n"); sstd::measureTime_stop_print(timem);
	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

