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
        for (auto& polewej : elem1) {
            if (elem2.count(polewej) > 0) wspol.insert(polewej);
        }

        res += min(wspol.size(), min(elem1.size() - wspol.size(), elem2.size() - wspol.size()));
    }
    return res;
}

long long co_po_zmianie(vector<struct grupka>& grupki, int poz) {
    long long res = 0;

    if (poz > 0) {
        set<string> elem1;
        for (auto& elem : grupki[poz - 1].o1.tagi) elem1.insert(elem);
        if (grupki[poz - 1].o1.orient == 'V') {
            for (auto& elem : grupki[poz - 1].o2.tagi) elem1.insert(elem);
        }

        set<string> elem2;
        for (auto& elem : grupki[poz].o1.tagi) elem2.insert(elem);
        if (grupki[poz].o1.orient == 'V') {
            for (auto& elem : grupki[poz].o2.tagi) elem2.insert(elem);
        }

        set<string> wspol;
        for (auto& polewej : elem1) {
            if (elem2.count(polewej) > 0) wspol.insert(polewej);
        }

        res -= min(wspol.size(), min(elem1.size() - wspol.size(), elem2.size() - wspol.size()));
    }
    if (poz + 1 < grupki.size()) {
        set<string> elem1;
        for (auto& elem : grupki[poz + 1].o1.tagi) elem1.insert(elem);
        if (grupki[poz + 1].o1.orient == 'V') {
            for (auto& elem : grupki[poz + 1].o2.tagi) elem1.insert(elem);
        }

        set<string> elem2;
        for (auto& elem : grupki[poz].o1.tagi) elem2.insert(elem);
        if (grupki[poz].o1.orient == 'V') {
            for (auto& elem : grupki[poz].o2.tagi) elem2.insert(elem);
        }

        set<string> wspol;
        for (auto& polewej : elem1) {
            if (elem2.count(polewej) > 0) wspol.insert(polewej);
        }

        res -= min(wspol.size(), min(elem1.size() - wspol.size(), elem2.size() - wspol.size()));
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

    for (int iter = 0; iter < 250000 * 10; iter++) {
        int pos1 = dis(g);
        int pos2 = dis(g);
        if (abs(pos1 - pos2) <= 3) continue;
        auto jakiNowyWynik = bestScore + co_po_zmianie(bestGrupki, pos1) + co_po_zmianie(bestGrupki, pos2);
        swap(bestGrupki[pos1], bestGrupki[pos2]);
        jakiNowyWynik -= co_po_zmianie(bestGrupki, pos1) + co_po_zmianie(bestGrupki, pos2);
        if (jakiNowyWynik > bestScore) {
            // spoko
            bestScore = jakiNowyWynik;
        } else {
            // rollback
            swap(bestGrupki[pos1], bestGrupki[pos2]);
        }
    }
    std::cerr << bestScore << endl;

    cout << bestGrupki.size() << endl;
    for (auto& grupka : bestGrupki) {
        if (grupka.o1.orient == 'V') {
            cout << grupka.o1.id << " " << grupka.o2.id << endl;
        } else cout << grupka.o1.id << endl;
    }
    return 0;
}
