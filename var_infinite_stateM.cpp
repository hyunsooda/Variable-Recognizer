#include <iostream>
#include <vector>
#include <cctype>
#include <cassert>
using namespace std;

namespace Reserved {
    std::vector<std::string> keyword {"if", "else", "else if", "switch", "int", "float", "double", "char", "string"};
};

class MyException : public exception {
    public:
        const char* what () const throw () {
            return "error (not defined language)";
        }
};

class Machine {
    private:
        enum {
            STATE_A, STATE_B, STATE_C
        };
        std::string set;      // input string
        char start_state;
        char final_state;
        char current_state;
        bool is_passed;
    public:
        Machine(std::string input) : set(input), current_state(STATE_A), final_state(STATE_B) {};
        void transition(char) throw(MyException);
        bool process();
};

void Machine::transition(char ch) throw(MyException) {
    switch(current_state) {
        case STATE_A:
            cout << ch << endl;
            if(isalpha(ch)) {
                current_state = STATE_B;
            } else {
                current_state = STATE_C;
            }
        break;
        case STATE_B:
            cout << ch << endl;
            if(isalpha(ch) || isdigit(ch)) {
                current_state = STATE_B;
            } else {
                current_state = STATE_C;
            }
        break;
        case STATE_C:
            throw MyException();
        break;
    }
}
bool Machine::process()  {
    for(const auto& ch : set) {
        try {
            transition(ch);
        } catch(MyException& error) {
            cout << error.what() << endl;
            break;
        }
    }

    if(current_state == final_state) {
        for(const auto& kw : Reserved::keyword) {
            if(kw == set) {
                cout << "not passed(reserved keyword)" << endl;
                return false;
            }
        }
        cout << "passed" << endl;
        return true;
    } else {
        cout << "not passed" << endl;
        return false;
    }
}

int main() {
    Machine m {std::string("switch")};
    bool is_passed = m.process();
    assert(!is_passed);
    Machine m1 {std::string("if")};
    is_passed = m1.process();
    assert(!is_passed);
    Machine m2 {std::string("switch123")};
    is_passed = m2.process();
    assert(is_passed);
    Machine m3 {std::string("KNU")};
    is_passed = m3.process();
    assert(is_passed);
    Machine m4 {std::string("1KNU")};
    is_passed = m4.process();
    assert(!is_passed);
    Machine m5 {std::string("!@ss")};
    is_passed = m5.process();
    assert(!is_passed);
    Machine m6 {std::string("s1@@")};
    is_passed = m6.process();
    assert(!is_passed);
}