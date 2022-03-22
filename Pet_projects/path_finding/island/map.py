#!/usr/bin/env python3
#coding: utf-8

import fileinput
import matplotlib.patches
import matplotlib.pyplot as plt

#######################################################

def drawGrid():
    plt.xlim(0, 13)
    plt.ylim(-8, 1)
    plt.grid()

def defineAxes():
    axes = plt.gca()
    axes.set_aspect("equal")
    return axes

def makeStep(axes, begPoint, endPoint):
    x0, y0 = begPoint
    xn, yn = endPoint

    arrow_dx = xn - x0
    arrow_dy = yn - y0

    arrow = matplotlib.patches.Arrow(x0, y0, arrow_dx, arrow_dy, width=0.2, color="g")
    axes.add_patch(arrow)

#######################################################

lst = []
for line in fileinput.input():
   lst += line.split()

#print(lst)

drawGrid()
axes = defineAxes()

# lst_ = []

# for index in range(len(lst)):
# 	if index < len(lst) - 1:
# 		y, x = lst[index].split(',')
# 		lst_.append([int(x), int(y)])

# lst_ = sorted(lst_)
# print(lst_)

# for index in range(len(lst_)):
# 	if index < len(lst_) - 1:
# 		xbeg, ybeg = lst_[index]
# 		xend, yend = lst_[index + 1]
# 		makeStep(axes, [xbeg, -ybeg], [xend, -yend])

for index in range(len(lst)):
	if index < len(lst) - 1:
		ybeg, xbeg = lst[index].split(',')
		yend, xend = lst[index + 1].split(',')
		#print(xbeg, ybeg)
		#print(xend, yend)
		makeStep(axes, [int(xbeg), -int(ybeg)], [int(xend), -int(yend)])

plt.show()
