#include <iostream>

#include "graphex.h"



int main() {

	using namespace std;

	using namespace graphex::gen_node::conv_petrinet;
	using namespace graphex::gen_node;
	using namespace graphex;

	//auto a = new gn_data<int>;
	//auto b = new gn_func;

	gn_area m_reg;

	int counter = 5;
	gn_data<int>* z_a;

	for(int i = 0; i < 10; i++){	
		z_a = new gn_data<int>;
		z_a->data = counter++;
		m_reg.gn_reg_data.push_back(z_a);
	}

	gn_func* z_b;
	z_b = new gn_func;
	z_b->func = c_add_pn;
	m_reg.gn_reg_func.push_back(z_b);

	for(auto& i : m_reg.gn_reg_data){
		o_link(z_b, i, e_sl::input, e_sl::output);
	}	

	z_a = new gn_data<int>;
	z_a->data = 0;
	m_reg.gn_reg_data.push_back(z_a);

	o_link(z_b, z_a, e_sl::output, e_sl::input);
	cout<<"Data nodes..."<<endl;

	z_b->func(z_b);

	o_unlink(z_b, z_a);

	
	for(auto&i : m_reg.gn_reg_data){
		//o_unlink(z_b, i);
		cout<<"Deleting: "<<i<<endl;
		o_delete(i);
	}
	

	//o_delete(z_b);

	for(auto& i : m_reg.gn_reg_data){
		cout<<"Node: "<<i<<" Data: "<<((gn_data<int>*)i)->data<<endl;
		for(auto& k : i->con){
			cout<<k.first<<" + "<<(int)(k.second)<<endl;
		}
	}
	
	cout<<"Function nodes..."<<endl;

	for(auto& i : m_reg.gn_reg_func){
		cout<<"Node: "<<i<<endl;
		for(auto& k : i->con){
			cout<<k.first<<" + "<<(int)(k.second)<<endl;
		}
	}
	
	cout<<"complete"<<endl;

}
