//
// Created by Ethan on 2/11/2020.
//
#include"eurovision.h"


/**@class String (for use instead of std::string)*/
void error(const char* str) {
    cerr << "Error: " << str << endl;
    exit(0);
}

char* String::allocate_and_copy(const char* str, int size) {
    return strcpy(new char[size + 1], str);
}

String::String(const char* str) {
    length = strlen(str);
    data = allocate_and_copy(str, length);
}

String::String(const String& str) :
        length(str.size()),
        data(allocate_and_copy(str.data, str.length)) {
}

String::~String() {
    delete[] data;
}

int String::size() const {
    return length;
}

String& String::operator=(const String& str) {
    if (this == &str) {
        return *this;
    }
    delete[] data;
    data = allocate_and_copy(str.data, str.length);
    length = str.length;
    return *this;
}

String& String::operator+=(const String& str) {
    char* new_data = allocate_and_copy(data, this->length + str.length);
    strcat(new_data, str.data);
    delete[] data;
    length += str.length;
    data = new_data;
    return *this;
}

void String::verify_index(int index) const {
    if (index >= size() || index < 0) {
        error("Bad index");
    }
    return;
}

const char& String::operator[](int index) const {
    verify_index(index);
    return data[index];
}

char& String::operator[](int index) {
    verify_index(index);
    return data[index];
}

bool operator==(const String& str1, const String& str2) {
    return strcmp(str1.data, str2.data) == 0;
}

ostream& operator<<(ostream& os, const String& str) {
    return os << str.data;
}

bool operator<(const String& str1, const String& str2) {
    return strcmp(str1.data, str2.data) < 0;
}

bool operator!=(const String& str1, const String& str2) { return !(str1 == str2); }
bool operator<=(const String& str1, const String& str2) { return !(str2 < str1); }
bool operator>(const String& str1, const String& str2) { return str2 < str1; }
bool operator>=(const String& str1, const String& str2) { return str2 <= str1; }
String operator+(const String& str1, const String& str2) { return String(str1) += str2; }



/**@class Participant*/

Participant::Participant(const String &name, const String &song, int songLength, const String &singer)
{
    this->name = name;
    p_song = song;
    p_length = songLength;
    p_singer = singer;
    registered = false;
    regular_votes = 0;
    judge_votes = 0;
}

String Participant::state() const {
    return name;
}

String Participant::song() const {
    return p_song;
}

String Participant::singer() const {
    return p_singer;
}

void Participant::updateRegistered(bool is_registered) {
    this->registered = is_registered;
}

bool Participant::isRegistered()
{
    return registered;
}

int Participant::timeLength() const {
    return p_length;
}

void Participant::update(const String &song, int length, const String &singer) {
    if (registered)
        return;
    p_song = song;
    p_singer = singer;
    p_length = length;
}

void Participant::setName(const String& p_name) {
    name = p_name;
}

int Participant::judgeVotes() const {
    return judge_votes;
}

int Participant::regularVotes() const {
    return regular_votes;
}

void Participant::setRegularVotes(int amount) {
    regular_votes = amount;
}

void Participant::setJudgeVotes(int amount) {
    judge_votes = amount;
}

ostream &operator<<(ostream &os, const Participant &p) {
    String song = p.song();
    String name = p.state();
    String singer = p.singer();
    int len = p.timeLength();
    return os << '['
              << name <<
              '/' << song <<
              '/' << len <<
              '/' << singer << ']';
}

/** @class Voter*/

Voter::Voter(String state, VoterType type) {
    v_state = state;
    v_type = type;
    votes = 0;
}

VoterType Voter::voterType()
{
    return v_type;
}

String Voter::state()
{
    return v_state;
}

Voter& Voter::operator++()
{
    votes++;
    return *this;
}

ostream &operator<<(ostream &os, const Voter &v) {
    // TODO
}

/** @struct Vote*/


Vote& Vote::operator++()
{
    ++votes_num;
    return *this;
}

#define TARGET_NUM(i) ("target_"#i)

Vote::Vote(Voter source, const String &target_1, const String &target_2, const String &target_3, const String &target_4,
           const String &target_5, const String &target_6, const String &target_7, const String &target_8,
           const String &target_9, const String &target_10) {

    this->targets = new String[10];
    for (int i=0;i<10;i++) {
        cout << "Testing targets: " << TARGET_NUM(i+1) << endl;
        this->targets[i] = TARGET_NUM(i+1);
    }
    this->source = source.state();
    this->type = source.voterType();
    this->votes_num =1;

}

/** @class MainControl */

MainControl::MainControl(int max_length, int max_participants, int max_regular_votes) {
    this->phase = Registration;
    this->max_length = max_length;
    this->max_participants = max_participants;
    this->max_regular_votes = max_regular_votes;
    this->current_participants = 0;
    this->participants = new Participant[max_participants];
}

MainControl &MainControl::operator+=(Vote v) {
    // TODO
}


MainControl &MainControl::operator+=(Participant &p) {
    if (this->phase != Registration)
        return *this;
    if (participate(p.state()))
        return *this;
    // reference is also updated for future use
    p.updateRegistered(true);
    for (int i=0;i<max_participants;i++) {
        Participant& p1 = participants[i];
        if (p1.state() == "") {
            p1.update(p.song(), p.timeLength(), p.singer());
            p1.setName(p.state());
            p1.updateRegistered(true);
            return *this;
        }
    }
//    current_participants++;
    return *this;
}

MainControl &MainControl::operator-=(Participant &p) {
    if (this->phase != Registration)
        return *this;
    if (!participate(p.state()))
        return *this;
    // reference is also updated for future use
    p.updateRegistered(false);
    for (int i = 0; i < current_participants; i++) {
        Participant& curr_p = participants[i];
        Participant& last = participants[current_participants];
        if (curr_p.state() == p.state()) {
            curr_p.setName("");
            curr_p.updateRegistered(false);
            return *this;
        }
    }
    current_participants--;
    return *this;
}


void MainControl::setPhase(const Phase &p) {
    if (p-phase != 1)
        return;
    phase = p;
}

bool MainControl::participate(const String &name) const {
    if (name == "")
        return false;
    for (int i = 0; i < max_participants; i++) {
        if (participants[i].state() == name)
            return true;
    }
    return false;
}

bool MainControl::legalParticipant(Participant &p) const {
    return p.song() != "" && p.state() != "" && p.timeLength() < max_length;
}


ostream &operator<<(ostream &os, const MainControl &eurovision) {
    os << '{' << endl;

    switch (eurovision.phase) {
        case Registration:
            os << "Registration" << endl;
            break;
        case Voting:
            os << "Voting" << endl;
            break;
        case Contest:
            os << "Contest" << endl;
            break;
    }

    for (int i = 0; i < eurovision.max_participants; i++) {
        Participant& p = eurovision.participants[i];
        if (p.state() == "")
            continue;
        os << p << endl;
    }

    os << '}' << endl;
    return os;
}
