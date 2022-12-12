//by javier Alvarez Reyes
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <string>
#include <mutex>
#include <thread>
#include <map>
#include <string>

#include <ctime>
#include <chrono>

#define INT_MINI numeric_limits<int>::min() 
#define INT_MAXI numeric_limits<int>::max()

static int max_level ;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;
using namespace std;



class KeyValuePair {
private:
	int key;
	string value;
public:
	KeyValuePair();
	KeyValuePair(int key, string value);
	~KeyValuePair();
	int get_key();
	string get_value();
};

KeyValuePair::KeyValuePair() {
}

KeyValuePair::KeyValuePair(int k, string v) {
	key = k;
	value = v;
}


int KeyValuePair::get_key()
{
	return key;
	
}

string KeyValuePair::get_value() {
	return value;
}

KeyValuePair::~KeyValuePair() {
}

class Node {
public:
	// Almacena la clave y el valor del nodo.
	KeyValuePair key_value_pair;
	// Almacena la referencia del siguiente nodo hasta el nivel superior del nodo
	vector<Node*> next;
	// Bloquear para bloquear el nodo al modificarlo
	mutex node_lock;
	// Variable atómica donde al marcar si se elimina este Nodo
	atomic<bool> marked = { false };
	// Variable atómica para indicar que el Nodo está completamente vinculado a predecesores y sucesores.
	atomic<bool> fully_linked = { false };
	//El nivel máximo hasta el cual el nodo está disponible
	int top_level;
	Node();
	Node(int key, int level);
	Node(int key, string value, int level);
	~Node();
	int get_key();
	string get_value();
	void lock();
	void unlock();
};

Node::Node() {
}

Node::Node(int key, int level) {
	key_value_pair = KeyValuePair(key, "");
	next.resize(level + 1);
	for (size_t i = 0; i < next.size(); i++) {
		next[i] = NULL;
	}
	top_level = level;
}

Node::Node(int key, string value, int level) {
	key_value_pair = KeyValuePair(key, value);
	next.resize(level + 1);
	for (size_t i = 0; i < next.size(); i++) {
		next[i] = NULL;
	}
	top_level = level;
}


int Node::get_key() {
	return key_value_pair.get_key();
}


string Node::get_value() {
	return key_value_pair.get_value();
}


void Node::lock() {
	node_lock.lock();
}


void Node::unlock() {
	node_lock.unlock();
}

Node::~Node() {
}


class SkipList {
private:

	Node* head;
	Node* tail;
public:
	SkipList();
	SkipList(int max_elements, float probability);
	~SkipList();
	int get_random_level();


	int find(int key, vector<Node*>& predecessors, vector<Node*>& successors);
	bool add(int key, string value);
	string search(int key);
	bool remove(int key);
	map<int, string> range(int start_key, int end_key);
	void display();
};

SkipList::SkipList(int max_elements, float prob) {
    max_level = (int)round(log(max_elements) / log(1 / prob)) - 1;
    head = new Node(INT_MINI, max_level);
    tail = new Node(INT_MAXI, max_level);

    for (size_t i = 0; i < head->next.size(); i++) {
        head->next[i] = tail;
    }
}


int SkipList::find(int key, vector<Node*>& predecessors, vector<Node*>& successors) {
    int found = -1;
    Node* prev = head;

    for (int level = max_level; level >= 0; level--) {
        Node* curr = prev->next[level];

        while (key > curr->get_key()) {
            prev = curr;
            curr = prev->next[level];
        }

        if (found == -1 && key == curr->get_key()) {
            found = level;
        }

        predecessors[level] = prev;
        successors[level] = curr;
    }
    return found;
}


int SkipList::get_random_level() {
    int l = 0;
    while (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) <= 0.5) {
        l++;
    }
    return l > max_level ? max_level : l;
}



bool SkipList::add(int key, string value) {


    int top_level = get_random_level();


    vector<Node*> preds(max_level + 1);
    vector<Node*> succs(max_level + 1);

    for (size_t i = 0; i < preds.size(); i++) {
        preds[i] = NULL;
        succs[i] = NULL;
    }


    while (true) {


        int found = find(key, preds, succs);


        if (found != -1) {
            Node* node_found = succs[found];

            if (!node_found->marked) {
                while (!node_found->fully_linked) {
                }
                return false;
            }
            continue;
        }


        map<Node*, int> locked_nodes;


        try {
            Node* pred;
            Node* succ;


            bool valid = true;

            for (int level = 0; valid && (level <= top_level); level++) {
                pred = preds[level];
                succ = succs[level];


                if (!(locked_nodes.count(pred))) {
                    pred->lock();
                    locked_nodes.insert(make_pair(pred, 1));
                }


                valid = !(pred->marked.load(std::memory_order_seq_cst)) && !(succ->marked.load(std::memory_order_seq_cst)) && pred->next[level] == succ;
            }


            if (!valid) {
                for (auto const& x : locked_nodes) {
                    x.first->unlock();
                }
                continue;
            }

            Node* new_node = new Node(key, value, top_level);


            for (int level = 0; level <= top_level; level++) {
                new_node->next[level] = succs[level];
            }

            for (int level = 0; level <= top_level; level++) {
                preds[level]->next[level] = new_node;
            }


            new_node->fully_linked = true;


            for (auto const& x : locked_nodes) {
                x.first->unlock();
            }

            return true;
        }
        catch (const std::exception& e) {

            std::cerr << e.what() << '\n';
            for (auto const& x : locked_nodes) {
                x.first->unlock();
            }
        }
    }
}


string SkipList::search(int key) {


    vector<Node*> preds(max_level + 1);
    vector<Node*> succs(max_level + 1);

    for (size_t i = 0; i < preds.size(); i++) {
        preds[i] = NULL;
        succs[i] = NULL;
    }

    int found = find(key, preds, succs);

    if (found == -1) {
        return "";
    }

    Node* curr = head;

    for (int level = max_level; level >= 0; level--) {
        while (curr->next[level] != NULL && key > curr->next[level]->get_key()) {
            curr = curr->next[level];
        }
    }

    curr = curr->next[0];

    if ((curr != NULL) && (curr->get_key() == key) && succs[found]->fully_linked && !succs[found]->marked) {
        return curr->get_value();
    }
    else {
        return "";
    }
}


bool SkipList::remove(int key) {

    Node* victim = NULL;
    bool is_marked = false;
    int top_level = -1;


    vector<Node*> preds(max_level + 1);
    vector<Node*> succs(max_level + 1);

    for (size_t i = 0; i < preds.size(); i++) {
        preds[i] = NULL;
        succs[i] = NULL;
    }


    while (true) {

        int found = find(key, preds, succs);


        if (found != -1) {
            victim = succs[found];
        }


        if (is_marked |(found != -1 &&(victim->fully_linked && victim->top_level == found && !(victim->marked))))
        {

            if (!is_marked) {
                top_level = victim->top_level;
                victim->lock();
                if (victim->marked) {
                    victim->unlock();
                    return false;
                }
                victim->marked = true;
                is_marked = true;
            }


            map<Node*, int> locked_nodes;

            try {
                Node* pred;

                bool valid = true;

                for (int level = 0; valid && (level <= top_level); level++) {
                    pred = preds[level];


                    if (!(locked_nodes.count(pred))) {
                        pred->lock();
                        locked_nodes.insert(make_pair(pred, 1));
                    }

                    valid = !(pred->marked) && pred->next[level] == victim;
                }

                if (!valid) {
                    for (auto const& x : locked_nodes) {
                        x.first->unlock();
                    }
                    continue;
                }

                for (int level = top_level; level >= 0; level--) {
                    preds[level]->next[level] = victim->next[level];
                }

                victim->unlock();

                for (auto const& x : locked_nodes) {
                    x.first->unlock();
                }

                return true;
            }
            catch (const std::exception& e) {
               
                for (auto const& x : locked_nodes) {
                    x.first->unlock();
                }
            }

        }
        else {
            return false;
        }
    }
}


map<int, string> SkipList::range(int start_key, int end_key) {

    map<int, string> range_output;

    if (start_key > end_key) {
        return range_output;
    }

    Node* curr = head;

    for (int level = max_level; level >= 0; level--) {
        while (curr->next[level] != NULL && start_key > curr->next[level]->get_key()) {
            if (curr->get_key() >= start_key && curr->get_key() <= end_key) {
                range_output.insert(make_pair(curr->get_key(), curr->get_value()));
            }
            curr = curr->next[level];
        }
    }

    while (curr != NULL && end_key >= curr->get_key()) {
        if (curr->get_key() >= start_key && curr->get_key() <= end_key) {
            range_output.insert(make_pair(curr->get_key(), curr->get_value()));
        }
        curr = curr->next[0];
    }

    return range_output;

}


void SkipList::display() {
    for (int i = 0; i <= max_level; i++) {
        Node* temp = head;
        int count = 0;
        if (!(temp->get_key() == INT_MINI && temp->next[i]->get_key() == INT_MAXI)) {
            printf("Level %d  ", i);
            while (temp != NULL) {
                printf("%d -> ", temp->get_key());
                temp = temp->next[i];
                count++;
            }
            cout << endl;
        }
        if (count == 3) break;
    }
    printf("---------- Termino con Exito ----------\n\n");
}

SkipList::SkipList() {
}

SkipList::~SkipList() {
}

//_______________________________________________________________________________________________________________
size_t num_threads = 4;

//_______________________________________________________________________________________________________________
SkipList skiplist;


vector<int> numbers_insert;
vector<int> numbers_delete;
vector<int> numbers_get;
vector<pair<int, int>> numbers_range;





void generate_input(int max_number) {

    for (int i = 1; i <= max_number; i++) {
        numbers_insert.push_back(i);
    }


    for (size_t i = 0; i < numbers_insert.size(); i++) {
        if (rand() % 4 == 0) {
            numbers_delete.push_back(numbers_insert[i]);
        }
    }

    for (size_t i = 0; i < numbers_insert.size(); i++) {
        if (rand() % 4 == 0) {
            numbers_get.push_back(numbers_insert[i]);
        }
    }


    for (size_t i = 0; i < num_threads; i++) {
        int a = (rand() % static_cast<int>(max_number + 1));
        int b = a + (rand() % static_cast<int>(max_number - a + 1));
        numbers_range.push_back(make_pair(a, b));
    }
}


void skiplist_add(size_t start, size_t end) {
    if (end >= numbers_insert.size()) end = numbers_insert.size();
    if (start == end) skiplist.add(numbers_insert[start], to_string(numbers_insert[start]));
    for (size_t i = start; i < end; i++) {
        skiplist.add(numbers_insert[i], to_string(numbers_insert[i]));
    }
}


void skiplist_remove(size_t start, size_t end) {
    if (end >= numbers_delete.size()) end = numbers_delete.size();
    if (start == end) skiplist.remove(numbers_delete[start]);
    for (size_t i = start; i < end; i++) {
        skiplist.remove(numbers_delete[i]);
    }
}


void skiplist_search(size_t start, size_t end) {
    if (end >= numbers_get.size()) end = numbers_get.size();
    if (start == end) end++;
    for (size_t i = start; i < end; i++) {
        string s = skiplist.search(numbers_get[i]);
        if (s.empty()) s = "No encontrar";
        cout << "Buscando: " << numbers_get[i] << " Buscar valor: " << s << endl;
    }
}


void skiplist_range(int start, int end) {
    map<int, string> range_output = skiplist.range(start, end);

    string s = "";
    for (auto const& x : range_output) {
        s += x.second + " ";
    }
    cout << "Rango (" << start << ", " << end << ") = " << s << endl;
}


void concurrent_skiplist_seperate() {
    vector<thread> threads;
    
    // insert
    int chunk_size = ceil(float(numbers_insert.size()) / num_threads);
    
    for (size_t i = 0; i < numbers_insert.size(); i = i + chunk_size) {
        threads.push_back(thread(skiplist_add, i, i + chunk_size));
    }
    for (auto& th : threads) {
        th.join();
    }
    
    cout << "\n---------- Skip list antes de insertar ----------" << endl;

    skiplist.display();

    threads.clear();
    
    cout << "\n----------Eliminación de los siguientes elementos de la lista  ----------" << endl;
	
    for (auto i = numbers_delete.begin(); i != numbers_delete.end(); ++i)
        std::cout << *i << ' ';

    // delete
    chunk_size = ceil(float(numbers_delete.size()) / num_threads);
    for (size_t i = 0; i < numbers_delete.size(); i = i + chunk_size) {
        threads.push_back(thread(skiplist_remove, i, i + chunk_size));
    }
    for (auto& th : threads) {
        th.join();
    }

    cout << "\n\n---------- Skip list después de eliminar ---------- " << endl;

    skiplist.display();

    threads.clear();
    
    cout << "\n---------- Buscando números aleatorios en Skip list ---------- " << endl;
    // search
    chunk_size = ceil(float(numbers_get.size()) / num_threads);
    for (size_t i = 0; i < numbers_get.size(); i = i + chunk_size) {
        threads.push_back(thread(skiplist_search, i, i + chunk_size));
    }
    for (auto& th : threads) {
        th.join();
    }

    threads.clear();

    cout << "\n---------- Rango entre números aleatorios en Skip list ---------- " << endl;
    // range
    for (size_t i = 0; i < num_threads; i++) {
        threads.push_back(thread(skiplist_range, numbers_range[i].first, numbers_range[i].second));
    }
    for (auto& th : threads) {
        th.join();
    }

    threads.clear();
}


void skiplist_combined_operations() {

    int start = (rand() % static_cast<int>(numbers_insert.size() + 1));
    int end = start + (rand() % static_cast<int>(numbers_insert.size() - start + 1));
    cout << "Insertar: " << start << " " << end << endl;
    skiplist_add(start, end);
    
    start = (rand() % static_cast<int>(numbers_delete.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_delete.size() - start + 1));
    cout << "Eliminar: " << start << " " << end << endl;
    skiplist_remove(start, end);

    start = (rand() % static_cast<int>(numbers_get.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_get.size() - start + 1));
    cout << "Buscar: " << start << " " << end << endl;
    skiplist_search(start, end);

    start = (rand() % static_cast<int>(numbers_insert.size() + 1));
    end = start + (rand() % static_cast<int>(numbers_insert.size() - start + 1));
    cout << "Rango: " << start << " " << end << endl;
    skiplist_range(start, end);
}


void concurrent_skiplist_combined() {
    
    cout << "\n---------- Numeros insertados ----------" << endl;

    for (auto i = numbers_insert.begin(); i != numbers_insert.end(); ++i)
        std::cout << *i << ' ';
    cout << endl;
    
    cout << "\n---------- Numeros Eliminados ----------" << endl;

    for (auto i = numbers_delete.begin(); i != numbers_delete.end(); ++i)
        std::cout << *i << ' ';
    cout << endl;

    cout << "\n---------- Numeros Buscados ----------" << endl;

    for (auto i = numbers_get.begin(); i != numbers_get.end(); ++i)
        std::cout << *i << ' ';
    cout << endl;
    
    vector<thread> threads;
    
    for (size_t i = 0; i < num_threads; i++) {
        threads.push_back(thread(skiplist_combined_operations));
    }
    for (auto& th : threads) {
        th.join();
    }
    
    skiplist.display();

}



int main(int argc, char* argv[]) 
{
    generate_input(100000);
    skiplist = SkipList(numbers_insert.size(), 0.5);
    concurrent_skiplist_seperate();
	cout << "numero de hilos usados" << num_threads << endl;
	return 0;
}