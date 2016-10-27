#include "stdafx.h"

//function template 的参数推到argument deducation（argument:实参；parameter:形参）
template<typename I,typename T>
void func_impl(I itertor,T t){

}

template<typename I>
void func(I iterator){
	func_impl(iterator,*iterator);
}

//int main(){
//	int i=0;
//	func(&i);
//}
//function template的argument deducation不能解决所有情况，需要更全面的解法，函数返回值无法推导
