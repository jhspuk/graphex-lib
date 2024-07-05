#include <iostream>
#include <vector>
#include <stdint.h>
#include <mutex>
#include <thread>

template <class A, class X>
struct pl_index{
	A inside_index;
	X outside_index;
};

template <class A, class X>
struct pl_token{
	A type;
	X token;
};

template <class T>
struct pl{

	//number of tokens array
	//each T represents one place
	std::vector<T> data;
	
	std::recursive_mutex pl_lock;	//place lock
	
	uint32_t type;		//maximum size/capacity of each place
	
	uint32_t change;	//increment when written to	
	bool protection;	//are mutexes required?


};

//todo: class concepts/constraints
template <class T, class Y>
struct tr{
	
	//index of place capacity
	//input/output vectors alternate
	std::vector<std::vector<T>> data;	
	
	//place group registry
	//used to reference place groups
	std::vector<pl<Y>*> place_reg;

	uint8_t size_a;	//place struct index maximum size
	uint8_t size_b;	//place struct number maximum size
	
	bool clear;			//did the last run yield all complete?	

};

template <class T, class Y>
int update(tr<T, Y>* a, uint32_t mode){

    if (a->data.empty()) {
        return -1;
    }
    
    int counter = 0;
	bool f_input = false;	//from interleaved pattern; has input side?
	std::vector<T> ra = a->data[0];	//ra is input side, initial any
	std::vector<u_int32_t> e_list;	//list of possible executions
	for (const auto& i : a->data){
		if(f_input){
			uint32_t f_zero_tokens=0;	//increment this if zero tokens
			for (const auto& k : ra){
				
				auto z_deref = a->place_reg[k.outside_index];
				if (z_deref->protection == 1){
					//lock place group if protection enabled
					z_deref->pl_lock.lock();
				}					
				if (z_deref->data[k.inside_index].type == 0 && z_deref->data[k.inside_index].token < 1){
					//if normal PN and no tokens, increase zero_tokens variable
					f_zero_tokens++;
					//std::cout << f_zero_tokens << " Here!" << std::endl;
				}
				//std::cout << k.inside_index << "ii" << std::endl;
				//std::cout << k.outside_index << "oi" << std::endl;
				//std::cout << &a->place_reg[k.outside_index] << "p" << std::endl;
				//std::cout << a->place_reg[k.outside_index]->data.size() << "sp" << std::endl;
				//std::cout << a->place_reg[k.outside_index]->data[k.inside_index].token << "dp" << std::endl;
				
			}
			if(f_zero_tokens == 0){
				
				//given there is at least a token for each input
				//push back index of input for a possible execution
				e_list.push_back(counter-1);
				
			}
			
		} else {
			//buffer next ra side
			ra = i;
		}
		//alternate f_input for interleaved pattern
		f_input = !f_input;
		//increment counter
		counter++;
	}
	
	switch(mode){
		//process all with no order
		case 0:
			for(const auto& i : e_list){
				//std::cout << i << " AAAAAAA" << std::endl;
				auto p_in = a->data[i];
				auto p_out = a->data[i+1];
				
				//execute transition
				for (const auto& k : p_out){
					auto z_deref = a->place_reg[k.outside_index];
					if(z_deref->protection == 1){
						z_deref->pl_lock.lock();	//lock recursive mutex
					}
					if(z_deref->data[k.inside_index].type == 0){
						//if normal PN type, increment
						z_deref->data[k.inside_index].token++;
						
					}else if(z_deref->data[k.inside_index].type == 1){
						//if type 1, execute function...
					}
					if(z_deref->protection == 0){
						z_deref->pl_lock.unlock();	//unlock recursive mutex
					}
				}
				
				//if no 'zero tokens', decrement input
				for (const auto& k : p_in){
					auto z_deref = a->place_reg[k.outside_index];
					if(z_deref->data[k.inside_index].type == 0){
						//if normal PN type, decrement
						z_deref->data[k.inside_index].token--;
						
					} else if(z_deref->data[k.inside_index].type == 1){
						//if type 1, execute function...o
					}
				}
				break;
			}
			break;
		default:
			break;	
	}
	
	//unlock all inputs
	f_input = 0;
	for (const auto& i : a->data){
		if(!f_input){
			for (const auto& k : i){
				if(a->place_reg[k.outside_index]->protection){
					a->place_reg[k.outside_index]->pl_lock.unlock();
				}
			}
		}
		f_input = !f_input;
	}
	
    return 0; // success
}

template <class T, class Y, class C>
void funUp(tr<T, Y>* a, pl<C> & b){
	for(int i=0; i<100; i++){
		for(const auto& v : b.data)
			std::cout<<v.token<<" ";
		std::cout<<"num: "<<i<<std::endl;
		update<pl_index<int,int>, pl_token<int,int>>(a, 0);
	}
return;}

int main(){

	tr<pl_index<int,int>,pl_token<int,int>> tr_i;
	pl<pl_token<int,int>> pl_i;
		
	std::vector<pl_index<int,int>> te = {{1,0}};
	std::vector<pl_index<int,int>> ta = {{0,0}};
	
	pl_i.data.push_back({0,0});
	pl_i.data.push_back({0,1});
	
	pl_i.protection = 1;
	
	tr_i.data.push_back(te);
	tr_i.data.push_back(ta);
	tr_i.data.push_back(ta);
	tr_i.data.push_back(te);
	
	tr_i.place_reg.push_back(&pl_i);
	
	std::cout << "Size of place_reg: " << tr_i.place_reg.size() << std::endl;
	for (size_t i = 0; i < tr_i.place_reg.size(); ++i) {
	    if (tr_i.place_reg[i] == nullptr) {
	        std::cout << "place_reg[" << i << "] is nullptr\n";
	    } else {
	        std::cout << "place_reg[" << i << "] is initialized, data size: " << tr_i.place_reg[i]->data.size() << std::endl;
	    }
	}
	
	
	std::thread a1(funUp<pl_index<int,int>, pl_token<int,int>, pl_token<int,int>>,&tr_i, std::ref(pl_i));
	std::thread a2(funUp<pl_index<int,int>, pl_token<int,int>, pl_token<int,int>>,&tr_i, std::ref(pl_i));
	std::thread a3(funUp<pl_index<int,int>, pl_token<int,int>, pl_token<int,int>>,&tr_i, std::ref(pl_i));
	
	a1.join();
	a2.join();
	a3.join();
	
	std::cout<<"Finished!"<<std::endl;
	//funUp<pl_index<int,int>, pl_token<int,int>, pl_token<int,int>>(&tr_i, pl_i);

	//for(int i=0; i<100; i++){
		//for(const auto& i : pl_i.data)
			//std::cout<<i.token<<", ";
		//std::cout<<std::endl;
		//update<pl_index<int,int>, pl_token<int,int>>(&tr_i, 0);
	//}


return 0;}
