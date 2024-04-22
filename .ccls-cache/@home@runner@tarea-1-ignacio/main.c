#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para obtener la hora actual

// Definición de la estructura para representar a un paciente
typedef struct {
    char nombre[100];
    int edad;
    char sintoma[100];
    time_t hora_registro; // Para almacenar la hora de registro
    char prioridad[10];   // "Bajo", "Medio" o "Alto"
} Paciente;

// Función para limpiar la pantalla
void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Sistema de Gestión Hospitalaria");
  puts("========================================");

  puts("1) Registrar paciente");
  puts("2) Asignar prioridad a paciente");
  puts("3) Mostrar lista de espera");
  puts("4) Atender al siguiente paciente");
  puts("5) Mostrar pacientes por prioridad");
  puts("6) Salir");
}

// Función para comparar pacientes por prioridad y luego por hora de registro
int comparar_pacientes(const void *a, const void *b) {
    const Paciente *pa = *(const Paciente **)a;
    const Paciente *pb = *(const Paciente **)b;

    // Comparar por prioridad
    int prioridad_cmp = strcmp(pa->prioridad, pb->prioridad);
    if (prioridad_cmp != 0) {
        return prioridad_cmp;
    }

    // Si tienen la misma prioridad, comparar por hora de registro
    return difftime(pa->hora_registro, pb->hora_registro);
}

// Función para mostrar la lista de pacientes en espera
void mostrar_lista_pacientes(List *pacientes) {
    printf("\nLista de pacientes en espera:\n");

    // Contador para almacenar el número de pacientes
    int num_pacientes = 0;

    // Iterar sobre la lista de pacientes y contar el número de elementos
    if (list_first(pacientes) != NULL) {
        num_pacientes++;
        while (list_next(pacientes) != NULL) {
            num_pacientes++;
        }
    }

    // Verificar si la lista está vacía
    if (num_pacientes == 0) {
        printf("\nNo hay pacientes en espera.\n");
        return;
    }

    // Crear un array dinámico para almacenar los punteros a los pacientes
    Paciente **array_pacientes = (Paciente **)malloc(num_pacientes * sizeof(Paciente *));
    if (array_pacientes == NULL) {
        fprintf(stderr, "\nError: No se pudo asignar memoria para ordenar los pacientes.\n");
        return;
    }

    int i = 0;
    Paciente *paciente_actual = (Paciente *)list_first(pacientes);
    array_pacientes[i++] = paciente_actual;
    while ((paciente_actual = (Paciente *)list_next(pacientes)) != NULL) {
        array_pacientes[i++] = paciente_actual;
    }
    // Ordenar el array de pacientes usando la función de comparación
    qsort(array_pacientes, num_pacientes, sizeof(Paciente *), comparar_pacientes);

    // Mostrar la lista ordenada por pantalla
    for (int j = 0; j < num_pacientes; j++) {
        Paciente *paciente_actual = array_pacientes[j];
        printf("Nombre: %s, Edad: %d, Síntoma: %s, Prioridad: %s\n",
               paciente_actual->nombre, paciente_actual->edad,
               paciente_actual->sintoma, paciente_actual->prioridad);
    }

    // Liberar la memoria del array de pacientes
    free(array_pacientes);
}

// Función para mostrar pacientes por prioridad
void mostrar_pacientes_por_prioridad(List *pacientes, char *prioridad) {
    printf("Pacientes con prioridad '%s':\n", prioridad);

    // Bandera para verificar si se encontraron pacientes con la prioridad dada
    int pacientes_encontrados = 0;

    Paciente *paciente_actual = (Paciente *)list_first(pacientes);
    while (paciente_actual != NULL) {
        if (strcmp(paciente_actual->prioridad, prioridad) == 0) {
            printf("Nombre: %s, Edad: %d, Sintoma: %s, Hora de registro: %s",
                   paciente_actual->nombre, paciente_actual->edad,
                   paciente_actual->sintoma, ctime(&paciente_actual->hora_registro));
            pacientes_encontrados = 1;
        }
        paciente_actual = (Paciente *)list_next(pacientes);
    }
    // Verificar si no se encontraron pacientes con la prioridad dada
    if (!pacientes_encontrados) {
        printf("No hay pacientes con prioridad '%s'.\n", prioridad);
    }
}

void atender_siguiente_paciente(List *pacientes) {
    // Verificar si hay pacientes en espera
    if (list_first(pacientes) == NULL) {
        printf("No hay pacientes en espera.\n");
        return;
    }
    Paciente *paciente_atendido = (Paciente *)list_popFront(pacientes);
    
    // Mostrar los datos del paciente atendido
    printf("Paciente atendido:\n");
    printf("Nombre: %s\nEdad: %d\nSíntoma: %s\nPrioridad: %s\n",
           paciente_atendido->nombre, paciente_atendido->edad,
           paciente_atendido->sintoma, paciente_atendido->prioridad);

    // Liberar la memoria del paciente atendido
    free(paciente_atendido);
}
// Función para registrar un nuevo paciente
void registrar_paciente(List *pacientes) {
    // Crear un nuevo paciente
    Paciente *nuevo_paciente = (Paciente *)malloc(sizeof(Paciente));

    // Solicitar al usuario que ingrese los datos del paciente
    printf("Ingrese el nombre del paciente: ");
    scanf("%s", nuevo_paciente->nombre);
    printf("Ingrese la edad del paciente: ");
    scanf("%d", &nuevo_paciente->edad);
    printf("Ingrese el síntoma del paciente: ");
    scanf("%s", nuevo_paciente->sintoma);

    // Obtener la hora de registro actual
    nuevo_paciente->hora_registro = time(NULL);

    // Establecer el nivel de prioridad inicial como "Bajo"
    strcpy(nuevo_paciente->prioridad, "Bajo");

    // Agregar el paciente a la lista de espera de pacientes
    list_pushBack(pacientes, nuevo_paciente);

    printf("Paciente registrado con éxito.\n");
}


// Función para asignar prioridad a un paciente
void asignar_prioridad(List *pacientes) {
    char nombre[100];
    char nueva_prioridad[10];
    Paciente *paciente_actual = (Paciente *)list_first(pacientes);
    // Solicitar al usuario que ingrese el nombre del paciente
    printf("Ingrese el nombre del paciente: ");
    scanf("%s", nombre);
    
    // Buscar al paciente por nombre y asignar prioridad si se encuentra
    list_first(pacientes);
    while (paciente_actual != NULL) {
      
        if (strcmp(paciente_actual->nombre, nombre) == 0) {
            // Solicitar al usuario que ingrese el nuevo nivel de prioridad
            printf("Ingrese el nuevo nivel de prioridad ('Alto', 'Medio', 'Bajo'): ");
            scanf("%s", nueva_prioridad);
            // Actualizar el nivel de prioridad del paciente
            strcpy(paciente_actual->prioridad, nueva_prioridad);
            printf("Prioridad actualizada con éxito.\n");
          
            return;
        }
        paciente_actual = (Paciente *)list_next(pacientes);
    }
    printf("El paciente no se encuentra en la lista de espera.\n");
}

int main() {
  char opcion;
  List *pacientes = list_create(); 
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior

    switch (opcion) {
    case '1':
      registrar_paciente(pacientes);
      break;
    case '2':
      asignar_prioridad(pacientes);
      break;
    case '3':
      mostrar_lista_pacientes(pacientes);
      break;
    case '4':
      atender_siguiente_paciente(pacientes);
      break;
    case '5':
    {
        char prioridad[10];
        printf("Ingrese el nivel de prioridad ('Alto', 'Medio', 'Bajo'): ");
        scanf("%s", prioridad);
        mostrar_pacientes_por_prioridad(pacientes, prioridad);
    }
    break;
    case '6':
      puts("Saliendo del sistema de gestión hospitalaria...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '6');

  // Liberar recursos, si es necesario
  list_clean(pacientes);

  return 0;
}