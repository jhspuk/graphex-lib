#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <iterator>
#include <type_traits>
#include <algorithm>
#include <random>
#include <ctime>

#ifndef GRAPHEX_H
#define GRAPHEX_H

#define DEBUG_BUILD

#ifdef DEBUG_BUILD 
#define D(x) x
#else 
#define D(x)
#endif

namespace graphex{
	
	namespace petrinet{
		
		//******ENUMS
		enum class Exe_mode{
			Random,
			Sequence
		};
		
		enum class GS_vars{
			place,
			bit_1,
			byte_1,
			byte_2,
			byte_4,
			byte_8
		};
		
		//******ENUMS END
		
		//word associations: pl = place, tr = transition
		
		//******Small compositional structs
		
		//place_frame -- What kind of token and its current value
		template<class T_pl_type, class T_pl_data>
		struct PL_frame{
			T_pl_type place_type;
			T_pl_data place_data;
		};
		
		//index_frame -- How to find a specific place
		template<class T_group_index, class T_pl_index>
		struct TR_index_frame{
			T_group_index group_index;
			T_pl_index place_index;
		};
		
		struct GS_index_frame{
			int group_index;
			int place_index;
		};
		
		//******Small compositional structs END
		
		//forward definition
		template <class T_pl_frame, class T_tr_index_frame>
		struct PL_TR_link;
		
		//******Place group --	That which holds information for a group
		//					of places.
		
		//place -- holds an array of place_frames, which each have data
		//		   associated (number of tokens, etc.)
		template <class T_pl_frame>
		struct PL_s{
			//main payload, note position of frame is used as index
			//for transitions to link back to - labels and pl_frames
			//must have coherent indexes
			std::vector<T_pl_frame> data;
			std::vector<std::string> labels;
		};
		
		//place_header -- Metadata about the format of data stored
		template <class T_pl_frame, class T_tr_index_frame>
		struct PL_header_s{
			PL_s<T_pl_frame>* body;
			
			//mutex/execution control, and also enabling flag
			std::recursive_mutex pl_lock;
			bool protection;
			
			//registry of place-transition links
			std::vector<PL_TR_link<T_pl_frame, T_tr_index_frame>*> link_reg;
			
			//internal graph facing index of pl_groups
			int index_graph;
			
			//meta description of place group
			std::vector<std::string> descriptor;
			
		};
		
		//******Place group END
		
		//******Transition group --  That which holds place indexes for
		//						  a group of transitions.
		
		//transition -- holds an array of index_frames and labels
		template <class T_tr_index_frame>
		struct TR_s{
			std::vector<std::vector<T_tr_index_frame>> data;
			std::vector<std::string> labels;
		};
		
		//transition header -- Metadata about the format of data stored
		template <class T_pl_frame, class T_tr_index_frame>
		struct TR_header_s{
			TR_s<T_tr_index_frame>* body;
			
			//registry of place groups, pairing index values to pointers
			std::vector<PL_header_s<T_pl_frame, T_tr_index_frame>*> place_reg;
			//registry of place-transition links
			std::vector<PL_TR_link<T_pl_frame, T_tr_index_frame>*> link_reg;
			
			//meta description of transition group
			std::vector<std::string> descriptor;
			
		};
		
		//******Transition group END
		
		//******Place-Transition link -- Where a transition group has
		//a connection to a place group
		
		template <class T_pl_frame, class T_tr_index_frame>
		struct PL_TR_link{
			TR_header_s<T_pl_frame, T_tr_index_frame>* tr_p;
			PL_header_s<T_pl_frame, T_tr_index_frame>* pl_p;
			
			bool change;
		};
		
		//******Place-Transition link END
		
		//******pattern -- specify a label set, its substitution
		//							and the whitelist of PL groups it can be 
		//							searched for inside
		
		struct pattern{
			//name of label set
			std::string name;
			//substitution to be made
			std::string sub;
			//search scope of substitution
			std::vector<int> whitelist;
		};
		
		//******pattern END
		
		//******loader helpers -- abstract form that can be translated to
		//						an executable form
		template <class T_tr_index_frame>
		struct loader_pl_concept{
			std::string old_label;
			std::string new_label;
			std::string label_set;
			
			std::vector<int> whitelist;
			
			//note: 32bit integer used here since file format
			uint32_t value;
			
			//local index
			T_tr_index_frame index_l;
			
			bool exists;		//exists priorly in Graph
			bool special;		//has x_ prefix
			bool expressed;
			
		};
		
		template <class T_tr_index_frame>
		struct loader_tr_concept{
			std::string label;
			std::vector<loader_pl_concept<T_tr_index_frame>*> inputs;
			std::vector<loader_pl_concept<T_tr_index_frame>*> outputs;
			
		};
		
		//******loader helpers END
		
		//******GET AND SET -- struct to define bits of a variable
		//(assuming dual rail logic) such it can be either get or set
		
		struct GS_s{
			std::vector<GS_index_frame> lower_bits;
			std::vector<GS_index_frame> upper_bits;
		};
		
		struct GS_header_s{			
			GS_s* body;
			GS_vars type_l;
		};
		
		//******GET AND SET END
		

		
		template <class T_pl_frame, class T_tr_index_frame>
		class Graph{
			public:
				Graph();
				~Graph();
				
				int add(std::string path, std::vector<pattern> patterns);
				int add(std::string path, int);
				int add(std::string path);
				
				int find(std::string search_term, std::vector<int> whitelist, T_tr_index_frame&);
				void find_pg(int, std::vector<int>&);
				
				void print_pl();
				
				
				//methods to read PN as vars, or spur a transition
				int attach(std::vector<pattern> lower, std::vector<pattern> upper, GS_vars);
				int attach(pattern place);
				
				template<class T_in>
				void set(int, T_in);
				
				template<class T_ret>
				void get(int, T_ret&);
				
				void watch();
				void prod();
				
				int execute(Exe_mode mode_outer, Exe_mode mode_inner);
				
				int execute__base(int index, Exe_mode mode);
				std::vector<GS_header_s*> gs_reg;
				std::vector<TR_header_s<T_pl_frame, T_tr_index_frame>*> tr_reg;
			private:
			
				//add overloading base function
				int add__base(std::string path, std::vector<pattern> patterns);
				int compile(std::vector<loader_pl_concept<T_tr_index_frame>*>, 
							std::vector<loader_tr_concept<T_tr_index_frame>*>, std::string);
				
				
				std::vector<PL_header_s<T_pl_frame, T_tr_index_frame>*> pl_reg;
				
		};
		
	}
	
	
	
}//end namespace graphex

namespace graphex{
	namespace petrinet{
		template <class T_pl_frame, class T_tr_index_frame>
		Graph<T_pl_frame, T_tr_index_frame>::Graph(){
			
		}
		template <class T_pl_frame, class T_tr_index_frame>
		Graph<T_pl_frame, T_tr_index_frame>::~Graph(){
			for(auto&i:tr_reg){
				for(auto&k : i->link_reg){
					delete k;
				}
				delete i->body;
				delete i;
			}
			for(auto&i:pl_reg){
				delete i->body;
				delete i;
			}
			for(auto&i:gs_reg){
				delete i->body;
				delete i;
			}
		}
		
		
		template<class T_pl_frame, class T_tr_index_frame>
		void Graph<T_pl_frame, T_tr_index_frame>::print_pl(){
			using namespace std;
			cout<<"Begin print:"<<endl;
			int index_l_f = 0;
			for(auto&i : pl_reg){
				cout<<"------ Name of PL_group: "<<i->descriptor[0]<<", "<<index_l_f<<", Protection: "<<i->protection<<", Ex Index: "<< ", Locked? "<<!(i->pl_lock.try_lock())<<endl;
				
				int counter = 0;
				for(auto&k : i->body->data){
					cout<<"Label: "<<i->body->labels[counter]<<" Value: "<<k.place_type<<":"<<k.place_data<<endl;
					counter++;
				}
			index_l_f++;
			}
			return;
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
						D(cout<<"SEARCH MATCH: "<<search_term<<endl;)
						T_tr_index_frame index_f = {mask,count};
						index_i_f = index_f;
						return mask;
					}
					count++;
				}
			}
			D(cout<<"SEARCH NOT FOUND: "<<search_term<<endl;)
			return -1;
		}
		
		template<class T_pl_frame, class T_tr_index_frame>
		void Graph<T_pl_frame, T_tr_index_frame>::find_pg(int tr_index_f, std::vector<int>& pg_result){
			pg_result.clear();
			for(auto&i : tr_reg[tr_index_f]->place_reg){
				pg_result.push_back(i->index_graph);
			}
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
			
			pl_group_f->index_graph = pl_index_f;
			
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
					D(cout<<i->old_label<<" matches to new "<<i->new_label<<endl;)
					i->index_l = search_result;
					i->exists=1;
					
					auto it = pl_groupings_exist.find(index_result);
					if(it == pl_groupings_exist.end()){
						pl_groupings_exist.insert({index_result,vector<loader_pl_concept<T_tr_index_frame>*>{i}});
					} else {
						it->second.push_back(i);
					}
					
				} else if(i->special != 1) {
					//case two: the place is not special, therefore is the central case
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
							//initialise empty
							current_place_p->value = 0;
							
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
										//888
										cout<<"Valid Substitution: "<< pattern_it.name<< " , "<< token << endl;
										
										for(auto&v : pattern_it.whitelist){
											cout<<v<<endl;
										}
										//888
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
							
							//check for end bracket "}" and remove
							s_start = token.find("}", s_start);
							if(s_start!=-1){
								token.erase(s_start,1);
							}
							
							if(s_end != -1){
								

								
								//use position of equals to find number, then convert to integer
								cout<<token.substr(s_end+1, token.length())<<" : "<<s_end<<endl; //debug
								capacity = stoi(token.substr(s_end+1, token.length()));
								
								cout<<"normals: "<<token.substr(0, s_end)<<endl; //debug
								//find relevant place and set capacity
								f_pl_map.at(token.substr(0, s_end))->value = capacity;
								
							} else {
								//implicit '1' assumed, set concept capacity to 1
								try{
									f_pl_map.at(token)->value = 1;
								}catch(const out_of_range& e){
									cerr<<__func__<<" Error - token capacity check out of range, token: "<<token<<endl;
								}
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
								tr->outputs.push_back(pl);
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
									tr->inputs.push_back(pl);
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
		
		template <class T_pl_frame, class T_tr_index_frame>
		int Graph<T_pl_frame, T_tr_index_frame>::execute(Exe_mode mode_outer, Exe_mode mode_inner){
			switch(mode_outer){
				case Exe_mode::Sequence:{
					for(size_t i = 0; i < tr_reg.size(); i++){
						execute__base(i, mode_inner);
					}
					break;
				}
				case Exe_mode::Random:{
					auto rng = std::default_random_engine {static_cast<unsigned long>(time(nullptr))};
					std::uniform_int_distribution<int> gen(0, tr_reg.size());
						execute__base(gen(rng), mode_inner);
				}
				default:
					break;
			}
		}
		
		template <class T_pl_frame, class T_tr_index_frame>
		int Graph<T_pl_frame, T_tr_index_frame>::execute__base(int index_graph_tr_l, Exe_mode mode){
			
			using namespace std;
			
			TR_header_s<T_pl_frame, T_tr_index_frame>* tr_group_f = tr_reg[index_graph_tr_l];
			auto data = tr_group_f->body->data;
			
			vector<PL_header_s<T_pl_frame, T_tr_index_frame>*> pl_reg_l_f;
			
			vector<int> exe_list; //list of indexes that can be executed
			int counter = 0;
			
			vector<T_tr_index_frame> iv;	//input vector
			bool out_flag = false;			//interleaving process flag
			for(auto&iv : data){
				if(!out_flag){
					int zero_tokens = 0;
					for (auto&i : iv){
						
						//ascertain place group from group_index internal
						//value
						auto pl_group_f = tr_group_f->place_reg[i.group_index];
						
						if(pl_group_f->protection == 1){
							//pushback pointer for mass unlock after
							pl_reg_l_f.push_back(pl_group_f);
							//lock mutex
							pl_group_f->pl_lock.lock();
						}
						
						if(pl_group_f->body->data[i.place_index].place_type == 0 && pl_group_f->body->data[i.place_index].place_data < 1){
							//if normal place type and has less than 1 token, register this outcome
							zero_tokens++;
						}
						
						//can add more patterns here, with different types etc.
					}

					if(zero_tokens == 0){
						//register an execution, there was minimum 1 token
						//in all input places
						exe_list.push_back(counter);
						if(mode == Exe_mode::Sequence){
							break;
						}
					}
					
				}
				
				counter++;
				out_flag = !out_flag;

			}
			//random element to shuffle execution stack
			
			
			switch(mode){
				case Exe_mode::Random:{
					auto rng = std::default_random_engine {static_cast<unsigned long>(time(nullptr))};
					shuffle(exe_list.begin(),exe_list.end(),rng);
					break;
				}
				default:
					break;
			}
			switch(mode){
				case Exe_mode::Sequence:
				case Exe_mode::Random:
					for(auto&i : exe_list){
						auto iv = data[i];
						auto ov = data[i+1];
						//execute transition, start with output vector
						//adding one to each
						for(auto& k : ov){
							auto pl_group_f = tr_group_f->place_reg[k.group_index];
							if(pl_group_f->protection == 1){
								//pushback pointer for mass unlock after
								pl_reg_l_f.push_back(pl_group_f);
								//lock mutex
								pl_group_f->pl_lock.lock();
							}
							
							//for (auto& j : pl_group_f->link_reg){
								////indicate change
								//j->change = 1;
							//}
							
							if(pl_group_f->body->data[k.place_index].place_type == 0){
								pl_group_f->body->data[k.place_index].place_data++;
							}
						}
						
						for(auto& k : iv){
							auto pl_group_f = tr_group_f->place_reg[k.group_index];
							
							if(pl_group_f->body->data[k.place_index].place_type == 0){
								pl_group_f->body->data[k.place_index].place_data--;
							}
						}
						break;	//only one execution per list!
					}
				default:
					break;
			}

			for(auto&i : pl_reg_l_f){
				i->pl_lock.unlock();
			}
			return 1;
		}
		
		template<class T_pl_frame, class T_tr_index_frame>
		int Graph<T_pl_frame, T_tr_index_frame>::attach(std::vector<pattern> lower_bits_f, std::vector<pattern> upper_bits_f, GS_vars var_type){
			using namespace std;
			T_tr_index_frame tr_index_temp_f;
			GS_index_frame gs_index_temp_f;
			
			GS_header_s* gs_header_f = new GS_header_s;
			gs_header_f->body = new GS_s;
			gs_header_f->type_l = var_type;
			int gs_index_f = 0;
			
			gs_reg.push_back(gs_header_f);
			gs_index_f = gs_reg.size() - 1;
			
			for(auto&i : lower_bits_f){
				int result = find(i.name, i.whitelist, tr_index_temp_f);
				if(result >= 0){
					gs_index_temp_f.group_index = result;
					gs_index_temp_f.place_index = static_cast<int>(tr_index_temp_f.place_index);
					gs_header_f->body->lower_bits.push_back(gs_index_temp_f);
				} else {
					cerr<<__func__<<" - GS lower failed to acquire!"<<endl;
				}
			}
			
			for(auto&i : upper_bits_f){
				int result = find(i.name, i.whitelist, tr_index_temp_f);
				if(result >= 0){
					gs_index_temp_f.group_index = result;
					gs_index_temp_f.place_index = static_cast<int>(tr_index_temp_f.place_index);
					
					gs_header_f->body->upper_bits.push_back(gs_index_temp_f);
				} else {
					cerr<<__func__<<" - GS higher failed to acquire!"<<endl;
				}
			}

			return gs_index_f;
		}
		
		template<class T_pl_frame, class T_tr_index_frame>
		int Graph<T_pl_frame, T_tr_index_frame>::attach(pattern place){
			using namespace std;
			T_tr_index_frame tr_index_temp_f;
			GS_index_frame gs_index_temp_f;
			
			GS_header_s* gs_header_f = new GS_header_s;
			gs_header_f->body = new GS_s;
			gs_header_f->type_l = GS_vars::place;
			int gs_index_f = 0;
			
			gs_reg.push_back(gs_header_f);
			gs_index_f = gs_reg.size() - 1;
			

			int result = find(place.name, place.whitelist, tr_index_temp_f);
			if(result >= 0){
				gs_index_temp_f.group_index = result;
				gs_index_temp_f.place_index = static_cast<int>(tr_index_temp_f.place_index);
				gs_header_f->body->lower_bits.push_back(gs_index_temp_f);
			} else {
				cerr<<__func__<<" - Place failed to acquire! "<<tr_index_temp_f.place_index<<endl;
				cerr<<result<<endl;
			}
			return gs_index_f;
		}
		
		template<class T_pl_frame, class T_tr_index_frame>
		template<class T_ret>
		void Graph<T_pl_frame, T_tr_index_frame>::get(int index_f, T_ret& var){
			
			using namespace std;
			
			auto GS_header_f = gs_reg[index_f];
			int counter = 1; var = 0;
			vector<PL_header_s<T_pl_frame, T_tr_index_frame>*> pl_reg_lv_f;
			
			switch(GS_header_f->type_l){
				case GS_vars::byte_1: {
					for(auto&i : GS_header_f->body->upper_bits){
						auto pl_group_f = pl_reg[i.group_index];
		
						if(pl_group_f->protection == 1){
							pl_reg_lv_f.push_back(pl_group_f);
							pl_group_f->pl_lock.lock();
						}
						
						if(pl_group_f->body->data[i.place_index].place_data > 0){
							var |= counter;
						}
						
						counter = counter << 1;
					}
					break;
				}
				
				//single place conventionally stored in lower bits
				case GS_vars::place: {
					auto i = GS_header_f->body->lower_bits[0];
					auto pl_group_f = pl_reg[i.group_index];
					
					if(pl_group_f->protection == 1){
						pl_reg_lv_f.push_back(pl_group_f);
						pl_group_f->pl_lock.lock();
					}
						
					var = static_cast<T_ret>(pl_group_f->body->data[i.place_index].place_data);
					break;
				}
				default:
					break;

			}
			
			for(auto&i : pl_reg_lv_f){
				i->pl_lock.unlock();
			}
		}
		
		template<class T_pl_frame, class T_tr_index_frame>
		template<class T_in>
		void Graph<T_pl_frame, T_tr_index_frame>::set(int index_f, T_in input){
			using namespace std;
			
			auto GS_header_f = gs_reg[index_f];
			T_in counter = 1;
			vector<PL_header_s<T_pl_frame, T_tr_index_frame>*> pl_reg_lv_f;
			
			switch(GS_header_f->type_l){
				case GS_vars::place: {
					auto i = GS_header_f->body->lower_bits[0];
					auto pl_group_f = pl_reg[i.group_index];
					
					if(pl_group_f->protection == 1){
						pl_reg_lv_f.push_back(pl_group_f);
						pl_group_f->pl_lock.lock();
					}
						
					pl_group_f->body->data[i.place_index].place_data = static_cast<decltype(T_pl_frame::place_data)>(input);
					break;
				}
				default: {
					//dual rail logic: assign upper bits
					for(auto&i : GS_header_f->body->upper_bits){
						auto pl_group_f = pl_reg[i.group_index];
		
						if(pl_group_f->protection == 1){
							pl_reg_lv_f.push_back(pl_group_f);
							pl_group_f->pl_lock.lock();
						}
						
						if(input & counter){
							pl_group_f->body->data[i.place_index].place_data = 1;
						} else {
							pl_group_f->body->data[i.place_index].place_data = 0;
						}
						
						counter = counter << 1;
					}
					
					//dual rail logic: assign lower bits
					counter = 1;
					for(auto&i : GS_header_f->body->lower_bits){
						auto pl_group_f = pl_reg[i.group_index];
		
						if(pl_group_f->protection == 1){
							pl_reg_lv_f.push_back(pl_group_f);
							pl_group_f->pl_lock.lock();
						}
						
						if(input & counter){
							pl_group_f->body->data[i.place_index].place_data = 0;
						} else {
							pl_group_f->body->data[i.place_index].place_data = 1;
						}
						
						counter = counter << 1;
					}
				break;
				}
				
			}
			
			for(auto&i : pl_reg_lv_f){
				i->pl_lock.unlock();
			}

		}
		
		
	}
}



#endif
