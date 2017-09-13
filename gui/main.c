#include <gtk/gtk.h>
#include "menu.h"
#include <globals.h>
#include <string.h>

static GtkWidget *
create_tree_view (void)
{

  GtkWidget *view;
  GtkTreeViewColumn *col;

  view = gtk_tree_view_new();


  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Files / Folders");
  gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(view), FALSE);
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  return view;
}

void  on_changed(GtkWidget *widget, gpointer statusbar) 
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  char *value;
  char out[MAX_PATH_SIZE + 80];
	int isDir;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, 1, &value,  -1);
	out[0]='\0';
	strcat(out," Full Path = ");
	strcat(out,value);

    gtk_tree_model_get(model, &iter, 2, &isDir,  -1);
	if (isDir) {
		strcat(out,"  (Folder)");
	} else {
		strcat(out,"  (Text File)");
	}
    gtk_statusbar_push(GTK_STATUSBAR(statusbar),
        gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar), 
            value), out);
    g_free(value);
  }
}


int
main (int argc, char **argv)
{
    GtkWidget *window;
    GtkWidget *view, *sw;
    GtkWidget *vbox;
    GtkWidget *statusbar;
	GtkWidget *menubar;
	GtkTreeModel *model;
	GtkTreeViewColumn *col;
	GtkCellRenderer *renderer;
	GtkTreeSelection  *selection;


    gtk_init(&argc, &argv);

	//Create the window, scrolled pane and add the treeview and status bar

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    sw = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(sw),
                     GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(sw),
                        GTK_SHADOW_ETCHED_IN);


    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_title(GTK_WINDOW(window), "Virtual File System");
    gtk_widget_set_size_request (window, 300, 400);


    view = create_tree_view();


	col = gtk_tree_view_column_new();
//	gtk_tree_view_column_set_title(col, "Files & Folders");
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, 
		  "text", 0);

	selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);

    vbox = gtk_vbox_new(FALSE, 2);

	gtk_box_pack_start(GTK_BOX(vbox), get_menubar_menu(window, view), FALSE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 0);

    statusbar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(sw), view);
    gtk_container_add(GTK_CONTAINER(window), vbox);


	//Connect the required signal handlers
	g_signal_connect(selection, "changed", G_CALLBACK(on_changed), statusbar);

    g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK(gtk_main_quit), NULL);

	setup_context_menu(view);

    gtk_widget_show_all(window);

	//this is a blocking call
    gtk_main();

    return 0;
}

