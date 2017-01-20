#include <bits/stdc++.h>


#include <lz/debug.h>
#include <lz/big_integer/unsigned_big_integer.h>

using namespace lz;
using namespace std;


using U = UnsignedBigInteger;

bool validPlusMinusMultiplyDivideMod(const U& a, const U& b)
{
    auto q = a / b;
    auto r = a % b;
    bool valid = 1;
    valid &= (b * q + r == a);
    valid &= (b * q == a - r);

    auto tmp1 = q;
    tmp1 *= b;
    tmp1 += r;
    tmp1 -= a;
    valid &= (tmp1 == 0U);

    auto tmp2 = a;
    tmp2 /= b;
    valid &= (q == tmp2);

    auto tmp3 = a;
    tmp3 %= b;
    valid &= (r == tmp3);
    return valid;

}

void testOnePair(const U& a, const U & b)
{
    auto isValid = validPlusMinusMultiplyDivideMod(a, b);
    cout << isValid << endl;
    assert(isValid);
}

void testU()
{
    U a, b;
    a = "234234029834098213132";
    b = "1239042034778080";

    testOnePair(
            "234234029834098213132",
            "234234234234534763675674576474576457");

    testOnePair(
            "234234029834098213132",
            "476");

    testOnePair(
            "2342340298340345634534634634756745764576346345635634698213132",
            "456356345634635635634634547457");

    testOnePair(
            "234234029834576457645764574574098213132",
            "1239045764777474203");

    testOnePair(
            "2342340298340457457698213132",
            "1239042476457457603");

    testOnePair(
            "234234029834098213132",
            "123904203678978976899");


    testOnePair(
            "2342340292342342342342342346376457467868748343460129834790145198472034701234891720347129374981237490781203478234234098213132",
            "123904203678978976899");

    testOnePair(
            "23423402983423423423423423420976856885788568857887856234234788568856785685678567856785678436345634568213132",
            "123904203678978972352345234582390589032856899");

    testOnePair(
                "234234",
                "234234234534609128340912834098");
    testOnePair(
                23423424321231231231234_Ub,
                0X23423424FFF_uB);


    testOnePair(
                234234243234234234234234980890889080823424_Ub,
                0X23423424afffbbbbFFF_uB);

    testOnePair(
                1234125234523452345_Ub,
                000000345345345353452342342_uB);

    testOnePair(
                00000000233453453453576345765736423423423423423423423423424324_Ub,
                0X00023423424FFF_uB);

    testOnePair(
                234234234234234_Ub,
                234234245645645667457457_uB);

    testOnePair(
                23457457457457_Ub,
                745745764576_uB);

    testOnePair(
                568568568523423424324_Ub,
                678568568567856_uB);

    testOnePair(
                567856856856856785678_Ub,
                567856785678568_uB);

    testOnePair(
                5685685678568_Ub,
                5685685678_uB);

    testOnePair(
                23423424324_ub,
                5685678568_UB);





}




int main()
{
    testU();





	return 0;
}
