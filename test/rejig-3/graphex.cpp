#include "graphex.h"
#include <iostream>

namespace graphex{
	namespace gen_node{
	
		e_r o_link(gn_base* a, gn_base* b, e_sl x, e_sl y){
	
			//#define DEBUG_GN_OLINK
			#ifdef DEBUG_GN_OLINK
			#define D_GN_OL(x) x
			#else
			#define D_GN_OL(x)
			#endif
		
			using namespace std;

			e_r res_l = e_r::SUCCESS;

			D_GN_OL(cout<<__func__<<" Start: "<<a<<" + "<<b<<endl;)

			bool redundant = 0;

			//forward link
			if(x != e_sl::nc){

				auto& a_a = a->con;
				pair a_a_2 = {b,x};

				for(auto& i : a_a){
					if(i == a_a_2){
						res_l = e_r::REDUNDANT;
						redundant = 1;
						D_GN_OL(cout<<__func__<<" Redundant! "<<i.first<<endl;)
					}
				};

				if(!redundant)
				a_a.push_back(a_a_2);
			}

			redundant = 0;

			//backward link
			if(y != e_sl::nc){
				auto& a_b = b->con;
				pair a_b_2 = {a,y};

				for(auto& i : a_b){
					if(i == a_b_2){
						res_l = e_r::REDUNDANT;
						redundant = 1;
						D_GN_OL(cout<<__func__<<" Redundant! "<<i.first<<endl;)
					}
				};

				if(!redundant)
				a_b.push_back(a_b_2);
			}

			return res_l;
			
		}

		e_r o_unlink(gn_base* a, gn_base* b){

			#define DEBUG_GN_OUNLINK
			#ifdef DEBUG_GN_OUNLINK
			#define D_GN_OUL(x) x
			#else
			#define D_GN_OUL(x)
			#endif

			using namespace std;
			
			e_r res_l = e_r::SUCCESS;
			
			auto& a_a = a->con;
			auto& a_b = b->con;
			
			auto a_size = a_a.size();
			auto b_size = a_b.size();

			//if there exists b pointer in a's connections, erase
			//Note: regardless of system link type
			a_a.erase(
				std::remove_if(a_a.begin(), a_a.end(), 
						[&](const std::pair<gn_base*, e_sl>& i) {
						return i.first == b;  // Condition to match and remove
						}),
					a_a.end()
			);
			
			//if there exists a pointer in b's connections, erase
			//Note: regardless of system link type
			a_b.erase(
				std::remove_if(a_b.begin(), a_b.end(), 
						[&](const std::pair<gn_base*, e_sl>& i) {
						return i.first == a;  // Condition to match and remove
						}),
					a_b.end()
			);

			if(a_size == a_a.size() && b_size == a_b.size()){
				res_l = e_r::REDUNDANT;
			}
		
			return res_l;
		}

		e_r o_delete(gn_base* a){
			
			#define DEBUG_GN_ODELETE
			#ifdef DEBUG_GN_ODELETE
			#define D_GN_ODEL(x) x
			#else
			#define D_GN_ODEL(x)
			#endif
		
			using namespace std;

			e_r res_l = e_r::SUCCESS;
			
			auto& a_a = a->con;
			auto b = a_a;
			for(auto& i : b){
				o_unlink(a, i.first);
			}

			delete a;

			return res_l;
		}

		e_r o_merge(gn_base* a, gn_base* b){
			
			#define DEBUG_GN_OMERGE
			#ifdef DEBUG_GN_OMERGE
			#define D_GN_OMG(x) x
			#else
			#define D_GN_OMG(x)
			#endif
		
			using namespace std;

			e_r res_l = e_r::SUCCESS;
			
			auto& a_a = a->con;
			auto& a_b = b->con;

			decltype(a->con) a_temp;
			decltype(a->con) b_temp;

			for(auto& i : a_b){
				if(i.second == e_sl::input
					|| i.second == e_sl::output
					|| i.second == e_sl::input_interface
					|| i.second == e_sl::output_interface)
				{
					a_temp.push_back(i);
				} else {
					o_unlink(b, i.first);
				}
			}

			for(auto& i : a_temp){
				for(auto& k : i.first->con){
					if(k.first == b){
						k.first = a;
					}	
				}
			}

			a_a.insert(a_a.end(), a_temp.begin(), a_temp.end());

			o_delete(b);

			return res_l;

		}
		
		e_r o_s_name(gn_base* a, std::string& name){

			//#define DEBUG_O_S_NAME
			#ifdef DEBUG_O_S_NAME
			#define D_O_S_N(x) x
			#else
			#define D_O_S_N(x)
			#endif

			using namespace std;

			e_r res_l = e_r::FAILURE;
			D_O_S_N(cout<<__func__<<" begin: "<<a<<endl;)
			for(auto& i : a->con){
				
				D_O_S_N(cout<<__func__<<" "<<a->con.size()<<" : "<<i.first<<endl;)
				
				if(i.second == e_sl::name){
					res_l = e_r::SUCCESS;
					name = ((gn_data<string>*)i.first)->data;
				}
			}

			return res_l;

		}

		
		
		//GN AREA CLASS START---
		gn_area::gn_area() : random_generator(std::random_device{}()){
			gn_reg_data = new gn_data<gn_area*>;
			o_l_is(gn_reg_data, e_sl::label, e_l::data_list);
			gn_reg_func = new gn_data<gn_area*>;
			o_l_is(gn_reg_func, e_sl::label, e_l::func_list);
			gn_reg_exe = new gn_data<gn_area*>;
			o_l_is(gn_reg_func, e_sl::label, e_l::exe_list);


			gn_reg_data->data = this;
			gn_reg_func->data = this;

			gn_k_rack = new gn_base;

			is_complete = 0;
		}

		gn_area::~gn_area(){
			o_delete(gn_reg_data);	
			o_delete(gn_reg_func);
			o_delete(gn_reg_exe);
			o_delete(gn_k_rack);	
		}

		e_r gn_area::execute(){
			e_r res_l = e_r::SUCCESS;

			return res_l;
		}
		
		//GN AREA CLASS END---

	//	//GN INTERFACE CLASS START---
	//	
	//	gn_interface::gn_interface() : gn_area(){
	//			

	//	}

	//	gn_interface::~gn_interface(){


	//	}

		//GN INTERFACE CLASS END---
		namespace conv_petrinet{

			e_r c_add_pn(gn_base* ctx, gn_area*){

				using T_data = gn_data<int>;
				
				e_r res_l = e_r::SUCCESS;
				
				T_data* output;

				int acc = 0;

				for(auto& i: ctx->con){
					if(i.second == e_sl::output){
						output = (T_data*)(i.first);
					}else if(i.second == e_sl::input){
						acc += ((T_data*)(i.first))->data;
					}
				}

				output->data = acc;

				return res_l;
			} 

			e_r c_activate_pn(gn_base* ctx, gn_area*){

				//define DEBUG_C_ACT_PN
				#ifdef DEBUG_C_ACT_PN
				#define D_C_ACT(x) x
				#else
				#define D_C_ACT(x)
				#endif
		
				using namespace std;
				using T_data = gn_data<uint8_t>;

				e_r res_l = e_r::SUCCESS;
			
				auto& a_a = ctx->con;

				decltype(ctx->con) input_l;
				decltype(ctx->con) output_l;
				decltype(ctx->con) input_interface_l;
				decltype(ctx->con) output_interface_l;

				D_C_ACT(cout<<__func__<<": begin..."<<endl;)

				//acquisition
				for(auto& i : a_a){
					if(i.second == e_sl::input){
						input_l.push_back(i);	
					}else if(i.second == e_sl::output){
						output_l.push_back(i);
					}else if(i.second == e_sl::input_interface){
						input_interface_l.push_back(i);	
					}else if(i.second == e_sl::output_interface){
						output_interface_l.push_back(i);
					}
				}
				
				D_C_ACT(cout<<__func__<<": acquired "<<input_l.size()<<" : "<<output_l.size()<<" : "<<input_interface_l.size()<<" : "<<output_interface_l.size()<<endl;)

				bool exe_possible = 1;

				vector<gn_area*> interface_area_l;

				for(auto& i : input_l){
					if(((T_data*)(i.first))->data == 0){
						exe_possible = 0;
					}
				}

				//todo: put protection logic for interface block here...
				for(auto& i : input_interface_l){
					for(auto& k : i.first->con){
						if(k.second == e_sl::list_up){

							auto area_l = ((gn_data<gn_area*>*)k.first)->data;
							interface_area_l.push_back(area_l);
							area_l->area_lock.lock();

							for(auto& j : area_l->gn_k_rack->con){
								if(j.second == e_sl::k_rack_exe){
									auto area_link_l = ((gn_data<gn_area_link*>*)j.first)->data;
									area_link_l->link_lock.lock();
									area_link_l->change = 1;
									area_link_l->link_lock.unlock();
								}
							}
						}
					}
					if(((T_data*)(i.first))->data == 0){
						exe_possible = 0;
					}
				}
			

				if(exe_possible == 1){

					for(auto& i : input_l){

						((T_data*)(i.first))->data--;

						for(auto& k : i.first->con){
							if(k.second == e_sl::output){
								for(auto& j : k.first->con){
									if(j.second == e_sl::list_up){
										o_link(((gn_data<gn_area*>*)j.first)->data->gn_reg_exe, k.first, e_sl::list_down, e_sl::nc);
										//((gn_data<gn_area*>*)j.first)->data->gn_reg_exe->con.push_back({k.first, e_sl::list_down}); //.push_back(i.first);
									}
								}
							}
						}
					}

					for(auto& i : input_interface_l){
						((T_data*)(i.first))->data--;

						for(auto& k : i.first->con){
							if(k.second == e_sl::output){
								for(auto& j : k.first->con){
									if(j.second == e_sl::list_up){
										auto& k_rack_l = ((gn_data<gn_area*>*)j.first)->data->gn_k_rack;
										for(auto& v : k_rack_l->con){

										}
									}
								}
							}
						}
					}

					for(auto& i : output_l){
						//give each output one token, also add neighbour transitions
						//to exe queue
						((T_data*)i.first)->data++;

						for(auto& k : i.first->con){
							if(k.second == e_sl::output){
								for(auto& j : k.first->con){
									if(j.second == e_sl::list_up){
										o_link(((gn_data<gn_area*>*)j.first)->data->gn_reg_exe, k.first, e_sl::list_down, e_sl::nc);
									}
								}
							}
						}
					}

					for(auto& i : output_interface_l){
						((T_data*)i.first)->data++;

						for(auto& k : i.first->con){
							if(k.second == e_sl::output){
								for(auto& j : k.first->con){
									if(j.second == e_sl::list_up){
										o_link(((gn_data<gn_area*>*)j.first)->data->gn_reg_exe, k.first, e_sl::list_down, e_sl::nc);
									}
								}
							}
						}
					}

					for(auto& i : interface_area_l){
						i->area_lock.unlock();
					}
				}

				return res_l;
			}


			pn_area::pn_area() : gn_area() {

			}
			pn_area::~pn_area(){

			}
			e_r pn_area::execute(){
				
				using namespace std;

				e_r res_l = e_r::SUCCESS;

				auto reg_exe_buffer = gn_reg_exe->con;

				//if there has been change, add all implicated transitions (pre calculated) from link
				//into exe buffer
				for(auto& i : gn_k_rack->con){
					if(i.second == e_sl::k_rack_int){
						auto area_link_l = ((gn_data<gn_area_link*>*)i.first)->data;
						if(area_link_l->change == 1){
							for(auto& j : area_link_l->gn_reg_func->con){
								if(j.second == e_sl::list_down){
									reg_exe_buffer.push_back({j.first, j.second});
								}
							}	
						}
						area_link_l->link_lock.lock();
						area_link_l->change = 0;
						area_link_l->link_lock.unlock();
					}
				}

				//if there is nothing in the buffer to be processed, quit immediately
				if(reg_exe_buffer.size()==0){

					is_complete = 1;

					res_l = e_r::REDUNDANT;
					return res_l;
				}

				gn_reg_exe->con.erase(gn_reg_exe->con.begin() + 1, gn_reg_exe->con.end());

				if(exe_mode == e_e::ordered){

					for(auto& i : reg_exe_buffer){
						if(i.second == e_sl::list_down){
							o_unlink(gn_reg_exe, i.first);
							((gn_func*)i.first)->func(i.first, (gn_area*)this);
						}
					}

				} else if(exe_mode == e_e::random){
					shuffle(reg_exe_buffer.begin(), reg_exe_buffer.end(), random_generator);
					for(auto& i : reg_exe_buffer){
						if(i.second == e_sl::list_down){
							o_unlink(gn_reg_exe, i.first);
							((gn_func*)i.first)->func(i.first, (gn_area*)this);
						}
					}
				}
				

				////now that pending calculations have been processed, tell all links that there is no change
				//for(auto& i : gn_k_rack->con){
				//	if(i.second == e_sl::k_rack_int){
				//		auto area_link_l = ((gn_data<gn_area_link*>*)i.first)->data;
				//		area_link_l->link_lock.lock();
				//		area_link_l->change = 0;
				//		area_link_l->link_lock.unlock();
				//	}
				//}


				

				return res_l;

			}
			e_r pn_area::load(std::string path){
				
				#define DEBUG_PN_AREA_LOAD
				#ifdef DEBUG_PN_AREA_LOAD
				#define D_PN_AL(x) x
				#else
				#define D_PN_AL(x)
				#endif
				
				using namespace std;
				
				e_r res_l = e_r::SUCCESS;
				
				ifstream file(path);
			
				if(!file.is_open()){
					cerr<<__func__<<": Cannot open file: "<<path<<endl;
					return e_r::FAILURE;
				}
				
				unordered_map<string, gn_func*> f_tr_map;
				unordered_map<string, gn_data<uint8_t>*> f_pl_map;
				
				string line;
				string name_f;
				
				bool state_initial = 1;
				bool state_body_1 = 0;
				bool state_body_2 = 0;
				bool state_wait_1 = 0;
				bool state_con_1 = 0;
				bool state_finish = 0;
				
				while(getline(file,line)){
					
					istringstream iss(line);
					string token;
					iss >> token;
					
					if(state_initial){
						
						if(token == ".name"){
							if(iss >> token){
								name_f = token;
							}
						} else if(token == ".dummy"){
								while(iss >> token){
									//transition strings are inside dummy block, read all of them into a map
									D_PN_AL(cout<<__func__<<": state initial, transition found: "<<token<<endl;)

									auto temp_gn_func = new gn_func;

									temp_gn_func->func = c_activate_pn;

									//label node...
									o_l_is(temp_gn_func, e_sl::conv_label, e_l::transition);
									o_l_is(temp_gn_func, e_sl::name, token);
									//add to rack function list...
									o_link(temp_gn_func, gn_reg_func, e_sl::list_up, e_sl::list_down);

									f_tr_map.insert({token,temp_gn_func});

									D_PN_AL(cout<<__func__<<": function node created: "<<f_tr_map.at(token)<<endl;)
								}
						} else if(token == ".graph"){
							state_body_1 = 1;
							state_initial = 0;
						}
						
					} else if(state_body_1){
						
						//current line is the start of .graph section
						if(token.rfind(".",0)==0){
							state_body_1 = 0;
							state_body_2 = 1;
							//go back to the beginning of the file for another pass
							file.seekg(0);
						} else {
							//only read lines that start with a new place string
							if(f_tr_map.find(token) == f_tr_map.end()){
								D_PN_AL(cout<<__func__<<": state body, place found: "<<token<<endl;)

								auto temp_gn_data = new gn_data<uint8_t>;

								//label node...
								o_l_is(temp_gn_data, e_sl::conv_label, e_l::place);
								o_l_is(temp_gn_data, e_sl::name, token);
								//initialise to zero
								temp_gn_data->data = 0;

								//add to rack data list
								o_link(temp_gn_data, gn_reg_data, e_sl::list_up, e_sl::list_down);
								
								f_pl_map.insert({token, temp_gn_data});
								D_PN_AL(cout<<__func__<<": data node created: "<<f_pl_map.at(token)<<endl;)
								
							}
						}
						
					} else if(state_body_2){
						if(token == ".marking"){
							uint8_t capacity = 0;
							while(iss >> token){
								token.erase(std::remove(token.begin(), token.end(), '{'), token.end());
								token.erase(std::remove(token.begin(), token.end(), '}'), token.end());
								
								int s_pos = token.find("=", 0);
								
								if(s_pos != -1){
									capacity = (uint8_t)stoi(token.substr(s_pos + 1, token.length()));
									D_PN_AL(cout<<__func__<<": long capacity found: "<<token.substr(0, s_pos)<<" : "<<(int)capacity<<endl;)
									try{
										f_pl_map.at(token.substr(0, s_pos))->data = capacity;
									}catch(const out_of_range& e){
										D_PN_AL(cout<<__func__<<": out of range error in marking."<<endl;)
									}
								} else {
									capacity = 1;
									D_PN_AL(cout<<__func__<<": short capacity found: "<<token<<endl;)
									try{
										f_pl_map.at(token)->data = capacity;
									}catch(const out_of_range& e){
										D_PN_AL(cout<<__func__<<": out of range error in marking."<<endl;)
									}
								}
							}
						} else if(token == ".end"){
							state_body_2 = 0;
							state_wait_1 = 1;
							//go back to the beginning of the file for another pass
							file.seekg(0);
						}
						
					} else if(state_wait_1){
						if(token == ".graph"){
							state_wait_1 = 0;
							state_con_1 = 1;
						}
						
					} else if(state_con_1){
						
						if(f_tr_map.find(token) != f_tr_map.end()){
							gn_func* temp_gn_func = f_tr_map.at(token);
							
							while(iss >> token){
								//cout<<temp_gn_func<< " : "<< f_pl_map.at(token)<<endl;
								o_link(temp_gn_func, f_pl_map.at(token), e_sl::output, e_sl::input);
							}
						} else if(f_pl_map.find(token) != f_pl_map.end()){
							gn_data<uint8_t>* temp_gn_data = f_pl_map.at(token);
							
							while(iss >> token){
								o_link(temp_gn_data, f_tr_map.at(token), e_sl::output, e_sl::input);
							}
						} else {
							state_con_1 = 0;
							state_finish = 1;
						}
						
					}
						
				}
			
				if(!state_finish){
					D_PN_AL(cerr<<__func__<<": error, unexpected exit!"<<path<<endl;)
				}
				
				D_PN_AL(cout<<__func__<<": finished reading file: "<<path<<endl;)
				
				return res_l;
				
				
			}

		}
	}
}



