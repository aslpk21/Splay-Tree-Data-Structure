#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Node {
    int key;
    Node *left, *right;
};

// Make a new node
Node* newNode(int key) {
    Node* node = new Node();
    node->key = key;
    node->left = node->right = nullptr;
    return node;
}

// Make a right rotation for X (the left child node of P)
Node* rightRotate(Node* P) {
    Node* X = P->left;
    P->left = X->right;
    X->right = P;
    return X;
}

// Make a left rotation for X (the right child node of P)
Node* leftRotate(Node* P) {
    Node* X = P->right;
    P->right = X->left;
    X->left = P;
    return X;
}




// Splaying function
Node* Splay(Node* root, int key) {
    if (root == nullptr || root->key == key)
        return root;

    if (root->key > key) {
        if (root->left == nullptr)
            return root;
        if (root->left->key > key) {
            root->left->left = Splay(root->left->left, key);
            root = rightRotate(root);
        }
        else if (root->left->key < key) {
            root->left->right = Splay(root->left->right, key);
            if (root->left->right != nullptr)
                root->left = leftRotate(root->left);
        }
        return (root->left == nullptr) ? root : rightRotate(root);
    } else {
        if (root->right == nullptr)
            return root;
        if (root->right->key > key) {
            root->right->left = Splay(root->right->left, key);
            if (root->right->left != nullptr)
                root->right = rightRotate(root->right);
        }
        else if (root->right->key < key) {
            root->right->right = Splay(root->right->right, key);
            root = leftRotate(root);
        }
        return (root->right == nullptr) ? root : leftRotate(root);
    }
}

// Insertion function 
void Insert(Node*& root, int key) {
    if (root == nullptr) {            
        root = newNode(key);
        return;
    }

    root = Splay(root, key);          

    if (root->key == key)              
        return;

    Node* Temp = newNode(key);        
    if (root->key > key) {            
        Temp->right = root;
        Temp->left = root->left;
        root->left = nullptr;
    } else {                          
        Temp->left = root;
        Temp->right = root->right;
        root->right = nullptr;
    }
    root = Temp;                      
}

// Deletion function
void Deletee(Node*& root, int key) {
    if (root == nullptr) return;

    root = Splay(root, key);

    if (root->key != key) return;

    if (root->left == nullptr) {
        Node* temp = root;
        root = root->right;
        delete temp;
    } else {
        Node* temp = root;
        root = Splay(root->left, key);
        root->right = temp->right;
        delete temp;
    }
}

// Searching function
// This function returns true if the key is found in the tree and moves it to the root.
bool search(Node*& root, int key) {
    root = Splay(root, key);
    return root && root->key == key;
}

// The functions from this line to line 301 are additional functions for displaying the tree.
// These functions were modified from Christopher Oicles's program from StackOverflow:
// https://stackoverflow.com/posts/36810117/revisions

struct cell_display {
    string valstr;
    bool present;
    cell_display() : present(false) {}
    cell_display(string valstr) : valstr(valstr), present(true) {}
};

using display_rows = vector< vector< cell_display > >;

int max_depth(Node* root) {
    if (!root) return 0;
    int left_depth = max_depth(root->left);
    int right_depth = max_depth(root->right);
    return max(left_depth, right_depth) + 1;
}

display_rows get_row_display(Node* root) {
    vector<Node*> traversal_stack;
    vector<vector<Node*>> rows;
    if (!root) return display_rows();

    Node* p = root;
    int depth = 0;
    int max_depth = ::max_depth(root);
    rows.resize(max_depth);

    while (true) {
        if (depth == max_depth - 1) {
            rows[depth].push_back(p);
            if (depth == 0) break;
            depth--;
            continue;
        }

        if (traversal_stack.size() == depth) {
            rows[depth].push_back(p);
            traversal_stack.push_back(p);
            p = p ? p->left : nullptr;
            depth++;
            continue;
        }

        if (rows[depth + 1].size() % 2) {
            p = traversal_stack.back();
            p = p ? p->right : nullptr;
            depth++;
            continue;
        }

        if (depth == 0) break;

        traversal_stack.pop_back();
        p = traversal_stack.back();
        depth--;
    }

    display_rows rows_disp;
    stringstream ss;
    for (const auto& row : rows) {
        rows_disp.emplace_back();
        for (Node* pn : row) {
            if (pn) {
                ss << pn->key;
                rows_disp.back().push_back(cell_display(ss.str()));
                ss = stringstream();
            } else {
                rows_disp.back().push_back(cell_display());
            }
        }
    }
    return rows_disp;
}

vector<string> row_formatter(const display_rows& rows_disp) {
    using s_t = string::size_type;
    s_t cell_width = 0;

    for (const auto& row_disp : rows_disp) {
        for (const auto& cd : row_disp) {
            if (cd.present && cd.valstr.length() > cell_width) {
                cell_width = cd.valstr.length();
            }
        }
    }

    if (cell_width % 2 == 0) ++cell_width;
    if (cell_width < 3) cell_width = 3;

    vector<string> formatted_rows;
    s_t row_count = rows_disp.size();
    s_t row_elem_count = 1 << (row_count - 1);
    s_t left_pad = 0;

    for (s_t r = 0; r < row_count; ++r) {
        const auto& cd_row = rows_disp[row_count - r - 1];
        s_t space = (s_t(1) << r) * (cell_width + 1) / 2 - 1;
        string row;

        for (s_t c = 0; c < row_elem_count; ++c) {
            row += string(c ? left_pad * 2 + 1 : left_pad, ' ');
            if (cd_row[c].present) {
                const string& valstr = cd_row[c].valstr;
                s_t long_padding = cell_width - valstr.length();
                s_t short_padding = long_padding / 2;
                long_padding -= short_padding;
                row += string(c % 2 ? short_padding : long_padding, ' ');
                row += valstr;
                row += string(c % 2 ? long_padding : short_padding, ' ');
            } else {
                row += string(cell_width, ' ');
            }
        }

        formatted_rows.push_back(row);
        if (row_elem_count == 1) break;

        s_t left_space = space + 1;
        s_t right_space = space - 1;
        for (s_t sr = 0; sr < space; ++sr) {
            string row;
            for (s_t c = 0; c < row_elem_count; ++c) {
                if (c % 2 == 0) {
                    row += string(c ? left_space * 2 + 1 : left_space, ' ');
                    row += cd_row[c].present ? '/' : ' ';
                    row += string(right_space + 1, ' ');
                } else {
                    row += string(right_space, ' ');
                    row += cd_row[c].present ? '\\' : ' ';
                }
            }
            formatted_rows.push_back(row);
            ++left_space;
            --right_space;
        }
        left_pad += space + 1;
        row_elem_count /= 2;
    }

    reverse(formatted_rows.begin(), formatted_rows.end());
    return formatted_rows;
}

void trim_rows_left(vector<string>& rows) {
    if (rows.empty()) return;
    auto min_space = rows.front().length();
    for (const auto& row : rows) {
        auto i = row.find_first_not_of(' ');
        if (i == string::npos) i = row.length();
        if (i == 0) return;
        if (i < min_space) min_space = i;
    }
    for (auto& row : rows) {
        row.erase(0, min_space);
    }
}

void displayTree(Node* root) {
    const int depth = max_depth(root);
    if (depth == 0) {
        cout << " <empty tree>\n";
        return;
    }

    const auto rows_disp = get_row_display(root);
    auto formatted_rows = row_formatter(rows_disp);
    trim_rows_left(formatted_rows);
    for (const auto& row : formatted_rows) {
        cout << ' ' << row << '\n';
    }
}

int main() {
    Node* root = nullptr;
    while (true) {
        cout << "---------------Options---------------" << "\n";
        cout << "1. Insert Node \n";
        cout << "2. Delete Node \n";
        cout << "3. Find Node \n";
        cout << "4. Display the current Tree \n";
        cout << "5. Generate a sample Tree \n";
        cout << "6. Exit \n";
        cout << "-------------------------------------" << " \n";
        cout << "Choose 1 option: ";
        int option;
        cin >> option; 
        cout << "\n\n\n";
        if( option < 1 || option > 6){
            cout << "Invalid option, pick again!\n\n";
            continue;
        }
        if (option == 1) {
            int x;
            cout << "Enter the value to insert: ";
            cin >> x;
            cout << "\n";
            Insert(root, x);
        } else if (option == 2) {
            int x;
            cout << "Enter the value to delete: ";
            cin >> x;
            cout << "\n";
            Deletee(root, x);
        } else if (option == 3) {
            int x;
            cout << "Enter the value to find: ";
            cin >> x;
            cout << "\n\n\n";
            if (search(root, x)) {
                cout << "Found! " << x << " is root right now" << "\n\n";
            } else {
                cout << "Key is not present in Tree!\n\n";
            }
        } else if (option == 4) {
            displayTree(root);
        }
        else if( option == 5){
            root = NULL;
            Insert(root, 4);
            Insert(root, 10);
            Insert(root, 5);
            Insert(root, 12);
            Insert(root, 2);
            Insert(root, 8);
            displayTree(root);
            cout << "\n\n\n";
        } 
        else {
            break;
        }
        cout << "\n\n\n\n";
    }
    cout << "\n\n\n\n";
    cout << "Display the current Splay tree:\n";
    displayTree(root);

    return 0;
}
