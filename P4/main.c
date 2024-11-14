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

// Inserta un elemento `x` en el montículo `m`, manteniendo la propiedad de mínimo.
void insertarMonticulo(pmonticulo m, int x) {
    int i, padre;  

    // Verificamos si el montículo está lleno
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
        m->vector[i] = m->vector[(i - 1) / 2];  // Subimos el valor del padre
        i = (i - 1) / 2;  // Actualizamos `i` al índice del padre
    }

    // Colocamos el nuevo elemento en su posición correcta
    m->vector[i] = x;
}

// Elimina el elemento mínimo del montículo `m` (la raíz) y mantiene la propiedad de mínimo.
void quitarMenor(pmonticulo m) {
    int menor, ultimo, i, hijo;  

    // Verificamos si el montículo está vacío
    if (m->ultimo == -1) {
        printf("Montículo vacío\n");
        return;
    }

    // Almacenamos el mínimo (la raíz) y el último elemento del montículo
    menor = m->vector[0];
    ultimo = m->vector[m->ultimo];

    // Reducimos el tamaño del montículo
    m->ultimo--;

    // Comenzamos desde la raíz (índice 0) y ajustamos hacia abajo (down-heapify)
    i = 0;
    while (2 * i + 1 <= m->ultimo) {
        hijo = 2 * i + 1;  // Calculamos el índice del hijo izquierdo

        // Si el hijo derecho existe y es menor que el hijo izquierdo, seleccionamos el hijo derecho
        if (hijo < m->ultimo && m->vector[hijo] > m->vector[hijo + 1]) {
            hijo++;
        }

        // Si el último elemento es menor o igual al hijo más pequeño, hemos encontrado su posición
        if (ultimo <= m->vector[hijo]) {
            break;
        }

        // Subimos el valor del hijo más pequeño
        m->vector[i] = m->vector[hijo];
        i = hijo;  // Movemos `i` hacia abajo en el árbol
    }

    // Colocamos el último elemento en la posición correcta
    m->vector[i] = ultimo;
}

// Verifica si el montículo `m` cumple con la propiedad de mínimo.
// Devuelve 1 si el montículo es válido, y 0 en caso contrario.
int esMonticuloValido(const pmonticulo m) {
    int hijoIzq, hijoDer, i;  

    // Recorremos todos los nodos que tienen hijos (hasta el penúltimo nivel)
    for (i = 0; i <= (m->ultimo - 1) / 2; i++) {
        hijoIzq = 2 * i + 1;  // Calculamos el índice del hijo izquierdo
        hijoDer = 2 * i + 2;  // Calculamos el índice del hijo derecho

        // Verificamos que el nodo actual sea menor o igual que su hijo izquierdo
        if (hijoIzq <= m->ultimo && m->vector[i] > m->vector[hijoIzq]) {
            return 0;  // El montículo no es válido si el padre es mayor que el hijo izquierdo
        }

        // Verificamos que el nodo actual sea menor o igual que su hijo derecho
        if (hijoDer <= m->ultimo && m->vector[i] > m->vector[hijoDer]) {
            return 0;  // El montículo no es válido si el padre es mayor que el hijo derecho
        }
    }
    return 1;  // El montículo cumple con la propiedad de mínimo
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
