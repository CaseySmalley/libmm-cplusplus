#ifndef MM_ARRAY_HPP
#define MM_ARRAY_HPP
#include "mm/utility.hpp"
#include "mm/iterator_traits.hpp"

namespace mm {
	template <class T,size_t Length>
	class array {
	private:
		T m_arr[Length];

	public:
		template <class... Args>
		explicit array(Args&&... args) : m_arr{ move(args)... } {}

		array() = default;
		array(const array<T,Length>&) = default;
		array(array<T,Length>&&) = default;
		array<T,Length>& operator=(const array<T,Length>&) = default;
		array<T,Length>& operator=(array<T,Length>&&) = default;
		~array() = default;

		using value_type = T;
		using size_type = size_t;
		using difference_type = ptrdiff_t;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;
		using iterator = pointer;
		using const_iterator = const T*;
		using reverse_iterator = mm::reverse_iterator<iterator>;
		using const_reverse_iterator = mm::reverse_iterator<const_iterator>;

		reference operator[](size_type i) { return m_arr[i]; }
		const_reference operator[](size_type i) const { return m_arr[i]; }

		reference front() { return m_arr[0]; }
		const_reference front() const { return m_arr[Length - 1]; }

		reference back() { return m_arr[Length - 1]; }
		const_reference back() const { return m_arr[Length - 1]; }

		pointer data() { return m_arr; }
		const_pointer data() const { return m_arr; }

		iterator begin() { return m_arr; }
		const_iterator cbegin() const { return m_arr; }

		iterator end() { return m_arr + Length; }
		const_iterator cend() { return m_arr + Length; }

		reverse_iterator rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator crbegin() { return reverse_iterator(end()); }

		reverse_iterator rend() { return reverse_iterator(begin()); }
		const_reverse_iterator crend() { return reverse_iterator(begin()); }

		bool empty() { return Length == 0; }
		size_type size() { return Length; }
		size_type max_size() { return Length; }

		void fill(const T& v) {
			for (size_type i = 0; i < Length; ++i) {
				m_arr[i] = v;
			}
		}
	};

	template <class T,class... Args>
	array<T,sizeof...(Args)> make_array(Args&&... args) {
		return array<T,sizeof...(Args)>(forward<T>(args)...);
	}
}

#endif
