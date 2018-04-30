#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

obj = open(sys.argv[1], "rb")
wrl = open(sys.argv[2], "w")

points = []
faces = []

#Leer OBJ y guardar vértices y caras

for bline in obj:
	try:
		line = bline.decode()
		if line[0:2] == "v ":
			words = line[2:].split()
			points.append([float(words[0]), float(words[1]), float(words[2])])
		elif line[0:2] == "f ":
			words = line[2:].split()
			pointsface = []
			for w in words:
				pointsface.append(int(w.split('/')[0]))
			faces.append(pointsface)
	except UnicodeDecodeError:
		pass

obj.close()

wrl.write("""\
#VRML V2.0 utf8 
Shape {
	appearance Appearance { material Material { diffuseColor 1 1 1 } }
	geometry IndexedFaceSet {
		solid FALSE
		coord Coordinate {
			point [
""")

#Escribir puntos
for p in points:
	wrl.write("\t\t\t\t{} {} {},\n".format(p[0], p[1], p[2]))

wrl.write("""\
			]
		}
		coordIndex [
""")

#Escribir caras
for f in faces:
	wrl.write("\t\t\t")
	for p in f:
		wrl.write("{}, ".format(p-1))
	wrl.write("-1,\n")

wrl.write("""\
		]
		colorPerVertex FALSE
		color Color {
			color [
""")

#Asignar colores a las caras siguiendo el espectro cromático
#(Si el orden de las caras en el obj origen es aleatorio, no
#se apreciará ningún patrón en los colores)
step = 0.01
RGB = [1, 0, 0]
i = 0
for f in faces:
	wrl.write("\t\t\t\t{} {} {},\n".format(RGB[0], RGB[1], RGB[2]))
	if RGB[i] == 0:
		i = (i+1)%3
	RGB[i] = round(RGB[i]-step, 2)
	RGB[(i+1)%3] += step

wrl.write("""\
			]
		}
	}
}
""")

wrl.close()

print ("Convertidos " + str(len(points)) + " vértices y " + str(len(faces)) + " superficies.")