#include <iostream>
#include <algorithm>
#include <set>
#include <vector>

#include "z3++.h"
#include <random>

using namespace std;
using namespace z3;

struct obrazek {
    int id;
    char orient;
    vector<string> tagi;
};

struct obrazek obrazki[100000 + 955];

struct grupka {
    struct obrazek o1;
    struct obrazek o2;
};

long long get_result(vector<struct grupka>& grupki) {
    long long res = 0;

    for (int i = 1; i < grupki.size(); i++) {
        set<string> elem1;
        for (auto& elem : grupki[i - 1].o1.tagi) elem1.insert(elem);
        if (grupki[i - 1].o1.orient == 'V') {
            for (auto& elem : grupki[i - 1].o2.tagi) elem1.insert(elem);
        }

        set<string> elem2;
        for (auto& elem : grupki[i].o1.tagi) elem2.insert(elem);
        if (grupki[i].o1.orient == 'V') {
            for (auto& elem : grupki[i].o2.tagi) elem2.insert(elem);
        }

        set<string> wspol;
        wspol.insert(elem1.begin(), elem1.end());
        wspol.insert(elem2.begin(), elem2.end());

        res += min(wspol.size(), min(elem1.size() - wspol.size(), elem2.size() - wspol.size()));
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    vector<struct obrazek> obrazkiPion;
    vector<struct obrazek> obrazkiPoziom;
    for (int i = 0; i < n; i++) {
        obrazki[i].id = i;
        cin >> obrazki[i].orient;
        int cnt;
        cin >> cnt;
        while (cnt--) {
            string tag;
            cin >> tag;
            obrazki[i].tagi.push_back(tag);
        }
        std::sort(obrazki[i].tagi.begin(), obrazki[i].tagi.end());
        if (obrazki[i].orient == 'V') {
            obrazkiPion.push_back(obrazki[i]);
        } else {
            obrazkiPoziom.push_back(obrazki[i]);
        }
    }
    vector<struct grupka> grupki;
    for (int i = 1; i < obrazkiPion.size(); i += 2) {
        struct grupka g;
        g.o1 = obrazkiPion[i - 1];
        g.o2 = obrazkiPion[i];
        grupki.push_back(g);
    }
    for (int i = 0; i < obrazkiPoziom.size(); i++) {
        struct grupka g;
        g.o1 = obrazkiPoziom[i];
        grupki.push_back(g);
    }
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(grupki.begin(), grupki.end(), g);

    vector<struct grupka> bestGrupki = grupki;
    long long bestScore = get_result(bestGrupki);

    std::uniform_int_distribution<> dis(0, bestGrupki.size() - 1);

    for (int iter = 0; iter < 100; iter++) {
        auto noweGrupki = bestGrupki;
        int pos1 = dis(g);
        int pos2 = dis(g);
        swap(noweGrupki[pos1], noweGrupki[pos2]);
        auto nowyScore = get_result(noweGrupki);
        if (nowyScore > bestScore) {
            bestScore = nowyScore;
            bestGrupki = noweGrupki;
        }
    }

    cout << bestGrupki.size() << endl;
    for (auto& grupka : bestGrupki) {
        if (grupka.o1.orient == 'V') {
            cout << grupka.o1.id << " " << grupka.o2.id << endl;
        } else cout << grupka.o1.id << endl;
    }
    return 0;
}
