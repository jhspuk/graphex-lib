#include <iostream>
#include <vector>
#include <stdint.h>

template <class T>
struct tr{

	uint32_t size_a;	//place struct index maximum size
	uint32_t size_b;	//place struct number maximum size

	std::vector<std::vector<T>> data;	//input/output vectors alternate

};

template <class T>
struct pl{

	uint32_t size;		//maximum size/capacity of each place

	std::vector<T> data;
};

template <class T, class Y>
int update(tr<T>* a, pl<Y>* b){

    if (a->data.empty()) {
        return -1;
    }

    auto f1 = false;
    std::vector<T> ra = a->data[0];
    for (const auto& i : a->data){
        if (f1) {
            // iterate input vector
            int f2 = 0;
            std::cout<<"f2 is "<<f2<<std::endl;
            for (const auto& k : ra) {
                // add one if any input is not 1
                if (b->data[k] == 0) {
                    f2++;
                    std::cout<<k<<" is not 1 "<<"("<<b->data[k]<<")"<<std::endl; }
            }
            // if there were no "not 1"s, perform transition
            if (f2 == 0) {
                // increment all output places by 1
                for (const auto& k : i){
                    b->data[k]++;
                    std::cout<<k<<" increment "<<"("<<b->data[k]<<")"<<std::endl;
                    }
                // decrement all input places by 1
                for (const auto& k : ra){
                    b->data[k]--;
                    std::cout<<k<<" decrement "<<"("<<b->data[k]<<")"<<std::endl;
                    }
		break;
            }
        } else {
            ra = i;
        }
        f1 = !f1;
    }

    return 0; // success
}



int main(){

	tr<int> tr_i;
	pl<int> pl_i;

	std::vector<int> te = {1};
	std::vector<int> ta = {0};

	tr_i.data.push_back(te);
	tr_i.data.push_back(ta);
	tr_i.data.push_back(ta);
	tr_i.data.push_back(te);
	
	pl_i.data.push_back(0);
	pl_i.data.push_back(1);
	/*
	for(const auto& i : tr_i.data)
		for(const auto& k : i)
			std::cout<<k<<std::endl;
	*/
	for(int i=0; i<100; i++){
		for(const auto& i : pl_i.data)
			std::cout<<i<<", ";
		std::cout<<std::endl;
		update<int, int>(&tr_i, &pl_i);
	}

return 0;}
