#include <iostream>
#include <vector>
#include <string>
#include <mutex>

#ifndef GRAPHEX_H
#define GRAPHEX_H

namespace graphex{
	
	namespace Petrinet{
		
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
			
			std::recursive_mutex pl_lock;
			bool protection;
			
			std::vector<PL_TR_link<T_pl_frame, T_tr_index_frame>*> link_reg;
			
		};
		
		//******Place group END
		
		//*******Transition group --  That which holds place indexes for
		//						  a group of transitions.
		
		//transition -- holds an array of index_frames and labels
		template <class T_tr_index_frame>
		struct TR_s{
			std::vector<T_tr_index_frame> data;
			std::vector<std::string> labels;
		};
		
		//transition header -- Metadata about the format of data stored
		template <class T_pl_frame, class T_tr_index_frame>
		struct TR_header_s{
			TR_s<T_tr_index_frame>* body;
			
			//registry of place groups, pairing index values to pointers
			std::vector<PL_header_s<T_pl_frame, T_tr_index_frame>*> place_reg;
			
			//
			
		};
		
		//*******Transition group END
		
		//*******Place-Transition link -- Where a transition group has
		//a connection to a place group
		
		template <class T_pl_frame, class T_tr_index_frame>
		struct PL_TR_link{
			TR_header_s<T_pl_frame, T_tr_index_frame>* tr_p;
			PL_header_s<T_pl_frame, T_tr_index_frame>* pl_p;
			
			int change;
		};
		
		//*******Place-Transition link END
		
		class Graph{
			public:
				Graph();
				
				void add();
				void fuse();
				
				//methods to read PN as vars, or spur a transition
				void watch();
				void prod();
				
				
				
				void run();
			private:
			
		};
		
	}
	
	
	
}//end namespace graphex


#endif
