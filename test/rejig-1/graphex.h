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

namespace graphex {
	
	//------ENUMS begin
	enum class result{
		SUCCESS,
		PASS,
		REJECT,
		LOGIC_ERROR,
		FAILURE
	};
	//------ENUMS end
	
	//------NODES as types
	//generic node type
	template <class T_payload, class T_connection>
	struct gen_node {
		using T_self = gen_node<T_payload, T_connection>;
		
		T_payload node_payload;
		
		std::vector<std::tuple<T_self*, T_connection*>> connections;
		std::vector<std::tuple<std::string,std::string>> tags;
	};
	//------NODES end
	
	
	//------LOADER OBJECTS
	class base_loader{
		public:
			base_loader();
	};
	
	class PN_loader : public base_loader{
		public:
			PN_loader();
		
	};
	
	//------LOADER OBJECTS end
	
	//------PAINTER OBJECTS
	
	class base_painter{
		public:
			base_painter();
	};
	
	class PN_painter : public base_painter{
		public:
			PN_painter();
	};
	
	//------PAINTER OBJECTS end
	
	//------IMPLEMENTATION OBJECTS
	
	class base_implementation{
		public:
			base_implementation();
	}
	
	class PN_implementation : public base_implementation{
		public:
			PN_implementation();
	}
	
	//------IMPLEMENTATION OBJECTS end
	
	//------MAIN CONTROLLER OBJECT
	
	class controller{
		public:
		
		private:
			base_loader();
			base_painter();
			base_implementation();
	};
	
	//------MAIN CONTROLLER OBJECT end
	
};



#endif
