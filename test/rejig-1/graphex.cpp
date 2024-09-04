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
				pair a_b_2 = {b,y};

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

		e_r o_merge(gn_base* a, gn_base* b){
			#define DEBUG_GN_OMERGE
			#ifdef DEBUG_GN_OMERGE
			#define D_GN_MG(x) x
			#else
			#define D_GN_MG(x)
			#endif
		
			using namespace std;

			e_r res_l = e_r::SUCCESS;
			
			auto& a_b = b->con;

			for(auto& i : a_b){

			}


		}

		gn_area::gn_area(){

		}

		gn_area::~gn_area(){
			
		}

		e_r gn_area::execute(){

		}


		namespace conv_petrinet{

			void c_add_pn(gn_base* ctx){

				using T_data = gn_data<int>;
				std::cout<<"hello from add"<<std::endl;
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

				return;
			} 




			pn_area::pn_area() : gn_area() {

			}
			pn_area::~pn_area(){

			}
			e_r pn_area::execute(){
				std::cout<<"Execute!"<<std::endl;
			}

		}
	}
}



