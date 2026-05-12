#ifndef POINT_LIST
#define POINT_LIST

#include "vec2.h"

template <typename T>
struct Node{
    T v;
    Node* next;
    Node(T v){
        this->v = v;
        next = nullptr;
    }
    float duration;
};

template <typename T>
struct PointList{
    Node<T>* head {nullptr};
    int size = 0;

    void push(T p){
        Node<T>* newPoint = new Node<T>(p);
        if (head == nullptr){
            head = newPoint;
            size++;
            return;
        }

        Node<T>* lastPoint = head;
        while(lastPoint->next)
            lastPoint = lastPoint->next;

        lastPoint->next = newPoint;
        size++;
    }

    void clean(){
        Node<T>* current = head;
        while (current) {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;
        size = 0;
    }

    Node<T>* getNodeAtPosition(int position) const {
        if(position >= size || position < 0)
            return nullptr;
        Node<T>* current = head;
        while(position--)
            current = current->next;
        return current;
    }

    //the rest are honestly very extra, just wrote them for fun as I haven't seen lists in a long time
    int length() const{
        int total = 0;
        Node<T>* current = head;
        // A B C D E
        while(current){
            current = current->next;
            total++;
        }
        return total;
    }
    
    void remove(int pos){
        if(pos < 0)
            return;

        Node<T>* current = head;

        if(pos == 0){
            head = current->next;
            delete current;
            size--;
            return;
        }

        for(int i = 0; i < pos-1; i++)
            current = current->next;

        if(current == nullptr || current->next == nullptr)
            return;

        Node<T>* next = current->next->next;
        delete current->next;
        current->next = next;
        size--;
    }

    void pop(){
        if (head == nullptr) return;

        if (head->next == nullptr) { 
            delete head;
            head = nullptr;
            size--;
            return;
        }

        Node<T>* lastPoint = head;
        while (lastPoint->next->next)
            lastPoint = lastPoint->next;

        delete lastPoint->next;
        lastPoint->next = nullptr; 
        size--;
    }
};

#endif