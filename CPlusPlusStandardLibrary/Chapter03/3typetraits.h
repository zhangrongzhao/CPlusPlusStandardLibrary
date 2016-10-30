#include "stdafx.h"

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION
#   define __STL_TEMPLATE_NULL template<>
#else
#   define __STL_TEMPLATE_NULL
#endif

struct __true_type{};
struct __false_type{};

template<typename Type>
struct __type_traits{
	typedef __true_type this_dummy_member_must_be_first;

	typedef __false_type has_trivial_default_constructor;
	typedef __false_type has_trivial_destructor;
	typedef __false_type has_trivial_copy_constructor;
	typedef __false_type has_trivial_assignment_operator;
	typedef __false_type is_POD_type;
};

template<typename T>
struct __type_traits<T*>{
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};


__STL_TEMPLATE_NULL struct __type_traits<char> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<signed char> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned char> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<short> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned short> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<int> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned int> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<long> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned long> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<float> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<double> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<long double> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

__STL_TEMPLATE_NULL struct __type_traits<unsigned long> {
	typedef __true_type has_trivial_default_constructor;
	typedef __true_type has_trivial_destructor;
	typedef __true_type has_trivial_copy_constructor;
	typedef __true_type has_trivial_assignment_operator;
	typedef __true_type is_POD_type;
};

