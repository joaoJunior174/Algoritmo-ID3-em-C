import sys
import random


flores=[]
with open("iris.data") as myfile:
    
    for f in myfile:
       conjunto = f.split(',')
       flores.append(conjunto)

del flores[-1]
random.shuffle(flores)
f=open("iris.txt", "a+")

for i in range(0,len(flores),1):
  for j in range(0,len(flores[i]),1):
     f.write("%s "% flores[i][j])

f.close()
