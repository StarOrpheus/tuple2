#pragma once

#include <typeindex>

namespace tuple2_impl_
{
    template<typename IndexSequence, typename... Ts>
    struct tuple2_impl;
}

template<typename... Ts>
struct tuple2 final
    : tuple2_impl_::tuple2_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...>
{
    using ImplType = tuple2_impl_::tuple2_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...>;
    using ImplType::ImplType;

    tuple2() = default;
};

template<size_t Ind, typename... Ts>
decltype(auto) get(tuple2<Ts...>& t)
{
    return tuple2_impl_::get<Ind>(t);
}

template<size_t Ind, typename... Ts>
decltype(auto) get(tuple2<Ts...> const& t)
{
    return tuple2_impl_::get<Ind>(t);
}

template<typename Ind, typename... Ts>
decltype(auto) get(tuple2<Ts...>& t)
{
    return tuple2_impl_::get<Ind>(t);
}

template<typename Ind, typename... Ts>
decltype(auto) get(tuple2<Ts...> const& t)
{
    return tuple2_impl_::get<Ind>(t);
}

namespace tuple2_impl_
{
    template <typename T, std::size_t I>
    struct indexed
    {
        using type = T;
    };

    template<typename T, size_t I>
    struct indexed_elem_storage
        : indexed<T, I>
    {
        indexed_elem_storage() = default;

        indexed_elem_storage(T const& other)
            : holder(std::forward<T const&>(other))
        {}

        indexed_elem_storage(T&& other)
            : holder(std::forward<T&&>(other))
        {}

        T& get() { return holder; }
        T const& get() const { return holder; }

    private:
        T holder;
    };

    template <typename Is, typename ...Ts>
    struct indexer;

    template <std::size_t ...Is, typename ...Ts>
    struct indexer<std::index_sequence<Is...>, Ts...>
        : indexed<Ts, Is>...
    { };

    template <std::size_t I, typename T>
    static indexed<T, I> select(indexed<T, I>);

    template <std::size_t I, typename ...Ts>
    using nth_element = typename decltype(select<I>(
        indexer<std::index_sequence_for<Ts...>, Ts...>{}
    ))::type;

    template<typename T, typename Head, typename... Ts>
    struct find_index
    {
        constexpr static size_t value = find_index<T, Ts...>::value + 1;
    };

    template<typename T, typename... Ts>
    struct find_index<T, T, Ts...>
    {
        constexpr static size_t value = 0;
    };

    template<typename T, typename Head, typename... Ts>
    struct count_occurences
    {
        constexpr static size_t value = count_occurences<T, Ts...>::value;
    };

    template<typename T, typename... Ts>
    struct count_occurences<T, T, Ts...>
    {
        constexpr static size_t value = 1 + count_occurences<T, Ts...>::value;
    };

    template<typename T, typename Head>
    struct count_occurences<T, Head>
    {
        constexpr static size_t value = 0;
    };

    template<typename T>
    struct count_occurences<T, T>
    {
        constexpr static size_t value = 1;
    };

    // template<typename T, typename... Ts, typename Ok>
    // struct find_the_only_one_index
    // {};

    template<typename... Ts, size_t... I>
    struct tuple2_impl<std::index_sequence<I...>, Ts...>
        : indexed_elem_storage<Ts, I>...
    {
        using Type = tuple2_impl<std::index_sequence<I...>, Ts...>;

        tuple2_impl() = default;

        tuple2_impl(Type const& other)
            : indexed_elem_storage<Ts, I>(static_cast<indexed_elem_storage<Ts, I> const&>(other))...
        {}

        tuple2_impl(Type&& other) noexcept
            : indexed_elem_storage<Ts, I>(static_cast<indexed_elem_storage<Ts, I>&&>(std::move(other)))...
        {}

        tuple2_impl(Ts... args)
            : indexed_elem_storage<Ts, I>(std::forward<Ts>(args))...
        {}

        Type& operator=(Type const& other) = default;
        Type& operator=(Type&& other) = default;

        template<size_t Ind>
        friend decltype(auto) get(tuple2<Ts...>& t)
        {
            using elem_type = nth_element<Ind, Ts...>;
            return static_cast<indexed_elem_storage<elem_type, Ind>&>(t).get();
        }

        template<size_t Ind>
        friend decltype(auto) get(tuple2<Ts...> const& t)
        {
            using elem_type = nth_element<Ind, Ts...>;
            return static_cast<indexed_elem_storage<elem_type, Ind> const&>(t).get();
        }

        template<typename T,
                 typename T2 = typename std::enable_if<count_occurences<T, Ts...>::value == 1>::type,
                 size_t I = find_index<T, Ts...>::value>
        friend T& get(tuple2<Ts...>& t)
        {
            using elem_type = nth_element<I, Ts...>;
            return static_cast<indexed_elem_storage<elem_type, I> &>(t).get();
        }

        template<typename T,
                 typename T2 = typename std::enable_if<count_occurences<T, Ts...>::value == 1>::type,
                 size_t I = find_index<T, Ts...>::value>
        friend T const& get(tuple2<Ts...> const& t)
        {
            using elem_type = nth_element<I, Ts...>;
            return static_cast<indexed_elem_storage<elem_type, I> const&>(t).get();
        }
    };
}