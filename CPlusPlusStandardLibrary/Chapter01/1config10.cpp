//���� template �����ɷ����ǰһ��template�������趨Ĭ��ֵ

#include "stdafx.h"
#include <iostream>
using namespace std;

class alloc{};

template<typename T,typename Alloc=alloc,size_t BufSiz=0>
class deque{
public:
	deque(){ cout << "deque" << endl; }
};
	
template<typename T,typename Sequence=deque<T> >
class stack{
	public:
		stack(){ cout << "stack" << endl; }
	private:
		Sequence c;
};

//void test(){
//	stack<int> x;
//}
