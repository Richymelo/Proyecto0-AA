/*
                Proyecto 0: Generador de Cuadros mágicos
                Hecha por: Carmen Hidalgo Paz, Jorge Guevara Chavarría y Ricardo Castro Jiménez
                Fecha: Jueves 13 de marzo del 2025

                Esta sección contiene el main, donde se indica lo que tiene que hacer
                cada objeto mostrado en la interfaz. Además se tienen las funciones para
                crear los cuadros, agregar los números y los movimientos en los que
                estos aparecen.
*/

#include <gtk/gtk.h>
#include <cairo.h>
#include <math.h>     // Para calcular los rayos
#include <stdlib.h>  // Para utilizar rand()
#include <time.h>    // Para utilizar rand()
#include "movimientos.h"

int matriz[max][max];   // Crear la matriz
int currentNumber = 1;  // Para ir revisando por cuál número se va
int n = 3;              // Tamaño de la matriz
int pos = 0;            // Posición en la que se encuentran los números en la matriz

// Rellenar cuadro con los valores
void actualizarCuadro(GtkWidget *grid, int n) {
    GList *celdas = gtk_container_get_children(GTK_CONTAINER(grid));
    int i = 0;

    for (GList *actual = celdas; actual != NULL; actual = actual->next) {
        GtkWidget *celda = GTK_WIDGET(actual->data);
        GtkWidget *etiqueta = gtk_bin_get_child(GTK_BIN(celda));
        
        // Convertir número a string para imprimirlo
        char buffer[5];
        if(matriz[i / n][i % n] != 0){
            snprintf(buffer, sizeof(buffer), "%d", matriz[i / n][i % n]);
        }else{
            snprintf(buffer, sizeof(buffer), " ");
        }
        gtk_label_set_text(GTK_LABEL(etiqueta), buffer);

        i++;
    }
    g_list_free(celdas);
}
// Crear el cuadrado vacío
void crear_cuadrado(GtkWidget *cuadrado, int size) {
    // Limpiar cuadrados antiguos
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
            // Crear celdas con un etiqueta
            GtkWidget *etiqueta = gtk_label_new(buffer);
            gtk_container_add(GTK_CONTAINER(marco), etiqueta);

            // Pegar los marcos a la tabla
            gtk_grid_attach(GTK_GRID(cuadrado), marco, col, fila, 1, 1);
        }
    }
    // Limpiar la matriz de los números que tiene
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = 0;
        }
    }
    actualizarCuadro(cuadrado, n);
    gtk_widget_show_all(cuadrado);
}
// Definición de tipo de función para métodos de llenado
typedef int (*MetodoLlenado)(int[max][max], int, int, int);
MetodoLlenado metodoSeleccionado = metodoSiames;  // Método por defecto
// Selección de movimientos de los números
void movimiento_elegido(GtkComboBoxText *combo, gpointer user_data) {
    const char *selected_text = gtk_combo_box_text_get_active_text(combo);
    if (strcmp(selected_text, "Método Siamés") == 0) {
        metodoSeleccionado = metodoSiames;
    } else if (strcmp(selected_text, "En L") == 0) { 
        metodoSeleccionado = metodoEnL;
    } else if (strcmp(selected_text, "Método De la Loubère") == 0) { 
        metodoSeleccionado = metodoLouber;
    } else if (strcmp(selected_text, "Alterno de Diagonales") == 0){
        metodoSeleccionado = metodoAlterno;
    }
}
// Función para llenar la matriz completa con el método seleccionado
void llenarCuadroMagico(int matriz[max][max], int n, MetodoLlenado metodo) {
    srand(time(NULL)); // Inicializa la semilla para números aleatorios
    int fila;
    int columna;
    if (currentNumber == 1){
        // Posiciones iniciales
        if (metodoSeleccionado == metodoSiames || metodoSeleccionado == metodoAlterno) {
            fila = 0;
            columna = n/2;
        } else if (metodoSeleccionado == metodoEnL) {
            fila = rand() % n;
            columna = rand() % n;
        } else if (metodoSeleccionado == metodoLouber) {
            fila = n-1;
            columna = n/2;
        }
    }else{
        // Calcular el resto de posiciones
        fila = pos / max;
        columna = pos % max;
    }
    for (int num = currentNumber; num <= n * n; num++) {
        matriz[fila][columna] = num;
        int posicion = metodoSeleccionado(matriz, n, fila, columna);
        fila = posicion / max;
        columna = posicion % max;
    }
    currentNumber = n * n + 1;
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
    currentNumber = 1;
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
// Llenar todo el cuadro
void llenar_todo(GtkButton *button, gpointer user_data) {
    // Parar si el cuadro está lleno
    if (currentNumber > n * n){
        return;
    }
    // Agregar todos los números
    llenarCuadroMagico(matriz, n, metodoSeleccionado);
    actualizarCuadro(GTK_WIDGET(user_data), n);
}
// Llenar los números de uno en uno
void llenar_uno(GtkButton *button, gpointer user_data) {
    static int fila = 0, columna = 0;
    // Posiciones iniciales
    if (currentNumber == 1){
        if (metodoSeleccionado == metodoSiames || metodoSeleccionado == metodoAlterno) {
            fila = 0;
            columna = n/2;
        } else if (metodoSeleccionado == metodoEnL) {
            fila = rand() % n;
            columna = rand() % n;
        } else if (metodoSeleccionado == metodoLouber) {
            fila = n-1; 
            columna = n/2;
        }
    }
    // Parar si la tabla está llena
    if (currentNumber > n * n){
        return;
    }
    matriz[fila][columna] = currentNumber++;
    actualizarCuadro(GTK_WIDGET(user_data), n);

    // Ir a siguiente posición
    int posicion = metodoSeleccionado(matriz, n, fila, columna);
    fila = posicion / max;
    columna = posicion % max;
    pos = posicion;
}
// Quitar los números de un cuadro
void limpiar_cuadrado(GtkButton *button, gpointer user_data) {
    // Resetear variable
    currentNumber = 1;
    // Limipiar los números del cuadro
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = 0;
        }
    }
    actualizarCuadro(GTK_WIDGET(user_data), n);
}
// Actualizar etiquetas de suma
void etiquetas_sumas(int matriz[max][max], int n, GtkBuilder *builder) {
    char buffer[256];  
    int sumaFila = 0, sumaColumna = 0, sumaDiagonales = 0;

    GtkWidget *labelFila = GTK_WIDGET(gtk_builder_get_object(builder, "suma_filas"));
    GtkWidget *labelColumna = GTK_WIDGET(gtk_builder_get_object(builder, "suma_columnas"));
    GtkWidget *labelDiagonal = GTK_WIDGET(gtk_builder_get_object(builder, "suma_diagonales"));

    //Suma filas
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sumaFila += matriz[i][j];
        }
    }
    // Suma columnas
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            sumaColumna += matriz[i][j];
        }
    }
    // Suma diagonales
    for (int i = 0; i < n; i++) {
        sumaDiagonales += matriz[i][i];
        sumaDiagonales += matriz[i][n - i - 1];
    }
    // Cambiar etiquetas
    snprintf(buffer, sizeof(buffer), "Suma de sus filas: %d", sumaFila/n);
    gtk_label_set_text(GTK_LABEL(labelFila), buffer);

    snprintf(buffer, sizeof(buffer), "Suma de sus columnas: %d", sumaColumna/n);
    gtk_label_set_text(GTK_LABEL(labelColumna), buffer);

    snprintf(buffer, sizeof(buffer), "Suma de sus diagonales mayores: %d", sumaDiagonales/2);
    gtk_label_set_text(GTK_LABEL(labelDiagonal), buffer);
}
// Actualizar sumas con botón
void actualizar_sumas(GtkButton *button, gpointer user_data) {
    GtkBuilder *builder = user_data;
    etiquetas_sumas(matriz, n, builder);
}

int main(int argc, char *argv[]) {
    GtkBuilder *builder;        // Utilizado para obtener los objetos de glade
    GtkWidget *ventana;         // La ventana
    GtkWidget *panel;           // El panel que divide el área de dibujo y el área de interacción
    GtkWidget *size;            // Para escoger el tamaño del cuadro
    GtkWidget *cuadrado;        // Donde se va a crear el cuadrado
    GtkWidget *menu;            // El usuario selecciona el tipo de movimiento
    GtkWidget *un_valor;        // Botón que llena un valor a la vez
    GtkWidget *todo_cuadrado;   // Botón que llena todo el cuadrado de un solo
    GtkWidget *boton_limpiar;   // Botón que limpia un cuadro cuando se llena
    GtkWidget *labelFila;       // Para la suma de las filas
    GtkWidget *labelColumna;    // Para la suma de las columnas
    GtkWidget *labelDiagonal;   // Para la suma de las diagonales
    GtkWidget *boton_sumas;     // Para actualizar las sumas
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
    // Selección de movimientos
    menu = GTK_WIDGET(gtk_builder_get_object(builder, "menu_movimientos"));
    g_signal_connect(menu, "changed", G_CALLBACK(movimiento_elegido), NULL);
    // Botones de relleno del cuadrado
    un_valor = GTK_WIDGET(gtk_builder_get_object(builder, "solo_numero"));
    g_signal_connect(un_valor, "clicked", G_CALLBACK(llenar_uno), cuadrado);
    todo_cuadrado = GTK_WIDGET(gtk_builder_get_object(builder, "todo_cuadrado"));
    g_signal_connect(todo_cuadrado, "clicked", G_CALLBACK(llenar_todo), cuadrado);
    // Botón que limpia el cuadrado
    boton_limpiar = GTK_WIDGET(gtk_builder_get_object(builder, "limpiar"));
    g_signal_connect(boton_limpiar, "clicked", G_CALLBACK(limpiar_cuadrado), cuadrado);
    // Etiquetas de las sumas
    boton_sumas = GTK_WIDGET(gtk_builder_get_object(builder, "sumas"));
    g_signal_connect(boton_sumas, "clicked", G_CALLBACK(actualizar_sumas), builder);
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