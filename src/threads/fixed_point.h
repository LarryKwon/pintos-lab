#define F (1<<14)
#define INT_MAX((1<<31) -1)
#define INT_MIN(-(1<<31))

int int_to_fp(int n); /* integer에서 fixed point로*/
int fp_to_int_round(int x); /* fixed point에서 int로, 반올림 */
int fp_to_int(int x); /* fixed point에서 int로, 버림*/
int add_fp(int x, int y); /* fp 끼리 더하기 */
int add_mixed(int x, int n); /* int와 fp끼리 더하기 */
int sub_fp(int x, int y); /* FP끼리 빼기 */
int sub_mixed(int x, int n); /* FP에서 int 빼기 */
int mult_fp(int x, int y); /* fp끼리 곱셈 */
int mult_mixed(int x, int n); /* FP에 int 곱셈 */
int div_fp(int x, int y); /* FP끼리 int 나누기(x/y) */
int div_mixed(int x, int n); /* FP를 int로 나누기(x/n) */

int int_to_fp(int n){
    return n * F;
}

int fp_to_int_round(int x){
    return x>=0 ? (x+F/2)/F : (x-F/2)/F;
}

int fp_to_int(int x){
    return n/F;
}

int add_fp(int x, int y){
    return x+y;
}

int add_mixed(int x, int n){
    return x + n*F;
}

int sub_fp(int x, int y){
    return x- y;
}

int sub_mixed(int x, int n){
    return x - n*F;
}

int mult_fp(int x, int y){
    return ((int64_t) x) * y / F;
}

int mult_mixed(int x, int n){
    return x * n;
}

int div_fp(int x, int y){
    return ((int64_t)x) * F / y;
}

int div_mixed(int x, int n){
    return x/n;
}