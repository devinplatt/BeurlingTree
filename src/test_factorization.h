/*
 * test_factorization.h
 *
 *  Created on: Nov 5, 2013
 *      Author: Devin
 *
 *  A test for the Factorization class.
 */

#ifndef TEST_FACTORIZATION_H_
#define TEST_FACTORIZATION_H_

#include "factorization.h"
#include "compatibility.h"
#include "test_utils.h"
#include<string>
using std::string;

namespace Platt {

bool TestFactorization(string* error) {
  bool pass = true;
  *error = "";

  Factorization f0;             // {(0,0)}
  Factorization f1(0);          // {(0,1)}
  Factorization f01(1);         // {(1,1)}
  Factorization f11 = f1+f01;   // {(0,1),(1,1)}
  Factorization fcopy(f11);     // {(0,1),(1,1)}
  Factorization fSerial(f11.ToSerialString());
  Factorization f2 = f1+f1;
  Factorization f3 = f2+f1;
  Factorization f4 = f2+f2;
  Factorization f001(2);
  Factorization f31 = f3 + f01;
  Factorization f211 = f2+f01+f001;
  Factorization f221 = f211 + f01;

  // Test construction, ToDotString()
  EXPECT_EQ(f0.ToDotString(), (string)"{(0,0)}", &pass, error, "Default");
  EXPECT_EQ(f1.ToDotString(), (string)"{(0,1)}", &pass, error, "First Prime");
  EXPECT_EQ(f01.ToDotString(), (string)"{(1,1)}", &pass, error, "Second Prime");
  EXPECT_EQ(f11.ToDotString(), (string)"{(0,1),(1,1)}", &pass, error,
           "First prime plus second");
  EXPECT_EQ(fcopy.ToDotString(), (string)"{(0,1),(1,1)}", &pass, error,
           "Copy of First prime plus second");

  // Test ToSerialString, construction from serial string
  EXPECT_EQ(f11.ToSerialString(), (string)"0,1|1,1", &pass, error,
            "Serialization of first prime plus second");
  EXPECT_EQ(fSerial.ToDotString(), (string)"{(0,1),(1,1)}", &pass, error,
              "Serialization of construction by serialization of first prime plus second");

  // Test IsPrime()
  EXPECT_TRUE(f1.IsPrime(), &pass, error,
              "The first prime is falsely mistaken as composite.");
  EXPECT_FALSE(f11.IsPrime(), &pass, error,
              f11.ToDotString() + " is falsely mistaken as prime.");

  // Test the lexicographical ordering
  EXPECT_TRUE(Factorization::LexicographicallyLess(f0, f1), &pass, error,
              "First prime should be lexicographically greater than identity.");
  EXPECT_TRUE(Factorization::LexicographicallyLess(f0, f01), &pass, error,
              "Second prime should be lexicographically greater than identity.");
  EXPECT_TRUE(Factorization::LexicographicallyLess(f01, f1), &pass, error,
              "First prime should be lexicographically greater than second prime.");
  EXPECT_TRUE(Factorization::LexicographicallyLess(f1, f2), &pass, error,
              "First prime squared should be lexicographically greater than first prime.");
  EXPECT_TRUE(Factorization::LexicographicallyLess(f01, f11), &pass, error,
              "First prime times second prime should be lexicographically greater than second prime.");
  EXPECT_TRUE(Factorization::LexicographicallyLess(f211, f221), &pass, error,
              "<2,2,1> should be lexicographically greater than <2,1,1>.");

  // Test RequiredCount()
  EXPECT_EQ(f0.RequiredCount(), (unsigned int)0, &pass, error,
            "Required count for identity");
  EXPECT_EQ(f1.RequiredCount(), (unsigned int)0, &pass, error,
              "Required count for first prime");
  EXPECT_EQ(f01.RequiredCount(), (unsigned int)0, &pass, error,
                "Required count for second prime");
  EXPECT_EQ(f001.RequiredCount(), (unsigned int)0, &pass, error,
                  "Required count for third prime");
  EXPECT_EQ(f2.RequiredCount(), (unsigned int)1, &pass, error,
                "Required count for first prime squared");
  EXPECT_EQ(f3.RequiredCount(), (unsigned int)1, &pass, error,
                "Required count for first prime cubed");
  EXPECT_EQ(f4.RequiredCount(), (unsigned int)2, &pass, error,
                "Required count for first prime to the fourth power");
  EXPECT_EQ(f11.RequiredCount(), (unsigned int)1, &pass, error,
                  "Required count for " + f11.ToDotString());
  EXPECT_EQ(f31.RequiredCount(), (unsigned int)3, &pass, error,
                  "Required count for " + f31.ToDotString());
  EXPECT_EQ(f211.RequiredCount(), (unsigned int)5, &pass, error,
                  "Required count for " + f211.ToDotString());
  EXPECT_EQ(f221.RequiredCount(), (unsigned int)8, &pass, error,
                  "Required count for " + f221.ToDotString());

  EXPECT_TRUE((f221 == f221), &pass, error,
              "<2,2,1> should be equal to <2,2,1>.");
  EXPECT_FALSE((f221 != f221), &pass, error,
               "<2,2,1> should not be not equal to <2,2,1>.");
  EXPECT_FALSE((f211 == f221), &pass, error,
              "<2,1,1> should not be equal to <2,2,1>.");
  EXPECT_TRUE((f211 != f221), &pass, error,
              "<2,1,1> should be not equal to <2,1,1>.");

  EXPECT_TRUE((Tuple(1,2) < Tuple(1,3)), &pass, error,
                "(1,2) should be lexicographically less than (1,3).");
  EXPECT_TRUE((Tuple(1,3) < Tuple(2,1)), &pass, error,
                  "(1,3) should be lexicographically less than (2,1).");
  EXPECT_FALSE((Tuple(2,1) < Tuple(2,1)), &pass, error,
                    "(2,1) should not be lexicographically less than (2,1).");
  EXPECT_FALSE((Tuple(1,3) < Tuple(1,2)), &pass, error,
                  "(1,3) should not be lexicographically less than (1,2).");
  EXPECT_FALSE((Tuple(2,1) < Tuple(1,3)), &pass, error,
                  "(2,1) should not be lexicographically less than (1,3).");

  return pass;
}

}  // namespace Platt

#endif  /* TEST_FACTORIZATION_H_ */
