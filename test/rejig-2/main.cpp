#include <iostream>

#include "graphex.h"



int main() {

	using namespace std;

	using namespace graphex::gen_node::conv_petrinet;
	using namespace graphex::gen_node;
	using namespace graphex;

	//auto a = new gn_data<int>;
	//auto b = new gn_func;

	pn_area m_reg;
	
	m_reg.load("multinet_format.lpn");

	//int counter = 5;
	//gn_data<int>* z_a;

	//for(int i = 0; i < 10; i++){	
		//z_a = new gn_data<int>;
		//z_a->data = counter++;
		//o_link(m_reg.gn_reg_data, z_a, e_sl::list_in, e_sl::list_out);
	//}

	//gn_func* z_b;
	//z_b = new gn_func;
	//z_b->func = c_activate_pn;
	////z_b->func = c_add_pn;
	//o_link(m_reg.gn_reg_func, z_b, e_sl::list_in, e_sl::list_out);

	//for(auto& i : m_reg.gn_reg_data->con){
		//o_link(z_b, i.first, e_sl::input, e_sl::output);
	//}	

	//z_a = new gn_data<int>;
	//z_a->data = 0;
	//o_link(m_reg.gn_reg_data, z_a, e_sl::list_in, e_sl::list_out);

	//o_link(z_b, z_a, e_sl::output, e_sl::input);
	//cout<<"Data nodes..."<<endl;

	////z_b->func(z_b, ((gn_area*)&m_reg));
	//z_b->func(z_b, &m_reg);

////	o_unlink(z_b, z_a);

	
	//for(auto&i : m_reg.gn_reg_data->con){
		////o_unlink(z_b, i);
		////cout<<"Deleting: "<<i<<endl;
		////o_delete(i);
		////o_merge(z_b, i.first);
	//}
	

	////o_delete(z_b);
	
	for(auto& i : m_reg.gn_reg_data->con){
		cout<<"Node: "<<i.first<<" Data: "<<(int)((gn_data<uint8_t>*)i.first)->data<<endl;
		for(auto& k : i.first->con){
			cout<<k.first<<" + "<<(int)(k.second)<<endl;
			if(o_s_is(k.first, e_sl::name, "p3") == e_r::TRUE){
				cout<<i.first<<" is p1!"<<endl;
			}
		//	if(k.second == e_sl::list_up){
		//		cout<<"list base pointer is... "<<((gn_data<gn_area*>*)k.first)->data<<endl;
		//	}
		}
	}
	
	cout<<"Function nodes..."<<endl;

	for(auto& i : m_reg.gn_reg_func->con){
		cout<<"Node: "<<i.first<<endl;
		for(auto& k : i.first->con){
			cout<<k.first<<" + "<<(int)(k.second)<<endl;
		}
	}
	
	cout<<"complete"<<endl;

}
