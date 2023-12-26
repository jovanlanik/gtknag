// gtknag
// Copyright (c) 2022 Jovan Lanik

// GTK-based swaynag replacement

#include <gtk/gtk.h>
#include "gtknag.h"
#include "util.h"

struct GtkNag *gtknag = NULL;

static char *style_path = NULL;

static GOptionEntry gtknag_entries[] = {
	{ "style", 's', 0, G_OPTION_ARG_FILENAME, &style_path, "CSS style to use", NULL },
	{ NULL },
};

static gchar *gtk_theme = NULL;
static char *message = NULL;
static char *layer = NULL;

static GOptionEntry swaynag_entries[] = {
	{ "gtk-theme", 'g', 0, G_OPTION_ARG_STRING, &gtk_theme, "Set GTK theme", NULL },
	// TODO: button
	// TODO: button-no-terminal
	// TODO: button-dismiss
	// TODO: button-dismiss-no-terminal
	// TODO: config
	// TODO: debug (ignore)
	// TODO: edge
	{ "layer", 'l', G_OPTION_FLAG_NONE, G_OPTION_ARG_STRING, &layer, "Set the layer to use.", NULL },
	// TODO: font
	// TODO: detailed-message
	// TODO: detailed-button
	{ "message", 'm', 0, G_OPTION_ARG_STRING, &message, "Set the message text", NULL },
	// TODO: output
	// TODO: dismiss-button
	// TODO: type
	
	// TODO: appearance options
	{ NULL },
};

static void activate(GtkApplication *app, gpointer data) {
	gtknag_activate(gtknag);
}

static void attach_custom_style(const char* path) {
	GtkCssProvider *provider = gtk_css_provider_new();
	GError *err = NULL;

	gtk_css_provider_load_from_path(provider, path, &err);
	if(err != NULL) {
		g_warning("Style loading failed: %s", err->message);
		g_error_free(err);
	} else {
		gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
			GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	}
	g_object_unref(provider);
}

int main(int argc, char **argv) {
	GError *error = NULL;
	GOptionContext *option_context = g_option_context_new("- GTK-based swaynag replacement");
	g_option_context_add_main_entries(option_context, gtknag_entries, NULL);
	g_option_context_add_main_entries(option_context, swaynag_entries, NULL);
	g_option_context_add_group(option_context, gtk_get_option_group(TRUE));
	if(!g_option_context_parse(option_context, &argc, &argv, &error))
		report_error_and_exit("Option parsing failed: %s\n", error->message);

	if(message == NULL) report_error_and_exit("No message passed. Please provide --message/-m");

	gtknag = create_gtknag();

	gtknag->layer = GTK_LAYER_SHELL_LAYER_TOP;
	if(layer) {
		if(g_strcmp0(layer, "overlay") == 0) gtknag->layer = GTK_LAYER_SHELL_LAYER_OVERLAY;
		else if(g_strcmp0(layer, "top") == 0) gtknag->layer = GTK_LAYER_SHELL_LAYER_TOP;
		else if(g_strcmp0(layer, "bottom") == 0) gtknag->layer = GTK_LAYER_SHELL_LAYER_BOTTOM;
		else if(g_strcmp0(layer, "background") == 0) gtknag->layer = GTK_LAYER_SHELL_LAYER_BACKGROUND;
		else g_warning("Invalid layer: %s", layer);
	}

	if(gtk_theme) {
		GtkSettings *settings = gtk_settings_get_default();
		g_object_set(settings, "gtk-theme-name", gtk_theme, NULL);
	}

	if(style_path != NULL) attach_custom_style(style_path);
	gtknag->message = message;

	g_signal_connect(gtknag->app, "activate", G_CALLBACK(activate), NULL);
	int status = g_application_run(G_APPLICATION(gtknag->app), argc, argv);

	gtknag_destroy(gtknag);
	return status;
}

