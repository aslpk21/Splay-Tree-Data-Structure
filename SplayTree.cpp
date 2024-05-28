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

Node* newNode(int key) {
    Node* node = new Node();
    node->key = key;
    node->left = node->right = nullptr;
    return node;
}

// Thực hiện quay phải X ( con trái của P )
Node* rightRotate(Node* P) {
    Node* X = P->left;
    P->left = X->right;
    X->right = P;
    return X;
}

// Thực hiện quay trái X ( con phải của P )
Node* leftRotate(Node* P) {
    Node* X = P->right;
    P->right = X->left;
    X->left = P;
    return X;
}


// Splaying function
/*
Trường hợp cơ sở: Gốc là rỗng, return về root
Bước 1: Splay khóa key, nếu key tồn tại, key trở thành gốc, nếu không nút gần key nhất sẽ trở thành gốc
Bước 2: Kiểm tra nút gốc, nếu gốc là khóa key, không làm gì thêm
Bước 3: Nếu key chưa tồn tại:
- Nếu root.key > key:
    - Nếu root.left == NULL, trả về root
    - Nếu root.left.key > key (Zig-Zig - Left Left):
        - Đệ quy gọi Splay trên con trái của con trái của root
        - Quay phải root
    - Nếu root.left.key < key (Zig-Zag - Left Right):
        - Đệ quy gọi Splay trên con phải của con trái của root
        - Nếu con phải của con trái không là NULL, quay trái con trái của root
    - Nếu con trái của root là NULL, trả về root, nếu không quay phải root
- Nếu root.key < key:
    - Nếu root.right == NULL, trả về root
    - Nếu root.right.key > key (Zig-Zag - Right Left):
        - Đệ quy gọi Splay trên con trái của con phải của root
        - Nếu con trái của con phải không là NULL, quay phải con phải của root
    - Nếu root.right.key < key (Zig-Zig - Right Right):
        - Đệ quy gọi Splay trên con phải của con phải của root
        - Quay trái root
    - Nếu con phải của root là NULL, trả về root, nếu không quay trái root
*/
Node* Splay(Node* root, int key) {
    // Trường hợp cơ sở: root là NULL hoặc khóa của root bằng key thì trả về root
    if (root == nullptr || root->key == key)
        return root;

    // Nếu khóa của root lớn hơn key
    if (root->key > key) {
        // Nếu con trái của root là null, trả về root
        if (root->left == nullptr)
            return root;


        //Zig-Zig Situation (Left Left)
        if (root->left->key > key) {
            // Đệ quy gọi Splay trên con trái của con trái của root
            root->left->left = Splay(root->left->left, key);
            // Thực hiện phép quay phải trên root
            root = rightRotate(root);
        }

        // Zig-Zag Situation (Left Right)
        else if (root->left->key < key) {
            // Đệ quy gọi Splay trên con phải của con trái của root
            root->left->right = Splay(root->left->right, key);
            // Nếu con phải của con trái không là null, thực hiện phép quay trái trên con trái của root
            if (root->left->right != nullptr)
                root->left = leftRotate(root->left);
        }
        // Nếu con trái của root là null, trả về root, ngược lại thực hiện quay phải và trả về kết quả
        return (root->left == nullptr) ? root : rightRotate(root);
        
    } 

    // Nếu khóa của root nhỏ hơn key
    else {
        // Nếu con phải của root là null, trả về root
        if (root->right == nullptr)
            return root;

        // Zig-Zag Situation (Right Left)
        if (root->right->key > key) {
            // Đệ quy gọi Splay trên con trái của con phải của root
            root->right->left = Splay(root->right->left, key);
            // Nếu con trái của con phải không là null, thực hiện phép quay phải trên con phải của root
            if (root->right->left != nullptr)
                root->right = rightRotate(root->right);
        }

        // Zig-Zig Situation (Right Right)
        else if (root->right->key < key) {
            // Đệ quy gọi Splay trên con phải của con phải của root
            root->right->right = Splay(root->right->right, key);
            // Thực hiện phép quay trái trên root
            root = leftRotate(root);
        }
        // Nếu con phải của root là null, trả về root, ngược lại thực hiện quay trái và trả về kết quả
        return (root->right == nullptr) ? root : leftRotate(root);
    }
}


// Insertion function 
/*
Trường hợp cơ sở: Gốc là rỗng, cấp phát 1 nút mới và return về nút đó
Bước 1: Splay khóa key, nếu key tồn tại, key trở thành gốc, nếu không nút gần key nhất sẽ trở thành gốc
Bước 2: Kiểm tra nút gốc, nếu gốc là khóa key, không làm gì thêm
Bước 3: Nếu key chưa tồn tại: 
- Tạo 1 nút mới với khóa key
- Nếu key < root, gán root thành con phải của key, sao chép con trái của root làm con trái của key và gán con trái của root = NULL 
- Nếu key > root, gán root thành con trái của key, sao chép con phải của root làm con phải của key và gán con phải của root = NULL 
*/
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
/*
Trường hợp cơ sở: Gốc là rỗng, return về nút gốc
Bước 1: Splay khóa key, nếu key không tồn tại, Splay khóa có giá trị gần key nhất
Bước 2: Kiểm tra nút gốc, nếu nút gốc != key, return về nút gốc vì key không tồn tại
Bước 3: 
- Chia cây thành 2 cây Tree1 và Tree2 ứng với cây con bên trái và cây con bên phải của nút gốc
- Nếu Root1 = NULL, return về Root2
- Nếu Root1 != NULL, Splay nút lớn nhất của Tree1 và gán Root2 thành con phải của Root1 và return về Root1
*/
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
// Hàm này sẽ kiểm tra xem nút gốc có phải là key không, nếu có thì key được tìm thấy, return true và return false nếu ngược lại
bool search(Node*& root, int key) {
    root = Splay(root, key);
    return root && root->key == key;
}


// The functions from this line to line 370 are additional functions for displaying the tree.
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
            cout << "Invalid option!\n\n";
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
                cout << "Found " << x << "\n\n";
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
