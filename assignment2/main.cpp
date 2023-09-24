#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Node class for the BST
class Node {
    friend class AVL;

   private:
    int key;      // Value to be stored at the node
    int height;   // Height of the node
    Node *left;   // Pointer to the left child
    Node *right;  // Pointer to the right child

   public:
    // Default constructor
    Node() {
        key = 0;     // Assign key as 0
        height = 0;  // Assign height as 0 since the first node is the root at level 0

        // Assign left and right as nullptr as nodes are inserted as leaves, which do not have any children.
        left = nullptr;
        right = nullptr;
    };

    // Constructor with key parameter
    Node(int k) {
        key = k;     // Assign key as k
        height = 0;  // Assign height as 0 since the first node is the root at level 0

        // Assign left and right as nullptr as nodes are inserted as leaves, which do not have any children.
        left = nullptr;
        right = nullptr;
    };

    // Destructor
    ~Node(){};
};

// AVL tree class
class AVL {
   private:
    Node *root;  // Pointer to the root node

    // Helper of balance to find the height of a subtree
    int height(Node *currNode) {
        int leftHeight = currNode->height, rightHeight = currNode->height;  // Initialize left and right heights as 0

        // If left is not empty, find the height of the left subtree
        if (currNode->left != nullptr) {
            leftHeight = height(currNode->left);
        }

        // If right is not empty, find the height of the right subtree
        if (currNode->right != nullptr) {
            rightHeight = height(currNode->right);
        }

        // Return the height of the subtree
        return max(leftHeight, rightHeight);
    }

    // Helper of balance to adjust the heights of the nodes
    Node *adjustHeights(Node *currNode, int level) {
        if (currNode->left != nullptr) {
            currNode->left = adjustHeights(currNode->left, level + 1);
        }
        if (currNode->right != nullptr) {
            currNode->right = adjustHeights(currNode->right, level + 1);
        }

        currNode->height = level;

        return currNode;
    }

    Node *rotateRight(Node *currNode) {
        Node *temp = currNode->left->right;

        currNode->left->right = currNode;
        currNode = currNode->left;
        currNode->right->left = temp;

        return currNode;
    }

    Node *rotateLeft(Node *currNode) {
        Node *temp = currNode->right->left;

        currNode->right->left = currNode;
        currNode = currNode->right;
        currNode->left->right = temp;

        return currNode;
    }

    // Check if tree is balanced, balance if necessary
    Node *balance(Node *currNode) {
        // If the tree is empty, return nullptr
        if (currNode == nullptr) {
            return nullptr;
        }

        // If the tree is not empty, check if the tree is balanced
        else {
            // If the tree is not balanced, balance it
            int leftHeight = 0, rightHeight = 0, leftleftHeight = 0, leftRightHeight = 0, rightLeftHeight = 0, rightRightHeight = 0;
            if (currNode->left != nullptr) {
                leftHeight = height(currNode->left);
            }
            if (currNode->right != nullptr) {
                rightHeight = height(currNode->right);
            }
            if (abs(leftHeight - rightHeight) > 1) {
                // If the left subtree is taller than the right subtree
                if (leftHeight > rightHeight) {
                    // If the left subtree's left subtree is taller than the left subtree's right subtree
                    if (currNode->left->left != nullptr) {
                        leftleftHeight = height(currNode->left->left);
                    }
                    if (currNode->left->right != nullptr) {
                        leftRightHeight = height(currNode->left->right);
                    }
                    if (leftleftHeight > leftRightHeight) {  // Right Right Case
                        currNode = rotateRight(currNode);
                    }

                    // If the left subtree's right subtree is taller than the left subtree's left subtree
                    else if (leftleftHeight < leftRightHeight) {  // Left Right Case
                        currNode->left = rotateLeft(currNode->left);
                        currNode = rotateRight(currNode);
                    }
                }

                // If the right subtree is taller than the left subtree
                else if (leftHeight < rightHeight) {
                    // If the right subtree's right subtree is taller than the right subtree's left subtree
                    if (currNode->right->right != nullptr) {
                        rightRightHeight = height(currNode->right->right);
                    }
                    if (currNode->right->left != nullptr) {
                        rightLeftHeight = height(currNode->right->left);
                    }
                    if (rightRightHeight > rightLeftHeight) {
                        // Rotate left
                        // currNode = rotateLeft(currNode);
                    }

                    // If the right subtree's left subtree is taller than the right subtree's right subtree
                    else if (rightRightHeight < rightLeftHeight) {
                        // Rotate right, then rotate left
                        // currNode->right = rotateRight(currNode->right);
                        // currNode = rotateLeft(currNode);
                    }
                }
            }
        }

        // Return the current node
        return currNode;
    };

    // Insertion helper function. Used to check if the node should be inserted to the left or right of the current node.
    void BSTinsert(Node *currNode, int k, int level) {
        if (k < currNode->key) {               // If k < currNode
            if (currNode->left == nullptr) {   // and left child is empty,
                currNode->left = new Node(k);  // insert here.
                currNode = currNode->left;
                // Update the height of the current node
                currNode->height = level;
            } else {
                BSTinsert(currNode->left, k, level + 1);  // Otherwise, recursive call on the left child.
            }
        } else if (k > currNode->key) {         // If k > currNode
            if (currNode->right == nullptr) {   // and right child is empty,
                currNode->right = new Node(k);  // insert here.
                currNode = currNode->right;
                // Update the height of the current node
                currNode->height = level;
            } else {
                BSTinsert(currNode->right, k, level + 1);  // Otherwise, recursive call on the right child.
            }
        } else {
            return;  // If k is already in the tree, do nothing.
        }

        return;
    };

    // Deletion helper function, used to find the node to be deleted
    Node *BSTremove(Node *currNode, int k) {
        if (currNode == nullptr) {
            return nullptr;
        }

        if (k < currNode->key) {                                            // If k < currNode
            currNode->left = BSTremove(currNode->left, k);                  // Recursive call on the left child.
        } else if (k > currNode->key) {                                     // If k > currNode
            currNode->right = BSTremove(currNode->right, k);                // Recursive call on the right child.
        } else {                                                            // If k == currNode
            if (currNode->left == nullptr && currNode->right == nullptr) {  // If currNode is a leaf
                delete currNode;
                currNode = nullptr;                  // Set currNode to nullptr
            } else if (currNode->left == nullptr) {  // If currNode has only a right child
                Node *temp = currNode;               // Set temp to currNode
                currNode = currNode->right;          // Set currNode to its right child
                delete temp;
            } else if (currNode->right == nullptr) {  // If currNode has only a left child
                Node *temp = currNode;                // Set temp to currNode
                currNode = currNode->left;            // Set currNode to its left child
                delete temp;
            } else {                          // If currNode has two children, pick the left child to replace currNode
                Node *temp = currNode->left;  // Set temp to currNode's left child

                // Find the rightmost node of the left subtree
                while (temp->right != nullptr) {
                    temp = temp->right;
                }

                // Replace currNode's key with the rightmost node of the left subtree's key
                currNode->key = temp->key;

                // Delete the rightmost node of the left subtree
                currNode->left = BSTremove(currNode->left, temp->key);
            }
        }

        return currNode;
    }

    // Preorder output
    void preOut(Node *currNode) {
        if (currNode == nullptr) {
            return;
        }

        cout << currNode->key << " ";
        preOut(currNode->left);
        preOut(currNode->right);

        return;
    };

    // Inorder output
    void inOut(Node *currNode) {
        if (currNode == nullptr) {
            return;
        }

        inOut(currNode->left);
        cout << currNode->key << " ";

        inOut(currNode->right);

        return;
    };

    // Postorder output
    void postOut(Node *currNode) {
        if (currNode == nullptr) {
            return;
        }

        postOut(currNode->left);
        postOut(currNode->right);
        cout << currNode->key << " ";

        return;
    };

   public:
    // Default constructor
    AVL() {
        root = nullptr;  // Assign root as nullptr
    };

    // Insertion function
    void insert(int k) {
        // If the tree is empty, insert the node as the root
        if (root == nullptr) {
            root = new Node(k);
            return;
        }

        // If the tree is not empty, insert the node as a leaf
        else {
            BSTinsert(root, k, root->height + 1);
        }

        // Now that the node is inserted, check if the tree is balanced, balance if necessary
        root = balance(root);

        // Adjust the heights of the nodes
        root = adjustHeights(root, 0);

        return;
    };

    // Deletion function
    void remove(int k) {
        // If the tree is empty, do nothing
        if (root == nullptr) {
            return;
        }

        // If the tree is not empty, find the key and delete if found
        else {
            root = BSTremove(root, k);
        }

        // Now that the node is deleted, check if the tree is balanced, balance if necessary
        root = balance(root);

        // Adjust the heights of the nodes
        root = adjustHeights(root, 0);

        return;
    };

    // Output functions
    void preOut() {
        if (root == nullptr) {
            cout << "EMPTY" << endl;
        } else {
            preOut(root);
        }
    };

    void inOut() {
        if (root == nullptr) {
            cout << "EMPTY" << endl;
        } else {
            inOut(root);
        }
    };

    void postOut() {
        if (root == nullptr) {
            cout << "EMPTY" << endl;
        } else {
            postOut(root);
        }
    };

    // Destructor, using postorder traversal removal of nodes
    ~AVL(){};
};

// Split input into vector of instructions
vector<string> separateInput(string input) {
    vector<string> instructions;
    stringstream ss(input);
    string instruction;

    while (ss >> instruction) {
        instructions.push_back(instruction);
    }

    return instructions;
}

// Print Vector function
template <class T>
void printVector(vector<T> v) {
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
}

int main() {
    string input;
    getline(cin, input);  // Get input from user as a string

    // Separate input into instructions by spaces
    vector<string> instructions = separateInput(input);
    // printVector(instructions);  // Print instructions

    // Create AVL tree
    AVL tree;

    // Execute instructions for insertion or deletion
    for (size_t i = 0; i < instructions.size() - 1; i++) {
        if (instructions[i][0] == 'A') {
            int key = stoi(instructions[i].substr(1, instructions[i].size() - 1));
            tree.insert(key);
        } else if (instructions[i][0] == 'D') {
            int key = stoi(instructions[i].substr(1, instructions[i].size() - 1));
            tree.remove(key);
        }
    }

    if (instructions[instructions.size() - 1] == "PRE") {
        // If the last instruction is PRE, print the tree in preorder
        tree.preOut();
    } else if (instructions[instructions.size() - 1] == "IN") {
        // If the last instruction is IN, print the tree in inorder
        tree.inOut();
    } else if (instructions[instructions.size() - 1] == "POST") {
        // If the last instruction is POST, print the tree in postorder
        tree.postOut();
    }

    return 0;
}