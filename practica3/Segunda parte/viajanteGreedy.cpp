#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <list>

using namespace std;

int insercion (int ** distancias, int n, list<int> & resultado, vector<pair<double, double>> ciudades){
  // Lo primero formamos el circuito inicial: norte, sur y este
  pair<int, double> norte;
  pair<int, double> sur;
  pair<int, double> este;

  for (int i = 1 ; i <= n ; i++){
    pair<double, double> c = ciudades[i-1];

    if (i == 1 || c.second > norte.second){
      norte.first = i;
      norte.second = c.second;
    }
    if (i == 1 || c.second < sur.second){
      sur.first = i;
      sur.second = c.second;
    }
    if (i == 1 || c.first > este.second){
      este.first = i;
      este.second = c.first;
    }
  }

  resultado.push_back(norte.first);
  resultado.push_back(este.first);
  resultado.push_back(sur.first);

  set<int> candidatos;

  for (int i = 1 ; i <= n ; i++)
    if (i != norte.first && i != sur.first && i != este.first)
      candidatos.insert(i);

  while (!candidatos.empty()){
    
  }

}

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
    distancia += (distancias[seleccionados.front()][seleccionados.back()]);

    completados[distancia] = seleccionados;
  }

  resultado = completados.begin() -> second;
  return completados.begin() -> first;
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

  cout << "****** VIAJANTE GREEDY *******" << endl << endl;
  cout << "/tSelecciona una estrategia" << endl;
  cout << "1) Vecino más cercano" << endl;
  cout << "2) Inserción" << endl;
  cout << "3) Una de nuestra cosecha" << endl;

  int opcion;
  cin >> opcion;

  while (opcion < 1 || opcion > 3)
    cin >> opcion;
  
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
  int distanciaFinal;
  string nombreDeSalida = argv[1];

  if (opcion == 1){
    distanciaFinal = vecinosCercanos(distancias, n, resultado);
    nombreDeSalida += "-> vecinosCercanos";
  }

  ofstream salida(nombreDeSalida);

  salida << "DIMENSION: " << n << endl;

  for (int ciudad : resultado)
    salida << ciudad << endl;
  
  cout << "Archivo " << nombreDeSalida << " creado!" << endl;
  cout << "Distancia: " << distanciaFinal << endl;

  string archivoSolucion;
  cout << "Si quieres, introduce el nombre de un archivo solución para comparar: " << endl;
  cin >> archivoSolucion;

  ifstream entradaSolucion (archivoSolucion);

  if (entradaSolucion){
    entradaSolucion.ignore(256, '\n');
    vector<int> ciudadesSolucion;
    int c;
    for (int i = 0 ; i < n ; i++){
      entradaSolucion >> c;
      ciudadesSolucion.push_back(c);
    }

    int distanciaSolucion = 0;

    for (int i = 1 ; i <n ; i++)
      distanciaSolucion += distancias[ciudadesSolucion[i]][ciudadesSolucion[i-1]];

    cout << "La distancia de la solución es: " << distanciaSolucion << endl;
  }

  entradaSolucion.close();
  entrada.close();
  salida.close();


}