#include <iostream>
#include <variant>
#include <vector>
#include <utility>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <tuple>
#include <string>
#include <mutex>
#include <fstream>
#include <sstream>
#include <atomic>

#ifndef GRAPHEX_H
#define GRAPHEX_H

#ifdef DEBUG_BUILD_1
#define D1(x) x
#else 
#define D1(x)
#endif

#ifdef DEBUG_BUILD_2
#define D2(x) x
#else 
#define D2(x)
#endif

#ifdef DEBUG_BUILD_3
#define D3(x) x
#else 
#define D3(x)
#endif

#ifdef DEBUG_BUILD_4
#define D4(x) x
#else 
#define D4(x)
#endif

namespace graphex{
	
	enum class e_r{
		TRUE,
		FALSE,
		SUCCESS,
		FAILURE,
		REDUNDANT,
		INPUT_ERROR
	};

	namespace gen_node{
	
		//---GENERIC NODE DATASTRUCTURES BEGIN---

		//enum for (S)ystem (L)inks - GEX uses fixed notation
		//for executable networks, and label network
		enum class e_sl : uint8_t {
			//exe
			input,
			output,
			input_interface,
			output_interface,
			//label
			label,
			gen_label,
			conv_label,
			//identity
			name,			
			//k
			k_in,
			k_out,
			k_in_func,
			k_out_func,
			k_in_data,
			k_out_data,
			//loader
			loader_in,
			loader_out,
			//list
			list_up,
			list_down,
			//sys
			nc		//no connection
		};

		enum class e_l : uint8_t {
			rack,
			interface,
			func_list,
			data_list,
			exe_list
		};

		//forward declarations
		class gn_area;

		//base struct of generic node, used such that connection
		//operations are independent of data payload type
		struct gn_base{
			std::vector<std::pair<gn_base*,e_sl>> con;
		};

		//data can be templated to any type (although small preferred)
		template <class T_payload>
		struct gn_data : public gn_base{
				T_payload data;
				
				gn_data(){
					data = {};
				}
		};

		//use of function pointers on presumed type means that the
		//linking process must contain logic to ensure that
		//func nodes pull from the correct data (type) nodes
		struct gn_func : public gn_base{
			//function pointer here...	
			e_r (*func)(gn_base* ctx, gn_area* area);
		};

		//---GENERIC NODE OPERATIONS BEGIN---
		//operation link: given a to b, link from a to b with x
		//and link b to a with y. If nc given, omit link - 
		//therefore single link
		e_r o_link(gn_base* a, gn_base* b, e_sl x, e_sl y);
	
		//operation unlink: given a to b, remove all connections
		//between a and b	
		e_r o_unlink(gn_base* a, gn_base* b);

		//operation detele: given a, delete the node, including
		//connections in referenced nodes
		e_r o_delete(gn_base* a);

		//operation merge: given a to b, a adds all of b's 
		//connections and then deletes b
		e_r o_merge(gn_base* a, gn_base* b);
		
		//single linked label using term and system link b
		template <class T>
		e_r o_l_is(gn_base* a, e_sl b, T term){
			
			e_r res_l = e_r::SUCCESS;

			gn_data<T>* label = new gn_data<T>;
			label->data = term;

			res_l = o_link(a, label, b, e_sl::nc);

			return res_l;
		}

		//operation search is: is node a labelled with connection b and label term?
		template <class T>
		e_r o_s_is(gn_base* a, e_sl b, T term){

			#define DEBUG_O_S_IS
			#ifdef DEBUG_O_S_IS
			#define D_O_S_IS(x) x
			#else
			#define D_O_S_IS(x)
			#endif

			using namespace std;

			e_r res_l = e_r::FAILURE;

			for(auto& i : a->con){
				D_O_S_IS(cout<<__func__<<": Searching... "<<i.first<<endl;)
				if(i.second == b){
					D_O_S_IS(cout<<"Found: "<< ((gn_data<T>*)i.first)->data<<endl;)
					D_O_S_IS(cout<<"For term: "<<term<<endl;)
					if(term == ((gn_data<T>*)i.first)->data){

						D_O_S_IS(cout<<__func__<<": Match! "<< ((gn_data<T>*)i.first)->data<<endl;)
						res_l = e_r::TRUE;
						return res_l;
					}
				}		
			}

			res_l = e_r::FALSE;

			return res_l;
		}

		e_r o_s_name(gn_base* a, std::string& name);
		
		//operation print list: Print entire list of nodes, including names	
		template <class T>
		e_r o_p_list(gn_base* a, bool is_data){
		
			using namespace std;

			e_r res_l = e_r::SUCCESS;

			decltype(a->con) a_a;

			string name = "None";
			string name_2 = "None";

			o_s_name(a, name);
			cout<<"Begin list print: "<<name<<" with pointer : "<<a<<endl;

			name = "None";

			for(auto& i : a->con){
				if(i.second == e_sl::list_down){
				//	for(auto& k : i.first->con){
				//		if(k.second == e_sl::name){
				//			name = ((gn_data<string>*)k.first)->data;
				//		}
				//		a_a.push_back(k);
				//	}
					
					o_s_name(i.first, name);

					cout<<"----------------------------------------"<<endl;
					cout<<"Node: "<<name<<" with pointer: "<<i.first;
					if(is_data){
						cout<<" value: "<<(int)((gn_data<T>*)i.first)->data<<endl;
					} else {
						cout<<endl;
					}
					
					for(auto& k : i.first->con){
						name_2 = "None";
						o_s_name(k.first, name_2);
						cout<<" +connection to... "<<(int)k.second<<" "<<name_2<<" with pointer: "<<k.first<<endl;
					}
						
					cout<<"----------------------------------------"<<endl;
				}
			}

			return res_l;
		}


		class gn_area{
			public:
				gn_area();
				~gn_area();

				virtual e_r execute();

				gn_data<gn_area*>* gn_reg_data;
				gn_data<gn_area*>* gn_reg_func;

				gn_data<gn_area*>* gn_reg_exe;

				gn_base* gn_k_rack;

				std::mt19937 random_generator;

				bool set_exe_spread;

				std::recursive_mutex area_lock;

		};

		struct gn_area_link{

			bool change;
			std::recursive_mutex link_lock;
		};

	//	class gn_interface : public gn_area{
	//		public:
	//			gn_interface();
	//			~gn_interface();

	//			std::recursive_mutex area_lock;
	//	};

		class controller{
			public:
				controller();
				~controller();

				gn_data<controller*> reg_area;
				gn_data<controller*> reg_interface;

				size_t add_rack(gn_area*);


		};

		namespace conv_petrinet{

			//forward declarations
			class pn_area;

			enum class e_l : uint8_t {
				transition,
				place,
				interface
			};

			enum class e_e : uint8_t{
				ordered,
				random
			};
			
			e_r c_add_pn(gn_base* ctx, gn_area* area);

			e_r c_activate_pn(gn_base* ctx, gn_area* area);

			class pn_area : public gn_area{
				public:
					pn_area();
					~pn_area();

					e_r load(std::string path);

					e_e exe_mode;

					virtual e_r execute();	
			};

		}
	}


}



#endif
