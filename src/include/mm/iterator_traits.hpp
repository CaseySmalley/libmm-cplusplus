#ifndef MM_ITERATOR_TRAITS_HPP
#define MM_ITERATOR_TRAITS_HPP
#include "mm/common.hpp"

namespace mm {
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	template <class It>
	struct iterator_traits {
		using difference_type = typename It::difference_type;
		using value_type = typename It::value_type;
		using pointer = typename It::pointer;
		using reference = typename It::reference;
		using iterator_category = typename It::iterator_category;
	};

	template <class T>
	struct iterator_traits<T*> {
		using difference_type = ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = random_access_iterator_tag;
	};

	template <class T>
	struct iterator_traits<const T*> {
		using difference_type = ptrdiff_t;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = random_access_iterator_tag;
	};

	template <class Iter>
	class reverse_iterator {
	public:
		using iterator_type = Iter;
		using difference_type = typename iterator_traits<Iter>::difference_type;
		using value_type = typename iterator_traits<Iter>::value_type;
		using pointer = typename iterator_traits<Iter>::pointer;
		using reference = typename iterator_traits<Iter>::reference;
		using iterator_category = typename iterator_traits<Iter>::iterator_category;

	private:
		iterator_type current;

	public:
		reverse_iterator() = default;
		explicit reverse_iterator(iterator_type it) : current(it) {}
		template <class U> reverse_iterator(const reverse_iterator<U>& other) : current(other.base()) {}

		template <class U>
		reverse_iterator<Iter>& operator=(const reverse_iterator<U>& other) {
			current = other.current;
			return *this;
		}

		iterator_type base() const {
			return current;
		}

		reference operator*() const {
			iterator_type tmp = base();
			return *--tmp;
		}

		pointer operator->() const { return address_of(operator*()); }
		reference operator[](difference_type i) { return base()[-i - 1]; }

		reverse_iterator<Iter>& operator++() {
			--current;
			return *this;
		}

		reverse_iterator<Iter>& operator--() {
			++current;
			return *this;
		}

		reverse_iterator<Iter> operator++(int) {
			reverse_iterator<Iter> tmp(current);
			return --tmp;
		}

		reverse_iterator<Iter> operator--(int) {
			reverse_iterator<Iter> tmp(current);
			return ++tmp;
		}

		reverse_iterator<Iter> operator+(difference_type i) const {
			return reverse_iterator<Iter>(base() - i);
		}

		reverse_iterator<Iter> operator-(difference_type i) const {
			return reverse_iterator<Iter>(base + i);
		}

		reverse_iterator<Iter>& operator+=(difference_type i) {
			current -= i;
			return *this;
		}

		reverse_iterator<Iter>& operator-=(difference_type i) {
			current += i;
			return *this;
		}
	};

	template <class Iter_1,class Iter_2> bool operator==(const reverse_iterator<Iter_1>& lhs,const reverse_iterator<Iter_2>& rhs) { return lhs.base() == rhs.base(); }
	template <class Iter_1,class Iter_2> bool operator!=(const reverse_iterator<Iter_1>& lhs,const reverse_iterator<Iter_2>& rhs) { return lhs.base() != rhs.base(); }
	template <class Iter_1,class Iter_2> bool operator>(const reverse_iterator<Iter_1>& lhs,const reverse_iterator<Iter_2>& rhs) { return lhs.base() > rhs.base(); }
	template <class Iter_1,class Iter_2> bool operator<(const reverse_iterator<Iter_1>& lhs,const reverse_iterator<Iter_2>& rhs) { return lhs.base() < rhs.base(); }
	template <class Iter_1,class Iter_2> bool operator>=(const reverse_iterator<Iter_1>& lhs,const reverse_iterator<Iter_2>& rhs) { return lhs.base() >= rhs.base(); }
	template <class Iter_1,class Iter_2> bool operator<=(const reverse_iterator<Iter_1>& lhs,const reverse_iterator<Iter_2>& rhs) { return lhs.base() <= rhs.base(); }

	template <class Iter> reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type i,const reverse_iterator<Iter>& it) { return reverse_iterator<Iter>(it.base() - i); }
	template <class Iter> reverse_iterator<Iter> operator-(typename reverse_iterator<Iter>::difference_type i,const reverse_iterator<Iter>& it) { return reverse_iterator<Iter>(it.base() + i); }

	template <class C>
	class reverse_adapter {
	public:
		using iterator = typename C::reverse_iterator;
		using const_iterator = typename C::const_reverse_iterator;

	private:
		C& ref;

	public:
		reverse_adapter() = delete;
		reverse_adapter(const reverse_adapter<C>&) = default;
		reverse_adapter(reverse_adapter<C>&&) = default;
		reverse_adapter<C>& operator=(const reverse_adapter<C>&) = delete;
		reverse_adapter<C>& operator=(reverse_adapter<C>&&) = delete;
	
		explicit reverse_adapter(C& ref) : ref(ref) {}

		iterator begin() { return ref.rbegin(); }
		const_iterator cbegin() { return ref.crbegin(); }
		
		iterator end() { return ref.rend(); }
		const_iterator cend() { return ref.crend(); }
	};

	template <class C>
	reverse_adapter<C> make_reverse_adapter(C& ref) {
		return reverse_adapter<C>(ref);
	}
}

#endif
