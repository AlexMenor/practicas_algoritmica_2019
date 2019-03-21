/**********************************************************************/
#ifndef FUNCIONES_DEL_PROFESOR
#define FUNCIONES_DEL_PROFESOR

#include <cstdlib>
#include <climits>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;
static int const MAX_LONG  = 10;
static int const UMBRAL_QS  = 50;

/**********************************************************************/	

void insercion_lims(int T[], int inicial, int final)
{
  int i, j;
  int aux;
  for (i = inicial + 1; i < final; i++) {
    j = i;
    while ((T[j] < T[j-1]) && (j > 0)) {
      aux = T[j];
      T[j] = T[j-1];
      T[j-1] = aux;
      j--;
    };
  };
}

void insercion (int T[], int nelem){
  insercion_lims (T, 0, nelem);
}

/**********************************************************************/	

void dividir_qs(int T[], int inicial, int final, int & pp)
{
  int pivote, aux;
  int k, l;

  pivote = T[inicial];
  k = inicial;
  l = final;
  do {
    k++;
  } while ((T[k] <= pivote) && (k < final-1));
  do {
    l--;
  } while (T[l] > pivote);
  while (k < l) {
    aux = T[k];
    T[k] = T[l];
    T[l] = aux;
    do k++; while (T[k] <= pivote);
    do l--; while (T[l] > pivote);
  };
  aux = T[inicial];
  T[inicial] = T[l];
  T[l] = aux;
  pp = l;
};

/**********************************************************************/	

void quicksort_lims(int T[], int inicial, int final)
{
  int k;
  if (final - inicial < UMBRAL_QS) {
    insercion_lims(T, inicial, final);
  } else {
    dividir_qs(T, inicial, final, k);
    quicksort_lims(T, inicial, k);
    quicksort_lims(T, k + 1, final);
  };
}

void quicksort (int T[], int nelem){
  quicksort_lims(T, 0, nelem);
}

/**********************************************************************/	

double uniforme() //Genera un n�mero uniformemente distribuido en el
                  //intervalo [0,1) a partir de uno de los generadores
                  //disponibles en C.
{
 int t = rand();
 double f = ((double)RAND_MAX+1.0);
 return (double)t/f;
}

/**********************************************************************/	

int generador(int * v, int n){

    for (int j=0; j<n; j++) {
        double u=uniforme();
        int k=(int)(2*n*u);
        v[j]=k;
    }

    double u=uniforme();
    return (int)(3*n*u);
}

/**********************************************************************/	

#endif
