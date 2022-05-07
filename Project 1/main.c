int add(int a, int b) {
int x;
x = a + b;
return x;
}
int mul(int a, int b) {
return a * b;
}
int main(int argc, char **argv) {
int a, b, c;
int * ret;
a = 10;
b = 10;
c = 5;
*ret = mul(a, add(b, c));
}
