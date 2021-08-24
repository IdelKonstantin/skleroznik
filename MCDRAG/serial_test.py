import subprocess

def toString(number):
	return (str(number) + " ")

dataToWork = [
	[155.0, 5.65, 3.01, 0.5, 0.58, 0.848, 0.09, 1.02, 3.35, 3],
	[5.7, 5.48, 3.0, 0.5, 1.0, 0.754, 0.0, 1.0, 3.34, 2],
	[55.6, 3.25, 0.967, 0.0, 1.18, 1.63, 0.2, 1.0, 1.76, 3],
	[7.82, 3.94, 2.24, 0.9, 0.5, 0.78, 0.211, 1.0, 0.0, 3],		#sierra 168 gr
	[7.82, 4.01, 2.1, 1.0, 0.57, 0.81, 0.26, 1.0, 0.0, 3],		#scenar 167 gr
]

j = 1
for dataSet in dataToWork:

	print("\nDataset number= " + str(j) + "\nMach    CD0     CDH     CDSF    CDBND   CDBT    CDB     PB/PINF")
	dataArgs = ""
	for i in dataSet:
		dataArgs += toString(i)

	p = subprocess.Popen("./app " + dataArgs, shell = True)
	p.wait()
	j += 1
