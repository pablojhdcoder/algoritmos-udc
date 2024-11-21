#include <stdio.h>
#include <stdlib.h>

#define TAM 256000


struct monticulo {
    int ultimo;     //Ultimo celda ocupada del vector, ultimo = 1 => 2 nodos( pos 0 y 1) vacio -1 lleno 255999
    int vector[TAM];
};
typedef struct monticulo * pmonticulo;


void iniMonticulo(pmonticulo *m);        //ultimo -1
void insertarMonticulo(pmonticulo m, int x);    //Añade x al monticulo
void quitarMenor(pmonticulo m);
int consultarMenor(const pmonticulo m);     //Que elemento ocupa la reíz
void crearMonticulo(pmonticulo m, int v [], int n);     //Construye nuevo montículo a partir de v

//implementar también hundir (en insertar) y flotar
//diseñar test de prueba propio

//Si indice del vector empieza en 1 => hijo izq = 2 * poso, hijo derecho = 2 * pos +1 y padre pos/2

/*Recalcular pos tengo que pasar a empezar en 1
 * tengo que pasar a la escala de 1, 2, ..
 * hi (i) = 2 (i+1) -1 = 2i +2 -1 = 2i+1
 * hd (i) = 2i +2
 * pad (i) = 1+1/2 -1 = 1+1-2/2 = i-1/2
 */

//Demostrar que añadir n numeros tiene nlogn es decir insertar n veces x número

//Punto 4, comparar los resultados con los implementados en la práctica 2.



int main(void) {
    pmonticulo m;
    iniMonticulo(m);
    //test();
    return 0;
}

static void intercambiar (pmonticulo m, int pos1, int pos2) {
    int tmp;
    tmp = m->vector[pos1];
    m->vector[pos1] = m->vector[pos2];
    m->vector[pos2] = tmp;
}

static void flotar(pmonticulo m, int i) {
    while (i > 0 && m->vector[(i - 1) / 2] < m->vector[i]) {
        intercambiar(m, (i - 1) / 2, i);                // Intercambiamos el nodo actual con su padre
        i = (i - 1) / 2;                // Subimos al índice del padre
    }
}

static void hundir(pmonticulo m, int i) {
    int hijoIzq, hijoDer, j;

    do {
        hijoIzq = 2 * i + 1;  // Índice del hijo izquierdo
        hijoDer = 2 * i + 2;  // Índice del hijo derecho
        j = i;                // Índice original antes de buscar el mayor

        // Comparamos con el hijo derecho
        if (hijoDer <= m->ultimo && m->vector[hijoDer] > m->vector[i]) {
            i = hijoDer;
        }

        // Comparamos con el hijo izquierdo
        if (hijoIzq <= m->ultimo && m->vector[hijoIzq] > m->vector[i]) {
            i = hijoIzq;
        }

        // Intercambiamos los elementos si es necesario
        if (j != i) {
            intercambiar(m, j, i);
        }

    } while (j != i);  // Terminamos cuando el nodo alcanza su posición final
}

void iniMonticulo(pmonticulo *m) {
    *m = malloc(sizeof(struct monticulo));
    if (*m == NULL) {
        fprintf(stderr, "Error al asignar memoria para el montículo.\n");
        exit(EXIT_FAILURE);
    }
    (*m)->ultimo = -1;
}

// Inserta un elemento `x` en el montículo `m`, manteniendo la propiedad de mínimo.
void insertarMonticulo(pmonticulo m, int x) {
    // Verificamos si el montículo está lleno
    if (m->ultimo == TAM - 1) {
        printf("Montículo lleno\n");
        return;
    }

    // Incrementamos `ultimo` y colocamos el nuevo elemento al final
    m->ultimo++;
    m->vector[m->ultimo] = x;

    // Ajuste hacia arriba (flotar) para restaurar la propiedad del montículo
    flotar(m, m->ultimo);
}

// Elimina el elemento mínimo del montículo `m` (la raíz) y mantiene la propiedad de mínimo.
void quitarMenor(pmonticulo m) {
    int ultimo;

    // Verificamos si el montículo está vacío
    if (m->ultimo == -1) {
        printf("Montículo vacío\n");
        return;
    }

    // Guardamos el último elemento
    ultimo = m->vector[m->ultimo];

    // Reducimos el tamaño del montículo
    m->ultimo--;

    // Si el montículo tiene más de un elemento, realizamos el ajuste hacia abajo (hundir)
    if (m->ultimo >= 0) {
        m->vector[0] = ultimo;  // Colocamos el último elemento en la raíz

        // Ajustamos el montículo para restaurar la propiedad de mínimo
        hundir(m, 0);  // Llamamos a la función hundir para restaurar el montículo
    }

}

int consultarMenor(const pmonticulo m) {
    if (m->ultimo == -1) {
        printf("El montículo está vacío.\n");
        return -1; // Se podría elegir un valor especial o generar un error
    }
    // El menor elemento está siempre en la raíz (posición 0)
    return m->vector[0];
}


void crearMonticulo(pmonticulo m, int v[], int n) {
    for (int i = 0; i < n; i++) {
        m->vector[i] = v[i];
    }
    m->ultimo = n-1;

    //Construímos montículo desde el nodo hoja
    for (int i = m->ultimo/2; i >= 0; i--) {
        hundir(m,i);
    }

}

// Ordena un array usando el algoritmo de montículos
void ordenarPorMonticulos(int v[], int n) {
    struct monticulo m;
    crearMonticulo(&m, v, n);
    for (int i = 0; i < n; i++) {
        v[i] = consultarMenor(&m);
        quitarMenor(&m);
    }
}

// Imprime un array
void imprimirArray(const int v[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
}

// Verifica si un array está ordenado
int comprobarOrdenacion(const int v[], int n) {
    for (int i = 1; i < n; i++) {
        if (v[i - 1] > v[i]) return 0;
    }
    return 1;
}

// Prueba el algoritmo de ordenación
void testOrdenacion() {
    int v[] = {5, 3, 8, 4, 2, 7, 1, 6};
    int n = sizeof(v) / sizeof(v[0]);
    struct monticulo m;

    printf("Array original:\n");
    imprimirArray(v, n);

    crearMonticulo(&m, v, n);
    printf("Contenido del montículo inicial:\n");
    imprimirArray(m.vector, m.ultimo + 1);

    ordenarPorMonticulos(v, n);
    printf("Array después de ordenar:\n");
    imprimirArray(v, n);

    if (comprobarOrdenacion(v, n)) {
        printf("El algoritmo de ordenación ha funcionado correctamente.\n");
    } else {
        printf("El algoritmo de ordenación no ha funcionado correctamente.\n");
    }
}
