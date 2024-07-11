#include "graphex.h"

#include <fstream>
#include <sstream>
#include <unordered_set>
#include <tuple>

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
		void Graph<T_pl_frame, T_tr_index_frame>::add(std::string path, std::vector<pattern> patterns){
			std::ifstream file(path);
			
			if(!file.is_open()){
				std::cerr<<"Cannot open file: "<<path<<std::endl;
				return;
			}
			
			//.lpn lists all transitions in as "dummies"
			std::unordered_set<std::string> f_tran_list;
			//ascertain all place labels
			std::unordered_set<std::string> f_place_list;
			
			struct pl_concept{
				std::string label;
				std::vector<std::tuple<int, int, PL_header_s<T_pl_frame, T_tr_index_frame>*>> reg;
			};
			
			//*******file iteration -- loop over file to first find the
			//labels of places and transitions (needed because special 
			//names will link different place groups). Then, loop a second
			//time, searching for labels in created place groups. If those
			//places do not exist, then create them. Then link transitions
			//by again searching file for labels.
			std::string line; bool flag = 0;
			//***first pass of file to find transitions and places
			while(std::getline(file,line)){
				std::istringstream iss(line);
				std::string token;
				iss >> token;
				if(flag == 0){
					if(token == ".dummy"){
						while (iss >> token){
							std::cout<<"found dummy: "<<token<<std::endl;
							f_tran_list.insert(token);
						}
					} else if(token == ".graph"){
						flag=1;
					}
				} else {
					if(token.rfind(".",0)==0){flag = 0;}
					else {
						if(f_tran_list.find(token) == f_tran_list.end()){
							std::cout<<"found place: "<<token<<std::endl;
						}
					}
				}
			}
			
			file.seekg(0);
			
			
		}
		
		//cpp nonsense -- workaround for cpp definition not in .h
		void dummy_link()
		{
			Graph<PL_frame<int,int>,TR_index_frame<int,int>> d1;
			std::vector<std::tuple<int,int>> dv1;
			d1.add("d",dv1);
		}

	}
}
