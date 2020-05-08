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

	template <class Container>
	class reverse_adapter {
	public:
		using container_type = Container;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;
		using difference_type = typename container_type::difference_type;
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using reverse_iterator = typename container_type::reverse_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;

	private:
		container_type& m_container;

	public:
		reverse_adapter() = delete;
		reverse_adapter(const reverse_adapter&) = delete;
		reverse_adapter(reverse_adapter&&) = delete;
		reverse_adapter& operator=(const reverse_adapter&) = delete;
		reverse_adapter& operator=(reverse_adapter&&) = delete;

		reverse_adapter(const container_type& c) : m_container(c) {}

		reverse_iterator begin() { return m_container.rbegin(); }
		const_reverse_iterator cbegin() const { return m_container.crbegin(); }

		reverse_iterator end() { return m_container.rend(); }
		const_reverse_iterator cend() const { return m_container.crend(); }

		iterator rbegin() { return m_container.begin(); }
		const_iterator crbegin() const { return m_container.cbegin(); }

		iterator rend() { return m_container.end(); }
		const_iterator crend() const { return m_container.cend(); }
	};

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
	class move_adapter {
	public:
		using container_type = Container;
		using value_type = typename container_type::value_type;
		using size_type = typename container_type::size_type;
		using difference_type = typename container_type::difference_type;
		using iterator = typename container_type::iterator;
		using const_iterator = typename container_type::const_iterator;
		using reverse_iterator = typename container_type::reverse_iterator;
		using const_reverse_iterator = typename container_type::const_reverse_iterator;

	private:
		container_type& m_container;

	public:
		move_adapter() = delete;
		move_adapter(const move_adapter&) = delete;
		move_adapter(move_adapter&&) = delete;
		move_adapter& operator=(const move_adapter&) = delete;
		move_adapter& operator=(move_adapter&&) = delete;

		move_adapter(const container_type& c) : m_container(c) {}

		reverse_iterator begin() { return mm::make_move_iterator(m_container.begin()); }
		const_reverse_iterator cbegin() const { return mm::make_move_iterator(m_container.cbegin()); }

		reverse_iterator end() { return mm::make_move_iterator(m_container.end()); }
		const_reverse_iterator cend() const { return mm::make_move_iterator(m_container.cend()); }

		iterator rbegin() { return mm::make_move_iterator(m_container.begin()); }
		const_iterator crbegin() const { return mm::make_move_iterator(m_container.crbegin()); }

		iterator rend() { return mm::make_move_iterator(m_container.end()); }
		const_iterator crend() const { return mm::make_move_iterator(m_container.crend()); }
	};

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

		back_insert_iterator& operator=(const typename container_type::value_type& value) {
			m_container->push_back(value);
			return *this;
		}

		back_insert_iterator& operator=(typename container_type::value_type&& value) {
			m_container->push_back(mm::move(value));
			return *this;
		}

		back_insert_iterator& operator*() {
			return *this;
		}

		back_insert_iterator& operator++() {
			return *this;
		}

		back_insert_iterator& operator--() {
			return *this;
		}
	};

	template <class Container>
	mm::back_insert_iterator<Container> back_inserter(Container& c) {
		return mm::back_insert_iterator<Container>(c);
	}

	template <class Container>
	class front_insert_iterator {
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
		front_insert_iterator() : m_container(nullptr) {}
		front_insert_iterator(const container_type& c) : m_container(mm::address_of(c)) {}

		front_insert_iterator& operator=(const typename container_type::value_type& value) {
			m_container->push_front(value);
			return *this;
		}

		front_insert_iterator& operator=(typename container_type::value_type&& value) {
			m_container->push_front(mm::move(value));
			return *this;
		}

		front_insert_iterator& operator*() {
			return *this;
		}

		front_insert_iterator& operator++() {
			return *this;
		}

		front_insert_iterator& operator--() {
			return *this;
		}
	};

	template <class Container>
	mm::front_insert_iterator<Container> front_inserter(Container& c) {
		return mm::front_insert_iterator<Container>(c);
	}

	template <class Container>
	class insert_iterator {
	public:
		using difference_type = void;
		using value_type = void;
		using pointer = void;
		using reference = void;
		using iterator_category = mm::output_iterator_tag;
		using container_type = Container;

	private:
		container_type* m_container;
		typename container_type::iterator m_it;

	public:
		insert_iterator(container_type& c,typename container_type::iterator it) : m_container(mm::address_of(c)), m_it(it) {}

		insert_iterator& operator=(const typename container_type::value_type& value) {
			m_container->insert(m_it,value);
			++m_it;
			return *this;
		}

		insert_iterator& operator=(typename container_type::value_type&& value) {
			m_container->insert(m_it,mm::move(value));
			++m_it;
			return *this;
		}

		insert_iterator& operator*() {
			return *this;
		}

		insert_iterator& operator++() {
			return *this;
		}

		insert_iterator& operator--() {
			return *this;
		}
	};

	template <class Container>
	mm::insert_iterator<Container> inserter(Container& c,typename Container::iterator it) {
		return mm::insert_iterator<Container>(c,it);
	}

	namespace detail {
		template <class Iter>
		void advance_impl(Iter& it,typename mm::iterator_traits<Iter>::difference_type n,mm::input_iterator_tag) {
			while (n > 0) {
				--n;
				++it;
			}
		}

		template <class Iter>
		void advance_impl(Iter& it,typename mm::iterator_traits<Iter>::difference_type n,mm::bidirectional_iterator_tag) {
			while (n > 0) {
				--n;
				++it;
			}

			while (n < 0) {
				++n;
				--it;
			}
		}

		template <class Iter>
		void advance_impl(Iter& it,typename mm::iterator_traits<Iter>::difference_type n,mm::random_access_iterator_tag) {
			it += n;
		}
	}

	template <class Iter,class Distance>
	void advance(Iter& it,Distance n) {
		detail::advance_impl<Iter>(
			it,
			typename mm::iterator_traits<Iter>::difference_type(n),
			mm::iterator_traits<Iter>::iterator_category
		);
	}

	namespace detail {
		template <class Iter>
		typename mm::iterator_traits<Iter>::difference_type distance_impl(Iter first,Iter last,mm::input_iterator_tag) {
			typename mm::iterator_traits<Iter>::difference_type n = 0;

			while(first != last) {
				++first;
				++n;
			}

			return n;
		}
		
		template <class Iter>
		typename mm::iterator_traits<Iter>::difference_type distance_impl(Iter first,Iter last,mm::random_access_iterator_tag) {
			return last - first;
		}
	}

	template <class Iter>
	typename mm::iterator_traits<Iter>::difference_type distance(Iter first,Iter last) {
		return detail::distance_impl<Iter>(
			first,
			last,
			mm::iterator_traits<Iter>::iterator_category
		);
	}

	template <class ForwardIter>
	ForwardIter next(ForwardIter it,typename mm::iterator_traits<ForwardIter>::difference_type n = 1) {
		mm::advance(it,n);
		return it;
	}

	template <class BidirIter>
	BidirIter prev(BidirIter it,typename mm::iterator_traits<BidirIter>::difference_type n = 1) {
		mm::advance(it,-n);
		return it;
	}

	template <class Container>
	auto begin(Container& c) -> decltype(c.begin()) {
		return c.begin();
	}

	template <class Container>
	auto cbegin(Container& c) -> decltype(c.cbegin()) {
		return c.cbegin();
	}

	template <class Container>
	auto end(Container& c) -> decltype(c.end()) {
		return c.end();
	}

	template <class Container>
	auto cend(Container& c) -> decltype(c.cend()) {
		return c.cend();
	}

	template <class Container>
	auto rbegin(Container& c) -> decltype(c.rbegin()) {
		return c.rbegin();
	}

	template <class Container>
	auto crbegin(Container& c) -> decltype(c.crbegin()) {
		return c.crbegin();
	}

	template <class Container>
	auto rend(Container& c) -> decltype(c.rend()) {
		return c.rend();
	}

	template <class Container>
	auto crend(Container& c) -> decltype(c.crend()) {
		return c.crend();
	}
}

#endif
