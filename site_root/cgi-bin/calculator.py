#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cgi
import math
import argparse

# Funciones para las operaciones básicas
def suma(a, b):
    return a + b

def resta(a, b):
    return a - b

def multiplicacion(a, b):
    return a * b

def division(a, b):
    if b == 0:
        return "Error: Divided by zero"
    return a / b

# Procesar la entrada del formulario
form = cgi.FieldStorage()

# Obtener los valores de entrada
num1 = float(form.getfirst("num1", 0))
num2 = float(form.getfirst("num2", 0))
op = form.getfirst("op", "+")
#print (num1, num2, op)
# Realizar la operación según la selección del usuario
if op == "+":
    resultado = suma(num1, num2)
elif op == "-":
    resultado = resta(num1, num2)
elif op == "*":
    resultado = multiplicacion(num1, num2)
elif op == "/":
    resultado = division(num1, num2)
else :
    resultado = suma(num1, num2)
# Imprimir el resultado en formato HTML
#print("Content-Type: text/html")
#print("<html>")
#print("  <head>")
#print("    <title>Calculadora</title>")
#print("  </head>")
#print("  <body>")
print("    <p>" "<b>Resultado</b>" , resultado, "</p>")
#print("  </body>")
#print("</html>")