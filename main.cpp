#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;


struct Transaction {
    int id;
    string credit_card;
    string currency;
    float amount;
    bool processed;
    string type;
};

void display(const Transaction &transaction) {
    cout << "ID: " << transaction.id << "\t\t";
    cout << "Credit Card: " << transaction.credit_card << "\t\t";
    cout << "Currency: " << transaction.currency << "\t\t";
    cout << "Amount: " << transaction.amount << "\t\t";
    cout << "Processed: " << transaction.processed << "\t\t";
    cout << "Type: " << transaction.type << "\t\t";
    cout << endl;
}

void display(const vector<Transaction> &transactions) {
    for (const Transaction &transaction: transactions) {
        display(transaction);
    }
}

void parseLine(const string &line, Transaction &transaction) {
    string temp;
    stringstream ss(line);

    getline(ss, temp, ',');
    transaction.id = stoi(temp);
    getline(ss, temp, ',');
    transaction.credit_card = temp;
    getline(ss, temp, ',');
    transaction.currency = temp;
    getline(ss, temp, ',');
    transaction.amount = stof(temp);
    getline(ss, temp, ',');
    transaction.processed = (temp == "true");
    getline(ss, temp, ',');
}


void loadData(vector<Transaction> &transactions) {
    ifstream fin("data.csv");
    if (fin) {
        string line;
        while (getline(fin, line)) {
            Transaction transaction;
            parseLine(line, transaction);
            transactions.push_back(transaction);
        }
        fin.close();
    } else {
        cout << "Error opening file: " << endl;
    }
}





int main() {
    string file_name = "data.csv";
    vector<Transaction> transactions;
    loadData(transactions);

    display(transactions);

    return 0;
}
