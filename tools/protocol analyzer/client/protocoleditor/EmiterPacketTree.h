#ifndef _EMITER_PACKET_TREE_H_
#define _EMITER_PACKET_TREE_H_

#include <gtk/gtk.h>

class EmiterPacketTree
{
public:
    enum Collumns
    {
        COL_ICON, COL_DESC, COL_PORT, NUM_COLS
    };

    EmiterPacketTree();
    GtkWidget *GetWidget();

private:
    GtkWidget *treeWidget;
    GtkTreeStore *treeStore;
};

#endif//_EMITER_PACKET_TREE_H_
