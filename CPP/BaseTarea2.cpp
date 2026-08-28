#include <iostream>
#include <string>


using namespace std;


struct NodoArbol {
    string id;
    float energia;
    float masa;

    NodoArbol* izq = nullptr;
    NodoArbol* der = nullptr;

    NodoArbol(string p_id, float p_energia, float p_masa) {
        id = p_id;
        energia = p_energia;
        masa = p_masa;
    }
};

class ListaEnlazada;

class ArbolBinario {
private:
    NodoArbol* raiz = nullptr;

public:
    ArbolBinario() {} //Constructor
    
    void insertar(string id, float energia, float masa);
    
    void preOrden();

    void seleccionarPorUmbral(float umbral, ListaEnlazada &lista);
};


struct NodoLista {
    string id;
    float energia;
    float masa;

    NodoLista* siguiente = nullptr;

    NodoLista(string p_id, float p_energia, float p_masa) {
        id = p_id;
        energia = p_energia;
        masa = p_masa;
    }
};


class ListaEnlazada {
private:
    NodoLista* cabeza = nullptr;

public:
    ListaEnlazada() {} // Constructor

    void insertarAlFinal(string id, float energia, float masa);
    void invertirOrden(); 
    void imprimir();

    void purgarInestabilidad(float eMax);
};

int main() {

    return 0;
}