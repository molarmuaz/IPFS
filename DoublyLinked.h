#pragma once
//
// pch.h
//

#pragma once

#include <iostream>

using namespace std;

class DoublyNode {
public:
	int data;
	DoublyNode* prev;
	DoublyNode* next;

	DoublyNode() {
		prev = NULL;
		next = NULL;
		data = 0;
	}

	DoublyNode(int a, DoublyNode* x, DoublyNode* y) {
		data = a;
		prev = x;
		next = y;

	}


};


class DoublyLinkedList {
public:
	DoublyNode* head;
	DoublyNode* tail;

	DoublyLinkedList() {
		head = NULL;
		tail = NULL;
	}

	DoublyNode* getHead() {
		return head;
	}

	void insert(int v) {
		DoublyNode* temp = new DoublyNode;
		temp->data=v;
		temp->next=NULL;
		temp->prev=NULL;

		if (head == NULL && tail == NULL) {
			head = temp;
			tail = temp;
		}

		else {
			tail->next = temp;
			temp->prev=tail;
			tail = temp;
		}

	}

	void insertToHead(int v) {
		DoublyNode* temp = new DoublyNode;
		temp->data=v;
		temp->next=NULL;
		temp->prev;

		if (head == NULL && tail == NULL) {
			head = temp;
			tail = temp;
		}

		else {
			temp->next=(head);
			head->prev=(temp);
			head = temp;
		}
	}

	bool isEmpty() {
		if (head == NULL && tail == NULL) {
			return true;
		}

		return false;
	}

	bool search(int v) {
		DoublyNode* p = new DoublyNode;
		p = head;

		while (p->next != NULL) {
			if (p->data == v) {
				return true;
			}
			p = p->next;
		}

		return false;
	}

	void update(int v, int x) {
		DoublyNode* p = new DoublyNode;
		p = head;

		while (p->next != NULL) {
			if (p->data == v) {
				p->data = x;
			}
			p = p->next;
		}

	}

	void insertAtIndex(int v, int index) {
		DoublyNode* temp = new DoublyNode;
		temp->data=v;
		temp->next=NULL;
		temp->prev=NULL;

		if (head == NULL && tail == NULL) {
			head = temp;
			tail = temp;
		}

		else {
			DoublyNode* p = new DoublyNode;
			p = head;
			DoublyNode* c = new DoublyNode;
			c = head;

			for (int i = 0; i < index; i++) {
				p = c;
				c = c->next;
			}

			p->next=(temp);
			temp->prev=(p);
			c->prev=(temp);
			temp->next=(c);
		}

	}

	void deleteData(int v) {


		DoublyNode* p = new DoublyNode;
		p = head;
		DoublyNode* c = new DoublyNode;
		c = head;

		while (c->data != v) {
			p = c;
			c = c->next;
		}

		p->next=(c->next);
		c->next->prev=(p);

		delete c;
	}


	void print() {
		DoublyNode* p = head;
		while (p != NULL) {
			cout << p->data << "\t";
			p = p->next;
		}
	}

};






