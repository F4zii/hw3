#ifndef EUROVISION_H_
#define EUROVISION_H_

#include <iostream>
#include<cstring>
using std::ostream;
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
    String p_song;
    String p_singer;
    int p_length;
    bool registered;

public :
    Participant(const String &name, const String &song, int songLength, const String &singer);

    Participant(const Participant& ) = delete;

    Participant operator=(const Participant& ) = delete;

    String state() const;

    String song() const;

    String singer() const;

    void updateRegistered(bool registered);

    bool isRegistered();

    int timeLength() const;

    void update(const String &song, int length, const String &singer);

    ~Participant() = default;
};

ostream &operator<<(ostream &os, const Participant &p);


//---------------------------------------------------


class Voter {
    String v_state; //Named vstate and vtype to not interfere with state() and voterType() functions
    VoterType v_type;
    int votes;

public :

    explicit Voter(String state, VoterType type = Regular);

    VoterType voterType();

    String state();

    Voter &operator++(); //Votes number check is internal int operator+=(Vote)

    int timesOfVotes() { return votes; };
};


// -----------------------------------------------------------

struct Vote {
    String source;
    VoterType type;
    String *targets;
    int votes_num;

    explicit Vote(Voter source,
            const String &target1 = "",
            const String &target2 = "",
            const String &target3 = "",
            const String &target4 = "",
            const String &target5 = "",
            const String &target6 = "",
            const String &target7 = "",
            const String &target8 = "",
            const String &target9 = "",
            const String &target10 = ""
            );

    Vote &operator++(); //Adds a vote to numOfVotes (reduces number of structs needed to store votes)
    ~Vote() = default;

};

ostream &operator<<(ostream &os, const Voter &v);
// -----------------------------------------------------------


class MainControl {
    Phase phase;
    Participant *participants{};
    Vote *votes{};
    int max_length;
    int max_participants;
    int max_regular_votes;


public :
    explicit MainControl(int max_length = 180, int max_participants = 26, int max_regular_votes = 5);

    void setPhase(const Phase &p);

    MainControl &operator+=(Participant &p);

    MainControl &operator+=(Vote v);

    MainControl &operator-=(Participant &p);

    bool participate(const String &name) const;

    bool legalParticipant(Participant &p) const;

    friend ostream &operator<<(std::ostream &os, const MainControl &eurovision);
};

// -----------------------------------------------------------

#endif
