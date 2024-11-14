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

void insertarMonticulo(pmonticulo m, int x) {
    int i, padre;  

    if (m->ultimo == TAM - 1) {
        printf("Montículo lleno\n");
        return;
    }
// Incrementamos `ultimo` para agregar el nuevo elemento al final del montículo
    m->ultimo++;
    i = m->ultimo;

    // Ajuste hacia arriba (up-heapify): movemos el nuevo elemento hacia arriba 
    // hasta encontrar su posición correcta para mantener la propiedad de mínimo
    while (i > 0 && x < m->vector[(i - 1) / 2]) {
        m->vector[i] = m->vector[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    m->vector[i] = x;
}

void quitarMenor(pmonticulo m) {
    int menor, ultimo, i, hijo; 

    if (m->ultimo == -1) {
        printf("Montículo vacío\n");
        return;
    }

    menor = m->vector[0];
    ultimo = m->vector[m->ultimo];
    m->ultimo--;
    i = 0;

    // Ajuste hacia abajo (down-heapify)
    while (2 * i + 1 <= m->ultimo) {
        hijo = 2 * i + 1;

        // Elegir el hijo menor
        if (hijo < m->ultimo && m->vector[hijo] > m->vector[hijo + 1]) {
            hijo++;
        }

        if (ultimo <= m->vector[hijo]) {
            break;
        }

        m->vector[i] = m->vector[hijo];
        i = hijo;
    }
    m->vector[i] = ultimo;
}


//FUNCION OPCIONAL

int esMonticuloValido(const pmonticulo m) {
    int hijoIzq, hijoDer, i;  

    for (i = 0; i <= (m->ultimo - 1) / 2; i++) {
        hijoIzq = 2 * i + 1;  // Calculamos el índice del hijo izquierdo
        hijoDer = 2 * i + 2;  // Calculamos el índice del hijo derecho

        // Verificamos que el nodo actual sea menor o igual que su hijo izquierdo
        if (hijoIzq <= m->ultimo && m->vector[i] > m->vector[hijoIzq]) {
            return 0;  // El montículo no es válido
        }

        // Verificamos que el nodo actual sea menor o igual que su hijo derecho
        if (hijoDer <= m->ultimo && m->vector[i] > m->vector[hijoDer]) {
            return 0;  // El montículo no es válido
        }
    }
    return 1;  // El montículo es válido
}



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
