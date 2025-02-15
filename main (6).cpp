/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <vector>
#include <unordered_set>
#include <thread>
#include <mutex>
using namespace std;

//unordered_set <int> magicSet;
mutex magicMutex;

void isMagic(int start, int MAX, bool& magic);

const int MAX = 1000000;

int main()
{
    //thread stuff
    int numThreads = thread::hardware_concurrency();
    int range = MAX/numThreads;
    vector <thread> threads;

    bool magic = true;
    cout<<numThreads<<endl;
    for (int t = 0; t < numThreads; t++)
    {
        int start = t * range + 1;
        int end = (t == numThreads - 1) ? MAX : start + range - 1;
        threads.emplace_back(isMagic, start, end, std::ref(magic));
    }

    for (auto& thread : threads)
    {
        thread.join();
    }
    //isMagic(1,MAX,magic);

    //since magic is passed by reference, if any of the threads find a non-magic number, magic becomes false
    if (magic)
    {
        cout<<"All numbers from 1-"<<MAX<<" are Magic Numbers!"<<endl;
    }

    return 0;

}


void isMagic(int start, int MAX, bool& magic)
{
    unordered_set <int> magicSet;
    unordered_set <int> set;
    int test;
    for(int i = start; i <= MAX; i++)
    {
        //used to show what numbers have been reached.
        //This does add some slowdown but is good for testing
        if (i%100000 == 0)
        {
            cout<<i<<endl;
        }


        test = i;
        while (test>1)
        {
            if(magicSet.count(test))//checks if number matches any known magic numbers
            {
                goto label1;
            }

            //cout<<test<<endl;

            if((test&1) == 1)//same as test%2==1 but faster
            {
                test = test*3+1;
            }
            test>>=1; //same as /=2 but faster
            /*if((test&1) == 0) //same as test%2==0 but faster
            {
                test>>=1;//same as /2 but faster
            }
            else
            {
                test = test*3+1;

            }*/


            if (set.count(test)) //initially went through set as shown in the commented out sections below, but this is slightly faster.
            {
                cout<<i<<" is not a magic number"<<endl;
                magic = false;
                goto label2;
            }

            set.insert(test);//puts test into set

            /*for(int k : set)
            {
                if(i==k)
                {
                    cout<<i<<" is not a magic number"<<endl;
                    goto label2;
                }
            }*/
            /*
            if(!magic)
            {
                cout<<i<<" is not a magic number"<<endl;
                goto label;
            }
            */

        }
        label1:
        //lock_guard<mutex> lock(magicMutex);

        //magicSet.insert(set.begin(), set.end()); I thought this may be faster, but ended up slower than the for loop

        for (int i : set)//making this set shared between threads makes it take like twice as long.
        {
            magicSet.insert(i);//inserts newly found magic number and every number encountered on the way to 1
        }

        //magicSet.merge(set); //does not work on this version of c++

        set.clear();
        //cout<<endl;

    }
    label2:
    //return magic;
    return;
}
