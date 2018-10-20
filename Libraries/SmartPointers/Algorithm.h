#pragma once
// Implementations for Various Useful Algorithms
// Present in C++ to be used with SmartPtr class

// Taken from Visual Studio

// Verify if it is of the type Array

#include <stddef.h>
#include <stdint.h>

template <class _Ty>
struct is_array
{
   static constexpr const bool value = false;
};
template <class _Ty, size_t _Nx>
struct is_array<_Ty[_Nx]>
{
   static constexpr const bool value = true;
};
template <class _Ty>
struct is_array<_Ty[]>
{
   static constexpr const bool value = true;
};

template <class _Ty>
constexpr const bool is_array_v = is_array<_Ty>::value;

// Get Raw Type of Array
template <class _Ty>
struct underlying_array_type
{
   // Is not an array
   using type = _Ty;
};
template <class _Ty, size_t _Nx>
struct underlying_array_type<_Ty[_Nx]>
{
   using type = _Ty;
};
template <class _Ty>
struct underlying_array_type<_Ty[]>
{
   using type = _Ty;
};
// This will return
//	is_same<int, underlying_array_type_t<int[]>>
template <class _Ty>
using underlying_array_type_t = typename underlying_array_type<_Ty>::type;

// STRUCT TEMPLATE enable_if
template <bool _Test, class _Ty = void>
struct enable_if
{ // type is undefined for assumed !_Test
};

template <class _Ty>
struct enable_if<true, _Ty>
{ // type is _Ty for _Test
   using type = _Ty;
};

template <bool _Test, class _Ty = void>
using enable_if_t = typename enable_if<_Test, _Ty>::type;