#include "graphex.h"
#include <iostream>

namespace graphex{

	namespace gen_node{
	
		e_r o_link(gn_base* a, gn_base* b, e_sl x, e_sl y){
	
			#define DEBUG_GN_OLINK
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
		
		//GN AREA CLASS START---
		gn_area::gn_area(){
			gn_reg_data = new gn_base;
			gn_reg_func = new gn_base;
			gn_reg_interface = new gn_base;
		}

		gn_area::~gn_area(){
			o_delete(gn_reg_data);	
			o_delete(gn_reg_func);	
			o_delete(gn_reg_interface);	
		}

		e_r gn_area::execute(){
			e_r res_l = e_r::SUCCESS;

			return res_l;
		}
		
		//GN AREA CLASS END---

		//GN INTERFACE CLASS START---
		
		gn_interface::gn_interface() : gn_area(){
				

		}

		gn_interface::~gn_interface(){


		}

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

				#define DEBUG_GN_OMERGE
				#ifdef DEBUG_GN_OMERGE
				#define D_GN_OMG(x) x
				#else
				#define D_GN_OMG(x)
				#endif
		
				using namespace std;
				using T_data = gn_data<uint8_t>;

				e_r res_l = e_r::SUCCESS;
			
				auto& a_a = ctx->con;

				decltype(ctx->con) input_l;
				decltype(ctx->con) output_l;
				decltype(ctx->con) input_interface_l;
				decltype(ctx->con) output_interface_l;

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


				bool exe_possible = 1;

				for(auto& i : input_l){
					if(((T_data*)(i.first))->data == 0){
						exe_possible = 0;
					}
				}

				//todo: put protection logic for interface block here...
				for(auto& i : input_interface_l){
					if(((T_data*)(i.first))->data == 0){
						exe_possible = 0;
					}
				}
			

				if(exe_possible == 1){

					for(auto& i : input_l){
						((T_data*)(i.first))->data--;
					}

					for(auto& i : input_interface_l){
						((T_data*)(i.first))->data--;
					}

					for(auto& i : output_l){
						//give each output one extra token, also add neighbour transitions
						//to exe queue
						((T_data*)i.first)->data++;
						for(auto& k : i.first->con){
							if(k.second == e_sl::input){
								//put pointer in exe queue...
							}
						}
					}

					for(auto& i : output_interface_l){
						((T_data*)i.first)->data++;
					}
				}

				return res_l;
			}


			pn_area::pn_area() : gn_area() {

			}
			pn_area::~pn_area(){

			}
			e_r pn_area::execute(){
				e_r res_l = e_r::SUCCESS;

				return res_l;

			}

		}
	}
}



