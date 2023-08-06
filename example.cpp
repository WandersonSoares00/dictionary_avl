#include <exception>
#include <iostream>
#include "dict.hpp"

using namespace std;

int main (){
    try    {
        Dictionary<int,char> map;
        Dictionary<int,char>::Iterator its[10], it;
        int i;
        
        for (i = 48; i < 58; ++i){
            it = map.insert(i, (char) i);
            
            if (it == map.end())
                throw new logic_error ("Different iterator than expected on insert");
            
            its[i-48] = it;
        }

        for (it = map.begin(); it != map.end(); ++it){
            cout << "The code of " << it.value() << " is " << it.key() << '\n';
        }
        
        for (i = 48; i < 58; ++i){
            it = map.search(i);
            if (it != its[i-48])
                throw new logic_error ("Different iterator than expected on search");
            
            map.remove(it);
        }
        
    }
    
    catch (const exception &e){
        cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


