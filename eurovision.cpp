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
    _song = song;
    _length = songLength;
    _singer = singer;
    registered = false;
    regular_votes = 0;
    judge_votes = 0;
}

String Participant::state() const {
    return name;
}

String Participant::song() const {
    return _song;
}

String Participant::singer() const {
    return _singer;
}

void Participant::updateRegistered(bool is_registered) {
    this->registered = is_registered;
}

bool Participant::isRegistered()
{
    return registered;
}

int Participant::timeLength() const {
    return _length;
}

void Participant::update(const String &song, int length, const String &singer) {
    if (registered)
        return;
    if (song!="")
        _song = song;
    if (singer!="")
        _singer = singer;
    if(length!=0)
        _length = length;
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

Participant &Participant::operator++() { //Regular Votes Increment
    regular_votes++;
    return *this;
}


Participant &Participant::operator+=(int amount) {
    judge_votes+=amount;
    return *this;
}

int Participant::getVotes(VoterType type) const {
    switch (type)
    {
        case Regular:
            return regular_votes;
        case Judge:
            return judge_votes;
        default:
            return regular_votes+judge_votes;
    }
}

int getJudgeScore(int rank) {
    if (rank>10 || rank<0)
        return 0;
    else if (rank==0)
        return 12;
    else if (rank==1)
        return 10;
    else
        return 10-rank;
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
    _state = state;
    _type = type;
    votes = 0;
}

VoterType Voter::voterType() const
{
    return _type;
}

String Voter::state() const
{
    return _state;
}

Voter& Voter::operator++()
{
    votes++;
    return *this;
}

ostream &operator<<(ostream &os, const Voter &v) {
    os<<'<'<<v.state()<<'/';
    if (v.voterType() == Regular)
        os<<"Regular>";
    else
        os<<"Judge>";
    return os;
}

/** @struct Vote*/
Vote::Vote(Voter& source, const String &target1, const String &target2, const String &target3, const String &target4,
           const String &target5, const String &target6, const String &target7, const String &target8,
           const String &target9, const String &target10): source(source), votes (new String[10]) {
    votes[0]=target1;
    votes[1]=target2;
    votes[2]=target3;
    votes[3]=target4;
    votes[4]=target5;
    votes[5]=target6;
    votes[6]=target7;
    votes[7]=target8;
    votes[8]=target9;
    votes[9]=target10;
}





/** @class MainControl */

MainControl::MainControl(int max_length, int max_participants, int max_regular_votes): iterable_participants() {
    this->phase = Registration;
    this->max_length = max_length;
    this->max_participants = max_participants;
    this->max_regular_votes = max_regular_votes;
    this->current_participants = 0;
    this->participants = new Participant[max_participants];
}

MainControl &MainControl::operator+=(Vote v) {
    if (!participate(v.source.state())) //Checks if votes comes from registered state
        return *this;
    if (v.source.timesOfVotes()>=max_regular_votes || v.source.timesOfVotes()<=-1) //Checks if regular voter voted less than allowed
        return *this;
    if (v.source.voterType()==Judge && v.source.timesOfVotes()==-1)
        return *this;
    for (int i=0;i<10;i++)
    {
        if (v.votes[i]=="") //Checks if end of votes has been reached
            return *this;
        if (v.source.voterType()==Regular && i>0) //Prevents regular voters from voting more than once
            return *this;
        if (v.votes[i]==v.source.state()) //Checks if voter voted for their own state
            return *this;
        if(!participate(v.votes[i])) //Checks that voted state is registered
            return *this;
        ++v.source;
        for(int j=0;j<max_participants;j++)
        {
            if (participants[j].state()==v.votes[i]) //Find state which was voted for
            {
                v.source.voterType() == Regular ? ++participants[j] : participants[j]+=getJudgeScore(i); //Increments participant votes
//                cout<<"Testing voter from "<<v.source.state()<<": Num of votes: "<<v.source.timesOfVotes()<<endl;
            }
        }
    }
}

struct compareParticipantsByName{
    bool operator()(Participant* p1, Participant* p2){
        return p1->state() < p2->state();
    }
};


MainControl &MainControl::operator+=(Participant &p) {
    if (this->phase != Registration)
        return *this;
    if (participate(p.state()))
        return *this;
    if (!legalParticipant(p))
        return *this;
    // reference is also updated for future use
    p.updateRegistered(true);
    iterable_participants.push_back(&p);
    std::sort(iterable_participants.begin(),iterable_participants.end(),compareParticipantsByName());
    auto it = std::unique(iterable_participants.begin(),iterable_participants.end());
    iterable_participants.resize(std::distance(iterable_participants.begin(),it));
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
    for (int i = 0; i < max_participants; i++) {
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
    return p.song() != "" && p.state() != "" && p.timeLength() <= max_length;
}

void MainControl::participantsNameSort(String* names, int len)
{
    for(int i=0; i<len-1;i++)
    {
        for(int j=0;j<len-i-1;j++)
        {
            if(names[j]> names[j+1])
            {
                String temp = names[j];
                names[j] = names[j+1];
                names[j+1] = temp;
            }
        }
    }
}

void MainControl::registrationPrint(ostream &os) const
{
    auto* names = new String[max_participants];
    for(int i=0;i<max_participants;i++)
    {
        if (participants[i].state()=="")
            continue;
        names[i] = participants[i].state();
    }
    participantsNameSort(names,max_participants);
    for(int i=0;i<max_participants;i++)
    {
        if(names[i]=="")
            continue;
        for(int j=0;j<max_participants;j++)
        {
            if (names[i]==participants[j].state())
                os<<participants[j]<<endl;
        }
    }
    delete[] names;
}

void MainControl::contestPrint(ostream &os) const
{
    auto* names = new String[max_participants];
    for(int i=0;i<max_participants;i++)
    {
        if (participants[i].state()=="")
            continue;
        names[i] = participants[i].state();
    }
    participantsNameSort(names,max_participants);
    for(int i=0;i<max_participants;i++)
    {
        if(names[i]=="")
            continue;
        for(int j=0;j<max_participants;j++)
        {
            if (names[i]==participants[j].state())
            {
                os<<participants[j].state()<<" : Regular("<<participants[j].regularVotes()
                <<") Judge("<<participants[j].judgeVotes()<<")"<<endl;
            }

        }
    }
    delete[] names;
}

MainControl::~MainControl()
{
    delete[] participants;
}


ostream &operator<<(ostream &os, const MainControl &eurovision) {
    os << '{' << endl;

    switch (eurovision.phase) {
        case Registration:
            os << "Registration" << endl;
            eurovision.registrationPrint(os);
            break;
        case Voting:
            os << "Voting" << endl;
            eurovision.contestPrint(os);
            break;
        case Contest:
            os << "Contest" << endl;
            eurovision.contestPrint(os);
            break;
    }
    os << '}' << endl;
    return os;
}

struct ParticipantsCompare{
    VoterType type;
    explicit ParticipantsCompare(VoterType type): type(type) {};
    bool operator() (Participant* p1, Participant* p2) {
        if (p1->getVotes(type) == p2->getVotes(type))
            return p1->state() > p2->state();
        return p1->getVotes(type) > p2->getVotes(type);
    }
};

String MainControl::operator()(int place, VoterType type) {
    vector<Participant*> sorted_participants;
    for(int i=0;i<max_participants;i++)
    {
        if (participants[i].state()!="")
            sorted_participants.push_back(&participants[i]);
    }
//    auto it = std::unique(sorted_participants.begin(),sorted_participants.end());
//    sorted_participants.resize(std::distance(sorted_participants.begin(),it));
    auto p = get(place,sorted_participants.begin(),sorted_participants.end(),ParticipantsCompare(type));
    if (p==sorted_participants.end())
        return "";
    return (*p)->state();
}

MainControl::Iterator MainControl::begin() {
    return Iterator(this,0);
}

MainControl::Iterator MainControl::end(){
    return Iterator(this,std::distance(iterable_participants.begin(),iterable_participants.end()));
}


template<class Iterator, class Predicate>
Iterator get(int i, Iterator begin, Iterator end, Predicate predicate)
{
    if(i<=0) //Checks if index is positive
        return end;
    if (i>std::distance(begin,end))
        return end;
    vector<Iterator> maxElements;
    for(int j=0; j<i;j++)
    {
        Iterator curr_max = begin;
        for(auto k = begin; !(k==end);++k)
        {
            if (predicate(*k,*curr_max) && std::find(maxElements.begin(),maxElements.end(),k) == maxElements.end())
            {
                curr_max = k;
            }
        }
        maxElements.push_back(curr_max);
    }
    return maxElements.at(maxElements.size()-1);
}




