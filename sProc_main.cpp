#include <sstd/sstd.hpp>
#include "./bench.hpp"

//-----------------------------------------------------------------------------------------------------------------------------------------------

void vecPath2v_vvecXY(std::vector<sstd::vvec<double>>& vC_vT_vecX_out, std::vector<sstd::vvec<double>>& vC_vT_vecY_out, const std::vector<std::string>& vecPath){
	vC_vT_vecX_out.resize(vecPath.size());
	vC_vT_vecY_out.resize(vecPath.size());
	
	for(uint i=0; i<vecPath.size(); i++){
		sstd::vvec<std::string> vvecOrig = sstd::csv2vvec(vecPath[i]);
		sstd::vvec<std::string> header   = vvecOrig && sstd::slice_mv(sstd::begin(), 1);
		sstd::vvec<std::string> vvecStr  = vvecOrig && sstd::slice_mv(1, sstd::end());
		sstd::vvec<     double> vvecD    = sstd::Tr(sstd::str2double(vvecStr));
		
		sstd::vvec<double> vT_vecX = {vvecD[0], vvecD[0], vvecD[0], vvecD[0], vvecD[0], vvecD[0]}; // depending on csv format
		sstd::vvec<double> vT_vecY = {vvecD[1], vvecD[2], vvecD[3], vvecD[4], vvecD[5], vvecD[6]}; // depending on csv format
		vC_vT_vecX_out[i] = std::move(vT_vecX);
		vC_vT_vecY_out[i] = std::move(vT_vecY);
	}
}
std::vector<double> vvec2vecMed(const sstd::vvec<double>& rhs){
	std::vector<double> ret(rhs.size());
	for(uint i=0; i<rhs.size(); i++){
		ret[i] = sstd::med(rhs[i]);
	}
	return ret;
}
void vecPath2plot(sstd::vvec<double>& vvecX_out, sstd::vvec<double>& vvecY_out, const std::vector<std::string>& vecPath){
	
	sstd::vec<sstd::vvec<double>> vC_vT_vecX, vC_vT_vecY; // vecCSV vecType vecVal
	vecPath2v_vvecXY(vC_vT_vecX, vC_vT_vecY, vecPath);
	
	vvecX_out                                =          vC_vT_vecX[0];
	sstd::vec<sstd::vvec<double>> vT_vC_vecY = sstd::Tr(vC_vT_vecY);
	
	uint typeNum = vT_vC_vecY.size();
	vvecY_out.resize(typeNum);
	for(uint i=0; i<typeNum; i++){
		sstd::vvec<double> vecY_vC = sstd::Tr(vT_vC_vecY[i]);
		vvecY_out[i] = vvec2vecMed(vecY_vC);
	}
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
	
	{
		sstd::vvec<double> vvecX, vvecY; vecPath2plot(vvecX, vvecY, sstd::glob("./tmpBench/find_wRehash/*"));
		
		const char* savePath="./tmpBench/find_wRehash_med"; std::vector<std::string> saveAs={".png", ".pdf"};
		vvec2plot_find(savePath, saveAs, vvecX, vvecY);
	}
	{
		sstd::vvec<double> vvecX, vvecY; vecPath2plot(vvecX, vvecY, sstd::glob("./tmpBench/find_failedAll/*"));
		
		const char* savePath="./tmpBench/find_failedAll_med"; std::vector<std::string> saveAs={".png", ".pdf"};
		vvec2plot_find(savePath, saveAs, vvecX, vvecY);
	}
	
	printf("\n■ measureTime_stop----------------\n"); sstd::measureTime_stop_print(timem);
	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

