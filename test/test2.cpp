#include <iostream>
#include <vector>
#include <stdint.h>
#include <mutex>

template <class T>
struct pl{

	//number of tokens array
	//each T represents one place
	std::vector<T> data;
	
	std::mutex pl_lock;	//place lock
	
	uint32_t size;		//maximum size/capacity of each place
	
	uint32_t change;	//increment when written to	
	bool protection;	//are mutexes required?


};

template <class A, class X>
struct pl_index{
	A inside_index;
	X outside_index;
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
int update(tr<T, Y>* a){

    if (a->data.empty()) {
        return -1;
    }
    
    int increment = 0;
    
	bool f_input = false;	//from interleaved pattern; has input side?
	std::vector<T> ra = a->data[0];	//ra is input side, initial any
	for (const auto& i : a->data){
		if(f_input){
			
			//input side loaded, proceed check
			uint32_t f_zero_tokens=0;	//increment this if zero tokens
			for (const auto& k : ra){
				if (a->place_reg[k.outside_index]->data[k.inside_index] == 0){
					f_zero_tokens++;
					std::cout << k.inside_index << "here!" << std::endl;
				}
				std::cout << k.inside_index << "ii" << std::endl;
				std::cout << k.outside_index << "oi" << std::endl;
				std::cout << &a->place_reg[k.outside_index] << "p" << std::endl;
				std::cout << a->place_reg[k.outside_index]->data.size() << "sp" << std::endl;
				std::cout << a->place_reg[k.outside_index]->data[k.inside_index] << "dp" << std::endl;
			}
			std::cout << f_zero_tokens << "zeros" << std::endl;
			if(f_zero_tokens == 0){
				//if no 'zero tokens', increment output
				for (const auto& k : i){
					if(k.outside_index == 0){
						a->place_reg[k.outside_index]->data[k.inside_index]++;
					}else{
						a->place_reg[k.outside_index]->pl_lock.lock();
						a->place_reg[k.outside_index]->data[k.inside_index]++;
						a->place_reg[k.outside_index]->pl_lock.unlock();
					}
				}
				//if no 'zero tokens', decrement input
				for (const auto& k : ra){
					if(k.outside_index == 0){
						a->place_reg[k.outside_index]->data[k.inside_index]--;
					}else{
						a->place_reg[k.outside_index]->pl_lock.lock();
						a->place_reg[k.outside_index]->data[k.inside_index]--;
						a->place_reg[k.outside_index]->pl_lock.unlock();
					}
				}
				break;
			}
			
			
			
		} else {
			//buffer next ra side
			ra = i;
		}
		//alternate f_input for interleaved pattern
		f_input = !f_input;
		std::cout << f_input << " fi000" << std::endl;
		increment++;
		std::cout << increment << " ILLL" << std::endl;
	}
    return 0; // success
}

int main(){

	tr<pl_index<int,int>,int> tr_i;
	pl<int> pl_i;
		
	std::vector<pl_index<int,int>> te = {{1,0}};
	std::vector<pl_index<int,int>> ta = {{0,0}};
	
	pl_i.data.push_back(0);
	pl_i.data.push_back(1);
	
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


	for(int i=0; i<100; i++){
		for(const auto& i : pl_i.data)
			std::cout<<i<<", ";
		std::cout<<std::endl;
		update<pl_index<int,int>, int>(&tr_i);
	}


return 0;}
