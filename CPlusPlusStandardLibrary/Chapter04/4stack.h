#include "stdafx.h"
#include "4deque.h"

//stack:先进后出，后进先出，只有顶端元素才会被外界使用。stack不提供走访功能，也不提供迭代器
template<typename T,typename Sequence=deque<T> >
class stack{
	//以下的__STL_NULL_TMPL_ARGS会扩展为<>,见1.9.1节
	friend bool operator== __STL_NULL_TMPL_ARGS(const stack&,const stack&);
	friend bool operator< __STL_NULL_TMPL_ARGS(const stack&,const stack&);
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;//底层容器
public:
	//以下完全利用Sequence c的操作，完成stack的操作
	bool empty() const{ return c.empty();}
	size_type size() const{ return c.size(); }
	reference top(){ return c.back(); }
	const_reference top() const{ return c.back(); }
	//deque是两头可进出，stack是末端进，末端出(后进先出)
	void push(const value_type& x){ c.push_back(x); }
	void pop(){ c.pop_back(); }
};

template<typename T,typename Sequence>
bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y){ return x.c == y.c; }

template<typename T,typename Sequence>
bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y){ return x.c < y.c; }