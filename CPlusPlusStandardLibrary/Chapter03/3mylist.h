#include <iostream>
using namespace std;

template<typename T>
class ListItem{
private:
	T _value;
	ListItem* _next;//µ•œÓ¡¥±Ì£¨single linked list
public:
	T value() const{ return _value; }
	ListItem* next() const{ return _next; }
};

template<typename T>
class List{
private:
	ListItem<T>* _end;
	ListItem<T>* _front;
	long _size;
public:
	ListItem<T>* front(){ return _front; }
	ListItem<T>* end(){ return _end; }
public:
	void insert_front(T value);
	void insert_end(T value);
	void display(std::ostream& os=std::cout) const;
};

template<typename T>
void List<T>::insert_front(T value){

}

template<typename T>
void List<T>::insert_end(T value){

}

template<typename T>
void List<T>::display(std::ostream& os=std::cout) const{

}

