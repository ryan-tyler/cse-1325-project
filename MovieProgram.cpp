#include "MovieProgram.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include "stdlib.h"

float ticketprice = 7.50;
float concessions_total;
int order_number;


void menu::fill_coupons()
{
  /*
  1 - 1098
  2 - 2004
  3 - 4513
  4 - 3523
  5 - 5559
  6 - 2308
  7 - 1376
  8 - 4873
  9 - 2410
  10- 4027
  */

  c.push_back(1098); // code 1
  c.push_back(2004); // code 2
  c.push_back(4513); // code 3
  c.push_back(3523); // code 4
  c.push_back(5559); // code 5
  c.push_back(2308); // code 6
  c.push_back(1376); // code 7
  c.push_back(4873); // code 8
  c.push_back(2410); // code 9
  c.push_back(4027); // code 10
}

menu::menu():box(Gtk::ORIENTATION_VERTICAL),purchase_button("Purchase"),redeem_button("Redeem"),refund_button("Refund"),exit_button("Exit")
{
  set_size_request(600,500);
	set_title("~~WELCOME TO THE MOVIES!!!!!~~");
	add(box);
  fill_coupons();
  purchase_button.signal_pressed().connect(sigc::mem_fun(*this,&menu::movie_selection_window));
	box.pack_start(purchase_button);
	redeem_button.signal_pressed().connect(sigc::mem_fun(*this,&menu::redeem_menu));
	box.pack_start(redeem_button);
  refund_button.signal_pressed().connect(sigc::mem_fun(*this,&menu::refund_menu));
	box.pack_start(refund_button);
  exit_button.signal_pressed().connect(sigc::mem_fun(*this,&menu::close));
	box.pack_start(exit_button);

  show_all_children();
}
menu::~menu()
{
  counter = 0;
  while ( counter < all_orders.size() )
  {
    delete all_orders[counter];
    counter++;
  }
}

void menu::movie_selection_window()
{
  Order *newOrder = new Order();
  Gtk::Window *w = new Gtk::Window();
  Gtk::Dialog *dialog = new Gtk::Dialog();
  dialog->set_transient_for(*w);
  dialog->set_size_request(400,100);
  dialog->set_title("~~Movie Selection~~");
  Gtk::Label *label = new Gtk::Label("Enter movie you want to see:");
  dialog->get_content_area()->pack_start(*label);
  label->show();
  Gtk::Entry *entry = new Gtk::Entry();
  entry->set_text("<movie title>");
  entry->set_max_length(50);
  entry->show();
  dialog->get_vbox()->pack_start(*entry);
  dialog->add_button("Enter",1);
  dialog->set_default_response(0);
  int result = dialog->run();
  std::string movie_name = entry->get_text();

  newOrder->set_movie_name(movie_name);

  dialog->hide();
  //  //  //  //  //  //  //  // // // // // // // // // // // // // // // // // // // // // // // //
  // dialog window is already set; the configuration needs to be tweaked //

  label->set_text("Enter movie time:");
  entry->set_text("<movie time>");
  dialog->show();
  result = dialog->run();
  std::string movie_time = entry->get_text();
  newOrder->set_movie_time(movie_time);
  //std::cout << newOrder->get_movie_time() << std::endl;
  dialog->hide();
  //  //  //  //  //  //  //  // // // // // // // // // // // // // // // // // // // // // // // //

  label->set_text("Enter Number of Adult tickets:");
  Gtk::Label *label2 = new Gtk::Label("Enter Number of children[12 and under]tickets:");
  dialog->get_content_area()->pack_start(*label2);
  label2->show();

  entry->set_text("");
  Gtk::Entry *entry2 = new Gtk::Entry();
  entry2->set_text("");
  entry2->set_max_length(50);
  entry2->show();
  dialog->get_vbox()->pack_start(*entry2);

  dialog->show();
  result = dialog->run();
  //  //  //  //  //  //  //  // // // // // // // // // // // // // // // // // // // // // // // //

  delete w,label,label2,entry,dialog;
  concessions_menu();
}

void menu::redeem_menu()
{
  Gtk::Window w;
  Gtk::Dialog *dialog = new Gtk::Dialog();
  dialog->set_transient_for(w);
  dialog->set_size_request(300,200);
  dialog->set_title("~~Redeem Menu~~");

  //std::shared_ptr<Gtk::Label>label = std::make_shared<Gtk::Label>("Enter coupon code:");
  Gtk::Label *label = new Gtk::Label("Enter coupon code(Example 1234):");
  dialog->get_content_area()->pack_start(*label);
  label->show();

  dialog->add_button("Cancel",0);
  dialog->add_button("Ok",1);
  //dialog->set_default_response(0);

  //std::shared_ptr<Gtk::Entry>user_coupon = std::make_shared<Gtk::Entry>();
  Gtk::Entry *user_coupon = new Gtk::Entry();
  user_coupon->set_text("");
  user_coupon->set_max_length(50);
  user_coupon->show();
  dialog->get_vbox()->pack_start(*user_coupon);

  int result = dialog->run();
  if ( result == 0 ) { dialog->close(); }
  std::string temp = user_coupon->get_text();

  Gtk::MessageDialog *true_msg = new Gtk::MessageDialog(*this,"20% discount applied!",false,Gtk::MESSAGE_INFO);
  Gtk::MessageDialog *false_msg = new Gtk::MessageDialog(*this,"No discount available.",false,Gtk::MESSAGE_INFO);

   if(!temp.empty()){
   int code = std::stoi(temp);
   for(int i=0; i<c.size(); i++)
   {
     if ( c[i] == code )
     {
       discount_applied = true;
       c.erase(c.begin()+i);
     }
   }

   if ( discount_applied && result != 0 )
   {
     dialog->close();
     true_msg->run();
   }
   else if ( discount_applied == false && result !=0 )
   {
     dialog->close();
     false_msg->run();
   }

   }
    delete dialog;
    delete label;
    delete user_coupon;
    delete true_msg;
    delete false_msg;

}
void menu::refund_menu()
{
  Gtk::Window w;
  Gtk::Dialog *dialog = new Gtk::Dialog();
  dialog->set_transient_for(w);
  dialog->set_size_request(300,200);
  dialog->set_title("~~Refund Menu~~");

  Gtk::Label *label = new Gtk::Label("Enter order number:");
  dialog->get_content_area()->pack_start(*label);
  label->show();

  dialog->add_button("Cancel",0);
  dialog->add_button("Ok",1);
  dialog->set_default_response(1);

  //std::shared_ptr<Gtk::Entry>user_coupon = std::make_shared<Gtk::Entry>();
  Gtk::Entry *user_coupon = new Gtk::Entry();
  user_coupon->set_text("<Order Number>");
  user_coupon->set_max_length(50);
  user_coupon->show();
  dialog->get_vbox()->pack_start(*user_coupon);

  int result = dialog->run();
  std::string order_num = user_coupon->get_text();

  bool order_found = false;
  Gtk::MessageDialog *true_msg = new Gtk::MessageDialog(*this,"Order found; refund applied!",false,Gtk::MESSAGE_INFO);
  Gtk::MessageDialog *false_msg = new Gtk::MessageDialog(*this,"No order number or incorrect number; try again...",false,Gtk::MESSAGE_INFO);

  /*for(int i=0; i<all_orders.size();i++)
  {
    if(order_num.compare(all_orders[i])==0)
    {
      order_found=true;
    }
  }

  if (discount_applied) { true_msg->run(); }
  else if(discount_applied==false) { false_msg->run(); }*/

    delete dialog;
    delete label;
    delete user_coupon;
    delete true_msg;
    delete false_msg;
}
void menu:: concessions_menu()
{
    Gtk::Window w;
    Gtk::Dialog *dialog = new Gtk::Dialog();
  dialog->set_transient_for(w);
  dialog->set_size_request(600,400);
  dialog->set_title("~~Concessions Menu~~");

  Gtk::Label *label = new Gtk::Label("Enter Number of Popcorn($5.75 each):");
  dialog->get_content_area()->pack_start(*label);
  label->show();
  Gtk::Entry *popcorn = new Gtk::Entry();
  popcorn->set_text("0");
  popcorn->set_max_length(2);
  popcorn->show();
  dialog->get_vbox()->pack_start(*popcorn);

  Gtk::Label *label2 = new Gtk::Label("Enter Number of Drinks($4.00 each):");
  dialog->get_content_area()->pack_start(*label2);
  label2->show();
  Gtk::Entry *drinks = new Gtk::Entry();
  drinks->set_text("0");
  drinks->set_max_length(2);
  drinks->show();
  dialog->get_vbox()->pack_start(*drinks);

  Gtk::Label *label3 = new Gtk::Label("Enter Number of Hot dogs($4.50 each):");
  dialog->get_content_area()->pack_start(*label3);
  label3->show();
  Gtk::Entry *hot_dog = new Gtk::Entry();
  hot_dog->set_text("0");
  hot_dog->set_max_length(2);
  hot_dog->show();
  dialog->get_vbox()->pack_start(*hot_dog);

  Gtk::Label *label4 = new Gtk::Label("Enter Number of Nachos($4.50 each):");
  dialog->get_content_area()->pack_start(*label4);
  label4->show();
  Gtk::Entry *nachos = new Gtk::Entry();
  nachos->set_text("0");
  nachos->set_max_length(2);
  nachos->show();
  dialog->get_vbox()->pack_start(*nachos);

  Gtk::Label *label5 = new Gtk::Label("Enter Number of Candy($3.50 each):");
  dialog->get_content_area()->pack_start(*label5);
  label5->show();
  Gtk::Entry *candy = new Gtk::Entry();
  candy->set_text("0");
  candy->set_max_length(2);
  candy->show();
  dialog->get_vbox()->pack_start(*candy);

  dialog->add_button("Cancel",0);
  dialog->add_button("Ok",1);
  dialog->set_default_response(1);
  dialog->run();

  std::string value = popcorn->get_text();
  float popcorn_price = 5.75;
  float popcorn_number  = std::stof(value);
  float popcorn_total = popcorn_price*popcorn_number;


  std::string value2 = drinks->get_text();
  float drinks_price = 4.00;
  float drinks_number  = std::stof(value2);
  float drinks_total = drinks_price*drinks_number;


  std::string value3 = hot_dog->get_text();
  float hot_dog_price = 4.00;
  float hot_dog_number  = std::stof(value3);
  float hot_dog_total = hot_dog_price*hot_dog_number;


  std::string value4 = nachos->get_text();
  float nachos_price = 4.00;
  float nachos_number  = std::stof(value4);
  float nachos_total = nachos_price*nachos_number;



  std::string value5 = candy->get_text();
  float candy_price = 3.50;
  float candy_number  = std::stof(value5);
  float candy_total = drinks_price*candy_number;

  float concessions_total = popcorn_total+drinks_total+nachos_total+candy_total+hot_dog_total;



    delete dialog;
    delete label;
    delete label2;
    delete label3;
    delete label4;
    delete label5;
    delete popcorn;
    delete candy;
    delete nachos;
    delete drinks;
    delete hot_dog;
    confirmation_menu();
}
void menu:: confirmation_menu()
{

    srand (time(NULL));
    int order_number = rand () % 10000;
    std::string order_2 = std::to_string(order_number);
    float total_amount = concessions_total + ticketprice;

    std::ostringstream ss;
    ss << total_amount;
    std::string s(ss.str());

    Gtk::Window w;
    Gtk::Dialog *dialog = new Gtk::Dialog();
  dialog->set_transient_for(w);
  dialog->set_size_request(600,400);
  dialog->set_title("~~Confirmation Window~~");
  Gtk::Label *label = new Gtk::Label("Thank you for ordering concessions or a movie ticket!!!");
  dialog->get_content_area()->pack_start(*label);
  label->show();
  Gtk::Label *label2 = new Gtk::Label("Would you like to confirm your purchase?");
  dialog->get_content_area()->pack_start(*label2);
  label2->show();
  Gtk::Label *label3 = new Gtk::Label("Order number:");
  dialog->get_content_area()->pack_start(*label3);
  label3->show();
  Gtk::Label *label5 = new Gtk::Label(order_2);
  dialog->get_content_area()->pack_start(*label5);
  label5->show();
  Gtk::Label *label4 = new Gtk::Label("Total Price: ");
  dialog->get_content_area()->pack_start(*label4);
  label4->show();
  Gtk::Label *label6 = new Gtk::Label(s);
  dialog->get_content_area()->pack_start(*label6);
  label6->show();

  dialog->add_button("Cancel",0);
  dialog->add_button("Ok",1);
  dialog->set_default_response(1);
  dialog->run();

  delete dialog;
  delete label;
  delete label2;
  delete label3;
  delete label4;
  delete label5;
  delete label6;
}
