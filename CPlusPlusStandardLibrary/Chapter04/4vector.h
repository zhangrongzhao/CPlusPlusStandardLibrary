#include "stdafx.h"
#include <cstdlib>
#include "../Chapter02/2stdalloc.cpp"
//alloc��SGI STL�Ŀռ�������
class alloc{};

template<typename T,typename Alloc=alloc>
class vector{
public:
	//vector��Ƕ�����Ͷ���
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
protected:
	//���£�simple_alloc��SGI STL�Ŀռ�����������2.2.4
	typedef simple_alloc<value_type, Alloc> data_allocator;
	iterator start;//��ʾĿǰʹ�ÿռ��ͷ
	iterator finish;//��Ŀǰʹ�ÿռ��β
	iterator end_of_storage;//��ʾĿǰ���ÿռ��β
	void insert_aux(iterator position, const T& x);
	void deallocate(){
		if (start){
			data_allocator::deallocate(start,end_of_storage-start);
		}
	}
	void fill_initialize(size_type n,const T& value){
		start = allocate_and_fill(n,value);
		finish = start + n;
		end_of_storage = finish;
	}
public:
	iterator begin(){ return start; }
	iterator end(){ return finish; }
	size_type size() const{ return size_type(end()-begin()); }
	size_type capacity() const{return size_type(end_of_storage-begin())};
	bool empty() const{ return begin() == end(); }
	reference operator[](size_type n){ return *(begin()+n); }

	vector() :start(0), finish(0), end_of_storage(0){ }
	vector(size_type n, const T& value){ fill_initialize(n,value); }
	vector(int n, const T& value){ fill_initialize(n,value); }
	vector(long n, const T& value){ fill_initialize(n,value); }
	explicit vector(size_type n){ fill_initialize(n,T()); }

	~vector(){
		destroy(start,finish);//���ٶ���
		deallocate();//�黹�ڴ�
	}

	reference front(){ return *begin(); }
	reference back(){ return *(end()-1); }
	void push_back(const T& x){//��Ԫ�ز�����β��
		if (finish!=end_of_storage){//���б��ÿռ�
			construct(finish,x);//ȫ�ֺ���
			++finish;
		}else{//�ռ����أ����ݺ󣬲���β��
			insert_aux(end(),x);
		}
	}
	void pop_back(){//ȡ��β��Ԫ��
		--finish;
		destory(finish);
	}

	iterator erase(iterator position){//���ĳ��λ���ϵ�Ԫ��
		if ((position+1)!=end()){
			copy(position+1,finish,position);//����Ԫ����ǰ�ƶ�
		}
		--finish;
		destory(finish);
		return position;
	}

	void resize(size_type new_size,const T& x){
		if (new_size<size()){
			erase(begin()+new_size,end());
		}else{
			insert(end(),new_size-size(),x);
		}
	}

	void resize(size_type new_size){ resize(new_size,T()); }
	void clear(){ erase(begin(),end()); }
protected:
	//���ÿռ䲢��������
	iterator allocate_end_fill(size_type n,const T& x){
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result,n,x);//ȫ�ֺ���
		return result;
	}
};

template<typename T,typename Alloc>
void vector<T,Alloc>::insert_aux(iterator position,const T& x){
	if (finish!=end_of_storage){//���б��ÿռ�
		construct(finish,*(finish-1));//���ÿռ���ʼ������һ��Ԫ�أ�����vector���һ��Ԫ��Ϊ��ֵ
		++finish;//����ˮλ
		T x_copy = x;
		copy_backward(position,finish-2,finish-1);
		*position = x_copy;
	}else{//���ޱ��ÿռ�

	}
}


////���Դ���
//class Shape{};
//void main(){
//	vector<int>::iterator iviterator;
//	vector<Shape>::iterator sviterator;
//}