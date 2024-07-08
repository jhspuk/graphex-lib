#include "graphex.h"

#include <fstream>

namespace graphex{
	namespace petrinet{
		template <class T_pl_frame, class T_tr_index_frame>
		Graph<T_pl_frame, T_tr_index_frame>::Graph(){
			std::cout<<"Hello, world!"<<std::endl;
		}
		template <class T_pl_frame, class T_tr_index_frame>
		Graph<T_pl_frame, T_tr_index_frame>::~Graph(){
			std::cout<<"Goodbye, world!"<<std::endl;
		}
		template <class T_pl_frame, class T_tr_index_frame>
		void Graph<T_pl_frame, T_tr_index_frame>::add(std::string path){
			std::ifstream file(path);
			
			if(!file.is_open()){
				std::cerr<<"Cannot open file: "<<path<<std::endl;
				return;
			}
			
			std::string line;
			while(std::getline(file,line)){
				std::cout<<line<<std::endl;
			}
		}
		
		//cpp nonsense -- workaround for cpp definition not in .h
		void dummy_link()
		{
			Graph<PL_frame<int,int>,TR_index_frame<int,int>> d1;
			d1.add("d");
		}

	}
}
