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


// Para que no se mueva la línea del panel
void fijar_panel(GtkPaned *panel, GParamSpec *pspec, gpointer user_data) {
    const int pos_fijada = 895;    // Posición donde se fija la división
    int current_pos = gtk_paned_get_position(panel);
    if (current_pos != pos_fijada) {
        gtk_paned_set_position(panel, pos_fijada);
    }
}


int main(int argc, char *argv[]) {
    GtkBuilder *builder;        // Utilizado para obtener los objetos de glade
    GtkWidget *ventana;         // La ventana
    GtkWidget *panel;           // El panel que divide el área de dibujo y el área de interacción

    gtk_init(&argc, &argv);
    // Cargar la interfaz de Glade
    builder = gtk_builder_new_from_file("interfaz.glade");

    // La ventana
    ventana = GTK_WIDGET(gtk_builder_get_object(builder, "ventana"));
    g_signal_connect(ventana, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    // El panel divisor
    panel = GTK_WIDGET(gtk_builder_get_object(builder, "panel"));
    g_signal_connect(panel, "notify::position", G_CALLBACK(fijar_panel), NULL);

    // Mostrar ventana
    gtk_widget_show_all(ventana);
    // Que la ventana utilize toda la pantalla
    gtk_window_maximize(GTK_WINDOW(ventana));

    // Correr GTK
    gtk_main();

    g_object_unref(builder);

    return 0;
}