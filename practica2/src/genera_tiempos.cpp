/**********************************************************************/

#include <chrono>
#include <iostream>
#include <fstream>
#include <utility>
#include "funciones_del_profesor.cpp"
using namespace std::chrono;
using namespace std;

/**********************************************************************/

pair <int, int> sumaObvia(int * v, int n, int x){
    pair <int, int> sol(0,0);
    for (int i = 0 ; i < n-1 ; i++)
        for (int j = i+1 ; j < n ; j++){
            if (v[i] + v[j] == x){
                sol.first = v[i];
                sol.second = v[j];
            }
        }
    return sol;
}

/**********************************************************************/

pair<int,int> sumaDivideYVenceras(int * v, int izda, int dcha, int x){
    int sum = v[izda] + v[dcha];
    if (dcha - izda == 1 || sum == x)
        return pair <int,int> (v[izda], v[dcha]);
    else if (sum > x)
        return sumaDivideYVenceras(v, izda, dcha - 1, x);
    else
        return sumaDivideYVenceras(v, izda+1, dcha, x);

}

/**********************************************************************/

pair <int, int> sumaDivideYVenceras (int * v, int n, int x){
    int izda = 0;
    int dcha = n - 1;

    while ((izda != dcha) && (v[izda] + v[dcha] != x)){
        if (v[izda] + v[dcha] > x)
            dcha--;
        else
            izda++;
    }

    return izda == dcha ? pair<int,int> (0,0) : pair<int,int> (v[izda], v[dcha]);
}

/**********************************************************************/

int main (int argc, char ** argv){
    const int VECES = 100;
    int opcion, desde, hasta, incremento;
    ofstream fichero;

    if (argc == 2)
        fichero.open(argv[1]);

    ostream & salida = fichero.is_open() ? fichero : cout;
    
    cout << "Selecciona el algoritmo:" << endl
    << "1) Obvio" << endl
    << "2) Divide y vencerás" << endl;

    cin >> opcion;
    while (!(opcion > 0 && opcion < 3))
        cin >> opcion;
    
    cout << "¿Desde que tamaño?" << endl;
    cin >> desde;
    while(desde<2){
     cout << "Introduce un tamaño mayor que 1 " << endl;
     cin >> desde;
    }
    cout << "¿Hasta que tamaño?" << endl;
    cin >> hasta;
    while (hasta < desde){
        cout << "Introduce un tamaño mayor o igual que " << desde<< endl;
        cin >> hasta;
    }
    cout << "¿Qué incremento?" << endl;
    cin >> incremento;
    while (incremento <= 0){
        cout << "Introduce un incremento mayor que 0" << endl;
        cin >> incremento;
    }

    cout << "Empezamos: \n" << endl;

    high_resolution_clock::time_point tantes, tdespues;
    duration <double> transcurrido;

    for (int tam = desde ; tam <= hasta ; tam += incremento){
        double sum = 0.0;
        int * v = new int [tam];
        for (int i = 0 ; i < VECES ; i++){
            int x = generador(v, tam);
            tantes = high_resolution_clock::now();
            pair<int,int> sol;
            if (opcion == 1){
               sol = sumaObvia(v, tam, x);
            }
            else{
                quicksort(v, tam);
                sol = sumaDivideYVenceras(v, 0, tam - 1, x);

                // Algoritmo iterativo
                //sol = sumaDivideYVenceras(v, tam,x);
            }
            tdespues = high_resolution_clock::now();

            transcurrido = duration_cast<duration<double>> (tdespues - tantes);
            sum += transcurrido.count();
        }
        salida << tam << " " << sum/VECES << endl;
        delete [] v;
    }

}

/**********************************************************************/