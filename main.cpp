#include <iostream>
#include <limits>
#include <vector>
#include <cassert>
#include <vector>
#include <exception>

using namespace std;

typedef unsigned long long val;
typedef val Denomination;

/*
  self memory releasing array
*/
struct valptr
{
    val * vp;
    valptr() : vp() {}
    ~valptr() { if (vp) delete [] vp; }
};

/*
  general solution table for solving dynamic programming problems
*/
class solution_table
{
    valptr stage[2];
    val capacity;
public:
    solution_table (val max_value) : capacity(max_value+1) {
        stage[0].vp = new val[capacity];
        stage[1].vp = new val[capacity];
    }

    inline val& operator() (val s, val i) {
        return stage[s].vp[i];
    }

    inline val array_size() {
        return capacity;
    }
};

/*
  returns number of ways to represent a value using dollars, dimes,
  nickels, and pennies
*/
inline val rest_of_five (val restfive)
{
    val restfive5 = restfive/5;
    val restfive10 = restfive5/2;
    val maxone = restfive10/10;

    return ((100*maxone - 15*(restfive5) + 20)*maxone
                + 3*(restfive5)*((restfive10) + 1)
                - 3*(restfive10)*(restfive10)
                + 3)*(maxone+1)/3;
}

/*
  returns number of ways to represent a value using the denominations
  $100, $50, $20, $10, $5, $1, $0.25, $0.10, $0.05 and $0.01

    time complexity - O(n)
    space complexity - O(n)
*/
val find_solution (val total_pennies)
{
    val total_nickels = total_pennies/5;
    solution_table solution (total_nickels);

    static val coin_value[6] = {500,1000,2000,5000,10000,25};
    static val coin_value5[6] = {100,200,400,1000,2000,5};

    for (val nickels=0; nickels<solution.array_size(); nickels++)
    {
        val pennies = 5*nickels;
        if (pennies >= coin_value[0])
            solution (0,nickels) = solution(0,nickels-100) + rest_of_five(pennies);
        else
            solution (0,nickels) = rest_of_five(pennies);
    }
    for (Denomination d=1; d<=5; d++)
    {
        val d1 = d % 2;
        val d2 = (d-1) % 2;
        val end = ( coin_value5[d]<solution.array_size() ? coin_value5[d] : solution.array_size() );
        for (val nickels=0; nickels<end; nickels++)
            solution (d1,nickels) = solution (d2,nickels);
        for (val nickels=coin_value5[d]; nickels<solution.array_size(); nickels++)
            solution (d1,nickels) = solution (d1,nickels-coin_value5[d]) + solution (d2,nickels);
    }
    return solution (5%2,total_nickels);
}

void test (val a, val b)
{
    cout << (a==b ? "T" : "F") << " " << a << " " << b   << endl;
}

int main()
{
    try{
        cout << numeric_limits<val>::max() << endl;
        test (find_solution(19821), 4091511866353);
        test (find_solution(15678), 848080222225);
        test (find_solution(1000), 474189);
        test (find_solution(100), 243);
        test (find_solution(30), 18);
        test (find_solution(20), 9);
        test (find_solution(10), 4);
        test (find_solution(5), 2);
        cout << find_solution(100000000) << endl;
    } catch (std::exception & e) {
        cout << e.what() << endl;
    }

    cout << "Hello World!" << endl;
    return 0;
}

