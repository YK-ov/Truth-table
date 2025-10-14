#include <iostream>

using namespace std;

//comment

void printCharArr(char arr[], int size){
    for (int i = 0; i < size; i++){
        cout << arr[i] << " ";
    }
    cout << endl;

}

int main(){
    char predicatesInput[100];
    char exitInput;
    int counter = 0;

    for (int i = 0; i < 100; i++){
        cout << "type '0' for exit" << endl;
        cin >> exitInput;
        cout << "type the predicates: " << endl;
        cin >> predicatesInput[i];
        if (predicatesInput[i] != ' '){
            counter++;
        }


        if (exitInput == '0'){
            cout << "in exit";
            break;
        }
    }
    cout << counter << "=counter" << endl;

    int predicatesSize = counter + 1;

    predicatesInput[predicatesSize] = '\0';

    char* predicates = new char[predicatesSize];

    for (int i = 0; i < predicatesSize; i++){
        predicates[i] = predicatesInput[i];
        if (i == predicatesSize - 1){
            predicates[i] = '\0';
        }
    }

    printCharArr(predicates, predicatesSize);



    delete[] predicates;

    return 0;
}
