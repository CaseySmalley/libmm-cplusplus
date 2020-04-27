#ifndef MM_OPTIONAL_HPP
#define MM_OPTIONAL_HPP
#include "mm/error.hpp"
#include "mm/utility.hpp"

namespace mm {
	struct nullopt_t {};
	static constexpr nullopt_t nullopt;

	template <class T>
	class optional {
	private:
		bool initialized;

		union {
			struct {} empty {};
			T value;
		};

		T& get_value() {
			ASSERT(initialized,ERROR_UNINITIALIZED_OPTIONAL,"tried to access uninitialized optional");
			return value;
		}

	public:
		optional() = delete;
		optional(nullopt_t) : initialized{false} {}
		optional(const T& t) : initialized{true}, value(t) {}
		optional(T&& t) : initialized{true}, value(t) {}
		optional(const optional<T>& o) : initialized{o.initialized}, value(o.value) {}
		optional(optional<T>&& o) : initialized{o.initialized}, value(move(o.value)) {}

		optional<T>& operator=(const optional<T>&) = delete;
		optional<T>& operator=(optional<T>&&) = delete;
		operator bool() { return initialized; }
		T& operator*() { return get_value(); }
		T* operator->() { return address_of(get_value()); }
		T&& value() { return get_value(); }
		
		template <class U>
		T&& value_or(U&& default_value) { return initialized ? value : forward(default_value); }
	};

	template <class T,class U,class... Args>
	optional<T> make_optional(Args&& args...) {
		return optional<T>(U(forward<Args>(args)...));
	}
}

#endif
