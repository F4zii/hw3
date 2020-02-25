/**************************************************************************
For questions and clarifications : Please use the forum in Moodle.
**************************************************************************/

#include <iostream>
#include "seat.h"
using std::cout;
using std::endl;


#define TEST(num)  cout << endl << "TEST" << " " << (num) << endl;


int main()
{

TEST("C.1")
//   Seat s1(5, 10, 100); /* line 5, chair 10, base price 100 */
//                        /* complilation error : cannot declare variable 's1' to be of abstract type 'Seat' */

   GreenRoomSeat grs1(2, 10); /* line 2, chair 10. NO base price is relevant for GreenRoomSeat. Only
                                 Special/Distinguished guests will get these seats with No price */
TEST("C.2")
//   GreenRoomSeat grs2(2, 10, 100); /* compilation error because of 3rd parameter. */

TEST("C.3")
   cout << grs1.location() << endl;
   try
   {
     cout << grs1.price() << endl; /* GreenRoomSeat is not for sale. calling price() here
                                    will throw NoPrice exception with text "Not For Sale !". */
   }
   catch (exception& e)
   {
       cout << e.what() << endl;
   }

TEST("C.4")
//   MainHallSeat mhs1(30, 50, 100); /* line 30, chair 50, base price 100 */
//                                  /* compilation error. can not define an object of this type. */

TEST("C.5")
//   RegularSeat rs1('B', 25, 15, 100); /* area B, line 25, chair 15, base price 100 */
//                                      /* compilation error.  can not define an object of this type. */


TEST("C.6")
   FrontRegularSeat frs1('C', 8, 20, 100);  /* area C, line 8, chair 20, base price 100 */
   cout << frs1.location() << endl;
   cout << frs1.price() << endl; /* price is base price + addition of 100 for MainHallSeat +
                                    another addition of 500 for FrontRegularSeat. */

TEST("C.7")
   MiddleRegularSeat mrs1('D', 30, 15, 80); /* area D, line 30, chair 15, base price 80 */
   cout << mrs1.location() << endl;
   cout << mrs1.price() << endl; /* price is base price + addition of 100 for MainHallSeat +
                                    another addition of 250 for MiddleRegularSeat. */

TEST("C.8")
   RearRegularSeat rrs1('E', 60, 18, 70);    /* area E, line 60, chair 18, base price 70 */
   cout << rrs1.location() << endl;
   cout << rrs1.price() << endl; /* price is base price + addition of 100 for MainHallSeat.
                                    No other addition */

TEST("C.9")
//   SpecialSeat sps1(1, 17, 100); /* line 1, chair 17, base price 100 */
//                                  /* compilation error. can not define an object of this type. */

TEST("C.10")
   GoldenCircleSeat gcs1(2, 14, 100);  /* no area for any SpecialSeat, specifically nor for GoldenCircleSeat which counts
                                          as SpecialSeat.
                                          we get here line 2, chair 14, base price 100 */
   cout << gcs1.location() << endl;
   cout << gcs1.price() << endl; /* price is base price + addition of 100 for MainHallSeat +
                                    another addition of 300 for SpecialSeat + another addition of 1000 for GoldenCircleSeat.
                                    Total of 100+100+300+1000 = 1500 */

TEST("C.11")
   DisablePodiumSeat dps1(1, 10, 100);  /* no area for any SpecialSeat, specifically nor for DisablePodiumSeat which counts
                                          as SpecialSeat.
                                          we get here line 1, chair 10, base price 100 (base price is meaningless here) */
   cout << dps1.location() << endl;
   cout << dps1.price() << endl; /* price is fixed 200, with no other additions, nor base price. */

TEST("C.12")
   DisablePodiumSeat dps2(1, 11);
   cout << dps2.location() << endl;
   cout << dps2.price() << endl;

TEST("C.13")
   const int len=6;
   Seat* seats[len] = {new GreenRoomSeat(3,8), new FrontRegularSeat('A',5,8,150), new MiddleRegularSeat('B',25,35,120),
                       new RearRegularSeat('E',70,4,90), new GoldenCircleSeat(2,19,200),
                       new DisablePodiumSeat(1,5)};

   for (int i=0; i<len; ++i)
   {
      cout << seats[i]->location() << endl;
      try
      {
          cout << seats[i]->price() << endl;
      }
      catch (exception& e)
      {
          cout << e.what() << endl;
      }
   }

   for (Seat** p=seats; p<seats+len; ++p) delete *p;

   return 0;
}
