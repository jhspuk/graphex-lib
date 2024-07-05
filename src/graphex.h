#include <iostream>
#include <vector>
#include <string>

#ifndef GRAPHEX_H
#define GRAPHEX_H

namespace graphex{
	
	namespace Petrinet{
		
		//word associations: pl = place, tr = transition
		
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
		
		//***Place group --	That which holds information for a group of
		//					places.
		
		//place_header -- Metadata about the format of data stored
		struct PL_header_s{
			
			
		};
		
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
		
		//***Transition group --  That which holds place indexes for a
		//						  group of transitions.
		
		//transition header -- Metadata about the format of data stored
		struct TR_header_s{
			
			
		};
		
		template <class T_pl_frame, class T_tr_index_frame>
		struct TR_s{
			
			
		};
		
		class Graph{
			public:
				Graph();
				
				void add();
				void fuse();
				
				void watch();
				void prod();
				
				void run();
			private:
			
		};
		
	}
	
	
	
}//end namespace graphex


#endif
