#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <string>

using namespace std;

class BankDeposit {
private:
    string name;
    double amount;
    string currency;
    double rate;

public:
    BankDeposit() : name(""), amount(0.0), currency(""), rate(0.0) {}
    BankDeposit(const string& n, double a, const string& c, double r)
        : name(n), amount(a), currency(c), rate(r) {}

    const string& getName() const { return name; }
    double getAmount() const { return amount; }
    const string& getCurrency() const { return currency; }
    double getRate() const { return rate; }

    bool operator<(const BankDeposit& other) const {
        return name < other.name;
    }

    bool operator==(const BankDeposit& other) const {
        return name == other.name && 
               amount == other.amount &&
               currency == other.currency && 
               rate == other.rate;
    }

    friend ostream& operator<<(ostream& os, const BankDeposit& deposit) {
        os << "Deposit: " << deposit.name 
           << ", Amount: " << deposit.amount 
           << " " << deposit.currency 
           << ", Rate: " << deposit.rate << "%";
        return os;
    }
};

namespace std {
    template<>
    struct hash<BankDeposit> {
        size_t operator()(const BankDeposit& deposit) const {
            return hash<string>()(deposit.getName()) ^
                   hash<double>()(deposit.getAmount()) ^
                   hash<string>()(deposit.getCurrency()) ^
                   hash<double>()(deposit.getRate());
        }
    };
}

vector<BankDeposit> readFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }

    vector<BankDeposit> deposits;
    string name, currency;
    double amount, rate;

    while (file >> name >> amount >> currency >> rate) {
        deposits.emplace_back(name, amount, currency, rate);
    }

    file.close();
    return deposits;
}

template<typename Container>
void writeToFile(ofstream& file, const Container& container, const string& title) {
    file << title  << endl;
    for (const auto& deposit : container) {
        file << deposit << endl;
    }
    file << endl;
}

void processSets(const vector<BankDeposit>& deposits, ofstream& outFile) {
    set<BankDeposit> depositSet(deposits.begin(), deposits.end());
    unordered_set<BankDeposit> depositUnorderedSet(deposits.begin(), deposits.end());

    outFile << "Set Container" << endl;
    for (const auto& deposit : depositSet) {
        outFile << deposit << endl;
    }
    outFile << endl;

    outFile << "Unordered Set Container" << endl;
    for (const auto& deposit : depositUnorderedSet) {
        outFile << deposit << endl;
    }
    outFile << endl;
}

int main() {
    vector<BankDeposit> deposits = readFromFile("input.txt");
    ofstream outFile("output.txt");

    writeToFile(outFile, deposits, "Original Container (vector)");

    sort(deposits.begin(), deposits.end(), 
        [](const BankDeposit& a, const BankDeposit& b) {
            return a.getName() < b.getName();
        });
    writeToFile(outFile, deposits, "Sorted Container (vector)");

    deque<BankDeposit> depositDeque;
    copy(deposits.begin(), deposits.end(), back_inserter(depositDeque));
    writeToFile(outFile, depositDeque, "Copied Container (deque)");

    processSets(deposits, outFile);

    outFile.close();
    cout << "Results written to output.txt" << endl;

    return 0;
}
