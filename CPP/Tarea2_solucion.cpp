#include <iostream>
#include <fstream>
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

    NodoArbol* insertarRec(NodoArbol* nodo, string id, float energia, float masa) {
        if (nodo == nullptr) {
            return new NodoArbol(id, energia, masa);
        }

        if (energia < nodo->energia) {
            nodo->izq = insertarRec(nodo->izq, id, energia, masa);
        } else {
            nodo->der = insertarRec(nodo->der, id, energia, masa);
        }

        return nodo;
    }

    void preOrdenRec(NodoArbol* nodo) {
        if (nodo == nullptr) return;

        cout << nodo->id << " "
             << nodo->energia << " "
             << nodo->masa << endl;

        preOrdenRec(nodo->izq);
        preOrdenRec(nodo->der);
    }

    void seleccionarPorUmbralRec(
        NodoArbol* nodo,
        float umbral,
        ListaEnlazada& lista
    );

    void liberarRec(NodoArbol* nodo) {
        if (nodo == nullptr) return;

        liberarRec(nodo->izq);
        liberarRec(nodo->der);

        delete nodo;
    }

public:
    ArbolBinario() {}

    ~ArbolBinario() {
        liberarRec(raiz);
        raiz = nullptr;
    }

    void insertar(string id, float energia, float masa) {
        raiz = insertarRec(raiz, id, energia, masa);
    }

    void preOrden() {
        preOrdenRec(raiz);
    }

    void seleccionarPorUmbral(
        float umbral,
        ListaEnlazada& lista
    ) {
        seleccionarPorUmbralRec(raiz, umbral, lista);
    }
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
    ListaEnlazada() {}

    ~ListaEnlazada() {
        NodoLista* actual = cabeza;

        while (actual != nullptr) {
            NodoLista* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }

        cabeza = nullptr;
    }

    void insertarAlFinal(string id, float energia, float masa) {
        NodoLista* nuevo = new NodoLista(id, energia, masa);

        if (cabeza == nullptr) {
            cabeza = nuevo;
            return;
        }

        NodoLista* actual = cabeza;

        while (actual->siguiente != nullptr) {
            actual = actual->siguiente;
        }

        actual->siguiente = nuevo;
    }

    void invertirOrden() {
        NodoLista* anterior = nullptr;
        NodoLista* actual = cabeza;
        NodoLista* siguienteTemp = nullptr;

        while (actual != nullptr) {
            siguienteTemp = actual->siguiente;
            actual->siguiente = anterior;
            anterior = actual;
            actual = siguienteTemp;
        }

        cabeza = anterior;
    }

    void imprimir() {
        NodoLista* actual = cabeza;

        while (actual != nullptr) {
            cout << actual->id << " -> ";
            actual = actual->siguiente;
        }

        cout << "NULL" << endl;
    }

    void purgarInestabilidad(float eMax) {
        if (cabeza == nullptr || cabeza->siguiente == nullptr) {
            return;
        }

        NodoLista* anterior = nullptr;
        NodoLista* actual = cabeza;

        while (actual != nullptr && actual->siguiente != nullptr) {
            NodoLista* sucesor = actual->siguiente;

            float sumaEnergia = actual->energia + sucesor->energia;

            if (sumaEnergia > eMax) {
                if (actual->energia >= sucesor->energia) {
                    if (anterior == nullptr) {
                        cabeza = sucesor;
                    } else {
                        anterior->siguiente = sucesor;
                    }

                    delete actual;
                    actual = sucesor;
                } else {
                    actual->siguiente = sucesor->siguiente;
                    delete sucesor;
                }
            } else {
                anterior = actual;
                actual = actual->siguiente;
            }
        }
    }
};

void ArbolBinario::seleccionarPorUmbralRec(
    NodoArbol* nodo,
    float umbral,
    ListaEnlazada& lista
) {
    if (nodo == nullptr) return;

    seleccionarPorUmbralRec(nodo->izq, umbral, lista);

    if (nodo->energia >= umbral) {
        lista.insertarAlFinal(
            nodo->id,
            nodo->energia,
            nodo->masa
        );
    }

    seleccionarPorUmbralRec(nodo->der, umbral, lista);
}

int main() {
    ArbolBinario arbol;
    ListaEnlazada lista;

    ifstream archivo("Particulas.txt");

    if (!archivo.is_open()) {
        return 1;
    }

    int n;
    archivo >> n;

    for (int i = 0; i < n; i++) {
        string id;
        float energia, masa;

        archivo >> id >> energia >> masa;

        arbol.insertar(id, energia, masa);
    }

    archivo.close();

    arbol.preOrden();

    float umbral;
    cin >> umbral;

    arbol.seleccionarPorUmbral(umbral, lista);

    lista.imprimir();

    float eMax;
    cin >> eMax;

    lista.purgarInestabilidad(eMax);

    lista.imprimir();

    lista.invertirOrden();

    lista.imprimir();

    return 0;
}

