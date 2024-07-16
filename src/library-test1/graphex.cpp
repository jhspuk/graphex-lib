#include "graphex.h"

#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <tuple>

#define DEBUG_BUILD

#ifdef DEBUG_BUILD 
#define D(x) x
#else 
#define D(x)
#endif

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
		
		template<class T_pl_frame, class T_tr_index_frame>
		T_tr_index_frame Graph<T_pl_frame, T_tr_index_frame>::find(std::string search_term, std::vector<int> whitelist){
			//for each specified place group (specified in whitelist)
			for(auto& mask : whitelist){
				//scan the body of the place group's labels for a match
				for(auto& i : pl_reg[mask]->body->labels){
					//if there's a match, then calculate the exact index
					if(i==search_term){
						T_tr_index_frame index_f = {mask,distance(pl_reg[mask]->body->labels.begin(),i)};
						return index_f;
					}
				}
			}
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
				
				std::vector<int> whitelist;
				
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
			
			std::unordered_map<std::string, f_tr_concept*> f_tr_map;
			std::unordered_map<std::string, f_pl_concept*> f_pl_map;
			
			std::string prefix_shared = "x_";
			
			using namespace std;
			
			string line;
			bool flag1 = 1;
			bool flag2 = 0;
			bool flag3 = 0;
			bool flag4 = 0;
			bool flag5 = 0;
			bool flag5_2 = 0;
			bool flag6 = 0;
			
			while(getline(file,line)){
				istringstream iss(line);
				string token;
				iss >> token;
				if(flag1){
					if(token == ".name") {
						
					} else if(token == ".dummy"){
						while(iss >> token){
							
							cout<<"found transition: "<<token<<endl; //debug
							
							//create a new concept struct and add it to the map
							//map is there as a more efficient array of concept structs
							f_tr_map.insert({token, new f_tr_concept{token,{},{}}});
							
							cout<<"pointer: "<<f_tr_map.at(token)->label<<endl; //debug
						}
					} else if(token == ".graph"){
						//found graph section of lpn, start graph section routine
						flag1 = 0;
						flag2 = 1;
					}
				} else if(flag2) {
					//begin the first pass of graph to populate place structs
					//simultaneously, parse substitution list where applicable
					
					//when .graph section has finished, move onto next routine
					if(token.rfind(".",0)==0){
						flag2 = 0;
						flag3 = 1;
						//go back to the beginning of the file for another pass
						file.seekg(0);
					} else {
						//if the line starts with a transition, ditch, since we're
						//reading in place labels that exist
						if(f_tr_map.find(token) == f_tr_map.end()){
							
							//create new place concept and add it to the map
							f_pl_concept* current_place_p = new f_pl_concept;
							f_pl_map.insert({token, current_place_p});
							
							//old label is current name
							current_place_p->old_label = token;
							
							int s_start = token.find(prefix_shared,0);
							int s_end = prefix_shared.length();
							
							//if there exists a special prefix for shared memory
							//then there may exist a valid substitution
							if(s_start==0){
								
								//grab the label set name (between prefix and the next underscore)
								s_start = s_end;
								s_end = token.find("_", s_start);
								std::string label_set_name = token.substr(s_start,s_end-s_start);
								
								cout<<"special prefix found!: "<< label_set_name<<endl; //debug
								//search patterns to find if it belongs to a substitution
								bool pattern_present = 0;
								for(auto& pattern_it : patterns){
									//test if there is a valid substitution pattern
									if(pattern_it.name == label_set_name){
										//replace the label set name with the substitution
										token.replace(s_start,s_end-s_start,pattern_it.sub);
										//add altered name to the current place concept
										current_place_p->new_label = token;
										//add whitelist from pattern to concept
										current_place_p->whitelist = pattern_it.whitelist;
										//if pattern is present, set flag
										pattern_present = 1;
									}
								}
								if(pattern_present == 0){
									current_place_p->new_label = token;
									current_place_p->whitelist = {};
								}
							} else {
								//if no match, copy unchanged label
								cout<<"Should be empty?: "<<token<<endl;
								current_place_p->new_label = token;
								current_place_p->whitelist = {};
							}
						}
					}
					
				} else if(flag3){
					if(token == ".capacity"){
						
					} else if(token == ".marking"){
						bool first = 1;
						uint32_t capacity = 0;
						while(iss >> token){
							
							//remove first bracket "{"
							if(first){
								token = token.substr(1, token.length());
								first = 0;
							}
							
							//find if there's an equals sign - this gives a specific
							//capacity value, otherwise implicit '1'
							int s_start = 0;
							int s_end = token.find("=", s_start);
							if(s_end != -1){
								
								//check for end bracket "}" and remove
								s_start = token.find("}", s_start);
								if(s_start!=-1){
									token.erase(s_start,1);
								}
								
								//use position of equals to find number, then convert to integer
								cout<<token.substr(s_end+1, token.length())<<" : "<<s_end<<endl; //debug
								capacity = stoi(token.substr(s_end+1, token.length()));
								
								cout<<"normals: "<<token.substr(0, s_end+1)<<endl; //debug
								//find relevant place and set capacity
								f_pl_map.at(token.substr(0, s_end))->value = capacity;
								
							} else {
								//implicit '1' assumed, set concept capacity to 1
								f_pl_map.at(token)->value = capacity;
							}
							cout<<"Capacity: "<<capacity<<endl; //debug
						}
					} else if(token == ".end"){
						flag3 = 0;
						flag4 = 1;
						//go back to the beginning of the file for another pass
						file.seekg(0);
					}
				} else if(flag4){
					if(token == ".graph"){
						flag4 = 0;
						flag5 = 1;
					}
				} else if(flag5){
					
					g_5_2:
					if(!flag5_2){
						D(cout<<"Link-file-to-concept pass, tr:"<<token<<endl;)
						//for each transition, their outputs are listed
						//catch tr-pl boundary in file (pl token is not in tr map)
						try{
							//find transition concept, add place concepts to it from same file line
							f_tr_concept* tr = f_tr_map.at(token);
							f_pl_concept* pl;
							while(iss >> token){
								pl = f_pl_map.at(token);
								tr->inputs.push_back(pl);
							}
						}catch(const out_of_range& e){
							D(cerr<<"Exception reached TR not in map (normal once) "<<token<<endl;)
							flag5_2 = 1;
							goto g_5_2;
						}
					}else{
						//if . found, graph field has finished
						if(token.rfind(".",0)==0){
							flag5 = 0;
							flag5_2 = 0;
							flag6 = 1;
						}
						D(cout<<"Link-file-to-concept pass, pl:"<<token<<endl;)
						try{
							//find place concept, add itself to transition concepts on same file line
							f_tr_concept* tr;
							f_pl_concept* pl = f_pl_map.at(token);
							while(iss >> token){
									tr = f_tr_map.at(token);
									tr->outputs.push_back(pl);
							}
						}catch(const out_of_range& e){
								D(cerr<<"Exception reached PL not in map (normal once) "<<token<<endl;)
						}
					} 
					
					
					//cout<<tr->label<<endl;
				} else if(flag6){
					D(cout<<"File read: "<<path<<endl;)
					break;
				}
				
				
				
			}
			for(auto&i : f_tr_map){
				for(auto&k : i.second->inputs){
					cout<<k->new_label<<endl;
				}
			}
		}
		
		//cpp nonsense -- workaround for cpp definition not in .h
		void dummy_link()
		{
			Graph<PL_frame<int,int>,TR_index_frame<int,int>> d1;
			d1.add("d",std::vector<pattern>{});
		}

	}
}
