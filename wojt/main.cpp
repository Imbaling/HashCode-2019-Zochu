
#include <stdio.h>
#include <iostream>
#include <string>
#include <climits>
#include <cassert>
#include <functional>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <sstream>
#include <bitset>


#define REP(i,n) for(int (i) = 0; (i)<(n); (i)++)
#define FOR(i,a,b) for(int (i) = (a); (i) <= (b); (i)++)
#define FORD(i,a,b) for(int (i) = (a); (i) >= (b); (i)--)
#define PB push_back

using namespace std;



typedef vector<string> VS;
typedef vector<VS> VVS;
typedef vector<long> MinHash;
typedef vector<MinHash> Sig;
typedef pair<int,int> PII;

VVS slides;

const int MAXN = 1e5 + 5;
const int rows = 4;
const int bands = 10;
const int hashSize = rows * bands;
const int maxkraw = 1e6 * 5;


const double lowerb = 0.4;
const double upperb = 0.8;

map <PII, int> graph;
vector <int> jakiezdjecia[MAXN];
VS tagiwzdjeciu[MAXN];
Sig signature;
long long sumawag = 0;
bitset <MAXN> v;

vector<int> sas[MAXN];

vector<vector<int>> output;

void dfs(int x){
    v[x] = true;
    vector<int> zdj;
    zdj.clear();
    zdj = jakiezdjecia[x];
    output.PB(zdj);
    for(int w : sas[x]){
        if(!v[w]){
            dfs(w);
        }
    }
}


MinHash make_minhash(VS &input){
    MinHash ret(hashSize, LONG_MAX);

    for(string s : input){
        srand(std::hash<std::string>{}(s));
        REP(i, hashSize){
            long r = rand();
            ret[i] = std::min(ret[i], r);
        }
    }

    return ret;
}

void dorzuc_krawedz(int ix, int iy){

    int positions = 0;
    REP(i, hashSize){
        if(signature[ix][i] == signature[iy][i])
            positions++;
    }

    if(((double(positions)) / (double(hashSize))) < lowerb){
        return;
    }

    if(((double(positions)) / (double(hashSize))) > upperb){
        return;
    }

    VS &x = slides[ix];
    VS &y = slides[iy];

    sort(x.begin(), x.end());
    sort(y.begin(), y.end());

    VS diff1, diff2, inters;
    set_difference(x.begin(), x.end(), y.begin(), y.end(), inserter(diff1, diff1.begin()));
    set_difference(y.begin(), y.end(), x.begin(), x.end(), inserter(diff2, diff2.begin()));
    std::set_intersection(x.begin(), x.end(),
                          y.begin(), y.end(),
                          std::back_inserter(inters));

    int wynik = min(diff1.size(), diff2.size());
    wynik = min(wynik, (int)(inters.size()));

    PII krawedz = make_pair(min(ix,iy), max(ix,iy));
    if(graph.count(krawedz) > 0){
        wynik = max(wynik, graph[krawedz]);
        sumawag -= graph[krawedz];
    }
    graph[krawedz] = wynik;
    sumawag+=wynik;
}


int main() {

    int ilezdjec;
    cin>>ilezdjec;
    REP(i,ilezdjec){
        char znak;
        int ilet;
        scanf(" %c %d", &znak, &ilet);
        REP(p, ilet){
            string s;
            cin>>s;
            tagiwzdjeciu[i].PB(s);
        }
    }

    int liczbaslajdow;

    cin>>liczbaslajdow;
    string smiec;
    getline(cin, smiec);
    REP(i,liczbaslajdow){
        string line;
        getline(cin, line);
        istringstream is(line);
        int zdjecie;
        while(is >> zdjecie){
            jakiezdjecia[i].PB(zdjecie);
        }
    }

    cerr<<"wczytalem zdjecia w slajdach"<<endl;
    fflush(stderr);
    //popraw wczytywanie ponizej

    //int n; // ile slajdow w pliku
    //scanf(" %d", &n);
    int n = liczbaslajdow;

    slides.clear();

    REP(i,n){
        //int ile;
        //scanf(" %d", &ile);

        VS temp;
        temp.clear();
/*
        REP(p, ile){
            string s;
            cin>>s;
            temp.PB(s);
        }
*/
        if(jakiezdjecia[i].size() == 1){
            temp = tagiwzdjeciu[jakiezdjecia[i][0]];
        }
        else if (jakiezdjecia[i].size() == 2){
            sort(tagiwzdjeciu[jakiezdjecia[i][0]].begin(),
                 tagiwzdjeciu[jakiezdjecia[i][0]].end());
            sort(tagiwzdjeciu[jakiezdjecia[i][1]].begin(),
                 tagiwzdjeciu[jakiezdjecia[i][1]].end());
            set_union(tagiwzdjeciu[jakiezdjecia[i][0]].begin(),
                      tagiwzdjeciu[jakiezdjecia[i][0]].end(),
                      tagiwzdjeciu[jakiezdjecia[i][1]].begin(),
                      tagiwzdjeciu[jakiezdjecia[i][1]].end(),
                      back_inserter(temp));
        }
        else{
            assert(false);
        }
        slides.PB(temp);
    }
    cerr<<"wczytalem slajdy"<<endl;
    fflush(stderr);


    signature.clear();

    for(VS vs : slides){
        signature.PB(make_minhash(vs));
    }
    cerr<<"stworzylem sygnatury"<<endl;
    fflush(stdout);

    REP(b, bands){
        cerr<<"band "<<b<<"na "<<bands<<endl;
        fflush(stderr);
        std::unordered_map<long, vector<int>> mapa;
        mapa.clear();

        REP(i, signature.size()){
            long klucz = 0;
            FOR(p, b * rows, (b + 1) * rows - 1){
                klucz ^= signature[i][p];
            }
            if(mapa.count(klucz) == 0){
                mapa[klucz] = std::vector<int>();
            }
            mapa[klucz].PB(i);
        }

        for(auto const& para : mapa){
            if(graph.size() > maxkraw)
                break;

            for(int x : para.second){
                for(int y : para.second){
                    if(x != y){
                        dorzuc_krawedz(x,y);
                    }
                }
            }
        }
    }


    for(auto const& para : graph) {
        //cout << para.first.first << " " << para.first.second << " " << para.second << "\n";
        sas[para.first.first].PB(para.first.second);
        sas[para.first.second].PB(para.first.first);
    }


    cerr<<graph.size();
    cerr<<"\n srednia waga: "<<(double(sumawag))/(double(graph.size()));



    vector <int> kolejnosc;
    REP(i, slides.size()){
        kolejnosc.PB(i);
    }

    srand(30121997);
    random_shuffle(kolejnosc.begin(), kolejnosc.end());

    for(int x : kolejnosc){
        if(!v[x])
        dfs(x);
    }

    cout<<output.size()<<endl;
    for(vector<int> &x : output){
        for(int y : x){
            cout<<y<<" ";
        }
        cout<<endl;
    }
    return 0;
}
