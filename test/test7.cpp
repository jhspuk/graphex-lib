#include <iostream>
#include <variant>
#include <vector>
#include <utility>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <tuple>

template <class T_payload, class T_connection>
struct gen_node {
	using T_self = gen_node<T_payload, T_connection>;
	
	T_payload node_payload;
	
	std::vector<gen_node<T_payload, T_connection>*> connections;
	std::vector<T_connection> arc_payload;
	
	std::vector<std::tuple<std::string,std::string>> tags;
	
};


int main(){
	
	using namespace std;
	using node = gen_node<int, int>;
	vector<node*> reg;
	vector<node*> exe;
	
	size_t temp[10];
	
	reg.push_back(new node);
	temp[0] = reg.size() - 1;
	
	reg[temp[0]]->tags.push_back({"source",""});
	reg[temp[0]]->node_payload = 1;
	exe.push_back(reg[temp[0]]);
	
	reg.push_back(new node);
	temp[1] = reg.size() - 1;
	
	reg[temp[1]]->tags.push_back({"source",""});
	reg[temp[1]]->node_payload = 2;
	exe.push_back(reg[temp[1]]);
	
	reg.push_back(new node);
	temp[2] = reg.size() - 1;
	
	reg[temp[2]]->tags.push_back({"source",""});
	reg[temp[2]]->node_payload = 3;
	exe.push_back(reg[temp[2]]);
	
	reg.push_back(new node);
	temp[3] = reg.size() - 1;
	
	reg[temp[3]]->tags.push_back({"source",""});
	reg[temp[3]]->node_payload = 4;
	exe.push_back(reg[temp[3]]);
	
	
	//adder one
	reg.push_back(new node);
	temp[4] = reg.size() - 1;
	
	reg[temp[4]]->tags.push_back({"add",""});
	reg[temp[4]]->node_payload = 0;
	
	reg[temp[4]]->connections.push_back(reg[temp[0]]);
	reg[temp[4]]->arc_payload.push_back(1);
	reg[temp[0]]->connections.push_back(reg[temp[4]]);
	reg[temp[0]]->arc_payload.push_back(0);
	
	reg[temp[4]]->connections.push_back(reg[temp[1]]);
	reg[temp[4]]->arc_payload.push_back(1);
	reg[temp[1]]->connections.push_back(reg[temp[4]]);
	reg[temp[1]]->arc_payload.push_back(0);
	
	//minus one
	reg.push_back(new node);
	temp[5] = reg.size() - 1;
	
	reg[temp[5]]->tags.push_back({"minus",""});
	reg[temp[5]]->node_payload = 0;
	
	reg[temp[5]]->connections.push_back(reg[temp[1]]);
	reg[temp[5]]->arc_payload.push_back(1);
	reg[temp[1]]->connections.push_back(reg[temp[5]]);
	reg[temp[1]]->arc_payload.push_back(0);
	
	reg[temp[5]]->connections.push_back(reg[temp[2]]);
	reg[temp[5]]->arc_payload.push_back(1);
	reg[temp[2]]->connections.push_back(reg[temp[5]]);
	reg[temp[2]]->arc_payload.push_back(0);
	
	//adder two
	
	reg.push_back(new node);
	temp[6] = reg.size() - 1;
	
	reg[temp[6]]->tags.push_back({"add",""});
	reg[temp[6]]->node_payload = 0;
	
	reg[temp[6]]->connections.push_back(reg[temp[4]]);
	reg[temp[6]]->arc_payload.push_back(1);
	reg[temp[4]]->connections.push_back(reg[temp[6]]);
	reg[temp[4]]->arc_payload.push_back(0);
	
	reg[temp[6]]->connections.push_back(reg[temp[5]]);
	reg[temp[6]]->arc_payload.push_back(1);
	reg[temp[5]]->connections.push_back(reg[temp[6]]);
	reg[temp[5]]->arc_payload.push_back(0);
	
	while(true){
		vector<node*> t_exe;
		for(auto&i : exe){
			int ty = 0;
			for(auto&k : i->tags){
				if(get<0>(k) == "source"){
					ty = 1;
					cout<<"tya == 1"<<endl;
				} else if(get<0>(k) == "add") {
					ty = 2;
					cout<<"tya == 2"<<endl;
				} else if(get<0>(k) == "minus"){
					ty = 3;
					cout<<"tya == 3"<<endl;
				};
			}
			
			if(ty == 1){
				int counter = 0;
				for(auto&k : i->arc_payload){
					if(k==0){
						cout<<"k == 0"<<endl;
						t_exe.push_back(i->connections[counter]);
					}
					counter++;
				}
				cout<<"ty == 1"<<endl;
			}else if(ty == 2){
				cout<<"ty == 2"<<endl;
			}else if(ty == 3){
				cout<<"ty == 3"<<endl;
			}
		}
		
		if(t_exe.size() == 0){break;}
		exe.clear();
		exe=t_exe;
		t_exe.clear();
	}
	
		
	return 0;
}
