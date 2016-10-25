#include "3mylist.h"

template<typename Item>
struct ListIterator{
private:
	Item* ptr;//保持与容器之间的一个联系（keep a reference to container）
public:
	ListIterator(Item* p=0) :ptr(p){ }
	//不必实现copy ctor,编译器提供的缺省行为已足够
	//不必实现operator=,编译器提供的缺省行为已足够
	Item& operator*() const{ return *ptr; }
	Item* operator->()const{ return ptr; }

	//以下两个operator++,遵循标准做法
	//(1)pre-increment operator
	ListIterator& operator++(){
		ptr = ptr->next();//暴露next
		return *this;
	}

	//(2) post-increment operator
	ListIterator operator++(int){
		ListIterator temp = *this;
		++*this;//调用前置++运算
		//ptr = ptr->next();
		return temp;
	}

	bool operator==(const ListIterator& i) const{
		return ptr == i.ptr;
	}

	bool operator!=(const ListIterator& i) const{
		return ptr != i.ptr;
	}
};




