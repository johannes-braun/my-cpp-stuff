#pragma once
#include <algorithm>

#if __has_include(<execution>)
#include <execution>
#define MPP_PAR_UNSEQ_FOR_RANGE(IBeg, IEnd, Fun) std::for_each(std::execution::par_unseq, detail::count_iter<decltype(IBeg)>(IBeg), detail::count_iter<decltype(IEnd)>(IEnd), Fun);
#elif defined(_OPENMP)
#if _MSC_VER
#define INLINE_PRAGMA(x) __pragma(x)
#else
#define INLINE_PRAGMA(x) _Pragma(x)
#endif
#define MPP_PAR_UNSEQ_FOR_RANGE(IBeg, IEnd, Fun) { using ty = long long; INLINE_PRAGMA("omp parallel for schedule(dynamic)") for(ty i = ty(IBeg); i < ty(IEnd); ++i) Fun(decltype(IBeg)(i)); }
#endif

namespace mpp
{
    namespace detail
    {
        template<typename Int>
        class count_iter
        {
        public:
            using value_type = Int;
            using difference_type = Int;
            using iterator_category = std::random_access_iterator_tag;
            using pointer = value_type;
            using reference = value_type;

            count_iter() : _val(std::numeric_limits<Int>::max()) {}
            count_iter(value_type val) : _val(val) {}
            count_iter operator++() noexcept { const auto old = *this; _val++; return old; }
            count_iter& operator++(int) noexcept { _val++; return *this; }
            count_iter operator--() noexcept { const auto old = *this; _val--; return old; }
            count_iter& operator--(int) noexcept { _val--; return *this; }
            count_iter operator+(difference_type diff) const noexcept { return count_iter(_val + diff); }
            count_iter operator-(difference_type diff) const noexcept { return count_iter(_val - diff); }
            difference_type operator-(count_iter other) const noexcept { return _val - other._val; }
            count_iter& operator+=(difference_type diff) noexcept { _val += diff; return *this; }
            count_iter& operator-=(difference_type diff) noexcept { _val -= diff; return *this; }

            bool operator==(const count_iter& other) const noexcept { return _val == other._val; }
            bool operator!=(const count_iter& other) const noexcept { return _val != other._val; }

            value_type operator*() const noexcept { return _val; }

        private:
            value_type _val;
        };
    }

    template<typename Int, typename Fun>
    void for_n(Int n, Fun&& fun)
    {
        static_assert(std::is_integral_v<Int>, "Given value is not an integral type.");
        MPP_PAR_UNSEQ_FOR_RANGE(Int(0), n, std::forward<Fun>(fun));
    }

    template<typename Int, typename Fun>
    void for_range(Int begin, Int end, Fun&& fun)
    {
        static_assert(std::is_integral_v<Int>, "Given value is not an integral type.");
        MPP_PAR_UNSEQ_FOR_RANGE(begin, end, std::forward<Fun>(fun));
    }
}

#ifdef MPP_PAR_UNSEQ_FOR_RANGE
#undef MPP_PAR_UNSEQ_FOR_RANGE
#endif // MPP_PAR_UNSEQ_FOR_RANGE

#ifdef INLINE_PRAGMA
#undef INLINE_PRAGMA
#endif // INLINE_PRAGMA