#include <iostream>
#include <fstream>
#include <sstream>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

//cvor stabla
struct Node {
    string data; 
    Node* child;  
    Node* sibling;  

    Node(string value) : data(value), child(nullptr), sibling(nullptr) {}
};

//cvor ulancane liste
struct ListNode {
    Node* data;
    ListNode* next;

    ListNode(Node* node) : data(node), next(nullptr) {}
};



//struktura stek, potrebna zbog izbacivanja programskog steka iz stabla
struct Stack {
    ListNode* top;
    Stack() : top(nullptr) {}

    Node* getTop() {
        if (top != nullptr) {
            return top->data;
        }
        else {
            return nullptr;
        }
    }

    void push(Node* node) {
        ListNode* newNode = new ListNode(node);
        if (top == nullptr) {
            top = newNode;
        }
        else {
            newNode->next = top;
            top = newNode;
        }
    }
    Node* pop() {
        if (top == nullptr) {
            return nullptr;
        }
        else {
            ListNode* curr = top;
            top = top->next;
            Node* d = curr->data;
            curr->next = nullptr;
            delete curr;
            return d;
        }
    }
    bool isEmpty() {
        if (top == nullptr) {
            return true;
        }
        else {
            return false;
        }
    }
};

//struktura red, potrebna za level order
struct Queue {
    ListNode* front;
    ListNode* rear;

    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(Node* node) {
        ListNode* newNode = new ListNode(node);
        if (rear == nullptr) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    Node* dequeue() {
        if (front == nullptr) {
            return nullptr;
        }
        ListNode* temp = front;
        Node* node = temp->data;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        return node;
    }

    bool isEmpty() const {
        return front == nullptr;
    }
};

//dodavanje deteta cvoru stabla
void addChild(Node* parent, string value) {
    Node* newChild = new Node(value);
    if (parent->child == nullptr) {
        parent->child = newChild;
    }
    else {
        Node* curr = parent->child;
        while (curr->sibling != nullptr) {
            curr = curr->sibling;
        }
        curr->sibling = newChild;
    }
}

//dodavanje siblinga
void addSibling(Node* node, string value) {
    Node* newSibling = new Node(value);
    if (node != nullptr) {
        while (node->sibling != nullptr) {
            node = node->sibling;
        }
        node->sibling = newSibling;
    }
}

//brisanje stabla iz memorije
void deleteTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    Queue queue;
    queue.enqueue(root);

    while (!queue.isEmpty()) {
        Node* current = queue.dequeue();

        delete current;

        // Dodaj sve decu čvora u red
        Node* child = current->child;
        while (child != nullptr) {
            queue.enqueue(child);
            child = child->sibling;
        }
    }
}

//ispis cvorova stabla po level order obilasku
void printTree(Node* root) {
    if (root == nullptr) {
        return;
    }

    Queue queue;
    queue.enqueue(root);

    while (!queue.isEmpty()) {
        Node* current = queue.dequeue();

        cout << current->data << " ";

        Node* child = current->child;
        while (child != nullptr) {
            queue.enqueue(child);
            child = child->sibling;
        }
    }
}

//cvor grafa
struct GraphNode {
    string data;
    int inputBranches;
    ListNode* neighbour;
    GraphNode* nextNode;
    GraphNode(string value) : data(value), inputBranches(0), neighbour(nullptr), nextNode(nullptr) {}
};

struct ListGraphNode {
    GraphNode* data;
    ListGraphNode* next;

    ListGraphNode(GraphNode* node) : data(node), next(nullptr) {}
};

struct GraphNodeQueue {
    ListGraphNode* front;
    ListGraphNode* rear;

    GraphNodeQueue() : front(nullptr), rear(nullptr) {}

    void enqueue(GraphNode* node) {
        ListGraphNode* newNode = new ListGraphNode(node);
        newNode->data->neighbour = node->neighbour;
        if (rear == nullptr) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    GraphNode* dequeue() {
        if (front == nullptr) {
            return nullptr;
        }
        ListGraphNode* temp = front;
        GraphNode* node = temp->data;
        node->neighbour = temp->data->neighbour;
        front = front->next;
        if (front == nullptr) {
            rear = nullptr;
        }
        delete temp;
        return node;
    }

    bool isEmpty() const {
        return front == nullptr;
    }
};

void bfsPrint(GraphNode* g) { //zelimo da printujemo po bfs obilasku
    string visited[100]; //cuvamo one koji su poseceni
    int cap = 0;
    for (int i = 0; i < 100; i++) {
        visited[i] = "";
    } // inicijalizovano za svaki slucaj
    if (g == nullptr) {
        return;
    }
    visited[0] = g->data; //ubacimo g u visited
    cap = 1;
    cout << g->data; //obradimo g
    GraphNodeQueue q; //inicijalizujemo queue
    q.enqueue(g); //ubacimo g u queue
    while (!q.isEmpty()) {
        GraphNode* curr = q.dequeue(); //izvadimo jedan iz queue-a
        ListNode* neighbours = curr->neighbour;
        //cout << neighbours->data->data;
        ListNode* currNeighbour = neighbours; //iteriramo kroz sve njegove neighboure
        while (currNeighbour) {
            
            bool found = false; //pretpostavimo da nije bio u visited
            for (int i = 0; i < 100; i++) {
                if (currNeighbour->data->data == visited[i]) { //ako ga nadjemo u visited, break
                    found = true;
                    break;
                }
            } //ako je posecen prethodno, found je true, ako nije found je false
            if (!found) {
                cout << curr->data; //obradimo trenutni cvor
                visited[cap++] = curr->data; //dodamo ga u visited
                GraphNode* iter = g; //zelimo da pronadjemo node koji je currNeighbour i njega da stavimo u red
                while (iter) { //iteriramo kroz nodeove
                    if (iter->data == currNeighbour->data->data) { //kad ga nadjemo, dodajemo u red i breakujemo
                        q.enqueue(iter);
                        break;
                    }
                    iter = iter->nextNode; //ako nismo nasli, idemo na sledeci pa opet proveravamo
                }
            }
            currNeighbour = currNeighbour->next;
        }
    }
}

//konverzija iz stabla u graf
GraphNode* Tree2Graph(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }

    Queue queue;
    queue.enqueue(root);

    GraphNode* graph = new GraphNode(root->data);
    
    GraphNode* currGraphNode = graph;
    ListNode* dataArr = new ListNode(root);

    while (!queue.isEmpty()) {
        Node* current = queue.dequeue();
        ListNode* iter = dataArr;
        bool found = false;

        while (iter) {
            if (iter->data->data == current->data) {
                found = true;
                break;
            }
            else if (iter->next == nullptr) {
                iter->next = new ListNode(current);
                break;
            }
            else {
                iter = iter->next;
            }
        }
        if (current == root) {
            found = false;
        }

        if (!found) {
            GraphNode* newGN = new GraphNode(current->data);
            if (current == root) {
                GraphNode* temp = graph;
                graph = newGN;
                delete temp;
            }
            ListNode* neighbours = nullptr;
            ListNode* currNeighbour = neighbours;
            Node* children = current->child;
            while (children) {
                
                if (neighbours == nullptr) {
                    neighbours = new ListNode(children);
                    currNeighbour = neighbours; 
                }
                else {
                    ListNode* newNeighbour = new ListNode(children);
                    currNeighbour->next = newNeighbour;
                    currNeighbour = newNeighbour;
                }
                children = children->sibling;
            }
            newGN->neighbour = neighbours;
            currGraphNode->nextNode = newGN;
            currGraphNode = newGN;
        }
        else {
            GraphNode* currGraphNode = graph;
            while (currGraphNode) {
                if (currGraphNode->data == current->data) {
                    break;
                }
                currGraphNode = currGraphNode->nextNode;
            }
            //dosli smo do tog graphNoda u grafu
            
            Node* children = current->child;
            Node* currChild = children;
            //currNeighbour iterira kroz neighboure, currChild iterira kroz siblinge

            while (currChild) {
                ListNode* neighbours = currGraphNode->neighbour;
                ListNode* currNeighbour = neighbours;
                bool foundInNeighbours = false;
                while (currNeighbour) {
                    if (currNeighbour->data->data == currChild->data) {
                        foundInNeighbours = true;
                        break;
                    }
                    currNeighbour = currNeighbour->next;
                }
                if (foundInNeighbours) {
                    
                }
                else {
                    ListNode* newNeighbour = new ListNode(currChild);
                    newNeighbour->next = currGraphNode->neighbour;
                    currGraphNode->neighbour = newNeighbour;
                }
                currChild = currChild->sibling;
            }
        }
        // Dodaj svu decu čvora u red
        Node* child = current->child;
        while (child != nullptr) {
            queue.enqueue(child);
            child = child->sibling;
        }
    }
    return graph;
}

//ispis stabla
void graphPrint(GraphNode* graph) {
    
    GraphNode* curr = graph;
    cout << endl;
    while (curr) {
        cout << "Cvor " << curr->data << "{" << curr->inputBranches << "}" << ": ";
        while (curr->neighbour) {
            cout << curr->neighbour->data->data << " ";
            curr->neighbour = curr->neighbour->next;
        }
        cout << endl;
        curr = curr->nextNode;
    }
}

//ucitavanje podataka iz .txt fajla
bool readFromFile(const string& fileName, string*** dataArray, int& rowCount, int*& columnCounts) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Nemoguce otvoriti fajl: " << fileName << endl;
        return false;
    }

    rowCount = 0;

    columnCounts = nullptr;

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string word;

        int currentColumnCount = 0;

        while (iss >> word) {
            currentColumnCount++;
        }

        if (rowCount == 0) {
            columnCounts = new int[currentColumnCount];
        }

        columnCounts[rowCount] = currentColumnCount;

        rowCount++;
    }

    file.close();


    file.open(fileName);

    *dataArray = new string * [rowCount];

    for (int i = 0; i < rowCount; i++) {
        (*dataArray)[i] = new string[columnCounts[i]];

        for (int j = 0; j < columnCounts[i]; j++) {
            file >> (*dataArray)[i][j];
        }
    }

    // Zatvori fajl
    file.close();

    return true;
}

//oslobadjanje memorije iz matrice iz .txt fajla
void releaseMemory(string*** dataArray, int rowCount, int* columnCounts) {
    for (int i = 0; i < rowCount; i++) {
        delete[](*dataArray)[i];
    }

    delete[] * dataArray;

    delete[] columnCounts;

    *dataArray = nullptr;
    columnCounts = nullptr;
}

//dodavanje programskog steka u stablo
Node* addStack(Node* root, string* dataArr, int rowCount) {
    
    if (!root) {
        Node* newRoot = new Node(dataArr[0]);
        Node* curr = newRoot;
        for (int i = 1; i < rowCount; i++) {
            addChild(curr, dataArr[i]);
            curr = curr->child;
        }
        return newRoot;
    }
    else {
        Node* newRoot = root;
        Node* curr = newRoot;
        if (curr->data != dataArr[0]) {
            cout << "Nemoguce je formirati zadato stablo";
        }
        else {
            for (int i = 1; i < rowCount; i++) {
                if (curr->child) {
                    curr = curr->child;
                }
                else {
                    addChild(curr, dataArr[i]);
                    curr = curr->child;
                }
                while (curr) {
                    if (curr->data != dataArr[i]) {
                        if (curr->sibling) {
                            curr = curr->sibling;
                        }
                        else {
                            addSibling(curr, dataArr[i]);
                            curr = curr->sibling;
                        }
                    }
                    else {
                        break;
                    }
                    
                }
            }
        }
        return newRoot;
    }
    
}

//brisanje programskog steka iz stabla
void deleteStack(Node*& root, string* dataArr, int rowCount) {
    Node* curr = root;
    Stack s;
    Stack prev;
    s.push(root);
    prev.push(nullptr);
    if (curr->data != dataArr[0]) {
        cout << "Programski stek nije pronadjen";
        return;
    }
    for (int i = 1; i < rowCount; i++) {
        Node* currChild = curr->child;
        while (currChild) {
            if (currChild->data == dataArr[i]) {
                s.push(currChild);
                prev.push(nullptr);
                curr = currChild;
                break;
            }
            else {
                Node* prevChild = currChild;
                currChild = currChild->sibling;
                if (currChild == nullptr) {
                    cout << "Programski stek nije pronadjen";
                    return;
                }
                if (currChild->data == dataArr[i]) {
                    s.push(currChild);
                    prev.push(prevChild);
                    curr = currChild;
                    break;
                }
            }
            
        }
    }
    
    while (!s.isEmpty()) {
        curr = s.pop();
        Node* prevChild = prev.pop();
        if (!curr->child) {
            if (prevChild == nullptr) {
                Node* father = s.getTop();
                if (father == nullptr) {
                    root = nullptr;
                    delete curr;
                    return;
                }
                father->child = curr->sibling;
                curr->sibling = nullptr;
                delete curr;
            }
            else {
                prevChild->sibling = curr->sibling;
                curr->sibling = nullptr;
                delete curr;
            }
        }
    }
}

//konvertovanje iz matrice iz .txt fajla u stablo
Node* Matrix2Tree(string** dataMatrix, int rowCount, int* columnCounts) {
    Node* newRoot = nullptr;
    for (int i = 0; i < rowCount; i++) {
        newRoot = addStack(newRoot, dataMatrix[i], columnCounts[i]);
    }
    return newRoot;
}

//postavljanje broja ulaznih grana za svaki cvor grafa
void updateInputBranches(GraphNode* graph) {
   
    GraphNode* curr = graph;
    while (curr) { //cvor kome zelimo da updateujemo br ulaznih grana
        GraphNode* iter = graph;
        int n = 0;
        while (iter) {
            ListNode* iterNeighbours = iter->neighbour;
            while (iterNeighbours) {
                if (iterNeighbours->data->data == curr->data) {
                    n++;
                }
                iterNeighbours = iterNeighbours->next;
            }
            iter = iter->nextNode;
        }
        curr->inputBranches = n;
        curr = curr->nextNode;
    }
}

//provera da li je graf ciklican
bool isCyclic(GraphNode graph) {
    GraphNode* currGN = &graph;
    ListNode* visited = nullptr;
    //currGN - trenutni cvor grafa koji obilazimo
    //visited - lista posecenih cvorova
    while (currGN) {
        //currGN je trenutni 
        if (currGN->inputBranches == 0) { //da li ima inputBranches
            
            bool currVisited = false; //pretpostavimo da ga nismo posecivali
            ListNode* iter = visited;
            while (iter) {
                if (iter->data->data == currGN->data) {
                    currVisited = true;
                    break;
                }
                iter = iter->next;
            }
            //iteriramo tako da currVisited ima vr 1 ako smo ga nasli, 0 ako nismo
            if (!currVisited) { //ako nije posecen
                Node* n = new Node(currGN->data);
                ListNode* newVisit = new ListNode(n); //novi listnode da dodamo u listu posecenih
                if (visited == nullptr) {
                    visited = newVisit;
                }
                else {
                    ListNode* iter = visited;
                    while (iter->next) {
                        iter = iter->next;
                    }
                    iter->next = newVisit;
                } // dodali smo u listu visited
                currGN->neighbour = nullptr;
                updateInputBranches(&graph);
                //update-ovali smo input branches nakon obilaska currNode
                currGN = &graph; 

            }
            else {
                currGN = currGN->nextNode;
            } 
        }
        else {
            currGN = currGN->nextNode;
        }
    }

    GraphNode* iter = &graph;
    bool cyclic = false;
    while (iter) {
        if (iter->inputBranches != 0) {
            cyclic = true;
            break;
        }
        iter = iter->nextNode;
    }
    return cyclic;
}

int main() {
    
    int izbor;
    string fileName;
    bool kraj = false;
    bool success;
    string** dataArray = nullptr;
    int rowCount = 0;
    int rowCount1 = 0;
    int rowCount2 = 0;
    int* columnCounts = nullptr;
    Node* root = nullptr;
    GraphNode* graph = nullptr;
    bool ciklus = true;
    char input1[100];
    string input;
    char* token;
    istringstream stream;
    string stringData1[100];
    string stringData2[100];
    while (!kraj) {
        cout << endl << "------------------------------------------------------------------------------------------------------" << endl << endl <<
            "1. Ucitaj programske stekove iz .txt fajla i formiraj stablo" << endl <<
            "2. Dodaj programski stek u stablo" << endl <<
            "3. Ukloni programski stek iz stabla" << endl <<
            "4. Ispisi stablo" << endl <<
            "5. Konvertuj stablo u graf" << endl <<
            "6. Ispisi graf" << endl <<
            "7. Proveri da li ima rekurzivnih poziva funkcija" << endl <<
            "8. Zavrsi rad" << endl <<
            "Vas izbor: ";

        cin >> izbor;
        cin.ignore();
        switch (izbor) {
        case 1:
            cout << "Unesi naziv fajla (sa ekstenzijom .txt): " << endl;
            cin >> fileName;
            success = readFromFile(fileName, &dataArray, rowCount, columnCounts);
            if (success) {
                root = Matrix2Tree(dataArray, rowCount, columnCounts);
            }
            break;
        case 2:
            cout << "Unesi programski stek koji zelis da ubacis: " << endl;
            getline(cin, input);
            rowCount1 = 0;
            stream = (istringstream)input;
            
            while (stream >> input) {
                stringData1[rowCount1++] = input;
            }
            root = addStack(root, stringData1, rowCount1);
            break;
        case 3:
            cout << "Unesi programski stek koji zelis da izbrises: " << endl;
            getline(cin,input);
            rowCount1 = 0;
            stream = (istringstream)input;
            while (stream >> input) {
                stringData1[rowCount1++] = input;

            }
            deleteStack(root, stringData1, rowCount1);
            break;
        case 4:
            printTree(root);
            break;
        case 5:
            graph = Tree2Graph(root);
            break;
        case 6:
            
            updateInputBranches(graph);
            
            graphPrint(graph);
            bfsPrint(graph);
            break;
        case 7:
            ciklus = isCyclic(*graph);
            if (ciklus == true) {
                std::cout << "Ima rekurzivnih fukncija";
            }
            else {
                std::cout << "Nema rekurzivnih funkcija";
            }
            graph = Tree2Graph(root);
            break;
        case 8:
            kraj = true;
            break;
        default:
            std::cout << "Ne postoji ta opcija";
            kraj = true;
        }
    }
    
    
    //string** dataArray = nullptr;
    //string fileName = "primer1.txt";
    //Node* root = nullptr;
    //int rowCount = 0;
    //int* columnCounts  = nullptr;
    //bool success = readFromFile(fileName, &dataArray, rowCount, columnCounts);
    //if (success) {
      //  root = Matrix2Tree(dataArray, rowCount, columnCounts);
    //}

    
    
    //string treci[] = { "main", "b", "c", "d" };
    //deleteStack(root, treci, 5);
    //GraphNode* g = Tree2Graph(root);
    //graphPrint(g);
    //bool c = isCyclic(g);
    //cout << c;
    // Primer korišćenja
    //Node* root = new Node("kaca");
    //addChild(root, "joakim");
    //addChild(root, "mama");
    //addChild(root->child, "tata");
    //addChild(root->child->sibling, "bla");
    //addChild(root, "aca");
    //string prvi[] = { "main", "a", "c", "d" };
    //string drugi[] = { "main", "a", "d" };
   
    //string cetvrti[] = { "main", "b", "c", "d" };

    //string peti[] = { "main", "x", "c", "d" };
    //Node* root = nullptr;
    //root = addStack(root, prvi, 4);
    //root = addStack(root, drugi, 3);
    //root = addStack(root, treci, 5);
    //root = addStack(root, cetvrti, 4);
    //cout << "Stablo: ";
   // levelOrderPrint(root);
    //printTree(root);
    //GraphNode* g = Tree2Graph(root);
   // graphPrint(g);

    //deleteStack(root, treci, 5);
    //cout << "Stablo: ";
    //levelOrderPrint(root);
    //deleteStack(root, prvi, 4);
    //cout << "Stablo: ";
    //levelOrderPrint(root);
    //deleteStack(root, drugi, 3);
    //cout << "Stablo: ";
    //levelOrderPrint(root);
    //deleteStack(root, peti, 4);
    //cout << "Stablo: ";
    //levelOrderPrint(root);

    // Oslobađanje memorije
   // deleteTree(root);
    // ucitavanje iz txt fajla
    
    //releaseMemory(&dataArray, rowCount, columnCounts);
    
    return 0;
}

