

a = 0x80000000ffffffffffffffffffffffff
b = 0xffffffffffffffff

q = a / b
r = a - b * q


print hex(q)
print hex(r)


ta = 0x80000000ffffffffffffffff
tb = 0xffffffffffffffff
tq = ta / tb
tr = ta - tb * tq

print hex(tq)
print hex(tr)
