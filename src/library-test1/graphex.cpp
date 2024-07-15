#include "graphex.h"

#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
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
			add__base(path, patterns);
		}
		
		template<class T_pl_frame, class T_tr_index_frame>
		void Graph<T_pl_frame, T_tr_index_frame>::add__base(std::string path, std::vector<pattern> patterns){
			
			//read file from provided path
			std::ifstream file(path);
			
			if(!file.is_open()){
				std::cerr<<"Cannot open file: "<<path<<std::endl;
				return;
			}
			
			//****** Conceptual struct framework -- used to express exact state of the network
			struct f_pl_concept{
				std::string old_label;
				std::string new_label;
				
				//note: 32bit integer used here since file format
				uint32_t value;
				
				bool exists;
				bool expressed;
				
			};
			
			struct f_tr_concept{
				std::string label;
				std::vector<f_pl_concept*> inputs;
				std::vector<f_pl_concept*> outputs;
				
			};
			//****** Conceptual struct framework END
			
			
			
			
			/*
			struct f_pl_sub{
				std::string sub_specific;
				std::vector<int> whitelist;
			};
			
			//.lpn lists all transitions in as "dummies"
			std::unordered_set<std::string> f_tran_list;
			//grab all place labels
			std::unordered_set<std::string> f_place_list;
			//vector of substituions
			std::unordered_map<std::string, f_pl_sub> f_sub_list;
			
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
							std::cout<<"found transition: "<<token<<std::endl;
							f_tran_list.insert(token);
						}
					} else if(token == ".graph"){
						flag=1;
					}
				} else {
					if(token.rfind(".",0)==0){flag = 0;}
					else {
						if(f_tran_list.find(token) == f_tran_list.end()){
							
							f_place_list.insert(token);
							
							int s_start = token.find("x_"); int s_end = 2;
							if(s_start==0){
								//gained
								s_start = s_end;
								s_end = token.find("_", s_start);
								std::string label_set_name = token.substr(s_start,s_end-s_start);
								//-gained
								std::cout<<"found special place: "<<token.substr(s_start,s_end-s_start)<<std::endl;
								std::cout<<s_start<<" + "<<s_end<<std::endl;
								for(auto&i : patterns){
									if(label_set_name==i.name){
										f_sub_list.insert({token,f_pl_sub{token.replace(s_start,s_end-s_start,i.sub),i.whitelist}});
										std::cout<<"Made substitution: "<<token.replace(s_start,s_end-s_start+1,i.sub)<<std::endl;
									}
								}
							}
						}
					}
				}
			}
			
			for(auto &k:f_sub_list){
				auto vi = k.second.whitelist;
				for(auto &i:vi){
					std::cout<<"Whitelists: "<<i<<std::endl;
				}
			}
			
			file.seekg(0);
			
			std::cout<<"Stating: "<<std::endl;
			for(auto& i:f_tran_list){
				std::cout<<i<<std::endl;
			}
			for(auto& i:f_place_list){
				std::cout<<i<<std::endl;
			}
			*/
			
		}
		
		//cpp nonsense -- workaround for cpp definition not in .h
		void dummy_link()
		{
			Graph<PL_frame<int,int>,TR_index_frame<int,int>> d1;
			d1.add("d",std::vector<pattern>{});
		}

	}
}
