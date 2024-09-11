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
					data = 0;
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


		class gn_area{
			public:
				gn_area();
				~gn_area();

				virtual e_r execute();

				gn_data<gn_area*>* gn_reg_data;
				gn_data<gn_area*>* gn_reg_func;

				std::vector<gn_func*> exe_list;

				gn_base* gn_reg_interface;

				bool set_exe_spread;

				std::recursive_mutex area_lock;

		};

	//	class gn_interface : public gn_area{
	//		public:
	//			gn_interface();
	//			~gn_interface();

	//			std::recursive_mutex area_lock;
	//	};

		namespace conv_petrinet{

			//forward declarations
			class pn_area;
			
			e_r c_add_pn(gn_base* ctx, gn_area* area);

			e_r c_activate_pn(gn_base* ctx, gn_area* area);

			class pn_area : public gn_area{
				public:
					pn_area();
					~pn_area();

					e_r load(std::string path);

					virtual e_r execute();	
			};

		}
	}


}



#endif
