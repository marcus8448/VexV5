
/*
<https://github.com/rafagafe/base85>

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
  Copyright (c) 2016-2018 Rafa Garcia <rafagarcia77@gmail.com>.
  Permission is hereby  granted, free of charge, to any  person obtaining a copy
  of this software and associated  documentation files (the "Software"), to deal
  in the Software  without restriction, including without  limitation the rights
  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#ifndef BASE85_HPP
#define	BASE85_HPP
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"

#ifdef __GNUC__
#define restrict __restrict__
#else
#define restrict
#endif

#include <cstddef>

namespace base85 {
extern "C" {

/**
 * Convert a binary memory block in a base85 null-terminated string.
 * If the size of the source memory block is not a multiple of four,
 * as many zeros as necessary are added to convert it to a multiple of four.
 * @param dest Destination memory where to put the base85 null-terminated string.
 * @param src Source binary memory block.
 * @param size Size in bytes of source binary memory block.
 * @return A pointer to the null character of the base85 null-terminated string.
 */
char* bintob85(char* restrict dest, void const* restrict src, size_t size);

/**
 * Convert a base85 string to binary format.
 * @param dest Destination memory block.
 * @param src Source base85 string.
 * @return If success a pointer to the next byte in memory block.
 *         Null if string has a bad format.
 */
void* b85tobin(void* restrict dest, char const* restrict src);
}
}
#pragma GCC diagnostic pop
#endif // BASE85_HPP
