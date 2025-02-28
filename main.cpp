#include <algorithm>
#include <limits.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <list>
#include <map>
#include <sstream>
#include <iomanip>

using namespace std;


struct Transaction {
    int id;
    string credit_card;
    string currency;
    float amount;
    bool processed;
    string type;
};

struct MinMaxAvg {
    Transaction min;
    Transaction max;
    int avg;
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

void displayTable(const Transaction &transaction) {
    cout << left << setw(10) << transaction.id
            << setw(25) << transaction.credit_card
            << setw(20) << transaction.currency
            << setw(15) << transaction.type
            << setw(10) << fixed << setprecision(2) << transaction.amount
            << endl;
}

void display(const vector<Transaction> &transactions) {
    cout << left << setw(10) << "ID"
            << setw(25) << "Credit card number"
            << setw(20) << "Currency"
            << setw(15) << "Type"
            << setw(10) << "Amount"
            << endl;

    for (const Transaction &transaction: transactions) {
        displayTable(transaction);
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
    transaction.type = temp;
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

int findTransaction(const vector<Transaction> &transactions, const string &credit_card) {
    for (int i = 0; i < transactions.size(); i++) {
        if (transactions[i].credit_card == credit_card) {
            return i;
        }
    }
    return -1;
}

map<string, int> countCurrencyTransactions(const vector<Transaction> &transactions) {
    map<string, int> currencyCounts;
    for (const Transaction &transaction: transactions) {
        currencyCounts[transaction.currency]++;
    }
    return currencyCounts;
}

vector<Transaction> getTransactionsByCurrency(const vector<Transaction> &transactions, const string &currency) {
    vector<Transaction> result;
    for (const Transaction &transaction: transactions) {
        if (transaction.currency == currency) {
            result.push_back(transaction);
        }
    }
    return result;
}

MinMaxAvg getTransactionsByType(const vector<Transaction> &transactions) {
    MinMaxAvg result;
    double min_sum = INT_MAX;
    double max_sum = 0;
    double total_sum = 0;

    for (const Transaction &transaction: transactions) {
        total_sum += transaction.amount;

        if (transaction.amount < min_sum) {
            min_sum = transaction.amount;
            result.min = transaction;
        }

        if (transaction.amount > max_sum) {
            max_sum = transaction.amount;
            result.max = transaction;
        }
    }
    result.avg = (int) (total_sum / transactions.size());
    return result;
}

list<Transaction> findTransactionsByType(const vector<Transaction> &transactions, const string &card) {
    list<Transaction> result;
    vector<Transaction>::const_iterator iter;

    for (iter = transactions.cbegin(); iter != transactions.cend(); ++iter) {
        const Transaction &transaction = *iter;
        if (transaction.credit_card.find(card) != std::string::npos) {
            //  C++23 >> transaction.credit_card.contains(card)
            result.push_back(transaction);
        }
    }
    return result;
}

vector<Transaction> sortByAmount(vector<Transaction> transactions) {
    auto func = [](Transaction a, Transaction b) { return a.amount > b.amount; };
    sort(transactions.begin(), transactions.end(), func);
    return transactions;
}

void displayOptions() {
    cout << "-------------------------------------------------" << endl;
    cout << "Choose an option:" << endl;
    cout << "1. Display all transactions" << endl;
    cout << "2. Find transaction by credit card" << endl;
    cout << "3. Count transactions by currency" << endl;
    cout << "4. Get transactions by currency" << endl;
    cout << "5. Get transactions by type" << endl;
    cout << "6. Find transactions by type" << endl;
    cout << "7. Sort transactions by amount" << endl;
    cout << "8. Exit" << endl;
    cout << "-------------------------------------------------" << endl;
}

int main() {
    string file_name = "data.csv";
    vector<Transaction> transactions;
    loadData(transactions);
    bool exit = false;
    int index;
    map<string, int> currencyCounts;

    do {
        displayOptions();
        int option;
        cin >> option;

        switch (option) {
            case 1:
                display(transactions);
                break;
            case 2: {
                string credit_card;
                cout << "Enter credit card: ";
                cin >> credit_card;
                index = findTransaction(transactions, credit_card);
                if (index != -1) {
                    display(transactions[index]);
                } else {
                    cout << "Transaction not found" << endl;
                }
                break;
            }
            case 3:
                currencyCounts = countCurrencyTransactions(transactions);
                for (const auto &currency_count: currencyCounts) {
                    cout << currency_count.first << ": " << currency_count.second << endl;
                }
                break;
            case 4: {
                string currency;
                cout << "Enter currency: (EUR, USD, GBP)";
                cin >> currency;
                vector<Transaction> card_transactions = getTransactionsByCurrency(transactions, currency);
                display(card_transactions);
                break;
            }
            case 5: {
                MinMaxAvg minMaxAvg = getTransactionsByType(transactions);
                cout << "Min transaction: " << endl;
                display(minMaxAvg.min);
                cout << "Max transaction: " << endl;
                display(minMaxAvg.max);
                cout << "Average transaction value: " << minMaxAvg.avg << endl;
                break;
            }
            case 6: {
                string card;
                cout << "Enter card: ";
                cin >> card;
                list<Transaction> list = findTransactionsByType(transactions, card);
                for (const Transaction &transaction: list) {
                    display(transaction);
                }
                break;
            }
            case 7: {
                vector<Transaction> sorted_transactions = sortByAmount(transactions);
                display(sorted_transactions);
                break;
            }
            case 8:
                exit = true;
                break;
            default:
                cout << "Invalid option" << endl;
                break;
        }
    } while (!exit);


    return 0;
}
