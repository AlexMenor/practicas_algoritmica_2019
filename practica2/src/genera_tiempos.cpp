#include <chrono>
#include <iostream>
#include <fstream>
#include <utility>
#include "funciones_del_profesor.cpp"
using namespace std::chrono;
using namespace std;

pair <int, int> sumaObvia(int * v, int n, int x){
    for (int i = 0 ; i < n-1 ; i++)
        for (int j = i+1 ; j < n ; j++){
            if (v[i] + v[j] == x){
                return pair<int,int>(v[i], v[j]);
            }
        }
    return pair<int,int> (0,0);
}

pair<int,int> sumaDivideYVenceras(int * v, int izda, int dcha, int x){
    int sum = v[izda] + v[dcha];
    if (abs(izda - dcha) == 1 || sum == x)
        return pair <int,int> (v[izda], v[dcha]);
    else if (sum > x)
        return sumaDivideYVenceras(v, izda, dcha - 1, x);
    else
        return sumaDivideYVenceras(v, izda+1, dcha, x);

}

double uniforme() //Genera un n�mero uniformemente distribuido en el
                  //intervalo [0,1) a partir de uno de los generadores
                  //disponibles en C.
{
 int t = rand();
 double f = ((double)RAND_MAX+1.0);
 return (double)t/f;
}

int generador(int * v, int n){

    for (int j=0; j<n; j++) {
        double u=uniforme();
        int k=(int)(2*n*u);
        v[j]=k;
    }

    double u=uniforme();
    return (int)(3*n*u);
}


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
    cout << "¿Hasta que tamaño?" << endl;
    cin >> hasta;
    cout << "¿Qué incremento?" << endl;
    cin >> incremento;
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
            if (opcion == 1)
               sol = sumaObvia(v, tam, x);
            else{
                quicksort(v, tam);
                sol = sumaDivideYVenceras(v, 0, tam - 1, x);
            }
            tdespues = high_resolution_clock::now();

            transcurrido = duration_cast<duration<double>> (tdespues - tantes);
            sum += transcurrido.count();
        }
        salida << tam << " " << sum/VECES << endl;
        delete [] v;
    }

}
