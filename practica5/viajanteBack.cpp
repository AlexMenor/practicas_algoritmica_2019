#include <iostream>
#include <climits>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <list>
#include <queue>

using namespace std;

int viajanteGreedy_vecinosCercanos (vector<vector<int>> & distancias, vector<int> & resultado){
  int distanciaOptima = INT_MAX;
  vector<int> listaOptima;
  set <int> todasLasCiudades;

  for (int i = 1 ; i < distancias.size() ; i++)
    todasLasCiudades.insert(i);

  for (int i = 1 ; i < distancias.size() ; i++){
    set<int> candidatos = todasLasCiudades;
    candidatos.erase(i);
    vector<int> seleccionados;
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

    if (distancia < distanciaOptima){
      distanciaOptima = distancia;
      listaOptima = seleccionados;
    }
  }

  resultado = listaOptima;
  return distanciaOptima;
}


void calcularDistancias (vector<vector<int>> & m, vector<pair<double, double>> & ciudades){
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


class Solucion {
  private:
    vector<int> x;
    vector<vector<int>> & distancias;
    int n;
    int minimoValor;
    vector<int> solucionOptima;
    int distanciaActual;
    vector<bool> ciudadesYaSituadas;
    vector<int> distanciasOptimistas;
    int distanciaFinCircuito;
  public:
    Solucion(vector<vector<int>> & distanciasDadas, int nDada)
    : distancias(distanciasDadas), n(nDada), minimoValor(0), 
    distanciaActual(0), ciudadesYaSituadas(nDada+1, false), x(nDada, 21)
    {
      minimoValor = viajanteGreedy_vecinosCercanos (distancias, solucionOptima);

      // Empezamos con la ciudad 0 ya situada
      // para no tener soluciones equivalentes
      x[0] = 1;
      ciudadesYaSituadas[1] = true;
      
      // Distancia optimista al finalizar el circuito
      distanciaFinCircuito = 0;
      for (int i = 0 ; i < n ; i++)
        if (distancias[1][i+1] < distanciaFinCircuito)
          distanciaFinCircuito = distancias[1][i+1];

      // Distancias optimistas al seleccionar cada ciudad
      // las precalculamos al empezar
      // para no hacerlo en cada ejecución de la cota local

      distanciasOptimistas.push_back(0); // Valor basura; no usaremos la componente 0
      for (int i = 1 ; i <= n ; i++){
        pair<int,int> tuplaDeLaCiudad (INT_MAX,INT_MAX);
        for (int j = i+1 ; j <= n ; j++){
          if (distancias[i][j] < tuplaDeLaCiudad.first)
            tuplaDeLaCiudad.first = distancias[i][j];
          else if (distancias[i][j] < tuplaDeLaCiudad.second)
            tuplaDeLaCiudad.second = distancias[i][j];
        }
        distanciasOptimistas.push_back(ceil((tuplaDeLaCiudad.first + tuplaDeLaCiudad.second) / 2));
      }
    }

    bool hemosTerminado (int k){
      return k == n;
    }
    void iniciaComp (int k){
      x[k] = 1; // NULL
    }
    void sigValComp(int k){
      x[k]++;
    }
    bool factible (int k){
      // Comprobamos que la ciudad no esté situada
      if (ciudadesYaSituadas[x[k]])
        return false;

      // Seleccionamos la ciudad temporalmente (o no)
      // para la función cotaLocal
      
      ciudadesYaSituadas[x[k]] = true;
      distanciaActual += distancias[x[k]][x[k-1]];

      if (cotaLocal(k) + distanciaActual < minimoValor)
        return true;
      
      // Deseleccionamos la ciudad si no se cumple la condición
      // de arriba
      ciudadesYaSituadas[x[k]] = false;
      distanciaActual -= distancias[x[k]][x[k-1]];
      return false;
    }
    bool todosGenerados(int k){
      return x[k] == n+1; // END
    }

    void procesaSolucion(){
      // Sumamos la afinidad del último con el primero
      distanciaActual += distancias[1][x[n-1]];

      // Actualizamos la cota global si encontramos una
      // solución mejor
      if (distanciaActual < minimoValor){
        minimoValor = distanciaActual;
        solucionOptima = x;
      }

      distanciaActual -= distancias[1][x[n-1]];
    }
    void imprimeSolucion (){
      cout << "Con esta matriz de distancias:" << endl;

      for (int i = 1 ; i <= n ; i++){
        for(int j = 1 ; j <= n ; j++)
          cout << distancias[i][j] << " ";
        cout << endl;
      }

      cout << "La mejor asignación de ciudades, con distancia: "
      << minimoValor << " es:" << endl;

      for (int i = 0 ; i < n ; i++)
        cout << solucionOptima[i] << " ";
      cout << endl;
    }

    void vueltaAtras (int k){
      distanciaActual -= distancias[x[k]][x[k-1]];
      ciudadesYaSituadas[x[k]] = false;
    }
    
    vector<int> obtenerSolucionOptima () const {
        return (solucionOptima);
    }

    int cotaLocal (int k){
      int toReturn = 0;

      // Sumamos la distancia optimista que "esperamos"
      // de las ciudades no seleccionadas aún
      for (int i = 1 ; i <= n ; i++){
        if (!ciudadesYaSituadas[i])
          toReturn += distanciasOptimistas[i];
      }

      // Sumamos la distancia optimista de la
      // última ciudad con la primera
      return toReturn + distanciaFinCircuito;
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

// El programa tiene como argumento el fichero
// con el "mapa", se generará en el
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

  cout << endl << "\t****** VIAJANTE BACKTRACKING *******" << endl << endl;
 
  entrada.ignore(256, ' ');

  int n;
  entrada >> n;

  vector<pair<double, double>> ciudades;

  double ign, x, y;
  for (int i = 0 ; i < n ; i++){
    entrada >> ign >> x >> y;
    ciudades.push_back(pair<double, double>(x,y));
  }

  vector<vector<int>> distancias;
  distancias.resize(n+1);
  
  for (int i=1; i<n+1; i++)
      distancias[i].resize(n+1);
  
  calcularDistancias(distancias, ciudades);
  Solucion sol (distancias, n);

  backtracking(sol);
  vector <int> resultado = sol.obtenerSolucionOptima();
  sol.imprimeSolucion();

  string nombreDeSalida = argv[1];

  nombreDeSalida += "-back";

  ofstream salida(nombreDeSalida);

  salida << "DIMENSION: " << n << endl;

  for (int ciudad : resultado)
    salida << ciudad << endl;

  cout << "*************************************" << endl;
  cout << "Archivo:" << nombreDeSalida << " creado!" << endl;
  cout << "*************************************" << endl << endl;

  string archivoSolucion (argv[1]);

  int posPunto = archivoSolucion.find_first_of('.');

  archivoSolucion = archivoSolucion.substr(0, posPunto);

  archivoSolucion += ".opt.tour";

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

    for (int i = 1 ; i < n ; i++)
      distanciaSolucion += distancias[ciudadesSolucion[i]][ciudadesSolucion[i-1]];
    
    distanciaSolucion += distancias[ciudadesSolucion[n-1]][ciudadesSolucion[0]];

    cout << "La distancia de la solución 'optima' es: " << distanciaSolucion << endl;
  }

  entradaSolucion.close();
  entrada.close();
  salida.close();
}
