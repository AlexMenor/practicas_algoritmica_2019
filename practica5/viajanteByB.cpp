#include <iostream>
#include <climits>
#include <fstream>
#include <vector>
#include <set>
#include <cmath>
#include <list>
#include <queue>

using namespace std;

class Solucion {

    public:
      // Mejor solucion encontrada
      // atributos de clase
    static vector<int> mejorSolucion;
    static int cotaGlobal;

    private:

      // Atributos de instancia
    vector <int> x;
    int n;
    vector<vector<int>> distancias;
    int cotaLocal; 
    int distanciaActual;
    vector<bool> ciudadesYaSeleccionadas;

  public:
    Solucion (int ** distanciasDadas, int nDada)
    : n(nDada), cotaLocal(0), distanciaActual(0), ciudadesYaSeleccionadas(nDada+1, false)
    {
      // Inicializamos la matriz de distancias a partir de la
      // que usa el algoritmo greedy (de esta forma no tenemos que implementar un constructor de copia)

      vector<int> fila;
      // Fila basura para poder acceder con índices que empiezan en 1
      distancias.push_back(fila);

      for (int i = 1 ; i <= n ; i++){
        fila.clear();
        fila.push_back(0);
        for (int j = 1 ; j <= n ; j++)
          fila.push_back(distanciasDadas[i][j]);
        distancias.push_back(fila);
      }

      // Seleccionamos la primera ciudad para reducir el número de nodos hoja posibles
      x.push_back(1);
      ciudadesYaSeleccionadas[1] = true;
    }

    bool factible(){
      return cotaLocal < cotaGlobal;
    }

    vector<Solucion> generaHijos(){
      vector<Solucion> toReturn;

      for (int i = 1 ; i <= n ; i++){
        if (!ciudadesYaSeleccionadas[i]){
          Solucion hijo (*this);
          hijo.x.push_back(i);
          hijo.ciudadesYaSeleccionadas[i] = true;
          hijo.distanciaActual += distancias[i][x.back()];
          // En caso de que sea ya una solución (es decir, sólamente queda
          // por seleccionar una ciudad, la metemos también y actualizamos
          //la distancia acumulada)
          if (hijo.x.size() == n-1){
            // Encontramos la última que queda
            for (int j = 1 ; j <= n ; j++)
              if (!hijo.ciudadesYaSeleccionadas[j]){
                hijo.x.push_back(j);
                hijo.distanciaActual += distancias[j][i];
                // Cerramos el ciclo
                hijo.distanciaActual += distancias[j][1];
                // Actualizamos la cota local
                // para que la función de factibilidad
                // decida si es mejor que la cota global
                // y en ese caso se actualizará
                hijo.cotaLocal = hijo.distanciaActual;
                break;
              }
          }
          else
            hijo.actualizarCotaLocal();

          toReturn.push_back(hijo);
        }
      }
      return toReturn;
    }

    void actualizarCotaLocal(){
      int nuevaCotaLocal = distanciaActual;
      // Primero hallamos la distancia mínima del último que hemos seleccionado
      // a los que quedan sin seleccionar (excluyendo al primero porque no es posible que se cierre 
      // el ciclo ya)
      int minimo = INT_MAX;

      for (int i = 2 ; i <= n ; i++)
        if (!ciudadesYaSeleccionadas[i])
          minimo = minimo > distancias[x.back()][i] ? distancias[x.back()][i] : minimo;

      nuevaCotaLocal += minimo;

      // Ahora sumamos para cada ciudad no seleccionada la mínima distancia
      // de todas las otras ciudades no seleccionadas o la primera

      for (int i = 2 ; i <= n ; i++){
        if (!ciudadesYaSeleccionadas[i]){
          // Lo inicializamos a la primera por si
          // fuese su mínima
          minimo = distancias[i][1]; 
          for (int j = 2 ; j <= n ; j++)
            if (!ciudadesYaSeleccionadas[j] && i != j && distancias[i][j] < minimo)
              minimo = distancias[i][j];

          nuevaCotaLocal += minimo;
        }
      }

      cotaLocal = nuevaCotaLocal;
    }

    bool esSolucion(){
      if (x.size() != n)
        return false;

      // Si hemos llegado aquí es porque es un nodo hoja
      // y además ha superado el test de factibilidad
      // teniendo en su cota local su distancia final.
      // Es decir, sabemos que es una solución mejor que la
      // encontrada hasta el momento

      Solucion::cotaGlobal = distanciaActual;
      Solucion::mejorSolucion = x;

      return true;
    }

    bool operator < (const Solucion & otra) const {
      if (this->cotaLocal < otra.cotaLocal)
        return true;
      else if (this->cotaLocal > otra.cotaLocal)
        return false;
      else
        return this->x.back() < otra.x.back();
    }

    // Métodos de clase, los utilizamos para inicializar
    // la cota global a una solución obtenida por el
    // algoritmo greedy de inserción y para
    // obtener la solución óptima una vez hemos acabado 
    // el B&B

    static int insercion (int ** distancias, int n, list<int> & resultado, vector<pair<double, double>> ciudades){

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
      int distanciaFinal;

      distanciaFinal = distancias[norte.first][este.first] + 
      distancias[este.first][sur.first] + distancias[sur.first][norte.first];

      for (int i = 1 ; i <= n ; i++)
        if (i != norte.first && i != sur.first && i != este.first)
          candidatos.insert(i);

      while (!candidatos.empty()){

        // Almacenan candidato, coste y posición
        // de la inserción mas "barata" actual
        int calculoMinimo = INT_MAX;
        int candidataMinima;
        list<int>::iterator posicionMinima;

        // Probamos para cada candidato
        for (int c : candidatos){
          // Almacenan posición y coste 
          // de la inserción más barata para el 
          // candidato actual
          list<int>::iterator insercionMinima;
          int distanciaMinima;
          // Probamos todas las posibles posiciones
          for (auto it = resultado.begin() ; it != resultado.end() ; it++){
            list<int>::iterator siguiente = it;
            siguiente++;
            // Si es el último, cerramos el circuito con el primero 
            if (siguiente == resultado.end())
              siguiente = resultado.begin();

            // Como vamos a eliminar la arista existente, 
            // la restamos al costo final de la inserción
            int diferencia = -distancias[*it][*siguiente];
            // Sumamos la distancia del primero al candidato
            // y del candidato al siguiente
            diferencia += distancias[*it][c];
            diferencia += distancias[c][*siguiente];

            // Actualizamos la mejor posición de este candidato
            if (it == resultado.begin() || diferencia < distanciaMinima){
              distanciaMinima = diferencia;
              insercionMinima = it;
            }
          }

          if (distanciaMinima < calculoMinimo){
            calculoMinimo = distanciaMinima;
            posicionMinima = insercionMinima;
            candidataMinima = c;
          }
        }
        
        // Borramos de candidatos
        candidatos.erase(candidataMinima);

        // Insertamos 
        posicionMinima++;
        resultado.insert(posicionMinima, candidataMinima);

        // Actualizamos la distancia con la inserción
        distanciaFinal += calculoMinimo;
      }

      return distanciaFinal;
  }

  static void inicializarCotaGlobal(int ** distancias, vector<pair<double, double>> ciudades, int n){
    list<int> resultado;
    cotaGlobal = insercion(distancias, n, resultado, ciudades);
    mejorSolucion.clear();
    for (int c : resultado)
      mejorSolucion.push_back(c);
  }

  static vector<int> obtenerSolucionOptima(int & distanciaFinal){
    distanciaFinal = cotaGlobal;
    return mejorSolucion;
  }
};

vector<int> Solucion::mejorSolucion;
int Solucion::cotaGlobal = INT_MAX;



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

void branchAndBound (Solucion sol){
  priority_queue<Solucion> cola;
  cola.push(sol);
  bool fin = false;

  while (!cola.empty() && !fin){
    Solucion enodo = cola.top();
    cola.pop();

    fin = !enodo.factible();

    if (!fin){
      vector<Solucion> hijosDelEnodo = enodo.generaHijos();

      for (Solucion hijo : hijosDelEnodo){
        if (hijo.factible()){
          if (!hijo.esSolucion())
            cola.push(hijo);
        }
      }
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

  cout << endl << "\t****** VIAJANTE B&B *******" << endl << endl;
 
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

  Solucion::inicializarCotaGlobal(distancias, ciudades, n);

  Solucion sol (distancias, n);

  branchAndBound(sol);

  int distanciaFinal;
  vector <int> resultado = Solucion::obtenerSolucionOptima(distanciaFinal);

  cout << "Distancia calculada: " << distanciaFinal << endl;

  string nombreDeSalida = argv[1];

  nombreDeSalida += "-byb";

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
