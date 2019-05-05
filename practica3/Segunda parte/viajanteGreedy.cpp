#include <iostream>
#include <climits>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <list>

using namespace std;

int orientation (pair<double,double> a, pair<double,double> b, pair<double,double> c){

  int val = (b.second - a.second) * (c.first - b.first) - 
            (b.first - a.first) * (c.second - b.second);

  if (val == 0)
    return 0;
  else
    return (val > 0) ? 1 : 2;
}

int convexHull (int ** distancias, vector<pair<double,double>> ciudades, list<int> & resultado){
  int toReturnDistancia = 0;
  // Empezamos encontrando la ciudad más a la izquierda
  int leftmost = 1;
  int x = ciudades[0].first;
  for (int i = 2 ; i <= ciudades.size() ; i++){
    if (ciudades[i-1].first < x){
      leftmost = i;
      x = ciudades[i-1].first;
    }
  }

  resultado.push_back(leftmost);

  bool hemosTerminado = false;
  while (!hemosTerminado){
    int actual = resultado.back();
    int seleccionado = (actual + 1) % ciudades.size();
    // Seleccionamos el siguiente
    for (int i = 1 ; i <= ciudades.size() ; i++){
      if (orientation (ciudades[actual-1], ciudades[i-1], ciudades[seleccionado-1]) == 2)
        seleccionado = i;
    }

    toReturnDistancia += distancias[actual][seleccionado];

    hemosTerminado = seleccionado == leftmost;

    if (!hemosTerminado)
      resultado.push_back(seleccionado);
    
  }

  return toReturnDistancia;
}

int nuestroAlgoritmo(int ** distancias, int n, list<int> & resultado, vector<pair<double,double>> ciudades){
  int distancia = convexHull(distancias, ciudades, resultado);

  set<int> candidatos; 

  for (int i = 1 ; i <= n ; i++)
    candidatos.insert(i);
  for (int r : resultado)
    candidatos.erase(r);

  
  while (!candidatos.empty()){
    
    int ciudadMasLejana;
    int maximaDistancia = 0;

    for (int c : candidatos){
      auto it = resultado.begin();
      int dMasLejana = distancias[c][*it];
      it++;
      while (it != resultado.end()){
        if (distancias[c][*it] > dMasLejana)
          dMasLejana = distancias[c][*it];
        it++;
      }
      
      if (dMasLejana > maximaDistancia){
        maximaDistancia = dMasLejana;
        ciudadMasLejana = c;
      }
    }

    list<int>::iterator insercionMasBarata;
    int cuantoCuesta;

    for (auto it = resultado.begin() ; it != resultado.end() ; it++){
      auto siguiente = it;
      siguiente++;

      if (siguiente == resultado.end())
        siguiente = resultado.begin();
      
      int coste = -distancias[*it][*siguiente];

      coste += distancias[*it][ciudadMasLejana];
      coste += distancias[ciudadMasLejana][*siguiente];

      if (it == resultado.begin() || coste < cuantoCuesta){
        insercionMasBarata = it;
        cuantoCuesta = coste;
      }
    }

    candidatos.erase(ciudadMasLejana);
    distancia += cuantoCuesta;
    insercionMasBarata++;
    resultado.insert(insercionMasBarata, ciudadMasLejana);
    
  }
  
  return distancia;
}
  

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

int vecinosCercanos (int ** distancias, int n, list<int> & resultado){
  int distanciaOptima = INT_MAX;
  list<int> listaOptima;
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

    if (distancia < distanciaOptima){
      distanciaOptima = distancia;
      listaOptima = seleccionados;
    }
  }

  resultado = listaOptima;
  return distanciaOptima;
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

  cout << endl << "\t****** VIAJANTE GREEDY *******" << endl << endl;
  cout << "\tSelecciona una estrategia" << endl;
  cout << "\t1) Vecino más cercano" << endl;
  cout << "\t2) Inserción" << endl;
  cout << "\t3) Una de nuestra cosecha" << endl;

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
    nombreDeSalida += "->vecinosCercanos";
  }
  else if (opcion == 2){
    distanciaFinal = insercion(distancias, n, resultado, ciudades);
    nombreDeSalida += "->insercion";
  }
  else if (opcion == 3){
    distanciaFinal = nuestroAlgoritmo(distancias, n, resultado, ciudades);
    nombreDeSalida += "->nuestroAlgoritmo";
  }

  ofstream salida(nombreDeSalida);

  salida << "DIMENSION: " << n << endl;

  for (int ciudad : resultado)
    salida << ciudad << endl;

  cout << "*************************************" << endl;
  cout << "Archivo:" << nombreDeSalida << " creado!" << endl;
  cout << "Distancia: " << distanciaFinal << endl;
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
