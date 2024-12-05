#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 256000


struct monticulo {
    int ultimo;     //Ultimo celda ocupada del vector, ultimo = 1 => 2 nodos( pos 0 y 1) vacio -1 lleno 255999
    int vector[TAM];
};
typedef struct monticulo * pmonticulo;

void test();
void iniMonticulo(pmonticulo *m);        //ultimo -1
void insertarMonticulo(pmonticulo m, int x);    //Añade x al monticulo
void quitarMenor(pmonticulo m);
int consultarMenor(const pmonticulo m);     //Que elemento ocupa la reíz
void crearMonticulo(pmonticulo m, int v [], int n);     //Construye nuevo montículo a partir de v
void ordenarPorMonticulos(int v[], int n);
int comprobarOrdenacion(const int v[], int n);
void inicializar_semilla();

void aleatorio(int v[], int n);
void ascendente(int v[], int n);
void descendente(int v[], int n);
void imprimirArray(const int v[], int n);

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
    inicializar_semilla();
    pmonticulo m;       // Declaración del puntero
    iniMonticulo(&m);   // Inicialización del montículo
    test();             // Llama al test
    free(m);            // Libera la memoria al final
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
    (*m)->ultimo = -1;  // Inicializa el montículo vacío
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
    int i;
    pmonticulo m;
    iniMonticulo(&m);           // Inicializa el montículo
    crearMonticulo(m, v, n);    // Construye el montículo con el array `v`

    for (i = n-1; i >=0; i--) {
        v[i] = consultarMenor(m); // Obtiene el menor elemento (raíz del montículo)
        quitarMenor(m);           // Elimina la raíz y ajusta el montículo
    }

    free(m); // Libera la memoria asignada al montículo
}


void inicializar_semilla() {
    srand(time(NULL));
}

void aleatorio(int v[], int n) {/* se generan números pseudoaleatorio entre -n y +n */
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}
void ascendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
        v[i] = i;
}

void descendente(int v [], int n) {
    int i;
    for (i=0; i < n; i++)
        v[i] = n-i;
}

// Imprime un array
void imprimirArray(const int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (i == n - 1) {
            printf("%d", v[i]);  // Último elemento sin coma
        } else {
            printf("%d, ", v[i]);  // Elementos anteriores con coma
        }
    }
    printf("\n");
}


// Verifica si un array está ordenado
int comprobarOrdenacion(const int v[], int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        if (v[i] > v[i + 1]) {
            return 0;
        }
    }
    return 1;
}

// Función para realizar la ordenación y mostrar resultados
void ordenacion(void (inicializacion)(int[], int), void (algoritmo)(int[], int), const int n, int v[n]) {
    inicializacion(v, n);
    imprimirArray(v, n);
    printf("Ordenado? %d\n", comprobarOrdenacion(v, n));
    printf("Ordenando...\n");
    algoritmo(v, n);
    imprimirArray(v, n);
    printf("Ordenado? %d\n\n", comprobarOrdenacion(v, n));
}

void imprimir_test(const int n, int v[n], void (algoritmo)(int[], int), char nombre_algoritmo[]) {
    printf("Ordenacion %s con inicializacion aleatoria\n", nombre_algoritmo);
    ordenacion(aleatorio, algoritmo, n, v);
    printf("Ordenacion %s con inicializacion descendente\n", nombre_algoritmo);
    ordenacion(descendente, algoritmo, n, v);
    printf("Ordenacion %s con inicializacion ascendente\n", nombre_algoritmo);
    ordenacion(ascendente, algoritmo, n, v);
}



void test() {
    inicializar_semilla();
    int n = 10;
    int v[n];
    imprimir_test(n, v, ordenarPorMonticulos, "por monticulos");
}

// Mide el tiempo en microsegundos
double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}



//Mide el tiempo de ejecución
double medirTiempo(void (*inicializacion)(int*, int), int n, bool *promedio) {
    double t1, t2, t, tTotal, tInicializacion;
    int j, k = 1000;
    int *v = malloc(n * sizeof(int));  // Array de tamaño n


    // Inicializar el array si se proporciona una función de inicialización
    if (inicializacion != NULL) {
        inicializacion(v, n);
    }

    // Medir tiempo de ejecución del algoritmo
    t1 = microsegundos();
    ordenarPorMonticulos(v, n);
    t2 = microsegundos();
    t = t2 - t1;

    // Si el tiempo es menor que 500 microsegundos
    if (t < 500) {
        t1 = microsegundos();  // Medir tiempo con promediado
        for (j = 0; j < k; j++) {
            if (inicializacion != NULL) {
                inicializacion(v, n);  // Reinicializar array antes de cada ejecución
            }
            ordenarPorMonticulos(v, n);
        }
        t2 = microsegundos();
        tInicializacion = t2 - t1;
        t = (tTotal - tInicializacion) / k;
        *promedio = true; //Indica que se realizó un promedio
    }else {
        *promedio = false;  // No se utilizó promedio
    }
    free(v);
    return t;
}
