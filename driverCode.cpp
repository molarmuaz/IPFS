#include "btreemain.h"


int main() {
    bTree t(4);
    t.insert2(1, "string1");
    //t.insert2(1, "string11");
    t.insert2(2, "string2");
    t.insert2(3, "string3");
    t.insert2(4, "string4");
    t.insert2(5, "string5");
    t.insert2(6, "string6");
    t.insert2(7, "string7");
    t.insert2(8, "string8");
    t.insert2(9, "string9");
    t.insert2(3, "string31");
    //t.Delete(t.root, 3);
    //t.insert(10, "string10");
    //t.insert(11, "string11");
    //t.insert(13, "string13");
    t.Display(t.root);
    //t.Delete(t.root, 10);
    //t.Delete(t.root, 13);
    //cout << endl;
    //cout << "After deletions: " << endl;
   // t.traverse(t.root);
    return 0;
}
