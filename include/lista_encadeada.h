#ifndef LISTA_ENCADEADA_H
#define LISTA_ENCADEADA_H

template <typename T>
class ListaEncadeada {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d) : data(d), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
    ListaEncadeada() : head(nullptr), tail(nullptr), size(0) {}
    ~ListaEncadeada();

    void push_back(const T& value);
    void clear();
    int get_size() const;
    bool empty() const;

    class Iterator {
    private:
        Node* current;
    public:
        Iterator(Node* node) : current(node) {}
        T& operator*() { return current->data; }
        T* operator->() { return &current->data; }
        Iterator& operator++() { current = current->next; return *this; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
    };

    class ConstIterator {
    private:
        const Node* current;
    public:
        ConstIterator(const Node* node) : current(node) {}
        const T& operator*() const { return current->data; }
        const T* operator->() const { return &current->data; }
        ConstIterator& operator++() { current = current->next; return *this; }
        bool operator!=(const ConstIterator& other) const { return current != other.current; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
    ConstIterator begin() const { return ConstIterator(head); }
    ConstIterator end() const { return ConstIterator(nullptr); }
};


template <typename T>
ListaEncadeada<T>::~ListaEncadeada() {
    clear();
}

template <typename T>
void ListaEncadeada<T>::push_back(const T& value) {
    Node* newNode = new Node(value);
    if (tail) {
        tail->next = newNode;
    } else {
        head = newNode;
    }
    tail = newNode;
    ++size;
}

template <typename T>
void ListaEncadeada<T>::clear() {
    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    size = 0;
}

template <typename T>
int ListaEncadeada<T>::get_size() const {
    return size;
}

template <typename T>
bool ListaEncadeada<T>::empty() const {
    return size == 0;
}

#endif