from z3 import *

x = Real('x')
y = Real('y')
s = Solver()
s.add(x + y > 5, x > 1, y > 1)
s.add(x <2)
s.add(Or(x<2,x<2) )
print(s.check())
print(s.model())
