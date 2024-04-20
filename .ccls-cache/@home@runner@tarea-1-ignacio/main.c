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
    printf("Lista de pacientes en espera:\n");

    // Contador para almacenar el número de pacientes
    int num_pacientes = 0;

    // Iterar sobre la lista de pacientes y contar el número de elementos
    list_first(pacientes);
    while (list_next(pacientes) != NULL) {
        num_pacientes++;
    }

    // Verificar si la lista está vacía
    if (num_pacientes == 0) {
        printf("No hay pacientes en espera.\n");
        return;
    }

    // Crear un array dinámico para almacenar los punteros a los pacientes
    Paciente **array_pacientes = (Paciente **)malloc(num_pacientes * sizeof(Paciente *));
    if (array_pacientes == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para ordenar los pacientes.\n");
        return;
    }

    // Volver al inicio de la lista
    list_first(pacientes);

    // Llenar el array con los punteros a los pacientes
    int i = 0;
    while (list_next(pacientes) != NULL) {
        Paciente *paciente_actual = (Paciente *)list_next(pacientes);
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

// Función para registrar un nuevo paciente
void registrar_paciente(List *pacientes) {
    // Crear un nuevo paciente
    Paciente nuevo_paciente;

    // Solicitar al usuario que ingrese los datos del paciente
    printf("Ingrese el nombre del paciente: ");
    scanf("%s", nuevo_paciente.nombre);
    printf("Ingrese la edad del paciente: ");
    scanf("%d", &nuevo_paciente.edad);
    printf("Ingrese el síntoma del paciente: ");
    scanf("%s", nuevo_paciente.sintoma);

    // Obtener la hora de registro actual
    nuevo_paciente.hora_registro = time(NULL);

    // Establecer el nivel de prioridad inicial como "Bajo"
    strcpy(nuevo_paciente.prioridad, "Bajo");

    // Agregar el paciente a la lista de espera de pacientes
    list_pushBack(pacientes, &nuevo_paciente);

    printf("Paciente registrado con éxito.\n");
}

// Función para asignar prioridad a un paciente
void asignar_prioridad(List *pacientes) {
    char nombre[100];
    char nueva_prioridad[10];

    // Solicitar al usuario que ingrese el nombre del paciente
    printf("Ingrese el nombre del paciente: ");
    scanf("%s", nombre);

    // Solicitar al usuario que ingrese el nuevo nivel de prioridad
    printf("Ingrese el nuevo nivel de prioridad ('Alto', 'Medio', 'Bajo'): ");
    scanf("%s", nueva_prioridad);

    // Recorrer la lista de pacientes para buscar al paciente por nombre
    list_first(pacientes);
    while (list_next(pacientes) != NULL) {
        Paciente *paciente_actual = (Paciente *)list_next(pacientes);
        if (strcmp(paciente_actual->nombre, nombre) == 0) {
            // Actualizar el nivel de prioridad del paciente
            strcpy(paciente_actual->prioridad, nueva_prioridad);
            printf("Prioridad actualizada con éxito.\n");
            return;
        }
    }

    // Si el paciente no se encuentra en la lista
    printf("El paciente no se encuentra en la lista de espera.\n");
}

int main() {
  char opcion;
  List *pacientes = list_create(); // puedes usar una lista para gestionar los pacientes

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
      // Lógica para atender al siguiente paciente
      break;
    case '5':
      // Lógica para mostrar pacientes por prioridad
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
