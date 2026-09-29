#include<iostream>


using namespace std;


int main() {
    int m = 3,n=4;
    int arr[m][n]={
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 9, 10, 11, 12 },
    };
    int arr2[n][m];

    for (int i =0 ;i<m;i++){
        for (int j=0;j<n;j++){
            arr2[j][m-1-i] = arr[i][j]; 
        }
    }

    // for (int i=0;i<3;i++){
    //     for (int j=0;j<4;j++){
    //         cout << arr[i][j]<<" ";
    //     }
    //     cout << "\n";
    // }
    for (int i=0;i<n;i++){
        for (int j=0;j<m;j++){
            cout << arr2[i][j]<<" ";
        }
        cout << "\n";
    }
}