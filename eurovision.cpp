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

Participant::Participant(String name, String song, int songLength, String singer)
{
    this->name = name;
    p_song = song;
    p_length = songLength;
    p_singer = singer;
    registered = false;
}

String Participant::state()
{
    return name;
}

String Participant::song()
{
    return p_song;
}

String Participant::singer()
{
    return p_singer;
}

void Participant::updateRegistered(bool registered)
{
    this->registered = registered;
}

bool Participant::isRegistered()
{
    return registered;
}

int Participant::timeLength()
{
    return p_length;
}

void Participant::update(String song, int length, String singer) {
    if (registered)
        return;
    p_song = song;
    p_singer = singer;
    p_length = length;
}

ostream &operator<<(ostream &os, const Participant &p) {
    // TODO print the stats
    return os;
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

Vote::Vote(Voter source, String target)
{
    this->source = source.state();
    this->type = source.voterType();
    this->target = target;
    this->votes_num =1;
}

Vote& Vote::operator++()
{
    ++votes_num;
    return *this;
}

/** @class MainControl */

MainControl::MainControl(int max_length, int max_participants, int max_regular_votes) {
    this->phase = Registration;
    this->max_length = max_length;
    this->max_participants = max_participants;
    this->max_regular_votes = max_regular_votes;
}

MainControl &MainControl::operator+=(Vote v) {
    // TODO
}

MainControl &MainControl::operator+=(Participant &p) {
    // TODO
}

MainControl &MainControl::operator-=(Participant &p) {
    // TODO
}


void MainControl::setPhase(const Phase &p) {
    phase = p;
}

bool MainControl::participate(const String &name) const {
    // TODO return if the participate exists
}

bool MainControl::legalParticipant(Participant &p) const {
    // TODO
}

ostream &operator<<(ostream &os, const MainControl &eurovision) {
    // TODO print the stats
    return os;
}