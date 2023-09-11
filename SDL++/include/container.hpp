#ifndef container_hpp_
#define container_hpp_
#pragma once

#include <type_traits>

namespace SDL
{
	template <typename t, typename T>
	struct ContinuousContainer_traits
	{
		template <typename T>
		static auto has_data(T x) -> decltype((t*)x.data(), std::true_type{});
		static auto has_data(...) -> std::false_type;

		template <typename T>
		static auto has_size(T x) -> decltype((size_t)x.size(), std::true_type{});
		static auto has_size(...) -> std::false_type;

		/// <summary>
		/// This trait is used to check if a type is a container with a .data() and .size() method.
		/// It can match std::vector, std::array, etc.
		/// </summary>
		/// <example> This sample shows how to use is_continuous_container.
		/// <code>
		/// template &lt;typename T, typename = std::enable_if_t&lt;ContinuousContainer_traits&lt;int, T&gt;::is_continuous_container&gt;&gt;
		/// int sum()
		/// {
		///     const int* ptr = numbers.data();
		///     const size_t size = numbers.size();
		///     int sum = 0;
		///     for (size_t i = 0; i &lt; size; i++)
		///     {
		///         sum += ptr[i];
		///     }
		///     return sum;
		/// }
		/// </code>
		/// </example>
		/// <typeparam name="t">The type this container stores.</typeparam>
		/// <typeparam name="T">The type to verify as a container.</typeparam>
		static constexpr bool is_continuous_container = decltype(has_data(std::declval<T>()))::value && decltype(has_size(std::declval<T>()))::value;
	};
}

#endif