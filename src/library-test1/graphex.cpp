#include "graphex.h"

#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <iterator>
#include <type_traits>

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
		int Graph<T_pl_frame, T_tr_index_frame>::find(std::string search_term, std::vector<int> whitelist, T_tr_index_frame& index_i_f){
			using namespace std;
			//for each specified place group (specified in whitelist)
			for(auto& mask : whitelist){
				
				//make sure whitelist is not out of bounds
				if((int)pl_reg.size() == 0){
					break;
				}
				if(mask > (int)pl_reg.size()-1){
					cerr<<"Compile error: requesting whitelist search out of bounds!"<<endl;
					break;
				}
				D(cout<<search_term<<" ...searching whitelist in "<<pl_reg.size()<<" many"<<endl;)
				//scan the body of the place group's labels for a match
				int count = 0;
				for(auto& i : pl_reg[mask]->body->labels){
					//if there's a match, then calculate the exact index
					if(i==search_term){
						T_tr_index_frame index_f = {mask,count};
						index_i_f = index_f;
						return mask;
					}
					count++;
				}
			}
			
			return -1;
		}
		
		template <class T_pl_frame, class T_tr_index_frame>
		int Graph<T_pl_frame, T_tr_index_frame>::compile(std::vector<loader_pl_concept<T_tr_index_frame>*> pl_concepts, std::vector<loader_tr_concept<T_tr_index_frame>*> tr_concepts, std::string name_f){
			
			using namespace std;
			//****** DEBUG Section -- print data about loaded concepts
			#ifdef DEBUG_BUILD
			cout<<"DEBUG: "<<__func__<<endl;
			for(auto&i:pl_concepts){
				cout<<i->old_label<<", "<<i->new_label<<" - capacity: "<<i->value<<endl;
			}
			
			for(auto&i:tr_concepts){
				cout<<i->label<<" - inputs: ";
				for(auto&k:i->inputs){
					cout<<k->new_label<<" ";
				}
				cout<<std::endl<<"outputs: ";
				for(auto&k:i->outputs){
					cout<<k->new_label<<" ";
				}
				cout<<std::endl;
			}
			cout<<"DEBUG: "<<__func__<<" END"<<endl;
			#endif
			//****** DEBUG Section END
			
			
			//overall index of transition and place group
			int tr_index_f; int pl_index_f;
			//create a new TR header - there is only ever one per addition - and one (unshared) PL group
			TR_header_s<T_pl_frame, T_tr_index_frame>* tr_group_f = new TR_header_s<T_pl_frame, T_tr_index_frame>;
			PL_header_s<T_pl_frame, T_tr_index_frame>* pl_group_f = new PL_header_s<T_pl_frame, T_tr_index_frame>;
			//create new tr pl link
			PL_TR_link<T_pl_frame, T_tr_index_frame>* link_f = new PL_TR_link<T_pl_frame, T_tr_index_frame>;
			
			//configure tr pl link
			link_f->tr_p = tr_group_f;
			link_f->pl_p = pl_group_f;
			link_f->change = 1;	//change is one to force first compute
			
			//configure base place group
			pl_group_f->body = new PL_s<T_pl_frame>;
			pl_group_f->protection = 0;
			pl_group_f->descriptor.push_back(name_f);
			pl_group_f->link_reg.push_back(link_f);
			
			//configure root transition group
			tr_group_f->body = new TR_s<T_tr_index_frame>;
			tr_group_f->descriptor.push_back(name_f);
			tr_group_f->place_reg.push_back(pl_group_f);
			tr_group_f->link_reg.push_back(link_f);
			
			//special, protected pl group pointer (there may be multiple)
			PL_header_s<T_pl_frame, T_tr_index_frame>* pl_group_temp_f;
			//links for shared place groups
			PL_TR_link<T_pl_frame, T_tr_index_frame>* link_temp_f;
			
			//add TR, PL header to the register
			tr_reg.push_back(tr_group_f);
			pl_reg.push_back(pl_group_f);
			//calculate index value of newly copied region
			tr_index_f = tr_reg.size() - 1;
			pl_index_f = pl_reg.size() - 1;
			
			//map to find common label sets (that do not currently exist) such that new PL groups
			//can be created to contain them
			unordered_map<string, vector<loader_pl_concept<T_tr_index_frame>*>> pl_groupings;
			unordered_map<string, PL_header_s<T_pl_frame, T_tr_index_frame>*> pl_groupings_reg;
			
			unordered_map<int, vector<loader_pl_concept<T_tr_index_frame>*>> pl_groupings_exist;
			
			T_tr_index_frame search_result = {0,0};
			T_pl_frame contents = {0,0};

			for(auto&i : pl_concepts){
				
				int index_result = find(i->new_label, i->whitelist, search_result);
				//determine what kind of place is incoming, and resolve index
				if(index_result >= 0){
					//case one: the concept is special and
					//there already exists a shared place within the whitelist
					//therefore we know its index 
					i->index_l = search_result;
					i->exists=1;
					
					auto it = pl_groupings_exist.find(index_result);
					if(it == pl_groupings_exist.end()){
						pl_groupings_exist.insert({index_result,vector<loader_pl_concept<T_tr_index_frame>*>{i}});
					} else {
						it->second.push_back(i);
					}
					
				} else if(i->special != 1) {
					//case two: the place is not special, therefore we will be
					//creating it 
					D(cout<<i->new_label<<" does not exist!"<<endl;)
					//place does not already exist
					i->exists = 0;
					
					//create contents
					contents.place_type = 0;
					contents.place_data = static_cast<decltype(contents.place_data)>(i->value);
					
					//default place group index to 0 since this place group is
					//the central one
					i->index_l.group_index = 0;
					
					pl_group_f->body->data.push_back(contents);
					pl_group_f->body->labels.push_back(i->new_label);
					
					i->index_l.place_index = static_cast<decltype(i->index_l.place_index)>(pl_group_f->body->data.size() - 1);
					
				} else {
					//case three: the place is special, but nothing was returned
					//in this case we therefore must create this shared memory
					//such that it can be shared later on. However, the shared
					//pl groups are bundled by label set, therefore we must also
					//find its relatives when deciding how to create these new regions
					i->exists=1;
					auto it=pl_groupings.find(i->label_set);
					if(it == pl_groupings.end()){
						//Success: this label set is unique: create a new pl group
						//add pl_concept to shared groupings
						pl_groupings.insert({i->label_set,vector<loader_pl_concept<T_tr_index_frame>*>{i}});
						//since grouping for this label set does not exist, set up a new place group and set up its features
						pl_group_temp_f = new PL_header_s<T_pl_frame, T_tr_index_frame>;
						//configurations of this new place group...
						pl_group_temp_f->protection=1;
						pl_group_temp_f->descriptor.push_back(i->label_set);
						pl_group_temp_f->body = new PL_s<T_pl_frame>;
						
						pl_reg.push_back(pl_group_temp_f);
						pl_group_temp_f->index_graph = pl_reg.size() - 1;
						
						pl_groupings_reg.insert({i->label_set,pl_group_temp_f});
						
						link_temp_f = new PL_TR_link<T_pl_frame, T_tr_index_frame>;
						link_temp_f->pl_p = pl_group_temp_f;
						link_temp_f->tr_p = tr_group_f;
						link_temp_f->change = 1;
						
						tr_group_f->link_reg.push_back(link_temp_f);
						pl_group_temp_f->link_reg.push_back(link_temp_f);
						
						//set group index to newly created place region
						//search_result.group_index = static_cast<decltype(search_result.group_index)>(pl_groupings_reg.at(i->label_set)->index_graph);
						
						//set contents data to value of concept
						//contents.place_data = static_cast<decltype(contents.place_data)>(i->value);
						
						//append data of concept
						//pl_group_temp_f->body->data.push_back(contents);
						//pl_group_temp_f->body->labels.push_back(i->new_label);
						
						//append newly created index from data to the place_index
						//search_result.place_index = pl_group_temp_f->body->data.size() - 1;
					} else {
						//Otherwise, this label set is not unique: find its record
						//and append the current concept to it
						it->second.push_back(i);
						
						search_result.group_index = static_cast<decltype(search_result.group_index)>(pl_groupings_reg.at(i->label_set)->index_graph);
						
						//set contents data to value of concept
						//contents.place_data = static_cast<decltype(contents.place_data)>(i->value);
						
						//append data of concept
						//pl_group_temp_f->body->data.push_back(contents);
						//pl_group_temp_f->body->labels.push_back(i->new_label);
						
						//append newly created index from data to the place_index
						//search_result.place_index = pl_group_temp_f->body->data.size() - 1;
					}
				}
			}
			
			T_tr_index_frame tr_index_temp_f = {0,0};
			
			for(auto&i : pl_groupings){
				D(cout<<"Groupings: "<<i.first<<endl;)
				pl_group_temp_f = pl_groupings_reg.at(i.first);
				
				decltype(tr_index_temp_f.group_index) tr_index_l_f;
				
				tr_group_f->place_reg.push_back(pl_group_temp_f);
				tr_index_l_f = tr_group_f->place_reg.size() - 1;
				
				tr_index_temp_f.group_index = tr_index_l_f;
				
				for(auto&k : i.second){
					k->index_l.group_index = tr_index_temp_f.group_index;
					//determine the values of the new to be created place
					//note: type_data here defaults 0
					contents.place_type = 0;
					contents.place_data = static_cast<decltype(contents.place_data)>(k->value);
					//create place data since this batch does not exist
					pl_group_temp_f->body->data.push_back(contents);
					pl_group_temp_f->body->labels.push_back(k->new_label);
					
					//find inner index based on new size of vector
					k->index_l.place_index = static_cast<decltype(k->index_l.place_index)>(pl_group_temp_f->body->data.size() - 1);
				}
			}
			
			//ensure that all existing place concepts have the correct index associated with them
			//no data should be added to be place group
			//links must also be created to accommodate this place group - tr group connection
			for(auto&i : pl_groupings_exist){
				D(cout<<"Groupings that exist: "<<i.first<<endl;)
				link_temp_f = new PL_TR_link<T_pl_frame, T_tr_index_frame>;
				link_temp_f->pl_p = pl_reg[i.first];
				link_temp_f->tr_p = tr_group_f;
				link_temp_f->change = 1;
				
				tr_group_f->link_reg.push_back(link_temp_f);
				pl_reg[i.first]->link_reg.push_back(link_temp_f);
				
				decltype(tr_index_temp_f.group_index) tr_index_l_f;
				
				tr_group_f->place_reg.push_back(pl_reg[i.first]);
				tr_index_l_f = tr_group_f->place_reg.size() - 1;
				
				tr_index_temp_f.group_index = tr_index_l_f;
				
				for(auto&k : i.second){
					//contents.place_data = static_cast<decltype(contents.place_data)>(k->value);
					//pl_reg[i.first]->body->data.push_back(contents);
					//pl_reg[i.first]->body->labels.push_back(k->new_label);
					k->index_l.group_index = tr_index_temp_f.group_index;
				}
			}
			
			//******transition group linking
			
			for(auto&i : tr_concepts){
				
				//push back input and output connections
				//Important note: input/output vector is
				//INTERLEAVED within TR_s.data, with input
				//stated first
				std::vector<T_tr_index_frame> tr_index_lv_f;
				for(auto&k : i->inputs){
					tr_index_lv_f.push_back(k->index_l);
				}
				tr_group_f->body->data.push_back(tr_index_lv_f);
				tr_index_lv_f.clear();
				
				for(auto&k : i->outputs){
					tr_index_lv_f.push_back(k->index_l);
				}
				tr_group_f->body->data.push_back(tr_index_lv_f);
			}
			
			//******transition group linking -- END
			
			#ifdef DEBUG_BUILD
			for(auto&i : tr_group_f->place_reg){
				cout<<i->link_reg[0]<<endl;
				for(auto&k : i->body->labels){
					cout<<"For PG "<<i->descriptor[0]<<": label: "<<k<<endl;
				}
			}
			
			for(auto&i:pl_concepts){
				cout<<"Concept: "<<i->new_label<<" Index: "<<i->index_l.group_index<<":"<<i->index_l.place_index<<":"<<i->value<<endl;
			}
			
			for(auto&i:tr_group_f->body->data){
				cout<<"segment"<<endl;
				for(auto&j:i){
					cout<<"TR index: "<<j.group_index<<":"<<j.place_index<<endl;
				}
			}
			#endif
			
			D(cout<<"Compile finished"<<endl;)
			return tr_index_f;
		}
		
		template <class T_pl_frame, class T_tr_index_frame>
		int Graph<T_pl_frame, T_tr_index_frame>::add(std::string path, std::vector<pattern> patterns){
			return add__base(path, patterns);
		}
		
		template<class T_pl_frame, class T_tr_index_frame>
		int Graph<T_pl_frame, T_tr_index_frame>::add__base(std::string path, std::vector<pattern> patterns){
			
			//read file from provided path
			std::ifstream file(path);
			
			if(!file.is_open()){
				std::cerr<<"Cannot open file: "<<path<<std::endl;
				return -1;
			}
			
			//****** Conceptual struct framework -- used to express exact state of the network
			/*
			struct loader_pl_concept{
				std::string old_label;
				std::string new_label;
				
				std::vector<int> whitelist;
				
				//note: 32bit integer used here since file format
				uint32_t value;
				
				bool exists;
				bool expressed;
				
			};
			
			struct loader_tr_concept{
				std::string label;
				std::vector<loader_pl_concept*> inputs;
				std::vector<loader_pl_concept*> outputs;
				
			};
			//****** Conceptual struct framework END
			*/
			
			//using loader_pl_concept = loader_pl_concept;
			//using loader_tr_concept = loader_tr_concept;
			
			std::unordered_map<std::string, loader_tr_concept<T_tr_index_frame>*> f_tr_map;
			std::unordered_map<std::string, loader_pl_concept<T_tr_index_frame>*> f_pl_map;
			
			std::string prefix_shared = "x_";
			
			using namespace std;
			
			string line;
			string name_f;
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
						if(iss >> token){
							name_f=token;
						}
					} else if(token == ".dummy"){
						while(iss >> token){
							
							cout<<"found transition: "<<token<<endl; //debug
							
							//create a new concept struct and add it to the map
							//map is there as a more efficient array of concept structs
							f_tr_map.insert({token, new loader_tr_concept<T_tr_index_frame>{token,{},{}}});
							
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
							loader_pl_concept<T_tr_index_frame>* current_place_p = new loader_pl_concept<T_tr_index_frame>;
							f_pl_map.insert({token, current_place_p});
							
							//old label is current name
							current_place_p->old_label = token;
							
							int s_start = token.find(prefix_shared,0);
							int s_end = prefix_shared.length();
							
							//if there exists a special prefix for shared memory
							//then there may exist a valid substitution
							if(s_start==0){
								
								current_place_p->special = 1;	//set concept to special prefix
								//grab the label set name (between prefix and the next underscore)
								s_start = s_end;
								s_end = token.find("_", s_start);
								std::string label_set_name = token.substr(s_start,s_end-s_start);
								current_place_p->label_set = label_set_name;	//set concept label set
								
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
										//set concept label set to new substitution
										current_place_p->label_set = pattern_it.sub;
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
								f_pl_map.at(token)->value = 1;
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
							loader_tr_concept<T_tr_index_frame>* tr = f_tr_map.at(token);
							loader_pl_concept<T_tr_index_frame>* pl;
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
							loader_tr_concept<T_tr_index_frame>* tr;
							loader_pl_concept<T_tr_index_frame>* pl = f_pl_map.at(token);
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
			
			vector<loader_pl_concept<T_tr_index_frame>*> out_pl_concepts;
			vector<loader_tr_concept<T_tr_index_frame>*> out_tr_concepts;
			
			for(auto&i : f_pl_map){
				out_pl_concepts.push_back(i.second);
			}
			
			for(auto&i : f_tr_map){
				out_tr_concepts.push_back(i.second);
			}
			
			//pass concepts to the concept compiler
			return compile(out_pl_concepts, out_tr_concepts, name_f);
			
		}
		
		//cpp nonsense -- workaround for cpp definition not in .h
		void dummy_link()
		{
			Graph<PL_frame<int,int>,TR_index_frame<int,int>> d1;
			d1.add("d",std::vector<pattern>{});
		}

	}
}
