#pragma once
#include <RakX/common/constants.hpp>

namespace RakX {

	// Polly fill for std::observer_ptr
	template<NotAReference T>
	class AccessPtr
	{
	public:
		constexpr AccessPtr() = default;
		constexpr AccessPtr(T* p) : ptr(p) {};
		constexpr T& operator*() const {
			return *ptr;
		}

		constexpr T* operator->() const {
			return ptr;
		}

		constexpr operator T* () const {
			return ptr;
		}

		constexpr T* get() const {
			return ptr;
		}
        constexpr T& operator[](std::size_t index) const {
            return ptr[index];
        }

        constexpr AccessPtr& operator++() {
            ++ptr;
            return *this;
        }

        constexpr AccessPtr operator++(int) {
            AccessPtr temp = *this;
            ++ptr;
            return temp;
        }

        constexpr AccessPtr& operator--() {
            --ptr;
            return *this;
        }

        constexpr AccessPtr operator--(int) {
            AccessPtr temp = *this;
            --ptr;
            return temp;
        }

        constexpr AccessPtr& operator+=(std::size_t offset) {
            ptr += offset;
            return *this;
        }

        constexpr AccessPtr& operator-=(std::size_t offset) {
            ptr -= offset;
            return *this;
        }

        constexpr AccessPtr operator+(std::ptrdiff_t offset) const {
            return AccessPtr(ptr + offset);
        }

        constexpr AccessPtr operator-(std::ptrdiff_t offset) const {
            return AccessPtr(ptr - offset);
        }

        constexpr bool operator==(const AccessPtr& other) const {
            return ptr == other.ptr;
        }

        constexpr bool operator!=(const AccessPtr& other) const {
            return ptr != other.ptr;
        }
	private:
		T* ptr{nullptr};
	};
}