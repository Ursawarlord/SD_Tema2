#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
class Node
{
public:
    long long key;
    Node **forward;
    Node(long long key, long long level)
    {
        this->key = key;
        forward = new Node*[level+1];
        memset(forward, 0, sizeof(Node*)*(level+1));
    }
};

class SkipList {
private:
    long long MAXLVL;
    float P;
    long long level;
    Node *header;
    long long randomLevel()
    {
        float r = static_cast<float>(rand())/RAND_MAX;
        long long lvl = 0;
        while (r < P && lvl < MAXLVL)
        {
            lvl++;
            r = static_cast<float>(rand())/RAND_MAX;
        }
        return lvl;
    }
    Node* createNode(long long key, long long level)
    {
        Node *n = new Node(key, level);
        return n;
    }
public:
    SkipList(long long MAXLVL, float P)
    {
        this->MAXLVL = MAXLVL;
        this->P = P;
        level = 0;
        header = new Node(-1, MAXLVL);
    }
    void insertElement(long long key)
    {
        Node *current = header;
        Node **update;
        update = new Node*[MAXLVL+1];
        memset(update, 0, sizeof(Node *) * (MAXLVL + 1));
        for (long long i = level; i >= 0; i--) {
            while (current->forward[i] != NULL &&
                   current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }
        current = current->forward[0];
        if (current == NULL || current->key != key) {
            long long rlevel = randomLevel();
            if (rlevel > level) {
                for (long long i = level + 1; i < rlevel + 1; i++)
                    update[i] = header;
                level = rlevel;
            }
            Node *n = createNode(key, rlevel);
            for (long long i = 0; i <= rlevel; i++) {
                n->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = n;
            }
        }
        delete[] update;
    }
    void displayLevels()
    {
        for (long long i=0;i<=level;i++)
        {
            Node *node = header->forward[i];
            std::cout << "Level " << i << ": ";
            while (node != NULL)
            {
                std::cout << node->key<<" ";
                node = node->forward[i];
            }
            std::cout << "\n";
        }
    }
    long long elementLowerOrEqualTo(long long x)
    {
        long long max = -1;
        for(long long i=level;i>=0;i--)
        {
            Node *node = header->forward[i];
            while(node != NULL)
            {
                if(node->key == x)
                    return x;
                if(node->key > max && node->key < x)
                    max = node->key;
                node = node->forward[i];
            }
        }
        return max;
    }
    long long elementHigherOrEqualTo(long long x)
    {
        long long min = 99999;
        for(long long i=level;i>=0;i--)
        {
            Node *node = header->forward[i];
            while(node != NULL)
            {
                if(node->key == x)
                    return x;
                if(node->key < min && node->key > x)
                    min = node->key;
                node = node->forward[i];
            }
        }
        if(min == 99999)
            min = -1;
        return min;
    }
    void printElementsInBetween(std::ofstream &g, long long x, long long y)
    {
        std::vector<long long> vect;
        Node *node = header->forward[0];
        while(node != NULL)
        {
            if(node->key >= x && node->key <= y)
                vect.push_back(node->key);
            node = node->forward[0];
        }
        std::sort(vect.begin(),vect.end());
        for(auto &x : vect)
            g << x << " ";
        g << "\n";
    }

    void deleteElement(long long key)
    {
        Node *current = header;
        Node **update;
        update = new Node*[MAXLVL+1];
        memset(update, 0, sizeof(Node*)*(MAXLVL+1));
        for(long long i = level; i >= 0; i--)
        {
            while(current->forward[i] != NULL  &&
                  current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }
        current = current->forward[0];
        if(current != NULL && current->key == key)
        {
            for(long long i=0;i<=level;i++)
            {
                if(update[i]->forward[i] != current)
                    break;
                update[i]->forward[i] = current->forward[i];
            }
            while(level>0 &&
                  header->forward[level] == 0)
                level--;
        }
    }

    bool findElement(long long key)
    {
        Node *current = header;
        for(long long i = level; i >= 0; i--)
        {
            while(current->forward[i] &&
                  current->forward[i]->key < key)
                current = current->forward[i];
        }
        current = current->forward[0];
        if(current && current->key == key)
            return true;
        return false;
    }
};


int main()
{
    srand((unsigned)time(0));
    SkipList sk_lst(3, 0.5);
//    std::ifstream f("../abce.in");
//    std::ofstream g("../abce.out");
    std::ifstream f("abce.in");
    std::ofstream g("abce.out");
    long long Q;
    f >> Q;
    for(long long i=0;i<Q;i++)
    {
        long long op, x, y;
        f >> op;
        switch(op)
        {
            case 1:
                f >> x;
                sk_lst.insertElement(x);
                break;
            case 2:
                f >> x;
                sk_lst.deleteElement(x);
                break;
            case 3:
                f >> x;
                g << sk_lst.findElement(x) << "\n";
                break;
            case 4:
                f>>x;
                g << sk_lst.elementLowerOrEqualTo(x) << '\n';
                break;
            case 5:
                f>>x;
                g << sk_lst.elementHigherOrEqualTo(x) << '\n';
                break;
            case 6:
                f>>x>>y;
                sk_lst.printElementsInBetween(g,x,y));
                break;
        }
    };
    sk_lst.displayLevels();
}
