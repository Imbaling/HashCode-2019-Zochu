#include <bits/stdc++.h>

using namespace std;

inline long long gettime() {
	return chrono::time_point_cast<chrono::milliseconds>(chrono::system_clock::now()).time_since_epoch().count();
}

#define SA_DEBUG

template<class Sol, class Mut> void SimulatedAnnealing(Sol &solution, double maxt, double mint, double finishattime_ms) {
    //synchronization means calculating current temperature from current work time
    static const int ITERS_PER_SYNC = 4096;       //must be power of two
    //sometimes we dump current progress to stderr
    static const int ITERS_PER_DUMP = (1 << 20);  //must be power of two
    //type of solution score
    typedef double ScoreType;
 
    //number of iterations done so far
    unsigned iters = 0;
    //part of the full cooling schedule passed
    double done = 0.0;
    //best solution so far
    Sol best = solution;
    //number of accepted mutations overall
    unsigned accmuts = 0;
 
    //time when the SA begins
    long long starttime = gettime();
    //time dedicated for SA processing
    double hastime = finishattime_ms;

    mt19937 randomGen;
    uniform_real_distribution<> dis(0, 1);
 
    for (; ; iters++) {
        //dump stats so that you can watch the progress of SA
	#ifdef SA_DEBUG
        if (!(iters & (ITERS_PER_DUMP - 1)))
            fprintf(stderr, "Iteration:%6d  Acc:%6d  Temp:%7.3lf  Score:%0.5lf Best Score:%0.5lf\n"
            , iters, accmuts, maxt * pow(mint/maxt, done), solution.GetScore(), best.GetScore());
	#endif
        //synchronize the temperature with time
        if (!(iters & (ITERS_PER_SYNC - 1))) {
            done = (double)(gettime() - starttime) / hastime;
            if (done >= 1.0) break;
        }
 
        //create mutation for current solution
        Mut mut;
        mut.Init(solution);
        //get the score delta of the mutation
        ScoreType delta = mut.GetDelta();
 
        //if mutated solution is better, accept it
        bool move = false;
        if (delta <= 0) move = true;
        else {
            //otherwise calculate current temperature
            double temp = maxt * pow(mint/maxt, done);
            //and accept with the tricky probability
            move = dis(randomGen) < exp(-delta / temp);
        }
 
        //if mutation is accepted, apply it to the solution
        if (move) {
            accmuts++;
            mut.Apply(solution);
        }
 
        //do not forget to store the best solution
        if (solution.GetScore() < best.GetScore()) best = solution;
    }
 
    //return the best solution as the result
    #ifdef SA_DEBUG
    fprintf(stderr, "Simulated annealing made %d iterations (accepted: %d)\n", iters, accmuts);
    #endif
    solution = best;
}

int** old = new int*[100];

void OL(int** T, int x, int y, int rozm) {
	for (int i = 0; i < rozm; i++) {
		for (int j = 0; j < rozm; j++) old[i][j] = T[x + i][y + j];
	}
	for (int i = 0; i < rozm; i++) {
		for (int j = 0; j < rozm; j++) T[x + i][y + j] = old[j][rozm - 1 - i];
	}
}

int** T3;

struct Solution {
    int** T1;
    int** T2;
    vector<pair<pair<int, int>, int>> ops;
    int rozm;
    double score;
 
    void CalcScore() {
        //this function is not used in SA
	for (int i = 0; i < rozm; i++) {
		for (int j = 0; j < rozm; j++) {
			T3[i][j] = T1[i][j];
		}
	}
	score = (double)ops.size();

	for (auto op : ops) {
		OL(T3, op.first.first, op.first.second, op.second);
	}

	int ileNieZg = 0;
	for (int i = 0; i < rozm; i++) {
		for (int j = 0; j < rozm; j++) {
			if (T3[i][j] != T2[i][j]) ileNieZg++;
		}
	}

	score += ileNieZg * rozm * rozm;
	
    }
    double GetScore() { return score; }
};
 
struct Mutation {
    double delta;
    int tryb; // 0 - dodanie, 1 - usuniecie
    int poz;
    pair<pair<int, int>, int> dodane;
    Solution newSol;
    void Init(Solution &s) {
        if (s.ops.size() == 0) {
		tryb = 0;
	} else tryb = rand() % 4;
	newSol.T1 = s.T1; newSol.T2 = s.T2;
	newSol.ops = s.ops; newSol.rozm = s.rozm;
	if (tryb == 0) {
		int ile = rand() % 10 + 1;
		for (int i = 0; i < ile; i++) {
			poz = rand() % (s.ops.size() + 1);
			int X = rand() % (s.rozm - 1), Y = rand() % (s.rozm - 1);
			int maksRozm = min(s.rozm - X, s.rozm - Y);
			dodane = make_pair(make_pair(X, Y), rand() % maksRozm + 1);
			newSol.ops.insert(newSol.ops.begin() + poz, dodane);
		}
	} else if (tryb == 1) {
		int ile = rand() % min(10, (int)s.ops.size()) + 1;
		for (int i = 0; i < ile; i++) {
			poz = rand() % (s.ops.size());
			newSol.ops.erase(newSol.ops.begin() + poz);
		}
	} else if (tryb == 2) {
		int ile = rand() % 10 + 1;
		for (int i = 0; i < ile; i++) {
			poz = rand() % (s.ops.size());
			int X = rand() % (s.rozm - 1), Y = rand() % (s.rozm - 1);
			int maksRozm = min(s.rozm - X, s.rozm - Y);
			dodane = make_pair(make_pair(X, Y), rand() % maksRozm + 1);
			newSol.ops[poz] = dodane;
		}
	} else {
		int ile = rand() % 10 + 1;
		for (int i = 0; i < ile; i++) {
			poz = rand() % (s.ops.size());
			int pX = (rand() % 2) * 2 - 1, pY = (rand() % 2) * 2 - 1;
			if (newSol.ops[poz].first.first + pX >= 0 && newSol.ops[poz].first.first + pX < s.rozm 
				&& newSol.ops[poz].first.second + pY >= 0 && newSol.ops[poz].first.second + pY < s.rozm
				&& newSol.ops[poz].first.first + pX + newSol.ops[poz].second - 1 < s.rozm
				&& newSol.ops[poz].first.second + pY + newSol.ops[poz].second - 1 < s.rozm) {
				newSol.ops[poz].first.first += pX;
				newSol.ops[poz].first.second += pY;
			}
		}
	}
	newSol.CalcScore();
	delta = (newSol.GetScore() - s.GetScore());
    }
    double GetDelta() { return delta; }
    void Apply(Solution &s) {
        s.ops = newSol.ops;
	s.score = newSol.score;
    }
};

int** A = new int*[6];
int** B = new int*[6];

int main() {
	T3 = new int*[100];
	for (int i = 0; i < 100; i++) {
		T3[i] = new int[100];
		old[i] = new int[100];
	}
	srand(time(NULL));
	for (int i = 0; i < 6; i++) { A[i] = new int[6]; B[i] = new int[6]; }
	A[0][0] = 2; A[0][1] = 11; A[0][2] = 0; A[0][3] = 0; A[0][4] = 0; A[0][5] = 0;  
	A[1][0] = 1; A[1][1] = 1; A[1][2] = 1; A[1][3] = 1; A[1][4] = 1; A[1][5] = 1;  
	A[2][0] = 2; A[2][1] = 2; A[2][2] = 2; A[2][3] = 5; A[2][4] = 2; A[2][5] = 2;  
	A[3][0] = 3; A[3][1] = 3; A[3][2] = 3; A[3][3] = 3; A[3][4] = 3; A[3][5] = 3;  
	A[4][0] = 4; A[4][1] = 5; A[4][2] = 4; A[4][3] = 4; A[4][4] = 4; A[4][5] = 4;  
	A[5][0] = 5; A[5][1] = 5; A[5][2] = 5; A[5][3] = 5; A[5][4] = 5; A[5][5] = 5;  

	B[0][0] = 0; B[0][1] = 0; B[0][2] = 0; B[0][3] = 0; B[0][4] = 0; B[0][5] = 0;  
	B[1][0] = 1; B[1][1] = 1; B[1][2] = 2; B[1][3] = 3; B[1][4] = 4; B[1][5] = 5;  
	B[2][0] = 5; B[2][1] = 1; B[2][2] = 5; B[2][3] = 3; B[2][4] = 4; B[2][5] = 5;  
	B[3][0] = 3; B[3][1] = 1; B[3][2] = 2; B[3][3] = 3; B[3][4] = 4; B[3][5] = 5;  
	B[4][0] = 4; B[4][1] = 1; B[4][2] = 2; B[4][3] = 3; B[4][4] = 4; B[4][5] = 5;  
	B[5][0] = 5; B[5][1] = 1; B[5][2] = 2; B[5][3] = 3; B[5][4] = 4; B[5][5] = 11;  

	OL(A, 1, 1, 4);  OL(B, 2, 2, 3); OL(A, 0, 0, 3); OL(A, 2, 4, 2); OL(A, 1, 1, 4);  OL(B, 2, 2, 3);
	OL(A, 2, 4, 2); OL(A, 3, 1, 2);  OL(B, 2, 2, 3); OL(A, 1, 1, 3);  OL(B, 2, 2, 2); OL(A, 0, 0, 3);

	Solution initialSol;
	initialSol.T1 = A; initialSol.T2 = B;
	initialSol.rozm = 6; initialSol.CalcScore();
	cout << "BRUTAL: " << initialSol.GetScore() << endl;
	SimulatedAnnealing<Solution, Mutation>(initialSol, 100.0, 0.001, 10.0 * 1000.0);
	cout << "ANNEAL: " << initialSol.GetScore() << endl;
	cout << "ile ops: " << initialSol.ops.size() << endl;
	return 0;
}
