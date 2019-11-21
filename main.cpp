#include <iostream>
#include <vector>
#include <cmath>

typedef struct Nodo{
    int dato;
    struct Nodo* izq;
    struct Nodo* der;
} Nodo;

enum Side {LEFT,RIGHT};

using namespace std;

Nodo* crear(){
    return NULL;
}

bool estaVacio(Nodo* T){
    return (T == NULL)?true:false;
}

Nodo* crearNodo(int n){
    Nodo* nuevo;
    nuevo = (Nodo*) malloc(sizeof(Nodo)); //Alternativa 1
    //nuevo = new Nodo;                     //Alternativa 2
    if(nuevo == NULL){
        cout << "CATASTROPHIC FAILURE!!!!! :'(\n";
        exit(EXIT_FAILURE);
    }
    nuevo->dato = n;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    return nuevo;
}

Nodo* insertar(Nodo* T, int n){
    if(estaVacio(T)) return crearNodo(n);
    if(n <= T->dato) T->izq = insertar(T->izq, n);
    else T->der = insertar(T->der, n);
    return T;
}

void mostrarInOrder(Nodo* T){
    if(estaVacio(T)) return;
    mostrarInOrder(T->izq);
    cout << T->dato << "\n";
    mostrarInOrder(T->der);
}

void mostrarPreOrder(Nodo* T){
    if(estaVacio(T)) return;
    cout << T->dato << "\n";
    mostrarPreOrder(T->izq);
    mostrarPreOrder(T->der);
}

void mostrarPostOrder(Nodo* T){
    if(estaVacio(T)) return;
    mostrarPostOrder(T->izq);
    mostrarPostOrder(T->der);
    cout << T->dato << "\n";
}

bool esta(Nodo* T, int n){
    if(estaVacio(T)) return false;
    if(T->dato == n) return true;
    if(n <= T->dato) return esta(T->izq, n);
    else return esta(T->der, n);
}

int inOrderSuccessor(Nodo* T){
    if(T->izq == NULL) return T->dato;
    return inOrderSuccessor(T->izq);
}

Nodo* borrar(Nodo* T, int n){
    if(T->dato == n){
        if(T->izq == NULL && T->der == NULL){
            free(T);
            return NULL;
        }
        if(T->izq == NULL || T->der == NULL){
            Side lado_nulo = (T->izq == NULL)?LEFT:RIGHT;
            Nodo* temp = T;
            if(lado_nulo == LEFT) T = T->der;
            else T = T->izq;
            free(temp);
            return T;
        }
        int sustituto = inOrderSuccessor(T->der);
        T->dato = sustituto;
        T->der = borrar(T->der, sustituto);
        return T;
    }
    if(n < T->dato) T->izq = borrar(T->izq, n);
    else T->der = borrar(T->der, n);
    return T;
}

void eliminarOcurrencias(Nodo* T, int n){
    while(esta(T, n)){
        borrar(T, n);
    }
}

vector<int> inOrder(Nodo* T, vector<int> v){
    if(!estaVacio(T)){
        mostrarInOrder(T->izq);
        v.push_back(T->dato);
        mostrarInOrder(T->der);
    }
    return v;
}

bool validarBST(Nodo* T){
    vector<int> v;
    v = inOrder(T, v);
    int temp = v.at(0);
    for(int i = 0; i < v.size(); i++){
        if(i != 0){
            if(v.at(i) > temp) temp = v.at(i);
            else  return false;
        }
    }
    return true;
}

void destruirArbol(Nodo* T){
    if(estaVacio(T)) return;
    destruirArbol(T->izq);
    destruirArbol(T->der);
    free(T);
}


bool ancestroComun(Nodo* node, int a, int b){
    if(node != NULL){
        if(node->dato == a || node->dato == b) return true;
        else{
            bool f1 = ancestroComun(node->der, a, b);
            bool f2 = ancestroComun(node->izq, a, b);
            if(f1 && f2) cout << node->dato << endl;
            else if(f1 || f2 ) return true;
        }
    }
}

int numeroSaltos(Nodo* node, int a, int b){
    if(node != NULL){
        if(node->dato == a || node->dato == b) {
            int f1 = numeroSaltos(node->der, a, b), f2 = numeroSaltos(node->izq, a, b);
            if(f1 != 0 || f2 != 0){
                cout << f1 + f2;
            }
            else return 1;
        }
        else{
            int f1 = 0, f2 = 0;
            if(node->der != NULL) f1 = numeroSaltos(node->der, a, b);
            if(node->izq != NULL) f2 = numeroSaltos(node->izq, a, b);
            int r = f1+f2;
            if(f1 != 0 && f2 != 0) cout << r << endl;
            else if(f1 != 0 || f2 != 0){
                return 1+f1+f2;
            }
            else if(f1 == 0 && f2 == 0) return 0;
        }
    }
}

Nodo* arrayToTree(Nodo* T, vector<int> v, int i, int f){
    if(f == i) T = insertar(T, v.at(i));
    else {
        int m = floor((f-i)/2);
        if(m == 0) {
            T = insertar(T, v.at(i));
            T->der = arrayToTree(T->der, v, i+m+1, f);
        }
        else {
            T = insertar(T, v.at(i+m));
            T->izq = arrayToTree(T->izq, v, i, i+m-1);
            T->der = arrayToTree(T->der, v, i+m+1, f);
        }
    }
    return T;
}

int menorQue(int n, vector<int> v, int i, int  f){
    int cont = 0;
    for(int j = i; j <= f; j++){
        if(n > v.at(j)) cont++;
    }
    return cont;
}

int mayorQue(int n, vector<int> v, int i, int f){
    for(int j = i; j <= f; j++){
        if(n < v.at(j)) return j;
    }
    return 0;
}

Nodo* arrayToTreePre(Nodo* T, vector<int> v, int i, int f){
    T = insertar(T, v.at(i));
    if(i != f){
        int tempF = i+menorQue(v.at(i), v, i, f);
        if(v.at(i) > v.at(i+1)) T->izq = arrayToTreePre(T->izq, v, i+1, tempF);
        int tempI = mayorQue(v.at(i), v, i, f);
        if(tempI != i) T->der = arrayToTreePre(T->der, v, tempI, f);
    }
    return T;
}

int main(){
    /*Nodo* T;

    T = crear();

    //cout << estaVacio(T) << "\n";

    //T = insertar(T, 50);

    //cout << estaVacio(T) << "\n";
    //cout << T->dato << "\n";

    T = insertar(T, 40);
    T = insertar(T, 70);
    T = insertar(T, 60);
    T = insertar(T, 30);
    T = insertar(T, 80);
    T = insertar(T, 20);
    T = insertar(T, 45);
    T = insertar(T, 10);
    T = insertar(T, 90);*/

    /*vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    v.push_back(40);
    v.push_back(50);
    v.push_back(60);
    v.push_back(70);
    v.push_back(80);
    v.push_back(90);
    Nodo* M = crear();
    M = arrayToTree(M, v, 0, v.size()-1);
    cout << endl;
    mostrarPreOrder(M);
    cout << validarBST(M) << endl;*/

    /*vector<int> v;
    v.push_back(50);
    v.push_back(20);
    v.push_back(10);
    v.push_back(30);
    v.push_back(40);
    v.push_back(70);
    v.push_back(60);
    v.push_back(80);
    v.push_back(90);
    Nodo* M = crear();
    M = arrayToTreePre(M, v, 0, v.size()-1);
    mostrarInOrder(M);*/

    /*mostrarPreOrder(T);
    mostrarInOrder(T);
    mostrarPostOrder(T);

    cout << esta(T, 30) << "\n";
    cout << esta(T, 25) << "\n";*/

    /*T = borrar(T, 10);
    mostrarInOrder(T);
    cout << "\n\n";
    T = borrar(T, 30);
    mostrarInOrder(T);
    cout << "\n\n";
    T = borrar(T, 40);
    mostrarInOrder(T);
    cout << "\n\n";
    T = borrar(T, 50);
    mostrarInOrder(T);
    eliminarOcurrencias(T, 30);*/
    //cout << endl;
    //mostrarInOrder(T);
    //cout << endl;
    /*cout << validarBST(T) << endl;
    cout << endl;
    ancestroComun(T, 10, 90);
    cout << endl;
    numeroSaltos(T, 10, 30);*/
    //destruirArbol(T);
    return 0;
}