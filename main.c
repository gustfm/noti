#include <gtk/gtk.h>

#include "src/notes.c"

GtkWidget *editor_area;
GtkWidget *grid;
GtkWidget *notes_list;

const int CONTROL_KEY_CODE = 65507;
const int S_KEY_CODE = 115;

bool is_ctrl_pressed = FALSE;
char *selected_filename = NULL;

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

void activate_cb(GtkListBox *listbox, GtkListBoxRow *row, gpointer user_data)
{
    if (row != NULL)
    {
        char *text = (char *)g_object_get_data(G_OBJECT(row), "text");
        selected_filename = (char *)g_object_get_data(G_OBJECT(row), "filename");
        GtkTextBuffer *tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor_area));
        gtk_text_buffer_set_text(tb, text, -1);
    }
}

void apply_css()
{
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(css_provider, "./style/style.css");
    gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                               GTK_STYLE_PROVIDER(css_provider),
                                               GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

GtkWidget *create_window(GApplication *app)
{
    GtkWidget *window = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Noti");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);
    gtk_window_present(GTK_WINDOW(window));
    return window;
}

GtkWidget *create_grid(GtkWidget *window)
{
    GtkWidget *grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 0);
    return grid;
}

void add_item(GtkWidget *listbox, char *filename)
{
    GtkWidget *row = gtk_list_box_row_new();
    gtk_list_box_row_set_child(GTK_LIST_BOX_ROW(row), gtk_label_new(filename));
    char *content = get_note_content(filename);
    g_object_set_data(G_OBJECT(row), "text", content);
    g_object_set_data(G_OBJECT(row), "filename", filename);
    gtk_list_box_insert(GTK_LIST_BOX(listbox), row, -1);
}

GtkWidget *create_notes_list(GtkWidget *window)
{
    GtkWidget *listbox = gtk_list_box_new();
    GList *notes = get_notes();

    for (GList *iter = notes; iter->next != NULL; iter = iter->next)
    {
        char *filename = ((Note *)(iter->data))->filename;
        add_item(listbox, filename);
    }

    g_signal_connect(listbox, "row-activated", G_CALLBACK(activate_cb), NULL);

    return listbox;
}

GtkWidget *create_editor_area(GtkWidget *window)
{
    GtkWidget *editor_area = gtk_text_view_new();
    gtk_widget_set_hexpand(editor_area, TRUE);
    gtk_widget_set_vexpand(editor_area, TRUE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(editor_area), GTK_WRAP_WORD_CHAR);
    gtk_window_present(GTK_WINDOW(window));
    return editor_area;
}

void load_custom_icons()
{
    GtkIconTheme *icon_theme = gtk_icon_theme_get_for_display(gdk_display_get_default());
    gtk_icon_theme_add_search_path(icon_theme, "/home/gus/Workspace/Personal/noti/resource/hicolor/apps/60x60");
}

static gboolean
event_key_pressed_cb(GtkEventControllerKey *controller,
                     guint keyval,
                     guint keycode,
                     GdkModifierType state)
{
    if (keyval == CONTROL_KEY_CODE)
    {
        is_ctrl_pressed = TRUE;
    }

    if (is_ctrl_pressed && keyval == S_KEY_CODE)
    {
        g_print("Saving");

        GtkTextIter start, end;
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor_area));
        gchar *text;

        gtk_text_buffer_get_bounds(buffer, &start, &end);
        text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

        set_note_content(selected_filename, text);

        GtkListBoxRow *selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(notes_list));
        g_object_set_data(G_OBJECT(selected_row), "text", text);
    }
}

static gboolean
event_key_released_cb(GtkEventControllerKey *controller,
                      guint keyval,
                      guint keycode,
                      GdkModifierType state)
{
    if (keyval == CONTROL_KEY_CODE)
    {
        is_ctrl_pressed = FALSE;
    }
}

static void
app_activate(GApplication *app, gpointer user_data)
{
    GtkWidget *window = create_window(app);
    load_custom_icons();
    apply_css();

    grid = create_grid(window);
    editor_area = create_editor_area(window);
    notes_list = create_notes_list(window);

    GtkEventController *event_controller = gtk_event_controller_key_new();
    g_signal_connect_object(event_controller, "key-pressed",
                            G_CALLBACK(event_key_pressed_cb),
                            NULL, G_CONNECT_SWAPPED);
    g_signal_connect_object(event_controller, "key-released",
                            G_CALLBACK(event_key_released_cb),
                            NULL, G_CONNECT_SWAPPED);
    gtk_widget_add_controller(GTK_WIDGET(window), event_controller);

    gtk_grid_attach(GTK_GRID(grid), notes_list, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), editor_area, 1, 0, 1, 1);
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
