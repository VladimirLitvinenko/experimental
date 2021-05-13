/* 
 * File:   main.cpp
 * Author: litvinenko
 *
 * Created on 10 октября 2017 г., 6:15
 */

#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <future>
#include <iostream>
#include <chrono>
#include <iomanip>

template< typename Iterator_, typename Function_ >
void                for_each_parallel
( 
  Iterator_         first, 
  Iterator_         last, 
  Function_         f, 
  const std::size_t min_per_task = 25 
)
{
    auto const length( std::distance( first, last ) );
    
    if( length )
    {
        if( length < 2 * min_per_task )
        {
            std::for_each( first, last, f );
            return;
        }

        auto const mid_point = first + length / 2;
        auto first_half = std::async(   &for_each_parallel< Iterator_, Function_>, 
                                        first, 
                                        mid_point, 
                                        f,
                                        min_per_task );
        for_each_parallel< Iterator_, Function_>( mid_point, last, f, min_per_task );
    //    first_half.wait();
        first_half.get();
    }
}

/*
 * 
 */
int main(int , char** ) 
{
  std::setlocale(LC_ALL, "ru_RU.UTF-8");
  std::cout << "ВНИМАНИЕ, использовать, если порядок обработки независимых элементов не важен!" << std::endl;
  std::vector< int > v( 100000, 0 );
  using IterTypen = std::vector< int >::iterator;

  auto idx( 0 );
  auto start_time ( std::chrono::high_resolution_clock::now() );
  for_each_parallel< IterTypen, std::function< void( int& )> >
  ( 
    v.begin(),
    v.end(),
    [ &idx ]( int & val )
    {
      val = idx + (++idx);
    }
  );
  auto stop_time  ( std::chrono::high_resolution_clock::now()  );
  auto distance   ( std::chrono::duration_cast< std::chrono::microseconds>( stop_time - start_time ).count() );
  std::cout << "\tElapsed time : " << std::setfill(' ') << std::setw( 5 ) << distance << " microseconds." << std::endl;

//  idx = 0;
//  for( auto value : v )
//  {
//      std::cout << "v[ " << idx++ << " ] = " << value << std::endl;
//  }
//  std::cout << "ВНИМАНИЕ, использовать, если порядок обработки независимых элементов не важен!" << std::endl;
  return EXIT_SUCCESS;
}

