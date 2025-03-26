#include <iostream>
#include <stack>
#include <sstream>
#include <unordered_map>
#include <iomanip> // For number formatting

using namespace std;

// Node structure for the Binary Expression Tree
struct TreeNode {
    string value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(string val) : value(val), left(nullptr), right(nullptr) {}
};

// Binary Expression Tree Class
class BinaryExpressionTree {
private:
    TreeNode* root;

    // Recursively evaluates the tree
    double evaluate(TreeNode* node) {
        if (!node) return 0;

        // If node is an operand (leaf), return its numerical value
        if (!node->left && !node->right)
            return stod(node->value);

        // Recursively evaluate left and right subtrees
        double leftValue = evaluate(node->left);
        double rightValue = evaluate(node->right);

        // Perform the operation
        if (node->value == "+") return leftValue + rightValue;
        if (node->value == "-") return leftValue - rightValue;
        if (node->value == "*") return leftValue * rightValue;
        if (node->value == "/") return leftValue / rightValue;  // Assume no division by zero

        return 0;
    }

    // In-order traversal (infix expression with improved formatting)
    void inorder(TreeNode* node) {
        if (!node) return;
        if (node->left) cout << "("; // Add opening parenthesis
        inorder(node->left);
        cout << node->value; 
        inorder(node->right);
        if (node->right) cout << ")"; // Add closing parenthesis
    }

    // Post-order traversal (postfix expression)
    void postorder(TreeNode* node) {
        if (!node) return;
        postorder(node->left);
        postorder(node->right);
        cout << node->value << " ";
    }

public:
    // Constructor
    BinaryExpressionTree() : root(nullptr) {}

    // Builds the tree from a postfix expression
    void buildFromPostfix(const string& postfix, const unordered_map<string, double>& variables) {
        stack<TreeNode*> st;
        stringstream ss(postfix);
        string token;

        while (ss >> token) {
            // If token is a variable, replace it with its value
            if (variables.find(token) != variables.end())
                token = to_string(variables.at(token));

            TreeNode* newNode = new TreeNode(token);

            // If token is an operator, pop two operands and make them children
            if (token == "+" || token == "-" || token == "*" || token == "/") {
                newNode->right = st.top(); st.pop();
                newNode->left = st.top(); st.pop();
            }

            st.push(newNode);
        }

        // Root of the tree is the last remaining node
        if (!st.empty()) root = st.top();
    }

    // Evaluates the tree
    double evaluateTree() {
        return evaluate(root);
    }

    // Prints the infix expression
    void printInfix() {
        inorder(root);
        cout << endl;
    }

    // Prints the postfix expression
    void printPostfix() {
        postorder(root);
        cout << endl;
    }
};

// Main driver program
int main() {
    // Define variable values
    unordered_map<string, double> variables = {
        {"A", 5}, {"B", 3}, {"C", 8}, {"D", 6}, {"E", 2}
    };

    // Postfix expressions from the worksheet
    string expressions[] = {
        "A B +",         // 5 3 +
        "C E - B +",     // 8 2 - 3 +
        "A B C * +",     // 5 3 8 * +
        "D E / B +",     // 6 2 / 3 +
        "A C + B -",     // 5 8 + 3 -
        "A B + C *",     // 5 3 + 8 *
        "C E B * + D -", // 8 2 3 * + 6 -
        "A B C * + E /", // 5 3 8 * + 2 /
        "C E + B D * -", // 8 2 + 3 6 * -
        "A B + C E / -"  // 5 3 + 8 2 / -
    };

    // Process each expression
    for (const string& expr : expressions) {
        BinaryExpressionTree bet;
        bet.buildFromPostfix(expr, variables);

        cout << "Postfix Expression: " << expr << endl;
        cout << "Infix Expression: ";
        bet.printInfix();
        cout << "Evaluated Result: " << bet.evaluateTree() << endl;
        cout << "Postfix (Validation): ";
        bet.printPostfix();
        cout << "---------------------------\n";
    }

    return 0;
}