#include "stdafx.h"
#include "4deque.h"

//queue:�Ƚ��ȳ���ֻ�ж���Ԫ�ز��ܱ����ʣ�queue���ṩ�߷ù��ܣ�Ҳ���ṩ������
template<typename T,typename Sequence=deque<T> >
class queue{
	//���µ�__STL_NULL_TMPL_ARGS����չΪ<>,��1.9.1��
	friend bool operator==__STL_NULL_TMPL_ARGS(const queue& x,const queue& y);
	friend bool operator< __STL_NULL_TMPL_ARGS(const queue& x,const queue& y);
public:
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::size_type size_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
protected:
	Sequence c;//�ײ�����
public:
	//������ȫ����Sequence c�Ĳ��������queue�Ĳ���
	bool empty() const{ return c.empty(); }
	size_type size() const{ return c.size(); }
	reference front(){ return c.front(); }
	const_reference front() const{ return c.front(); }
	reference back(){ return c.back(); }
	const_reference back() const{ return c.back(); }
	//deque����ͷ�ɽ�����queue��ĩ�˽���ǰ�˳�(�Ƚ��ȳ�)
	void push(const value_type& x){ c.push_back(x); }
	void pop(){ c.pop_front(); }
};

template<typename T,typename Sequence>
bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y){ return x.c == y.c; }

template<typename T,typename Sequence>
bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y){ return x.c < y.c; }