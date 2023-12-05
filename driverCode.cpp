#include "tree.h"

//run the tests here
int main() {
    bTree t(3);
    t.insert(1,"string1");
    t.insert(3,"string2");
    t.insert(7,"string3");
    t.insert(10,"string4");
    t.insert(11,"string5");
    t.insert(13,"string6");
    t.traverse(t.root);
    t.Delete(t.root, 10);
    t.Delete(t.root, 13);
    cout << endl;
    cout << "After deletions: " << endl;
    t.traverse(t.root);
	return 0;
}
