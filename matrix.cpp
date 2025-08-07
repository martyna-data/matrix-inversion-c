#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nDim );
// oblicza macierz dopelnien pTabD na podstawie tablic wejsc pTab
void ComplMatrix( double** pTabD, double** pTab, int nDim ); //macierz dopelnien


int CreateMatrix( double*** pTab, int nDim ) 
{
  *pTab = (double**)malloc( nDim * sizeof( double* ) ); // Alokacja tablicy wska�nik�w
  if( *pTab == NULL )
  {
    //printf( "Memory allocation error" );
    return 0;
  }
  memset( *pTab, 0, nDim * sizeof( double* ) );

  for( int i = 0; i < nDim; i++ )
  {
    (*pTab)[ i ] = (double*)malloc( nDim * sizeof( double ) ); // Alokacja dla wiersza
    if( (*pTab)[ i ] == NULL )
    {
      // Zwolnienie wcze�niej zaalokowanej pami�ci
      for( int j = 0; j < i; j++ )
      {
        free( (*pTab)[ j ] );
      }
      free( *pTab );
      *pTab = NULL;
      return 0;
    }

    // R�czne zerowanie pami�ci
    memset( (*pTab)[ i ], 0, nDim * sizeof( double ) );
  }

  return 1;
}   

void DeleteMatrix( double*** pTab, int nDim )
{
  if( !pTab || !(*pTab) || nDim<1 ) return; //Spr, czy wska�nik do tablicy i tablica nie s� puste
  
  // Zwalnianie pami�ci ka�dego wiersza
  for( int i = 0; i < nDim; i++ ) 
  {
    free( (*pTab)[ i ] ); // Zwalniamy wiersz i-tej kolumny
  }
  //zwalniamy tablic� wska�nik�w (do wierszy)
  free( *pTab );
  *pTab = NULL;
}

//transponowanie macierzy
void TransMatrix( double** pTab, int nDim )
{
  if( !pTab ) return;
  for( int i = 0; i < nDim - 1; i++ ) {
    for( int j = i + 1; j < nDim; j++ ) {
      // Zamiana pTab[i][j] z pTab[j][i]
      double temp = pTab[ i ][ j ];
      pTab[ i ][ j ] = pTab[ j ][ i ];
      pTab[ j ][ i ] = temp;
    }
  }
}

// macierz odwrotna, na we obliczony wczesniej w main() det  (tu w funkcji juz na pewno musi byc det!=0)
void InverseMatrix( double** pInv, double** pTab, int nDim, double det )
{
  if( !pTab || !pInv ) return;
  // Obliczamy macierz kofaktor�w
  ComplMatrix( pInv, pTab, nDim );

  // Transponujemy j�, aby uzyska� macierz dope�nie� algebraicznych 
  TransMatrix( pInv, nDim );

  // Dzielimy ka�dy element przez wyznacznik, uzyskuj�c macierz odwrotn�
  for( int i = 0; i < nDim; i++ ) {
    for( int j = 0; j < nDim; j++ ) {
      pInv[ i ][ j ] /= det;
    }
  }
}

// wyznacznik rekurencyjnie 
double Det( double** pTab, int nDim )
{
  if( !pTab ) return 0;
  if( nDim == 1 ) return pTab[ 0 ][ 0 ];

  if( nDim == 2 ) {
    return pTab[ 0 ][ 0 ] * pTab[ 1 ][ 1 ] - pTab[ 0 ][ 1 ] * pTab[ 1 ][ 0 ];
  }
  //macierz pomocnicza do dope�nie�
  double** pTemp = NULL;
  if( !CreateMatrix( &pTemp, nDim - 1 ) ) {
    printf( "B��d w tworzeniu pTemp, det()\n" );
    return 0;
  }

  double det = 0;
  double sign = 1;

  for( int i = 0; i < nDim; i++ ) 
  {
    Complement( pTemp, pTab, 0, i, nDim );               // Budujemy dope�nienie dla elementu (0, i)
    det += pTab[ 0 ][ i ] * Det( pTemp, nDim - 1 ) * sign;   // Rekurencyjne obliczenie det
    sign = -sign;                                      // Zmiana znaku (+ - + - ...)
  }

  DeleteMatrix( &pTemp, nDim - 1 );
  return det;
}

void PrintMatrix( double** pTab, int nDim )
{
  if( !pTab ) return;

  for( int i = 0; i < nDim; i++ ) 
  {
    double* v = *pTab++;
    for( int j = 0; j < nDim; j++ ) 
    {
      printf( "%.2f ", *v++ );   // Wypisz element z wiersza i i kolumny j
     
    }
    printf( "\n" );  // Nowa linia po ka�dym wierszu
  }
}

//macierz dope�nie�
void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nDim )
{
  if( !pTabO || !pTabI || nRow < 0 || nRow >= nDim 
    || nCol < 0 || nCol >= nDim ) return;

  int outRow = 0; // indeks wiersza macierzy wynikowej

  for( int i = 0; i < nDim; i++ ) {
    if( i == nRow ) continue; // Pomijamy wybrany wiersz

    int outCol = 0; // indeks kolumny macierzy wynikowej

    for( int j = 0; j < nDim; j++ ) {
      if( j == nCol ) continue; // Pomijamy wybran� kolumn�

      // Przepisujemy element z macierzy wej�ciowej
      // do macierzy wynikowej pTabO [outRow][outCol]
      pTabO[ outRow ][ outCol ] = pTabI[ i ][ j ]; 
      outCol++;
    }

    outRow++;
  }
}

// oblicza macierz dopelnien pTabD na podstawie tablic wejsc pTab
void ComplMatrix( double** pTabD, double** pTab, int nDim )
{
  if( !pTab || !pTabD ) return;
  double** pTabTemp = NULL;

  // Tworzymy tymczasow� macierz (nDim - 1) x (nDim - 1) do przechowywania dope�nie�
  if( !CreateMatrix( &pTabTemp, nDim - 1 ) ) {
    printf( "B��d w tworzeniu pTabTemp w ComplMatrix()\n" );
    return;
  }

  // Przechodzimy po wszystkich pozycjach macierzy
  for( int i = 0; i < nDim; i++ ) {
    double* row = pTabD[i];  // Wiersz macierzy wynikowej (dope�nie�)
    int sign = (i % 2) ? -1 : 1;               // Naprzemienny znak: + - + - ...

    for( int j = 0; j < nDim; j++ ) {
      Complement( pTabTemp, pTab, i, j, nDim );               // Usuwamy i-ty wiersz i j-t� kolumn�
      row[ j ] = sign * Det( pTabTemp, nDim - 1 );              // Obliczamy kofaktor i zapisujemy
      sign = -sign;
    }
  }

  DeleteMatrix( &pTabTemp, nDim - 1 ); // Zwolnienie pami�ci tymczasowej
}

