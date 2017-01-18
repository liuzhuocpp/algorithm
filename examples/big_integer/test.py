a = [9,3,7,3,8,7,4,8,2,0,5,1,9,9,2,9,8,8,8,3,0,1,5,5,9,8,9,6,8,8,0,7,4,0,3,2,0,1,2,9,3,0,9,3,5,3,3,6,5,4,1,4,6,1]
b = [2,8,8,7,2,7,3,5,6,8,6,2,6,7,3,4,8,4,1,4,8,8,9,9,6,8,6,4,4,0,8,9,8,2,3,2,0,4,3]
q = [9,7,9,1,9,0,9,9,7,0,5,4,2,8,4]
r = [1,6,2,5,2,9,9,2,3,1,7,1,5,8,4,3,7,7,7,1,5,1,8,2,6,4,8,9,0,4,2,3,2,5,0,2,8,4,1]

def convertLittleEndianListToNumber(a):
    numa = 0
    for x in xrange(len(a) - 1, -1, -1):
        numa = numa * 10 + a[x]
    return numa

a =  convertLittleEndianListToNumber(a)
b =  convertLittleEndianListToNumber(b)
q =  convertLittleEndianListToNumber(q)
r =  convertLittleEndianListToNumber(r)


print a / b
print a % b

print a / b == q
print a % b == r

