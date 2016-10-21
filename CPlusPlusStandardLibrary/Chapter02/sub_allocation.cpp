#include "stdafx.h"

enum{ __ALIGN = 8 };//小型区块的上调边界
enum{ __MAX_BYTES = 128 };//小型区块的上限
enum{ __NFREELISTS = __MAX_BYTES/__ALIGN};//free-list个数

//二级配置器
template<bool threads,int inst>
class __default_alloc_template{
private:
	//ROUND_UP()将bytes上调至8的倍数
	static size_t ROUND_UP(size_t bytes){
		return (((bytes)+__ALIGN-1) & ~(__ALIGN-1));
	}

private:
	union obj {//free-list的节点构造
		union obj* free_list_link;
		char client_data[1];/*The client sees this.*/
	};
private:
	//16个free-lists
	static obj* volatile free_list[__NFREELISTS];
	
	//根据区块大小，决定使用第n号free-list,n从0算起
	static size_t FREELIST_INDEX(size_t bytes){
		return (((bytes)+__ALIGN-1)/__ALIGN-1);
	}

	//返回大小为n的对象，并可能加入大小为n的其他区块到free list
	static void* refill(size_t n);

	//配置一大块空间，可容纳nobjs个大小为size的区块，如果配置nobjs个区块有所不便，nobjs可能会降低
	static char* chunk_alloc(size_t size,int& nobjs);

	//chunk allocation state
	static char* start_free;//内存池起始位置，只在chunk_alloc()中变化
	static char* end_free;//内存池结束位置，只在chunk_alloc()中变化
	static size_t heap_size;

public:
	static void* allocate(size_t n);
	static void deallocate(void* p,size_t n);
	static void* reallocate(void* p,size_t old_sz,size_t new_sz);
};

//以下是static data member的定义与初值设定
template<bool threads,int inst>
char* __default_alloc_template<threads, inst>::start_free = 0;

template<bool threads,int inst>
char* __default_alloc_template<threads, inst>::end_free = 0;

template<bool threads,int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

template<bool threads,int inst>
__default_alloc_template<threads, inst>::obj* volatile 
__default_alloc_template<threads, inst>::free_list[__NFREELISTS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

//空间配置函数allocate()
template<bool threads,int inst>
void* __default_alloc_template<threads, inst>::allocate(size_t n){
	obj* volatile * my_free_list;
	obj* result;

	//大于128就调用第一级配置器
	if (n>(size_t)__MAX_BYTES){
		return (malloc_alloc::allocate(n));
	}

	//寻找16个free lists中适当的一个
	my_free_list = free_list + FREELIST_INDEX(n);
	result = *my_free_list;
	if (result == 0){//没有找到可用的free list,准备重新填充free list
		void* r = refill(ROUND_UP(n));
		return r;
	}

	//调整free lists
	*my_free_list = result->free_list_link;
	return (result);
}

//空间释放函数deallocate()
template<bool threads,int inst>
void __default_alloc_template<threads, inst>::deallocate(void* p,size_t n){
	obj *q = (obj *)p;
	obj* volatile * my_free_list;

	//大于128调用一级配置器
	if (n > (size_t)__MAX_BYTES){
		malloc_alloc::deallocate(p,n);
		return;
	}

	//找对应的free list
	my_free_list = free_list + FREELIST_INDEX(n);

	//调整free list
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}

//返回一个大小为n的对象，并且有时候会为适当的free list增加节点，假设n已经上调至8的倍数
template<bool threads,int inst>
void __default_alloc_template<threads, inst>::refill(size_t n){
	int nobjs = 20;
	//调用chunk_alloc(),尝试取得nobjs个区块作为free list的新节点
	//注意参数nobjs是pass by reference
	char* chunk = chunk_alloc(n,nobjs);
	obj* volatile * my_free_list;

	obj* result;
	obj* current_obj, *next_obj;
	int i;

	//如果只有一个区块，这个区块就分配给调用者用，free list 无新节点
	if(nobjs == 1) return (chunk);
	//否者准备调整free list,纳入新节点
	my_free_list = free_list + FREELIST_INDEX(n);

	//以下在chunk空间内建立free list.
	result = (obj *)chunk;//chunk空间首地址，准备返回给客户端
	//以下导引free list指向新配置的空间（取自内存池）
	*my_free_list = next_obj = (obj *)(chunk+n);
	//以下将free list的各个节点串联起来
	for(i = 1;;i++){//从1开始，因为第0个将返回给客户端
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
//假设size已经适当上调至8的倍数
//注意参数nobjs是pass by reference
template<bool threads,int inst>
char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int& nobjs){
	char* result;
	size_t total_bytes = size*nobjs;
	size_t bytes_left = end_free - start_free;//内存池剩余空间

	if (bytes_left>=total_bytes){//内存池剩余空间完全满足需求量
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else if(bytes_left>=size){//内存池不能完全满足需求量，但是够供应一个(含)以上的区块
		nobjs = bytes_left / size;
		total_bytes = size*nobjs;
		result = start_free;
		start_free += total_bytes;
		return (result);
	}else {//内存池剩余空间连一个区块的大小都无法提供
		size_t bytes_to_get = 2 * toal_bytes + ROUND_UP(heap_size>>4);
		//以下试着让内存池中的残余零头还有利用价值
		if(bytes_left > 0){//内存池还有一些零头，先配给适当的free list.首先要找到适当的free list.
		   obj* volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
		   //调整free list. 将内存池中的残余空间编入free list.
		   ((obj *)start_free)->free_list_link = *my_free_list;
		   *my_free_list = (obj *)start_free;
		};

		//配置heap空间，用来补充内存池
		start_free = (char*)malloc(bytes_to_get);
		if(start_free == 0){//heap 空间不足，malloc()失败
			int i;
			obj* volatile * my_free_list, *p;
			//试着检视我们手上拥有的东西，这不会造成伤害。我们不打算尝试配置较小的区块，因为那在多进程机器上容易导致灾难。
			//以下搜寻适当的free list.
			//所谓适当是指“尚有未用区块，且区块够大”之 free list.
			for(i = size; i <= __MAX_BYTES;i+=__ALIGN){
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (p != 0){//free list内尚有未用区块,调整free list以释放出未用区块
					*my_free_list = p->free_list_link;
					start_free = (char *)p;
					end_free = start_free + i;
					//递归调用自己，为了修正nobjs
					return (chunk_alloc(size,nobjs));
					//注意，任何残余零头终将被编入适当的free list中备用
				}
			}

			end_free = 0;//如果出现意外（没有内存可用）
			//调用一级配置器，看看out-of-memory机制能否进点力
			start_free = (char*)malloc:allocate(bytes_to_get);
			//这会导致抛出异常exception,或者内存不足的情况获得改善
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			//递归调用自己，为了修正nobjs
			return (chunk_alloc(size,nobjs));
		}
	}
}