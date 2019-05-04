#include <iostream>
#include <climits>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <stack>
#include <list>

using namespace std;

pair<int,int> extremCoordenada(vector<pair<double, double>> ciudades){
	pair<int,int> ext;
	ext.first = ext.second = (int) ceil(ciudades[0].first);

	for (int i=0; i<ciudades.size(); i++){
		if (((int) ceil(ciudades[i].first)) < ext.first)
			ext.first = (int) ceil(ciudades[i].first);
		if (((int) ceil(ciudades[i].first)) > ext.second)
			ext.second = (int) ceil(ciudades[i].first);
	}
	return (ext);
}

// Método basado en ir seleccionando las ciudades de cada fila que menos distan entre sí
int metodoPropio (int ** distancias, int n, list<int> & resultado, vector<pair<double, double>> ciudades){
	map<double, int> candidatos;
	vector<pair<double, double>> ciudades_tmp = ciudades;
	resultado.clear();
	
	pair<int,int> ext=extremCoordenada(ciudades);
	set<int> indCiudades;
	bool ascendente=true;
	stack<pair<double, int>> pila;
	
	for (int i=0; i<ciudades.size(); i++)
		indCiudades.insert(i+1);

	for (int i=ext.first; i<=ext.second; i++){
		vector<pair<double,double>>::iterator it = ciudades_tmp.begin();

		set<int>::iterator ind = indCiudades.begin(), tmp;
		while (it != ciudades_tmp.end()){
			if (((int) ceil (it->first)) == i){
				candidatos.insert(pair<double,int>(it->second, *ind));
				ciudades_tmp.erase(it);
				tmp = ind; ind++;
				indCiudades.erase(tmp);
			}
			else{
				it++; ind++;
			}
		}
		if (!ascendente){
			for (auto it2 = candidatos.begin(); it2 != candidatos.end(); it2++)
				pila.push(pair<double, int>(*it2));
			
			while (!pila.empty()){
				int dato = pila.top().second;
				pila.pop();
				resultado.push_back(dato);
			}
		}
		else {
			for (auto it2 = candidatos.begin(); it2 != candidatos.end(); it2++)
				resultado.push_back(it2->second);
		}
		// Si no había ninguna ciudad asociada a la cordenada, no cambiamos de orden-
		if (!candidatos.empty())
			ascendente = !ascendente;

		candidatos.clear();
	}
	//Calculamos distancia a partir de la lista de resultados
	int distancia=0;
	list<int>::iterator in = resultado.begin(), sig = in;

	sig++;
	while (sig != resultado.end()){
		distancia += distancias[*in][*sig];
		in++; sig++;
	}
	distancia += distancias[resultado.front()][resultado.back()];
	return (distancia);
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
    distanciaFinal = metodoPropio(distancias, n, resultado, ciudades);
    nombreDeSalida += "->metodoPropio";
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