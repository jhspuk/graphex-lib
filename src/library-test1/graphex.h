#include <iostream>
#include <vector>
#include <string>
#include <mutex>

#ifndef GRAPHEX_H
#define GRAPHEX_H

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
				void set(int, uint8_t);
				void get(int, uint8_t&);
				void watch();
				void prod();
								
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


#endif
