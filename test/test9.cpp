#include <iostream>
#include <variant>
#include <vector>
#include <utility>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <tuple>

namespace graphex{

	struct gen_node{
		std::vector<std::pair<gen_node, uint8_t>> connections;
	}
 
	struct gen_node_exe : public gen_node{
		void*

	}

	template <class T_payload>
	struct gen_node_data : public gen_node{


	}

}

int main(){





	return 0;
}
