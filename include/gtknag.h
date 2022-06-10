// gtknag
// Copyright (c) 2022 Jovan Lanik

#include <gtk/gtk.h>
#include <gtk-layer-shell.h>

struct GtkNag {
	GtkApplication *app;
	GtkWidget *window;

	char *message;
	GtkLayerShellLayer layer;
};

extern struct GtkNag *gtknag;

struct GtkNag *create_gtknag(void);
void gtknag_activate(struct GtkNag *gtknag);
void gtknag_destroy(struct GtkNag *gtknag);

