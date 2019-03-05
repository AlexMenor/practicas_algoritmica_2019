#include "funciones_del_profesor.cpp"
#include <chrono>
using namespace std::chrono;

void probarFibo (int desde, int hasta, int incremento, ostream & salida){
    high_resolution_clock::time_point tantes, tdespues;
    duration<double> transcurrido;

    for (int i = desde ; i <= hasta ; i += incremento){
        tantes = high_resolution_clock::now();
        fibo(i);
        tdespues = high_resolution_clock::now();
        transcurrido = duration_cast<duration<double>> (tdespues - tantes);
        salida << i << " " << transcurrido.count() << endl;
    }
}

void probarFloyd (int desde, int hasta, int incremento, ostream & salida){
    high_resolution_clock::time_point tantes, tdespues;
    duration<double> transcurrido;
    int ** M ;
    for (int i = desde ; i <= hasta ; i+= incremento){
        M = ReservaMatriz(i);
        RellenaMatriz(M,i);
        tantes = high_resolution_clock::now();
        Floyd(M,i);
        tdespues = high_resolution_clock::now();

        transcurrido = duration_cast<duration<double>> (tdespues - tantes);
        salida << i << " " << transcurrido.count() << endl;

        LiberaMatriz(M,i);
    }
}

void probarOrdenacion (void (*f)(int*, int), int desde, int hasta, int incremento, ostream & salida){
    high_resolution_clock::time_point tantes, tdespues;
    duration<double> transcurrido;
    int * T;
    for (int i = desde ; i <= hasta ; i += incremento){
        T = new int [i];
        assert(T);
        srandom(time(0));

        for (int j = 0; j < i; j++)
        {
            T[j] = random();
        }
            
        tantes = high_resolution_clock::now();
        f(T, i);
        tdespues = high_resolution_clock::now();

        transcurrido = duration_cast<duration<double>> (tdespues - tantes);
        salida << i << " " << transcurrido.count() << endl;
        
        delete [] T;
    }
}


int main (int argc, char ** argv){

    int opcion, desde, hasta, incremento, repeticiones;
    ofstream fichero;

    if (argc == 2)
        fichero.open(argv[1]);

    ostream & salida = fichero.is_open() ? fichero : cout;
    
    cout << "Selecciona el algoritmo:" << endl
    << "1) Burbuja" << endl
    << "2) Fibonacci" << endl
    << "3) Floyd" << endl
    << "4) Heapsort" << endl
    << "5) Inserción" << endl
    << "6) Mergesort" << endl
    << "7) Quicksort" << endl
    << "8) Selección" << endl;

    cin >> opcion;
    while (!(opcion > 0 && opcion < 9))
        cin >> opcion;
    
    cout << "¿Desde que tamaño?" << endl;
    cin >> desde;
    cout << "¿Hasta que tamaño?" << endl;
    cin >> hasta;
    cout << "¿Qué incremento?" << endl;
    cin >> incremento;
    cout << "Empezamos: \n" << endl;

    switch (opcion){
        case 1:
            probarOrdenacion(burbuja, desde, hasta, incremento, salida);
            break;
        case 2: 
            probarFibo(desde, hasta, incremento, salida);
            break;
        case 3:
            probarFloyd (desde, hasta, incremento, salida);
            break;
        case 4:
            probarOrdenacion(heapsort, desde, hasta, incremento, salida);
            break;
        case 5:
            probarOrdenacion(insercion, desde, hasta, incremento, salida);
            break;
        case 6:
            probarOrdenacion(mergesort, desde, hasta, incremento, salida);
            break;
        case 7:
            probarOrdenacion(quicksort, desde, hasta, incremento, salida);
            break;
        case 8:
            probarOrdenacion(seleccion, desde, hasta, incremento, salida);
            break;

    }

    return 0;
}