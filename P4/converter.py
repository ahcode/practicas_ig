#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import numpy

#Ecuación punto pendiente de una recta (devuelve y en función de x)
def eqrecta(xypunto, m, x):
	return m*(x-xypunto[0])+xypunto[1]

obj = open(sys.argv[1], "r")
wrl = open(sys.argv[2], "w")

#Formato faces: [nº cara, [lista vertices], nº parte del cuerpo, [lista coordenadas punto medio]]
#formato points: [x, y, z]
points = []
faces = []
colors = [
	[1, 1, 1], #Cuerpo
	[0, 0, 1], #Cabeza
	[1, 0, 0], #Brazo izquierdo
	[0, 1, 0], #Brazo derecho
	[1, 1, 0], #Pierna izquierda
	[0, 1, 1]  #Pierna derecha
]

#Leer OBJ y guardar vértices y caras
iterador_caras = 0
for line in obj:
	if line[0:2] == "v ":
		words = line[2:].split()
		points.append([float(words[0]), float(words[1]), float(words[2])])
	elif line[0:2] == "f ":
		words = line[2:].split()
		pointsface = []
		for w in words:
			pointsface.append(int(w.split('/')[0]) - 1)
		faces.append([iterador_caras, pointsface, 0])
		iterador_caras += 1

#Calcular punto medio caras
for f in faces:
	x = 0; y = 0; z = 0
	for v in f[1]:
		x += points[v][0]
		y += points[v][1]
		z += points[v][2]
	x = x / len(f[1])
	y = y / len(f[1])
	z = z / len(f[1])
	f.append([x, y, z])

#Calcular máximos y mínimos
xmax = max([f[3][0] for f in faces])
xmin = min([f[3][0] for f in faces])
ymax = max([f[3][1] for f in faces])
ymin = min([f[3][1] for f in faces])
zmax = max([f[3][2] for f in faces])
zmin = min([f[3][2] for f in faces])

#Divide el cuarto superior de la figura en 50 franjas horizontales y las
#recorre calculando su profundidad (z) para encontrar el cuello (profundidad mínima)
#Se salta las 3 primeras franjas para evitar confundir la parte superior de la cabeza
faces.sort(key=lambda x: x[3][1], reverse = True)
rangos = list(numpy.linspace(start=ymax, stop=(ymin+ymax)*0.75, num=51))
profcuello = xmax - xmin
itcaras = 0
for i in range(4,51):
	zminrango = zmax
	zmaxrango = zmin
	while faces[itcaras][3][1] >= rangos[i]:
		if faces[itcaras][3][2] < zminrango:
			zminrango = faces[itcaras][3][2]
		if faces[itcaras][3][2] > zmaxrango:
			zmaxrango = faces[itcaras][3][2]
		itcaras += 1
	if zmaxrango - zminrango < profcuello:
		profcuello = zmaxrango - zminrango
		ycuello = rangos[i]

#Se establece una franja vertical en el centro de la figura y se busca la 'y' mínima
#dentro de esa franja la cual corresponderá a la altura de la entrepierna de la figura
xminfranjacentro = xmin + (xmax-xmin) / 2 - (xmax-xmin)*0.005
xmaxfranjacentro = xmin + (xmax-xmin) / 2 + (xmax-xmin)*0.005
xyentrepierna = [xmax, ymax]
for f in faces:
	if f[3][0] > xminfranjacentro and f[3][0] < xmaxfranjacentro:
		if f[3][1] < xyentrepierna[1]:
			xyentrepierna = [f[3][0], f[3][1]]

#Divide la mitad izquierda de la figura en 60 franjas verticales y las recorre
#almacenando en una lista la 'y' mínima de cada franja y su correspondiente 'x'
#Se ignorarán aquellas caras de la mitad inferior de la figura para no confundir
#con las piernas. La 'y' más grande de la lista obtenida corresponde a la 'y' de
#la axila, así como su 'x' correspondiente.
faces.sort(key=lambda x: x[3][0])
rangos = list(numpy.linspace(start=xmin, stop=(xmin+xmax)*0.5, num=61))
yminrangolist = []
itcaras = 0
for i in range(1,61):
	yminrango = [ymax, xmax]
	while faces[itcaras][3][0] <= rangos[i]:
		if faces[itcaras][3][1] <= yminrango[0] and faces[itcaras][3][1] > (ymax-ymin)*0.7:
			yminrango = [faces[itcaras][3][1], faces[itcaras][3][0]] 
		itcaras += 1
	if yminrango[1] != xmax:
		yminrangolist.append(yminrango)
xyaxilaizq = max(yminrangolist, key=lambda x: x[0])
xyaxilaizq = [xyaxilaizq[1], xyaxilaizq[0]]

#Se supone una figura simétrica por lo que la coordenada 'x' de la axila derecha
#es la opuesta de la izquierda.
xyaxilader = [xmax - (xyaxilaizq[0]-xmin), xyaxilaizq[1]]

#Las caras cuya y sea superior a ycuello corresponderán a la cabeza
#Se traza una recta de pendiente 2.2 desde la axila izquierda para separar el brazo izquierdo
#Se hace lo mismo con el brazo derecho pero con pendiente negativa
#Se trazan dos líneas que pasan por el punto de la entrepierna de pendiente 0.7 y -0.7
#Las caras que se encuentra debajo de estas líneas forman parte de los pies, si se encuentran
#en la mitad izquierda del humanoide corresponden a la pierna izquierda y el resto a la pierna derecha
for f in faces:
	if f[3][1] >= ycuello:
		f[2] = 1 #Cabeza
	elif f[3][1] >= eqrecta(xyaxilaizq, 2.2, f[3][0]):
		f[2] = 2 #Brazo IZQ
	elif f[3][1] >= eqrecta(xyaxilader, -2.2, f[3][0]):
		f[2] = 3 #Brazo DER
	elif f[3][1] <= eqrecta(xyentrepierna, 0.7, f[3][0]) or f[3][1] <= eqrecta(xyentrepierna, -0.7, f[3][0]):
		if f[3][0] < xmin + (xmax-xmin)/2:
			f[2] = 4 #Pierna IZQ
		else:
			f[2] = 5 #Pierna DER

faces.sort(key=lambda x: x[0])
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
	for p in f[1]:
		wrl.write("{}, ".format(p))
	wrl.write("-1,\n")

wrl.write("""\
		]
		colorPerVertex FALSE
		color Color {
			color [
""")

#Escribir colores en función de la parte del cuerpo asignada en la lista de caras
for f in faces:
	wrl.write("\t\t\t\t{} {} {},\n".format(colors[f[2]][0], colors[f[2]][1], colors[f[2]][2]))

wrl.write("""\
			]
		}
	}
}
""")

wrl.close()

print ("Convertidos " + str(len(points)) + " vértices y " + str(len(faces)) + " superficies.")