#include "tdas/list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Función para limpiar la pantalla
void limpiarPantalla() { system("clear"); }

void presioneTeclaParaContinuar() {
  puts("Presione una tecla para continuar...");
  getchar(); // Consume el '\n' del buffer de entrada
  getchar(); // Espera a que el usuario presione una tecla
}

// Estructura para representar a un paciente
typedef struct {
    char nombre[50];
    int edad;
    char sintoma[100];
    char prioridad[10];
    time_t hora_llegada;
} Paciente;
//..
// Función de comparación para ordenar la lista de pacientes
int comparar_pacientes(const void *a, const void *b) {
    Paciente *paciente_a = (Paciente *)a;
    Paciente *paciente_b = (Paciente *)b;

    // Compara por prioridad
    int prioridad_comp = strcmp(paciente_a->prioridad, paciente_b->prioridad);
    if (prioridad_comp != 0) {
        return prioridad_comp;
    }

    // Si la prioridad es la misma, compara por hora de llegada (menor primero)
    return difftime(paciente_a->hora_llegada, paciente_b->hora_llegada);
}

// Función para mostrar la lista de espera ordenada
void mostrar_lista_pacientes(List *pacientes) {
    limpiarPantalla();
    printf("Lista de pacientes en espera:\n");
    // Obtener el número de pacientes en la lista
    size_t num_pacientes = list_size(pacientes);
    if (num_pacientes == 0) {
        printf("No hay pacientes en espera.\n");
        return;
    }

    // Crear un arreglo para almacenar los pacientes y copiarlos desde la lista
    Paciente pacientes_array[num_pacientes];
    size_t i = 0;
    ListNode *current = list_begin(pacientes);
    while (current != NULL) {
        pacientes_array[i++] = *(Paciente *)list_data(current);
        current = list_next(current);
    }

    // Ordenar el arreglo de pacientes utilizando la función de comparación definida
    qsort(pacientes_array, num_pacientes, sizeof(Paciente), comparar_pacientes);

    // Mostrar la lista de pacientes ordenada
    printf("%-20s %-10s %-10s %-15s %s\n", "Nombre", "Edad", "Prioridad", "Síntoma", "Hora Registro");
    for (i = 0; i < num_pacientes; ++i) {
        printf("%-20s %-10d %-10s %-15s %s", pacientes_array[i].nombre, pacientes_array[i].edad, 
               pacientes_array[i].prioridad, pacientes_array[i].sintoma, ctime(&pacientes_array[i].hora_llegada));
    }
}

// Función para atender al siguiente paciente
void atender_siguiente_paciente(List *pacientes) {
    limpiarPantalla();
    printf("Atendiendo al siguiente paciente:\n");

    // Verificar si hay pacientes en espera
    if (list_size(pacientes) == 0) {
        printf("No hay pacientes en espera.\n");
        return;
    }

    // Obtener el siguiente paciente según el orden de prioridad
    ListNode *current = list_begin(pacientes);
    Paciente *siguiente_paciente = (Paciente *)list_data(current);
    printf("Nombre: %s\n", siguiente_paciente->nombre);
    printf("Edad: %d\n", siguiente_paciente->edad);
    printf("Síntoma: %s\n", siguiente_paciente->sintoma);
    printf("Prioridad: %s\n", siguiente_paciente->prioridad);
    printf("Hora de registro: %s", ctime(&siguiente_paciente->hora_llegada));

    // Eliminar al paciente de la lista de espera
    list_erase(pacientes, current);

    printf("Paciente atendido y eliminado de la lista de espera.\n");
}

// Función principal
int main() {
    char opcion;
    List *pacientes = list_create(); // Lista para gestionar los pacientes

    do {
        // Mostrar menú principal
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
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion); // Consumir el '\n' pendiente

        // Procesar la opción seleccionada
        switch (opcion) {
            case '1':
                // Implementar registro de paciente
                break;
            case '2':
                // Implementar asignación de prioridad
                break;
            case '3':
                mostrar_lista_pacientes(pacientes);
                presioneTeclaParaContinuar();
                break;
            case '4':
                atender_siguiente_paciente(pacientes);
                presioneTeclaParaContinuar();
                break;
            case '5':
                // Implementar mostrar pacientes por prioridad
                break;
            case '6':
                puts("Saliendo del sistema de gestión hospitalaria...");
                break;
            default:
                puts("Opción no válida. Por favor, intente de nuevo.");
                presioneTeclaParaContinuar();
        }
    } while (opcion != '6');

    // Liberar recursos
    list_destroy(pacientes);

    return 0;
}