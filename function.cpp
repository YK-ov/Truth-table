#include <iostream>
#include <cmath>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

struct ParseResult{

};

class Calculate{
private:
    struct Token {
      string type;
      char value;
    };

    enum NodeType{
      TRUE_NODE,
      FALSE_NODE,
      VARIABLE,
      NEGATE,
      AND_OP,
      OR_OP,
      IMPLIES_OP,
      IFF_OP
    };

    struct ASTNode {
        NodeType type;
        int varIndex;
        ASTNode* left = nullptr;
        ASTNode* right = nullptr;

        ASTNode(NodeType t, int idx = -1){
            type = t;
            varIndex = idx;
            left = nullptr;
            right = nullptr;
        }

        ASTNode(NodeType t, ASTNode* l, ASTNode* r = nullptr){
            type = t;
            varIndex = -1;
            left = l;
            right = r;
        }

        bool evaluate(const vector<bool>& assignment){
            if (type == TRUE_NODE){
                return true;
            }

            else if (type == FALSE_NODE){
                return false;
            }

            else if (type == VARIABLE){
                return assignment[varIndex];
            }

            else if (type == NEGATE){
                return !left->evaluate(assignment);
            }

            else if (type == AND_OP){
                return left->evaluate(assignment) && right->evaluate(assignment);
            }

            else if (type == OR_OP){
                return left->evaluate(assignment) || right->evaluate(assignment);
            }

            else if (type == IMPLIES_OP){
                return !left->evaluate(assignment) || right->evaluate(assignment);
            }

            else if (type == IFF_OP){
                return left->evaluate(assignment) == right->evaluate(assignment);
            }

            return false;
        }

        string toString(const vector<string>& vars){
            if (type == TRUE_NODE){
                return "T";
            }

            else if (type == FALSE_NODE){
                return "F";
            }

            else if (type == VARIABLE){
                return vars[varIndex];
            }

            else if (type == NEGATE){
                return "~" + left->toString(vars);
            }

            else if (type == AND_OP){
                return "(" + left->toString(vars) + " & " + right->toString(vars) + ")";
            }

            else if (type == OR_OP){
                return "(" + left->toString(vars) + " | " + right->toString(vars) + ")";
            }

            else if (type == IMPLIES_OP){
                return "(" + left->toString(vars) + " -> " + right->toString(vars) + ")";
            }

            else if (type == IFF_OP){
                return "(" + left->toString(vars) + " <-> " + right->toString(vars) + ")";
            }

            return "";

        }

    };


public:
    string input;
    vector<pair<char, int>> variables;
    vector<char> operatorsVector;
    vector<Token> tokens;
    vector<string> variableNames;

    int variableCounter;

    Calculate(string in){
        input = in;

        variableCounter = 0;
    }

    struct ParseResult{
        Calculate::ASTNode* ast;
        vector<string> variables;
    };

    ParseResult parse(){
        stack<Token> operators;
        stack<ASTNode*> operands;

        bool needOperand = true;

        for (int i = 0; i < tokens.size(); i++){
            Token currentToken = tokens[i];

            if (needOperand){

            if (currentToken.type == "variable"){
                //operands.push(currentToken.value);
                int index = -1;
                string varName(1, currentToken.value);
                for (int j = 0; j < variableNames.size(); j++){
                    if (variableNames[j] == varName){
                        index = j;
                    }
                }

                if (index == -1){
                    index = variableNames.size();
                    variableNames.push_back(varName);
                }

                ASTNode* varNode = new ASTNode(NodeType::VARIABLE, index);

                operands.push(varNode);
/*
                if (find(variableNames.begin(), variableNames.end(), string(1, currentToken.value)) == variableNames.end()){
                    variableNames.push_back(string(1, currentToken.value));
                }

                variableCounter++;*/

                needOperand = false;
            }

            else if (currentToken.type == "(" || currentToken.type == "~"){
                operators.push(currentToken);

            }

            else if (currentToken.type == "$"){
                if (operators.size() == 0){
                    cout << "Input was empty\n";

                    return {nullptr, {}};
                }

                if (operators.top().type == "("){
                    cout << "Open parenthesis has no matching close parenthesis\n";

                    return {nullptr, {}};
                }

                cout << "THis operator is missing an operand\n";

                return {nullptr, {}};
            }

            else {
                cout << "Variable or open parenthesis was expected here\n";
            }

            }

            else {
                if (isOperator(currentToken) || currentToken.type == "$"){
                    while (true){
                        if (currentToken.type == "$"){
                            cout << " breaked\n";
                            break;
                        }

                        if (operators.size() == 0){
                            break;
                        }

                        if (operators.top().type == "("){
                            break;
                        }

                        if (getPriority(operators.top()) >= getPriority(currentToken)){
                            break;
                        }

                        Token operatorToken = operators.top();
                        operators.pop();

                        if (operands.size() < 2){
                            break;
                        }

                        ASTNode* rhs = operands.top();
                        operands.pop();

                        ASTNode* lhs = operands.top();
                        operands.pop();

                        // Token opToken{operatorVal, '\0'};
                        // ASTNode* result = createOperatorNode(lhs, opToken, rhs);

                        //operands.push(result);

                        addOperand(createOperatorNode(lhs, operatorToken, rhs), operands, operators);

                    }

                    // operators.push(currentToken);
                    //
                    // cout << "Error: Operator '" << currentToken.type << " after while loop\n";
                    //
                    // needOperand = true;

                    if (currentToken.type == "$"){
                        break;
                    }


                    operators.push(currentToken);

                    //cout << "Error: Operator '" << currentToken.type << " after while loop\n";

                    needOperand = true;


                }

                else if (currentToken.type == ")"){
                    while (true){
                        if (operators.size() == 0){
                            cout << "No matching parenthesis found\n";

                            return {nullptr, {}};
                        }

                        Token currentOperation = operators.top();

                        operators.pop();

                        if (currentOperation.type == "("){
                            break;
                        }

                        if (currentOperation.type == "~"){
                            cout << "Nothing is negated by this operator\n";

                            return {nullptr, {}};
                        }

                        ASTNode* rhs = operands.top();
                        operands.pop();

                        ASTNode* lhs = operands.top();
                        operands.pop();

                        addOperand(createOperatorNode(lhs, currentOperation, rhs), operands, operators);

                    }

                    ASTNode* expression = operands.top();
                    operands.pop();

                    addOperand(expression, operands, operators);
                }

                else {
                    cout << "Close parenthesis or operator was expected here\n";

                    return {nullptr, {}};
                }

            }

        }

        if (operators.size() != 0){
            // for (int i = 0; i < operators.size(); i++){
            //     cout << operators[i] << " ";
            // }
            Token leftoverOp = operators.top();
            cout << "Error: Operator '" << leftoverOp.type << "' left on stack\n";

            cout << "Error in input\n";

            return {nullptr, {}};
        }

        return {
            operands.top(),
            variableNames
        };
    }

    ASTNode* createOperatorNode(ASTNode* lhs, Token token, ASTNode* rhs){
        if (token.type == "<->"){
            // ASTNode* iffNode = new ASTNode(NodeType::IFF_OP, lhs, rhs);
            //
            // return iffNode;

            ASTNode* iffNode = new ASTNode(NodeType::IFF_OP, lhs, rhs);

            return iffNode;
        }

        if (token.type == "->"){
            ASTNode* impliesNode = new ASTNode(NodeType::IMPLIES_OP, lhs, rhs);

            return impliesNode;
        }

        if (token.type == "OR"){
            ASTNode* orNode = new ASTNode(NodeType::OR_OP, lhs, rhs);

            return orNode;
        }

        if (token.type == "AND"){
            ASTNode* andNode = new ASTNode(NodeType::AND_OP, lhs, rhs);

            return andNode;
        }

        return nullptr;
    }


    void addOperand(ASTNode* node, stack<ASTNode*>& operands, stack<Token>& operators){
        while (operators.size() > 0 && operators.top().type == "~"){
            operators.pop();

            node = new ASTNode(NodeType::NEGATE, node);
        }

        operands.push(node);

    }

    bool isOperator(Token token){
        if (token.type == "<->" || token.type == "->" || token.type == "AND" || token.type == "OR"){
            return true;
        }
        else {
            return false;
        }
    }

    void check(){
        input = input + '$';

        for (int i = 0; i < input.size(); i++){
            if (input[i] == '$'){
                tokens.push_back({"$", '\0'});

                break;
            }

            else if ((input[i] >= 'A' && input [i] <= 'Z') || (input[i] >= 'a' && input[i] <= 'z')){
                tokens.push_back({"variable", input[i]});
            }

            else if (input[i] == '<' && input[i+1] == '-' && input[i+2] == '>'){
                tokens.push_back({"<->", '\0'});

                i = i + 2;

                //continue;
            }

            else if (input[i] == '-' && input[i+1] == '>'){
                tokens.push_back({"->", '\0'});

                i = i + 1;

                //continue;
            }

            else if (input[i] == 'v'){
                tokens.push_back({"OR", '\0'});
            }

            else if (input[i] == '&'){
                tokens.push_back({"AND", '\0'});
            }

            else if (input [i] == '('){
                tokens.push_back({"(", '\0'});
            }

            else if (input[i] == ')'){
                tokens.push_back({")", '\0'});
            }

            else if (input[i] == '~'){
                tokens.push_back({"~", '\0'});
            }

            else if (input[i] == ' '){
                continue;
            }

            else {
                cout << "Wrong input\n";

                return;
            }
        }

    }

    int getPriority(Token token){
            if (token.type == "<->"){
                return 0;
            }

            if (token.type == "->"){
                return 1;
            }

            if (token.type == "OR"){
                return 2;
            }

            if (token.type == "AND"){
                return 3;
            }

            if (token.type == "$"){
                return -1;
            }

            return 0;

    }

    // int getPriority(string stringInput){
    //         if (stringInput == "<->"){
    //             return 0;
    //         }
    //         if (stringInput == "->"){
    //             return 1;
    //         }
    //         if (stringInput == "OR"){
    //             return 2;
    //         }
    //         if (stringInput == "AND"){
    //             return 3;
    //         }
    //
    //         return 0;
    // }

    void createTruthTable(ParseResult parseRes){
        vector<bool> assignment(parseRes.variables.size(), false);

        do {
            for (int i = 0; i < assignment.size(); i++){
                if (assignment[i] == true){
                    cout << "T" << " ";
                }

                else {
                    cout << "F" << " ";
                }
            }

            bool result = parseRes.ast->evaluate(assignment);

            if (result == true){
                cout << "| " << " T" << "\n";
            }

            else {
                cout << "| " << "F" << "\n";
            }

        }

        while (nextAssigment(assignment));

    }

    bool nextAssigment(vector<bool>& assignment){
            int flipIndex = assignment.size() - 1;

            while (flipIndex >= 0 && assignment[flipIndex]){
                flipIndex--;
            }

            if (flipIndex == -1){
                return false;
            }

            assignment[flipIndex] = true;

            for (int i = flipIndex + 1; i < assignment.size(); i++){
                assignment[i] = false;
            }

            return true;
    }

    void print(){
        for (int i = 0; i < tokens.size(); i++){
            cout << tokens[i].type << " " << tokens[i].value << " ";
        }
        cout << "\n";
    }







};



int main(){
    string predicatesInput;

    getline(cin, predicatesInput);

    Calculate calc(predicatesInput);

    calc.check();

    calc.print();

    Calculate::ParseResult result = calc.parse();

    calc.createTruthTable(result);


    return 0;
}
