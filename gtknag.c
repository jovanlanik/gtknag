// gtknag
// Copyright (c) 2022 Jovan Lanik

#include <gtk/gtk.h>
#include <gtk-layer-shell.h>

#include "gtknag.h"

#if GLIB_CHECK_VERSION(2, 74, 0)
	#define GTKNAG_FLAGS G_APPLICATION_DEFAULT_FLAGS
#else
	#define GTKNAG_FLAGS G_APPLICATION_FLAGS_NONE
#endif

struct GtkNag* create_gtknag(void) {
	struct GtkNag *nag = calloc(1, sizeof(struct GtkNag));
	nag->app = gtk_application_new(NULL, GTKNAG_FLAGS);
	return nag;
}

static void gtknag_dismiss(void *data) {
	g_application_quit(G_APPLICATION(gtknag->app));
}

void gtknag_activate(struct GtkNag *gtknag) {
	gtknag->window = gtk_application_window_new(gtknag->app);
	gtk_window_set_title(GTK_WINDOW(gtknag->window), "Message");
	gtk_window_set_decorated(GTK_WINDOW(gtknag->window), FALSE);

	gtk_layer_init_for_window(GTK_WINDOW(gtknag->window));
	gtk_layer_set_layer(GTK_WINDOW(gtknag->window), gtknag->layer);
	gtk_layer_set_anchor(GTK_WINDOW(gtknag->window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
	gtk_layer_set_anchor(GTK_WINDOW(gtknag->window), GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);
	// TODO: edge option
	gtk_layer_set_anchor(GTK_WINDOW(gtknag->window), GTK_LAYER_SHELL_EDGE_TOP, TRUE);
	gtk_layer_set_anchor(GTK_WINDOW(gtknag->window), GTK_LAYER_SHELL_EDGE_BOTTOM, FALSE);
	gtk_layer_auto_exclusive_zone_enable(GTK_WINDOW(gtknag->window));
	gtk_layer_set_margin(GTK_WINDOW(gtknag->window), GTK_LAYER_SHELL_EDGE_BOTTOM, 8);

	GtkWidget *window_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	g_object_set(window_box, "margin", 5, NULL);
	gtk_widget_set_name(window_box, "window");
	gtk_container_add(GTK_CONTAINER(gtknag->window), window_box);

	GtkWidget *message = gtk_label_new(gtknag->message);
	gtk_container_add(GTK_CONTAINER(window_box), message);

	GtkWidget *button = gtk_button_new_from_icon_name("window-close", GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_set_tooltip_text(button, "Close message");
	g_signal_connect(button, "clicked", G_CALLBACK(gtknag_dismiss), NULL);
	gtk_box_pack_end(GTK_BOX(window_box), button, FALSE, FALSE, 0);

	gtk_widget_show_all(gtknag->window);
}

void gtknag_destroy(struct GtkNag *nag) {
	g_object_unref(nag->app);
	free(nag);
}

