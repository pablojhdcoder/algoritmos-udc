#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

#define LONGITUD_CLAVE 30
#define LONGITUD_SINONIMOS 300
#define N 38197
#define TEST_N 11
#define MAX_CLAVES 19062 // Número de claves en sinonimos.txt

typedef struct {
    char clave [LONGITUD_CLAVE];
    char sinonimos [LONGITUD_SINONIMOS];
}item;


typedef struct entrada_ {
    int ocupada;
    item data;
} entrada;

typedef int pos;
typedef entrada *tabla_cerrada;


// Definición de la estructura para las cotas
typedef struct {
    double cotaSubestimada;
    double cotaAjustada;
    double cotaSobreestimada;
    const char *strCotaSubestimada;
    const char *strCotaAjustada;
    const char *strCotaSobreestimada;
} Cotas;


// Cabeceras de funciones
void inicializar_cerrada(tabla_cerrada *diccionario, int tam);
unsigned int dispersionA(char *clave, int tamTabla);
unsigned int dispersionB(char *clave, int tamTabla);
unsigned int resol_lineal(int pos_ini, int num_intento);
unsigned int resol_cuadratica(int pos_ini, int num_intento);
unsigned int resol_doble(int pos_ini, int num_intento);
int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam,
                     unsigned int (*dispersion)(char *, int),
                     unsigned int (*resol_colisiones)(int pos_ini, int num_intento));
pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones,
                   unsigned int (*dispersion)(char *, int),
                   unsigned int (*resol_colisiones)(int pos_ini, int num_intento));
void mostrar_cerrada(tabla_cerrada diccionario, int tam);
int leer_sinonimos(item datos[]);
void test();
void testdiccionario(unsigned int (*dispersion)(char *, int), unsigned int (*resolucion)(unsigned int, int));
void imprimirTablas();
void pruebaInsercionYBusqueda();
//Mide el tiempo de ejecución
double medirTiempo(item sinonimos[],tabla_cerrada *diccionario,
                    unsigned int (*resol_colisiones)(int pos_ini, int num_intento),
                    unsigned int (*dispersion)(char*,int), int n, bool *promedio);

//Posicion en la que se encuentra la clave, no buscar en el orden de inserción de las claves ya que las primeras no va a haber colisiones y los datos para n pequeños no van a aparecer coliciones y los datos no saldrán lineales
//Por ello escoger al azar, generar número aleatorio y buscar ese elemento
//Numero de coliciones que se produjeron al introducir la clave

int main(void) {
    item sinonimos[MAX_CLAVES];
    srand(time(NULL));
    int cantidad = leer_sinonimos(sinonimos);
    if (cantidad != MAX_CLAVES) {
        printf("Error al leer el archivo de sinónimos.\n");
        return EXIT_FAILURE;
    }
    test();
    imprimirTablas(sinonimos);
    return 0;
}

//Función de dispersión n
unsigned int ndispersion(char *clave, int tamTabla) {
    if (strcmp(clave, "ANA")==0) return 7;
    if (strcmp(clave, "JOSE")==0) return 7;
    if (strcmp(clave, "OLGA")==0) return 7;
    return 6;
}

// Función de dispersión A
unsigned int dispersionA(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor += clave[i];
    return valor % tamTabla;
}

// Función de dispersión B
unsigned int dispersionB(char *clave, int tamTabla) {
    int i, n = MIN(8, strlen(clave));
    unsigned int valor = clave[0];
    for (i = 1; i < n; i++)
        valor = (valor<<5) + clave[i]; //El desplazamiento de 5 bits equivale a multipicar por 32
    return valor % tamTabla;
}

// Exploración lineal
unsigned int resol_lineal(int pos_ini, int num_intento) {
    return num_intento;
}

// Exploración cuadrática
unsigned int resol_cuadratica(int pos_ini, int num_intento) {
    return num_intento * num_intento;
}

// Exploración doble
unsigned int resol_doble(int pos_ini, int num_intento) {
    return num_intento * (10007 - (pos_ini % 10007));
}

unsigned int resol_doble_test(int pos_ini, int num_intento) {
    return num_intento * (5 - (pos_ini % 5));
}

//Esta función devuelve el numero de claves leidas
int leer_sinonimos(item datos[]) {
    char c;
    int i, j;
    FILE *archivo;
    if ((archivo = fopen("sinonimos.txt", "r")) == NULL) {
        printf("Error al abrir ’sinonimos.txt’\n");
        return(EXIT_FAILURE);
    }
    for (i = 0; fscanf(archivo, "%s", datos[i].clave) != EOF; i++) {
        if ((c = fgetc(archivo)) != '\t') {
            printf("Error al leer el tabulador\n");
            return(EXIT_FAILURE);
        }
        for (j = 0; (c = fgetc(archivo)) != '\n'; j++) {
            if (j < LONGITUD_SINONIMOS - 1)
                datos[i].sinonimos[j] = c;
        }
        datos[i].sinonimos[MIN(j, LONGITUD_SINONIMOS -1)] = '\0';
    }
    if (fclose(archivo) != 0) {
        printf("Error al cerrar el fichero\n");
        return(EXIT_FAILURE);
    }
    return(i);
}

void inicializar_cerrada(tabla_cerrada *diccionario, int tam) {
    int i;
    *diccionario = malloc(tam * sizeof(entrada));
    if(*diccionario == NULL) {
        perror("Error al reservar la memoria necesaria para el diccionario");
        return;
    }

    for (i = 0; i < tam; i++) {
        (*diccionario)[i].ocupada = 0;
        (*diccionario)[i].data.clave[0] = '\0';
        (*diccionario)[i].data.sinonimos[0] = '\0';
    }
}


// Función para insertar en la tabla cerrada
int insertar_cerrada(char *clave, char *sinonimos, tabla_cerrada *diccionario, int tam,
                     unsigned int (*dispersion)(char *, int),
                     unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    int colisiones = 0, intento = 0;
    unsigned int pos_ini = dispersion(clave, tam);
    unsigned index = pos_ini;

    while ((*diccionario)[index].ocupada && intento < tam) {
        colisiones++;
        intento++;
        index = (pos_ini + resol_colisiones(pos_ini, intento)) % tam;
        // Asegura que el índice se mantenga dentro del tamaño de la tabla
    }

    if (!(*diccionario)[index].ocupada) {
        (*diccionario)[index].ocupada = 1;
        strncpy((*diccionario)[index].data.clave, clave, LONGITUD_CLAVE);
        strncpy((*diccionario)[index].data.sinonimos, sinonimos, LONGITUD_SINONIMOS);
    } else {
        fprintf(stderr, "Error: Tabla de dispersión llena\n");
    }

    return colisiones;
}


// Función para buscar en la tabla cerrada
pos buscar_cerrada(char *clave, tabla_cerrada diccionario, int tam, int *colisiones,
                   unsigned int (*dispersion)(char *, int),
                   unsigned int (*resol_colisiones)(int pos_ini, int num_intento)) {
    unsigned int pos_ini = dispersion(clave, tam); // Cambiado a 'unsigned int'
    unsigned int index = pos_ini;  // Cambiado a 'unsigned int'
    int intento = 0;

    // Realiza la búsqueda mientras la posición esté ocupada y no haya alcanzado el límite de intentos
    while (diccionario[index].ocupada && intento < tam) {
        if (strcmp(diccionario[index].data.clave, clave) == 0)  // Si la clave coincide, retorna el índice
            return index;

        // Si la clave no coincide, incrementa el contador de colisiones y el número de intentos
        (*colisiones)++;
        intento++;

        // Calcula el nuevo índice aplicando la resolución de colisiones
        index = (pos_ini + resol_colisiones(pos_ini, intento)) % tam;
    }

    // Si la clave no se encontró, retorna -1
    return -1;
}



// Mostrar el contenido de la tabla
void mostrar_cerrada(tabla_cerrada diccionario, int tam) {
    int i;
    printf("{\n");
    for (i = 0; i < tam; i++) {
        if (diccionario[i].ocupada) {
            printf("%d- (%s )\n", i, diccionario[i].data.clave);
        } else {
            printf("%d-\n", i);
        }
    }
    printf("}\n");
}




// Función de prueba para un conjunto reducido de datos
void test_diccionario(unsigned int (*dispersion)(char *, int), unsigned int (*resolucion)(int, int)) {
    tabla_cerrada diccionario;
    char *nombres[] = {"ANA", "LUIS", "JOSE", "OLGA", "ROSA", "IVAN"};
    int total_colisiones = 0, colisiones, i;
    pos posicion;

    inicializar_cerrada(&diccionario, TEST_N);
    for (i = 0; i < 6; i++) {
        total_colisiones += insertar_cerrada(nombres[i], "",&diccionario, TEST_N, dispersion, resolucion);
    }
    mostrar_cerrada(diccionario, TEST_N);
    printf("Numero total de colisiones al insertar los elementos: %d\n", total_colisiones);

    // Búsqueda de elementos
    for (i = 0; i < 6; i++) {
        colisiones = 0;
        posicion = buscar_cerrada(nombres[i], diccionario, TEST_N, &colisiones, dispersion, resolucion);
        if (posicion != -1)
            printf("Al buscar: %s, encuentro: %s, colisiones: %d\n", nombres[i], nombres[i], colisiones);
        else
            printf("No encuentro: %s, colisiones: %d\n", nombres[i], colisiones);
    }

    //Probar a buscar un elemento que no está en la tabla
    colisiones = 0;
    posicion = buscar_cerrada("CARLOS", diccionario, TEST_N, &colisiones, dispersion, *resolucion);
    if(posicion == -1) {
        printf("No encuentro: CARLOS, colisiones: %d\n", colisiones);
    }
    free(diccionario);
}

void test () {
    printf("***TABLA CERRADA LINEAL\n");
    test_diccionario(ndispersion,resol_lineal);
    printf("***TABLA CERRADA CUADRATICA\n");
    test_diccionario(ndispersion,resol_cuadratica);
    printf("***TABLA CERRADA DOBLE\n");
    test_diccionario(ndispersion,resol_doble_test);
}



// Función para imprimir el encabezado de las tablas
void imprimirEncabezado(const char *nombreDispersion, const char *tipoResolucion, int numSinonimos, int numColisiones, Cotas *cotas) {
    // Imprime el título con el nombre de la función y el tipo de resolución
    printf("\n***Dispersion cerrada %s con %s\n"
                "Insertando %d elementos... Numero total de colisiones: %d\n"
                "Buscando n elementos...\n",nombreDispersion, tipoResolucion, numSinonimos, numColisiones);    // Encabezado de la tabla
    printf("\n%13s%16s%17s%s%19s%s%16s%s\n",
    "n",
    "t(n)",
    "t(n)/", cotas->strCotaSubestimada,
    "t(n)/", cotas->strCotaAjustada,
    "t(n)/", cotas->strCotaSobreestimada);
}

// Mide el tiempo en microsegundos
double microsegundos() {
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}


//Mide el tiempo de ejecución
double medirTiempo(item sinonimos[],tabla_cerrada *diccionario,
                    unsigned int (*resol_colisiones)(int pos_ini, int num_intento),
                    unsigned int (*dispersion)(char*,int), int n, bool *promedio) {

    double t1, t2, t, tTotal,tInicializacion;
    int i, j, k = 1000, indice, colisiones = 0 ;                       //Error: En vez de poner el tamaño máximo que voy a tratar es mejor poner n
    t1 = microsegundos();           // Mide el tiempo total (inicialización + ejecución)
    for (i = 0; i < n; i++) {
        indice = rand() % MAX_CLAVES;
        buscar_cerrada(sinonimos[indice].clave, *diccionario, N, &colisiones, dispersion, resol_colisiones);
    }
    t2 = microsegundos();
    tTotal = t2 - t1;

    t1 = microsegundos();           // Mide solo el tiempo de la inicialización
    for (i = 0; i < n; i++) {
        indice = rand() % MAX_CLAVES;
    }
    t2 = microsegundos();
    tInicializacion = t2 - t1;
    t = tTotal - tInicializacion;


    //Si el tiempo es menor que 500 microsegundos
    if (t < 500) {
        t1 = microsegundos();           // Mide el tiempo total (inicialización + ejecución)
        for (i = 0; i < k; i++) {
            for (j = 0; j < n; j++) {
                indice = rand() % MAX_CLAVES;
                buscar_cerrada(sinonimos[indice].clave, *diccionario, N, &colisiones, dispersion, resol_colisiones);
            }
        }
        t2 = microsegundos();
        tTotal = t2 - t1;

        t1 = microsegundos();           // Mide solo el tiempo de la inicialización
        for (i = 0; i < k; i++) {
            for (j = 0; j < n; j++) {
                indice = rand() % MAX_CLAVES;
            }
        }
        t2 = microsegundos();
        tInicializacion = t2 - t1;
        t = (tTotal - tInicializacion) / k;
        *promedio = true; //Indica que se realizó un promedio
    }
    return t;
}

//Devuelve la cota subestimada para el algoritmo correspondiente
double cotaSubestimada(int n,Cotas *cotas) {
    return pow(n,cotas->cotaSubestimada);
}

//Devuelve la cota ajustada para el algoritmo correspondiente
double cotaAjustada(int n, Cotas *cotas) {
    return pow(n,cotas->cotaAjustada);
}

double cotaSobreestimada(int n, Cotas *cotas) {
    return pow(n,cotas->cotaSobreestimada) * log2(n);
}

// Función para imprimir una fila de la tabla
void imprimirFila(int n, double t, double x, double y, double z, bool promedio) {
    if (promedio) {
        printf("*");    //Marca si se hizo un promedio
    } else {
        printf(" ");    //Marca si no se hizo un promedio, para mantener la alineación
    }
    printf("%12d%17.4f%20.6f%25.6f%25.6f\n", n, t, x, y, z);
}

//Imprime los tiempos de ejecución de la función dada
void imprimirTiempos(item sinonimos[MAX_CLAVES],
                    unsigned int(*dispersion)(char*, int),
                    unsigned int(*resol_colisiones)(int pos_ini, int num_intento),
                    const char *nombreDispersion, const char *tipoResolucion, Cotas *cotas) {
    int i, colisiones = 0;
    double t, x, y, z;
    bool promedio;
    tabla_cerrada diccionario;
    inicializar_cerrada(&diccionario,N);
    for (i = 0; i < MAX_CLAVES; i++) {
        colisiones += insertar_cerrada(sinonimos[i].clave,sinonimos[i].sinonimos,&diccionario,N,dispersion,resol_colisiones);
    }
    imprimirEncabezado(nombreDispersion, tipoResolucion,N,colisiones,cotas);

    for (i = 125; i <= 16000; i *= 2) {
        promedio = false;
        t = medirTiempo(sinonimos, &diccionario, resol_colisiones, dispersion,i, &promedio);
        x = t / cotaSubestimada(i, cotas);
        y = t / cotaAjustada(i, cotas);
        z = t / cotaSobreestimada(i, cotas);

        imprimirFila(i, t, x, y, z, promedio);
    }
    free(diccionario);
}



// Imprime las tablas de tiempos
void imprimirTablas(item sinonimos[MAX_CLAVES]){
    Cotas cotas;

    // Pruebas con dispersión cerrada lineal y diferentes cotas para dispersionA
    cotas = (Cotas){0.81, 1.01, 1.01, "n^0.81", "n^1.01", "n^1.01*log(n)"};
    imprimirTiempos(sinonimos,dispersionA, resol_lineal, "lineal", "dispersión A", &cotas);

    // Pruebas con dispersión cerrada lineal y diferentes cotas para dispersionB
    cotas = (Cotas){0.85, 1.05, 1.05, "n^0.85", "n^1.05", "n^1.05*log(n)"};
    imprimirTiempos(sinonimos,dispersionB, resol_lineal, "lineal", "dispersión B", &cotas);

    // Pruebas con dispersión cerrada cuadrática y diferentes cotas para dispersionA
    cotas = (Cotas){0.81, 1.01, 1.01, "n^0.81", "n^1.01", "n^1.01*log(n)"};
    imprimirTiempos(sinonimos,dispersionA, resol_cuadratica, "cuadrática", "dispersión A", &cotas);


    // Pruebas con dispersión cerrada cuadrática y diferentes cotas para dispersionB
    cotas = (Cotas){0.83, 1.03, 1.03, "n^0.83", "n^1.03", "n^1.03*log(n)"};
    imprimirTiempos(sinonimos,dispersionB, resol_cuadratica, "cuadrática", "dispersión B", &cotas);

    // Pruebas con resolución doble y diferentes cotas para dispersionA
    cotas = (Cotas){0.81, 1.01, 1.01, "n^0.81", "n^1.01", "n^1.01*log(n)"};
    imprimirTiempos(sinonimos,dispersionA, resol_doble, "doble", "dispersión A", &cotas);

    // Pruebas con resolución doble y diferentes cotas para dispersionB
    cotas = (Cotas){0.82, 1.02, 1.02, "n^0.82", "n^1.02", "n^1.02*log(n)"};
    imprimirTiempos(sinonimos,dispersionB, resol_doble, "doble", "dispersión B", &cotas);

}//Comprobar empíricamente la complejidad O(n) del algoritmo que se utiliza para buscar