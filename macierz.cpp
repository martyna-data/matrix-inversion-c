// macierz.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include<math.h>
#include "matrix.h"
#define DEBUG
#define EPS 1e-12

void ReadData( FILE* fin, double** pMatrix, int nDim );

int main( int argc, char* argv[ ] )
{
  if( argc != 2 ) 
  {
    printf( "Usage: %s <input_file>\n", argv[ 0 ] );  
    return 1;
  }

  FILE* fin = NULL;
  // Próba otwarcia pliku w trybie tekstowym do odczytu
  if( (fin = fopen( argv[ 1 ], "rt" )) == NULL ) 
  {
    perror( "Error opening input file" );
    return -1;
  }

  //odczyt wymiaru macierzy
  int nDim = 0;
  if( fscanf( fin, "%d", &nDim ) != 1 || nDim <= 0 ) 
  {
    printf( "B³¹d: nieprawid³owy wymiar macierzy\n" );
    fclose( fin );
    return -2;
  }
#ifdef DEBUG

  printf( "Odczytano wymiar macierzy: %d\n", nDim );

#endif 
  
  double** pMatrix = NULL;
  // Tworzymy macierz uk³adu nDim x nDim
  if( !CreateMatrix( &pMatrix, nDim ) ) 
  {
    printf( "B³¹d alokacji pamiêci dla macierzy\n" );
    fclose( fin );
    return -1;
  }

  // Wczytujemy dane do macierzy
  ReadData( fin, pMatrix, nDim );
  fclose( fin );  // <--- zamykamy plik po odczycie danych
  fin = NULL;   

#ifdef DEBUG
  printf( "Wczytana macierz:\n" );
  PrintMatrix( pMatrix, nDim );
#endif

  // obliczamy wyznacznik
  double det = Det( pMatrix, nDim );

#ifdef DEBUG
  // wydruk kontrolny
  printf( "Wyznacznik macierzy wynosi: %f\n", det );
#endif

  if( fabs(det)<EPS )
  {
   printf( "Macierz osobliwa-wyznacznik rowny 0\n" );
   return -4;

  }

#ifdef DEBUG
//transpozycja na kopii
  double** pCopy = NULL;
  CreateMatrix( &pCopy, nDim );
  for( int i = 0; i < nDim; i++ )
    for( int j = 0; j < nDim; j++ )
      pCopy[ i ][ j ] = pMatrix[ i ][ j ];

  TransMatrix( pCopy, nDim );
  printf( "Macierz po transpozycji:\n" );
  PrintMatrix( pCopy, nDim );
  DeleteMatrix( &pCopy, nDim );

#endif

  double** pInverse = NULL;
  if( !CreateMatrix( &pInverse, nDim ) ) {
    printf( "B³¹d alokacji pamiêci dla macierzy odwrotnej\n" );
    DeleteMatrix( &pMatrix, nDim );
    return -1;
  }

  // Obliczamy macierz odwrotn¹ 
  InverseMatrix( pInverse, pMatrix, nDim, det );

#ifdef DEBUG
  printf( "Macierz odwrotna:\n" );
  PrintMatrix( pInverse, nDim );
#endif

  // Zwolniæ pamiêæ obu macierzy
  DeleteMatrix( &pMatrix, nDim );
  //pMatrix = NULL;

  DeleteMatrix( &pInverse, nDim );
  //pInverse = NULL;

   return 0;

}
 

void ReadData( FILE* fin, double** pMatrix, int nDim )
{
  for( int i = 0; i < nDim; i++ )
  {
    double* v = *pMatrix++;
    for( int j = 0; j < nDim; j++ )
    {
      if( fscanf( fin, "%lf", v++) != 1 )
      {
        printf( "Blad odczytu elementu macierzy na pozycji [%d][%d]\n", i, j );
        return;
      }
    }
  }
}

  
