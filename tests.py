import os

instances = ['texture-Cremer.max', 'texture-OLD-D103.max', 'texture-Paper1.max', 'texture-Temp.max']
instances = ['./instances/' + x for x in instances]

for instance in instances:
    print("Starting tests for instance", instance)
    os.system("./main " + instance + " S 0 >> out.txt")
    print("Test 1/3 finished")
    os.system("./main " + instance + " S 1 >> out.txt")
    print("Test 2/3 finished")
    os.system("./main " + instance + " K 0 >> out.txt")
    print("Test 3/3 finished")
