#include "stdafx.h"
#include "4deque.h"

//queue:先进先出，只有顶端元素才能被访问，queue不提供走访功能，也不提供迭代器
template<typename T,typename Sequence=deque<T> >
class queue{
	//以下的__STL_NULL_TMPL_ARGS会扩展为<>,见1.9.1节
	friend bool operator==__STL_NULL_TMPL_ARGS(const queue& x,const queue& y);
	friend bool operator< __STL_NULL_TMPL_ARGS(const queue& x,const queue& y);
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;//底层容器
public:
	//以下完全利用Sequence c的操作，完成queue的操作
	bool empty() const{ return c.empty(); }
	size_type size() const{ return c.size(); }
	reference front(){ return c.front(); }
	const_reference front() const{ return c.front(); }
	reference back(){ return c.back(); }
	const_reference back() const{ return c.back(); }
	//deque是两头可进出，queue是末端进，前端出(先进先出)
	void push(const value_type& x){ c.push_back(x); }
	void pop(){ c.pop_front(); }
};

template<typename T,typename Sequence>
bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y){ return x.c == y.c; }

template<typename T,typename Sequence>
bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y){ return x.c < y.c; }