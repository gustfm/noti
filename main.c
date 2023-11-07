#include <gtk/gtk.h>

GtkWidget *textView;

static void
setup_cb(GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data)
{
    GtkWidget *lb = gtk_label_new(NULL);
    gtk_list_item_set_child(listitem, lb);
}

static void
bind_cb(GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data)
{
    GtkWidget *lb = gtk_list_item_get_child(listitem);
    GtkStringObject *strobj = gtk_list_item_get_item(listitem);
    gtk_label_set_text(GTK_LABEL(lb), gtk_string_object_get_string(strobj));
}

static void
activate_cb(GtkSignalListItemFactory *self, GtkListItem *listitem, gpointer user_data) {
    GtkTextBuffer *tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
    gtk_text_buffer_set_text(tb, "AAAAAAAAAAAAAAAAAAAA", -1);
}

static void
app_activate(GApplication *app, gpointer user_data)
{
    GtkWidget *window;

    window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Noti");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);
    gtk_window_present(GTK_WINDOW(window));

    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "./style.css");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                              GTK_STYLE_PROVIDER(css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GtkWidget *grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);

    gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 2);

    char *notes[] = {
        "one", "two", "three", "four", NULL};

    GtkStringList *notesStringList = gtk_string_list_new((const char *const *)notes);
    GtkNoSelection *notesNoSelection = gtk_no_selection_new(G_LIST_MODEL(notesStringList));

    GtkListItemFactory *factory = gtk_signal_list_item_factory_new();
    g_signal_connect(factory, "setup", G_CALLBACK(setup_cb), NULL);
    g_signal_connect(factory, "bind", G_CALLBACK(bind_cb), NULL);

    GtkWidget *list = gtk_list_view_new(GTK_SELECTION_MODEL(notesNoSelection), factory);
    g_signal_connect(list, "activate", G_CALLBACK(activate_cb), NULL);
    gtk_list_view_set_single_click_activate(GTK_LIST_VIEW(list), TRUE);
    gtk_window_present(GTK_WINDOW(window));

    textView = gtk_text_view_new();
    gtk_widget_set_hexpand(textView, TRUE);
    gtk_widget_set_vexpand(textView, TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD_CHAR);
    gtk_window_present(GTK_WINDOW(window));

    gtk_grid_attach(GTK_GRID(grid), list, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), textView, 1, 0, 1, 1);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int stat;

    app = gtk_application_new("com.github.gustfm.noti", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    stat = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return stat;
}
