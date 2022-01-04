#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

string ltrim(const string&);
string rtrim(const string&);
vector<string> split(const string&);

void recursiveFunction(vector<int>& a, int current_location, int current_chain, int current_number, int current_skip, int& best_chain)
{
    for (int i = current_location; i < a.size(); i++)
    {
        //check to see if the current max chain can be beat from current location, if not
        //then no need to go on
        if (current_chain + (a.size() - i) <= best_chain) return;

        cout << a[i] << ", " << current_number << ", ";
        //if the number can be used, then use it
        if (a[i] == current_skip)
        {
            //don't start a chain with a number that has already been tested. This is a time saving optimization
            cout << "skip" << endl;
            continue;
        }
        if (current_chain == 0)
        {
            //any number can be used when the chain has no members
            cout << "yo" << endl;
            recursiveFunction(a, i + 1, 1, a[i], current_skip, best_chain);
        }
        else
        {
            if ((a[i] - current_number) <= 1 && (a[i] - current_number) >= -1)
            {
                cout << "yo" << endl;
                recursiveFunction(a, i + 1, current_chain + 1, a[i], current_skip, best_chain);
            }
            else cout << endl;
        }
        current_skip = i;
    }

    if (current_chain > best_chain) best_chain = current_chain;
    cout << "Chain length = " << current_chain << endl;

    return;
}

int pickingNumbers(vector<int> a)
{
    int answer = 0;
    recursiveFunction(a, 0, 0, 0, -1, answer);

    return answer;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string n_temp;
    getline(cin, n_temp);

    int n = stoi(ltrim(rtrim(n_temp)));

    string a_temp_temp;
    getline(cin, a_temp_temp);

    vector<string> a_temp = split(rtrim(a_temp_temp));

    vector<int> a(n);

    for (int i = 0; i < n; i++) {
        int a_item = stoi(a_temp[i]);

        a[i] = a_item;
    }

    int result = pickingNumbers(a);

    fout << result << "\n";

    fout.close();

    return 0;
}

string ltrim(const string& str) {
    string s(str);

    s.erase(
        s.begin(),
        find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace)))
    );

    return s;
}

string rtrim(const string& str) {
    string s(str);

    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );

    return s;
}

vector<string> split(const string& str) {
    vector<string> tokens;

    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));

        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
