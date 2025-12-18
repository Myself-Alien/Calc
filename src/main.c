#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

static char expression[128] = "";

static void load_css(void)
{
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css");

    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
}

static void update_display(GtkEditable *display)
{
    gtk_editable_set_text(display, expression);
}

static void calculate(GtkEditable *display)
{
    double a, b;
    char op;

    if (sscanf(expression, "%lf%c%lf", &a, &op, &b) != 3)
        return;

    double result = 0.0;

    switch (op)
    {
    case '+':
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
    case '*':
        result = a * b;
        break;
    case '/':
        result = (b != 0) ? a / b : 0;
        break;
    default:
        return;
    }

    g_snprintf(expression, sizeof expression, "%.2f", result);
    update_display(display);
}

static void on_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkEditable *display = GTK_EDITABLE(user_data);
    const char *label = gtk_button_get_label(button);

    if (strcmp(label, "C") == 0)
    {
        expression[0] = '\0';
        update_display(display);
        return;
    }

    if (strcmp(label, "=") == 0)
    {
        calculate(display);
        return;
    }

    if (strlen(expression) + strlen(label) < sizeof(expression) - 1)
    {
        strcat(expression, label);
        update_display(display);
    }
}

static gboolean on_key_pressed(
    GtkEventControllerKey *controller,
    guint keyval,
    guint keycode,
    GdkModifierType state,
    gpointer user_data)
{
    (void)controller;
    (void)keycode;
    (void)state;

    GtkEditable *display = GTK_EDITABLE(user_data);
    char buf[2] = {0};

    if (keyval >= GDK_KEY_0 && keyval <= GDK_KEY_9)
        buf[0] = keyval;
    else if (keyval == GDK_KEY_plus ||
             keyval == GDK_KEY_minus ||
             keyval == GDK_KEY_asterisk ||
             keyval == GDK_KEY_slash)
        buf[0] = keyval;
    else if (keyval == GDK_KEY_Return || keyval == GDK_KEY_KP_Enter)
    {
        calculate(display);
        return TRUE;
    }
    else if (keyval == GDK_KEY_BackSpace)
    {
        size_t len = strlen(expression);
        if (len > 0)
            expression[len - 1] = '\0';
        update_display(display);
        return TRUE;
    }
    else
        return FALSE;

    if (strlen(expression) < sizeof(expression) - 1)
    {
        strcat(expression, buf);
        update_display(display);
    }

    return TRUE;
}

static void on_activate(GtkApplication *app, gpointer user_data)
{
    (void)user_data;

    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 420);

    load_css();

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    GtkWidget *display = gtk_entry_new();
    gtk_editable_set_editable(GTK_EDITABLE(display), FALSE);
    gtk_entry_set_alignment(GTK_ENTRY(display), 1.0);
    gtk_widget_add_css_class(display, "display");
    gtk_box_append(GTK_BOX(vbox), display);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_box_append(GTK_BOX(vbox), grid);

    const char *buttons[4][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"0", "C", "+", "="}};

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            GtkWidget *btn = gtk_button_new_with_label(buttons[r][c]);
            gtk_widget_add_css_class(btn, "calc-btn");
            gtk_grid_attach(GTK_GRID(grid), btn, c, r, 1, 1);

            g_signal_connect(btn, "clicked",
                             G_CALLBACK(on_button_clicked), display);
        }
    }

    GtkEventController *keys = gtk_event_controller_key_new();
    g_signal_connect(keys, "key-pressed",
                     G_CALLBACK(on_key_pressed), display);
    gtk_widget_add_controller(window, keys);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{
    GtkApplication *app =
        gtk_application_new("org.example.gtk4calculator",
                            G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate",
                     G_CALLBACK(on_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
