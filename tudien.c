#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include <gdk/gdkkeysyms.h>
#include "goiy.h"
#include "soundex.h"
//Khai bao bien:
GtkWidget *GlobalForm           = NULL;
GtkWidget *TextBox001;
GtkWidget *GlobalTextBox001     = NULL;
GtkWidget *button;
GtkWidget *frame;
GtkWidget *window,*label,*entry,*list_view;
GtkWidget **help;
GtkWidget *themtu;
GtkWidget *suatu;
GtkWidget *xoatu;
GtkWidget *quit,*about;
GtkWidget *nhap_lb;
GtkTextBuffer*TextBox001_buffer;
GtkTextIter iter;
const gchar *list_item_data_key="list_item_data";
GtkWidget *scrolled_window,* scrolled2_window;
GtkWidget *gtklist;
GtkWidget *list_item;
GList *dlist;
guint i;
gchar buffer[256];
//sub window

GtkWidget *add_entry;
GtkWidget *add_textview;
GtkTextBuffer*add_textview_buffer;
GtkWidget *edit_entry;
GtkWidget *edit_textview;
GtkTextBuffer*edit_textview_buffer;
GtkWidget *delete_entry;


BTA*tree;

typedef struct xau2Rec
{
  char s1[30];
  char s2[10];
} xau2;
      
xau2 tachxau(char s[]);
long chiso(char ch,char*s);
long luythua(long a,long n);
long coso10(char *s);
BTA* nhapbtree(BTA*tree);
char*dichtiengviet(char t[]);
gint gtklist_click (GtkWidget *widget, GdkEventButton *event, gpointer func_data);
void buttontra_click( GtkWidget *widget, gpointer   data );
void entry_changed_cb( GtkWidget *widget, gpointer   data );
gboolean entry_key_press(GtkWidget *widget, GdkEventKey *pKey, gpointer user_data);
void insert_text_handler (GtkEntry    *entry, const gchar *text,  gint         length, gint        *position,  gpointer     data);
//sub
void delete_dialog( GtkWidget *widget, gpointer   data);
void ok_delete ( GtkWidget *widget, gpointer   data);
void add_dialog( GtkWidget *widget, gpointer   data);
void ok_add ( GtkWidget *widget, gpointer   data);
void edit_dialog( GtkWidget *widget, gpointer   data);
void ok_edit ( GtkWidget *widget, gpointer   data);
GdkPixbuf *create_pixbuf(const gchar * filename);

  void show_about(GtkWidget *widget, gpointer window)
{
  GtkWidget *dialog;
  dialog = gtk_about_dialog_new();
  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "EN-VI DICT");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "1.0"); 
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), 
				 "Tô Văn Hải\n Quán Văn Phú\nNguyễn Công Chỉnh\nTrần Công Khanh");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), 
				"từ điển làm với mục đích làm bài tập lớn ");
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}

//ham destroy se chay khi nhan nut X (thoat)
static void destroy( GtkWidget *widget,
                     gpointer   data )
{
    gtk_main_quit ();
}

int main(int argc, char **argv) 
{
  
  gtk_init(&argc, &argv);
  btinit();

  tree= btopn("tudienanhviet.dat",0,0);
  if(tree==NULL)
    {
      tree=nhapbtree(tree);
      btcls(tree);
    btopn("tudienanhviet.dat",0,0);
    }
  else
    btopn("tudienanhviet.dat",0,0);

  //Khoi tao cua so main:
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "EN_VN DICT");//title
  gtk_window_set_default_size(GTK_WINDOW(window),900 ,650);//kich thuoc mac dinh
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("dict.png"));//icon 
  
 //tao frame
  frame = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), frame);

  themtu =gtk_button_new_with_label("add");
  gtk_widget_set_size_request(themtu, 80, 35);
  gtk_fixed_put(GTK_FIXED(frame), themtu, 220, 80);
gtk_widget_show(themtu);
  suatu = gtk_button_new_with_label("edit");
  gtk_widget_set_size_request(suatu, 80, 35);
  gtk_fixed_put(GTK_FIXED(frame), suatu, 220, 120);
gtk_widget_show(suatu);
  xoatu = gtk_button_new_with_label("delete");
  gtk_widget_set_size_request(xoatu, 80, 35);
  gtk_fixed_put(GTK_FIXED(frame), xoatu, 220, 160);
gtk_widget_show(xoatu);
  about = gtk_button_new_with_label("About");
  gtk_widget_set_size_request(about, 80, 35);
  gtk_fixed_put(GTK_FIXED(frame), about, 220, 310);
gtk_widget_show(about);

  quit = gtk_button_new_with_label("Quit");
gtk_widget_set_size_request(quit, 80, 35);
  gtk_fixed_put(GTK_FIXED(frame),quit, 220, 200);
gtk_widget_show(quit);

  nhap_lb = gtk_label_new("input :");
  gtk_fixed_put(GTK_FIXED(frame), nhap_lb,13, 45);
gtk_widget_show(nhap_lb);
 gtk_widget_show_all(window);  
  g_signal_connect_swapped(G_OBJECT(window), "destroy",  G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
  g_signal_connect(G_OBJECT(themtu), "button-press-event",    G_CALLBACK(add_dialog), (gpointer)themtu);  
  g_signal_connect(G_OBJECT(suatu), "button-press-event",    G_CALLBACK(edit_dialog), (gpointer)suatu);  
  g_signal_connect(G_OBJECT(xoatu), "button-press-event",    G_CALLBACK(delete_dialog),(gpointer)xoatu);  
  g_signal_connect(G_OBJECT(quit), "button-press-event",    G_CALLBACK(gtk_main_quit), (gpointer)quit);
  
  gtk_widget_show_all(window);
  //create a text box
  entry = gtk_entry_new ();
  gtk_entry_set_max_length (GTK_ENTRY (entry),0);
  gtk_fixed_put(GTK_FIXED(frame), entry,53, 43);
  //khai bao su kien xay ra khi ki tu cua entry thay doi 
  g_signal_connect (G_OBJECT (entry), "changed",  G_CALLBACK (entry_changed_cb), button);

  g_signal_connect(G_OBJECT(entry), "insert_text",    G_CALLBACK(insert_text_handler),     NULL);
  
  g_signal_connect (G_OBJECT (entry), "key_press_event", G_CALLBACK (entry_key_press),NULL);
  
  gtk_widget_show (entry); 
  //gtk_widget_grab_focus(entry);
  //Tao 1 textview (chua nghia cua tu):,cai ten dat la textBox001 khong quan trong
 
  TextBox001 = gtk_text_view_new();
  GlobalTextBox001 = TextBox001;
  scrolled2_window=gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_size_request(scrolled2_window,550,600);
 gtk_fixed_put(GTK_FIXED(frame), scrolled2_window,330, 40);
  gtk_widget_show(scrolled2_window);
  gtk_scrolled_window_add_with_viewport( GTK_SCROLLED_WINDOW(scrolled2_window), TextBox001);
  
 // khai bao buff chua cac ki tu trong text_view
  TextBox001_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(TextBox001));
  gtk_text_buffer_get_iter_at_offset(TextBox001_buffer, &iter, 0);
  //Ham set text de xoa cac ki tu dang co trong buffer va gan lai gia tri
  gtk_text_buffer_set_text(TextBox001_buffer,"EN-VI DICT 1.0\n",-1); 
  
  gtk_widget_show (TextBox001); 
    
  //Create a new button 
  button = gtk_button_new_with_label("seach");
  gtk_widget_set_size_request(button, 80, 35);
  gtk_fixed_put(GTK_FIXED(frame),button, 220, 40);
  g_signal_connect (button, "clicked",   G_CALLBACK (buttontra_click), NULL);
  
  gtk_widget_show(button);
  
  scrolled_window=gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_size_request(scrolled_window,200,560);
  gtk_fixed_put(GTK_FIXED(frame), scrolled_window,13,80);
  gtk_widget_show(scrolled_window);
 

  gtklist=gtk_list_new();
  gtk_scrolled_window_add_with_viewport( GTK_SCROLLED_WINDOW(scrolled_window),  gtklist);
  gtk_widget_show(gtklist);
  g_signal_connect(G_OBJECT(gtklist),"button_press_event",  G_CALLBACK(gtklist_click), NULL);
   
  
  g_signal_connect(G_OBJECT(about), "button_press_event",    G_CALLBACK(show_about), NULL);
  gtk_widget_show_all(window);  
  g_signal_connect (window, "destroy",G_CALLBACK (destroy), NULL);
  gtk_main();

  return 0;
}

//ket thuc main
//su kien click item list

gint gtklist_click (GtkWidget *widget, GdkEventButton *event, gpointer func_data)
{
  if (GTK_IS_LIST(widget) &&   (event->type==GDK_2BUTTON_PRESS)&&(event->button==1))
    {     
      GtkWidget *list_item; GList *dlist; gchar *item_data_string;
      dlist=GTK_LIST(gtklist)->selection;
      list_item=GTK_OBJECT(dlist->data);
      item_data_string=gtk_object_get_data(list_item, list_item_data_key);
      //printf("%s\n",item_data_string);
    
      gtk_entry_set_text(  (GtkEntry*)entry,item_data_string);
      char *s,*s2; int value;
      s=(char*)malloc(20000*sizeof(char));
      s2=(char*)malloc(20000*sizeof(char));
      strcpy(s,gtk_entry_get_text((GtkEntry*)entry));
      if (bfndky(tree,s,&value)!=0)
	{
	  gtk_text_buffer_set_text(TextBox001_buffer,"Từ bạn nhập không có trong từ điển.\n",-1); 
	}
      else 
	{
	  btsel(tree,s,s2,20000,&value);
	  gtk_text_buffer_set_text(TextBox001_buffer,s2,-1); 
	}
      

    }
  return FALSE;
}

//Ham hello se chay khi nhan button "Tra"
void buttontra_click( GtkWidget *widget, gpointer   data )
{
  char *s,*s2; int value;
  s=(char*)malloc(20000*sizeof(char));
  s2=(char*)malloc(20000*sizeof(char));

  strcpy(s,gtk_entry_get_text((GtkEntry*)entry));
  
  if (bfndky(tree,s,&value)!=0)
    {
      gtk_text_buffer_set_text(TextBox001_buffer,"Từ bạn nhập không có trong từ điển.\n",-1); 

      gtk_list_clear_items(gtklist,0,100);        
      int i=0;
      char *ss1,*ss2,*s1;
      s1=(char*)malloc(20000*sizeof(char));
      ss1=(char*)malloc(4*sizeof(char));
      ss2=(char*)malloc(4*sizeof(char));
      strcpy(ss1,soundex(s));
      btpos(tree, 1);
      GtkWidget       *label;
      gchar           *string;
      
      while(btseln(tree,s1,s2,101,&value)==0&&i<500)
	{
	  strcpy(ss2,soundex(s1));
	  if(strcmp(ss2,ss1)==0)
	    {	      	      
	      sprintf(buffer,s1);
	      label=gtk_label_new(buffer);	
	      gtk_misc_set_alignment( GTK_MISC(label),0,0.5);
	      list_item=gtk_list_item_new();
	      gtk_container_add(GTK_CONTAINER(list_item), label);
	      gtk_widget_show(label);
	      gtk_container_add(GTK_CONTAINER(gtklist), list_item);
	      gtk_widget_show(list_item);
	      gtk_label_get(GTK_LABEL(label), &string);
	      gtk_object_set_data(GTK_OBJECT(list_item), list_item_data_key, string);	      
	      i++;
	    }
	}
      free(ss1);
      free(ss2);
      
    }
  else 
    {
      btsel(tree,s,s2,20000,&value);
      gtk_text_buffer_set_text(TextBox001_buffer,s2,-1); 
    }

}


//Ham entry_changed_cb se chay khi ki tu ben trong entry(tren win goi la textbox) thay odi
void entry_changed_cb( GtkWidget *widget, gpointer   data )
{
  char tmp[100];
  strcpy(tmp,gtk_entry_get_text((GtkEntry*)entry));
  int value,d,i=0;
  char s1[100],s2[100];
  btpos(tree, 1);

  GtkWidget       *label;
  gchar           *string;
  gtk_list_clear_items(gtklist,0,100);        
  while(btseln(tree,s1,s2, 101,&value)==0&&i<100)
    {
      if (ss2xau(s1,tmp)==1)
	{
	  i++;
	  
	  sprintf(buffer,s1);
	  label=gtk_label_new(buffer);	
	  gtk_misc_set_alignment( GTK_MISC(label),0,0.5);
	  list_item=gtk_list_item_new();
	  gtk_container_add(GTK_CONTAINER(list_item), label);
	  gtk_widget_show(label);
	  gtk_container_add(GTK_CONTAINER(gtklist), list_item);
	  gtk_widget_show(list_item);
	  gtk_label_get(GTK_LABEL(label), &string);
	  gtk_object_set_data(GTK_OBJECT(list_item), list_item_data_key, string);
	}
    }
}
//Ham entry_key_press se chay khi nhan 1 phim nao do khi dang o trong entry 
//nhung neu nhan enter hoac Tab se xay ra 1 so cau lenh.
gboolean entry_key_press(GtkWidget *widget, GdkEventKey *pKey, gpointer user_data)
{  
  if (pKey->type == GDK_KEY_PRESS){    
    switch (pKey->keyval)
      {
      case 65293 :
	{
	  char *s,*s2; int value;
	  s=(char*)malloc(20000*sizeof(char));
	  s2=(char*)malloc(20000*sizeof(char));
	  strcpy(s,gtk_entry_get_text((GtkEntry*)entry));
	  
	  if (bfndky(tree,s,&value)!=0)
	    {
	      gtk_text_buffer_set_text(TextBox001_buffer,"Từ bạn nhập không có trong từ điển.\n",-1); 
	      
	      gtk_list_clear_items(gtklist,0,100);        
	      int i=0;
	      char *ss1,*ss2,*s1;
	      s1=(char*)malloc(20000*sizeof(char));
	      ss1=(char*)malloc(4*sizeof(char));
	      ss2=(char*)malloc(4*sizeof(char));
	      strcpy(ss1,soundex(s));
	      btpos(tree, 1);
	      while(btseln(tree,s1,s2,101,&value)==0&&i<500)
		{
		  strcpy(ss2,soundex(s1));
		  if(strcmp(ss2,ss1)==0)
		    {
		      GtkWidget       *label;
		      gchar           *string;
		      
		      sprintf(buffer,s1);
		      label=gtk_label_new(buffer);	
		      gtk_misc_set_alignment( GTK_MISC(label),0,0.5);
		      list_item=gtk_list_item_new();
		      gtk_container_add(GTK_CONTAINER(list_item), label);
		      gtk_widget_show(label);
		      gtk_container_add(GTK_CONTAINER(gtklist), list_item);
		      gtk_widget_show(list_item);
		      gtk_label_get(GTK_LABEL(label), &string);
		      gtk_object_set_data(GTK_OBJECT(list_item), list_item_data_key, string);
		      
		      i++;
		      
		    }
		}
	    }
	  else 


	    {
	      btsel(tree,s,s2,20000,&value);
	      gtk_text_buffer_set_text(TextBox001_buffer,s2,-1); 
	    }
	  break;
	}
      case 65289 :
	{
	  gtk_list_select_item(gtklist,0);
	  GtkWidget *list_item; GList *dlist; gchar *item_data_string;
	  dlist=GTK_LIST(gtklist)->selection;
	  if(dlist==NULL)return;
	  list_item=GTK_OBJECT(dlist->data);
	  item_data_string=gtk_object_get_data(list_item, list_item_data_key);
	 
	  gtk_entry_set_text(  (GtkEntry*)entry,item_data_string);
	  char *s,*s2; int value;
	  s=(char*)malloc(20000*sizeof(char));
	  s2=(char*)malloc(20000*sizeof(char));
	  strcpy(s,gtk_entry_get_text((GtkEntry*)entry));
	  if (bfndky(tree,s,&value)!=0)
	    {
	      gtk_text_buffer_set_text(TextBox001_buffer,"Từ bạn nhập không có trong từ điển.\n",-1); 
	    }
	  else 
	    {
	      btsel(tree,s,s2,20000,&value);
	      gtk_text_buffer_set_text(TextBox001_buffer,s2,-1); 
	    }
	  
	  break;
	}	

      }
  }
  
  return FALSE;
}

void insert_text_handler (GtkEntry    *entry,   const gchar *text,
                          gint         length,
                          gint        *position,
                          gpointer     data)
{
  GtkEditable *editable = GTK_EDITABLE(entry);
  int i, count=0;
  gchar *result = g_new (gchar, length);

    for (i=0; i < length; i++) {
   
  result[count++] =text[i];
    }
  if (count > 0) {
    g_signal_handlers_block_by_func (G_OBJECT (editable),
                                     G_CALLBACK (insert_text_handler),
                                     data);
    gtk_editable_insert_text (editable, result, count, position);
    g_signal_handlers_unblock_by_func (G_OBJECT (editable),
                                       G_CALLBACK (insert_text_handler),
                                       data);
  }
  g_signal_stop_emission_by_name (G_OBJECT (editable), "insert_text");
  g_free (result);
}

void delete_dialog( GtkWidget *widget, gpointer   data) {
  
  GtkWidget *dialog, *label, *ok_button,*cancel_button,*table;
  GtkWidget *TextBox001;
  GtkWidget *GlobalTextBox001     = NULL;
  GtkWidget *scrolled_window,* scrolled2_window;
 
  GtkTextIter iter;
  dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);
  
  gtk_widget_set_size_request(dialog, 400,150);
  gtk_window_set_resizable(GTK_WINDOW(dialog),0);
  gtk_window_set_title(GTK_WINDOW(dialog), "Delete");
  
  //   dialog = gtk_dialog_new();
  label = gtk_label_new ("Enter text: ");
  delete_entry=gtk_entry_new();
  ok_button = gtk_button_new_with_label("Ok");
  cancel_button = gtk_button_new_with_label("Cancel");

  table = gtk_table_new (5,3, TRUE);
  gtk_container_add (GTK_CONTAINER (dialog), table);
  
  gtk_widget_show (table);
  gtk_table_attach_defaults (GTK_TABLE (table),label, 0, 1, 0, 2);
  gtk_table_attach_defaults (GTK_TABLE (table),delete_entry, 1, 5, 0, 2);
  gtk_table_attach_defaults (GTK_TABLE (table),ok_button, 1, 2, 3,4);
  gtk_table_attach_defaults (GTK_TABLE (table),cancel_button, 2, 3,3,4);
   
  gtk_signal_connect_object (GTK_OBJECT (cancel_button), "clicked",  GTK_SIGNAL_FUNC (gtk_widget_destroy), dialog);
  gtk_signal_connect_object (GTK_OBJECT (ok_button), "clicked",      GTK_SIGNAL_FUNC (ok_delete), dialog);
  gtk_widget_show_all (dialog);
}

void ok_delete ( GtkWidget *widget, gpointer   data)
{
  const gchar *s;
  s=gtk_entry_get_text(GTK_ENTRY(delete_entry));
  if(  strcmp(s,"")==0)
    {
      return;
    }
  btpos(tree, 1);
  int value;
  if(bfndky(tree,s,&value)==0)
    {
      btdel(tree,s);
      GtkWidget *deleteok_dialog;
      deleteok_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
					       GTK_DIALOG_DESTROY_WITH_PARENT,
					       GTK_MESSAGE_INFO,
					       GTK_BUTTONS_OK,
					       "Đã xóa thành công\n", "title");
      gtk_window_set_title(GTK_WINDOW(deleteok_dialog), "Information");
      gtk_dialog_run(GTK_DIALOG(deleteok_dialog));
      gtk_widget_destroy(deleteok_dialog);
    }
  else
    {
      
      GtkWidget *deleteerr_dialog;
      deleteerr_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
						GTK_DIALOG_DESTROY_WITH_PARENT,
						GTK_MESSAGE_ERROR,
						GTK_BUTTONS_OK,
						"Từ bạn nhập không có trong từ điển\n");
      gtk_window_set_title(GTK_WINDOW(deleteerr_dialog), "Error");
      gtk_dialog_run(GTK_DIALOG(deleteerr_dialog));
      gtk_widget_destroy(deleteerr_dialog);
       }
  gtk_widget_destroy(widget);
}


void add_dialog( GtkWidget *widget, gpointer   data) {
  
  GtkWidget *dialog, *label, *ok_button,*cancel_button,*table;
  GtkWidget *TextBox001;
  GtkWidget *GlobalTextBox001     = NULL;
  GtkWidget *scrolled_window,* scrolled2_window;
  
  GtkTextIter iter;
  dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);
  
  gtk_widget_set_size_request(dialog, 600,400);
  gtk_window_set_resizable(GTK_WINDOW(dialog),0);
  gtk_window_set_title(GTK_WINDOW(dialog), "Add");
  
  label = gtk_label_new ("Enter text: ");
  add_entry=gtk_entry_new();
  ok_button = gtk_button_new_with_label("Ok");
  cancel_button = gtk_button_new_with_label("Cancel");
  table = gtk_table_new (10,10, TRUE);
  gtk_container_add (GTK_CONTAINER (dialog), table);
  
  gtk_widget_show (table);
  gtk_table_attach_defaults (GTK_TABLE (table),label, 0, 3, 0, 2);
  gtk_table_attach_defaults (GTK_TABLE (table),add_entry, 3, 6, 0, 2);
  gtk_table_attach_defaults (GTK_TABLE (table),ok_button, 3, 5, 9, 10);
  gtk_table_attach_defaults (GTK_TABLE (table),cancel_button, 5, 7, 9, 10);
  TextBox001 = gtk_text_view_new();
  GlobalTextBox001 = TextBox001;
  scrolled2_window=gtk_scrolled_window_new(NULL, NULL);
  gtk_table_attach_defaults (GTK_TABLE (table),scrolled2_window,0,10,2,8.5);
  gtk_widget_show(scrolled2_window);
  gtk_scrolled_window_add_with_viewport( GTK_SCROLLED_WINDOW(scrolled2_window), TextBox001);
  
  // khai bao buff chua cac ki tu trong text_view
  add_textview_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(TextBox001));
  gtk_text_buffer_get_iter_at_offset(add_textview_buffer, &iter, 0);
  
  gtk_signal_connect_object (GTK_OBJECT (cancel_button), "clicked",  GTK_SIGNAL_FUNC (gtk_widget_destroy), dialog);
  gtk_signal_connect_object (GTK_OBJECT (ok_button), "clicked",      GTK_SIGNAL_FUNC (ok_add), dialog);
  
  gtk_widget_show_all (dialog);
}

void ok_add ( GtkWidget *widget, gpointer   data)
{
  const gchar *s;
  s=gtk_entry_get_text(GTK_ENTRY(add_entry));
  GtkTextIter start;
  GtkTextIter end;
  gchar *text;
  gtk_text_buffer_get_start_iter (add_textview_buffer, &start);
  gtk_text_buffer_get_end_iter (add_textview_buffer, &end);
  text = gtk_text_buffer_get_text (add_textview_buffer, &start, &end, FALSE);
  g_print ("%s", text);
  if(  strcmp(s,"")==0||strcmp(text,"")==0)
    {
      return;
    }
  int value;
  if (bfndky(tree,s,&value)==0)
    {
      GtkWidget *deleteerr_dialog;
      deleteerr_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
						GTK_DIALOG_DESTROY_WITH_PARENT,
						GTK_MESSAGE_ERROR,
						GTK_BUTTONS_OK,
						"Từ bạn nhập đã có trong từ điển\n");
      gtk_window_set_title(GTK_WINDOW(deleteerr_dialog), "Error");
      gtk_dialog_run(GTK_DIALOG(deleteerr_dialog));
      gtk_widget_destroy(deleteerr_dialog);
    
    }
  else
    {
      btins(tree,s,text,strlen(text)+1);
      //      add(tree,s,text);
      
      GtkWidget *deleteok_dialog;
      deleteok_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
					       GTK_DIALOG_DESTROY_WITH_PARENT,
					       GTK_MESSAGE_INFO,
					       GTK_BUTTONS_OK,
					       "Đã thêm thành công\n", "title");
      gtk_window_set_title(GTK_WINDOW(deleteok_dialog), "Information");
      gtk_dialog_run(GTK_DIALOG(deleteok_dialog));
      gtk_widget_destroy(deleteok_dialog);
    }
  
  g_free (text);
  gtk_widget_destroy(widget);
}
void edit_dialog( GtkWidget *widget, gpointer   data) {
  
  GtkWidget *dialog, *label, *ok_button,*cancel_button,*table;
  GtkWidget *TextBox001;
  GtkWidget *GlobalTextBox001     = NULL;
  GtkWidget *scrolled_window,* scrolled2_window;
  
  GtkTextIter iter;
  /* Create the widgets */
  dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);
  
  gtk_widget_set_size_request(dialog, 600,400);
  gtk_window_set_resizable(GTK_WINDOW(dialog),0);
  gtk_window_set_title(GTK_WINDOW(dialog), "Edit");
  
  //   dialog = gtk_dialog_new();
  label = gtk_label_new ("Enter text: ");
  edit_entry=gtk_entry_new();
  ok_button = gtk_button_new_with_label("Ok");
  cancel_button = gtk_button_new_with_label("Cancel");
  table = gtk_table_new (10,10, TRUE);
  gtk_container_add (GTK_CONTAINER (dialog), table);
  
  gtk_widget_show (table);
  gtk_table_attach_defaults (GTK_TABLE (table),label, 0, 3, 0, 2);
  gtk_table_attach_defaults (GTK_TABLE (table),edit_entry, 3, 6, 0, 2);
  gtk_table_attach_defaults (GTK_TABLE (table),ok_button, 3, 5, 9, 10);
  gtk_table_attach_defaults (GTK_TABLE (table),cancel_button, 5, 7, 9, 10);
  
  TextBox001 = gtk_text_view_new();
  GlobalTextBox001 = TextBox001;
  scrolled2_window=gtk_scrolled_window_new(NULL, NULL);
  gtk_table_attach_defaults (GTK_TABLE (table),scrolled2_window,0,10,2,8.5);
  gtk_widget_show(scrolled2_window);
  gtk_scrolled_window_add_with_viewport( GTK_SCROLLED_WINDOW(scrolled2_window), TextBox001);
  
  // khai bao buff chua cac ki tu trong text_view
  edit_textview_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(TextBox001));
  gtk_text_buffer_get_iter_at_offset(edit_textview_buffer, &iter, 0);
   /* Ensure that the dialog box is destroyed whe1n the user clicks ok. */
   
  gtk_signal_connect_object (GTK_OBJECT (cancel_button), "clicked",  GTK_SIGNAL_FUNC (gtk_widget_destroy), dialog);
  gtk_signal_connect_object (GTK_OBJECT (ok_button), "clicked",      GTK_SIGNAL_FUNC (ok_edit), dialog);
  // gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox),      label);
  gtk_widget_show_all (dialog);
}


void ok_edit ( GtkWidget *widget, gpointer   data)
{
  const gchar *s;
  
  s=gtk_entry_get_text(GTK_ENTRY(edit_entry));
  GtkTextIter start;
  GtkTextIter end;
  gchar *text;
  gtk_text_buffer_get_start_iter (edit_textview_buffer, &start);
  gtk_text_buffer_get_end_iter (edit_textview_buffer, &end);
  text = gtk_text_buffer_get_text (edit_textview_buffer, &start, &end, FALSE);
  g_print ("%s", text);
  
  if(  strcmp(s,"")==0||strcmp(text,"")==0)
    {
      return;
    }
  

  int value;
  if (bfndky(tree,s,&value)==0)
    {
      btdel(tree,s);
      btins(tree,s,text,strlen(text)+1);
      
      //      add(tree,s,text);
      GtkWidget *deleteok_dialog;
      deleteok_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
					       GTK_DIALOG_DESTROY_WITH_PARENT,
					       GTK_MESSAGE_INFO,
					       GTK_BUTTONS_OK,
					       "Đã sửa thành công\n", "title");
      gtk_window_set_title(GTK_WINDOW(deleteok_dialog), "Information");
      gtk_dialog_run(GTK_DIALOG(deleteok_dialog));
      gtk_widget_destroy(deleteok_dialog);

  
    }
  else
    {      


 GtkWidget *deleteerr_dialog;
      deleteerr_dialog = gtk_message_dialog_new(GTK_WINDOW(window),
						GTK_DIALOG_DESTROY_WITH_PARENT,
						GTK_MESSAGE_ERROR,
						GTK_BUTTONS_OK,
						"Từ bạn nhập không có trong từ điển\n");
      gtk_window_set_title(GTK_WINDOW(deleteerr_dialog), "Error");
      gtk_dialog_run(GTK_DIALOG(deleteerr_dialog));
      gtk_widget_destroy(deleteerr_dialog);
    }
 
  g_free (text);
  gtk_widget_destroy(widget);
}
////////////////////nhap vao btree
BTA* nhapbtree(BTA*tree)
{
  int value;
  FILE*fin;
  char s1[100],s2[40],s[100],*s3;
  tree=btcrt("tudienanhviet.dat",0,0);
  btopn("tudienanhviet.dat",0,0);
  fin=fopen("anhviet109K.index","r");
  while(!feof(fin))
    {
      fscanf(fin,"%[^\t]\t%[^\n]\n",s1,s2);
      s3=dichtiengviet(s2);
      if (bfndky(tree,s1,&value)!=0)
	{
	  btins(tree,s1,s3,strlen(s3)+1);
	}
      free(s3);
    }
  fclose(fin);
  return tree;
}

xau2 tachxau(char s[])
{
  xau2 a;
  int i,n,d=0,k=1,t=1;
  n=strlen(s);
  for (i=1;i<=n;i++)
    {
      if(s[i-1]=='\t') {d=1;i++;}
      if(d==0)  
	{
	  a.s1[t-1]=s[i-1];
	  t++;
	}
      if(d==1)  
	{
	  a.s2[k-1]=s[i-1];
	  k++;
	}
      a.s1[t-1]='\0';
      a.s2[k-1]='\0';
    }
  return a;
}


long chiso(char ch,char*s)
{
  long i,n=strlen(s);
  for(i=0;i<n;i++)
    if(ch==s[i]) 
      return i;
}
long luythua(long a,long n)
{
  long i,lt;
  lt=1;
  if(n==0)
    return 1;
  else
    {
      for(i=1;i<=n;i++)
	{
	  lt=lt*a;
	}
      return lt;
    }
}
long coso10(char *s)
{
  long i,n=strlen(s);
  long vt,coso=0;
  char ma64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  for(i=0;i<n;i++)
    {
      vt=chiso(s[i],ma64);
      coso += (long)vt*luythua(64,n-1-i);
    }
  return coso;
}
char*dichtiengviet(char t[])
{
  char*s;
  long vitri,dodai;
  xau2 a;
  FILE*fin;
  fin=fopen("anhviet109K.dict","r");
  a=tachxau(t);
  vitri=coso10(a.s1);
  dodai=coso10(a.s2);
  s=(char*)malloc(sizeof(char)*(dodai+1));
 
  fseek(fin,vitri,SEEK_SET);
  fread(s,sizeof(char),dodai,fin);
  s[dodai]='\0';
  fclose(fin);
  return s;
}
GdkPixbuf *create_pixbuf(const gchar * filename)
{
  GdkPixbuf *pixbuf;
  GError *error = NULL;
  pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   if(!pixbuf) {
     fprintf(stderr, "%s\n", error->message);
     g_error_free(error);
   }

   return pixbuf;
   
}

