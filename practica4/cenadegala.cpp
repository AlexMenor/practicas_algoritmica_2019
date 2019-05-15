#include <chrono>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <map>
#include <set>

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

int cenaDeGalaGreedy (const vector<vector<int>> & afinidades, vector<int> & resultado){
	set<int> posiblesCandidatos;
	int afin_global=0;

	// Generamos todos los candidatos posibles.
	for (int i=0; i<afinidades.size(); i++)
		posiblesCandidatos.insert(i);

	for (int i=0; i<afinidades.size(); i++){
		set<int> candidatos = posiblesCandidatos;
		vector<int> seleccionados;
		int suma_afin=0;

		candidatos.erase(i);
		seleccionados.push_back(i);

		// Generamos una solución partiendo del comensal i.
		while (!candidatos.empty()){
			int actual = seleccionados.back();
			int pos_mafin = *(candidatos.begin());
			int max_afin = afinidades[actual][pos_mafin];

			for (auto it = candidatos.begin(); it != candidatos.end(); it++){
				if (afinidades[actual][*it] > max_afin){
					max_afin = afinidades[actual][*it];
					pos_mafin = *it;
				}
			}
			candidatos.erase(pos_mafin);
			seleccionados.push_back(pos_mafin);
			suma_afin+=max_afin;
		}
		if (suma_afin>afin_global){
			afin_global=suma_afin;
			resultado=seleccionados;
		}
	}
	
	return (afin_global);
}

class Solucion {
  private:
    vector<int> x;
    vector<vector<int>> & afinidades;
    int n;
    int maximoValor;
    vector<int> solucionOptima;
    int afinidadActual;
    vector<bool> comensalesYaSentados;
    vector<int> afinidadesOptimistas;
    int afinidadAcabarMesa;
  public:
    Solucion(vector<vector<int>> & afinidadesDadas, int nDada)
    : afinidades(afinidadesDadas), n(nDada), maximoValor(0), 
    afinidadActual(0), comensalesYaSentados(nDada, false), x(nDada, 21)
    {
      maximoValor = cenaDeGalaGreedy (afinidades, solucionOptima);

      // Empezamos con el comensal 0 ya sentado
      // para no tener soluciones equivalentes
      x[0] = 0;
      comensalesYaSentados[0] = true;
      
      // Afinidad optimista de cerrar la mesa
      afinidadAcabarMesa = 0;
      for (int i = 0 ; i < n ; i++)
        if (afinidades[0][i] > afinidadAcabarMesa)
          afinidadAcabarMesa = afinidades[0][i];

      // Afinidades optimistas al sentar cada comensal
      // las precalculamos al empezar
      // para no hacerlo en cada ejecución de la cota local

      for (int i = 0 ; i < n ; i++){
        pair<int,int> tuplaDelComensal (0,0);
        for (int j = 0 ; j < n ; j++){
          if (afinidades[i][j] > tuplaDelComensal.first)
            tuplaDelComensal.first = afinidades[i][j];
          else if (afinidades[i][j] > tuplaDelComensal.second)
            tuplaDelComensal.second = afinidades[i][j];
        }
        afinidadesOptimistas.push_back(ceil((tuplaDelComensal.first + tuplaDelComensal.second) / 2));
      }
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

      // Lo sentamos temporalmente (o no)
      // para la función cotaLocal
      
      comensalesYaSentados[x[k]] = true;
      afinidadActual += afinidades[x[k]][x[k-1]];

      if (cotaLocal(k) + afinidadActual > maximoValor)
        return true;
      
      // Lo acabamos levantando si no se cumple la condición
      // de arriba
      comensalesYaSentados[x[k]] = false;
      afinidadActual -= afinidades[x[k]][x[k-1]];
      return false;
    }
    bool todosGenerados(int k){
      return x[k] == n; // END
    }

    void procesaSolucion(){
      // Sumamos la afinidad del último con el primero
      afinidadActual += afinidades[0][x[n-1]];

      // Actualizamos la cota global si encontramos una
      // solución mejor
      if (afinidadActual > maximoValor){
        maximoValor = afinidadActual;
        solucionOptima = x;
      }

      afinidadActual -= afinidades[0][x[n-1]];
    }
    void imprimeSolucion (){
      cout << "Con esta matriz de afinidades:" << endl;

      for (int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < n ; j++)
          cout << afinidades[i][j] << " ";
        cout << endl;
      }

      cout << "La mejor asignación de asientos, con afinidad: "
      << maximoValor*2 << " es:" << endl;

      for (int i = 0 ; i < n ; i++)
        cout << solucionOptima[i] << " ";
      cout << endl;
    }

    void vueltaAtras (int k){
      afinidadActual -= afinidades[x[k]][x[k-1]];
      comensalesYaSentados[x[k]] = false;
    }

    int cotaLocal (int k){
      int toReturn = 0;

      // Sumamos la afinidad optimista que "esperamos"
      // de los comensales no sentados aún
      for (int i = 0 ; i < n ; i++){
        if (!comensalesYaSentados[i])
          toReturn += afinidadesOptimistas[i];
      }

      // Sumamos la afinidad optimista del
      // último con el primero
      return toReturn + afinidadAcabarMesa;
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

	if (desde<2){
		cout << "El valor tamaño inicial debe ser mayor que 1." << endl;
		exit(1);	
	}


    for (int n = desde ; n <= hasta ; n++){
      vector<vector<int>> afinidades;
      generarAfinidades(afinidades, n);

      time_point<high_resolution_clock> tantes;
      time_point<high_resolution_clock> tdespues;
      tantes = high_resolution_clock::now();
      Solucion sol(afinidades, n);
      backtracking(sol, 1);
      sol.imprimeSolucion();
      tdespues = high_resolution_clock::now();

      duration<double> t = duration_cast<duration<double>> (tdespues - tantes);

      salida << n << "\t" << t.count() << endl;
    }
    return 0;
}
