#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <list>

using namespace std;

int vecinosCercanos (int ** distancias, int n, list<int> & resultado){
  map <int, list<int>> completados;
  set <int> todasLasCiudades;

  for (int i = 1 ; i <= n ; i++)
    todasLasCiudades.insert(i);

  for (int i = 1 ; i <= n ; i++){
    set<int> candidatos = todasLasCiudades;
    candidatos.erase(i);
    list<int> seleccionados;
    seleccionados.push_back(i);
    int distancia = 0;

    while (!candidatos.empty()){
      int actual = seleccionados.back();
      int masCercano = *candidatos.begin();
      int min = distancias[actual][masCercano];

      for (int c : candidatos){
        int d = distancias[actual][c];
        if (d < min){
          masCercano = c;
          min = d;
        }
      }

      seleccionados.push_back(masCercano);
      distancia += min;
      candidatos.erase(masCercano);
    }

    completados[distancia] = seleccionados;
  }

  resultado = completados.begin() -> second;
}

void calcularDistancias (int ** m, vector<pair<double, double>> & ciudades){
  for (int i = 0 ; i < ciudades.size() ; i++){
    int xi = ciudades[i].first;
    int yi = ciudades[i].second;
    for (int j = i+1 ; j < ciudades.size() ; j++){
      int xj = ciudades[j].first;
      int yj = ciudades[j].second;
      double distancia = sqrt(pow(xi-xj,2) + pow(yi-yj,2));
      m[i+1][j+1] = (int) ceil(distancia);
      m[j+1][i+1] = (int) ceil(distancia);
    }
  }
}

// El programa tiene como argumento el fichero
// con el "mapa", después se podrá seleccionar
// una de las alternativas, se generará en el
// mismo directorio un archivo salida 

int main (int argc, char ** argv){

  if (argc < 2){
    cerr << "Uso: " << argv[0] << " ficheroDeEntrada.tsp" << endl;
    exit (EXIT_FAILURE);
  }

  ifstream entrada (argv[1]);

  if (!entrada){
    cerr << argv[1] << " no existe o es inválido" << endl;
    exit (EXIT_FAILURE);
  }

  entrada.ignore(256, ' ');

  int n;
  entrada >> n;

  vector<pair<double, double>> ciudades;

  double ign, x, y;
  for (int i = 0 ; i < n ; i++){
    entrada >> ign >> x >> y;
    ciudades.push_back(pair<double, double>(x,y));
  }

  int ** distancias = new int * [n+1];
  for (int i = 0 ; i <= n ; i++)
    distancias[i] = new int[n+1];

  calcularDistancias(distancias, ciudades);

  list <int> resultado;

  int distanciaFinal = vecinosCercanos(distancias, n, resultado);

  ofstream salida("salida");

  salida << "DIMENSION: " << n << endl;

  for (int ciudad : resultado)
    salida << ciudad << endl;
  
  entrada.close();
  salida.close();


}