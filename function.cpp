#include <iostream>
#include <cmath>

using namespace std;

class Calculate{
public:
    string input;
    int variableCounter;
    bool isRightInput;
    int** resultMatrix;


    Calculate(string in){
        input = in;
        isRightInput = true;

        variableCounter = 0;
    }

    void parse(){
        for (int i = 0; i < input.size(); i++){
            if (input[i] != '~' && input[i] && input[i] != 'v' && input[i] != '&'){
                if ((input[i] >= 'A' && input[i] <= 'Z') || (input[i] >= 'a' && input[i] <= 'z')){
                    variableCounter++;
                }
                else {
                    cout << "Wrong input\n";
                    isRightInput = false;

                    return;
                }
            }
        }
    }

    void createMatrix(){
        if (!isRightInput){
            resultMatrix = nullptr;

            return;
        }

        variableCounter = pow(2, variableCounter);

        resultMatrix = new int*[variableCounter];

        //int resultMatrix[variableCounter][variableCounter];

        for (int i = 0; i < variableCounter; i++){

            resultMatrix[i] = new int[variableCounter];

            for (int j = 0; j < variableCounter; j++){
                resultMatrix[i][j] = 0;
            }
        }

        print();


    }

    void print(){
        for (int i = 0; i < variableCounter; i++){
            for (int j = 0; j < variableCounter; j++){
                cout << resultMatrix[i][j];
            }
            cout << "\n";
        }
    }

    ~Calculate(){
        if (resultMatrix != nullptr){
        for (int i = 0; i < variableCounter; i++){
            delete[] resultMatrix[i];
        }
        }

        delete[] resultMatrix;

    }




};



int main(){
    string predicatesInput;

    cin >> predicatesInput;

    Calculate calc(predicatesInput);

    calc.createMatrix();




    return 0;
}
