#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <iostream>
#include <sqlite3.h>
#include <sstream>
#include <vector>
#include <jsoncons/json.hpp>

using namespace std;

using namespace jsoncons;
using jsoncons::json;


class dbmanager {
 private:
  int rc;
  sqlite3 *db;

 public:
  dbmanager();
  ~dbmanager();
  void store_read_rules(string vcan_name, json can_json);
  void store_write_rules(string vcan_name, json can_json);
  void insert_into_read_table(int vcanid, int canid);
  void insert_into_write_table(int vcanid, int canid);
  void insert_into_vcan_table(string vcan_name);
  vector<int> get_read_rules(int vcanid);
  vector<int> get_write_rules(int vcanid);

  vector<int> select_vcanids_from_read_table(int canid);
  vector<int> select_vcanids_from_write_table(int canid);

  int get_vcanid(string vcan_name);
  string get_vcan_name(int vcanid);


};

#endif  // DBMANAGER_H
