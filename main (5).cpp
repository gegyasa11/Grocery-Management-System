#include <iostream>
#include <mysql.h>
#include <mysqld_error.h>
#include <sstream>
#include <limits>
using namespace std;

char host[] = "localhost";
char user[] = "root";
char pass[] = "password"; //Enter your MySQl server password.

void showMainMenu() {
	cout<<endl;
    cout << "MAIN MENU" << endl << endl;
    cout << "Select option from below >" << endl;
    cout << "[0]: Close" << endl;
    cout << "[1]: Get Records" << endl;
    cout << "[2]: Register Item" << endl;
    cout << "[3]: Update/Delete Item" << endl << endl;
    cout << "Answer: ";
}

void ExitOrMenu(bool &g) {
    cout << endl << "[a]: Exit" << endl;
    cout << "[b]: Main Menu" << endl;
    char tempans;
    cout << "Answer: ";
    cin >> tempans;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (tempans != 'b') {
        g = false;
    } 
	else {
        g = true;
    }
}

int main() {
    MYSQL* obj;
    MYSQL_ROW row;
    MYSQL_RES* res;

    int ID;
    char Item[100];
    int Quantity;
    float Price;
    bool g = true;
    int ans;
    int qstate;

    // Connection
    if (!(obj = mysql_init(0))) {
        cout << "ERROR: MySQL object could not be created." << endl;
        return 1;
    } 
	else if (!mysql_real_connect(obj, host, user, pass, "Grocery", 3306, NULL, 0)) {
        cout << "ERROR: Database info wrong." << endl;
        cout << mysql_error(obj) << endl;
        return 1;
    }
    cout << "WELCOME TO GROCERY STORE MANAGEMENT SYSTEM" << endl << endl;
    while (g) {
        showMainMenu();
        cin >> ans;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  
        if (ans == 0) {
            break;
        } 
		else if (ans == 1) {
            cout << endl << "WELCOME TO GET RECORDS" << endl;
            int id;
            cout << endl << "Enter Item ID= ";
            cin >> id;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            stringstream ss;
            ss << id;
            string str_id = ss.str();
            string query = "SELECT * FROM manage WHERE ID = " + str_id;
            qstate = mysql_query(obj, query.c_str());
            if (!qstate) {
                res = mysql_store_result(obj);
                if (mysql_num_rows(res) == 0) {
                    cout << "No records found." << endl;
                } 
                else {
                    while ((row = mysql_fetch_row(res))) {
                        cout << endl;
                        cout << "ID : " << row[0] << endl;
                        cout << "Item : " << row[1] << endl;
                        cout << "Quantity : " << row[2] << endl;
                        cout << "Price : " << row[3] << endl;
                    }
                }
                mysql_free_result(res);
            } 
			else {
                cout << "Query Execution Problem! " << mysql_error(obj) << endl;
            }
            ExitOrMenu(g);
            cout << endl;
        } 
		else if (ans == 2) {
            cout << endl << "WELCOME TO ITEM REGISTRATION" << endl<<endl;
            cout << "ID: ";
            cin >> ID;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Item: ";
            cin.getline(Item, 100);

            cout << "Quantity: ";
            cin >> Quantity;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Price: ";
            cin >> Price;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << endl;

            stringstream ss;
            ss << "INSERT INTO manage(ID, Item, Quantity, Price) VALUES(" << ID << ", '" << Item << "', " << Quantity << ", " << Price << ")";
            string query = ss.str();

            if (mysql_ping(obj)) {
                cout << "ERROR: Impossible to connect." << endl;
                cout << mysql_error(obj) << endl;
            } 
			else if (mysql_query(obj, query.c_str())) {
                cout << "ERROR " << mysql_error(obj) << endl;
                rewind(stdin);
                getchar();
            } 
			else {
                cout << "Info added successfully." << endl;
            }
            mysql_store_result(obj);
            ExitOrMenu(g);
        } 
		else if(ans == 3) {
            cout << endl << "WELCOME TO UPDATE/DELETE ITEMS" << endl<<endl;            
            cout << "[1]: Update Quantity" << "\n";
            cout << "[2]: Update Price" << "\n";
            cout<< "[3]: Delete Item"<<"\n";
            int ans;
            cout<<"Answer: ";
            cin >> ans;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout<<endl;
            if (ans == 1) {
            	cout<<"WELCOME TO UPDATE QUANTITY"<<endl<<endl;
                cout << "Enter Item ID: ";
                cin >> ID;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << endl;
                cout << "Enter new Quantity: ";
                int newQuantity;
                cin >> newQuantity;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                stringstream ss;
                ss << "UPDATE manage SET Quantity = " << newQuantity << " WHERE ID = " << ID;
                string query = ss.str();
                if (mysql_query(obj, query.c_str())) {
                    cout << "ERROR: Wrong Info" << mysql_error(obj) << endl;
                } 
				else {
                    cout << "Quantity updated successfully." << endl;
                }
            } 
			else if (ans == 2) {
				cout<<"WELCOME TO UPDATE PRICE"<<endl<<endl;
                cout << "Enter Item ID: ";
                cin >> ID;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << endl;
                cout << "Enter new Price: ";
                float newPrice;
                cin >> newPrice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                stringstream ss;
                ss << "UPDATE manage SET Price = " << newPrice << " WHERE ID = " << ID;
                string query = ss.str();
                if (mysql_query(obj, query.c_str())) {
                    cout << "ERROR: Wrong Info" << mysql_error(obj) << endl;
                } 
				else {
                    cout << "Price updated successfully." << endl;
                }
            } 
            else if(ans==3){
            	cout<<"WELCOME TO DELETE ITEMS !"<<endl<<endl;
                cout << "Enter Item ID: ";
                cin >> ID;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << endl;
                stringstream ss;
                ss<< "DELETE FROM manage WHERE ID ="<<ID;
                string query= ss.str();
                if (mysql_query(obj, query.c_str())) {
                    cout << "ERROR: Wrong Info" << mysql_error(obj) << endl;
                } 
				else {
                    cout << "Item Deleted successfully." << endl;
		  	    }
	     	}
			else {
                cout << "ERROR: Invalid Input" << endl;
            }
            ExitOrMenu(g);
            cout << endl;
        }   
		else {
            cout << "ERROR: Invalid Input" << endl;
        }
    }
    mysql_close(obj);
    return 0;
}


