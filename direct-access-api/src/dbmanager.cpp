#include "dbmanager.hpp"

dbmanager::dbmanager() {
  rc = sqlite3_open("daa.db", &db);

  //todo: PRAGMA foreign_keys = ON;


  if (rc) {
    cout << "Can't open database: " << sqlite3_errmsg(db) << "\n";
  } else {
    cout << "Connected to the database.." << endl;
  }
}

dbmanager::~dbmanager() { sqlite3_close(db); }

void dbmanager::insert_into_read_table(int vcanid, int canid) {
  char *zErrMsg = 0;

  stringstream ss;
  ss << "INSERT INTO READ VALUES(" << vcanid << "," << canid << ");";

  rc = sqlite3_exec(db, ss.str().c_str(), NULL, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
    sqlite3_free(zErrMsg);
  } else {
    cout << "SQL: insert_into_vcan_table successful" << endl;
  }
}

void dbmanager::insert_into_write_table(int vcanid, int canid) {
  char *zErrMsg = 0;

  stringstream ss;
  ss << "INSERT INTO WRITE VALUES(" << vcanid << "," << canid << ");";

  rc = sqlite3_exec(db, ss.str().c_str(), NULL, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
    sqlite3_free(zErrMsg);
  } else {
    cout << "SQL: insert_into_vcan_table successful" << endl;
  }
}

void dbmanager::insert_into_vcan_table(string vcan) {
  char *zErrMsg = 0;

  stringstream ss;
  ss << "INSERT INTO VCAN VALUES(NULL,\"" << vcan << "\");";

  rc = sqlite3_exec(db, ss.str().c_str(), NULL, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
    sqlite3_free(zErrMsg);
  } else {
    cout << "SQL: insert_into_vcan_table successful" << endl;
  }
}

int dbmanager::get_vcanid(string vcan_name) {
  char *zErrMsg = 0;
  stringstream ss;
  sqlite3_stmt *stmt;
  int vcanid = -1;

  ss << "SELECT VCANID FROM VCAN WHERE NAME = \"" << vcan_name << "\";";
  cout << ss.str() << endl;
  rc = sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
    sqlite3_free(zErrMsg);
    return vcanid;
  }
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    vcanid = sqlite3_column_int(stmt, 0);
  }

  sqlite3_finalize(stmt);
  return vcanid;
}

string dbmanager::get_vcan_name(int vcanid){
  char *zErrMsg = 0;
  stringstream ss;
  sqlite3_stmt *stmt;
  string vcan_name= "";

  ss << "SELECT NAME FROM VCAN WHERE VCANID = " << vcanid << ";";
  cout << ss.str() << endl;
  rc = sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
    sqlite3_free(zErrMsg);
    return vcan_name;
  }
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    vcan_name = string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
  }

  sqlite3_finalize(stmt);
  return vcan_name;
}

vector<int> dbmanager::get_read_rules(int vcanid) {
  char *zErrMsg = 0;
  stringstream ss;
  vector<int> results;
  sqlite3_stmt *stmt;

  ss << "SELECT CANID FROM READ WHERE VCANID = " << vcanid << ";";
  cout << ss.str() << endl;
  rc = sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
    sqlite3_free(zErrMsg);
    return results;
  }
  rc = sqlite3_step(stmt);
  while (rc != SQLITE_DONE) {
    results.push_back(sqlite3_column_int(stmt, 0));
    rc = sqlite3_step(stmt);
  }

  sqlite3_finalize(stmt);
  return results;
}

vector<int> dbmanager::select_vcanids_from_read_table(int canid) {
  char *zErrMsg = 0;
  stringstream ss;
  vector<int> results;
  sqlite3_stmt *stmt;

  ss << "SELECT VCANID FROM READ WHERE CANID = " << canid << ";";
  cout << ss.str() << endl;
  rc = sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
    sqlite3_free(zErrMsg);
    return results;
  }
  rc = sqlite3_step(stmt);
  while (rc != SQLITE_DONE) {
    results.push_back(sqlite3_column_int(stmt, 0));
    rc = sqlite3_step(stmt);
  }

  sqlite3_finalize(stmt);
  return results;
}

vector<int> dbmanager::get_write_rules(int vcanid) {
  char *zErrMsg = 0;
  stringstream ss;
  vector<int> results;
  sqlite3_stmt *stmt;

  ss << "SELECT CANID FROM WRITE WHERE VCANID = " << vcanid << ";";
  cout << ss.str() << endl;
  rc = sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
    sqlite3_free(zErrMsg);
    return results;
  }
  rc = sqlite3_step(stmt);
  while (rc != SQLITE_DONE) {
    results.push_back(sqlite3_column_int(stmt, 0));
    rc = sqlite3_step(stmt);
  }

  sqlite3_finalize(stmt);
  return results;
}

vector<int> dbmanager::select_vcanids_from_write_table(int canid) {
  char *zErrMsg = 0;
  stringstream ss;
  vector<int> results;
  sqlite3_stmt *stmt;

  ss << "SELECT VCANID FROM WRITE WHERE CANID = " << canid << ";";
  cout << ss.str() << endl;
  rc = sqlite3_prepare_v2(db, ss.str().c_str(), -1, &stmt, NULL);

  if (rc != SQLITE_OK) {
    cout << "SQL error: " << sqlite3_errmsg(db) << endl;
    sqlite3_free(zErrMsg);
    return results;
  }
  rc = sqlite3_step(stmt);
  while (rc != SQLITE_DONE) {
    results.push_back(sqlite3_column_int(stmt, 0));
    rc = sqlite3_step(stmt);
  }

  sqlite3_finalize(stmt);
  return results;
}

void dbmanager::store_read_rules(string vcan_name, json can_json) {

  int vcan_id = this->get_vcanid(vcan_name);

  auto perms = can_json["read"].as<std::deque<std::string>>();
  for (auto r : perms) {
    this->insert_into_read_table(vcan_id, stoi(r));
  }
}

void dbmanager::store_write_rules(string vcan_name, json can_json) {

  int vcan_id = this->get_vcanid(vcan_name);

  auto perms = can_json["write"].as<std::deque<std::string>>();
  for (auto r : perms) {
    this->insert_into_write_table(vcan_id, stoi(r));
  }
}

int main_test() {
  dbmanager db;
  vector<int> results;
  db.insert_into_read_table(1, 13);
  db.insert_into_write_table(1, 34);
 // results = db.select_canids_from_write_table(1);

  for (int k = 0; k < results.size(); k++) {
    cout << "K = " << results.at(k) << endl;
  }

  cout << db.get_vcanid("vcan2") << endl;

  return 0;
}
