#include "stdafx.h"

//function template �Ĳ����Ƶ�argument deducation��argument:ʵ�Σ�parameter:�βΣ�
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
//function template��argument deducation���ܽ�������������Ҫ��ȫ��Ľⷨ����������ֵ�޷��Ƶ�
