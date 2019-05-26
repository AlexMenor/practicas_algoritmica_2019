#include <chrono>
#include <iostream>
#include <climits>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <list>
#include <queue>

using namespace std;
using namespace std::chrono;

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
    double xi = ciudades[i].first;
    double yi = ciudades[i].second;
    for (int j = i+1 ; j < ciudades.size() ; j++){
      double xj = ciudades[j].first;
      double yj = ciudades[j].second;
      double distancia = sqrt(pow(xi-xj,2) + pow(yi-yj,2));
      m[i+1][j+1] = (int) round(distancia);
      m[j+1][i+1] = (int) round(distancia);
    }
  }
}


class Solucion {
  private:
    vector<int> x;
    vector<vector<int>> & distancias;
    int n;
    int cotaGlobal;
    vector<int> solucionOptima;
    int distanciaActual;
    vector<bool> ciudadesYaSituadas;
  public:
    Solucion(vector<vector<int>> & distanciasDadas, int nDada)
    : distancias(distanciasDadas), n(nDada), 
    distanciaActual(0), ciudadesYaSituadas(nDada+1, false), x(nDada)
    {
      cotaGlobal = viajanteGreedy_vecinosCercanos (distancias, solucionOptima);

      // Empezamos con la ciudad 0 ya situada
      // para no tener soluciones equivalentes
      x[0] = 1;
      ciudadesYaSituadas[1] = true;
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
      
      if (cotaLocal(k) < cotaGlobal)
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
      if (distanciaActual < cotaGlobal){
        cotaGlobal = distanciaActual;
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
      << cotaGlobal << " es:" << endl;

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
       int cotaL = distanciaActual;

      // Ahora sumamos para cada ciudad no seleccionada la mínima distancia
      // de todas las otras ciudades no seleccionadas, la primera o la última

      for (int i = 2 ; i <= n ; i++){
        if (!ciudadesYaSituadas[i]){
          // Lo inicializamos a la primera o a la última seleccionada por si
          // fuese su mínima
            int minimo = distancias[i][1]; 
			minimo = minimo > distancias[x[k]][i] ? distancias[x[k]][i] : minimo;

          for (int j = 2 ; j <= n ; j++)
            if (!ciudadesYaSituadas[j] && i != j && distancias[i][j] < minimo)
              minimo = distancias[i][j];

          cotaL += minimo;
        }
      }
	    return (cotaL);
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
  
  time_point<high_resolution_clock> tini;
  time_point<high_resolution_clock> tfin;
  
  tini = high_resolution_clock::now();

  backtracking(sol, 1);
  
  tfin = high_resolution_clock::now();
  
  duration<double> t = duration_cast<duration<double>> (tfin - tini);
  
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

  cout << "Tiempo: " << t.count() << endl;
  entradaSolucion.close();
  entrada.close();
  salida.close();
}