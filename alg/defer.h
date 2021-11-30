/*
MIT License

Copyright (c) 2021 Mikel Irazabal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef DEFER_H
#define DEFER_H

/*
 * Defer mechanism taken from http://www.open-std.org/jtc1/sc22/wg14/www/docs/n2542.pdf
 * It may become part of the C2X standard in <stddefer.h>
 */



#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define UNIQUE_DF_FUNC TOKENPASTE2(DF_ , __COUNTER__) 
#define UNIQUE_FUNC_IMPL TOKENPASTE2( DF_impl_ , __LINE__ ) 



#if defined __clang__  // requires -fblocks (lambdas)
void cleanup_deferred (void (^*d) (void));
#define defer(...) \
__attribute__((__cleanup__ (cleanup_deferred))) \
__attribute__((unused)) void (^UNIQUE_DF_FUNC) (void) = ^__VA_ARGS__ 
/*
__attribute__((__cleanup__(cleanup_deferred))) \
    void (^DF_##__LINE__##__FILE__##__func__) (void) = ^__VA_ARGS__
*/
#elif defined __GNUC__ // nested-function-in-stmt-expressionstatic
void cleanup_deferred (void (**d) (void));
#define defer(...) \
__attribute__((__cleanup__ (cleanup_deferred))) \
void (*UNIQUE_DF_FUNC) (void) = ({               \
void UNIQUE_FUNC_IMPL (void) __VA_ARGS__ UNIQUE_FUNC_IMPL; })
#endif

/*
__attribute__((__cleanup__ (cleanup_deferred))) \
void (*DF_##__LINE__##_func_##__func__##_file_##__FILE__) (void) = ({               \
void DF_##__LINE__##_func_##__func__##_file_##__FILE__##__impl (void) __VA_ARGS__    \
DF_##__LINE__##_func_##__func__##_file_##__FILE__##__impl; })
*/

#endif

