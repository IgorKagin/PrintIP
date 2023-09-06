#include <stdio.h>
#include <string>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <type_traits>
#include <list>
#include <tuple>

template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
void printIP(const T& integral) 
{
  const unsigned countOfBytes = sizeof( T );
  auto offset = 56;
  const auto step_offset = 8;
  uint8_t temp;
  for ( unsigned i = 0; i < countOfBytes; ++i )
  {
    if ( i != 0 )
      std::wcout << ".";
    temp = ( integral >> offset ) & 0xFF;
    std::wcout << temp;
    offset -= step_offset;
  }
  std::wcout << std::endl;
}

template<typename T, std::enable_if_t<std::is_same_v<T,std::string>, bool> = true>
void printIP( const T& string ) 
{
  std::cout << string << std::endl;
}

template<template<typename, typename> typename T, typename Type, typename Allocator = std::allocator<Type>, 
                  std::enable_if_t<std::is_same_v<T<Type, Allocator>, std::vector<Type>> || std::is_same_v<T<Type, Allocator>, std::list<Type>>, bool> = true >
void printIP( const T<Type, Allocator>& container )
{
  for ( auto iter = std::begin( container ); iter != std::end( container ); ++iter )
  {
    if ( iter != std::begin( container ) )
      std::cout << ".";
    std::cout << *iter;
  }
  std::cout << std::endl;
}

template< size_t N, typename ...ValueTypes>
struct is_all
{
  using myTuple = std::tuple<ValueTypes... >;
  static const bool value = std::is_same_v<std::tuple_element_t<0, myTuple>, std::tuple_element_t<1, myTuple>> && is_all<N - 2, ValueTypes...>::value;
};

template<typename ...ValueTypes>
struct is_all<0, ValueTypes...>
{
  static const bool value = true;
};

template<template<typename...> typename T, typename ...ValueTypes,
  std::enable_if_t<std::is_same_v<T<ValueTypes...>, std::tuple<ValueTypes...>> && is_all<std::tuple_size<T<ValueTypes...>>::value, ValueTypes...>::value, bool > = true >/*&& is_all<4, ValueTypes...>::value>*/
  void printIP( const T<ValueTypes...>& tuple )
{
  std::cout << std::get<0>( tuple ) << ".";
  std::cout << std::get<1>( tuple ) << ".";
  std::cout << std::get<2>( tuple ) << ".";
  std::cout << std::get<3>( tuple ) << std::endl;
}

int main(int, char**)
{
  
    printIP( int8_t{ -1 } ); // 255
    printIP( int16_t{ 0 } ); // 0.0
    printIP( int32_t{ 2130706433 } ); // 127.0.0.1
    printIP( int64_t{ 8875824491850138409 } ); // 123.45.67.89.101.112.131.41
    printIP( std::string{"Hello, World!"} ); // Hello, World!
    printIP( std::vector<int>{ 100, 200, 300, 400 } ); // 100.200.300.400
    printIP( std::list<short>{ 400, 300, 200, 100 } ); // 400.300.200.100 
    printIP( std::make_tuple( 123, 456, 789, 0 )); // 123.456.789.0 
  

    return 0;
}
