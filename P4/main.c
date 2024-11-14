#include <stdio.h>

#define TAM 256000


struct monticulo {
    int ultimo;     //Ultimo celda ocupada del vector, ultimo = 1 => 2 nodos( pos 0 y 1) vacio -1 lleno 255999
    int vector[TAM];
};
typedef struct monticulo * pmonticulo;


void iniMonticulo(pmonticulo m);        //ultimo -1
void insertarMonticulo(pmonticulo m, int x);    //Añade x al monticulo
void quitarMenor(pmonticulo m);
int consultarMenor(const pmonticulo m);     //Que elemento ocupa la reíz
void crearMonticulo(pmonticulo m, int v [], int n);     //Construye nuevo montículo a partir de v

//implementar también hundir (en insertar) y flotar
//diseñar test de prueba propio

//Si indice del vector empieza en 1 => hijo izq = 2 * poso, hijo derecho = 2 * pos +1 y padre pos/2

/*Recalcular pos tengo que pasar a empezar en 1
 * tengo que pasar a la escala de 1, 2, ..
 * hi ( i ) = 2 (i+1) -1 = 2i +2 -1 = 2i+1
 * hd (i) = 2i +2
 * pad (i) = 1+1/2 -1 = 1+1-2/2 = i-1/2
 */

//Demostrar que añadir n numeros tiene nlogn es decir insertar n veces x número

//Punto 4, comparar los resultados con los implementados en la práctica 2.

int consultarMenor(const pmonticulo m) {
    if (m->ultimo == -1) {
        printf("El montículo está vacío.\n");
        return -1; // Se podría elegir un valor especial o generar un error
    }
    // El menor elemento está siempre en la raíz (posición 0)
    return m->vector[0];
}

int main(void) {
    printf("Hello, World!\n");
    return 0;
}
