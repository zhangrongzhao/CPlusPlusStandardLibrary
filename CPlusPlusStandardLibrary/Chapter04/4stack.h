#include "stdafx.h"
#include "4deque.h"

//stack:�Ƚ����������ȳ���ֻ�ж���Ԫ�زŻᱻ���ʹ�á�stack���ṩ�߷ù��ܣ�Ҳ���ṩ������
template<typename T,typename Sequence=deque<T> >
class stack{
	//���µ�__STL_NULL_TMPL_ARGS����չΪ<>,��1.9.1��
	friend bool operator== __STL_NULL_TMPL_ARGS(const stack&,const stack&);
	friend bool operator< __STL_NULL_TMPL_ARGS(const stack&,const stack&);
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;//�ײ�����
public:
	//������ȫ����Sequence c�Ĳ��������stack�Ĳ���
	bool empty() const{ return c.empty();}
	size_type size() const{ return c.size(); }
	reference top(){ return c.back(); }
	const_reference top() const{ return c.back(); }
	//deque����ͷ�ɽ�����stack��ĩ�˽���ĩ�˳�(����ȳ�)
	void push(const value_type& x){ c.push_back(x); }
	void pop(){ c.pop_back(); }
};

template<typename T,typename Sequence>
bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y){ return x.c == y.c; }

template<typename T,typename Sequence>
bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y){ return x.c < y.c; }