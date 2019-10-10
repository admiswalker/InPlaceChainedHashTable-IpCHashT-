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
		
		sstd::vvec<double> vT_vecX; for(uint i=0; i<vvecD.size()-1; i++){ vT_vecX<<=vvecD[0]; } // depending on csv format
		sstd::vvec<double> vT_vecY; for(uint i=1; i<vvecD.size()  ; i++){ vT_vecY<<=vvecD[i]; } // depending on csv format
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
void vecPath2vvecXY(sstd::vvec<double>& vvecX_out, sstd::vvec<double>& vvecY_out, const std::vector<std::string>& vecPath){
	
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
	
	std::vector<std::string> saveAs={".png", ".pdf"};
	
	{
		const char* csvPath  = "./tmpBench/usedMemory/*";
		const char* savePath = "./tmpBench/usedMemory";
		std::vector<std::string> vecPath = sstd::glob(csvPath);
		
		if(vecPath.size()!=0){
			sstd::vvec<double> vvecX, vvecY; vecPath2vvecXY(vvecX, vvecY, vecPath);
			vvec2plot_usedMemory(savePath, saveAs, vvecX, vvecY);
		}
	}
	{
		const char* csvPath  = "./tmpBench/usedMemory_preAlloc/*";
		const char* savePath = "./tmpBench/usedMemory_preAlloc";
		std::vector<std::string> vecPath = sstd::glob(csvPath);
		
		if(vecPath.size()!=0){
			sstd::vvec<double> vvecX, vvecY; vecPath2vvecXY(vvecX, vvecY, vecPath);
			vvec2plot_usedMemory(savePath, saveAs, vvecX, vvecY);
		}
	}
	{
		const char* csvPath  = "./tmpBench/find_successful_search/*";
		const char* savePath = "./tmpBench/find_successful_search_med";
		std::vector<std::string> vecPath = sstd::glob(csvPath);
		
		if(vecPath.size()!=0){
			sstd::vvec<double> vvecX, vvecY; vecPath2vvecXY(vvecX, vvecY, vecPath);
			vvec2plot_find(savePath, saveAs, vvecX, vvecY);
		}
	}
	{
		const char* csvPath  = "./tmpBench/find_unsuccessful_search/*";
		const char* savePath = "./tmpBench/find_unsuccessful_search_med";
		std::vector<std::string> vecPath = sstd::glob(csvPath);
		
		if(vecPath.size()!=0){
			sstd::vvec<double> vvecX, vvecY; vecPath2vvecXY(vvecX, vvecY, vecPath);
			vvec2plot_find_failedAll(savePath, saveAs, vvecX, vvecY);
		}
	}
	{
		const char* csvPath  = "./tmpBench/insert/*";
		const char* savePath = "./tmpBench/insert_med";
		std::vector<std::string> vecPath = sstd::glob(csvPath);
		
		if(vecPath.size()!=0){
			sstd::vvec<double> vvecX, vvecY; vecPath2vvecXY(vvecX, vvecY, vecPath);
			vvec2plot_insert(savePath, saveAs, vvecX, vvecY);
		}
	}
	{
		const char* csvPath  = "./tmpBench/insert_et/*";
		const char* savePath = "./tmpBench/insert_et_med";
		std::vector<std::string> vecPath = sstd::glob(csvPath);
		
		if(vecPath.size()!=0){
			sstd::vvec<double> vvecX, vvecY; vecPath2vvecXY(vvecX, vvecY, vecPath);
			vvec2plot_insert_et(savePath, saveAs, vvecX, vvecY);
		}
	}
	{
		const char* csvPath  = "./tmpBench/insert_et_preAlloc/*";
		const char* savePath = "./tmpBench/insert_et_preAlloc_med";
		std::vector<std::string> vecPath = sstd::glob(csvPath);
		
		if(vecPath.size()!=0){
			sstd::vvec<double> vvecX, vvecY; vecPath2vvecXY(vvecX, vvecY, sstd::glob(csvPath));
			vvec2plot_insert_et(savePath, saveAs, vvecX, vvecY);
		}
	}
	{
		const char* csvPath  = "./tmpBench/erase/*";
		const char* savePath = "./tmpBench/erase_med";
		std::vector<std::string> vecPath = sstd::glob(csvPath);
		
		if(vecPath.size()!=0){
			sstd::vvec<double> vvecX, vvecY; vecPath2vvecXY(vvecX, vvecY, vecPath);
			for(uint i=0; i<vvecX.size(); i++){ // vecType
				vvecX[i] = sstd::nonzero(vvecX[i]);
				vvecY[i] = sstd::nonzero(vvecY[i]);
				sstd::suppress(vvecX[i], vvecY[i]); // <-> padding
			}
			vvec2plot_erase(savePath, saveAs, vvecX, vvecY);
		}
	}
	{
		const char* csvPath  = "./tmpBench/maxLoadFactor/*";
		const char* savePath = "./tmpBench/maxLoadFactor_med";
		std::vector<std::string> vecPath = sstd::glob(csvPath);
		
		if(vecPath.size()!=0){
			sstd::vvec<double> vvecX, vvecY; vecPath2vvecXY(vvecX, vvecY, vecPath);
			for(uint i=0; i<vvecX.size(); i++){ // vecType
				vvecX[i] = sstd::nonzero(vvecX[i]);
				vvecY[i] = sstd::nonzero(vvecY[i]);
				sstd::suppress(vvecX[i], vvecY[i]); // <-> padding
			}
			vvec2plot_maxLoadFactor(savePath, saveAs, vvecX, vvecY);
		}
	}
	
	printf("\n■ measureTime_stop----------------\n"); sstd::measureTime_stop_print(timem);
	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

