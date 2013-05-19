/*
 * jump - trivial prime jump table
 *
 * Copyright (C) 1999  Landon Curt Noll
 *
 * Calc is open software; you can redistribute it and/or modify it under
 * the terms of the version 2.1 of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * Calc is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU Lesser General
 * Public License for more details.
 *
 * A copy of version 2.1 of the GNU Lesser General Public License is
 * distributed with calc under the filename COPYING-LGPL.  You should have
 * received a copy with calc; if not, write to Free Software Foundation, Inc.
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @(#) $Revision: 30.1 $
 * @(#) $Id: jump.c,v 30.1 2007/03/16 11:09:46 chongo Exp $
 * @(#) $Source: /usr/local/src/cmd/calc/RCS/jump.c,v $
 *
 * Under source code control:	1994/06/29 04:03:54
 * File existed as early as:	1994
 *
 * chongo <was here> /\oo/\	http://www.isthe.com/chongo/
 * Share and enjoy!  :-)	http://www.isthe.com/chongo/tech/comp/calc/
 */

/*
 * If x is divisible by a trivial prime (2,3,5,7,11), then:
 *
 *		x + jmpindx[ (x>>1)%JMPMOD ]
 *
 * is the value of the smallest value > x that is not divisible by a
 * trivial prime.  JMPMOD is the product of the odd trivial primes.
 *
 * This table is useful for skipping values that are obviously not prime
 * by skipping values that are a multiple of trivial prime.
 *
 * If x is not divisible by a trivial prime, then:
 *
 *		x + jmp[ -jmpindx[(x>>1)%JMPMOD] ]
 *
 * is the value of the smallest value > x that is not divisible by a
 * trivial prime.
 *
 * Instead of testing successive odd values, this system allows us to
 * skip odd values divisible by trivial primes.	 This is process on the
 * average reduces the values we need to test by a factor of at least 2.4.
 */


#include "jump.h"

/*
 * jmpindx - how to find the next value not divisible by a trivial prime
 *
 * If jmpindx[y] > 0 (y = x mod JMPMOD*2), then it refers to an 'x' that
 * is divisible by a trivial prime and jmpindx[y] is the offset to the next
 * value that is not divisible.
 *
 * If jmpindx[y] <= 0, then 'x' is not divisible by a trivial prime and
 * the negative of jmpindx[y] is the index into the jmp[] table.  We use
 * successive values from jmp[] (wrapping around to the beginning when
 * we move off the end of jmp[]) to move to higher and higher values
 * that are not divisible by trivial primes.
 */
CONST short jmpindx[JMPMOD] = {
    0, 10, 8, 6, 4, 2, -1, 2, -2, -3, 2, -4, 4, 2, -5, -6, 4, 2, -7, 2, -8, -9,
    2, -10, 4, 2, -11, 4, 2, -12, -13, 4, 2, -14, 2, -15, -16, 4, 2, -17, 2,
    -18, 4, 2, -19, 6, 4, 2, -20, 2, -21, -22, 2, -23, -24, 2, -25, 12, 10,
    8, 6, 4, 2, -26, 2, -27, 4, 2, -28, -29, 8, 6, 4, 2, -30, -31, 4, 2, -32,
    4, 2, -33, 2, -34, -35, 2, -36, 4, 2, -37, -38, 8, 6, 4, 2, -39, -40, 2,
    -41, -42, 10, 8, 6, 4, 2, -43, 8, 6, 4, 2, -44, -45, 2, -46, -47, 2, -48,
    4, 2, -49, -50, 4, 2, -51, 2, -52, 4, 2, -53, 4, 2, -54, 4, 2, -55, -56,
    4, 2, -57, 2, -58, -59, 4, 2, -60, 2, -61, 4, 2, -62, 6, 4, 2, -63, 2,
    -64, -65, 2, -66, 4, 2, -67, 6, 4, 2, -68, 4, 2, -69, 8, 6, 4, 2, -70,
    -71, 2, -72, 4, 2, -73, -74, 4, 2, -75, 4, 2, -76, 2, -77, -78, 2, -79,
    4, 2, -80, -81, 4, 2, -82, 2, -83, -84, 4, 2, -85, 8, 6, 4, 2, -86, -87,
    8, 6, 4, 2, -88, -89, 2, -90, -91, 2, -92, 4, 2, -93, 6, 4, 2, -94, 2,
    -95, -96, 2, -97, 10, 8, 6, 4, 2, -98, -99, 4, 2, -100, 2, -101, -102, 4,
    2, -103, 2, -104, 4, 2, -105, 10, 8, 6, 4, 2, -106, -107, 2, -108, -109,
    2, -110, 6, 4, 2, -111, 4, 2, -112, 2, -113, 4, 2, -114, -115, 2, -116,
    4, 2, -117, -118, 4, 2, -119, 8, 6, 4, 2, -120, -121, 2, -122, 4, 2, -123,
    -124, 4, 2, -125, 2, -126, -127, 2, -128, -129, 8, 6, 4, 2, -130, -131,
    8, 6, 4, 2, -132, -133, 2, -134, 4, 2, -135, 4, 2, -136, -137, 4, 2, -138,
    4, 2, -139, 2, -140, 4, 2, -141, 4, 2, -142, -143, 4, 2, -144, 2, -145,
    -146, 4, 2, -147, 2, -148, 4, 2, -149, 6, 4, 2, -150, 2, -151, -152, 4,
    2, -153, 2, -154, 6, 4, 2, -155, 4, 2, -156, 2, -157, 4, 2, -158, -159,
    2, -160, 4, 2, -161, 6, 4, 2, -162, 4, 2, -163, 2, -164, -165, 8, 6, 4,
    2, -166, -167, 4, 2, -168, 2, -169, -170, 2, -171, -172, 8, 6, 4, 2, -173,
    -174, 8, 6, 4, 2, -175, -176, 2, -177, -178, 2, -179, 6, 4, 2, -180, 4,
    2, -181, 2, -182, -183, 2, -184, 4, 2, -185, 4, 2, -186, -187, 4, 2, -188,
    2, -189, 6, 4, 2, -190, 2, -191, 4, 2, -192, 6, 4, 2, -193, 2, -194, -195,
    2, -196, -197, 2, -198, 6, 4, 2, -199, 4, 2, -200, 2, -201, 4, 2, -202,
    4, 2, -203, 4, 2, -204, -205, 4, 2, -206, 4, 2, -207, 2, -208, -209, 2,
    -210, 4, 2, -211, -212, 4, 2, -213, 2, -214, -215, 2, -216, -217, 8, 6,
    4, 2, -218, -219, 8, 6, 4, 2, -220, -221, 4, 2, -222, 2, -223, 4, 2, -224,
    -225, 4, 2, -226, 2, -227, -228, 2, -229, 4, 2, -230, 4, 2, -231, 6, 4,
    2, -232, 2, -233, -234, 4, 2, -235, 8, 6, 4, 2, -236, 6, 4, 2, -237, 2,
    -238, -239, 2, -240, -241, 2, -242, 6, 4, 2, -243, 8, 6, 4, 2, -244, 4,
    2, -245, -246, 2, -247, 6, 4, 2, -248, 4, 2, -249, 4, 2, -250, 2, -251,
    -252, 2, -253, 4, 2, -254, -255, 4, 2, -256, 2, -257, 4, 2, -258, -259,
    8, 6, 4, 2, -260, -261, 8, 6, 4, 2, -262, -263, 2, -264, -265, 2, -266,
    4, 2, -267, -268, 4, 2, -269, 2, -270, -271, 2, -272, 4, 2, -273, 4, 2,
    -274, -275, 4, 2, -276, 4, 2, -277, 4, 2, -278, 2, -279, 4, 2, -280, 6,
    4, 2, -281, 2, -282, -283, 2, -284, -285, 2, -286, 6, 4, 2, -287, 4, 2,
    -288, 2, -289, 6, 4, 2, -290, 2, -291, 4, 2, -292, -293, 4, 2, -294, 4,
    2, -295, 2, -296, -297, 2, -298, 4, 2, -299, 6, 4, 2, -300, 2, -301, -302,
    2, -303, -304, 8, 6, 4, 2, -305, -306, 8, 6, 4, 2, -307, -308, 2, -309,
    -310, 2, -311, 4, 2, -312, -313, 8, 6, 4, 2, -314, -315, 2, -316, 4, 2,
    -317, 6, 4, 2, -318, 4, 2, -319, 2, -320, -321, 4, 2, -322, 2, -323, 4,
    2, -324, 6, 4, 2, -325, 2, -326, 4, 2, -327, -328, 2, -329, 6, 4, 2, -330,
    4, 2, -331, 2, -332, 4, 2, -333, -334, 2, -335, 4, 2, -336, -337, 4, 2,
    -338, 4, 2, -339, 2, -340, 4, 2, -341, 4, 2, -342, -343, 4, 2, -344, 4,
    2, -345, 2, -346, -347, 8, 6, 4, 2, -348, -349, 8, 6, 4, 2, -350, -351,
    2, -352, -353, 2, -354, 4, 2, -355, -356, 4, 2, -357, 2, -358, -359, 8,
    6, 4, 2, -360, 4, 2, -361, -362, 4, 2, -363, 2, -364, -365, 4, 2, -366,
    2, -367, 4, 2, -368, 6, 4, 2, -369, 2, -370, -371, 2, -372, -373, 10, 8,
    6, 4, 2, -374, 4, 2, -375, 2, -376, 4, 2, -377, -378, 2, -379, 4, 2, -380,
    -381, 10, 8, 6, 4, 2, -382, 2, -383, -384, 2, -385, 6, 4, 2, -386, 4,
    2, -387, 2, -388, -389, 2, -390, -391, 8, 6, 4, 2, -392, -393, 8, 6, 4,
    2, -394, 4, 2, -395, -396, 2, -397, 4, 2, -398, -399, 4, 2, -400, 2, -401,
    -402, 2, -403, 4, 2, -404, 4, 2, -405, -406, 4, 2, -407, 2, -408, -409,
    8, 6, 4, 2, -410, 4, 2, -411, 6, 4, 2, -412, 4, 2, -413, 2, -414, -415,
    2, -416, 6, 4, 2, -417, 4, 2, -418, 2, -419, 4, 2, -420, -421, 2, -422,
    4, 2, -423, -424, 4, 2, -425, 4, 2, -426, 4, 2, -427, 2, -428, 4, 2, -429,
    -430, 4, 2, -431, 2, -432, -433, 2, -434, -435, 8, 6, 4, 2, -436, 10, 8,
    6, 4, 2, -437, -438, 2, -439, -440, 8, 6, 4, 2, -441, -442, 4, 2, -443,
    2, -444, -445, 2, -446, 4, 2, -447, 4, 2, -448, -449, 8, 6, 4, 2, -450,
    -451, 4, 2, -452, 2, -453, 12, 10, 8, 6, 4, 2, -454, 2, -455, -456, 2,
    -457, -458, 2, -459, 6, 4, 2, -460, 4, 2, -461, 2, -462, 4, 2, -463, -464,
    2, -465, 4, 2, -466, -467, 4, 2, -468, 4, 2, -469, 2, -470, -471, 2, -472,
    4, 2, -473, -474, 4, 2, -475, 2, -476, -477, 2, -478, 10, 8, 6, 4, 2, -479
};

/*
 * jmp - intervals between successive integers not divisible by trivial primes
 */
CONST unsigned char jmp[JMPSIZE] = {
    12, 4, 2, 4, 6, 2, 6, 4, 2, 4, 6, 6, 2, 6, 4, 2, 6, 4, 6, 8, 4, 2, 4,
    2, 4, 14, 4, 6, 2, 10, 2, 6, 6, 4, 2, 4, 6, 2, 10, 2, 4, 2, 12, 10, 2,
    4, 2, 4, 6, 2, 6, 4, 6, 6, 6, 2, 6, 4, 2, 6, 4, 6, 8, 4, 2, 4, 6, 8, 6,
    10, 2, 4, 6, 2, 6, 6, 4, 2, 4, 6, 2, 6, 4, 2, 6, 10, 2, 10, 2, 4, 2, 4,
    6, 8, 4, 2, 4, 12, 2, 6, 4, 2, 6, 4, 6, 12, 2, 4, 2, 4, 8, 6, 4, 6, 2,
    4, 6, 2, 6, 10, 2, 4, 6, 2, 6, 4, 2, 4, 2, 10, 2, 10, 2, 4, 6, 6, 2, 6,
    6, 4, 6, 6, 2, 6, 4, 2, 6, 4, 6, 8, 4, 2, 6, 4, 8, 6, 4, 6, 2, 4, 6, 8,
    6, 4, 2, 10, 2, 6, 4, 2, 4, 2, 10, 2, 10, 2, 4, 2, 4, 8, 6, 4, 2, 4, 6,
    6, 2, 6, 4, 8, 4, 6, 8, 4, 2, 4, 2, 4, 8, 6, 4, 6, 6, 6, 2, 6, 6, 4, 2,
    4, 6, 2, 6, 4, 2, 4, 2, 10, 2, 10, 2, 6, 4, 6, 2, 6, 4, 2, 4, 6, 6, 8,
    4, 2, 6, 10, 8, 4, 2, 4, 2, 4, 8, 10, 6, 2, 4, 8, 6, 6, 4, 2, 4, 6, 2,
    6, 4, 6, 2, 10, 2, 10, 2, 4, 2, 4, 6, 2, 6, 4, 2, 4, 6, 6, 2, 6, 6, 6,
    4, 6, 8, 4, 2, 4, 2, 4, 8, 6, 4, 8, 4, 6, 2, 6, 6, 4, 2, 4, 6, 8, 4, 2,
    4, 2, 10, 2, 10, 2, 4, 2, 4, 6, 2, 10, 2, 4, 6, 8, 6, 4, 2, 6, 4, 6, 8,
    4, 6, 2, 4, 8, 6, 4, 6, 2, 4, 6, 2, 6, 6, 4, 6, 6, 2, 6, 6, 4, 2, 10, 2,
    10, 2, 4, 2, 4, 6, 2, 6, 4, 2, 10, 6, 2, 6, 4, 2, 6, 4, 6, 8, 4, 2, 4,
    2, 12, 6, 4, 6, 2, 4, 6, 2, 12, 4, 2, 4, 8, 6, 4, 2, 4, 2, 10, 2, 10, 6,
    2, 4, 6, 2, 6, 4, 2, 4, 6, 6, 2, 6, 4, 2, 10, 6, 8, 6, 4, 2, 4, 8, 6,
    4, 6, 2, 4, 6, 2, 6, 6, 6, 4, 6, 2, 6, 4, 2, 4, 2, 10, 12, 2, 4, 2, 10,
    2, 6, 4, 2, 4, 6, 6, 2, 10, 2, 6, 4, 14, 4, 2, 4, 2, 4, 8, 6, 4, 6, 2,
    4, 6, 2, 6, 6, 4, 2, 4, 6, 2, 6, 4, 2, 4, 12, 2
};
CONST unsigned char *CONST lastjmp = (jmp+JMPSIZE-1);
