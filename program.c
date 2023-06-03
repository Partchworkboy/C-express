#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

sqlite3 *db;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    for (int i = 0; i < argc; i++) {
        printf("%s: %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void createTable() {
    char *errMsg;
    const char *sql = "CREATE TABLE IF NOT EXISTS Deposits("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "Name TEXT NOT NULL,"
                      "Date TEXT NOT NULL,"
                      "Amount REAL NOT NULL,"
                      "Notes TEXT"
                      ");";
    int rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }
}

void insertData(char *name, char *date, double amount, char *notes) {
    char *errMsg;
    char sql[1000];
    snprintf(sql, sizeof(sql),
             "INSERT INTO Deposits(Name, Date, Amount, Notes) VALUES('%s', '%s', %f, '%s');",
             name, date, amount, notes);
    int rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        fprintf(stdout, "Data inserted successfully\n");
    }
}

void displayData() {
    char *errMsg;
    const char *sql = "SELECT * FROM Deposits;";
    int rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

int main() {
    int rc = sqlite3_open("database.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    createTable();

    char name[100], date[20], notes[1000];
    double amount;
    printf("Enter Name: ");
    scanf("%s", name);
    printf("Enter Date: ");
    scanf("%s", date);
    printf("Enter Amount: ");
    scanf("%lf", &amount);
    printf("Enter Notes: ");
    scanf(" %[^\n]s", notes);

    insertData(name, date, amount, notes);

    printf("Data saved successfully\n");

    displayData();

    sqlite3_close(db);
    return 0;
}

