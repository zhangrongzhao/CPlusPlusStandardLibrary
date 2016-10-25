#include "3mylist.h"

template<typename Item>
struct ListIterator{
private:
	Item* ptr;//����������֮���һ����ϵ��keep a reference to container��
public:
	ListIterator(Item* p=0) :ptr(p){ }
	//����ʵ��copy ctor,�������ṩ��ȱʡ��Ϊ���㹻
	//����ʵ��operator=,�������ṩ��ȱʡ��Ϊ���㹻
	Item& operator*() const{ return *ptr; }
	Item* operator->()const{ return ptr; }

	//��������operator++,��ѭ��׼����
	//(1)pre-increment operator
	ListIterator& operator++(){
		ptr = ptr->next();//��¶next
		return *this;
	}

	//(2) post-increment operator
	ListIterator operator++(int){
		ListIterator temp = *this;
		++*this;//����ǰ��++����
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




