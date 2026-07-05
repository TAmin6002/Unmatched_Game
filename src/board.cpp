#include "board.h"
#include "space.h"

#include <iostream>

using namespace std;

Board ::Board()
{
    spaces.resize(32);

    for (int i = 0; i < 32; i++)
        spaces.push_back(Space());

    spaces[0] = Space({&spaces[1], &spaces[2], &spaces[4], &spaces[5]}, {&spaces[1], &spaces[5]});

    spaces[1] = Space({&spaces[0], &spaces[2], &spaces[4], &spaces[5]}, {&spaces[0], &spaces[2]});

    spaces[2] = Space({&spaces[0], &spaces[1], &spaces[4], &spaces[5], &spaces[31], &spaces[30], &spaces[29], &spaces[28]}, {&spaces[1], &spaces[3], &spaces[31]});

    spaces[3] = Space({&spaces[0], &spaces[1], &spaces[2], &spaces[4], &spaces[5], &spaces[6], &spaces[7], &spaces[8], &spaces[9]}, {&spaces[2], &spaces[4], &spaces[6]});

    spaces[4] = Space({&spaces[0], &spaces[1], &spaces[2], &spaces[3], &spaces[5]}, {&spaces[3], &spaces[5]});

    spaces[5] = Space({&spaces[0], &spaces[1], &spaces[2], &spaces[3], &spaces[4]}, {&spaces[0], &spaces[4], &spaces[7]});

    spaces[6] = Space({&spaces[3], &spaces[7], &spaces[8], &spaces[9]}, {&spaces[3], &spaces[7]});

    spaces[7] = Space({&spaces[3], &spaces[6], &spaces[8], &spaces[9]}, {&spaces[5], &spaces[6], &spaces[8]});

    spaces[8] = Space({&spaces[3], &spaces[6], &spaces[7], &spaces[9]}, {&spaces[7], &spaces[9], &spaces[14]});

    spaces[9] = Space({&spaces[3], &spaces[6], &spaces[7], &spaces[8], &spaces[10], &spaces[11], &spaces[12], &spaces[13], &spaces[28]}, {&spaces[8], &spaces[10]});

    spaces[10] = Space({&spaces[9], &spaces[11], &spaces[12], &spaces[13], &spaces[28]}, {&spaces[9], &spaces[11], &spaces[13]});

    spaces[11] = Space({&spaces[9], &spaces[10], &spaces[12], &spaces[13], &spaces[28]}, {&spaces[10], &spaces[12], &spaces[13]});

    spaces[12] = Space({&spaces[9], &spaces[10], &spaces[11], &spaces[13], &spaces[28], &spaces[21], &spaces[22], &spaces[23], &spaces[24]}, {&spaces[11], &spaces[28], &spaces[24], &spaces[23], &spaces[18], &spaces[19]});

    spaces[13] = Space({&spaces[9], &spaces[10], &spaces[11], &spaces[12], &spaces[28], &spaces[8], &spaces[14], &spaces[15], &spaces[16], &spaces[17], &spaces[18], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[10], &spaces[16], &spaces[17], &spaces[18]});

    spaces[14] = Space({&spaces[8], &spaces[15], &spaces[16], &spaces[13], &spaces[17], &spaces[18], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[11], &spaces[8], &spaces[15]});

    spaces[15] = Space({&spaces[8], &spaces[14], &spaces[16], &spaces[13], &spaces[17], &spaces[18], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[14], &spaces[16]});

    spaces[16] = Space({&spaces[8], &spaces[14], &spaces[15], &spaces[13], &spaces[17], &spaces[18], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[15], &spaces[13], &spaces[17]});

    spaces[17] = Space({&spaces[8], &spaces[14], &spaces[15], &spaces[16], &spaces[13], &spaces[18], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[16], &spaces[13], &spaces[18]});

    spaces[18] = Space({&spaces[8], &spaces[14], &spaces[15], &spaces[16], &spaces[13], &spaces[17], &spaces[19], &spaces[20], &spaces[21]}, {&spaces[16], &spaces[13], &spaces[17], &spaces[12], &spaces[19]});

    spaces[19] = Space({&spaces[8], &spaces[14], &spaces[15], &spaces[16], &spaces[13], &spaces[17], &spaces[18], &spaces[20], &spaces[21]}, {&spaces[18], &spaces[12], &spaces[20], &spaces[21]});

    spaces[20] = Space({&spaces[8], &spaces[14], &spaces[15], &spaces[16], &spaces[13], &spaces[17], &spaces[18], &spaces[19], &spaces[21]}, {&spaces[19], &spaces[21]});

    spaces[21] = Space({&spaces[8], &spaces[14], &spaces[15], &spaces[16], &spaces[13], &spaces[17], &spaces[18], &spaces[19], &spaces[20], &spaces[12], &spaces[23], &spaces[22], &spaces[24]}, {&spaces[20], &spaces[22]});

    spaces[22] = Space({&spaces[12], &spaces[23], &spaces[24]}, {&spaces[21], &spaces[23], &spaces[24]});

    spaces[23] = Space({&spaces[12], &spaces[24], &spaces[22]}, {&spaces[12], &spaces[22]});

    spaces[24] = Space({&spaces[12], &spaces[23], &spaces[22]}, {&spaces[12], &spaces[22]});

    spaces[25] = Space({&spaces[26], &spaces[27], &spaces[28]}, {&spaces[24], &spaces[27]});

    spaces[26] = Space({&spaces[25], &spaces[27], &spaces[28]}, {&spaces[27]});

    spaces[27] = Space({&spaces[25], &spaces[26], &spaces[28]}, {&spaces[25], &spaces[26], &spaces[28]});

    spaces[28] = Space({&spaces[25], &spaces[26], &spaces[27], &spaces[29], &spaces[30], &spaces[31], &spaces[2], &spaces[12], &spaces[11], &spaces[10], &spaces[9], &spaces[13]}, {&spaces[25], &spaces[27], &spaces[29], &spaces[30], &spaces[12]});

    spaces[29] = Space({&spaces[2], &spaces[31], &spaces[30], &spaces[28]}, {&spaces[31], &spaces[30], &spaces[28]});

    spaces[30] = Space({&spaces[2], &spaces[31], &spaces[29], &spaces[28]}, {&spaces[31], &spaces[29], &spaces[28]});

    spaces[31] = Space({&spaces[2], &spaces[30], &spaces[29], &spaces[28]}, {&spaces[2], &spaces[30], &spaces[29]});
}

bool Board::is_Adjacent(Space *s1, Space *s2)
{
    for (auto const &p : s1->get_neighbor())
    {
        if (p == s2)
            return true;
    }
    return false;
}