#ifndef EUROVISION_H_
#define EUROVISION_H_

#include <iostream>
#include<algorithm>
#include<vector>
#include<cstring>
using std::ostream;
using std::vector;
using std::cerr;
using std::endl;
using std::cout;

// it's allowed to define here any using statements, according to needs.
// do NOT define here : using namespace std;

//---------------------------------------------------

enum VoterType { All, Regular, Judge };
enum Phase { Registration, Contest, Voting };

//---------------------------------------------------

class String {
    int length;
    char* data;
    static char* allocate_and_copy(const char* data, int size);
    void verify_index(int index) const;
public:
    class Bad_Index {};
    String(const char* str = ""); // String s1; or String s1 = "aa";
    String(const String& str); // String s2(s1);
    ~String();
    int size() const;
    String& operator=(const String&); // s1 = s2;
    String& operator+=(const String& str); // s1 += s2;
    const char& operator[](int index) const; // s[5] for const s
    char& operator[](int index); // s[5] for non-const s
    friend ostream& operator<<(ostream&, const String&); // cout << s1;
    friend bool operator==(const String&, const String&); // s1==s2
    friend bool operator<(const String&, const String&); // s1<s2
};
bool operator!=(const String& str1, const String& str2);
bool operator<=(const String& str1, const String& str2);
bool operator>(const String& str1, const String& str2);
bool operator>=(const String& str1, const String& str2);
String operator+(const String& str1, const String& str2);

class Participant
{
    String name;
    String _song;
    String _singer;
    int _length;
    bool registered;
    int regular_votes;
    int judge_votes;


public :
    explicit Participant(const String &name = "", const String &song = "", int songLength = 0, const String &singer  = "");

    Participant(const Participant& ) = delete;

    Participant operator=(const Participant& ) = delete;

    String state() const;

    String song() const;

    String singer() const;

    void updateRegistered(bool registered);

    bool isRegistered();

    int timeLength() const;

    int judgeVotes() const;

    int regularVotes() const;

    int getVotes(VoterType type=All) const;

    Participant& operator++(); //Increments Regular Votes

    Participant& operator+=(int amount); //Adds amount to judge votes

    void setName(const String& p_name);

    void update(const String &song, int length, const String &singer);

    ~Participant() = default;
};

ostream &operator<<(ostream &os, const Participant &p);

int getJudgeScore(int rank);


//---------------------------------------------------


class Voter {
    String _state; //Named vstate and vtype to not interfere with state() and voterType() functions
    VoterType _type;
    int votes;

public :

    explicit Voter(String state, VoterType type = Regular);

    VoterType voterType() const;

    String state() const;

    Voter &operator++(); //Votes number check is internal int operator+=(Vote)

    int timesOfVotes() { return votes; };
};


// -----------------------------------------------------------

struct Vote {
    String* votes;
    Voter& source;
    Vote(Voter& source,
            const String &target1,
            const String &target2 = "",
            const String &target3 = "",
            const String &target4 = "",
            const String &target5 = "",
            const String &target6 = "",
            const String &target7 = "",
            const String &target8 = "",
            const String &target9 = "",
            const String &target10 = "");
    ~Vote() {delete[] votes;}

};

ostream &operator<<(ostream &os, const Voter &v);
// -----------------------------------------------------------


class MainControl {
    Phase phase;
    Participant *participants{};
    int current_participants{};
    int max_length;
    int max_participants;
    int max_regular_votes;
    vector<Participant*> iterable_participants;

    static void participantsNameSort(String *names, int len);

    void registrationPrint(ostream &os) const;

    void contestPrint(ostream &os) const;


public :
    explicit MainControl(int max_length = 180, int max_participants = 26, int max_regular_votes = 5);

    class Iterator;

    void setPhase(const Phase &p);

    MainControl &operator+=(Participant &p);

    MainControl &operator+=(Vote v);

    MainControl &operator-=(Participant &p);

    bool participate(const String &name) const;

    bool legalParticipant(Participant &p) const;

    friend ostream &operator<<(std::ostream &os, const MainControl &eurovision);

    String operator()(int place, VoterType type);

    Iterator begin();

    Iterator end();

    ~MainControl();
};
// -----------------------------------------------------------
class MainControl::Iterator {
    MainControl* eurovision;
    int it;

    explicit Iterator(MainControl *e, int it) : eurovision(e), it(it) {};

    friend class MainControl;

public:
    Iterator() : eurovision(nullptr), it(0) {};

    Iterator& operator= (Iterator obj) {
        if (this == &obj)
            return *this;
        eurovision = obj.eurovision;
        it = obj.it;
        return *this;
    };

    Participant& operator*() {
        return *(eurovision->iterable_participants.at(it));
    }

    Iterator& operator++() {
        it++;
    }

    bool operator==(const Iterator &obj) {
        return (eurovision == obj.eurovision && it==obj.it);
    }

    bool operator<(const Iterator &obj) {
        return (eurovision == obj.eurovision && it< obj.it);
    }
};

// -----------------------------------------------------------

template<class Iterator, class Condition>
Iterator get(int i, Iterator begin, Iterator end, Condition condition);


#endif
