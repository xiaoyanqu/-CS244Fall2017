import math

output_1 = open("data_standing", "w")
output_2 = open("data_sitting", "w")
output_3 = open("data_sleeping", "w")
output_4 = open("data_walking", "w")

with open('standing') as f:
    for line in f:
        nums = line.split()
        X = float(nums[3])
        Y = float(nums[4])
        Z = float(nums[5])

        output_1.write(str(math.sqrt(X * X + Y * Y + Z * Z)) + " 1" + '\n')

with open('sitting') as f:
    for line in f:
        nums = line.split()
        X = float(nums[3])
        Y = float(nums[4])
        Z = float(nums[5])

        output_2.write(str(math.sqrt(X * X + Y * Y + Z * Z)) + " 1" + '\n')

with open('sleeping') as f:
    for line in f:
        nums = line.split()
        X = float(nums[3])
        Y = float(nums[4])
        Z = float(nums[5])

        output_3.write(str(math.sqrt(X * X + Y * Y + Z * Z)) + " 1" + '\n')

with open('walking') as f:
    for line in f:
        nums = line.split()
        X = float(nums[3])
        Y = float(nums[4])
        Z = float(nums[5])

        output_4.write(str(math.sqrt(X * X + Y * Y + Z * Z)) + " 1" + '\n')

output_1.close()
output_2.close()
output_3.close()
output_4.close()
