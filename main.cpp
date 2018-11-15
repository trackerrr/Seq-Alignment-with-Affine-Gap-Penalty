//
//  main.cpp
//  CS482A1
//
//  Xiangyuan Zeng
//  20608260
//

#include <iostream>
#include <vector>
#include <string>
using namespace std;

int match = 1;
int mis_match = -1;
int gapopen = -5;
int gapext = -1;

int max_3(int x, int y, int z) {
    if (x > y) {
        if (x > z) {
            return x;
        }
        else {
            return z;
        }
    }
    else {
        if (y > z) {
            return y;
        }
        else {
            return z;
        }
    }
}

struct Dmatrix {
    int **D0;
    int **D1;
    int **D2;
    // vector<vector<int> > D0, D1, D2;
    int len1, len2;
    string seq1, seq2;
    string ans1 = "";
    string ans2 = "";
    int score;
   
    Dmatrix(string s1, string s2) {
        this->len1 = s1.length();
        this->len2 = s2.length();
        this->seq1 = s1;
        this->seq2 = s2;
        
        // first row
        int *starter_row;
        starter_row= new int [len1 + 1];
       // vector<int> starter_row;
        starter_row[0] = 0;
        int indel_count = gapopen + gapext;
        for (int j = 1; j <= len1; j++) {
            starter_row[j] = indel_count;
            // starter_row.push_back(indel_count);
            indel_count += gapext;
        }
        D0 = new int *[len2 + 1];
        D1 = new int *[len2 + 1];
        D2 = new int *[len2 + 1];
        
        D0[0] = starter_row;
        D1[0] = starter_row;
        D2[0] = starter_row;
        
        // D0.push_back(starter_row);
        // D1.push_back(starter_row);
        // D2.push_back(starter_row);
        
        // first column
        indel_count = gapopen + gapext;
        for (int i = 1; i <= len2; i++) {
            int *row_D0;
            int *row_D1;
            int *row_D2;
            row_D0 = new int [len1 + 1];
            row_D1 = new int [len1 + 1];
            row_D2 = new int [len1 + 1];
            row_D0[0] = indel_count;
            row_D1[0] = indel_count;
            row_D2[0] = indel_count;
            // cout << row_D0[4] << endl;
            // vector<int> row;
            // row.push_back(indel_count);
            indel_count += gapext;
            
            D0[i] = row_D0;
            D1[i] = row_D1;
            D2[i] = row_D2;
            
        }
    }
    ~Dmatrix() {
        delete D0;
        delete D1;
        delete D2;
    }
    void BuildMatrix () {
        for (int i = 1; i <= len2; i++) {
            for (int j = 1; j <= len1; j++) {
                
                // build D0
                int max_val = max_3(D0[i-1][j-1], D1[i-1][j-1], D2[i-1][j-1]);
                if (seq2[i-1] == seq1[j-1]) {
                    // D0[i].push_back(match + max_val);
                    D0[i][j] = match + max_val;
                }
                else {
                    // D0[i].push_back(mis_match + max_val);
                    D0[i][j] = mis_match + max_val;
                }
                
                // build D1
                max_val = max_3(D0[i][j-1] + gapopen, D1[i][j-1], D2[i][j-1] + gapopen);
                // D1[i].push_back(gapext + max_val);
                D1[i][j] = gapext + max_val;
                
                // build D2
                max_val = max_3(D0[i-1][j] + gapopen, D1[i-1][j] + gapopen, D2[i-1][j]);
                // D2[i].push_back(gapext + max_val);
                D2[i][j] = gapext + max_val;
            }
        }
    }
    
    void backtrack() {
        int i = len2;
        int j = len1;
        score = max_3(D0[i][j], D1[i][j], D2[i][j]);
        
        // D0 -- 0, D1 -- 1, D2 -- 2
        int which = 0;
        if (score == D0[i][j]) {
            which = 0;
        }
        else if (score == D1[i][j]) {
            which = 1;
        }
        else {
            which = 2;
        }
        
        while (j > 0) {
            if (i == 0) {
                break;
            }
            // in D0
            if ( which == 0 ) {
                // which function?
                int f;
                if (seq2[i-1] == seq1[j-1]) {
                    f = match;
                }
                else {
                    f = mis_match;
                }
                
                // to D0
                if (D0[i-1][i-1] + f == D0[i][j]) {
                    which = 0;
                }
                // to D1
                else if (D1[i-1][j-1] + f == D0[i][j]) {
                    which = 1;
                }
                // to D2
                else if (D2[i-1][j-1] + f == D0[i][j]) {
                    which = 2;
                }
                ans1 = seq1[j-1] + ans1;
                ans2 = seq2[i-1] + ans2;
                i--;
                j--;
            }
            
            // in D1
            else if ( which == 1 ) {
                // to D0
                if (D0[i][j-1] + gapopen + gapext == D1[i][j]) {
                    which = 0;
                }
                // to D1
                else if (D1[i][j-1] + gapext == D1[i][j]) {
                    which = 1;
                }
                // to D2
                else if (D2[i][j-1] + gapopen + gapext == D1[i][j]) {
                    which = 1;
                }
                ans1 = seq1[j-1] + ans1;
                ans2 = '-' + ans2;
                j--;
            }
            // in D2
            else {
                // to D0
                if (D0[i-1][j] + gapopen + gapext == D2[i][j]) {
                    which = 0;
                }
                // to D1
                else if (D1[i-1][j] + gapopen + gapext == D2[i][j]) {
                    which = 1;
                }
                // to D2
                else if (D2[i-1][j] + gapext == D2[i][j]) {
                    which = 2;
                }
                ans1 = '-' + ans1;
                ans2 = seq2[i-1] + ans2;
                i--;
            }
        }
        if (i == 0) {
            while (j > 0) {
                ans1 = seq1[j-1] + ans1;
                ans2 = '-' + ans2;
                j--;
            }
        }
        while (i > 0) {
            ans1 = '-' + ans1;
            ans2 = seq2[i-1] + ans2;
            i--;
        }
    }

    // print out 3 matrices
    void print(){
        /*
        cout << "D0:"<< endl;
        for (int i = 0; i <= len2; i++) {
            for (int j = 0; j <= len1; j++) {
                cout << D0[i][j] << " ";
            }
            cout << endl;
        }
        cout << "D1:"<< endl;
        for (int i = 0; i <= len2; i++) {
            for (int j = 0; j <= len1; j++) {
                cout << D1[i][j] << " ";
            }
            cout << endl;
        }
        cout << "D2:"<< endl;
        for (int i = 0; i <= len2; i++) {
            for (int j = 0; j <= len1; j++) {
                cout << D2[i][j] << " ";
            }
            cout << endl;
        } */
        
        cout << score << endl;
        cout << ans1 << endl;
        cout << ans2 << endl;
    }
};

int main(int argc, const char * argv[]) {
    
    string seq1;
    string seq2;
    getline(cin, seq1);
    getline(cin, seq2);
    
    Dmatrix SeqCom(seq1,seq2);
    SeqCom.BuildMatrix();
    SeqCom.backtrack();
    SeqCom.print();
    
    
}
