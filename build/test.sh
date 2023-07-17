#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./JCC "$input" > tmp.s
  gcc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

# assert 0 0
# assert 42 42
# assert 41 " 12 + 34 - 5 "
# assert 47 "5+6*7"
# assert 15 "5*(9-6)"
# assert 4 "(3+5)/2"
# assert 4 " ( 3+ 5)/ 2"
# assert 37 "-5+6*+7"
# assert 1 "2 > 1"
# assert 1 "2 != 1"
# assert 1 "2 <= 3"
# assert 1 "2 == 2"
# assert 0 "2 < 1"
# assert 0 "2 >= 3"
# assert 1 "(1+2) >= (1 * 1)"
assert 3 "b=(1+2);"
assert 3 "abc = (1+2);"
assert 3 "abc = 1; abc = (1+2)*1/1;"
assert 3 "a = 1; b = 2; c = a + b;"
assert 3 "a = 1; b = (1+1)*1/1; return a+b;"
# assert 3 "a = 1; b = 2 return a+b;"
assert 3 "a=1; if(a==1) b=3; else b=1;"
assert 3 "a=1; if(a==2) b=1; b=3;"
assert 3 "a=1; while(a<3) a=a+1; if(a==3) b=a; else b=1;"
# assert 3 "a=1; if a==1) b=3; else b=1;"
assert 3 "b=0; for(a=0; a<3; a=a+1) b=b +1; return b;"
assert 3 "a=1; if(a==1){c=1; d = 2; b = c+d;} else{b = 1;} return b;"
assert 3 "a=1; if(a==2){b=1; c=2; d = b+c;} else{if(a>2){d=4;}else{b=1; c=2; d= b*c+1;}} return d;"
assert 3 "a=1; if(a==2){b=1; if(b==1){d=b;}else{c = 2; d=b*c;}} else{if(a>2){d=4;}else{b=1; c=2; d= b*c+1;}} return d;"
echo OK