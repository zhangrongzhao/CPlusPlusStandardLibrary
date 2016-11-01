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
	void pop_back(){//����β��Ԫ��
		--finish;//��β��Ԫ����ǰ�ƶ�һ�񣬱�ʾ����β��Ԫ��
		destory(finish);//����β��Ԫ�ء�destoryȫ�ֺ���
	}

	iterator erase(iterator position){//���ĳ��λ���ϵ�Ԫ��
		if ((position+1)!=end()){
			copy(position+1,finish,position);//����Ԫ����ǰ�ƶ�
		}
		--finish;
		destory(finish);
		return position;
	}
	iterator erase(iterator first,iterator last){
		iterator i = copy(last,finish,first);//copyΪȫ�ֺ���,��last��finish֮���Ԫ��copy��firstΪ��ʼλ�ô���
		destory(i,finish);//�ٽ�i��finish֮���Ԫ�����١�
		finish = finish - (last-first);
		return first;
	}

	void resize(size_type new_size,const T& x){
		if (new_size<size()){
			erase(begin()+new_size,end());
		}else{
			insert(end(),new_size-size(),x);
		}
	}
	void resize(size_type new_size){ resize(new_size,T()); }

	void insert(iterator position,size_type n,const T& x);

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
		const size_type old_size = size();
		const size_type len = old_size != 0 ? 2 * old_size : 1;
		//����ԭ�����ԭ��СΪ0��������1����Ԫ�ش�С��
		//���ԭ��С��Ϊ0��������ԭ��С��2����
		//ǰ��δ��ԭ���ݣ�����׼������������

		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = new_start;
		try{
		    //��ԭvector�����ݿ������µ�vector
			new_finish = uninitialized_copy(start,position,new_start);
			//Ϊ��Ԫ���趨��ֵx
			construct(new_finish,x);
			//����ˮλ
			++new_finish;
			//��������ԭ����copy��������ʾ��������Ҳ���ܱ�insert(p,x)���ã�
			new_finish = uninitialized_copy(position,finish,new_finish);//??????���ﲻ����ΪʲôҪ��������
		}
		catch (...){
			//"commit or rollback"
			destory(new_start,new_finish);
			data_allocator::deallocate(new_start,len);
			throw;
		}

		destory(begin(),end());
		deallocate();

		//������������ָ���µ�vector
		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + len;
	}
}

template<typename T,typename Alloc>
void vector<T, Alloc>::insert(iterator position,size_type n,const T& x){
	if (n!=0){
		if (size_type(end_of_storage-finish)>=n){//���ÿռ���ڵ��ڡ�����Ԫ�صĸ�����
			T x_copy = x;
			//���¼�������֮������Ԫ�ظ���
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			if (elems_after > n){
				//�������֮�������Ԫ�ظ�����>������Ԫ�ظ�����
				uninitialized_copy(finish-n,finish,finish);//����finish֮ǰ��n��Ԫ�ص�finish֮��
				finish += n;//β�˱�Ǻ���
				copy_backward(position,old_finish-n,old_finish);//����position��old_finish-n����Ԫ��copy��old_finish֮ǰ��
				fill(position,position+n,x_copy);//�Ӳ���㿪ʼ������ֵ
			}else{
				//"�����֮�������Ԫ�ظ���"<="����Ԫ�ظ���"
				uninitialized_fill_n(finish,n-elems_after,x_copy);
				finish += n - elems_after;
				uninitialized_copy(position,old_finish,finish);
				finish += elems_after;
				fill(position,old_finish,x_copy);
			}
		}
	}else{
		//���ÿռ�С��"����Ԫ�ظ���",(��Ҫ���ö����ڴ�)
		//���Ⱦ����³��ȣ��ͳ��ȵ�2������ɳ���+����Ԫ�ظ���
		const size_type old_size = size();
		const size_type len = old_size + max(old_size,n);

		//���������µ�vector�ռ�
		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = new_start;

		__STL_TRY{
		    //�������Ƚ���vector�����֮ǰ��Ԫ�ظ��Ƶ��¿ռ�
			new_finish = uninitialized_copy(start,position,new_start);
			//���½�����Ԫ�أ���ֵ��Ϊn�������¿ռ�
			new_finish = uninitialized_fill_n(new_finish,n,x);
			//�����ٽ�vector�����֮���Ԫ�ظ��Ƶ��¿ռ�
			new_finish = uninitialized_copy(position,finish,new_finish);
		}
        #ifdef  __STL_USE_EXCEPTIONS
		catch (...){
		    //����쳣��ʵ��"commit or rollback"
			destory(new_start,new_finish);
			data_allocator::deallocate(new_start,len);
			throw;
	    }
        #endif /*__STL_USE_EXCEPTIONS*/

		//����������ͷžɵ�vector
		destory(start,finish);
		deallocate();
		//���µ���ˮλ
		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + len;
	}
}


////���Դ���
//class Shape{};
//void main(){
//
//}