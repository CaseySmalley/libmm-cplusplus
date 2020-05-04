#ifndef MM_ITERATOR_HPP
#define MM_ITERATOR_HPP
#include "mm/utility.hpp"

namespace mm {
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : mm::input_iterator_tag {};
	struct bidirectional_iterator_tag : mm::forward_iterator_tag {};
	struct random_access_iterator_tag : mm::bidirectional_iterator_tag {};

	template <class Iter>
	struct iterator_traits {
		using difference_type = typename Iter::difference_type;
		using value_type = typename Iter::value_type;
		using pointer = typename Iter::pointer;
		using reference = typename Iter::reference;
		using iterator_category = typename Iter::iterator_category;
	};

	template <class T>
	struct iterator_traits<T*> {
		using difference_type = mm::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = mm::random_access_iterator_tag;
	};

	template <class T>
	struct iterator_traits<const T*> {
		using difference_type = mm::ptrdiff_t;
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		using iterator_category = mm::random_access_iterator_tag;
	};

	template <class Iter>
	class reverse_iterator {
	public:
		using iterator_type = Iter;
		using difference_type = typename mm::iterator_traits<Iter>::difference_type;
		using value_type = typename mm::iterator_traits<Iter>::value_type;
		using pointer = typename mm::iterator_traits<Iter>::pointer;
		using reference = typename mm::iterator_traits<Iter>::reference;
		using iterator_category = typename mm::iterator_traits<Iter>::iterator_category;

	private:
		iterator_type m_current;

	public:
		reverse_iterator() : m_current() {}
		explicit reverse_iterator(iterator_type it) : m_current(it) {}
		
		template <class U>
		explicit reverse_iterator(const reverse_iterator<U>& other) : m_current(other.base()) {}

		template <class U>
		reverse_iterator& operator=(const reverse_iterator<U>& other) {
			m_current = other.base();
			return *this;
		}

		iterator_type base() const {
			return m_current;
		}

		reference operator*() const {
			Iter tmp = m_current;
			return *--tmp;
		}

		pointer operator->() const {
			return mm::address_of(operator*());
		}

		reference operator[](difference_type n) const {
			return m_current[-n + 1];
		}

		reverse_iterator& operator++() {
			--m_current;
			return *this;
		}

		reverse_iterator& operator--() {
			++m_current;
			return *this;
		}

		reverse_iterator operator++(int) const {
			return reverse_iterator(m_current - 1);
		}

		reverse_iterator operator--(int) const {
			return reverse_iterator(m_current + 1);
		}

		reverse_iterator operator+(difference_type n) {
			return reverse_iterator(m_current - n);
		}

		reverse_iterator operator-(difference_type n) {
			return reverse_iterator(m_current + n);
		}

		reverse_iterator& operator+=(difference_type n) {
			m_current -= n;
			return *this;
		}

		reverse_iterator& operator-=(difference_type n) {
			m_current += n;
			return *this;
		}
	};

	template <class Iter1,class Iter2>
	bool operator==(const mm::reverse_iterator<Iter1>& lhs,const mm::reverse_iterator<Iter2>& rhs) {
		return lhs.base() == rhs.base();
	}

	template <class Iter1,class Iter2>
	bool operator!=(const mm::reverse_iterator<Iter1>& lhs,const mm::reverse_iterator<Iter2>& rhs) {
		return lhs.base() != rhs.base();
	}

	template <class Iter1,class Iter2>
	bool operator>(const mm::reverse_iterator<Iter1>& lhs,const mm::reverse_iterator<Iter2>& rhs) {
		return lhs.base() > rhs.base();
	}

	template <class Iter1,class Iter2>
	bool operator<(const mm::reverse_iterator<Iter1>& lhs,const mm::reverse_iterator<Iter2>& rhs) {
		return lhs.base() < rhs.base();
	}

	template <class Iter1,class Iter2>
	bool operator>=(const mm::reverse_iterator<Iter1>& lhs,const mm::reverse_iterator<Iter2>& rhs) {
		return lhs.base() >= rhs.base();
	}

	template <class Iter1,class Iter2>
	bool operator<=(const mm::reverse_iterator<Iter1>& lhs,const mm::reverse_iterator<Iter2>& rhs) {
		return lhs.base() <= rhs.base();
	}

	template <class Iter>
	mm::reverse_iterator<Iter> operator+(typename mm::reverse_iterator<Iter>::difference_type n,const mm::reverse_iterator<Iter>& it) {
		return mm::reverse_iterator<Iter>(it.base() - n);
	}

	template <class Iter>
	mm::reverse_iterator<Iter> operator-(typename mm::reverse_iterator<Iter>::difference_type n,const mm::reverse_iterator<Iter>& it) {
		return mm::reverse_iterator<Iter>(it.base() + n);
	}

	template <class Iter>
	mm::reverse_iterator<Iter> make_reverse_iterator(Iter it) {
		return mm::reverse_iterator<Iter>(it);
	}

	template <class Iter>
	class move_iterator {
	public:
		using iterator_type = Iter;
		using difference_type = typename mm::iterator_traits<Iter>::difference_type;
		using value_type = typename mm::iterator_traits<Iter>::value_type;
		using pointer = typename mm::iterator_traits<Iter>::pointer;
		using reference = value_type&&;
		using iterator_category = typename mm::iterator_traits<Iter>::iterator_category;

	private:
		iterator_type m_current;

	public:
		move_iterator() : m_current() {}
		explicit move_iterator(iterator_type it) : m_current(it) {}
		
		template <class U>
		explicit move_iterator(const move_iterator<U>& other) : m_current(other.base()) {}

		template <class U>
		move_iterator& operator=(const move_iterator<U>& other) {
			m_current = other.base();
			return *this;
		}

		iterator_type base() {
			return m_current;
		}

		reference operator*() const {
			return mm::move(*m_current);
		}

		reference operator[](difference_type n) const {
			return mm::move(*m_current[n]);
		}

		move_iterator& operator++() {
			++m_current;
			return *this;
		}

		move_iterator& operator--() {
			--m_current;
			return *this;
		}

		move_iterator operator++(int) const {
			return move_iterator(m_current + 1);
		}

		move_iterator operator--(int) const {
			return move_iterator(m_current - 1);
		}

		move_iterator operator+(difference_type n) const {
			return move_iterator(m_current + n);
		}

		move_iterator operator-(difference_type n) const {
			return move_iterator(m_current - n);
		}

		move_iterator& operator+=(difference_type n) {
			m_current += n;
			return *this;
		}

		move_iterator& operator-=(difference_type n) {
			m_current -= n;
			return *this;
		}
	};

	template <class Iter1,class Iter2>
	bool operator==(const mm::move_iterator<Iter1>& lhs,const mm::move_iterator<Iter2>& rhs) {
		return lhs.base() == rhs.base();
	}

	template <class Iter1,class Iter2>
	bool operator!=(const mm::move_iterator<Iter1>& lhs,const mm::move_iterator<Iter2>& rhs) {
		return lhs.base() != rhs.base();
	}

	template <class Iter1,class Iter2>
	bool operator>(const mm::move_iterator<Iter1>& lhs,const mm::move_iterator<Iter2>& rhs) {
		return lhs.base() > rhs.base();
	}

	template <class Iter1,class Iter2>
	bool operator<(const mm::move_iterator<Iter1>& lhs,const mm::move_iterator<Iter2>& rhs) {
		return lhs.base() < rhs.base();
	}

	template <class Iter1,class Iter2>
	bool operator>=(const mm::move_iterator<Iter1>& lhs,const mm::move_iterator<Iter2>& rhs) {
		return lhs.base() >= rhs.base();
	}

	template <class Iter1,class Iter2>
	bool operator<=(const mm::move_iterator<Iter1>& lhs,const mm::move_iterator<Iter2>& rhs) {
		return lhs.base() <= rhs.base();
	}

	template <class Iter>
	mm::move_iterator<Iter> operator+(typename mm::move_iterator<Iter>::difference_type n,const mm::move_iterator<Iter>& it) {
		return mm::move_iterator<Iter>(it.base() + n);
	}

	template <class Iter>
	mm::move_iterator<Iter> operator-(typename mm::move_iterator<Iter>::difference_type n,const mm::move_iterator<Iter>& it) {
		return mm::move_iterator<Iter>(it.base() - n);
	}

	template <class Iter>
	mm::move_iterator<Iter> make_move_iterator(Iter it) {
		return mm::move_iterator<Iter>(it);
	}

	template <class Container>
	class back_insert_iterator {
	public:
		using difference_type = void;
		using value_type = void;
		using pointer = void;
		using reference = void;
		using iterator_category = mm::output_iterator_tag;
		using container_type = Container;

	private:
		container_type m_container;

	public:
		back_insert_iterator() : m_container(nullptr) {}
		back_insert_iterator(const container_type& c) : m_container(mm::address_of(c)) {}

		
	};
}

#endif
