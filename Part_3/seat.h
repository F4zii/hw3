#include <iostream>
#include <string>
using std::string;
using std::exception;


//----------------------------------------------
class NoPrice : public std::exception
{
    virtual char const* what() const noexcept override { return "Not For Sale !"; }
};

// ---------------------------------------------
class Seat
{
protected:
    int row_num;
    int seat_num;
    int _price;
    Seat(int row_num, int num, int price) : row_num(row_num) , seat_num(num), _price(price) {};
public:
    virtual int price()=0;
    virtual string location() { return "line: "+std::to_string(row_num)
                                       +", chair: "+std::to_string(seat_num); };
    virtual ~Seat() = default;
};



// ---------------------------------------------
class MainHallSeat : public Seat
{
protected:
    const static int mh_price_addition= 100;
    MainHallSeat(int row_num, int num, int price): Seat(row_num,num,price+mh_price_addition){};
public:
    virtual ~MainHallSeat()  = default;
};

// ---------------------------------------------
class SpecialSeat : public MainHallSeat
{
    const static int sps_price_addition = 300;
protected:
    SpecialSeat(int row_num, int num, int price) : MainHallSeat(row_num,num,price+sps_price_addition) {};
    virtual ~SpecialSeat() = default;
};

// ---------------------------------------------
class GreenRoomSeat: public Seat
{
public:
    GreenRoomSeat(int row_num, int num): Seat(row_num,num,0){} ; //sets price to 0 for constructor
    int price() override{ throw NoPrice(); }
    string location() override{ return "Green Room-> "+Seat::location(); }
    ~GreenRoomSeat() override = default;
};

// ---------------------------------------------
class GoldenCircleSeat: public SpecialSeat
{
    const static int gcs_price_addition = 1000;
public:
    GoldenCircleSeat(int row_num, int num, int price): SpecialSeat(row_num,num,price+gcs_price_addition) {};
    int price() override { return _price; }
    string location() override { return "Golden Circle-> "+Seat::location(); }
    ~GoldenCircleSeat() override = default;
};

// ---------------------------------------------
class DisablePodiumSeat:public SpecialSeat
{
    const static int dps_price = 200;
public:
    DisablePodiumSeat(int row_num,int num,int price = dps_price): SpecialSeat(row_num,num,price) {};
    int price() override { return dps_price; }
    string location() override { return "Disable Podium-> "+Seat::location(); }
    ~DisablePodiumSeat() override = default;
};

// ---------------------------------------------
class RegularSeat: public MainHallSeat
{
protected:
    char section;
    RegularSeat(char section, int row_num, int num, int price):
        MainHallSeat(row_num,num,price), section(section) {};
    string location() override { return "area: "+string(1,section)+", "+Seat::location(); }
    virtual ~RegularSeat() = default;
};

// ---------------------------------------------
class FrontRegularSeat: public RegularSeat
{
    const static int frs_price_addition=500;
public:
    FrontRegularSeat(char section, int row, int num, int price) :
        RegularSeat(section,row,num,price+frs_price_addition) {};
    int price() override { return _price; }
    string location() final {
        return "Front-> "+RegularSeat::location();
    }
    ~FrontRegularSeat() override= default;
};

// ---------------------------------------------
class MiddleRegularSeat: public RegularSeat
{
    const static int mrs_price_addition = 250;
public:
    MiddleRegularSeat(char section, int row, int num, int price):
            RegularSeat(section,row,num,price+mrs_price_addition) {};
    int price() override { return _price; }
    string location() override {
        return "Middle-> "+RegularSeat::location();
    }
    ~MiddleRegularSeat() override = default;
};

// ---------------------------------------------
class RearRegularSeat: public RegularSeat
{
public:
    RearRegularSeat(char section, int row, int num, int price):
            RegularSeat(section,row,num,price) {};
    int price() override { return _price; }
    string location() override {
        return "Rear-> "+RegularSeat::location();
    }
    ~RearRegularSeat() override = default;
};

// ---------------------------------------------
