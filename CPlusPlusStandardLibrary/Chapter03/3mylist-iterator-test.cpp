#include "stdafx.h"
#include "3MyListIterator.h"

////现在我们可以将List和find()结合起来
//void main(){
//	List<int> mylist;
//	for (int i = 0; i < 5; ++i){
//		mylist.insert_front(i);
//		mylist.insert_end(i+2);
//	}
//	mylist.display();
//
//	ListIterator<ListItem<int> > begin(mylist.front());//暴露ListItem
//	ListIterator<ListItem<int> > end;//default 0,null
//	ListIterator<ListItem<int> > iterator;//default 0,null
//
//	iterator = find(begin,end,3);
//	if (iterator == end){
//		cout << "not found" << endl;
//	}
//	else{
//		cout << "found ." << iterator->value() << endl;
//	}
//
//	iterator = find(begin,end,7);
//	if (iterator == end){
//		cout << "not found" << endl;
//	}
//	else{
//		cout << "found ." << iterator->value() << endl;
//	}
//}
//
//template<typename T>
//bool operator!=(const ListItem<T>& item,T n){
//	return item.value() != n;
//}