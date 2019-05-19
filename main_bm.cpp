#include <sstd/sstd.hpp>

#include "./bench.hpp"

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
	
	RUN_ALL_BENCHS();
	
	printf("\n■ measureTime_stop----------------\n"); sstd::measureTime_stop_print(timem);
	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

