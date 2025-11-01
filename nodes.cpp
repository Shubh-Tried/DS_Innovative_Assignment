#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

struct Tick{
    string symbol;
    double price;
    long time;

    Tick(string s, double p,long t):symbol(s), price(p), time(t){}
};

struct ShareState{
    string symbol;
    string shareName;
    double currentPrice;

    double startPrice;
    double endPrice;
    double percentageChange;

    ShareState(string sy, string s, double sp=NAN){
        shareName = s;
        symbol = sy;
        startPrice = currentPrice = sp;
    }
};

class Analyzer{
    vector<vector<Tick>> HashTable[26];
    public:
    int firstHash(string s){
        return int(s[0]-'A');
    }
    void addTick(Tick t){
        int n = firstHash(t.symbol);
        HashTable[n];
    }
};

int main(){
    Tick t = {"SHU", double(0.0), 1};
    string s1 = "Hello";
    string s2 = "World";
    cout<<int(s1>s2);
}