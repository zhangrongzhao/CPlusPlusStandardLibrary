#include "stdafx.h"

enum{ __ALIGN = 8 };//С��������ϵ��߽�
enum{ __MAX_BYTES = 128 };//С�����������
enum{ __NFREELISTS = __MAX_BYTES/__ALIGN};//free-list����

//����������
template<bool threads,int inst>
class __default_alloc_template{
private:
	//ROUND_UP()��bytes�ϵ���8�ı���
	static size_t ROUND_UP(size_t bytes){
		return (((bytes)+__ALIGN-1) & ~(__ALIGN-1));
	}

private:
	union obj {//free-list�Ľڵ㹹��
		union obj* free_list_link;
		char client_data[1];/*The client sees this.*/
	};
private:
	//16��free-lists
	static obj* volatile free_list[__NFREELISTS];
	
	//���������С������ʹ�õ�n��free-list,n��0����
	static size_t FREELIST_INDEX(size_t bytes){
		return (((bytes)+__ALIGN-1)/__ALIGN-1);
	}

	//���ش�СΪn�Ķ��󣬲����ܼ����СΪn���������鵽free list
	static void* refill(size_t n);

	//����һ���ռ䣬������nobjs����СΪsize�����飬�������nobjs�������������㣬nobjs���ܻή��
	static char* chunk_alloc(size_t size,int& nobjs);

	//chunk allocation state
	static char* start_free;//�ڴ����ʼλ�ã�ֻ��chunk_alloc()�б仯
	static char* end_free;//�ڴ�ؽ���λ�ã�ֻ��chunk_alloc()�б仯
	static size_t heap_size;

public:
	static void* allocate(size_t n);
	static void deallocate(void* p,size_t n);
	static void* reallocate(void* p,size_t old_sz,size_t new_sz);
};

//������static data member�Ķ������ֵ�趨
template<bool threads,int inst>
char* __default_alloc_template<threads, inst>::start_free = 0;

template<bool threads,int inst>
char* __default_alloc_template<threads, inst>::end_free = 0;

template<bool threads,int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template<bool threads,int inst>
__default_alloc_template<threads, inst>::obj* volatile 
__default_alloc_template<threads, inst>::free_list[__NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

//�ռ����ú���allocate()
template<bool threads,int inst>
void* __default_alloc_template<threads, inst>::allocate(size_t n){
	obj* volatile * my_free_list;
	obj* result;

	//����128�͵��õ�һ��������
	if (n>(size_t)__MAX_BYTES){
		return (malloc_alloc::allocate(n));
	}

	//Ѱ��16��free lists���ʵ���һ��
	my_free_list = free_list + FREELIST_INDEX(n);
	result = *my_free_list;
	if (result == 0){//û���ҵ����õ�free list,׼���������free list
		void* r = refill(ROUND_UP(n));
		return r;
	}

	//����free lists
	*my_free_list = result->free_list_link;
	return (result);
}

//�ռ��ͷź���deallocate()
template<bool threads,int inst>
void __default_alloc_template<threads, inst>::deallocate(void* p,size_t n){
	obj *q = (obj *)p;
	obj* volatile * my_free_list;

	//����128����һ��������
	if (n > (size_t)__MAX_BYTES){
		malloc_alloc::deallocate(p,n);
		return;
	}

	//�Ҷ�Ӧ��free list
	my_free_list = free_list + FREELIST_INDEX(n);

	//����free list
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}

//����һ����СΪn�Ķ��󣬲�����ʱ���Ϊ�ʵ���free list���ӽڵ㣬����n�Ѿ��ϵ���8�ı���
template<bool threads,int inst>
void __default_alloc_template<threads, inst>::refill(size_t n){
	int nobjs = 20;
	//����chunk_alloc(),����ȡ��nobjs��������Ϊfree list���½ڵ�
	//ע�����nobjs��pass by reference
	char* chunk = chunk_alloc(n,nobjs);
	obj* volatile * my_free_list;

	obj* result;
	obj* current_obj, *next_obj;
	int i;

	//���ֻ��һ�����飬�������ͷ�����������ã�free list ���½ڵ�
	if(nobjs == 1) return (chunk);
	//����׼������free list,�����½ڵ�
	my_free_list = free_list + FREELIST_INDEX(n);

	//������chunk�ռ��ڽ���free list.
	result = (obj *)chunk;//chunk�ռ��׵�ַ��׼�����ظ��ͻ���
	//���µ���free listָ�������õĿռ䣨ȡ���ڴ�أ�
	*my_free_list = next_obj = (obj *)(chunk+n);
	//���½�free list�ĸ����ڵ㴮������
	for(i = 1;;i++){//��1��ʼ����Ϊ��0�������ظ��ͻ���
		current_obj = next_obj;
		next_obj = (obj*)((char *)next_obj+n);
		if (nobjs-1==i){
			current_obj->free_list_link = 0;
			break;
		}
		else{
			current_obj->free_list_link = next_obj;
		}
	}
	return (result);
}

//memory pool
//����size�Ѿ��ʵ��ϵ���8�ı���
//ע�����nobjs��pass by reference
template<bool threads,int inst>
char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs){
	char* result;
	size_t total_bytes = size*nobjs;
	size_t bytes_left = end_free - start_free;//�ڴ��ʣ��ռ�

	if (bytes_left>=total_bytes){//�ڴ��ʣ��ռ���ȫ����������
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else if(bytes_left>=size){//�ڴ�ز�����ȫ���������������ǹ���Ӧһ��(��)���ϵ�����
		nobjs = bytes_left / size;
		total_bytes = size*nobjs;
		result = start_free;
		start_free += total_bytes;
		return (result);
	}else {//�ڴ��ʣ��ռ���һ������Ĵ�С���޷��ṩ
		size_t bytes_to_get = 2 * toal_bytes + ROUND_UP(heap_size>>4);
		//�����������ڴ���еĲ�����ͷ�������ü�ֵ
		if(bytes_left > 0){//�ڴ�ػ���һЩ��ͷ��������ʵ���free list.����Ҫ�ҵ��ʵ���free list.
		   obj* volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
		   //����free list. ���ڴ���еĲ���ռ����free list.
		   ((obj *)start_free)->free_list_link = *my_free_list;
		   *my_free_list = (obj *)start_free;
		};

		//����heap�ռ䣬���������ڴ��
		start_free = (char*)malloc(bytes_to_get);
		if(start_free == 0){//heap �ռ䲻�㣬malloc()ʧ��
			int i;
			obj* volatile * my_free_list, *p;
			//���ż�����������ӵ�еĶ������ⲻ������˺������ǲ����㳢�����ý�С�����飬��Ϊ���ڶ���̻��������׵������ѡ�
			//������Ѱ�ʵ���free list.
			//��ν�ʵ���ָ������δ�����飬�����鹻��֮ free list.
			for(i = size; i <= __MAX_BYTES;i+=__ALIGN){
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (p != 0){//free list������δ������,����free list���ͷų�δ������
					*my_free_list = p->free_list_link;
					start_free = (char *)p;
					end_free = start_free + i;
					//�ݹ�����Լ���Ϊ������nobjs
					return (chunk_alloc(size,nobjs));
					//ע�⣬�κβ�����ͷ�ս��������ʵ���free list�б���
				}
			}

			end_free = 0;//����������⣨û���ڴ���ã�
			//����һ��������������out-of-memory�����ܷ������
			start_free = (char*)malloc:allocate(bytes_to_get);
			//��ᵼ���׳��쳣exception,�����ڴ治��������ø���
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			//�ݹ�����Լ���Ϊ������nobjs
			return (chunk_alloc(size,nobjs));
		}
	}
}