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
    Participant(String name, String song, int songLength, String singer );

    Participant(const Participant& ) = delete;

    Participant operator=(const Participant& ) = delete;

    String state();

    String song();

    String singer();

    void updateRegistered(bool registered);

    bool isRegistered();

    int timeLength();

    void update(String song, int length ,  String singer);


    ~Participant() = default;
};

//---------------------------------------------------


class Voter
{
    String v_state; //Named vstate and vtype to not interfere with state() and voterType() functions
    VoterType v_type;
    int votes;

public :

    explicit Voter(String state, VoterType type = Regular);

    VoterType voterType();

    String state();

    Voter& operator++(); //Votes number check is internal int operator+=(Vote)

};


// -----------------------------------------------------------

struct Vote
{
    String source;
    VoterType type;
    String target;
    int votes_num;

    Vote(Voter source, String target);
    Vote& operator++(); //Adds a vote to numOfVotes (reduces number of structs needed to store votes)
    ~Vote ()= default;

};

// -----------------------------------------------------------


class MainControl
{
    Phase phase;
    Participant*  participants;
    Voter* voters;
    Vote* votes;
    int max_length;
    int max_participants;
    int max_regular_votes;


public :
    MainControl(int max_length = 180, int max_participants = 26, int max_regular_votes = 5);
    void setPhase(Phase phase1);

};

// -----------------------------------------------------------

#endif
