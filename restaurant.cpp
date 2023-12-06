#include "main.h"

#include<bits/stdc++.h> 
#include<string>
using namespace std;
//----------------------------------------------------------------
int MAXSIZE = 0;

//----------------------------------------------------------------
auto findChar(const char c, vector<pair<char, int>> &v) {
    for (auto it = v.begin(); it != v.end(); ++it) if(it->first == c) return it;
    return v.end();
}
vector<pair<char, int>> foldString(string& s) {
    vector<pair<char, int>> v;
    int n = s.length();
    for(int i = 0; i < n;i++) {
        auto it = findChar(s[i], v);
        if(it != v.end()) it->second++;
        else v.push_back(pair<char, int>(s[i], 1)); 
    }    
    return v;
}
vector<pair<char,int>> SortCodeCeasar(vector<pair<char, int>> &v) {
    string rel = "";
    vector<pair<char, int>> res;
    for (auto& it : v) {
        int gap = it.second;
        if(it.first <= 'Z') it.first = (it.first + gap - 65) % 26 + 65;
        else it.first = (it.first + gap - 97) % 26 + 97;
        auto find = findChar(it.first, res);
        if(find != res.end()) find->second += it.second;
        else res.push_back(it);
    }
    sort(res.begin(), res.end(), [](const auto &a,const auto &b) {
        if(a.second == b.second) {
            if(a.first <= 'Z' && b.first >= 'a') return true;
        }
        return a.second < b.second;       
    });
    return res;
}
// Huffman tree node abstract base class
class HuffTree {
    public:
    HuffTree *root = nullptr;
    HuffTree *left = nullptr;
    HuffTree *right = nullptr;
    vector<HuffTree*> vHuff;
    char value = 0;
    int freq = 0;
    bool isLeaf = false;
    public:
    HuffTree(){};
    HuffTree(char value, int freq, bool isLeaf) : value(value), freq(freq), isLeaf(isLeaf) {};
    HuffTree(HuffTree *a, HuffTree *b) : freq(a->freq + b->freq), left(a), right(b) {}; 
    ~HuffTree(){};
    void freeHuffTree() {
        queue<HuffTree *> q;
        q.push(root);
        while(!q.empty()) {
            HuffTree *temp = q.front(); q.pop();
            if(temp->left) q.push(temp->left);
            if(temp->right) q.push(temp->right);
            delete temp;
            temp = nullptr;
        }
    }
    void deleteHuffTree() {
        for(auto i = vHuff.size() -1; i >= 0; i--) {
            if(vHuff[i]->left || vHuff[i]->right) {
                queue<HuffTree *> qdel; qdel.push(vHuff[i]);
                while(!qdel.empty()) {
                    HuffTree *del = qdel.front(); qdel.pop();
                    if(del->left) qdel.push(del->left);
                    if(del->right) qdel.push(del->right);
                    delete del; del = nullptr;
                }
                vHuff[i] = nullptr; vHuff.pop_back();
            }
            else { delete vHuff[i]; vHuff[i] = nullptr; vHuff.pop_back();}
        }
    }
    bool rotateLeft(HuffTree *&p) {
        HuffTree *t = p->right;
        p->right = t->left;
        t->left = p;
        p = t; 
        if(root->value) {
            deleteHuffTree();
            return true;
        }
        return false;
    }
    bool rotateRight(HuffTree *&p) {
        HuffTree *t = p->left;
        p->left = t->right;
        t->right = p;
        p = t;
        if(root->value) {
            deleteHuffTree();
            return true;
        }
        return false;
    }
    int getLevel(HuffTree *p) {
        if(!p) return 0;
        return 1 + max(getLevel(p->left), getLevel(p->right));
    }
    void updateBalance(HuffTree *&p, bool &checkRoot) {
        int levelLeft = getLevel(p->left);
        int levelRight = getLevel(p->right);
        if(abs(levelLeft - levelRight) > 1) {
            if(levelLeft > levelRight) {
                HuffTree *t = p->left;
                if(getLevel(t->left) >= getLevel(t->right)) rotateRight(p);
                else {
                    if(rotateLeft(p->left)) {checkRoot = true; return;}
                    if(rotateRight(p)) {checkRoot = true; return;}
                }
            }
            else {
                HuffTree *t = p->right;
                if(getLevel(t->right) >= getLevel(t->left)) rotateLeft(p);
                else {
                    if(rotateRight(p->right)) {checkRoot = true; return;}
                    if(rotateLeft(p)) {checkRoot = true; return;}
                }
            }
        }
        if(p->left) updateBalance(p->left, checkRoot);
        if(p->right) updateBalance(p->right, checkRoot); 
    }
    bool checkBalance(HuffTree *p) {
        if(!p) return true;
        if(abs(getLevel(p->left) - getLevel(p->right)) > 1) return false;
        return checkBalance(p->left) && checkBalance(p->right); 
    }
    bool createHuffTree(vector<pair<char, int>>& v) { //! trong qua trinh xoay cay neu root la char ->deleteTree
        for(auto it : v) vHuff.push_back(new HuffTree(it.first, it.second, true));
        while(vHuff.size()>= 2) {
            HuffTree *a = vHuff.back(); vHuff.pop_back();
            HuffTree *b = vHuff.back(); vHuff.pop_back();
            HuffTree *c  = new HuffTree(a, b);
            bool checkRoot = false;
            root = c;
            while(!checkBalance(c)) updateBalance(c, checkRoot);
            if(checkRoot) return false;
            for(auto i = 0; i < vHuff.size(); i++) {
                if(vHuff[i]->freq <= c->freq) {
                    vHuff.insert(vHuff.begin() + i, c);
                    break;
                }
            }
            if(!vHuff.size()) vHuff.push_back(c);
            //printQueueHuff(vHuff);
        }
        root = vHuff.back();
        vHuff.pop_back();
        return true;
    }
    void printQueueHuff(vector<HuffTree*> vHuff) {
        cout << "QueueHuff: " << endl;
        for (auto it : vHuff) cout << it->value << " " << it->freq << endl;
    }
    void printHuffTree() {
        if(!root) return;
        queue<HuffTree*> q;
		q.push(root);
		while(!q.empty()) {
			HuffTree *t = q.front(); q.pop();
			if(t->value) {
                char temp = t->value;
                // if(temp <= 'Z') temp = (temp - t->freq - 65) % 26 + 65;
                // else temp = (temp - t->freq - 97) % 26 + 97;
                cout << temp << "-" << t->freq << endl;
            }
			if(t->left) q.push(t->left);    
			if(t->right) q.push(t->right);
		}
    }

    string encodeBinaryName(const char x) {
        if(!root) return "";
        stack<string> s;
        stack<HuffTree *> t;
        t.push(root);
        while(!t.empty()) {
            HuffTree *temp = t.top(); t.pop();
            string st = ""; 
            if(!s.empty()) {
                st = s.top(); s.pop();
            }
            if(temp->value == x) return st;
            if(temp->left) {
                t.push(temp->left);
                s.push(st + "0");
            }
            if(temp->right) {
                t.push(temp->right);
                s.push(st + "1");
            }
        }
        return "";
    }
    int resultName(vector<pair<char, int>> &v) {
        vector<pair<string, int>> vBinary;
        for(auto it : v) {
            string t = encodeBinaryName(it.first);
            vBinary.push_back({t, it.second});
        }
        // cout << "Binary Name: " << endl;
        // for (auto it : vBinary) cout << it.first << " " << it.second << endl;
        string enBinary = "";
        for (auto it : vBinary) {
            //enBinary = it.first +"_"+ enBinary;
            for (int i = 0; i< it.second; i++) {
                if(enBinary.length()> 10) break;
                enBinary = it.first + enBinary;
            }
        }
        reverse(enBinary.begin(), enBinary.end());
        int decimal = bitset<10>(enBinary).to_ulong();
        cout << "Binary Name: " << enBinary <<" Decimal result: " << decimal << endl;
        return decimal;
    }
};
class Gojo {
    class BST{
        public:
        BST *left = nullptr;
        BST *right = nullptr;
        int data = 0;
        queue<BST *> qBST;
        friend class Gojo;
        public:
        BST(BST *left = nullptr, BST *right = nullptr, int data = 0) : left(left), right(right), data(data){};
        BST(int data) : data(data){};
        void insertBST(BST *&t, int result) {
            if(!t) {
                t = new BST(result);
                qBST.push(t);
                return;
            }
            if(result < t->data) insertBST(t->left, result);
            else insertBST(t->right, result);
        }
        void deleteNodeBST(BST *&t, int del) {
            if(!t) return;
            if(t->data == del) {
                if(t->left && t->right) {
                    BST *temp = t->right;
                    while(temp->left) temp = temp->left;
                    t->data = temp->data;
                    deleteNodeBST(t->right, temp->data);
                }
                else if(t->left) {BST *del = t; t = t->left; delete del; del = nullptr;}
                else if(t->right) {BST *del = t; t = t->right; delete del; del = nullptr;}
                else {delete t, t= nullptr;}
            }
            else if(del < t->data) deleteNodeBST(t->left, del);
            else deleteNodeBST(t->right, del);
        }
        void printBST(BST *t) {
            if(!t) return;
            printBST(t->left);
            cout << t->data << endl;
            printBST(t->right);            
        }
        void convertPos(BST *t, vector<int> &v) {
            if(!t) return;
            convertPos(t->left, v);
            convertPos(t->right, v);
            v.push_back(t->data);
        }
    };
    public:
    queue<HuffTree *> qHuff;
    vector<BST*> Areas; //? vector<BST*>:: Areas(MAXSIZE, nullptr);
    Gojo(int m) {
        for (int i = 0; i <= m; i++) Areas.push_back(nullptr);
    }

    // void backtrack(vector<int> &v, vector<vector<int>> &pmt, vector<int> &t, vector<bool> &vst) {
    //     if(t.size() == v.size()) {
    //         pmt.push_back(t);
    //         return;
    //     }
    //     for (int i = 0; i < v.size(); ++i) {
    //         if(!vst[i]) {
    //             t.push_back(v[i]);
    //             vst[i] = true;
    //             backtrack(v, pmt, t, vst);
    //             t.pop_back();
    //             vst[i] = false;
    //         }
    //     }
    // }
    // bool compareBST(BST* a, BST* b) {
    //     if(!a && !b) return true;
    //     if(!a || !b) return false;
    //     if(a->data != b->data) return false;
    //     return compareBST(a->left, b->left) && compareBST(a->right, b->right);
    // }
    void factory(int f[], int n) {
        f[0] = 1;
        for (long i = 1; i < n; i++) f[i] = f[i-1] * i;
    }
    int nCk(int f[], int n, int k) {
        if(k > n) return 0;
        int res = f[n]/ f[k];
        res /= f[n-k];
        return res;
    }
    int fac(int n) {
        if(n <= 1) return 1;
        return n * fac(n-1);
    }
    int countPmt(vector<int> &v, int f[]) {
        int n = v.size();
        if(n <= 2) return 1;
        vector<int> left, right;
        int root = v[0];
        for (int i = 1; i < n; i++) {
            if(v[i] < root) left.push_back(v[i]);
            else right.push_back(v[i]);
        }
        int l = left.size();
        //int r = right.size();
        int cntLeft = countPmt(left, f);
        int cntRight = countPmt(right, f);
        return nCk(f, n - 1, l) * cntLeft * cntRight;
    }
    int cntPermutation(BST *p) {
        vector<int> postList;
        p->convertPos(p, postList);
        cout << "Post List " << endl;
        for (auto it : postList) cout << it << " ";
        cout << endl; 
        // vector<vector<int>> permutation;
        // vector<bool> vst(postList.size(), false);
        // vector<int> crtPmt;
        // backtrack(postList, permutation, crtPmt, vst);
        // int cnt = 0;
        // for(vector<int> it : permutation) {
        //     if(it[0] == p->data) {
        //         BST *tempR = new BST(it[0]);
        //         for(auto i = 1; i< it.size(); i++) tempR->insertBST(tempR, it[i]);
        //         if(compareBST(p, tempR)) cnt++;
        //         for(int x: it) tempR->deleteNodeBST(tempR, x);             
        //     }
        // }
        reverse(postList.begin(), postList.end());
        set<int> m;
        for (int t : postList) {
            m.insert(t);
        }
        int s = m.size();
        int fact = fac(postList.size()- s);  
        cout << "Number of permutations : " << fact << endl;
        int n = postList.size();
        int f[n];
        factory(f, n);
        int c = countPmt(postList, f);
        cout << "Num Permutaion :" << c * fact << endl;
        return (c * fact) % MAXSIZE;
        //return cnt;
    }
    void deleteCusY(BST *p, int Y) { 
        while((Y--) && !(p->qBST.empty())) {
            BST *temp = p->qBST.front();
            p->qBST.pop();
            p->deleteNodeBST(p, temp->data);
        }
    }
    void insertGojo(HuffTree *p, int rel) {
        qHuff.push(p);
        int ID = rel % MAXSIZE + 1;
        if(!Areas[ID]) Areas[ID] = new BST(rel);
        else Areas[ID]->insertBST(Areas[ID], rel);
    }
    void printArea(int i) {
        Areas[i]->printBST(Areas[i]);
    }

    ~Gojo() {
        while(!qHuff.empty()) {
            qHuff.front()->freeHuffTree(); qHuff.pop();
        }
        for (BST *p : Areas) {
            queue<BST *> q; q.push(p);
            while(!q.empty()) {
                BST *temp = q.front(); q.pop();
                if(temp->left) q.push(temp->left);
                if(temp->right) q.push(temp->right);
                delete temp;
            }
        }
    }
};

class Sukuna{
    public:
    int order = 0;
    class Area {
        public:
        int label;
        queue<int> cus;
        int quantity;
        Area(int label, int quantity, int rel) : label(label), quantity(quantity){ cus.push(rel); };
        void insert(int rel) {
            this->quantity++;
            cus.push(rel);
        }
        void deleteCus(int n) {
            if(n >= quantity) n = quantity;
            this->quantity -= n;
            while(n-- && !cus.empty()) {
                cout << cus.front() << "-" << this->label<< endl;
                cus.pop();
            }
        }
    };
    //----------------------------------------
    vector<Area *> minHeap;
    vector<Area *> crtHeap;
    vector<Area *> orderAreas;
	queue<HuffTree *> qHuff;
    Sukuna(int m) { for (int i = 0; i <= m; i++) crtHeap.push_back(nullptr);}
    bool cmpOrder(Area *a, Area *b) {
        for (auto it : orderAreas) if(it == a) return true;
        return false;
    }
    void heapUp(vector<Area *> &heap, int idx) {
        while(idx) {
            int idxParent = (idx - 1) / 2;
            if(heap[idxParent]->quantity < heap[idx]->quantity) return;
            else if(heap[idxParent]->quantity == heap[idx]->quantity) if(cmpOrder(heap[idx], heap[idxParent])) return;
            swap(heap[idxParent], heap[idx]);
            idx = idxParent;
        }
    }
    void heapDown(vector<Area *> &heap, int idx, int n) {
        while(idx < n) {
            int cIdx = idx * 2 + 1;
            if(cIdx >= n) return;
            if(cIdx + 1 < n) {
                if(heap[cIdx]->quantity > heap[cIdx + 1]->quantity) cIdx++;
            }
            if(heap[cIdx]->quantity > heap[idx]->quantity) return; //! if equal then compare order insert
            else if(heap[cIdx]->quantity == heap[idx]->quantity) if(cmpOrder(heap[cIdx], heap[idx])) return;
            swap(heap[cIdx], heap[idx]);
            idx  = cIdx;   
        }
    }
    void deleteOrder(Area *del) {
        for (int i = 0; i < orderAreas.size(); ++i) {
        if (orderAreas[i] == del) {
            orderAreas.erase(orderAreas.begin() + i);
            --i; 
        }
    }

    }
    void insertCus(HuffTree *huff,int rel) {
		qHuff.push(huff);
        int ID = rel % MAXSIZE + 1;
        if(!crtHeap[ID]) {
            Area *heap = new Area(ID, 1, rel);
            minHeap.push_back(heap);
            crtHeap[ID] = heap;
        }
        else {
            crtHeap[ID]->insert(rel);
        }
        deleteOrder(crtHeap[ID]);
        orderAreas.push_back(crtHeap[ID]);
        for (int i = 0; i < minHeap.size(); i++) heapUp(minHeap, i);
    }

    Area *findMinLast() {
        Area *minlast = minHeap[0];
        for (Area * it : minHeap) {
            if(it->quantity < minlast->quantity) minlast = it;
            else if(it->quantity == minlast->quantity) {
                for (Area *i : orderAreas) {
                    if(i == it) {
                        minlast = it;
                        break;
                    }
                }
            }
        }
        return minlast;
    }
    void deleteCus(int num) {
        if(!minHeap.size()) return;
        int m = num;
        while(m--) {
            if(!minHeap.size()) return;
            Area *minlast = findMinLast();
            minlast->deleteCus(num);
            if(!minlast->quantity) {
                crtHeap[minlast->label] = nullptr;
                deleteOrder(minlast);
                int position = 0;
                for (int i = 0; i< minHeap.size(); i++) if(minHeap[i] == minlast) position = i;
                swap(minHeap[position], minHeap.back());
                Area *del = minHeap.back();
                minHeap.pop_back();
                delete del;
                cout << "SIZE : " << minHeap.size() << endl;
                heapDown(minHeap, position, minHeap.size());
            }
            else {
                deleteOrder(minlast);
                orderAreas.push_back(minlast);
                int position = 0;
                for (int i = 0; i< minHeap.size(); i++) if(minHeap[i] == minlast) position = i;
                heapUp(minHeap, position);
            }
        }
    }
    void printArea() {
        for (Area *it : minHeap) cout << it->label << " Quantity: " << it->quantity << endl; 
    }
    void printMinHeap(int idx, int n) {
        if(idx >= minHeap.size() || n <= 0) return;
        queue<int> q = minHeap[idx]->cus;
		while(n-- && !q.empty()) {
			cout <<"ID: " << q.front() << "- Result: " << minHeap[idx]->label << endl;
			q.pop();
		}
        printMinHeap(2 * idx + 1, n);
        printMinHeap(2 * idx + 2, n);
    }

    ~Sukuna() {
        while(!qHuff.empty()) {
            qHuff.front()->freeHuffTree(); qHuff.pop();
        }
        for (Area *p : minHeap) delete p;
    }
};

Sukuna *sukuna;
Gojo *gojo;

void LAPSE(string name) {
	vector<pair<char, int>> v = foldString(name);
	if(v.size() < 3) return;
	v = SortCodeCeasar(v);
    reverse(v.begin(), v.end());
	HuffTree *huff = new HuffTree();
	if(!huff->createHuffTree(v)) { delete huff; return;}
    //huff->printHuffTree();
	int result = huff->resultName(v);
	if(result % 2) sukuna->insertCus(huff ,result);
	else gojo->insertGojo(huff, result);
}
void KOKUSEN() {
	for (auto i = 0; i < gojo->Areas.size(); i++) {
		if (gojo->Areas[i]) {
			int n = gojo->cntPermutation(gojo->Areas[i]);
			gojo->deleteCusY(gojo->Areas[i], n);
		}
	}
}
void simulate(string filename)
{
	ifstream ss(filename);
	string str, name, num, maxsize;
	while(ss >> str) {
		if(str == "MAXSIZE") {
			ss >> maxsize;
			MAXSIZE = stoi(maxsize);
			sukuna = new Sukuna(MAXSIZE);
			gojo = new Gojo(MAXSIZE);
		}
		else if(str == "LAPSE") {
            cout << "----LAPSE----" << endl;
			ss >> name;
			LAPSE(name);
		}
		else if(str == "KOKUSEN") {
            cout << "----KOKUSEN----" << endl;
            KOKUSEN();
        }
		else if(str == "KEITEIKEN") {
            cout << "----KEITEIKEN-----" << endl;
			ss >> num;
			int n = stoi(num);
			sukuna->deleteCus(n);
		}
		else if(str == "HAND") { //! Print Char before char encode 
            cout << "----------HAND-----------" << endl;
			if(!gojo->qHuff.empty()) gojo->qHuff.front()->printHuffTree();
			if(!sukuna->qHuff.empty()) sukuna->qHuff.front()->printHuffTree();
            cout << endl;
		}
		else if(str == "LIMITLESS") {
			ss >> num; int n = stoi(num);
            cout << "----------LIMITLESS-----------" << n << endl;
			if(gojo->Areas[n]) gojo->Areas[n]->printBST(gojo->Areas[n]);
		}
		else {
            cout << "------CLEAVE--------" << endl;
			ss >> num; int n = stoi(num);
			sukuna->printMinHeap(0 ,n);
		}
	}
}