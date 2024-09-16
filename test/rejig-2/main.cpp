#include <iostream>
#include <chrono>

#include "graphex.h"



int main() {

	using namespace std;

	using namespace graphex::gen_node::conv_petrinet;
	using namespace graphex::gen_node;
	using namespace graphex;

	//auto a = new gn_data<int>;
	//auto b = new gn_func;

	pn_area m_reg;
	
	//m_reg.load("tmCompletePetriNetDemo.lpn");
	m_reg.load("pipe_test.lpn");

	o_p_list<uint8_t>(m_reg.gn_reg_data, 1);
	o_p_list<int>(m_reg.gn_reg_func, 0);

	m_reg.exe_mode = e_e::random;

	for(auto& i : m_reg.gn_reg_func->con){
		if(i.second == e_sl::list_down){
			o_link(m_reg.gn_reg_exe, i.first, e_sl::list_down, e_sl::nc);
		}
	}

	o_p_list<int>(m_reg.gn_reg_exe, 0);

	auto t_start = std::chrono::high_resolution_clock::now();	
	while(m_reg.gn_reg_exe->con.size() > 0){
		m_reg.execute();
	}
	auto t_end = std::chrono::high_resolution_clock::now();
	double elapsed_time_ms = std::chrono::duration<double, std::milli>(t_end-t_start).count();


	cout<<elapsed_time_ms<<" ms"<<endl;

	/*
	for(int i = 0; i < 100; i++){

		m_reg.execute();
		cout<<"STEP:"<<endl;


		o_p_list<int>(m_reg.gn_reg_exe, 0);

	}
	*/

	o_p_list<uint8_t>(m_reg.gn_reg_data, 1);
	o_p_list<int>(m_reg.gn_reg_func, 0);

	//o_p_list<int>(m_reg.gn_reg_exe, 0);

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
	/*	
	for(auto& i : m_reg.gn_reg_data->con){
		cout<<"Node: "<<i.first<<" Data: "<<(int)((gn_data<uint8_t>*)i.first)->data<<endl;
		for(auto& k : i.first->con){
			cout<<k.first<<" + "<<(int)(k.second)<<endl;
		//	if(k.second == e_sl::list_up){
		//		cout<<"list base pointer is... "<<((gn_data<gn_area*>*)k.first)->data<<endl;
		//	}
		}

		string ab = "p3";
		if(o_s_is(i.first, e_sl::name, ab) == e_r::TRUE){
			cout<<i.first<<" is p1!"<<endl;
		}
	}
	
	cout<<"Function nodes..."<<endl;

	for(auto& i : m_reg.gn_reg_func->con){
		cout<<"Node: "<<i.first<<endl;
		for(auto& k : i.first->con){
			cout<<k.first<<" + "<<(int)(k.second)<<endl;
		}
	}

	*/
	
	cout<<"complete"<<endl;

}
