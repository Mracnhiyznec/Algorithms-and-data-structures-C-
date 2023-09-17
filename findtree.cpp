#include<bits/stdc++.h>
using namespace std;
#define debug(x) std::cerr << __FUNCTION__ << ":" << __LINE__ << " " << #x << " = " << x << '\n';
struct airplane
{
    int num_race;
    string point;
    string typeOfPlane;
    airplane()
    {
        num_race = 0;
        point = "-";
        typeOfPlane = "-";
    }

    airplane(int num_race, string point, string typeOfPlane){
        this->num_race = num_race;
        this->point = point;
        this->typeOfPlane = typeOfPlane;
    }
};
bool compare(airplane *value1, airplane *value2){
    if (value1->num_race < value2->num_race){
        return true;
    }
    else{
        return false;
    }
}
struct AVLnode
{
    airplane* value;
    AVLnode* left;
    AVLnode* right;
    int depth;
};
AVLnode *MAINROOT;
int depth(AVLnode* node)
{
    if (node == NULL)
        return 0;
    return node->depth;
}

airplane* newPlane(int race, string point1, string type)
{
    airplane *air = new airplane;
    air->num_race = race;
    air->point = point1;
    air->typeOfPlane = type;
    return air;
}

AVLnode* newNode(airplane* value)
{
    AVLnode* node = new AVLnode;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    node->depth = 1;
    return node;
}

AVLnode* insert(AVLnode* node, airplane* value){
    if (node == NULL){
        return newNode(value);
    }
    else if (compare(value, node->value)) {
        node->left = insert(node->left, value);
    }
    else if (!compare(value, node->value)) {

        node->right = insert(node->right, value);
    }
    node->depth = max(depth(node->left), depth(node->right)) + 1;

    return node;
}
void Delete(AVLnode* node, vector<AVLnode*> &toInsert){
    if (node == NULL){
        return;
    }
    toInsert.push_back(node);
    Delete(node->left, toInsert);
    Delete(node->right, toInsert);
}
bool is_equal(AVLnode *node, int num_race, string point, string typeOfPlane){
    return (node->value->num_race == num_race && node->value->point == point && node->value->typeOfPlane == typeOfPlane);

}
void findToDelete(int num_race, string point, string typeOfPlane, AVLnode *node, vector<pair<bool, pair<AVLnode*, AVLnode*>>> &toDelete, AVLnode *pred, bool flag){
    if (node == NULL){
        return;
    }
    if (is_equal(node, num_race, point, typeOfPlane)){
        toDelete.push_back({flag, {pred, node}});
    }
    findToDelete(num_race, point, typeOfPlane, node->left, toDelete, node, false);
    findToDelete(num_race, point, typeOfPlane, node->right, toDelete, node, true);

}
void find_planes(AVLnode* node, string point, vector<pair<int, string>> &data){
    if (node->value->point == point){
            data.push_back({node->value->num_race, node->value->typeOfPlane});
        }
    if (node->left != NULL){
        find_planes(node->left, point, data);
    }
    if (node->right != NULL){
        find_planes(node->right, point, data);
    }
    return;
}
void find_more_than_k(AVLnode* node, int k, map<string, int> &data){
    data[node->value->typeOfPlane]++;
    if (node->left != NULL){
        find_more_than_k(node->left, k, data);
    }
    if (node->right != NULL){
        find_more_than_k(node->right, k, data);
    }
    return;
}
void helper_function_for_find_more_than_k(AVLnode* node, int k, vector<string> &database){
    map<string, int> checker;
    find_more_than_k(node, k, checker);
    for (auto c: checker){
        if (c.second > k){
            database.push_back(c.first);
        }
    }
}
void show(AVLnode* node, int lvl) { 
    if (node->left != NULL) {
        show(node->left, lvl + 1);
    }
    for (int i = 0; i < lvl - 1; i++) {
        cout << "     ";
    }
    if (node->value->point != "#initializator"){
        cout << node->value->num_race << ' '  << node->value->point << ' ' << node->value->typeOfPlane << '\n';
    }
    if (node->right != NULL) {
        show(node->right, lvl + 1);
    }
}
void addPlane(int num_race, string point, string typeOfPlane, AVLnode *root){
    insert(root, newPlane(num_race, point, typeOfPlane));
}
void Deleteplane(int num_race, string point, string typeOfPlane, AVLnode *root){
    vector<pair<bool, pair<AVLnode*, AVLnode*>>> toDelete;
    findToDelete(num_race, point, typeOfPlane, MAINROOT->right, toDelete,MAINROOT, true);
    findToDelete(num_race, point, typeOfPlane, MAINROOT->left, toDelete, MAINROOT, false);
    if (toDelete.empty()){
        return;
    }
    AVLnode *bad_root = toDelete.back().second.second;
    vector<AVLnode*> toInsert;
    Delete(bad_root->left, toInsert);
    Delete(bad_root->right, toInsert);
    if (toDelete.back().first){
        toDelete.back().second.first->right = NULL;
    }
    else{
        toDelete.back().second.first->left = NULL;
    }
    for (auto c: toInsert){
        addPlane(c->value->num_race, c->value->point, c->value->typeOfPlane, root);
    }
}
void init(){
    MAINROOT = insert(MAINROOT, newPlane(0, "#initializator", "#initializator"));
}
void hellotext(){
    cout << "1. vivesti derevo na ekran\n";
    cout << "2. Vivesti na ekran tipi samoletov kototrie vipolnyaut bolee k razlichnih reycov\n";
    cout << "3. Dobavit samolet\n";
    cout << "4. Izmenit uzel dereva\n";
    cout << "5. Vivod na erkan nomerom reycov i tipov samoletov, viletausih v punkt naznacheya k\n";
    cout << "6. Udalit kakoy-to airplane\n";
    cout << "7. exit\n";
}
void changeNode(int num_race, string point, string typeOfPlane, int new_num_race, string new_point, string new_typeOfPlane){
    vector<pair<bool, pair<AVLnode*, AVLnode*>>> toDelete;
    findToDelete(num_race, point, typeOfPlane, MAINROOT->right, toDelete,MAINROOT, true);
    findToDelete(num_race, point, typeOfPlane, MAINROOT->left, toDelete, MAINROOT, false);
    if (toDelete.empty()){
        return;
    }
    AVLnode *bad_root = toDelete.back().second.second;
    vector<AVLnode*> toInsert;
    Delete(bad_root->left, toInsert);
    Delete(bad_root->right, toInsert);
    if (toDelete.back().first){
        toDelete.back().second.first->right = NULL;
    }
    else{
        toDelete.back().second.first->left = NULL;
    }
    for (auto c: toInsert){
        addPlane(c->value->num_race, c->value->point, c->value->typeOfPlane, MAINROOT);
    }
    addPlane(new_num_race, new_point, new_typeOfPlane, MAINROOT);
}
void dosomething(int ask){
    if (ask == 1){
        show(MAINROOT, 0);
    }
    if (ask == 2){
        cout << "Vvedite K\n";
        int k;
        cin >> k;
        vector<string> database;
        helper_function_for_find_more_than_k(MAINROOT, k, database);
        for (auto c: database){
            cout << c << "\n";
        }
        database.clear();
    }
    if (ask == 3){
        int num_race;
        string point;
        string typeOfPlane;
        cout << "Vvedite nomer reyca, punkt naznachenya i typeOfPlane\n";
        cin >> num_race >> point >> typeOfPlane;
        addPlane(num_race, point, typeOfPlane, MAINROOT);
    }
    if (ask == 4){
        cout << "Vvedite nomer reyca, punkt naznachenya i typeOfPlane samoleta kotority hotite pomenyat\n";
        int num_race;
        string point;
        string typeOfPlane;
        cin >> num_race >> point >> typeOfPlane;
        cout << "Vvedite novii dannyy\n";
        int new_num_race;
        string new_point;
        string new_typeOfPlane;
        cin >> new_num_race >> new_point >> new_typeOfPlane;
        changeNode(num_race, point, typeOfPlane, new_num_race, new_point, new_typeOfPlane);
    }
    if (ask == 5){
        cout << "Vvedite punkt naznachenya\n";
        string k;
        cin >> k;
        vector<pair<int, string>> checker;
        find_planes(MAINROOT, k, checker);
        for (auto c: checker){
            cout << c.first << " " << c.second << "\n";
        }
    }
    if (ask == 6){
        cout << "Vvedite nomer reyca, punkt naznachenya i typeOfPlane samoleta kotority hotite udalit'\n";
        int num_race;
        string point, typeOfPlane;
        cin >> num_race >> point >> typeOfPlane;
        Deleteplane(num_race, point, typeOfPlane, MAINROOT);
    }
    if (ask == 7){
        return;
    }
}
int main(){
    init();
    int ask;
    while(true){
        hellotext();
        cin >> ask;
        dosomething(ask);
        if (ask == 7){
            break;
        }

    }
    //debug("ok");
    return 0;

}
