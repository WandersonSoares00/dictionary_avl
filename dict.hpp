
#ifndef DICTAVL_HPP
#define DICTAVL_HPP

#include <iostream>


template <typename TK, typename TV>
class Dictionary{

    struct Node{
        TK key;  TV value;
        struct Node *left, *right, *parent;
        int h;

        Node () : left(nullptr), right(nullptr), parent(nullptr), h(0) {}
        Node (TK c, TV v, Node* e, Node* d, Node* p, int hh) : key{c}, value{v}, left{e}, right{d}, parent{p}, h(hh) {}

        Node* get_right ()    {  return right; }

        Node* get_left ()     {  return left; }

        Node* get_parent ()   {  return parent; }
    };

    Node sent;
    Node *root;
    Node *temp;

    Node* rotateRight(Node* y){
        int hleft, hright;
        Node *X  = y -> left;
        Node *A2 = X -> right;
    /*      y                   X  
           / \                 / \
          X   yd   ---->      Xe  y
         / \                     / \
        Xe  A2                 A2   yd
    */
        X  -> right  = y;
        X  -> parent = y -> parent;
        y  -> parent = X;
        y  -> left   = A2;
        if (A2 != &sent)
            A2 -> parent = y;

        hleft = y -> left -> h;    hright = y -> right -> h;
        y -> h = std::max(hleft, hright) + 1;
        
        hleft = X -> left -> h;    hright = X -> right -> h;
        X -> h = std::max(hleft, hright) + 1;
        return X;
    }

    Node* rotateLeft(Node* y){
        int hleft, hright;
        Node *X  = y -> right;
        Node *A2 = X -> left;
    /*      y                   X  
           / \                 / \
         ye   X   ---->       y  Xd
             / \             / \  
            A2 Xd           ye  A2 
    */
        X  -> left   = y;
        X  -> parent = y -> parent;
        y  -> parent = X;
        y  -> right  = A2;
        if (A2 != &sent)
            A2 -> parent = y;

        hleft = y -> left -> h;    hright = y -> right -> h;
        y -> h = std::max(hleft, hright) + 1;

        hleft = X -> left -> h;    hright = X -> right -> h;
        X -> h = std::max(hleft, hright) + 1;
        return X;
    }

    inline int factor (Node* r){
        if (r == &sent)     return 0;
        return (r -> left -> h) - (r -> right -> h);
    }
    
    Node* balance (Node* r){
        int diff_r  = factor (r);
        int diff_rl = factor (r->left);
        
        if (diff_r > 1){
            if (diff_rl >= 0)   // Reta à leftuerda
                return rotateRight (r);
            if (diff_rl < 0){   // "Joelho" à leftuerda
                r->left = rotateLeft (r->left);
                return   rotateRight (r);
            }
        }
        
        int diff_rr = factor (r->right);

        if (diff_r < -1){
            if (diff_rr <= 0)   // Reta à righteita
                return rotateLeft (r);
            if (diff_rr  > 0){  // "Joelho" à righteita
                r -> right = rotateRight (r->right);
                return     rotateLeft (r);
            }
        }

        return r;
    }

    public:

    class Iterator{
        Node *no;

        friend Dictionary;

        Iterator (Node* n) : no{n} {}

        public:

        Iterator () {}
        
        bool operator != (Iterator j)  {
            return no != j.no;
        }

        bool operator == (Iterator j){
            return no == j.no;
        }

        TK key (){
            return no -> key;
        }

        TV value (){
            return no -> value;
        }

        Iterator& operator ++ (){
            if (no -> right -> right != nullptr){
                no = no -> right;
                while (no -> left -> left != nullptr)
                    no = no -> left;
            }
            else{
                Node* p = no -> parent;
                while (p -> parent != nullptr and no == p -> right){
                    no = p;
                    p = p -> parent;
                }
                no = p;
            }

            return *this;
        }

        Iterator& operator ++ (int){
            temp = no;

            if (no -> right -> right != nullptr){
                no = no -> right;
                while (no -> left -> left != nullptr)
                    no = no -> left;
            }
            else{
                Node* p = no -> parent;
                while (p -> parent != nullptr and no == p -> right){
                    no = p;
                    p = p -> parent;
                }
                no = p;
            }

            return Iterator (temp);
        }


    };

    Dictionary (){
        sent.h = 0;
        sent.left = sent.right = sent.parent = nullptr;
        root = temp = &sent;
    }

    ~Dictionary (){
        if (root != &sent){
            Iterator it;
            while (root -> left != &sent){
                root = root -> left;
            }
            
            it.no = root;
            while (it.no != &sent){
                temp = it.no;
                ++it;
                delete temp;
            }
        }
    }

    Iterator begin (){
        if (root == &sent)     return Iterator (&sent);

        temp = root;

        while (temp -> left != &sent){
            temp = temp -> left;
        }
        
        return  Iterator(temp);
    }

    Iterator end (){
        return Iterator(&sent);
    }
    
    Iterator insert (TK c, TV v){
        Node *curr, *next = root;
        
        if (next == &sent){
            root = new Node(c, v, &sent, &sent, &sent, 1);
            return Iterator(root);
        }

        while (next != &sent){
            curr = next;
            if (c < next -> key)          next = next -> left;
            else if (c > next -> key)     next = next -> right;
            else                          return Iterator(next);
        }

        Node *filho = new Node(c, v, &sent, &sent, curr, 1);

        if (c < curr -> key){
            curr -> left = filho;
        }
        else{
            curr -> right = filho;
        }

        while (true){
            curr -> h = std::max(curr -> left -> h, curr -> right -> h) + 1;
            if (curr == root){
                root = balance (curr); break;  }
            temp = curr -> parent;

            if (curr == temp -> left)
                temp -> left = balance (curr);
            else
                temp -> right = balance (curr);
            curr = temp;
        }

        return Iterator(filho);
    }

    Iterator search (TK c){
        
        temp = root;
        sent.key = c;

        while (true){
            if (temp -> key == c)
                return Iterator (temp);
            if (c < temp -> key)
                temp = temp -> left;
            else
                temp = temp -> right;
        }
    }

    void remove (Iterator i){
        if (i.no == &sent)
            return;
        temp = i.no;
        Node *aux;

        if (temp -> left == &sent and temp -> right == &sent){
            if (temp != root){
                aux = temp -> parent;
                if (aux -> left == temp) aux -> left = &sent;
                else                    aux -> right = &sent;
            }
            else
                root = &sent;
            delete temp;
        }
        else if (temp -> left != &sent and temp -> right != &sent){
            ++i;
            aux = i.no;

            if (aux -> right != &sent)        aux-> right -> parent = aux -> parent;
            if (aux -> parent -> left == aux)   aux-> parent -> left = aux -> right;
            else                            aux-> parent -> right = aux -> right;
            
            Node* p_aux = aux -> parent;
            if (p_aux == temp)    p_aux = aux;

            if (temp == root)
                root = aux;
            else{
                if (temp -> parent -> left == temp)  temp -> parent -> left = aux;
                else                             temp -> parent -> right = aux;
            }
            
            aux -> parent = temp -> parent;
            aux -> left = temp -> left;
            aux -> right = temp -> right;

            if (aux -> right != &sent)    aux -> right -> parent = aux;
            if (aux -> left != &sent)    aux -> left -> parent = aux;

            aux = p_aux;
            
            delete temp;
        }
        else{
            if (temp -> left == &sent){
                aux = temp -> right;
                if (temp == root)   root = aux;
                else{
                    if (temp -> parent -> left == temp) temp -> parent -> left = aux;
                    else                            temp -> parent -> right = aux;
                }
            }
            else{
                aux = temp -> left;
                if (temp == root)   root = aux;
                else{
                    if (temp -> parent -> left == temp) temp -> parent -> left = aux;
                    else                            temp -> parent -> right = aux;
                }
            }

            aux -> parent = temp -> parent;
            delete temp;
        }
        if (root == &sent)
            return;
        
        while (true){
            aux -> h = std::max(aux -> left -> h, aux -> right -> h) + 1;
            if (aux == root){
                root = balance (aux); break;  }
            temp = aux -> parent;

            if (aux == temp -> left)
                temp -> left = balance (aux);
            else{
                temp -> right = balance (aux);}
            
            aux = temp;
        }
    }

    void remove (TK c){
        Iterator target = search (c);
        if (target.no != &sent)
            remove (target);
    }


};


#endif
