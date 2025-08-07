#ifndef _MATRIX_H
#define _MATRIX_H

int CreateMatrix( double*** pTab, int nDim );   //!!! zwraca  0  gdy error  1 - ok
void DeleteMatrix( double*** pTab, int nDim );  // *pTab=NULL

//transponowanie macierzy
void TransMatrix( double** pTab, int nDim ); // bez pomocniczej, 

// macierz odwrotna, na we obliczony wczesniej w main() det  (tu w funkcji juz na pewno musi byc det!=0)
void InverseMatrix( double** pInv, double** pTab, int nDim, double det );

// wyznacznik rekurencyjnie 
double Det( double** pTab, int nDim ); //rozwiniecie wzgl 0-go wiersza 
// dla 2x2 uzyc znanego wzoru - mozna zaindeksowac

// drukuje macierz 
void PrintMatrix( double** pTab, int nDim );




#endif 