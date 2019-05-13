#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <map>

using namespace std::chrono;
using namespace std;

void generarAfinidades(vector<vector<int>> & afinidades, int n){
  srand(time(NULL));

  afinidades.clear();

  // Reservamos la matriz
  for (int i = 0 ; i < n ; i++){
    vector<int> fila(n);
    afinidades.push_back(fila);
  }

  for (int i = 0 ; i < n ; i++){
    for (int j = i + 1 ; j < n ; j++){
      int afinidad = rand() % 101;
      afinidades[i][j] = afinidad;
      afinidades[j][i] = afinidad;
    }
  }


}

class Solucion {
  private:
    vector<int> x;
    vector<vector<int>> & afinidades;
    int n;
    int maximoValor;
    vector<int> solucionOptima;
    int afinidadActual;
    vector<int> comensalesYaSentados;
  public:
    Solucion(vector<vector<int>> & afinidadesDadas, int nDada)
    : afinidades(afinidadesDadas), n(nDada), 
    maximoValor(0), afinidadActual(0), comensalesYaSentados(0, n)
    {
      // TODO: Solución óptima y maximoValor deberían ajustarse
      // de inicio a una solución con el algoritmo greedy


      x.reserve(n);

      // Empezamos con el comensal 0 ya sentado
      // para no tener soluciones equivalentes
      x[0] = 0;
      comensalesYaSentados[0] = 1;
    }

    bool hemosTerminado (int k){
      return k == n;
    }
    void iniciaComp (int k){
      x[k] = 0; // NULL
    }
    void sigValComp(int k){
      x[k]++;
    }
    bool factible (int k){
      // Comprobamos que ese comensal no está ya sentado
      if (comensalesYaSentados[x[k]])
        return false;

      // TODO: Comprobar que la cota local > cota global

      // Sumamos la afinidad del actual y el anterior
      afinidadActual += afinidades[x[k]][x[k-1]];
    }
    bool todosGenerados(int k){
      return x[k] == n; // END
    }
    int decision(int k) const {
      return k < n && k >= 0 ? x[k] : -1;
    }

    void procesaSolucion(){
      //TODO: Acordarse de sumar la afinidad entre el primer y último comensal
    }
    void imprimeSolucion (){

    }

    void vueltaAtras (int k){
      afinidadActual -= afinidades[x[k]][x[k-1]];
    }

    int cotaLocal (int k){
    }
};

void backtracking(Solucion & sol, int k = 0){
  if (sol.hemosTerminado(k))
    sol.procesaSolucion();
  else{
    sol.iniciaComp(k);
    sol.sigValComp(k);
    while (!sol.todosGenerados(k)){
      if (sol.factible(k)){
        backtracking(sol, k+1);
        sol.vueltaAtras(k);
      }
      sol.sigValComp(k);
    }
  }
}

int main (int argc, char ** argv){

    int desde, hasta, incremento;
    ofstream fichero;

    if (argc == 2)
        fichero.open(argv[1]);

    ostream & salida = fichero.is_open() ? fichero : cout;
    
    cout << "¿Desde que tamaño?" << endl;
    cin >> desde;
    cout << "¿Hasta que tamaño?" << endl;
    cin >> hasta;
    cout << "¿Qué incremento?" << endl;
    cin >> incremento;
    cout << "Empezamos: \n" << endl;


    for (int n = desde ; n <= hasta ; n++){
      vector<vector<int>> afinidades;
      generarAfinidades(afinidades, n);

      time_point<high_resolution_clock> tantes;
      time_point<high_resolution_clock> tdespues;
      tantes = high_resolution_clock::now();

      tdespues = high_resolution_clock::now();

      duration<double> t = duration_cast<duration<double>> (tdespues - tantes);

      salida << n << "\t" << t.count() << endl;
    }
    return 0;
}
