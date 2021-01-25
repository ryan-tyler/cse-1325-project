#ifndef MOVIEPROGRAM_H
#define MOVIEPROGRAM_H
#include <gtkmm.h>

class Order{
public:
  std::string movie_name,movie_time,age_on_ticket;
  int number_of_tickets;
  double total_price;
  void set_movie_name (std::string a) { movie_name = a; }
  void set_movie_time (std::string a) { movie_time = a; }
  void set_user_age (std::string b) { age_on_ticket = b; }
  // the total number of movie tickets the user orders //
  void set_ticket_quantity (std::string b) { number_of_tickets = std::stoi(b); }
  std::string get_movie_name() { return movie_name; }
  std::string get_movie_time() { return movie_time; }
  std::string get_age_on_ticket() { return age_on_ticket; }


};

class menu: public Gtk::Window
{
public:
  std::vector<int> c; // 4 digit coupon code
  bool discount_applied = false;
  int counter = 0;
  std::vector<Order *>all_orders;
  //Order *order_ptr;
  menu();
  virtual ~menu();
protected:
  // signal handlers
  void movie_selection_window();
  void time_selection_window();
  void ticket_selection_window();
  void redeem_menu();
  void refund_menu();
  void concessions_menu();
  void confirmation_menu();
  void show_start_menu();
  void fill_coupons();

  // widgets
  Gtk::Box box;
  Gtk::Button purchase_button,redeem_button,refund_button,exit_button;
};

#endif
