#!/usr/bin/env python3
#coding: utf-8

import fileinput
import matplotlib.patches
import matplotlib.pyplot as plt

#######################################################

def drawPolygon(axes):

    poly_x = ([
        0,
        20,
        17.5,
        19.5,
        10,
        11,
        3.5,
        5,
        0
    ]) 
    poly_y = ([
        0,
        2,
        8,
        12.5,
        10,
        13,
        13,
        5,
        6.5
        ])
    plt.fill(poly_x, poly_y, c='C0')


def drawHoles(axes):

    hole_1x = ([
        15,
        16.5,
        15,
        12.5,
        14.5
        ])

    hole_1y = ([
        3.5,
        3,
        8.5,
        8,
        6
    ])

    hole_2x = ([
        5.5,
        8.5,
        6.5
        ])

    hole_2y = ([
        10,
        10,
        11,
    ])

    hole_3x = ([
        7.5,
        8.5,
        8.5,
        6.5
        ])

    hole_3y = ([
        2.5,
        2.5,
        9,
        9
    ])


    plt.fill(hole_1x, hole_1y, c='C2')
    plt.fill(hole_2x, hole_2y, c='C2')
    plt.fill(hole_3x, hole_3y, c='C2')

def drawGrid():
    plt.xlim(-1, 5)
    plt.ylim(-5, 0)
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

drawPolygon(axes)
drawHoles(axes)

for index in range(len(lst)):
	if index < len(lst) - 1:
		xbeg, ybeg = lst[index].split(',')
		xend, yend = lst[index + 1].split(',')
		#print(xbeg, ybeg)
		#print(xend, yend)
		makeStep(axes, [float(xbeg), -float(ybeg)], [float(xend), -float(yend)])

plt.show()
