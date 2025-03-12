/*
                Proyecto 0: Generador de Cuadros mágicos
                Hecha por: Carmen Hidalgo Paz, Jorge Guevara Chavarría y Ricardo Castro Jiménez
                Fecha: Jueves 13 de marzo del 2025

                Esta sección contiene el main, donde se indica lo que tiene que hacer
                cada objeto mostrado en la interfaz.
*/

#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>     // Para calcular los rayos
#include <stdlib.h>  // Para utilizar rand()
#include <time.h>    // Para utilizar rand()

#define max 21  // Max size for magic square

int matriz[max][max];  // Store grid numbers
int currentNumber = 1; // Track progress for filling one-by-one
int n = 3;  // Default grid size, updated dynamically

// Crear el cuadrado
void crear_cuadrado(GtkWidget *cuadrado, int size) {
    // Limpiar antiguos cuadrados
    GList *celdas = gtk_container_get_children(GTK_CONTAINER(cuadrado));
    for (GList *actual = celdas; actual != NULL; actual = actual->next) {
        gtk_widget_destroy(GTK_WIDGET(actual->data));
    }
    g_list_free(celdas);

    // Asegurar que el cuadrado esté centrado cuando se expande
    gtk_widget_set_halign(cuadrado, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(cuadrado, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(cuadrado, TRUE);
    gtk_widget_set_vexpand(cuadrado, TRUE);

    // Un cuadrado de n*n
    n = size;
    currentNumber = 1;
    for (int fila = 0; fila < size; fila++) {
        for (int col = 0; col < size; col++) {
            char buffer[5];
            snprintf(buffer, sizeof(buffer), " ");

            // Crear marco alrededor de las celdas
            GtkWidget *marco = gtk_frame_new(NULL);
            gtk_widget_set_size_request(marco, 36, 36);
            // Crear celdas con un número
            GtkWidget *etiqueta = gtk_label_new(buffer);
            gtk_container_add(GTK_CONTAINER(marco), etiqueta);

            // Attach frame to grid (instead of attaching the label directly)
            gtk_grid_attach(GTK_GRID(cuadrado), marco, col, fila, 1, 1);
        }
    }

    gtk_widget_show_all(cuadrado);
}
// Definición de tipo de función para métodos de llenado
typedef int (*MetodoLlenado)(int[max][max], int, int, int);

// Método Siamés clásico: mueve en diagonal arriba-derecha y ajusta si está ocupado
int metodoSiames(int matriz[max][max], int n, int fila, int columna) {
    int nuevaFila = (fila - 1 + n) % n;
    int nuevaColumna = (columna + 1) % n;
    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }
    return nuevaFila * max + nuevaColumna;
}
// Función para llenar la matriz con el método seleccionado
void llenarCuadroMagico(int matriz[max][max], int n, MetodoLlenado metodo) {
    srand(time(NULL)); // Inicializa la semilla para números aleatorios
    int fila = rand() % n;
    int columna = rand() % n;
    
    for (int num = 1; num <= n * n; num++) {
        matriz[fila][columna] = num;
        int posicion = metodo(matriz, n, fila, columna);
        fila = posicion / max;
        columna = posicion % max;
    }
}
// Rellenar cuadro con los valores
void actualizarCuadro(GtkWidget *grid, int n) {
    GList *celdas = gtk_container_get_children(GTK_CONTAINER(grid));
    int i = 0;

    for (GList *actual = celdas; actual != NULL; actual = actual->next) {
        GtkWidget *celda = GTK_WIDGET(actual->data);
        GtkWidget *etiqueta = gtk_bin_get_child(GTK_BIN(celda));
        
        // Convertir número a string para imprimirlo
        char buffer[5];
        snprintf(buffer, sizeof(buffer), "%d", matriz[i / n][i % n]);
        gtk_label_set_text(GTK_LABEL(etiqueta), buffer);

        i++;
    }
    g_list_free(celdas);
}
// Para que no se mueva la línea del panel
void fijar_panel(GtkPaned *panel, GParamSpec *pspec, gpointer user_data) {
    const int pos_fijada = 895;    // Posición donde se fija la división
    int current_pos = gtk_paned_get_position(panel);
    if (current_pos != pos_fijada) {
        gtk_paned_set_position(panel, pos_fijada);
    }
}
// Revisar que el número ingresado sea un número impar
void validar_size(GtkSpinButton *ingresar_size, gpointer user_data) {
    int valor = gtk_spin_button_get_value_as_int(ingresar_size);
    // Si no es un número impar, ajustar al valor impar más cercano
    if (valor % 2 == 0){
        valor += 1;
    }
    // Corregir el valor
    gtk_spin_button_set_value(ingresar_size, valor);

    // Crear el cuadrado mágico
    GtkWidget *cuadrado = GTK_WIDGET(user_data);
    crear_cuadrado(cuadrado, valor);
}
void on_fill_all_clicked(GtkButton *button, gpointer user_data) {
    llenarCuadroMagico(matriz, n, metodoSiames); // Fill matriz
    actualizarCuadro(GTK_WIDGET(user_data), n);  // Update UI
}
void on_fill_one_clicked(GtkButton *button, gpointer user_data) {
    static int fila = 0, columna = 0; // Start position
    
    if (currentNumber > n * n) return; // Stop if grid is full

    // Place the next number in matriz
    matriz[fila][columna] = currentNumber++;

    // Update UI
    actualizarCuadro(GTK_WIDGET(user_data), n);

    // Get next position
    int pos = metodoSiames(matriz, n, fila, columna);
    fila = pos / max;
    columna = pos % max;
}


int main(int argc, char *argv[]) {
    GtkBuilder *builder;        // Utilizado para obtener los objetos de glade
    GtkWidget *ventana;         // La ventana
    GtkWidget *panel;           // El panel que divide el área de dibujo y el área de interacción
    GtkWidget *size;            // Para escoger el tamaño del cuadro
    GtkWidget *cuadrado;        // Donde se va a crear el cuadrado
    GtkWidget *un_valor;        // Botón que llena un valor a la vez
    GtkWidget *todo_cuadrado;   // Botón que llena todo el cuadrado de un solo
    GtkWidget *boton_salida;    // Botón para terminar el programa

    gtk_init(&argc, &argv);
    // Cargar la interfaz de Glade
    builder = gtk_builder_new_from_file("interfaz.glade");

    // La ventana
    ventana = GTK_WIDGET(gtk_builder_get_object(builder, "ventana"));
    g_signal_connect(ventana, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // El panel divisor
    panel = GTK_WIDGET(gtk_builder_get_object(builder, "panel"));
    g_signal_connect(panel, "notify::position", G_CALLBACK(fijar_panel), NULL);
    // El botón para escoger el tamaño del cuadro y el cuadrado que va a crearse
    size = GTK_WIDGET(gtk_builder_get_object(builder, "escoger_tamano"));
    cuadrado = GTK_WIDGET(gtk_builder_get_object(builder, "cuadrado"));
    g_signal_connect(GTK_SPIN_BUTTON(size), "value-changed", G_CALLBACK(validar_size), cuadrado);
    // Botones de relleno del cuadrado
    un_valor = GTK_WIDGET(gtk_builder_get_object(builder, "solo_numero"));
    g_signal_connect(un_valor, "clicked", G_CALLBACK(on_fill_one_clicked), cuadrado);
    todo_cuadrado = GTK_WIDGET(gtk_builder_get_object(builder, "todo_cuadrado"));
    g_signal_connect(todo_cuadrado, "clicked", G_CALLBACK(on_fill_all_clicked), cuadrado);
    // El bóton de terminación del programa
    boton_salida = GTK_WIDGET(gtk_builder_get_object(builder, "salir"));
    g_signal_connect(boton_salida, "clicked", G_CALLBACK(gtk_main_quit), NULL);

    // Mostrar ventana
    gtk_widget_show_all(ventana);
    // Que la ventana utilize toda la pantalla
    gtk_window_fullscreen(GTK_WINDOW(ventana));

    // Correr GTK
    gtk_main();

    g_object_unref(builder);

    return 0;
}