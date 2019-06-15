#pragma once
#include <algorithm>
#include <execution>

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

    template<typename Int, typename Policy, typename Fun>
    void for_n(Policy policy, Int n, Fun&& fun)
    {
        static_assert(std::is_integral_v<Int>, "Given value is not an integral type.");
        std::for_each(policy, detail::count_iter(Int(0)), detail::count_iter(n), std::forward<Fun>(fun));
    }

    template<typename Int, typename Policy, typename Fun>
    void for_range(Policy policy, Int begin, Int end, Fun&& fun)
    {
        static_assert(std::is_integral_v<Int>, "Given value is not an integral type.");
        std::for_each(policy, detail::count_iter(begin), detail::count_iter(end), std::forward<Fun>(fun));
    }

    template<typename InputIterator, typename OutputIterator, typename Predicate, typename TransformFunc>
    OutputIterator transform_if(
        InputIterator&& begin,
        InputIterator&& end,
        OutputIterator&& out,
        Predicate&& predicate,
        TransformFunc&& transformer
    ) {
        for (; begin != end; ++begin, ++out) {
            if (predicate(*begin))
                * out = transformer(*begin);
        }
        return out;
    }
}